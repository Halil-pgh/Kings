#include "pch.h"
#include "Self.h"

#include "Entities/Buildings/Home.h"
#include "Entities/Buildings/Mine.h"
#include "Network/Networker.h"
#include "Network/Packet.h"
#include "Core/Serializator.h"
#include "Core/SceneManager.h"
#include "Core/Application.h"
#include "UI/FontManager.h"
#include "UI/Options.h"

#define WINDOW_SIZE (float)Application::Get()->GetWindowBase().getSize()

Self::Self()
	: m_Speed(100), m_PlayerStats(new PlayerStats()), m_ShopBar(new ShopBar()) {
}

Self::~Self() {
	std::cout << "Self destroyed\n";
}

void Self::OnAttach() {
	SceneManager::GetScene("Game")->GetLayer("UI")->AddEntity(m_PlayerStats);
	SceneManager::GetScene("Game")->GetLayer("UI")->AddEntity(m_ShopBar);
}

void Self::OnDetach() {
	Reset();
	std::cout << "OnDetach called\n";
}

void Self::OnUpdate(float deltaTime) {
	sf::View& camera = SceneManager::GetActiveScene()->GetLayer("Game")->GetView();
	camera.setCenter((sf::Vector2f)sf::Vector2i(m_Rect.getPosition()));

	if (m_Mode == Mode::Walk) {
		m_Text.move(m_Velocity * deltaTime);
		m_Rect.move(m_Velocity * deltaTime);

		// Send position to server
		Packet<sf::Vector2f> packet = {PacketType::Move, m_Id, m_Rect.getPosition()};
		size_t size;
		void* buffer = Serializator::serialize(packet, size);
		m_Client->send(buffer, size);
		free(buffer);
	}

	// Updating money $$$
	m_MoneyTime += deltaTime;
	if (m_MoneyTime >= 1) {
		m_Money += m_MoneyPerSecond;
		m_PlayerStats->SetMoney(m_Money);
		m_MoneyTime = 0;
	}

	if (m_Client->isConnected()) {
		m_Client->update();
	}

	// Update players
	for (const auto& [_, player] : m_Players) {
		player->OnUpdate(deltaTime);
	}
}

void Self::OnDraw(sf::RenderWindow &window) {
	window.clear(sf::Color(82, 115, 6));

	window.draw(m_Rect);
	window.draw(m_Text);

	for (const auto& [_, player] : m_Players) {
		player->OnDraw(window);
	}

	for (auto building : m_Buildings) {
		building->OnDraw(window);
	}

	if (m_Mode == Mode::Build) {
		m_ProductionBuilding->SetProductable(CheckBuildingsForProduction());
		m_ProductionBuilding->OnDraw(window);
	}
}

bool Self::OnEvent(const sf::Event& event) {
	if (!m_Client->isConnected()) {
		SceneManager::SetActiveScene("Main");
		Application::Get()->DestroyGameScene();
		return true;
	}

	switch (event.type) {
		case sf::Event::KeyPressed: {
			sf::Vector2f mousePos = Application::GetMousePosition(SceneManager::GetActiveScene()->GetLayer("Game")->GetView());
			switch (event.key.code) {
				case sf::Keyboard::Q: {
					m_Mode = Mode::Build;
					m_ProductionBuilding = std::make_unique<Home>(mousePos.x, mousePos.y);
					m_ProductionBuilding->SetProduction(true);
					return true;
				}
				case sf::Keyboard::W: {
					m_Mode = Mode::Build;
					m_ProductionBuilding = std::make_unique<Mine>(mousePos.x, mousePos.y);
					m_ProductionBuilding->SetProduction(true);
					return true;
				}
				case sf::Keyboard::Escape: {
					if (m_Mode == Mode::Build) {
						m_Mode = Mode::Walk;
					}
					else {
						m_Mode = Mode::Settings;
						SceneManager::GetActiveScene()->AddLayer("Menu", 1);
						auto options = std::make_shared<Options>();
						options->OnDetach([&]() {
							m_Mode = Mode::Walk;
						});
						options->OnDisconnect([&]() {
							SceneManager::SetActiveScene("Main");
							Application::Get()->DestroyGameScene();
						});
						SceneManager::GetActiveScene()->GetLayer("Menu")->AddEntity(options);
					}
					return true;
				}
				default: {
					return false;
				}
			}
		}
		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
				case sf::Mouse::Button::Left: {
					if (m_Mode == Mode::Build) {
						m_Mode = Mode::Walk;
						if (!CheckBuildingsForProduction()) {
							return false;
						}

						std::shared_ptr<Building> newBuilding;
						if (m_ProductionBuilding->GetType() == BuildingType::Home) {
							auto newHome = std::make_shared<Home>(*(Home*)m_ProductionBuilding.get());
							m_MaxSoldier += newHome->GetMaxCount();
							m_PlayerStats->SetMaxSoldierCount(m_MaxSoldier);
							newBuilding = newHome;
							newBuilding->SetProduction(false);

							// Send building to server
							Packet<Home> packet = {PacketType::CreateHome, m_Id, *newHome};
							size_t size;
							void* buffer = Serializator::serialize(packet, size);
							m_Client->send(buffer, size);
							free(buffer);
						}
						else if (m_ProductionBuilding->GetType() == BuildingType::Mine) {
							auto newMine = std::make_shared<Mine>(*(Mine*)m_ProductionBuilding.get());
							m_MoneyPerSecond += newMine->MoneyPerSecond();
							newBuilding = newMine;
							newBuilding->SetProduction(false);

							// Send building to server
							Packet<Mine> packet = {PacketType::CreateMine, m_Id, *newMine};
							size_t size;
							void* buffer = Serializator::serialize(packet, size);
							m_Client->send(buffer, size);
							free(buffer);
						}
						m_Buildings.push_back(newBuilding);
						return true;
					}
					return false;
				}
				default: {
					return false;
				}
			}
		}
		case sf::Event::MouseMoved: {
			const sf::View& view = SceneManager::GetActiveScene()->GetLayer("Game")->GetView();
			sf::Vector2f mousePos = Application::Get()->GetWindow().mapPixelToCoords({event.mouseMove.x, event.mouseMove.y}, view);
			if (m_Mode == Mode::Walk) {
				float xdiff = mousePos.x - m_Rect.getPosition().x;
				float ydiff = mousePos.y - m_Rect.getPosition().y;
				float radian = atan2f(ydiff, xdiff);
				m_Rect.setRotation(radian * (180.0f / (float)std::numbers::pi));

				auto line = (float) sqrt(pow(xdiff, 2) + pow(ydiff, 2));
				float times = line / m_Speed;
				if (times != 0.0f) {
					m_Velocity.x = xdiff / times;
					m_Velocity.y = ydiff / times;
				}

				return true;
			}
			if (m_Mode == Mode::Build) {
				m_ProductionBuilding->SetPosition(mousePos);
			}
			return false;
		}
		default: {
			return false;
		}
	}
}

