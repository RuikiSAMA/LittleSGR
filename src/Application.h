#pragma once
#include <string>

namespace LittleSGR {
    class Application {
    public:
        Application(std::string name, const int width, const int height) {}

        ~Application() {}

        void Run() {}

    private:
        void Application::Init() {}

        void Application::Terminate() {}

        void Application::OnUpdate() {}

    private:
        std::string m_Name;
        int m_Width;
        int m_Height
    };
}