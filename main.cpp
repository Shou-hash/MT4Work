#include <Novice.h>
#include <imgui.h>
#include <cmath>

// 構造体定義
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

// 1. クォータニオンの積 (Multiply)
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return {
		lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x,
		lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y,
		lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

// 2. 乗法単位元 (Identity)
Quaternion IdentityQuaternion() {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// 3. 共役 Quaternion (Conjugate)
Quaternion Conjugate(const Quaternion& quaternion) {
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// 4. ノルム (Norm)
float Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
}

// 5. 正規化 (Normalize)
Quaternion Normalize(const Quaternion& quaternion) {
	float norm = Norm(quaternion);
	if (norm != 0.0f) {
		return {
			quaternion.x / norm,
			quaternion.y / norm,
			quaternion.z / norm,
			quaternion.w / norm
		};
	}
	return { 0.0f, 0.0f, 0.0f, 0.0f };
}

// 6. 逆 Quaternion (Inverse)
Quaternion Inverse(const Quaternion& quaternion) {
	float normSq =
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;

	if (normSq != 0.0f) {
		Quaternion conj = Conjugate(quaternion);
		return {
			conj.x / normSq,
			conj.y / normSq,
			conj.z / normSq,
			conj.w / normSq
		};
	}
	return { 0.0f, 0.0f, 0.0f, 0.0f };
}

const char kWindowTitle[] = "LC1C_12_ショウ_ズーウェン";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 課題指定の初期値
	Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
	Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };

	// 各関数の計算結果を取得
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

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

		// ImGui に計算結果を表示
		ImGui::Begin("MT2_00Basic");
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Identity", identity.x, identity.y, identity.z, identity.w);
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Conjugate", conj.x, conj.y, conj.z, conj.w);
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Inverse", inv.x, inv.y, inv.z, inv.w);
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Normalize", normal.x, normal.y, normal.z, normal.w);
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Multiply(q1, q2)", mul1.x, mul1.y, mul1.z, mul1.w);
		ImGui::Text("%5.2f %5.2f %5.2f %5.2f : Multiply(q2, q1)", mul2.x, mul2.y, mul2.z, mul2.w);
		ImGui::Text("%5.2f                : Norm", norm);
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