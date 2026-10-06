#include <Novice.h>
#include <imgui.h>
#include <cmath>

// ----------------------------------------------------------------
// 構造体定義
// ----------------------------------------------------------------
struct Vector3 {
	float x;
	float y;
	float z;
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

struct Matrix4x4 {
	float m[4][4];
};

// ----------------------------------------------------------------
// Quaternion 基本演算関数
// ----------------------------------------------------------------

// クォータニオンの積 (Multiply)
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return {
		lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x,
		lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y,
		lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

// 共役 Quaternion (Conjugate)
Quaternion Conjugate(const Quaternion& quaternion) {
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// ベクトルの正規化 (Vector3)
Vector3 Normalize(const Vector3& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length != 0.0f) {
		return { v.x / length, v.y / length, v.z / length };
	}
	return { 0.0f, 0.0f, 0.0f };
}

// 行列によるベクトルの変換 (Transform)
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f && w != 1.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}
	return result;
}

// ----------------------------------------------------------------
// 【課題指定】 Quaternion 応用関数
// ----------------------------------------------------------------

// 1. 任意軸回転を表す Quaternion の生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Vector3 normAxis = Normalize(axis); // 回転軸は必ず単位ベクトルにする
	float halfAngle = angle * 0.5f;
	float sinHalf = std::sin(halfAngle);
	float cosHalf = std::cos(halfAngle);

	return {
		normAxis.x * sinHalf,
		normAxis.y * sinHalf,
		normAxis.z * sinHalf,
		cosHalf
	};
}

// 2. ベクトルを Quaternion で回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	// ベクトル v を Quaternion r = (v.x, v.y, v.z, 0) に変換
	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConj = Conjugate(quaternion);

	// r' = q * r * q*
	Quaternion rPrime = Multiply(Multiply(quaternion, r), qConj);

	// 回転後のベクトル部を返す
	return { rPrime.x, rPrime.y, rPrime.z };
}

// 3. Quaternion から回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 result;

	float x = q.x, y = q.y, z = q.z, w = q.w;

	result.m[0][0] = w * w + x * x - y * y - z * z;
	result.m[0][1] = 2.0f * (x * y + w * z);
	result.m[0][2] = 2.0f * (x * z - w * y);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (x * y - w * z);
	result.m[1][1] = w * w - x * x + y * y - z * z;
	result.m[1][2] = 2.0f * (y * z + w * x);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (x * z + w * y);
	result.m[2][1] = 2.0f * (y * z - w * x);
	result.m[2][2] = w * w - x * x - y * y + z * z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// ----------------------------------------------------------------
// デバッグ描画用関数 (画面出力用)
// ----------------------------------------------------------------
static const int kRowHeight = 20;

void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label) {
	Novice::ScreenPrintf(x, y, "%.2f  %.2f  %.2f  %.2f %s", q.x, q.y, q.z, q.w, label);
}

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.2f  %.2f  %.2f %s", v.x, v.y, v.z, label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		Novice::ScreenPrintf(x, y + (row + 1) * kRowHeight, "  %.3f  %.3f  %.3f  %.3f",
			m.m[row][0], m.m[row][1], m.m[row][2], m.m[row][3]);
	}
}

const char kWindowTitle[] = "LE1C_12_ショウ_ズーウェン";

// ----------------------------------------------------------------
// エントリーポイント
// ----------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力準備
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 課題指定のテストデータ
	Quaternion rotation = MakeRotateAxisAngleQuaternion(
		Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f
	);
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

	// メインループ
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新・描画処理ここから
		///

		// スライド（画像2）と同じ ScreenPrintf による結果表示
		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, " : rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, " : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, " : rotateByMatrix");

		///
		/// ↑更新・描画処理ここまで
		///

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}