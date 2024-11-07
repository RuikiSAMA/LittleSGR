#include"Application.h"
#include"Window.h"
namespace LittleSGR {
	Application::Application(std::string name, const int width, const int height)
		: m_Name(name), m_Width(width), m_Height(height) {
		Init();
	}

	Application::~Application() {}

	void Application::Run()
	{
		bool running = true;
		while (running) {
			OnUpdate();
		}
	}

	void Application::Init() {
		Window::Init();
	}

	void Application::Terminate() {
		Window::Terminate();
	}

	void Application::OnUpdate()
	{

	}







}