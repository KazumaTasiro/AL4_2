#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete objSkydome;
	delete objGround;
	delete objFighter;
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete camera;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	// カメラ生成
	camera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight, input);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 1, 0 });
	camera->SetDistance(3.0f);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();

	objRay = Object3d::Create();
	objRay2 = Object3d::Create();
	objSphere = Object3d::Create();
	objSphere2 = Object3d::Create();

	sphereRed = Object3d::Create();
	triangleObj = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");

	rayModel = Model::CreateFromOBJ("ray");
	rayModel2 = Model::CreateFromOBJ("ray2");
	sphereModel = Model::CreateFromOBJ("maru");
	sphereModel2 = Model::CreateFromOBJ("maru2");

	triangleModel = Model::CreateFromOBJ("triangle_mat");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);

	objRay->SetModel(rayModel);
	objRay2->SetModel(rayModel2);
	objSphere->SetModel(sphereModel);
	objSphere2->SetModel(sphereModel2);

	sphereRed->SetModel(sphereModel);
	triangleObj->SetModel(triangleModel);

	//球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1);//中心座標
	sphere.radius = 1.0f;//半径

	sphereWhite.center = XMVectorSet(2, 0, 0, 1);
	sphereWhite.radius = 1.0f;

	//平面の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0);//法線ベクトル
	plane.distance = 0.0f;//原点（0,0,0）からの距離

	//三角形の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, 0, -1.0f, 1);//左手前
	triangle.p1 = XMVectorSet(-1.0f, 0, 1.0f, 1);//左奥
	triangle.p2 = XMVectorSet(+1.0f, 0, -1.0f, 1);//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);//上向き

	//レイの初期値を設定
	ray.start = XMVectorSet(0, 1, 0, 1);//原点やや上
	ray.dir = XMVectorSet(0, -1, 0, 0);

	objGround->SetPosition({ 0,plane.distance,0 });
	sphereRed->SetPosition({ sphereWhite.center.m128_f32[0],sphereWhite.center.m128_f32[1] ,sphereWhite.center.m128_f32[2] });
}

