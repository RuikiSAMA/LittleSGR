#include "WindowsWindow.h"
#include "Window.h"
#include "Base.h"

namespace LittleSGR {
	bool WindowsWindow::s_Inited = false;

	void WindowsWindow::Init() {
		ASSERT(!s_Inited, "已初始化");	// 断言，若条件为0则中断程序
		Register();
		s_Inited = true;
	}

	void WindowsWindow::Terminate() {
		ASSERT(s_Inited, "未初始化");
		Unregister();
		s_Inited = false;
	}


	 
}