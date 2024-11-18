#pragma once
#include <eigen/dense>
#include <cmath>

using namespace Eigen;

#define PI 3.1415926
const float DEFAULT_CAMERA_FOVY = (45 / 180) * PI;
const Vector3f DEFAULT_CAMERA_POS{ 0, 0, 0 };
const Vector3f DEFAULT_CAMERA_GAZE{ 0, 0, -1.0f };
const Vector3f DEFAULT_CAMERA_TOP{ 0, 1.0f, 0 };
const Vector3f DEFAULT_CAMERA_X{ 1.0f, 0, 0 };
const Vector3f DEFAULT_CAMERA_Y{ 0, 1.0f, 0 };
const Vector3f DEFAULT_CAMERA_Z{ 0, 0, 1.0f };
const int DEFAULT_CAMERA_NEAR = 10;
const int DEFAULT_CAMERA_FAR = 20;

namespace LittleSGR {
	class Frustum {
	public:
		Frustum(float near, float far, float fovY, float aspect);
		~Frustum();

	private:
		float m_Near;
		float m_Far;
		float m_FovY;	//	Radians
		float m_Aspect;
		Matrix4f m_Mat;
	};

	class Camera {
	public:
		Camera(Frustum frustum, Vector3f gaze, Vector3f top);

		~Camera();

	private:
		Frustum* m_Frustum;
		Vector3f m_Gaze;
		Vector3f m_Top;
		Vector3f m_Left;
	};

	class VertexShader{
	public:
		VertexShader(int width, int height);
		~VertexShader();

		Matrix4f ModelScale(float sx, float sy, float sz);
		Matrix4f ModelScale();
		Matrix4f ModelTranslate(float tx, float ty, float tz);
		Matrix4f ModelTranslate();
		Matrix4f ModelRotate(float rx, float ry, float rz);
		Matrix4f ModelRotate();
		Matrix4f ModelTransMat();
		Matrix4f ViewTransMat(Vector3f target, Vector3f top, Vector3f pos);
		Matrix4f ViewTransMat(Vector3f CameraPos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ);
		Matrix4f ViewTransMat();
		Matrix4f ProjectTransMat();
		Matrix4f ProjectTransMat(float near, float far, float fovY, float aspect);

	public:
		int GetWidth() { return m_Height; }

	private:
		Matrix4f ModelRotateX(float angle);
		Matrix4f ModelRotateY(float angle);
		Matrix4f ModelRotateZ(float angle);

	private:
		int m_Width;
		int m_Height;
		Camera* m_Camera;
	};
	


}