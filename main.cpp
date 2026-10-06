#include <iostream>
#include <windows.h>

// 敵の行動フェーズ
enum class Phase {
    Approach, // 接近 (0)
    Shoot,    // 射撃 (1)
    Escape,   // 離脱 (2)
    End       // 終了 (3)
};

// Enemyクラスの定義
class Enemy {
public:
    Enemy() : phase_(Phase::Approach) {}

    // 状態更新処理
    void Update() {
        if (IsEnd()) {
            return;
        }

        // enum classを配列インデックス用にキャストして関数テーブルから呼び出し
        size_t index = static_cast<size_t>(phase_);
        (this->*spFuncTable[index])();
    }

    // 行動終了の判定
    bool IsEnd() const {
        return phase_ == Phase::End;
    }

private:
    // フェーズごとのメンバ関数
    void Approach() {
        std::cout << "[接近] 敵が接近してきました。" << std::endl;
        phase_ = Phase::Shoot; // 射撃フェーズへ遷移
    }

    void Shoot() {
        std::cout << "[射撃] 敵が射撃を行いました。" << std::endl;
        phase_ = Phase::Escape; // 離脱フェーズへ遷移
    }

    void Escape() {
        std::cout << "[離脱] 敵が戦域から離脱しました。" << std::endl;
        phase_ = Phase::End; // 終了状態へ遷移
    }

private:
    Phase phase_; // 現在のフェーズ

    // メンバ関数ポインタテーブルの宣言
    static void (Enemy::* spFuncTable[])();
};

// メンバ関数ポインタテーブルの実体定義
void (Enemy::* Enemy::spFuncTable[])() = {
    &Enemy::Approach, // 要素番号 0
    &Enemy::Shoot,    // 要素番号 1
    &Enemy::Escape    // 要素番号 2
};

int main() {
    // コンソールの文字化け防止（UTF-8指定）
    SetConsoleOutputCP(65001);

    Enemy enemy;

    std::cout << "--- 敵の状態遷移開始 ---" << std::endl;

    // 敵が離脱を完了するまでループ処理
    while (!enemy.IsEnd()) {
        enemy.Update();
    }

    std::cout << "--- 敵の状態遷移終了 ---" << std::endl;

    return 0;
}