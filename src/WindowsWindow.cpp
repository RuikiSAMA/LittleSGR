#include "WindowsWindow.h"
#include "Base.h"
#include <iostream>

#define WINDOW_CLASS_NAME "Classname"
#define WINDOW_ENTER_NAME "enter"

namespace LittleSGR {
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height)
		:m_Title(title), m_Width(width), m_Height(height) {
		ASSERT((s_Inited), "未初始化");	// ���ԣ�������Ϊ0���жϳ���

		memset(m_Keys, false, KEY_MAX_COUNT);

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

		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;	// խ�߿򣬴�����

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
		m_Close = false;

		/*
		Ϊ
		为窗口添加附加属性
		BOOL SetPropA(
		  [in]           HWND   hWnd,	// 要添加附加属性的窗口
		  [in]           LPCSTR lpString,	// 添加的属性的名字
		  [in, optional] HANDLE hData	// 添加的属性的值
		);
		*/

		SetProp(m_Handle, WINDOW_ENTER_NAME, this);
		/* 为窗口添加具体窗口对象的指针，因为窗口过程函数为静态函数，
			因此需要传入窗口对象的指针来为具体的窗口实现交互*/


			/*通过句柄获取窗口的 设备上下文DC
			HDC GetDC(
			  [in] HWND hWnd	// 若值为NULL则获取整个屏幕设备的DC
			);
			*/
		HDC windowDC = GetDC(m_Handle);
		// 获取窗口的 设备上下文DC

		/*创建与目标 设备上下文DC 兼容的 内存设备上下文DC
		// 内存设备上下文DC 可以理解为用以模拟设备的 一片内存
		HDC CreateCompatibleDC(
		  [in] HDC hdc
		);
		*/
		m_MemoryDC = CreateCompatibleDC(windowDC);
		// 创建一份与窗口 DC 相同的 内存设备上下文DC

		ReleaseDC(m_Handle, windowDC);
		// 释放获取的屏幕设备上下文DC

		/* 指定需要创建的位图的参数
		typedef struct _tagBITMAPINFOHEADER {
		  DWORD biSize;	// 指定结构所需的字节数
		  LONG  biWidth;	// 位图宽度
		  LONG  biHeight;	// 位图长度
		  WORD  biPlanes;	// 目标设备的平面数，必须为1
		  WORD  biBitCount;	// 每像素的位数
		  DWORD biCompression;	// 位图的压缩类型，DIB位图无法压缩，未压缩 BI_RGB
		  DWORD biSizeImage;	// 指定图像大小，对于BI_RGB位图设为0
		  LONG  biXPelsPerMeter;// 目标设备的水平分辨率
		  LONG  biYPelsPerMeter;// 目标设备的垂直分辨率
		  DWORD biClrUsed;	// 指定位图实际使用的颜色索引数，用于 biCompression 指定的压缩模式，为0使用与biBitCount成员的之相应的最大颜色数
		  DWORD biClrImportant;	//指定位图所需的颜色索引数，为 0 默认需要所有颜色
		} _BITMAPINFOHEADER;

		*/

		BITMAPINFOHEADER biHeader = {};

		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biWidth = m_Width;
		biHeader.biHeight = -m_Height;	// 希望从左上开始向下渲染，因此需要加负号
		biHeader.biPlanes = 1;
		biHeader.biBitCount = 24;
		biHeader.biCompression = BI_RGB;

		/*
		创建 与设备无关的位图(DIB) 部分
		HBITMAP CreateDIBSection(
		  [in]  HDC              hdc,	// 指定目标设备的像素格式
		  [in]  const BITMAPINFO *pbmi,	// 定义好参数的 BITMAPINFO 对象指针
		  [in]  UINT             usage,	// DIB_RGB_COLORS 表示位图的颜色表实际包含的RGB值
		  [out] VOID             **ppvBits,	// 输出函数，函数执行成功后指向创建的 DIB位图 ，可以通过这个参数的指针来访问和操作DIB位图的像素数据
		  [in]  HANDLE           hSection,	// 默认NULL
		  [in]  DWORD            offset	// 默认0
		);
		*/

		HBITMAP newBitmap;

		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&biHeader,
			DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
		// 创建位图并与 m_MemoryDC 关联，并返回位图给 newBitmap 
		// m_Buffer 为指向图像缓冲区的指针，使用 m_Buffer 来操作图像缓冲区中的位图

		ASSERT(newBitmap != nullptr);

		constexpr int channelCount = 3;
		// constexpr 定义编译时常量，channelCount 定义颜色通道的数目为 3(RGB)

		int size = m_Width * m_Height * channelCount * sizeof(unsigned char);
		// 计算整个图像缓冲区的大小(图像面积 * 颜色通道数 * 每个颜色通道的大小(8位无符号整型))

		/*
		为缓冲区设置指定的字符
		void *memset(
		   void *dest,	// 指向目标的指针
		   int c,	// 需要设置的字符
		   size_t count	// 设置的字符数
		);
		*/
		memset(m_Buffer, 0, size);
		// 初始化缓冲区(0为黑色)

		HBITMAP oldBitmap;
		// oldBitmap 保存 m_Memory 已经选中过的位图对象，以调用 DeleteObject() 释放位图内存，以防止内存泄露

		/*用于选中 GDIproject 到DC中，并返回已经被选中的对象的句柄
		HGDIOBJ SelectObject(
		  [in] HDC     hdc,
		  [in] HGDIOBJ h
		);
		*/
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);
		// 将 newBitmap 选中到 m_MemoryDC 中，并返回之前选中的对象 

		DeleteObject(oldBitmap);
		// 删除之前被选中到 m_MemoryDC 的位图

		Show();
	}

	void  WindowsWindow::Show() const {
		HDC windowDC = GetDC(m_Handle);

		/* Blt(Block Transfer) Blit
		BOOL BitBlt(
		  [in] HDC   hdc,	// 要传入的DC句柄
		  [in] int   x,	// 目标矩形左上角的x坐标
		  [in] int   y, // 目标矩形左上角的y坐标
		  [in] int   cx,	// 源矩形和目标矩形的宽度
		  [in] int   cy,	// 源矩形和目标矩形的高度
		  [in] HDC   hdcSrc,// 被传入的DC句柄
		  [in] int   x1,	// 源矩形左上角的x坐标
		  [in] int   y1,	// 源矩形左上角的y坐标
		  [in] DWORD rop	// 定义如何将源矩形的颜色数据与目标矩形的颜色数据相结合。SRCCOPY为直接复制源矩形的颜色
		);
		*/
		BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		// 将 m_MemoryDC 中的值传输到窗口的实际 DC 中

		/* 指定窗口的显示状态
		BOOL ShowWindow(
		  [in] HWND hWnd,	// 窗口句柄
		  [in] int  nCmdShow	// SW_SHOW激活窗口并以当前的尺寸与位置显示窗口
		);
		*/
		ShowWindow(m_Handle, SW_SHOW);

		ReleaseDC(m_Handle, windowDC);
	}

	WindowsWindow::~WindowsWindow() {
		ShowWindow(m_Handle, SW_HIDE);
		// SW_HIDE 隐藏窗口并激活其他窗口

		RemoveProp(m_Handle, WINDOW_ENTER_NAME);
		// 解除附加属性

		DeleteDC(m_MemoryDC);
		// 删除 m_MemoryDC

		DestroyWindow(m_Handle);
		// 摧毁窗口
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

	void WindowsWindow::Unregister() {

	}

	void WindowsWindow::RunMessageLoop() {
		std::cout << "loop!";
		MSG message;
		while (GetMessage(&message, NULL, 0, 0)) {
			// 获取消息
			TranslateMessage(&message);
			// 翻译消息
			DispatchMessage(&message);
			// 分发消息到相应的窗口过程
		}
	}

	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetProp(hWnd, WINDOW_ENTER_NAME);
		// 创建 WindowsWindow 指针并指向当前窗口，GetProp() 获得窗口的附加属性

		if (window == nullptr)
			return DefWindowProc(hWnd, msgID, wParam, lParam);
		// DefWindowProc(hWnd, msgID, wParam, lParam); 为不处理的窗口消息提供默认处理

		switch (msgID)
		{
		case WM_DESTROY:
			window->m_Close = true;
			return 0;
		case WM_KEYDOWN:	// 按键按下
			KeyEvent(window, wParam, 1);
			return 0;
		case WM_KEYUP:	// 按键抬起
			KeyEvent(window, wParam, 0);
			return 0;
		default:
			return DefWindowProc(hWnd, msgID, wParam, lParam);
		}
	}

	void WindowsWindow::KeyEvent(WindowsWindow* wd, const WPARAM wParam, const bool state) {
		if (wParam >= '0' && wParam <= '9') {
			wd->m_Keys[wParam] = state;
			std::cout << wParam << std::endl;
			return;
		}
		if (wParam >= 'A' && wParam <= 'Z') {
			wd->m_Keys[wParam] = state;
			return;
		}
	}

	bool WindowsWindow::GetKeyState(const int i) {
		return m_Keys[i];
	}

	bool WindowsWindow::IsClosed() {
		return m_Close;
	}

	void WindowsWindow::DrawFrameBuffer(const FrameBuffer framebuffer) {
		//std::cout << framebuffer.GetWidth() << m_Width;
		int width = (framebuffer.GetWidth() > m_Width ? m_Width : framebuffer.GetWidth());
		int height = (framebuffer.GetHeight() > m_Height ? m_Height : framebuffer.GetHeight());
		std::cout << width << "and" << height;
		// 确保渲染画面不会超过窗口大小
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				const int m_bufferIndex = (i * width + j) * 3;
				//	m_buffer 相邻三个位置为一个单位表示g,b,r，因此vec[]数组的索引需要 *3
				m_Buffer[m_bufferIndex] = (unsigned char)(framebuffer.GetColorbuffer(j, height - i - 1).z() * 255.0f);	//	height - i - 1 为从上向下读取
				m_Buffer[m_bufferIndex + 1] = (unsigned char)(framebuffer.GetColorbuffer(j, height - i - 1).y() * 255.0f);
				m_Buffer[m_bufferIndex + 2] = (unsigned char)(framebuffer.GetColorbuffer(j, height - i - 1).x() * 255.0f);
				//	* 255.0f : 在framebuffer 中颜色值被归一化到 0-1 之间，*255 后再强制转换为 unsigned char 以便用8位无符号整型保存颜色值
				//	 注意：在 m_buffer 中颜色的排序为 b, g, r ,与 framebuffer 中 vec3d 的 r, g, b 相反，需要反着读取 vec3d 来对 m_buffer 赋值
			}
		}
		Show();
	}
}