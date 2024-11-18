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
	struct Vertex
	{
		Vertex(){}
		Vertex(Vector3f pos, Vector2f uv, Vector3f color, Vector3f normal)
			:VertexPos(pos), VertexTexCoor(uv), VertexColor(color), VertexNormal(normal){}
		Vector3f VertexPos;
		Vector2f VertexTexCoor;
		Vector3f VertexColor;
		Vector3f VertexNormal;
	};

	struct Triangle
	{
		Triangle(){}

		Vertex VertexData[3];

		Vertex& operator[](size_t i){ return VertexData[i]; }	//	重载运算符以使用[]访问三角形中的顶点元素
		const Vertex& operator[](size_t i) const { return VertexData[i]; }

		Vector3f GetTriangleNormal() const{
			const Vector3f V1 = VertexData[1].VertexPos - VertexData[0].VertexPos;
			const Vector3f V2 = VertexData[2].VertexPos - VertexData[1].VertexPos;
			return V1.cross(V2);
		}
	};

	class Renderer {
	public:
		Renderer(int width, int height);
		~Renderer();

		void DrawLine(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color);

		Matrix4f VertexShaderMat();

		static void Draw(FrameBuffer framebuffer, Triangle tri);

	private:
		void DrawLine_WithoutClip(Vector2i v0, Vector2i v1, FrameBuffer* fb, Vector3f color);
		bool ClipLine(Vector2i& v0, Vector2i& v1);
		int ComputeOutCode(Vector2i v);

	private:
		int m_Width;
		int m_Height;
		VertexShader* m_VertexShader;
	};


}