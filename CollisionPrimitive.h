#pragma once
#pragma once
/// <summary>
///当たり判定プリミティブ
/// </summary>


#include<DirectXMath.h>

struct Sphere {
	DirectX::XMVECTOR center = { 0,0,0,1 };

	float radius = 1.0f;

};

struct Plane {
	DirectX::XMVECTOR normal = { 0,1,0,0 };

	float distance = 0.0f;

};

