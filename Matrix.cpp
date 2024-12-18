#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include "Structure.h"
#include "Matrix.h"

/// <summary>
/// 行列の積を求める
/// </summary>
/// <param name="matrix1">行列1</param>
/// <param name="matrix2">行列2</param>
/// <returns>行列の積を求めた構造体を返却する</returns>
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2)
{
	// 行列の積
	Matrix3x3 multiplyMatrix;

	multiplyMatrix.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0];
	multiplyMatrix.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1];
	multiplyMatrix.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2];

	multiplyMatrix.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0];
	multiplyMatrix.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1];
	multiplyMatrix.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2];

	multiplyMatrix.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0];
	multiplyMatrix.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1];
	multiplyMatrix.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2];

	return multiplyMatrix;
}

/// <summary>
/// ベクトルと行列の積を求める
/// </summary>
/// <param name="vec">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>ベクトルと行列の積を求めた構造体を返却する</returns>
Coordinate2 Transform(Vec2 vec, Matrix3x3 matrix)
{
	// ベクトルと行列の積
	Coordinate2 transform;

	transform.x = vec.x * matrix.m[0][0] + vec.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	transform.y = vec.x * matrix.m[0][1] + vec.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vec.x * matrix.m[0][2] + vec.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];

	assert(w != 0.0f);

	transform.x /= w;
	transform.y /= w;

	return transform;
}

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="sclae">大きさ</param>
/// <returns>拡大縮小行列を作り、返却する</returns>
Matrix3x3 MakeScaleMatrix(Vec2 scale)
{
	// 拡大縮小行列
	Matrix3x3 scaleMatrix;

	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[0][1] = 0.0f;
	scaleMatrix.m[0][2] = 0.0f;

	scaleMatrix.m[1][0] = 0.0f;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[1][2] = 0.0f;

	scaleMatrix.m[2][0] = 0.0f;
	scaleMatrix.m[2][1] = 0.0f;
	scaleMatrix.m[2][2] = 1.0f;

	return scaleMatrix;
}

/// <summary>
/// 回転行列を作る
/// </summary>
/// <param name="theta">角度</param>
/// <returns>回転行列を作り、返却する</returns>
Matrix3x3 MakeRotateMatrix(float theta)
{
	// 回転行列
	Matrix3x3 rotateMatrix;

	rotateMatrix.m[0][0] = cosf((theta / 180.0f) * float(M_PI));
	rotateMatrix.m[0][1] = sinf((theta / 180.0f) * float(M_PI));
	rotateMatrix.m[0][2] = 0.0f;

	rotateMatrix.m[1][0] = -sinf((theta / 180.0f) * float(M_PI));
	rotateMatrix.m[1][1] = cosf((theta / 180.0f) * float(M_PI));
	rotateMatrix.m[1][2] = 0.0f;

	rotateMatrix.m[2][0] = 0.0f;
	rotateMatrix.m[2][1] = 0.0f;
	rotateMatrix.m[2][2] = 1.0f;

	return rotateMatrix;
}

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translate">平行移動</param>
/// <returns>平行移動行列を作り、返却する</returns>
Matrix3x3 MakeTranslateMatrix(Vec2 translate)
{
	// 平行移動行列
	Matrix3x3 translateMatrix;

	translateMatrix.m[0][0] = 1.0f;
	translateMatrix.m[0][1] = 0.0f;
	translateMatrix.m[0][2] = 0.0f;

	translateMatrix.m[1][0] = 0.0f;
	translateMatrix.m[1][1] = 1.0f;
	translateMatrix.m[1][2] = 0.0f;

	translateMatrix.m[2][0] = translate.x;
	translateMatrix.m[2][1] = translate.y;
	translateMatrix.m[2][2] = 1.0f;

	return translateMatrix;
}

/// <summary>
/// アフィン行列を作る
/// </summary>
/// <param name="scale">大きさ</param>
/// <param name="theta">角度</param>
/// <param name="translate">平行移動</param>
/// <returns>アフィン行列を作り、返却する</returns>
Matrix3x3 MakeAffineMatrix(Vec2 scale, float theta, Vec2 translate)
{
	// アフィン行列
	Matrix3x3 affineMatrix = Multiply(MakeScaleMatrix(scale), MakeRotateMatrix(theta));
	affineMatrix = Multiply(affineMatrix, MakeTranslateMatrix(translate));

	return affineMatrix;
}