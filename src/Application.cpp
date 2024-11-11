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
		while (!m_Window->Closed()) {
			OnUpdate();

			Window::RunMessageLoop();
		}           
	}

	void Application::Init() {
		Window::Init();
		m_Window = Window::Create(m_Name, m_Width, m_Height);
	}

	void Application::Terminate() {
		delete m_Window;
		// ����������Ҫ�ڵ��� Terminate() ֮ǰɾ�� m_Window �����ڴ�й¶

		Window::Terminate();
	}

	void Application::OnUpdate()
	{

	}







}