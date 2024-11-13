#pragma once
#include <eigen/dense>
#include <cmath>
using namespace Eigen;

namespace LittleSGR {
	Matrix4f ModelScale(float sx, float sy, float, sz) {
		Matrix4f mat;
		mat <<
			sx, 0, 0, 0 <<
			0, sy, 0, 0 <<
			0, 0, sz, 0 <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelTranslate(float tx, float ty, float, tz) {
		Matrix4f mat;
		mat <<
			0, 0, 0, tx <<
			0, 0, 0, ty <<
			0, 0, 0, tz <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateX(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			1, 0, 0, 0 <<
			0, c, -s, 0 <<
			0, s, c, 0 <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateY(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			c, 0, s, 0 <<
			0, 1, 0, 0 <<
			-s, 0, c, 0 <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ModelRotateZ(float angle) {
		const float c = (float)cos(angle);
		const float s = (float)sin(angle);
		Matrix4f mat;
		mat <<
			c, -s, 0, 0 <<
			s, c, 0, 0 <<
			0, 0, 1, 0 <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ViewTransformation(Vector3f target, Vector3f top, Vector3f pos) {
		Vector3f gare = (target - pos).normalized;
		Vector3f left = Cross(top, gare).normalized;
		Matrix4f mat;
		mat <<
			left.x(), left.y(), left.z(), pos.x() <<
			top.x(), top.y(), top.z(), pos.y() <<
			-gare.x(), -gare.y(), -gare.z(), pos.z() <<
			0, 0, 0, 1;
		return mat;
	}

	Matrix4f ViewTransformation(Vector3f worldO, Vector3f worldX, Vector3f worldY, Vector3f worldZ) {
		Matrix4f mat;
		mat <<
			worldX.x(), worldY.x(), worldZ.x(), -worldO * worldX <<
			worldX.y(), worldY.y(), worldZ.y(), -worldO * worldY <<
			worldX.z(), worldY.z(), worldZ.z(), -worldO * worldZ <<
			0, 0, 0, 1;
		return mat;
	}

}