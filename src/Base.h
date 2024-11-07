#pragma once

#define LOG(...) 
// 定义空的宏，只用来放可变变量

#define ASSERT(x, ...) { if(!(x)) { LOG(__VA_ARGS__); __debugbreak(); } }
// 定义断言宏，当 x 为 0 时触发断言，__VA_ARGS__ 用来替换 ... 中的可变变量，... 可以填充错误信息
// LOG 实际作用只是收留 ... 中的可变变量

