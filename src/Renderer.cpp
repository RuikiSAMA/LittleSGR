#include "Renderer.h"
#include "VertexShader.h"
#include <cmath>

namespace LittleSGR {
	Renderer::Renderer(int width, int height)
		:m_Width(width), m_Height(height){
	}

	Renderer::~Renderer(){
	}

	void Renderer::VertexShader(Varing& varings, const Vertex vertexs, const Uniform uniforms) {
		varings.ClipPos = uniforms.MVP * vertexs.ModelPos;
	}

	void Renderer::Draw(FrameBuffer framebuffer, Triangle triangle, Uniform unifrom) {

	}

	void Renderer::DrawLine(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color) {
		if (ClipLine(v0, v1)) {
			DrawLine_WithoutClip(v0, v1, fb, color);
		}
		else
			return;
	}

	int Renderer::ComputeOutCode(Vector2i v) {
		int code = INSIDE;  // initialised as being inside of clip window

		if (v.x() < 0)           // to the left of clip window
			code |= LEFT;
		if (v.x() > m_Width)      // to the right of clip window
			code |= RIGHT;

		if (v.y() < 0)           // below the clip window
			code |= BOTTOM;
		if (v.y() > m_Height)      // above the clip window
			code |= TOP;

		return code;
	}

	bool Renderer::ClipLine(Vector2i& v0, Vector2i& v1) {	//	��Ļ�ռ��µ��߶βü�
		int outcode0 = ComputeOutCode(v0);
		int outcode1 = ComputeOutCode(v1);

		bool accept = false;

		const int xmax = m_Width - 1;	//	�������ֵΪ�ߴ� -1
		const int xmin = 0;
		const int ymax = m_Height - 1;
		const int ymin = 0;

		while (true) {
			if (!(outcode0 | outcode1)) {
				// bitwise OR is 0: both points inside window; trivially accept and exit loop
				accept = true;
				break;
			}
			else if (outcode0 & outcode1) {
				// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
				// or BOTTOM), so both must be outside window; exit loop (accept is false)
				break;
			}
			else {
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				float x = 0, y = 0;

				// At least one endpoint is outside the clip rectangle; pick it.
				int outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;	// �ȶ�����ֵ�ϴ�ĵ���вü�����
				// No need to worry about divide-by-zero because, in each case, the
				// outcode bit being tested guarantees the denominator is non-zero

				if (outcodeOut & TOP) {    // point is above the clip window
					x = v0.x() + (v1.x() - v0.x()) * (ymax - v0.y()) / (v1.y() - v0.y());	//	����������
					y = ymax;
				}
				if (outcodeOut & BOTTOM) { // point is below the clip window
					x = v0.x() + (v1.x() - v0.x()) * (ymin - v0.y()) / (v1.y() - v0.y());
					y = ymin;
				}
				if (outcodeOut & RIGHT) {  // point is to the right of clip window
					y = v0.y() + (v1.y() - v0.y()) * (xmax - v0.x()) / (v1.x() - v0.x());
					x = xmax;
				}
				if (outcodeOut & LEFT) {   // point is to the left of clip window
					y = v0.y() + (v1.y() - v0.y()) * (xmin - v0.x()) / (v1.x() - v0.x());
					x = xmin;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == outcode0) {
					v0.x() = (int)x;
					v0.y() = (int)y;
					outcode0 = ComputeOutCode(v0);
				}
				else {
					v1.x() = (int)x;
					v1.y() = (int)y;
					outcode1 = ComputeOutCode(v1);
				}
			}
		}
		return accept;

		/* Cohen Sutherland �㷨��
		   ����Ļ�ı��ӳ�����Ϊ 9 �����򲢱�ţ�0000λ��Ļ�ɼ�����
		   1001|1000|1010
		   ----+----+----
		   0001|0000|0010
		   ----+----+----
		   0101|0100|0110

		   1����ֱ����������������ı���� ��λor ���㣬�����Ϊ 0 ��˵�������㶼����Ļ������
		   2������������� ��λand ��������Ϊ�㣬��˵���������λ�ö�����Ļ��
		   3���������˵�λ�ڲ�ͬ�������Ұ����ɼ�����ʱ������Ļ����֮���������֮һ��������һ������Ļ����֮�⣩��
			   ����������Ļ�߽�Ľ��㣬���滻�������Ļ֮��ĵ㣬Ȼ�����ظ�������ֱ�������㶼����Ļ������

		*/
	}

	void Renderer::DrawLine_WithoutClip(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color) {
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
				fb->SetColorbuffer(y, x, color);
			}
			else {
				fb->SetColorbuffer(x, y, color);
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

		/* �е��㷨
		�� e(i+1) Ϊ k + yi

		�� e(i+1) >= 0.5
		�� y(i+1) = yi + 1, �� e(i+1) -= 1	// ��Ϊ e(i+2) ����1����Ҫ�� 1.5 ���Ƚϣ�Ϊ��ʼ���� 0.5 ���Ƚϣ���ʽ����ͬʱ -1	// e(i+2) - 1 ? 1.5 -1

		�� e(i+1) < 0.5
		�� y(i+1) = yi

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

		Bresenham �Ż���ͬʱ���� 2dx ��ȥ��ĸ��dx > 0 ��˲�Ӱ��ȽϽ�����Ż�����Ҫ������������
		e0 = 0
		e(i+1) == ei + k == ei + dy/dx

		2dx * e(i+1) == 2dx * ei + 2 * dy
		2dx * ( e(i+1) ? 0.5 ) == 2dx * e(i+1) ? dx
		2dx * (e(i+1) - 1) == 2dx * e(i+1) - 2dx
		*/
	}
}