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
		static Window* Create(const std::string title, const int width, const int height);	//	��������
		/* ����������
		�ڹ�������ģʽ�У���һ������ ��Ʒ��(Window) ���������˴�����Ʒ����Ľӿڡ�
		��һ�� ������ӿ�(Creat();)���������˹����������÷�������һ�������Ʒ���͵Ķ���
		����� ������(WindowsWindow) ʵ���˹����ӿڣ������ؾ����Ʒ���͵�ʵ����
		�ͻ��˴���ͨ�������ӿ���������Ʒ���󣬶�����֪�������ʵ��ϸ�ڡ�
		*/

	protected:
		std::string m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed = true;
	};
}
