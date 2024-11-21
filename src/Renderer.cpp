#include "Renderer.h"
#include "VertexShader.h"
#include <cmath>

namespace LittleSGR {
	int Renderer::m_Width = 500;
	int Renderer::m_Height = 400;
	float Renderer::m_Aspect = 500.0f / 400.0f;
	Frustum Renderer::m_Frustum(Renderer::m_Aspect);
	Camera Renderer::m_Camera;

	Renderer::Renderer(){

	}

	Renderer::~Renderer() {
	}

	void Renderer::init(int width, int height, Frustum frustum, Camera camera, float aspect) {
		m_Frustum = frustum;
		m_Camera = camera;
		m_Aspect = aspect;
	}

	bool Renderer::FrustumCull(Varing* varing) {
		return false;
	}

	void Renderer::VertexShader(const Vertex vertex, Varing& varings, Uniform uniforms) {
		Frustum frustum(1.0f, 1.0f, 10.0f, 90.0f / 180.0f * PI);
		Matrix4f Pmat = ProjectTransMat(frustum);
		//std::cout << "P:" << Pmat << std::endl;
		uniforms.MVP = ProjectTransMat(frustum);
		varings.ClipPos = uniforms.MVP * vertex.ModelPos;
	}

	void Renderer::GetNdcPos(Varing* varing, int vertexNum) {
		for (int i = 0; i < vertexNum; i++) {
			varing[i].NdcPos = varing[i].ClipPos * (1.0 / varing[i].ClipPos.w());
			std::cout << "NdcPos[" << i << "]" << varing[i].NdcPos << std::endl;
		}
	}

	void Renderer::GetFragPos(Varing* varing, int vertexNum, int width, int height) {
		for (int i = 0; i < vertexNum; i++) {
			varing[i].FragPos.x() = (varing[i].NdcPos.x() + 1) * 0.5f * width;
			varing[i].FragPos.y() = (varing[i].NdcPos.y() + 1) * 0.5f * height;
			varing[i].FragPos.z() = (varing[i].NdcPos.z() + 1) * 0.5;
			varing[i].FragPos.w() = varing[i].NdcPos.w();
		}
	}

	void Renderer::Rasterization(VaringTriangle vTriangle, FrameBuffer& framebuffer) {
		
		//Vector2i v0((int)vTriangle[0].FragPos.x(), (int)vTriangle[0].FragPos.y());
		//Vector2i v1((int)vTriangle[1].FragPos.x(), (int)vTriangle[1].FragPos.y());
		//Vector2i v2((int)vTriangle[2].FragPos.x(), (int)vTriangle[2].FragPos.y());
		//
		//DrawLine(v0, v1, framebuffer, Vector3f(1.0f, 1.0f, 1.0f));
		//DrawLine(v1, v2, framebuffer, Vector3f(1.0f, 1.0f, 1.0f));
		//DrawLine(v2, v0, framebuffer, Vector3f(1.0f, 1.0f, 1.0f));
	}

	void Renderer::Draw(FrameBuffer& framebuffer, VertexTriangle vertexTriangle, Uniform unifrom) {
		Varing varing[9];	//	三角形经过裁剪最多形成9个顶点的多边形
		for (int i = 0; i < 3; i++)
			VertexShader(vertexTriangle[i], varing[i], unifrom);	//	对三角形每个顶点执行顶点着色器
		if (FrustumCull(varing))	//	进行视椎剔除
			return;
		int VertexNum = ClipVertex(varing);

		GetNdcPos(varing, VertexNum);	//	进行透视除法
		int fbWidth = framebuffer.GetWidth();
		int fbHeight = framebuffer.GetHeight();

		GetFragPos(varing, VertexNum, fbWidth, fbHeight);	//	进行视口变换

		if (VertexNum == 3) {
			VaringTriangle vTriangle;
			for (int i = 0; i < 3; i++) {
				vTriangle[i] = varing[i];
			}
			Rasterization(vTriangle, framebuffer);
		}
		else{	//	将顶点连成三角形
			for (int i = 0; i < VertexNum - 2; i++) {
				VaringTriangle vTriangle;
				vTriangle[0] = varing[i];
				vTriangle[1] = varing[i + 1];
				vTriangle[2] = varing[i + 2];

				Rasterization(vTriangle, framebuffer);
			}
		}
	}

