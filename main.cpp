#include <Novice.h>
#include <imgui.h>
#include <cmath>
#include <algorithm>

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
// 演算子オーバーロード
// ----------------------------------------------------------------
Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}

Quaternion operator-(const Quaternion& q) {
	return { -q.x, -q.y, -q.z, -q.w };
}

// ----------------------------------------------------------------
// ベクトル・Quaternion 基本演算関数
// ----------------------------------------------------------------

// 内積 (Vector3)
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 内積 (Quaternion)
float Dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

// 外積 (Cross)
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

// ベクトルの正規化 (Vector3)
Vector3 Normalize(const Vector3& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length != 0.0f) {
		return { v.x / length, v.y / length, v.z / length };
	}
	return { 0.0f, 0.0f, 0.0f };
}

// Quaternion の正規化
Quaternion Normalize(const Quaternion& q) {
	float norm = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	if (norm != 0.0f) {
		return { q.x / norm, q.y / norm, q.z / norm, q.w / norm };
	}
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

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
// Quaternion 応用関数
// ----------------------------------------------------------------

// 任意軸回転を表す Quaternion の生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Vector3 normAxis = Normalize(axis);
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

// ベクトルを Quaternion で回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConj = Conjugate(quaternion);
	Quaternion rPrime = Multiply(Multiply(quaternion, r), qConj);
	return { rPrime.x, rPrime.y, rPrime.z };
}

// Quaternion から回転行列を求める
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

// from を to へ向ける単位 Quaternion を返す関数
Quaternion DirectionToDirection(const Vector3& from, const Vector3& to) {
	float dot = Dot(from, to);
	const float tolerance = 1.0e-6f;

	if (dot <= -1.0f + tolerance) {
		Vector3 axis;
		if (std::abs(from.x) > 1.0e-5f || std::abs(from.y) > 1.0e-5f) {
			axis = Normalize(Vector3{ from.y, -from.x, 0.0f });
		}
		else {
			axis = Normalize(Vector3{ from.z, 0.0f, -from.x });
		}
		return Quaternion{ axis.x, axis.y, axis.z, 0.0f };
	}

	Vector3 cross = Cross(from, to);
	Quaternion rotation{
		cross.x, cross.y, cross.z,
		1.0f + dot
	};
	return Normalize(rotation);
}

// 【課題指定】 球面線形補間 (Slerp)
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	Quaternion q0Copy = q0;
	float dot = Dot(q0Copy, q1);

	// 最短経路を取るために内積が負の場合は反転
	if (dot < 0.0f) {
		q0Copy = -q0Copy;
		dot = -dot;
	}

	// 誤差により1.0fを超えないようクランプ
	if (dot > 1.0f) {
		dot = 1.0f;
	}

	// theta=0 の極限（特異点処理：ほぼ同じ向きの場合は線形補間）
	if (dot >= 1.0f - 1.0e-5f) {
		return {
			(1.0f - t) * q0Copy.x + t * q1.x,
			(1.0f - t) * q0Copy.y + t * q1.y,
			(1.0f - t) * q0Copy.z + t * q1.z,
			(1.0f - t) * q0Copy.w + t * q1.w
		};
	}

	// なす角 theta の計算
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	// 補間係数の計算
	float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	float scale1 = std::sin(t * theta) / sinTheta;

	return {
		scale0 * q0Copy.x + scale1 * q1.x,
		scale0 * q0Copy.y + scale1 * q1.y,
		scale0 * q0Copy.z + scale1 * q1.z,
		scale0 * q0Copy.w + scale1 * q1.w
	};
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
	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);

	Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

	// メインループ
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新・描画処理ここから
		///

		QuaternionScreenPrintf(0, kRowHeight * 0, interpolate0, " : interpolate0, Slerp(q0, q1, 0.0f)");
		QuaternionScreenPrintf(0, kRowHeight * 1, interpolate1, " : interpolate1, Slerp(q0, q1, 0.3f)");
		QuaternionScreenPrintf(0, kRowHeight * 2, interpolate2, " : interpolate2, Slerp(q0, q1, 0.5f)");
		QuaternionScreenPrintf(0, kRowHeight * 3, interpolate3, " : interpolate3, Slerp(q0, q1, 0.7f)");
		QuaternionScreenPrintf(0, kRowHeight * 4, interpolate4, " : interpolate4, Slerp(q0, q1, 1.0f)");

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