#pragma once

#define LOG(...) 
// ����յĺֻ꣬�����ſɱ����

#define ASSERT(x, ...) { if(!(x)) { LOG(__VA_ARGS__); __debugbreak(); } }
// ������Ժ꣬�� x Ϊ 0 ʱ�������ԣ�__VA_ARGS__ �����滻 ... �еĿɱ������... ������������Ϣ
// LOG ʵ������ֻ������ ... �еĿɱ����

