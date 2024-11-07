#include "WindowsWindow.h"
#include "Window.h"
#include "Base.h"

namespace LittleSGR {
	bool WindowsWindow::s_Inited = false;

	void WindowsWindow::Init() {
		ASSERT(!s_Inited, "�ѳ�ʼ��");	// ���ԣ�������Ϊ0���жϳ���
		Register();
		s_Inited = true;
	}

	void WindowsWindow::Terminate() {
		ASSERT(s_Inited, "δ��ʼ��");
		Unregister();
		s_Inited = false;
	}


	 
}