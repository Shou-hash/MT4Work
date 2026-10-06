#include <iostream>
#include <windows.h>

// 1. 基底クラス：動物
class Animal {
public:
    Animal() {}
    // 基底クラスには仮想デストラクタを用意する
    virtual ~Animal() {
        std::cout << "[Animal] デストラクタが呼ばれました" << std::endl;
    }

    // 仮想関数（頭に virtual をつける）
    virtual void MakeSound() {
        std::cout << "（動物の鳴き声）" << std::endl;
    }
};

// 2. 派生クラス：犬
class Dog : public Animal {
public:
    Dog() {}
    ~Dog() override {
        std::cout << "[Dog] 犬を片付けました" << std::endl;
    }

    // 仮想関数を上書き（お尻に override をつける）
    void MakeSound() override {
        std::cout << "犬：ワンワン！" << std::endl;
    }
};

// 3. 派生クラス：猫
class Cat : public Animal {
public:
    Cat() {}
    ~Cat() override {
        std::cout << "[Cat] 猫を片付けました" << std::endl;
    }

    // 仮想関数を上書き（お尻に override をつける）
    void MakeSound() override {
        std::cout << "猫：ニャーニャー！" << std::endl;
    }
};

int main() {
    // コンソールの文字化け防止（UTF-8指定）
    SetConsoleOutputCP(65001);

    const int NUM_ANIMALS = 2;
    // 基底クラスのポインタ配列で派生クラスをまとめて扱う
    Animal* animals[NUM_ANIMALS];

    // --- 生成フェーズ ---
    animals[0] = new Dog();
    animals[1] = new Cat();

    // --- 実行フェーズ（ポリモーフィズムの実例） ---
    std::cout << "=== 動物の鳴き声フェーズ ===" << std::endl;
    for (int i = 0; i < NUM_ANIMALS; i++) {
        animals[i]->MakeSound(); // 各インスタンスに応じた関数が実行される
    }

    // --- 破棄フェーズ ---
    std::cout << "\n=== 破棄フェーズ ===" << std::endl;
    for (int i = 0; i < NUM_ANIMALS; i++) {
        delete animals[i]; // 仮想デストラクタにより派生クラスのデストラクタも正しく呼ばれる
    }

    return 0;
}