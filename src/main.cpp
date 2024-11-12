#include<iostream>
#include"Application.h"

using namespace std;
using namespace LittleSGR;

int main() {
	cout << "hello" << endl;
	Application app("LittleSGR", 500, 400);
	app.Run();
	return 0;
}