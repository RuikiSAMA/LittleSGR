#pragma once
#include <string>

namespace LittleSGR {
	class Window {
	public:
		Window(std::string title, int width, int height);
		virtual ~Window();
		
		virtual void Show() const = 0;

		bool Closed() const { return m_Closed; };

	public:
		static void Init();
		static void Terminate();
		static Window* Create(const std::string title, const int width, const int height);	//	工厂方法
		/* 工厂方法：
		在工厂方法模式中，有一个抽象 产品类(Window) ，它声明了创建产品对象的接口。
		有一个 工厂类接口(Creat();)，它定义了工厂方法，该方法返回一个抽象产品类型的对象。
		具体的 工厂类(WindowsWindow) 实现了工厂接口，并返回具体产品类型的实例。
		客户端代码通过工厂接口来创建产品对象，而无需知道具体的实现细节。
		*/

	protected:
		std::string m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed = true;
	};
}
