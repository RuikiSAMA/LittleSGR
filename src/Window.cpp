#include"Window.h"
#include"WindowsWindow.h"

namespace LittleSGR {
	Window::Window(std::string title, int width, int height) 
	:m_Title(title), m_Width(width), m_Height(height){}

	void Window::Init(){
		WindowsWindow::Init();
	}

	void Window::Terminate(){
		WindowsWindow::Terminate();
	}

	Window* Window::Create(const std::string title, const int width, const int height) {
		return new WindowsWindow(title, width, height);
	}
}