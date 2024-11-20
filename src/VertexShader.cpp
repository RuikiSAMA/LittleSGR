#include"VertexShader.h"

namespace LittleSGR {
	Camera::Camera() {
		CameraPos = DEFAULT_CAMERA_POS;
		CameraTop = DEFAULT_CAMERA_TOP;
		CameraTarget = DEFAULT_CAMERA_GAZE;
		IsGazeMode = true;
	}

	Camera::Camera(Vector3f target, Vector3f top, Vector3f pos)
		:CameraTarget(target), CameraTop(top), CameraPos(pos) {
		IsGazeMode = true;
	}

	Camera::Camera(Vector3f pos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ)
		: CameraPos(pos), CameraXAxis(CameraX), CameraYAxis(CameraY), CameraZAxis(CameraZ) {
		IsGazeMode = false;
	}

	Matrix4f ModelScale(float sx, float sy, float sz) {
		Matrix4f mat;
		mat <<
			sx, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, sz, 0,
			0, 0, 0, 1;
		return mat;
	}


	Matrix4f ModelTranslate(float tx, float ty, float tz) {
		Matrix4f mat;
		mat <<
			0, 0, 0, tx,
			0, 0, 0, ty,
			0, 0, 0, tz,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateX(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateY(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateZ(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotate(float rx, float ry, float rz) {
		return ModelRotateX(rx) * ModelRotateY(ry) * ModelRotateZ(rz);
	}

	Matrix4f ViewTransMat(Vector3f CameraPos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ) {
		Matrix4f mat;
		/*	M1 <<
			CameraX.x(), CameraY.x(), CameraZ.x(), 0,
			CameraX.y(), CameraY.y(), CameraZ.y(), 0,
			CameraX.z(), CameraY.z(), CameraZ.z(), 0,
			0, 0, 0, 1;

			M2<<
			0,0,0,-CameraPos.x(),
			0,0,0,-CameraPos.y(),
			0,0,0,-CameraPos.z(),
			0,0,0,1

			mat = M1.cross(M2)
		*/
		mat <<
			CameraX.x(), CameraY.x(), CameraZ.x(), -CameraPos.dot(CameraX),
			CameraX.y(), CameraY.y(), CameraZ.y(), -CameraPos.dot(CameraY),
			CameraX.z(), CameraY.z(), CameraZ.z(), -CameraPos.dot(CameraZ),
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ViewTransMat(Camera camera) {	//	用摄像机看的目标、摄像机的上方向 来定义摄像机的 x、y、z 轴
		const Vector3f zAxis = -(camera.CameraTarget - camera.CameraPos).normalized();	//	-gaze
		const Vector3f yAxis = (camera.CameraTop).normalized();
		const Vector3f xAxis = (-zAxis.cross(camera.CameraTop)).normalized();	//	right = gaze.cross(CameraTop)
		if (camera.IsGazeMode)
			return ViewTransMat(camera.CameraPos, xAxis, yAxis, zAxis);
		else
			return ViewTransMat(camera.CameraPos, camera.CameraXAxis, camera.CameraYAxis, camera.CameraZAxis);
	}

	Matrix4f ProjectTransMat(Frustum frustum) {
		Matrix4f Projection_Trans;
		Projection_Trans <<
			1 / (frustum.FrustumAspect * tan(frustum.FrustumFovY / 2)), 0, 0, 0,
			0, 1 / tan(frustum.FrustumFovY / 2), 0, 0,
			0, 0, (frustum.FrustumNear + frustum.FrustumFar) / (frustum.FrustumNear - frustum.FrustumFar), 2 * frustum.FrustumNear * frustum.FrustumFar / frustum.FrustumFar - frustum.FrustumNear,
			0, 0, -1, 0;
		return Projection_Trans;
	}
}
