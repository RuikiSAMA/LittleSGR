#include "WindowsWindow.h"
#include "Window.h"
#include "Base.h"
#include <iostream>

#define WINDOW_CLASS_NAME "Classname"
#define WINDOW_ENTER_NAME "enter"

namespace LittleSGR {
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height) 
		:Window(title, width, height) {
		ASSERT((s_Inited), " δע�ᴰ���಻�ܴ�������");	// ���ԣ�������Ϊ0���жϳ���
		/*
		����
		typedef struct tagRECT {
		  LONG left;	// ָ���������Ͻǵ� x ���ꡣ
		  LONG top;		// ָ���������Ͻǵ� y ���ꡣ
		  LONG right;	// ָ���������½ǵ� x ���ꡣ
		  LONG bottom;	// ָ���������½ǵ� y ���ꡣ
		} RECT, *PRECT, *NPRECT, *LPRECT;

		SetRect(*RECT, left, top, right, bottom);
		*/

		RECT rect;
		SetRect(&rect, 0, 0, width, height);

		DWORD style = WS_BORDER | WS_CAPTION;	// խ�߿򣬴�����

		/*
		���ô��ھ��γߴ硢��ʽ
		BOOL AdjustWindowRect(
		  LPRECT lpRect,      // ָ��RECT�ṹ��ָ�룬�ýṹָ���˵�����Ĵ��ھ��δ�С
		  DWORD dwStyle,      // ָ�����ڵ���ʽ
		  BOOL  bMenu         // ָʾ�����Ƿ��в˵�
		);
		*/

		AdjustWindowRect(&rect, style, false);


		/*
		HWND CreateWindowA(
		[in, optional] LPCSTR    lpClassName,	// ע�����ClassName
		[in, optional] LPCSTR    lpWindowName,	// �������ƣ�string ������ʹ�� c.str() ת��Ϊ const char*
		[in]           DWORD     dwStyle,	// ������ʽ, WS_CAPTION ���ھ��б�����
		[in]           int       x,	// �������Ͻ�x����
		[in]           int       y,	// �������Ͻ�y����
		[in]           int       nWidth,	// ���ڿ���
		[in]           int       nHeight,	// ���ڸ߶�
		[in, optional] HWND      hWndParent,	// �����ڵľ��
		[in, optional] HMENU     hMenu,	// ���ڲ˵�
		[in, optional] HINSTANCE hInstance,	// ���ڵľ��
		[in, optional] LPVOID    lpParam	// ����Ҫ���������� NULL��
		);
		*/

		m_Handle = CreateWindow(WINDOW_CLASS_NAME, m_Title.c_str(), style, CW_USEDEFAULT,
			CW_USEDEFAULT, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

		ASSERT(m_Handle != nullptr);
		m_Closed = false;

		/*
		Ϊ
		BOOL SetPropA(
		  [in]           HWND   hWnd,
		  [in]           LPCSTR lpString,
		  [in, optional] HANDLE hData
		);
		*/

		SetProp(m_Handle, WINDOW_ENTER_NAME, this);

		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);

		BITMAPINFOHEADER biHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;

		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biWidth = m_Width;
		biHeader.biHeight = -m_Height;
		biHeader.biPlanes = 1;
		biHeader.biCompression = BI_RGB;

		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&biHeader,
			DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);

		ASSERT(newBitmap != nullptr);

		constexpr int channelCount = 3;
		int size = m_Width * m_Height * channelCount * sizeof(unsigned char);
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);

		Show();
	}
	
	WindowsWindow::~WindowsWindow(){
		ShowWindow(m_Handle, SW_HIDE);
		RemoveProp(m_Handle, WINDOW_ENTER_NAME);
		DeleteDC(m_MemoryDC);
		DestroyWindow(m_Handle);
	}

	bool WindowsWindow::s_Inited = false;

	void WindowsWindow::Init() {

		ASSERT(!s_Inited);	
		Register();
		s_Inited = true;
	}

	void WindowsWindow::Register() {
		/*
		typedef struct tagWNDCLASSEXA {
		UINT      cbSize;	// �ṹ���С��������Ϊsizeof(WNDCLASSEX)
		UINT      style;	// ����ʽ��������ϣ�CS_HREDRAW | CS_VREDRAW; ����ʱ�ػ洰��
		WNDPROC   lpfnWndProc;	// ���ڹ��̺��������ڴ������͵����ڵ���Ϣ
		int       cbClsExtra;	// ���ݴ�����ṹ����Ķ����ֽ���������Ĭ����Ϊ0
		int       cbWndExtra;	// ����ʵ��֮�����Ķ����ֽ���������Ĭ����Ϊ0
		HINSTANCE hInstance;	// Ӧ�ó����ʵ�������Winmain�ĵ�һ��������û�п����� GetModuleHandle(NULL); ��ȡ��
		HICON     hIcon;	// Ӧ�õ�ͼ�꣬NULLΪĬ��ͼ��
		HCURSOR   hCursor;	// Ӧ�õĹ�꣬NULLΪĬ�Ϲ��
		HBRUSH    hbrBackground;	// ���ñ�����ˢ����ɫֵ��(HBRUSH)GetStockObject(WHITE_BRUSH) ��ȡ��ɫˢ��
		LPCSTR    lpszMenuName;		// �˵����ƣ�NULL Ϊû��Ĭ�ϲ˵�
		LPCSTR    lpszClassName;	// ��Ϊ�ַ�����ָ��������������Ϊ ATOM ������ָ�� RegisterClass ��������
		HICON     hIconSm;	// Сͼ�꣬NULLΪĬ��ͼ��
		}WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;
		*/

		WNDCLASSEXA wc = { 0 };
		wc.style = CS_HREDRAW | CS_VREDRAW;	// ����ʱ�ػ洰��
		wc.lpfnWndProc = WindowsWindow::WndProc;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszClassName = WINDOW_CLASS_NAME;
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.hInstance = GetModuleHandle(NULL); 
		wc.lpszMenuName = NULL;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		/*
		ATOM RegisterClassExA(
			[in] const WNDCLASSEXA *unnamedParam1
		);
		ע�ᴰ����;
		ATOM ����ֵ��Ψһ��ʶ��ע�������ԭ��;
		*/
		ATOM atom = RegisterClassExA(&wc);
	}
	
	void WindowsWindow::Terminate() {
		ASSERT(s_Inited);
		Unregister();
		s_Inited = false;
	}

	void WindowsWindow::Unregister(){
	
	}

	void  WindowsWindow::Show() const{
		HDC windowDC = GetDC(m_Handle);
		BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);
	}

	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetProp(hWnd, WINDOW_ENTER_NAME);
		if (window == nullptr)
			return DefWindowProc(hWnd, msgID, wParam, lParam);

		switch (msgID)
		{
		case WM_DESTROY:
			window->m_Closed = true;
			return 0;
		break;
		}
		return DefWindowProc(hWnd, msgID, wParam, lParam);
	}
}