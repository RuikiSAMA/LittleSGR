#pragma once
#include <string>

namespace LittleSGR {
	class Window {
	pubilc:
		Window();
		virtual ~Window();
		
		virtual void Show() const = 0;

		bool Closed() const {return m_Closed};

	public:
		static void Init();
		static void Terminate();
		static Window* Creat(const std::string title, const int width, const int height);

	protected:
		std::string m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed = true;
	};
}
