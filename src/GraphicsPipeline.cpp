#include"GraphicsPipeline.h"
#include <iostream>
using namespace Eigen;

namespace LittleSGR {
	GraphicsPipeline::GraphicsPipeline(int width, int height)
		: m_Width(width), m_Height(height) {

	}

	GraphicsPipeline::~GraphicsPipeline() {

	}

	void GraphicsPipeline::DrawLine(Vector2i v0, Vector2i v1, FrameBuffer* fb) {
		bool swap = false;
		if (std::abs(v1.y() - v0.y()) > std::abs(v1.x() - v0.x())) {
			swap = true;
			std::swap(v0.x(), v0.y());
			std::swap(v1.x(), v1.y());
		}

		if (v0.x() > v1.x()) {
			std::swap(v0.x(), v1.x());
			std::swap(v0.y(), v1.y());

		}

		const int dx = v1.x() - v0.x();
		const int dy = v1.y() - v0.y();

		int derror2 = std::abs(dy) * 2;
		int error2 = 0;
		int y = v0.y();
		for (int x = v0.x(); x <= v1.x(); x++) {
			if (swap) {
				fb->SetColorbuffer(y, x, Vector3f(1, 1, 1));
			}
			else {
				fb->SetColorbuffer(x, y, Vector3f(1, 1, 1));
			}
			error2 += derror2;
			if (error2 > dx) {
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}

		//fb->SetColorbuffer(v0.x(), v0.y(), Vector3f(1, 0, 0));
		//fb->SetColorbuffer(v1.x(), v1.y(), Vector3f(1, 0, 0));
		//int y = v0.y();
		//int e = 0;
		//for (int i = 1; i < dx; i++) {	// e(i+1) == ei + k == ei + dy/dx 
		//	e = 2 * dx * e + 2 * dy;	// 2dx * e(i+1) == 2dx * ei + 2 * dy
		//	const int d = e;
		//	if (d >= dx) {				// 2dx * ( e(i+1) ? 0.5 ) == 2dx * e(i+1) ? dx	
		//		y++;
		//		if (swap)
		//			fb->SetColorbuffer(v0.y() + y, v0.x() + i, Vector3f(1, 1, 1));
		//		else
		//			fb->SetColorbuffer(v0.x() + i, v0.y() + y, Vector3f(1, 1, 1));
		//		e -= 2 * dx;			// 2dx * (e(i+1) - 1) == 2dx * e(i+1) - 2dx
		//	}
		//	if (d < dx) {
		//		if (swap)
		//			fb->SetColorbuffer(v0.y() + y, v0.x() + i, Vector3f(1, 1, 1));
		//		else
		//			fb->SetColorbuffer(v0.x() + i, v0.y() + y, Vector3f(1, 1, 1));
		//	}
		//}

		/* 中点算法
		设 e(i+1) 为 k + yi

		若 e(i+1) >= 0.5
		则 y(i+1) = yi + 1, 且 e(i+1) -= 1	// 因为 e(i+2) 加了1，需要与 1.5 作比较，为了始终与 0.5 作比较，等式两边同时 -1	// e(i+2) - 1 ? 1.5 -1

		若 e(i+1) < 0.5
		则 y(i+1) = yi

		const float k = dy / dx;
		float e = 0;
		int y = v0.y();

		fb->SetColorbuffer(v0.x(), v0.y(), Vector3f(1, 1, 1));
		for (int i = 1; i <= dx; i++) {
			e += k;
			if (e >= 0.5) {
				y++;
				fb->SetColorbuffer(v0.x() + i, v0.y() + y, Vector3f(1, 1, 1));
				e--;
			}
			if (e < 0.5) {
				fb->SetColorbuffer(v0.x() + i, v0.y() + y, Vector3f(1, 1, 1));
			}
		}

		Bresenham 优化：同时乘以 2dx 消去分母，dx > 0 因此不影响比较结果，优化后不需要做浮点数运算
		e0 = 0
		e(i+1) == ei + k == ei + dy/dx

		2dx * e(i+1) == 2dx * ei + 2 * dy
		2dx * ( e(i+1) ? 0.5 ) == 2dx * e(i+1) ? dx
		2dx * (e(i+1) - 1) == 2dx * e(i+1) - 2dx
		*/
	}
}