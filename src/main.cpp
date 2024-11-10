#include<iostream>
#include"Application.h"
#include "WindowsWindow.h"
#include "Window.h"

using namespace std;
using namespace LittleSGR;

int main() {
	cout << "hello" << endl;
	Application app("LittleSGR", 400, 300);
	app.Run();
	return 0;
}