void GameScene::Update()
{
	camera->Update();

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();

	objRay->Update();
	objRay2->Update();
	objSphere->Update();
	objSphere2->Update();

	sphereRed->Update();
	triangleObj->Update();

	debugText.Print("AD: move camera LeftRight", 50, 50, 1.0f);
	debugText.Print("WS: move camera UpDown", 50, 70, 1.0f);
	debugText.Print("ARROW: move camera FrontBack", 50, 90, 1.0f);
	hit = false;
	XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
	XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
	switch (Switch)
	{
	case GameScene::sphereVer:
		if (input->PushKey(DIK_2)) {
			Switch = rayPlane;
		}
		if (input->PushKey(DIK_3)) {
			Switch = raySphere;
		}
		if (input->PushKey(DIK_4)) {
			Switch = sphereTriangle;
		}
		if (input->PushKey(DIK_5)) {
			Switch = rayTriangle;
		}
		if (input->PushKey(DIK_6)) {
			Switch = sphereRayTriangle;
		}

		//球移動
		{
			XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
			if (input->PushKey(DIK_UP))
			{
				sphere.center += moveY;
			}
			else if (input->PushKey(DIK_DOWN)) { sphere.center -= moveY; }

			XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
			if (input->PushKey(DIK_RIGHT)) { sphere.center += moveX; }
			else if (input->PushKey(DIK_LEFT)) { sphere.center -= moveX; }
		}
		objSphere->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });
		objSphere2->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });
		break;
	case GameScene::rayPlane:
		if (input->PushKey(DIK_1)) {
			Switch = sphereVer;
		}
		if (input->PushKey(DIK_3)) {
			Switch = raySphere;
		}
		if (input->PushKey(DIK_4)) {
			Switch = sphereTriangle;
		}
		if (input->PushKey(DIK_5)) {
			Switch = rayTriangle;
		}
		if (input->PushKey(DIK_6)) {
			Switch = sphereRayTriangle;
		}
		//レイ操作
		
		if (input->PushKey(DIK_UP)) { ray.start += moveY; }
		else if (input->PushKey(DIK_DOWN)) { ray.start -= moveY; }

		
		if (input->PushKey(DIK_RIGHT)) { ray.start += moveX; }
		else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }

		objRay->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		objRay2->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });

		break;

	case GameScene::raySphere:
		if (input->PushKey(DIK_1)) {
			Switch = sphereVer;
		}
		if (input->PushKey(DIK_2)) {
			Switch = rayPlane;
		}
		if (input->PushKey(DIK_4)) {
			Switch = sphereTriangle;
		}
		if (input->PushKey(DIK_5)) {
			Switch = rayTriangle;
		}
		if (input->PushKey(DIK_6)) {
			Switch = sphereRayTriangle;
		}

		//レイ操作
		
		if (input->PushKey(DIK_UP)) { ray.start += moveY; }
		else if (input->PushKey(DIK_DOWN)) { ray.start -= moveY; }

		
		if (input->PushKey(DIK_RIGHT)) { ray.start += moveX; }
		else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }

		objRay->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		objRay2->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		break;
	case GameScene::sphereTriangle:
		if (input->PushKey(DIK_1)) {
			Switch = sphereVer;
		}
		if (input->PushKey(DIK_2)) {
			Switch = rayPlane;
		}
		if (input->PushKey(DIK_3)) {
			Switch = raySphere;
		}
		if (input->PushKey(DIK_5)) {
			Switch = rayTriangle;
		}
		if (input->PushKey(DIK_6)) {
			Switch = sphereRayTriangle;
		}
		//球移動
	{
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_UP))
		{
			sphere.center += moveY;
		}
		else if (input->PushKey(DIK_DOWN)) { sphere.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_RIGHT)) { sphere.center += moveX; }
		else if (input->PushKey(DIK_LEFT)) { sphere.center -= moveX; }
	}

	objSphere->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });
	objSphere2->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });
	break;
	case GameScene::rayTriangle:
		if (input->PushKey(DIK_1)) {
			Switch = sphereVer;
		}
		if (input->PushKey(DIK_2)) {
			Switch = rayPlane;
		}
		if (input->PushKey(DIK_3)) {
			Switch = raySphere;
		}
		if (input->PushKey(DIK_4)) {
			Switch = sphereTriangle;
		}
		if (input->PushKey(DIK_6)) {
			Switch = sphereRayTriangle;
		}
		//レイ操作

		if (input->PushKey(DIK_UP)) { ray.start += moveY; }
		else if (input->PushKey(DIK_DOWN)) { ray.start -= moveY; }


		if (input->PushKey(DIK_RIGHT)) { ray.start += moveX; }
		else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }

		objRay->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		objRay2->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		break;
	case GameScene::sphereRayTriangle:
		if (input->PushKey(DIK_1)) {
			Switch = sphereVer;
		}
		if (input->PushKey(DIK_2)) {
			Switch = rayPlane;
		}
		if (input->PushKey(DIK_3)) {
			Switch = raySphere;
		}
		if (input->PushKey(DIK_4)) {
			Switch = sphereTriangle;
		}
		if (input->PushKey(DIK_5)) {
			Switch = rayTriangle;
		}
		//レイ操作

		if (input->PushKey(DIK_UP)) { ray.start += moveY; }
		else if (input->PushKey(DIK_DOWN)) { ray.start -= moveY; }


		if (input->PushKey(DIK_RIGHT)) { ray.start += moveX; }
		else if (input->PushKey(DIK_LEFT)) { ray.start -= moveX; }

		objRay->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		objRay2->SetPosition({ ray.start.m128_f32[0],ray.start.m128_f32[1] ,ray.start.m128_f32[2] });
		//球移動
		{
			XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
			if (input->PushKey(DIK_W))
			{
				sphere.center += moveY;
			}
			else if (input->PushKey(DIK_S)) { sphere.center -= moveY; }

			XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
			if (input->PushKey(DIK_D)) { sphere.center += moveX; }
			else if (input->PushKey(DIK_A)) { sphere.center -= moveX; }
		}
		objSphere->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });
		objSphere2->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] });

	}


			//XMVECTOR inter;
		//bool hit = Collision::CheckSphere2Plane(sphere, plane, &inter);
		//if (hit) {
		//	debugText.Print("HIT", 50, 200, 1.0f);
		//	//stringstreamをリセットし、交点座標を埋め込む
		//	spherestr.str("");
		//	spherestr.clear();
		//	spherestr << "("
		//		<< std::fixed << std::setprecision(2)//小数点以下2まで
		//		<< sphere.center.m128_f32[0] << ","//x
		//		<< sphere.center.m128_f32[1] << ","//y
		//		<< sphere.center.m128_f32[2] << ")";//z
		//	debugText.Print(spherestr.str(), 50, 220, 1.0f);
		//}

	////stringstreamで変数の値を埋め込んで整形する
	//std::ostringstream spherestr;
	//spherestr << "Sphere:("
	//	<< std::fixed << std::setprecision(2)//小数点以下2まで
	//	<< sphere.center.m128_f32[0] << ","//x
	//	<< sphere.center.m128_f32[1] << ","//y
	//	<< sphere.center.m128_f32[2] << ")";//z

	//debugText.Print(spherestr.str(), 50, 180, 1.0f);



	////stringstreamで変数の値を埋め込んで整形する
	//std::ostringstream rayster;
	//rayster << "Lay.start:("
	//	<< std::fixed << std::setprecision(2)//小数点以下2まで
	//	<< ray.start.m128_f32[0] << ","//x
	//	<< ray.start.m128_f32[1] << ","//y
	//	<< ray.start.m128_f32[2] << ")";//z

	//debugText.Print(rayster.str(), 50, 180, 1.0f);



	////球と平面の当たり判定
	//XMVECTOR inter;
	//bool hit = Collision::CheckSphere2Triangle(sphere, triangle, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 200, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr << "("
	//		<< std::fixed << std::setprecision(2)//小数点以下2まで
	//		<< sphere.center.m128_f32[0] << ","//x
	//		<< sphere.center.m128_f32[1] << ","//y
	//		<< sphere.center.m128_f32[2] << ")";//z
	//	debugText.Print(spherestr.str(), 50, 220, 1.0f);
	//}

	////レイと平面の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::CheckRay2Plane(ray, plane, &distance,&inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 200, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr << "("
	//		<< std::fixed << std::setprecision(2)//小数点以下2まで
	//		<< inter.m128_f32[0] << ","//x
	//		<< inter.m128_f32[1] << ","//y
	//		<< inter.m128_f32[2] << ")";//z
	//	debugText.Print(spherestr.str(), 50, 280, 1.0f);
	//}

	////レイと三角形の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::CheckRay2Triangle(ray, triangle, &distance, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 200, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	rayster.str("");
	//	rayster.clear();
	//	rayster << "("
	//		<< std::fixed << std::setprecision(2)//小数点以下2まで
	//		<< inter.m128_f32[0] << ","//x
	//		<< inter.m128_f32[1] << ","//y
	//		<< inter.m128_f32[2] << ")";//z
	//	debugText.Print(rayster.str(), 50, 240, 1.0f);

	//	rayster.str("");
	//	rayster.clear();
	//	rayster << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";

	//	debugText.Print(rayster.str(), 50, 260, 1.0f);
	//}

	////レイと球の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::CheckRay2Sphere(ray, sphere, &distance, &inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 260, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	rayster.str("");
	//	rayster.clear();
	//	rayster << "("
	//		<< std::fixed << std::setprecision(2)//小数点以下2まで
	//		<< inter.m128_f32[0] << ","//x
	//		<< inter.m128_f32[1] << ","//y
	//		<< inter.m128_f32[2] << ")";//z
	//	debugText.Print(rayster.str(), 50, 280, 1.0f);

	//	rayster.str("");
	//	rayster.clear();
	//	rayster << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";

	//	debugText.Print(rayster.str(), 50, 300, 1.0f);
	//}

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);
	XMVECTOR inter;
	float distance;
	XMVECTOR inter2;
	float distance2;
	switch (Switch)
	{
	case GameScene::sphereVer:
		
	hit = Collision::CheckSphere2Plane(sphere, plane, &inter);
	if (hit) {
		objSphere2->Draw();
	}
	else
	{
		objSphere->Draw();
	}
	objGround->Draw();
		break;
	case GameScene::rayPlane:
		
		
		hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
		if (hit) {
			objRay2->Draw();
		}
		else
		{
			objRay->Draw();
		}
		objGround->Draw();
		break;

	case GameScene::raySphere:
		sphereRed->Draw();
		hit = Collision::CheckRay2Sphere(ray, sphereWhite, &distance, &inter);

		if (hit) {
			objRay2->Draw();
		}
		else
		{
			objRay->Draw();
		}
		break;
	case GameScene::sphereTriangle:
		triangleObj->Draw();

		hit = Collision::CheckSphere2Triangle(sphere, triangle, &inter);
		if (hit) {
			objSphere2->Draw();
		}
		else
		{
			objSphere->Draw();
		}
		break;

	case GameScene::rayTriangle:
		triangleObj->Draw();
		hit = Collision::CheckRay2Triangle(ray, triangle, &distance, &inter);
		if (hit) {
			objRay2->Draw();
		}
		else
		{
			objRay->Draw();
		}
		break;
	case GameScene::sphereRayTriangle:
		triangleObj->Draw();
		hit = Collision::CheckRay2Sphere(ray, sphere, &distance, &inter);

		if (hit) {
			objSphere2->Draw();
		}
		else
		{
			objSphere->Draw();
		}
		
		hit = Collision::CheckRay2Triangle(ray, triangle, &distance2, &inter2);
		if (hit) {
			objRay2->Draw();
		}
		else
		{
			objRay->Draw();
		}
		break;
	}

	/*objRay->Draw();*/
	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	
	//objFighter->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
