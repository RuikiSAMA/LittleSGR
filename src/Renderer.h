#pragma once
#include<eigen/dense>
#include<cmath>
#include "VertexShader.h"
#include "Framebuffer.h"

#define LEFT 0b0001
#define INSIDE 0b0000
#define RIGHT 0b0010
#define BOTTOM 0b0100
#define TOP 0b1000

using namespace Eigen;

namespace LittleSGR {
	struct Vertex	//	模型输出到顶点着色器的顶点
	{
		Vertex(){}
		Vertex(Vector3f pos, Vector2f uv = {0, 0})
			: ModelPos(pos), TexCoor(uv){}
		Vector3f ModelPos;
		Vector2f TexCoor;
	};

	struct Varing	//	顶点着色器向片元着色器输出的顶点
	{
		Varing(){}
		Varing(Vector3f pos, Vector2f uv = { 0, 0 })
			: ClipPos(pos), TexCoor(uv) {}
		Vector3f ClipPos;
		Vector3f NdcPos;
		Vector3f FragPos;
		Vector2f TexCoor;
	};

	struct Uniform
	{
		Matrix4f MVP;
	};

	struct Triangle
	{
		Triangle(){}

		Vertex VertexData[3];

		Vertex& operator[](size_t i){ return VertexData[i]; }	//	重载运算符以使用[]访问三角形中的顶点元素
		const Vertex& operator[](size_t i) const { return VertexData[i]; }
	};

	class Renderer {
	public:
		Renderer(int width, int height);
		~Renderer();

		void DrawLine(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color);

		void VertexShader(Varing &varings, const Vertex vertexs, const Uniform uniforms);

		static void Draw(FrameBuffer framebuffer, Triangle triangle, Uniform unifrom);

	private:
		void DrawLine_WithoutClip(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color);
		bool ClipLine(Vector2i& v0, Vector2i& v1);
		int ComputeOutCode(Vector2i v);

	private:
		int m_Width;
		int m_Height;
	};


}