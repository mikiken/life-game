#include <stdio.h>
#include "unistd.h"

#define WIDTH 32
#define HEIGHT 32
#define true 1
#define false 0

bool matrix[WIDTH][HEIGHT] = {0};

bool is_in_range(int x, int y) {
    return 0 <= x && x < WIDTH && 0 <= y && y < HEIGHT;
}

int count_alive_neighbors(int x, int y) {
    int count = 0;
    // 左上
    if (is_in_range(x - 1, y - 1))
        matrix[x - 1][y - 1] ? count++ : 0;
    // 上
    if (is_in_range(x, y - 1))
        matrix[x][y - 1] ? count++ : 0;
    // 右上
    if (is_in_range(x + 1, y - 1))
        matrix[x + 1][y - 1] ? count++ : 0;
    // 左
    if (is_in_range(x - 1, y))
        matrix[x - 1][y] ? count++ : 0;
    // 右
    if (is_in_range(x + 1, y))
        matrix[x + 1][y] ? count++ : 0;
    // 左下
    if (is_in_range(x - 1, y + 1))
        matrix[x - 1][y + 1] ? count++ : 0;
    // 下
    if (is_in_range(x, y + 1))
        matrix[x][y + 1] ? count++ : 0;
    // 右下
    if (is_in_range(x + 1, y + 1))
        matrix[x + 1][y + 1] ? count++ : 0;

    return count;
}

// 死んでいるセルに隣接する生きたセルがちょうど3つあれば，次の世代が誕生
bool to_be_born(int x, int y) {
    int alive = count_alive_neighbors(x, y);
    return alive == 3;
}

// 生きているセルに隣接する生きたセルが2つか3つならば，次の世代でも生存
bool to_be_alive(int x, int y) {
    int alive = count_alive_neighbors(x, y);
    return alive == 2 || alive == 3;
}

// 生きているセルに隣接する生きたセルが1つ以下ならば，過疎により死滅
bool to_be_underpopulated(int x, int y) {
    int alive = count_alive_neighbors(x, y);
    return alive <= 1;
}

// 生きているセルに隣接する生きたセルが4つ以上ならば，過密により死滅
bool to_be_overpopulated(int x, int y) {
    int alive = count_alive_neighbors(x, y);
    return alive >= 4;
}

void update_matrix() {
    bool next_matrix[WIDTH][HEIGHT] = {0};
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++) {
            // 現在のセルが死んでいる場合
            if (!matrix[x][y]) {
                if (to_be_born(x, y))
                    next_matrix[x][y] = true;
            }
            // 現在のセルが生きている場合
            else {
                if (to_be_alive(x, y))
                    next_matrix[x][y] = true;
                if (to_be_underpopulated(x, y))
                    next_matrix[x][y] = false;
                if (to_be_overpopulated(x, y))
                    next_matrix[x][y] = false;
            }
        }
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            matrix[x][y] = next_matrix[x][y];
}

void init_matrix() {
    // 初期状態
    matrix[13][13] = true;
    matrix[13][14] = true;
    matrix[13][15] = true;
    matrix[13][17] = true;

    matrix[14][13] = true;

    matrix[15][16] = matrix[15][17] = true;

    matrix[16][14] = matrix[16][15] = matrix[16][17] = true;

    matrix[17][13] = matrix[17][15] = matrix[17][17] = true;
}

void setup() {
    init_pin();
    init_matrix();
    delay(5000);
}

void loop() {
    unsigned display_start = millis();
    while (millis() - display_start < 120)
        display(matrix);
    update_matrix();
}
