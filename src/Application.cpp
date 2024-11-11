#include <iostream>
#include"Application.h"
#include"InputCode.h"

namespace LittleSGR {
	Application::Application(std::string name, const int width, const int height)
		: m_Name(name), m_Width(width), m_Height(height) {
		Init();
	}

	void Application::Init() {
		std::cout << "Init" << std::endl;
		WindowsWindow::Init();
		m_Window = new WindowsWindow(m_Name, m_Width, m_Height);
	}

	void Application::Run()
	{
		while (!m_Window->IsClosed()) {
			OnUpdate();

			WindowsWindow::RunMessageLoop();
		}           
	}

	void Application::OnUpdate()
	{

	}

	Application::~Application() {
		Terminate();
	}

	void Application::Terminate() {
		delete m_Window;
		// ����������Ҫ�ڵ��� Terminate() ֮ǰɾ�� m_Window �����ڴ�й¶

		WindowsWindow::Terminate();
	}







}