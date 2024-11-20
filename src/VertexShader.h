#pragma once
#include <eigen/dense>
#include <cmath>

using namespace Eigen;

#define PI 3.1415926
const Vector3f DEFAULT_CAMERA_POS{ 0, 0, 0 };
const Vector3f DEFAULT_CAMERA_GAZE{ 0, 0, -1.0f };
const Vector3f DEFAULT_CAMERA_TOP{ 0, 1.0f, 0 };
const float DEFAULT_FRUSTUM_FOVY = (45 / 180) * PI;
const int DEFAULT_FRUSTUM_NEAR = 1;
const int DEFAULT_FRUSTUM_FAR = 10;

namespace LittleSGR {
	struct Frustum {
	public:
		Frustum(float aspect,	//	aspect = width/height , tan(foyY/2) = height/2|n|
			float near = DEFAULT_FRUSTUM_NEAR,
			float far = DEFAULT_FRUSTUM_FAR,
			float fovY = DEFAULT_FRUSTUM_FOVY)
			:FrustumNear(near), FrustumFar(far), FrustumFovY(fovY), FrustumAspect(aspect) {}

		float FrustumNear;
		float FrustumFar;
		float FrustumFovY;	//	Radians
		float FrustumAspect;
	};

	struct Camera {
	public:
		Camera();
		Camera(Vector3f target, Vector3f top, Vector3f pos);
		Camera(Vector3f pos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ);

		bool IsGazeMode;
		Vector3f CameraTarget;
		Vector3f CameraTop;
		Vector3f CameraPos;
		Vector3f CameraXAxis;
		Vector3f CameraYAxis;
		Vector3f CameraZAxis;
	};

	Matrix4f ModelScale(float sx, float sy, float sz);
	Matrix4f ModelTranslate(float tx, float ty, float tz);
	Matrix4f ModelRotateX(float angle);
	Matrix4f ModelRotateY(float angle);
	Matrix4f ModelRotateZ(float angle);
	Matrix4f ModelRotate(float rx, float ry, float rz);
	Matrix4f ViewTransMat(Vector3f CameraPos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ);
	Matrix4f ViewTransMat(Camera camera);
	Matrix4f ProjectTransMat(Frustum frustum);
}



