#pragma once
#include <string>
#include <Windows.h>
#include "FrameBuffer.h"
#define KEY_MAX_COUNT 128

namespace LittleSGR {
	class WindowsWindow{
	public:
		WindowsWindow(const std::string title, const int width, const int height);
		~WindowsWindow();

		static void RunMessageLoop();

		void Show() const;
		bool IsClosed();
		bool GetKeyState(const int i);
		void DrawFrameBuffer(const FrameBuffer framebuffer);

	public:
		static void Init();
		static void Terminate();

	private:
		static void Register();
		static void Unregister();

		static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);

		static void KeyEvent(WindowsWindow* wd, const WPARAM wParam, const bool state);

	private:
		bool m_Close = true;

		std::string m_Title;
		int m_Width;
		int m_Height;

		HWND m_Handle;

		HDC m_MemoryDC;
		unsigned char* m_Buffer;

		static bool s_Inited;	// ³õÊ¼»¯×´Ì¬

		bool m_Keys[KEY_MAX_COUNT];
	};
}