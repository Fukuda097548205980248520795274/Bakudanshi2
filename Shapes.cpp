#include "Structure.h"
#include "Matrix.h"
#include "Shapes.h"

/// <summary>
/// 矩形の頂点のベクトルを作る
/// </summary>
/// <returns>矩形の頂点のベクトルの構造体を返却する</returns>
BoxVertexVec MakeBoxVertexVec()
{
	// 矩形の頂点のベクトル
	BoxVertexVec boxVertexVec;

	boxVertexVec.leftTop = { -1.0f , 1.0f };
	boxVertexVec.rightTop = { 1.0f , 1.0f };
	boxVertexVec.leftBottom = { -1.0f , -1.0f };
	boxVertexVec.rightBottom = { 1.0f , -1.0f };

	return boxVertexVec;
}

/// <summary>
/// 矩形の当たり判定を行う
/// </summary>
/// <param name="shape1">図形1</param>
/// <param name="shape2">図形2</param>
/// <returns>矩形の当たり判定を行いtrueかfalseを返却する</returns>
int HitBox(Shape shape1, Shape shape2)
{
	if (shape1.translate.x + shape1.scale.x > shape2.translate.x - shape2.scale.x &&
		shape1.translate.x - shape1.scale.x < shape2.translate.x + shape2.scale.x)
	{
		if (shape1.translate.y + shape1.scale.y > shape2.translate.y - shape2.scale.y &&
			shape1.translate.y - shape1.scale.y < shape2.translate.y + shape2.scale.y)
		{
			return true;
		} else
		{
			return false;
		}
	} else
	{
		return false;
	}
}

/// <summary>
/// 大きさ、角度、平行移動を頂点の位置に変換する
/// </summary>
/// <param name="shape">図形</param>
/// <returns>頂点の位置の構造体を返却する</returns>
BoxVertexWorld VertexAffineMatrix(Shape shape)
{
	// 矩形の頂点の位置
	BoxVertexWorld world;

	// 矩形の頂点のベクトル
	BoxVertexVec vec = MakeBoxVertexVec();

	world.leftTop = Transform(vec.leftTop, MakeAffineMatrix(shape.scale, shape.theta, shape.translate));
	world.rightTop = Transform(vec.rightTop, MakeAffineMatrix(shape.scale, shape.theta, shape.translate));
	world.leftBottom = Transform(vec.leftBottom, MakeAffineMatrix(shape.scale, shape.theta, shape.translate));
	world.rightBottom = Transform(vec.rightBottom, MakeAffineMatrix(shape.scale, shape.theta, shape.translate));

	return world;
}

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>スクリーン座標を返却する</returns>
BoxVertexScreen CoordinateTransformation(BoxVertexWorld world)
{
	// スクリーン座標
	BoxVertexScreen screen;

	screen.leftTop = { world.leftTop.x , -world.leftTop.y + 670.0f };
	screen.rightTop = { world.rightTop.x , -world.rightTop.y + 670.0f };
	screen.leftBottom = { world.leftBottom.x , -world.leftBottom.y + 670.0f };
	screen.rightBottom = { world.rightBottom.x , -world.rightBottom.y + 670.0f };

	return screen;
}
