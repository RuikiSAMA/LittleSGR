#include <iostream>
#include"Application.h"
#include"InputCode.h"
#include<eigen/dense>
using namespace Eigen;

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
		FrameBuffer fb(m_Width, m_Height);

		m_Window->DrawFrameBuffer(fb);
	}

	Application::~Application() {
		Terminate();
	}

	void Application::Terminate() {
		delete m_Window;
		// 结束程序需要在调用 Terminate() 之前删除 m_Window 否则内存泄露

		WindowsWindow::Terminate();
	}







}