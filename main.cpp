#include <iostream>
#include <windows.h>

// 2つの異なる型に対応するクラステンプレートの定義
template <typename T1, typename T2>
class NumberCompare {
private:
    T1 val1_;
    T2 val2_;

public:
    // コンストラクタ
    NumberCompare(T1 val1, T2 val2) : val1_(val1), val2_(val2) {}

    // 2つの値を比べて小さい値を返すメンバ関数
    T1 Min() const {
        if (val1_ < val2_) {
            return val1_;
        }
        else {
            // 第1テンプレート引数の型(T1)へ明示的にキャストして返す
            return static_cast<T1>(val2_);
        }
    }
};

int main() {
    // コンソールの文字化け防止（UTF-8指定）
    SetConsoleOutputCP(65001);

    // --- インスタンス生成（計6パターン） ---

    // パターン1: 同一型 (3通り)
    NumberCompare<int, int>         c1(10, 20);
    NumberCompare<float, float>     c2(5.5f, 2.3f);
    NumberCompare<double, double>   c3(3.1415, 9.999);

    // パターン2: 異なる型 (3C2 = 3通り)
    NumberCompare<int, float>       c4(15, 12.8f);
    NumberCompare<int, double>      c5(100, 99.5);
    NumberCompare<float, double>    c6(4.5f, 4.2);

    // --- main関数からの呼び出しと結果表示 ---
    std::cout << "=== Min() 呼び出し結果 (全6パターン) ===" << std::endl;
    std::cout << "1. <int, int>       (10, 20)   -> 小さい値: " << c1.Min() << std::endl;
    std::cout << "2. <float, float>   (5.5, 2.3) -> 小さい値: " << c2.Min() << std::endl;
    std::cout << "3. <double, double> (3.1415, 9.999) -> 小さい値: " << c3.Min() << std::endl;
    std::cout << "4. <int, float>     (15, 12.8) -> 小さい値: " << c4.Min() << std::endl;
    std::cout << "5. <int, double>    (100, 99.5)-> 小さい値: " << c5.Min() << std::endl;
    std::cout << "6. <float, double>  (4.5, 4.2) -> 小さい値: " << c6.Min() << std::endl;

    return 0;
}