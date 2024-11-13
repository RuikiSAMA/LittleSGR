#pragma once
#include<Eigen/Dense>

using namespace Eigen;

namespace LittleSGR {
	class FrameBuffer {
	public:
		FrameBuffer(const int width, const int height);
		FrameBuffer(const FrameBuffer& thant);
		~FrameBuffer();
		void SetColorbuffer(const int x, const int y, const Vector3f rgb);
		void SetZBuffer(const int x, const int y, const float z);
		Vector3f GetColorbuffer(const int x, const int y) const;
		float GetZBuffer(const int x, const int y) const;
		int GetWidth() const;
		int GetHeight() const;

	private:
		int GetPixelIndex(const int x, const int y) const;
		void ClearColorBuffer();
		void ClearZBuffer();

	private:
		int m_Width;
		int m_Height;
		int m_Size;
		Vector3f* m_ColorBuffer;
		float* m_ZBuffer;
	};
}