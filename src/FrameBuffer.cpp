#include "FrameBuffer.h"
#include "Base.h"
using namespace Eigen;

#define COLOR_BLACK 0
#define DEFAULT_Z 1

namespace LittleSGR {
	FrameBuffer::FrameBuffer(const int width, const int height)
		:m_Width(width), m_Height(height){
		m_Size = m_Width * m_Height;
		m_ColorBuffer = new Vector3d[m_Size];
		m_ZBuffer = new float[m_Size];
		ClearColorBuffer();
		ClearZBuffer();
	}

	FrameBuffer::~FrameBuffer() {
		delete[] m_ColorBuffer;
		m_ColorBuffer = nullptr;
		delete[] m_ZBuffer;
		m_ZBuffer = nullptr;
	}

	void FrameBuffer::SetColorbuffer(const int x, const int y, const Vector3d rgb) {
		int i = GetPixelIndex(x, y);
		if (i >= 0 && i < m_Size)
			m_ColorBuffer[i] = rgb;
		else
			ASSERT(0);
	}

	Vector3d FrameBuffer::GetColorbuffer(const int x, const int y) const {
		int i = GetPixelIndex(x, y);
		if (i >= 0 && i < m_Size)
			return m_ColorBuffer[i];
		else
			ASSERT(0);
	}

	void FrameBuffer::SetZBuffer(const int x, const int y, const float z) {
		int i = GetPixelIndex(x, y);
		if (i >= 0 && i < m_Size)
			m_ZBuffer[i] = z;
		else
			ASSERT(0);
	}

	float FrameBuffer::GetZBuffer(const int x, const int y) const {
		int i = GetPixelIndex(x, y);
		if (i >= 0 && i < m_Size)
			return m_ZBuffer[i];
		else
			ASSERT(0);
	}

	int FrameBuffer::GetHeight() {
		return m_Height;
	}

	int FrameBuffer::GetWidth() {
		return m_Width;
	}

	int FrameBuffer::GetPixelIndex(const int x, const int y) const {
		return (y * m_Width + x);
	}

	void FrameBuffer::ClearColorBuffer() {
		memset(m_ColorBuffer, COLOR_BLACK, m_Size);
	}

	void FrameBuffer::ClearZBuffer() {
		memset(m_ZBuffer, DEFAULT_Z, m_Size);
	}
}