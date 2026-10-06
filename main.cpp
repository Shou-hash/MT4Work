#include <Novice.h>
#include <imgui.h>

// 2Dベクトル構造体
struct Vector2 {
	float x;
	float y;
};

const char kWindowTitle[] = "LC1C_12_ショウ_ズーウェン";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 円A (ターゲット：マウス位置 / 半径 12)
	Vector2 posA = { 0.0f, 0.0f };
	const float radiusA = 12.0f;

	// 円B (追従する円 / 初期座標 640, 360 / 半径 20)
	Vector2 posB = { 640.0f, 360.0f };
	const float radiusB = 20.0f;

	// 追従速度パラメータ (ImGuiで変更可能)
	float speed = 5.0f;

	// 60fps前提の固定デルタタイム
	const float deltaTime = 1.0f / 60.0f;

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

		// 1. マウスカーソルの位置を取得して円Aの座標に設定
		int mouseX = 0;
		int mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);
		posA.x = static_cast<float>(mouseX);
		posA.y = static_cast<float>(mouseY);

		// 3. 円Bの位置を線形補間によって円Aの方向へ追従させる
		// 計算式 : pos += (speed * deltaTime) * (target - pos);
		posB.x += (speed * deltaTime) * (posA.x - posB.x);
		posB.y += (speed * deltaTime) * (posA.y - posB.y);

		// 4. ImGui で追従速度パラメータ (speed) をスライダー変更できるようにする
		ImGui::Begin("Interpolation Controller");
		ImGui::Text("Target: Mouse Position (Red Circle)");
		ImGui::SliderFloat("Speed", &speed, 0.0f, 30.0f);
		ImGui::Separator();
		ImGui::Text("Mouse Pos (Circle A): (%.1f, %.1f)", posA.x, posA.y);
		ImGui::Text("Circle B Pos: (%.1f, %.1f)", posB.x, posB.y);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 3.c. 円Aと円Bの中心を結ぶ線を表示し、追従の遅れを可視化
		Novice::DrawLine(
			static_cast<int>(posA.x), static_cast<int>(posA.y),
			static_cast<int>(posB.x), static_cast<int>(posB.y),
			0xFFFFFFFF
		);

		// 2. 半径20の円B (緑色) を描画
		Novice::DrawEllipse(
			static_cast<int>(posB.x), static_cast<int>(posB.y),
			static_cast<int>(radiusB), static_cast<int>(radiusB),
			0.0f, 0x00FF00FF, kFillModeSolid
		);

		// 1. 半径12の円A (赤色) を描画
		Novice::DrawEllipse(
			static_cast<int>(posA.x), static_cast<int>(posA.y),
			static_cast<int>(radiusA), static_cast<int>(radiusA),
			0.0f, 0xFF0000FF, kFillModeSolid
		);

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