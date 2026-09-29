#include <Novice.h>
#include <imgui.h>
#include <numbers>
#include <cmath>

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float m[4][4];
};

struct Spherical
{
	float radius;
	float theta;
	float phi;
};

// ベクトル演算ヘルパー関数
Vector3 Sub(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

float Length(const Vector3& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	if (len != 0.0f) {
		return { v.x / len, v.y / len, v.z / len };
	}
	return { 0.0f, 0.0f, 0.0f };
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Vector3 ToCartesian(const Spherical& s) {
	float rho = s.radius * std::cos(s.theta);
	return {
		rho * std::cos(s.phi),
		s.radius * std::sin(s.theta),
		rho * std::sin(s.phi)
	};
}

const char kWindowTitle[] = "LC1C_12_ショウ_ズーウェン";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 球面座標の初期値
	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Spherical s{ 6.0f, 0.0f, -halfPi };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 球面座標から直交座標（カメラ位置）を計算
		Vector3 pos = ToCartesian(s);

		// 注視点（原点）を向くカメラのワールド行列を作成
		Vector3 target{ 0.0f, 0.0f, 0.0f };
		Vector3 worldUp{ 0.0f, 1.0f, 0.0f };

		// 注視点への前 F を求める
		Vector3 forward = Normalize(Sub(target, pos));

		// 世界の上と前 F の外積で右 R を求める
		Vector3 right = Normalize(Cross(worldUp, forward));

		// 前 F と右 R の外積で上 U を求める
		Vector3 up = Cross(forward, right);

		// 求めた右・上・前とカメラ位置を行列に格納
		Matrix4x4 cameraMatrix = {
			right.x,   right.y,   right.z,   0.0f,
			up.x,      up.y,      up.z,      0.0f,
			forward.x, forward.y, forward.z, 0.0f,
			pos.x,     pos.y,     pos.z,     1.0f
		};

		// ImGui で球面座標を操作し、直交座標とカメラ行列を表示する
		ImGui::Begin("Spherical Coordinates");
		ImGui::Text("Target: (0, 0, 0) / +Y up / Camera +Z forward");
		ImGui::Separator();

		ImGui::InputFloat("Radius", &s.radius, 0.01f);
		ImGui::InputFloat("Theta: elevation (rad)", &s.theta, 0.01f);
		ImGui::InputFloat("Phi (rad)", &s.phi, 0.01f);

		ImGui::Separator();
		ImGui::Text("Spherical: r = %.3f, theta = %.3f rad, phi = %.3f rad", s.radius, s.theta, s.phi);
		ImGui::Text("Cartesian: x = %.3f, y = %.3f, z = %.3f", pos.x, pos.y, pos.z);

		ImGui::Separator();
		ImGui::Text("Camera matrix");
		ImGui::Text("%8.3f %8.3f %8.3f %8.3f", cameraMatrix.m[0][0], cameraMatrix.m[0][1], cameraMatrix.m[0][2], cameraMatrix.m[0][3]);
		ImGui::Text("%8.3f %8.3f %8.3f %8.3f", cameraMatrix.m[1][0], cameraMatrix.m[1][1], cameraMatrix.m[1][2], cameraMatrix.m[1][3]);
		ImGui::Text("%8.3f %8.3f %8.3f %8.3f", cameraMatrix.m[2][0], cameraMatrix.m[2][1], cameraMatrix.m[2][2], cameraMatrix.m[2][3]);
		ImGui::Text("%8.3f %8.3f %8.3f %8.3f", cameraMatrix.m[3][0], cameraMatrix.m[3][1], cameraMatrix.m[3][2], cameraMatrix.m[3][3]);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}