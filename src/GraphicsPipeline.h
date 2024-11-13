#pragma once
#include "Framebuffer.h"
#include <eigen/dense>

using namespace Eigen;

namespace LittleSGR {
	class GraphicsPipeline {
	public:
		GraphicsPipeline(int width, int height);
		~GraphicsPipeline();
		void DrawLine(Vector2i v1, Vector2i v2, FrameBuffer* fb);
	private:
		const int m_Width;
		const int m_Height;
	};
}
