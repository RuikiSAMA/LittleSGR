#pragma once
#include<iostream>
#include<eigen/dense>
#include<cmath>
#include<vector>
#include <algorithm> 
#include "VertexShader.h"
#include "Framebuffer.h"

#define LEFT 0b0001
#define INSIDE 0b0000
#define RIGHT 0b0010
#define BOTTOM 0b0100
#define TOP 0b1000
const float epsilon = 1e-6;

using namespace Eigen;

namespace LittleSGR {
	struct Vertex	//	模型输出到顶点着色器的顶点
	{
		Vertex() {}
		Vertex(Vector4f pos, Vector2f uv = { 0, 0 })
			: ModelPos(pos), TexCoor(uv) {}
		Vector4f ModelPos;
		Vector2f TexCoor;
	};

	struct Varing	//	顶点着色器向片元着色器输出的顶点
	{
		Varing() {}
		Varing(Vector4f pos, Vector2f uv = { 0, 0 })
			: ClipPos(pos), TexCoor(uv) {}
		Vector4f ClipPos;
		Vector4f NdcPos;
		Vector4f FragPos;
		Vector2f TexCoor;
	};

	struct Uniform
	{
		Matrix4f MVP;
	};

	struct VertexTriangle
	{
		VertexTriangle() {}

		Vertex VertexData[3];

		Vertex& operator[](size_t i) { return VertexData[i]; }	//	重载运算符以使用[]访问三角形中的顶点元素
		const Vertex& operator[](size_t i) const { return VertexData[i]; }
	};

	struct VaringTriangle
	{
		VaringTriangle() {}

		Varing VaringData[3];

		Varing& operator[](size_t i) { return VaringData[i]; }
		const Varing& operator[](size_t i) const { return VaringData[i]; }
	};

	struct BoundingBox
	{
		BoundingBox() {};
		BoundingBox(int minX, int minY, int maxX, int maxY)
			:MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY){}

		int MinX, MinY, MaxX, MaxY;
	};

	class Renderer {
	private:
		enum class Plane {
			POSITIVE_W,	//	x、y轴所形成的平面
			POSITIVE_X,
			POSITIVE_Y,
			Z_FAR,
			NEGATIVE_X,
			NEGATIVE_Y,
			Z_NEAR,
		};

	public:
		Renderer();
		~Renderer();

		static void Draw(FrameBuffer& framebuffer, VertexTriangle triangle, Uniform unifrom);
		static bool FrustumCull(Varing* varing);	//	视椎剔除

		static void init(int width, int height, Frustum frustum, Camera camera, float aspect);

		static void Rasterization(VaringTriangle vTriangle, FrameBuffer& framebuffer);

		static BoundingBox GetBoundingBox(const VaringTriangle vTriangle, const int width, const int height);
		static float Clamp(float val, float min, float max);

	private:
		static void VertexShader(const Vertex vertex, Varing& varings, Uniform uniforms, FrameBuffer fb);	//	执行顶点着色器的函数
		static void AddNdcPos(Varing* varing, int vertexNum);	//	执行透视除法
		static void AddFragPos(Varing* varing, int vertexNum, int width, int height);	//	执行视口变换

	private:	//	视口裁剪、视口裁剪后的画线方法
		static void DrawLine(Vector2i v0, Vector2i v1, FrameBuffer& fb, Vector3f color);
		static void DrawLine_WithoutClip(Vector2i v0, Vector2i v1, FrameBuffer& fb, Vector3f color);
		static int ComputeOutCode(Vector2i v);
		static bool ClipLine(Vector2i& v0, Vector2i& v1);

	private:
		//	在Clip空间中对三角形进行裁剪，并在被裁掉的边上添加新顶点的方法，入口为 ClipVertex()
		//	Clip空间裁剪教程 https://zhuanlan.zhihu.com/p/162190576

		static int ClipVertex(Varing* varings);
		//	对不在视椎的顶点进行裁剪，返回裁剪后一个多边形顶点的数量

		static bool IsInFrustum(Varing varing);
		//	判断点是否在视椎内

		static bool IsInPlane(Varing varing, Plane plane);
		//	判断点是否在平面里侧

		static int ClippingPlane(const Varing* invarings, Varing* outvarings, Plane plane, int vertexnum);
		//	用平面对点进行裁剪，输出 顶点数目VertexNum、裁剪后的顶点数outVarings(包括裁剪新生成的顶点)

		static float IntersectRatio(Varing v0, Varing v1, Plane plane);
		//	获取交点对两顶点的线性差值比例

		static Varing VaringLerp(Varing v0, Varing v1, float ratio, Plane plane);
		//	获取交点

	private:
		static int m_Width;
		static int m_Height;

		static Frustum m_Frustum;
		static Camera m_Camera;
		static float m_Aspect;
	};
}