void Self::BecomeServer(const std::string& serverName) {
	m_Server = std::make_shared<Server>(Server::PORT);
	// The thread holds its own reference, so the server can't be destroyed under it. Reset() stops and joins it.
	m_ServerThread = std::thread([server = m_Server]() {
		while (server->isRunning()) {
			server->update();
		}
	});
	BecomeClient();
	m_Client->connect("localhost", Server::PORT); // Make sure to call this function after setting the on connect callback.
}

void Self::BecomeClient() {
	m_Client = std::make_shared<Client>();
	m_Client->onConnect([&](uint16_t id) {
		std::cout << "[CLIENT]: Connected to the server." << "\n";
		m_Id = id;
		Packet<Player> packet = {PacketType::Join, id, *this};
		size_t size;
		void* buffer = Serializator::serialize(packet, size);
		m_Client->send(buffer, size);
		free(buffer);
	});
	m_Client->onDisconnect([]() {
		std::cout << "[CLIENT]: Disconnected from the server." << "\n";
	});
	m_Client->onReceive([&](void* data, size_t size) {
		// Packets can refer to players we don't know (e.g. a Leave for a peer that never joined).
		auto findPlayer = [&](uint16_t id) -> Player* {
			auto it = m_Players.find(id);
			return it != m_Players.end() ? it->second.get() : nullptr;
		};

		PacketType type = Serializator::deserializePacketType(data, size);
		if (type == PacketType::Join) {
			Packet<Player> packet = Serializator::deserialize<Player>(data, size);
			std::cout << "[CLIENT]: " + packet.data.GetName() + " joined the server." << "\n";
			m_Players[packet.id] = std::make_shared<Player>(packet.data);
		} else if (type == PacketType::Leave) {
			Packet<uint16_t> id = Serializator::deserialize<uint16_t>(data, size);
			if (Player* player = findPlayer(id.data)) {
				std::cout << "[CLIENT]: " + player->GetName() + " left the server." << "\n";
				m_Players.erase(id.data);
			}
		} else if (type == PacketType::Chat) {
			Packet<std::string> packet = Serializator::deserialize<std::string>(data, size);
			if (packet.id == SERVER_ID) {
				std::cout << packet.data << "\n";
			} else if (Player* player = findPlayer(packet.id)) {
				std::cout << player->GetName() + ": " + packet.data << "\n";
			}
		} else if (type == PacketType::Move) {
			Packet<sf::Vector2f> packet = Serializator::deserialize<sf::Vector2f>(data, size);
			if (Player* player = findPlayer(packet.id))
				player->SetPosition(packet.data);
		} else if (type == PacketType::CreateHome) {
			Packet<Home> packet = Serializator::deserialize<Home>(data, size);
			if (Player* player = findPlayer(packet.id))
				player->AddBuilding(std::make_shared<Home>(packet.data));
		} else if (type == PacketType::CreateMine) {
			Packet<Mine> packet = Serializator::deserialize<Mine>(data, size);
			if (Player* player = findPlayer(packet.id))
				player->AddBuilding(std::make_shared<Mine>(packet.data));
		}
	});
}

bool Self::CheckBuildingsForProduction() {
	static const int pudding = 30;

	static const auto check_function = [&](auto building) {
		sf::FloatRect biggerRect = building->GetRectangle();
		biggerRect.width += pudding * 2;
		biggerRect.height += pudding * 2;
		biggerRect.left -= pudding;
		biggerRect.top -= pudding;
		std::cout << m_ProductionBuilding.get() << "\n";
		return !m_ProductionBuilding->GetRectangle().intersects(biggerRect);
	};

	bool result = std::ranges::all_of(m_Buildings, check_function);
	if (!result)
		return result;

	for (const auto& [_, player] : m_Players) {
		result = std::ranges::all_of(player->m_Buildings, check_function);
		if (!result)
			return result;
	}

	return result;
}

std::shared_ptr<Client> Self::GetClient() {
	return m_Client;
}

std::shared_ptr<Server> Self::GetServer() {
	return m_Server;
}

void Self::Reset() {
	m_Buildings.clear();
	m_Players.clear();
	m_MaxSoldier = 0;
	m_MoneyPerSecond = 0;
	m_Money = 0;
	m_MoneyTime = 0;
	m_Speed = 100;
	m_Client->disconnect();
	m_Client.reset();
	if (m_Server)
		m_Server->stop();
	if (m_ServerThread.joinable())
		m_ServerThread.join();
	m_Server.reset();
	m_Mode = Mode::Walk;
}
