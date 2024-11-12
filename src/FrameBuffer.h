#pragma once
#include<Eigen/Dense>

using namespace Eigen;

namespace LittleSGR {
	class FrameBuffer {
	public:
		FrameBuffer(const int width, const int height);
		~FrameBuffer();
		void SetColorbuffer(const int x, const int y, const Vector3d rgb);
		Vector3d GetColorbuffer(const int x, const int y) const;
		void SetZBuffer(const int x, const int y, const float z);
		float GetZBuffer(const int x, const int y) const;
		int GetWidth();
		int GetHeight();

	private:
		int GetPixelIndex(const int x, const int y) const;
		void ClearColorBuffer();
		void ClearZBuffer();

	private:
		int m_Width;
		int m_Height;
		int m_Size;
		Vector3d* m_ColorBuffer;
		float* m_ZBuffer;
	};
}