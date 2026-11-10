#include <Novice.h>
#include <imgui.h>
#include <cmath>

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

Vector2 operator*(const Vector2& v, float s) {
	return { v.x * s, v.y * s };
}

Vector2 operator*(float s, const Vector2& v) {
	return { v.x * s, v.y * s };
}

// ----------------------------------------------------------------
// 関数定義
// ----------------------------------------------------------------

// ベクトルの正規化
Vector2 Normalize(const Vector2& v) {
	float length = std::sqrtf(v.x * v.x + v.y * v.y);
	if (length != 0.0f) {
		return { v.x / length, v.y / length };
	}
	return { 0.0f, 0.0f };
}

// 【課題指定 1】位置を返す Bezier
Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	return (1.0f - t) * (1.0f - t) * p0 + 2.0f * t * (1.0f - t) * p1 + t * t * p2;
}

// 【課題指定 1】接線ベクトルを返す BezierDerivative
Vector2 BezierDerivative(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	return 2.0f * (1.0f - t) * (p1 - p0) + 2.0f * t * (p2 - p1);
}

// ----------------------------------------------------------------
// 定数定義
// ----------------------------------------------------------------
const char kWindowTitle[] = "LE1C_12_ショウ_ズーウェン";
const int kRoadSegments = 100; // 道の分割数

// ----------------------------------------------------------------
// エントリーポイント
// ----------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力準備
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 道の制御点（初期値）
	Vector2 p0 = { 200.0f, 570.0f };
	Vector2 p1 = { 800.0f, 70.0f };
	Vector2 p2 = { 1080.0f, 570.0f };

	// パラメータ t
	float t = 0.5f;

	// メインループ
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓ImGui・計算処理ここから
		///

		// ImGui ウィンドウとスライダー表示
		ImGui::Begin("Bezier");
		ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
		ImGui::SliderFloat("p1 x", &p1.x, 400.0f, 880.0f);
		ImGui::SliderFloat("p1 y", &p1.y, 20.0f, 570.0f);

		// ベジェ曲線上の点と導関数（接線ベクトル）の計算
		Vector2 point = Bezier(p0, p1, p2, t);
		Vector2 derivative = BezierDerivative(p0, p1, p2, t);

		// B(t) と B'(t) の値を ImGui に表示
		ImGui::Text("B(t) : (%.1f, %.1f)", point.x, point.y);
		ImGui::Text("B'(t) : (%.1f, %.1f)", derivative.x, derivative.y);
		ImGui::End();

		// 接線ベクトルの正規化と120倍化
		Vector2 dir = Normalize(derivative);
		Vector2 lineEnd = point + dir * 120.0f;

		///
		/// ↑ImGui・計算処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 背景を描画 (青色)
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x5282B8FF, kFillModeSolid);

		// 道（2次ベジェ曲線）を折れ線で描画
		for (int i = 0; i < kRoadSegments; ++i) {
			float t0 = static_cast<float>(i) / static_cast<float>(kRoadSegments);
			float t1 = static_cast<float>(i + 1) / static_cast<float>(kRoadSegments);
			Vector2 c0 = Bezier(p0, p1, p2, t0);
			Vector2 c1 = Bezier(p0, p1, p2, t1);
			Novice::DrawLine(static_cast<int>(c0.x), static_cast<int>(c0.y),
				static_cast<int>(c1.x), static_cast<int>(c1.y), WHITE);
		}

		// 接線（赤色の線）を描画
		Novice::DrawLine(static_cast<int>(point.x), static_cast<int>(point.y),
			static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), RED);

		// B(t) の位置に点（オレンジ色の円）を描画
		Novice::DrawEllipse(static_cast<int>(point.x), static_cast<int>(point.y), 6, 6, 0.0f, 0xFF9900FF, kFillModeSolid);

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