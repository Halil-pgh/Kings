#include "pch.h"

#include "Core/Application.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif
	Application::Get()->Run();
	Application::Destroy();
}