	void Renderer::DrawLine(Vector2i v0, Vector2i v1, FrameBuffer& fb, Vector3f color) {
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

	bool Renderer::ClipLine(Vector2i& v0, Vector2i& v1) {	//	屏幕空间下的线段裁剪
		int outcode0 = ComputeOutCode(v0);
		int outcode1 = ComputeOutCode(v1);

		bool accept = false;

		const int xmax = m_Width - 1;	//	坐标最大值为尺寸 -1
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
				int outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;	// 先对掩码值较大的点进行裁剪运算
				// No need to worry about divide-by-zero because, in each case, the
				// outcode bit being tested guarantees the denominator is non-zero

				if (outcodeOut & TOP) {    // point is above the clip window
					x = v0.x() + (v1.x() - v0.x()) * (ymax - v0.y()) / (v1.y() - v0.y());	//	三角形相似
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

		/* Cohen Sutherland 算法：
		   将屏幕的边延长并分为 9 个区域并编号，0000位屏幕可见区域
		   1001|1000|1010
		   ----+----+----
		   0001|0000|0010
		   ----+----+----
		   0101|0100|0110

		   1、对直线两个点所在区域的编号做 按位or 运算，若结果为 0 则说明两个点都在屏幕区域内
		   2、若两个编号做 按位and 运算结果不为零，则说明两个点的位置都在屏幕外
		   3、在两个端点位于不同区域内且包含可见区域时，找屏幕区域之外的两个点之一（至少有一个在屏幕区域之外），
			   计算出点和屏幕边界的交点，并替换掉这个屏幕之外的点，然后再重复操作，直到两个点都在屏幕区域内

		*/
	}

	void Renderer::DrawLine_WithoutClip(Vector2i v0, Vector2i v1, FrameBuffer& fb, Vector3f color) {
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
				fb.SetColorbuffer(y, x, color);
			}
			else {
				fb.SetColorbuffer(x, y, color);
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

namespace LittleSGR {

	int Renderer::ClipVertex(Varing* varings) {
		//	注：传入的数组大小应为9(裁剪三角形后最多产生9个顶点的多边形)
		int VertexNum = 3;
		bool v0 = IsInFrustum(varings[0]);
		bool v1 = IsInFrustum(varings[1]);
  		bool v2 = IsInFrustum(varings[2]);
		if (v0 && v1 && v2)
			return VertexNum;	//	若三角形三个顶点都在视椎内则不需要裁剪

		Varing varings_[9];	//	使用双重缓存, 防止只使用一个顶点集时造成新的顶点将未处理的顶点数据覆盖的情况
		//	将顶点集与所有视椎的面进行裁剪
		
		VertexNum = ClippingPlane(varings, varings_, Plane::POSITIVE_W, VertexNum); 
		VertexNum = ClippingPlane(varings, varings_, Plane::Z_FAR, VertexNum);
		VertexNum = ClippingPlane(varings_, varings, Plane::Z_NEAR, VertexNum);
		VertexNum = ClippingPlane(varings, varings_, Plane::POSITIVE_X, VertexNum);
		VertexNum = ClippingPlane(varings_, varings, Plane::NEGATIVE_X, VertexNum);
		VertexNum = ClippingPlane(varings, varings_, Plane::POSITIVE_Y, VertexNum);
		VertexNum = ClippingPlane(varings_, varings, Plane::NEGATIVE_Y, VertexNum);
		varings = varings_;	//	将最后一次裁剪后的结果复制回varings
		std::cout << "After Clip , vertex Count:" << VertexNum << std::endl;
		return VertexNum;
	}

	int Renderer::ClippingPlane(const Varing* invarings, Varing* outvarings, Plane plane, int InVertexNum) {
		//	Suntherland hodgman算法
		int OutVertexNum = 0;	//	输出顶点数量、输出顶点的索引
		for (int i = 0; i < InVertexNum; i++) {
			int lastIndex = (i - 1 + InVertexNum) % InVertexNum;	//	上一个顶点的索引，+size 再 %size 防止出现负数

			if (IsInPlane(invarings[i], plane) != IsInPlane(invarings[lastIndex], plane)) {
				//	若当前顶点与上一个顶点中有一个不在当前面里侧，则两顶点连线一定会穿过平面
				float ratio = IntersectRatio(invarings[i], invarings[lastIndex], plane);
				//std::cout << "Ratio:" << ratio << std::endl;
				outvarings[OutVertexNum] = VaringLerp(invarings[i], invarings[lastIndex], ratio, plane);
				//std::cout << "CurrentPos:" << invarings[i].ClipPos << std::endl;
				//std::cout << "LastPos:" << invarings[lastIndex].ClipPos << std::endl;
				//std::cout << "IntersectPos:" << outvarings[OutVertexNum].ClipPos << std::endl;
				OutVertexNum++;
			}

			if (IsInPlane(invarings[i], plane)) {	//	若当前顶点在当前面里侧，则在输出顶点中加入当前顶点，输出顶点数量+1
				outvarings[OutVertexNum] = invarings[i];
				OutVertexNum++;
			}
		}
		std::cout << "vertex Count:" << OutVertexNum << std::endl;
		return OutVertexNum;
	}

	Varing Renderer::VaringLerp(Varing v0, Varing v1, float ratio, Plane plane) {
		Varing vOut;
		vOut.ClipPos.x() = v0.ClipPos.x() + ratio * (v1.ClipPos.x() - v0.ClipPos.x());
		vOut.ClipPos.y() = v0.ClipPos.y() + ratio * (v1.ClipPos.y() - v0.ClipPos.y());
		vOut.ClipPos.z() = v0.ClipPos.z() + ratio * (v1.ClipPos.z() - v0.ClipPos.z());
		vOut.ClipPos.w() = v0.ClipPos.w() + ratio * (v1.ClipPos.w() - v0.ClipPos.w());
		return vOut;
		//switch (plane)
		//{
		//case LittleSGR::Renderer::Plane::POSITIVE_X:
		//	vOut.ClipPos.x() = v0.ClipPos.x() + ratio * (v1.ClipPos.x() - v0.ClipPos.x());
		//	return vOut;
		//case LittleSGR::Renderer::Plane::POSITIVE_Y:
		//	vOut.ClipPos.y() = v0.ClipPos.y() + ratio * (v1.ClipPos.y() - v0.ClipPos.y());
		//	return vOut;
		//case LittleSGR::Renderer::Plane::Z_FAR:
		//	vOut.ClipPos.z() = v0.ClipPos.z() + ratio * (v1.ClipPos.z() - v0.ClipPos.z());
		//	return vOut;
		//case LittleSGR::Renderer::Plane::NEGATIVE_X:
		//	vOut.ClipPos.x() = v0.ClipPos.x() + ratio * (v1.ClipPos.x() - v0.ClipPos.x());
		//	return vOut;
		//case LittleSGR::Renderer::Plane::NEGATIVE_Y:
		//	vOut.ClipPos.y() = v0.ClipPos.y() + ratio * (v1.ClipPos.y() - v0.ClipPos.y());
		//	return vOut;
		//case LittleSGR::Renderer::Plane::Z_NEAR:
		//	vOut.ClipPos.z() = v0.ClipPos.z() + ratio * (v1.ClipPos.z() - v0.ClipPos.z());
		//	return vOut;
		//}
	}

	bool Renderer::IsInPlane(Varing varing, Plane plane) {
		switch (plane)
		{
		case LittleSGR::Renderer::Plane::POSITIVE_W:
			//	先剔除掉w值为负的顶点，这类顶点在z轴正侧，不可能满足下列任何条件，先剔除掉减少计算量。先进行near剔除也可以。
			return varing.ClipPos.w() > epsilon;
		case LittleSGR::Renderer::Plane::POSITIVE_X:
			return varing.ClipPos.x() - varing.ClipPos.w() <= epsilon;
		case LittleSGR::Renderer::Plane::POSITIVE_Y:
			return varing.ClipPos.y() - varing.ClipPos.w() <= epsilon;
		case LittleSGR::Renderer::Plane::Z_FAR:
			return varing.ClipPos.z() - varing.ClipPos.w() <= epsilon;
		case LittleSGR::Renderer::Plane::NEGATIVE_X:
			return varing.ClipPos.x() + varing.ClipPos.w() >= epsilon;
		case LittleSGR::Renderer::Plane::NEGATIVE_Y:
			return varing.ClipPos.y() + varing.ClipPos.w() >= epsilon;
		case LittleSGR::Renderer::Plane::Z_NEAR:
			return varing.ClipPos.z() + varing.ClipPos.w() >= epsilon;
		}
	}

	float Renderer::IntersectRatio(Varing v0, Varing v1, Plane plane) {
		switch (plane)
		{
		case LittleSGR::Renderer::Plane::POSITIVE_W:
			return (v0.ClipPos.w() - 0.0f) / ((v0.ClipPos.w() - 0.0f) - (v1.ClipPos.w() - 0.0f));
		case LittleSGR::Renderer::Plane::POSITIVE_X:
			return (v0.ClipPos.w() - v0.ClipPos.x()) / ((v0.ClipPos.w() - v0.ClipPos.x()) - (v1.ClipPos.w() - v1.ClipPos.x()));
		case LittleSGR::Renderer::Plane::POSITIVE_Y:
			return (v0.ClipPos.w() - v0.ClipPos.y()) / ((v0.ClipPos.w() - v0.ClipPos.y()) - (v1.ClipPos.w() - v1.ClipPos.y()));
		case LittleSGR::Renderer::Plane::Z_FAR:
			return (v0.ClipPos.w() - v0.ClipPos.z()) / ((v0.ClipPos.w() - v0.ClipPos.z()) - (v1.ClipPos.w() - v1.ClipPos.z()));
		case LittleSGR::Renderer::Plane::NEGATIVE_X:
			return (v0.ClipPos.w() - (-v0.ClipPos.x())) / ((v0.ClipPos.w() - (-v0.ClipPos.x())) - (v1.ClipPos.w() - (-v1.ClipPos.x())));
		case LittleSGR::Renderer::Plane::NEGATIVE_Y:
			return (v0.ClipPos.w() - (-v0.ClipPos.y())) / ((v0.ClipPos.w() - (-v0.ClipPos.y())) - (v1.ClipPos.w() - (-v1.ClipPos.y())));
		case LittleSGR::Renderer::Plane::Z_NEAR:
			return (v0.ClipPos.w() - (-v0.ClipPos.z())) / ((v0.ClipPos.w() - (-v0.ClipPos.z())) - (v1.ClipPos.w() - (-v1.ClipPos.z())));
		}
	}

	bool Renderer::IsInFrustum(Varing varing) {
		float W = std::fabs(varing.ClipPos.w());
		float X = std::fabs(varing.ClipPos.x());
		float Y = std::fabs(varing.ClipPos.y());
		float Z = std::fabs(varing.ClipPos.z());
		//std::cout << X << ' ' << W << std::endl;
		bool IsX = X - W <= epsilon;
		bool IsY = Y - W <= epsilon;
		bool IsZ = Z - W <= epsilon;
		//	通过判断Clip空间中的x、y、z值是否在[-w,w]内来判断顶点是否在视椎内部
		//	https://www.notion.so/1433146ac7c0807592f1d60c2f10f0c3?pvs=4
		if (IsX && IsY && IsZ)
			return true;
		else
			return false;
	}
}