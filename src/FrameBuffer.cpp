#include "FrameBuffer.h"
#include "Base.h"
#include <iostream>
using namespace Eigen;

#define COLOR_BLACK 0
#define DEFAULT_Z 1

namespace LittleSGR {

	FrameBuffer::FrameBuffer(const int width, const int height)
		:m_Width(width), m_Height(height) {
		m_Size = m_Width * m_Height;
		m_ColorBuffer = new Vector3f[m_Size];
		m_ZBuffer = new float[m_Size];
		ClearColorBuffer();
		ClearZBuffer();
	}

	FrameBuffer::FrameBuffer(const FrameBuffer& other) 	//	谨防浅拷贝！
		: m_Width(other.m_Width),
		m_Height(other.m_Height),
		m_Size(other.m_Size),
		m_ColorBuffer(new Eigen::Vector3f[m_Size]),
		m_ZBuffer(new float[m_Size]) {
		std::memcpy(m_ColorBuffer, other.m_ColorBuffer, m_Size * sizeof(Eigen::Vector3f));	// 深拷贝
		std::memcpy(m_ZBuffer, other.m_ZBuffer, m_Size * sizeof(float));
	}

	FrameBuffer::~FrameBuffer() {
		delete[] m_ColorBuffer;
		delete[] m_ZBuffer;
		m_ColorBuffer = nullptr;
		m_ZBuffer = nullptr;
	}
	void FrameBuffer::SetColorbuffer(const int x, const int y, const Vector3f rgb) {
		int i = GetPixelIndex(x, y);
		std::cout << x << " , " << y << std::endl;
		if ((x < 0) || (x >= m_Width) || (y < 0) || (y >= m_Height)) {
			//ASSERT(0);
		}
		else{
			m_ColorBuffer[i] = rgb;
		}
	}

	Vector3f FrameBuffer::GetColorbuffer(const int x, const int y) const {
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

	int FrameBuffer::GetPixelIndex(const int x, const int y) const {
		return (y * m_Width + x);
	}

	void FrameBuffer::ClearColorBuffer() {
		for (int i = 0; i < m_Size; ++i) {
			m_ColorBuffer[i] = Vector3f(0, 0, 0);
		}
	}

	void FrameBuffer::ClearZBuffer() {
		memset(m_ZBuffer, DEFAULT_Z, m_Size);
	}

	int FrameBuffer::GetHeight() const {
		return m_Height;
	}

	int FrameBuffer::GetWidth() const {
		return m_Width;
	}
}
