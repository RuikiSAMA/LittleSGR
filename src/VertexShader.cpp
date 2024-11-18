#include "VertexShader.h"

namespace LittleSGR {
	VertexShader::VertexShader(int width, int height) 
		:m_Width(width), m_Height(height) {
		float aspect = width / height;
		Frustum frustum(DEFAULT_CAMERA_NEAR, DEFAULT_CAMERA_FAR, DEFAULT_CAMERA_FOVY, aspect);
		m_Camera = new Camera(frustum, DEFAULT_CAMERA_GAZE, DEFAULT_CAMERA_TOP);
	}

	VertexShader::~VertexShader() {
	
	}

	Matrix4f VertexShader::ModelScale(float sx, float sy, float sz) {
		Matrix4f mat;
		mat <<
			sx, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, sz, 0,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f VertexShader::ModelScale() {
		return Matrix4f::Identity();
	}

	Matrix4f VertexShader::ModelTranslate(float tx, float ty, float tz) {
		Matrix4f mat;
		mat <<
			0, 0, 0, tx,
			0, 0, 0, ty,
			0, 0, 0, tz,
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f VertexShader::ModelTranslate() {
		return Matrix4f::Identity();
	}

	Matrix4f VertexShader::ModelRotate(float rx, float ry, float rz) {
		return ModelRotateX(rx) * ModelRotateY(ry) * ModelRotateZ(rz);
	}

	Matrix4f VertexShader::ModelRotate() {
		return Matrix4f::Identity();
	}

	Matrix4f VertexShader::ModelRotateX(float angle) {
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

	Matrix4f VertexShader::ModelRotateY(float angle) {
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

	Matrix4f VertexShader::ModelRotateZ(float angle) {
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

	Matrix4f VertexShader::ModelTransMat() {
		return ModelTranslate() * ModelRotate() * ModelScale();
	}

	Matrix4f VertexShader::ViewTransMat(Vector3f target, Vector3f top, Vector3f pos) {	//	用摄像机看的目标、摄像机的上方向 来定义摄像机的 x、y、z 轴
		const Vector3f zAxis = -(target - pos).normalized();	//	-gaze
		const Vector3f yAxis = top.normalized();
		const Vector3f xAxis = (-zAxis.cross(top)).normalized();	//	right = gaze.cross(top)

		return ViewTransMat(pos, xAxis, yAxis, zAxis);
	}

	Matrix4f VertexShader::ViewTransMat(Vector3f CameraPos, Vector3f CameraX, Vector3f CameraY, Vector3f CameraZ) {
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

	Matrix4f VertexShader::ViewTransMat() {
		return ViewTransMat(DEFAULT_CAMERA_POS, DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z);
	}

	Matrix4f VertexShader::ProjectTransMat(float near, float far, float fovY, float aspect) {
		Matrix4f Projection_Trans;
		Projection_Trans <<
			1 / (aspect * tan(fovY / 2)), 0, 0, 0,
			0, 1 / tan(fovY / 2), 0, 0,
			0, 0, (near + far) / (near - far), 2 * near * far / far - near,
			0, 0, -1, 0;
		return Projection_Trans;
	}

	Matrix4f VertexShader::ProjectTransMat() {
		float aspect = m_Width / m_Height;
		return ProjectTransMat(DEFAULT_CAMERA_NEAR, DEFAULT_CAMERA_FAR, DEFAULT_CAMERA_FOVY, aspect);
	}
}