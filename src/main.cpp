#include "pch.h"

#include "Core/Application.h"

int main() {
	Application::Get()->Run();
	Application::Destroy();
}
