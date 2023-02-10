﻿#pragma once

#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include <DirectXMath.h>
#include "CollisionPrimitive.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
  private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

  private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* spriteBG = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;

	Model* rayModel = nullptr;
	Model* rayModel2 = nullptr;
	Model* sphereModel = nullptr;
	Model* sphereModel2 = nullptr;
	Model* triangleModel = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;

	Object3d* objSphere = nullptr;
	Object3d* objSphere2 = nullptr;
	Object3d* objRay = nullptr;
	Object3d* objRay2 = nullptr;

	Object3d* sphereRed = nullptr;
	Object3d* triangleObj = nullptr;

	//当たり判定　球
	Sphere sphere;

	Sphere sphereWhite;
	//当たり判定　平面
	Plane plane;
	//当たり判定　三角形
	Triangle triangle;
	//当たり判定　レイ
	Ray ray;

	bool hit = false;

	enum CollisionSwitch
	{
		sphereVer,
		rayPlane,
		raySphere,
		sphereTriangle,
		rayTriangle

	};

	CollisionSwitch Switch = CollisionSwitch::sphereVer;
};
