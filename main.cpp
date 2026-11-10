#include <Novice.h>
#include <imgui.h>
#include <cmath>
#include <algorithm>

// ----------------------------------------------------------------
// 構造体定義
// ----------------------------------------------------------------
struct Vector2 {
	float x;
	float y;
};

// ----------------------------------------------------------------
// 演算子オーバーロード
// ----------------------------------------------------------------
Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}

Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}

Vector2 operator*(const Vector2& v, float scalar) {
	return { v.x * scalar, v.y * scalar };
}

Vector2 operator*(float scalar, const Vector2& v) {
	return { v.x * scalar, v.y * scalar };
}

// ----------------------------------------------------------------
// 関数定義
// ----------------------------------------------------------------

// 台の形 f(x) (位置を渡すと高さを返す)
float f(float x) {
	return 0.125f * x * x * x + 0.25f * x * x;
}

// 導関数 f'(x) (位置を渡すと傾きを返す)
// f(x) = (1/8)x^3 + (1/4)x^2 を微分 -> f'(x) = (3/8)x^2 + (1/2)x = 0.375x^2 + 0.5x
float fPrime(float x) {
	return 0.375f * x * x + 0.5f * x;
}

// ベクトルの正規化
Vector2 Normalize(const Vector2& v) {
	float length = std::sqrtf(v.x * v.x + v.y * v.y);
	if (length != 0.0f) {
		return { v.x / length, v.y / length };
	}
	return { 0.0f, 0.0f };
}

// ワールド座標からスクリーン座標への変換
Vector2 WorldToScreen(const Vector2& worldPos) {
	return {
		400.0f + 240.0f * worldPos.x,
		640.0f - 240.0f * worldPos.y
	};
}

// ----------------------------------------------------------------
// 定数定義
// ----------------------------------------------------------------
const char kWindowTitle[] = "LE1C_12_ショウ_ズーウェン";
const float kMoveSpeed = 1.0f;          // A/Dで動く速さ (m/s)
const float kDeltaTime = 1.0f / 60.0f;  // デルタタイム
const float kBodyHalfLength = 0.15f;    // 車体の長さの半分 (m)
const int kWheelRadius = 10;            // 車輪の半径 (px)
const int kSegments = 200;              // 台の折れ線の分割数

// ----------------------------------------------------------------
// エントリーポイント
// ----------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力準備
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// バイクの初期位置 (x)
	float x = 1.0f;

	// メインループ
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// A/Dキーでの移動制御
		if (keys[DIK_D]) {
			x += kMoveSpeed * kDeltaTime;
		}
		if (keys[DIK_A]) {
			x -= kMoveSpeed * kDeltaTime;
		}

		// 0 <= x <= 2 の範囲に限定
		x = std::clamp(x, 0.0f, 2.0f);

		// 位置と傾き・方向の計算
		Vector2 position{ x, f(x) };
		float slope = fPrime(x);
		Vector2 forward = Normalize({ 1.0f, slope });

		// 車輪の位置（ワールド座標）
		Vector2 backWorld = position - forward * kBodyHalfLength;
		Vector2 frontWorld = position + forward * kBodyHalfLength;

		// 車輪の位置（スクリーン座標）
		Vector2 screenBack = WorldToScreen(backWorld);
		Vector2 screenFront = WorldToScreen(frontWorld);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 背景を描画 (青色)
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x5282B8FF, kFillModeSolid);

		// 地面 (y = 0) の水平線
		Vector2 groundStart = WorldToScreen({ 0.0f, 0.0f });
		Novice::DrawLine(0, (int)groundStart.y, 1280, (int)groundStart.y, WHITE);

		// 台の折れ線を描画 (0 <= x <= 2)
		for (int i = 0; i < kSegments; ++i) {
			float x0 = 2.0f * (float)i / (float)kSegments;
			float x1 = 2.0f * (float)(i + 1) / (float)kSegments;
			Vector2 p0 = WorldToScreen({ x0, f(x0) });
			Vector2 p1 = WorldToScreen({ x1, f(x1) });
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, WHITE);
		}

		// 台の先端 (x = 2) の垂直ガイド線
		Vector2 rampTop = WorldToScreen({ 2.0f, f(2.0f) });
		Vector2 rampBottom = WorldToScreen({ 2.0f, 0.0f });
		Novice::DrawLine((int)rampTop.x, (int)rampTop.y, (int)rampBottom.x, (int)rampBottom.y, 0xFFFFFF88);

		// バイク車体（前後輪を結ぶ直線）
		Novice::DrawLine((int)screenBack.x, (int)screenBack.y, (int)screenFront.x, (int)screenFront.y, WHITE);

		// 車輪の描画（オレンジ色の円）
		Novice::DrawEllipse((int)screenBack.x, (int)screenBack.y, kWheelRadius, kWheelRadius, 0.0f, 0xFF9900FF, kFillModeSolid);
		Novice::DrawEllipse((int)screenFront.x, (int)screenFront.y, kWheelRadius, kWheelRadius, 0.0f, 0xFF9900FF, kFillModeSolid);

		// ImGui デバッグ表示
		ImGui::Begin("Jump Ramp");
		ImGui::Text("A / D : move");
		ImGui::Text("x: %.3f", x);
		ImGui::Text("f'(x): %.3f", slope);
		ImGui::Text("Forward: (%.3f, %.3f)", forward.x, forward.y);
		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}