#include <stdio.h>

// Первый - проверка старшего бита, сдвиг вправо
int first(int x, int Y) {
    int S = 0, X = x << 16;

    while (Y) {
        X >>= 1;
        if (Y & (1 << 15)) S += X; // Y & (1 << 7) - вернёт число либо 0x0 (восьмого бита нет), либо 10x0 (восьмой бит имеется)
        Y <<= 1;
    }
    return S;
}

// Второй - проверка младшего бита, сдвиг влево (классический метод)
int second(int x, int Y) {
    int S = 0, X = x;

    while (Y) {
        if (Y & 1) S += X;
        X <<= 1;
        Y >>= 1;
    }
    return S;
}

// Третий - проверка старшего бита, сумма сдвигается влево
int third(int x, int Y) {
    int S = 0;

    for (int i = 0; i < 16; i++) {
        S <<= 1;
        if (Y & (1 << 15)) S += x;
        Y <<= 1;
    }
    return S;
}

// Четвёртый - проверка младшего бита, сумма сдвигается вправо
int fourth(int x, int Y) {
    int S = 0;

    for (int i = 0; i < 16; i++) {
        if (Y & 1) S += (x << 16);
        S >>= 1;
        Y >>= 1;
    }
    return S;
}

int main() {
    int A = -1, B = 12;
    printf("%i * %i:\n- 1. %i\n- 2. %i\n- 3. %i\n- 4. %i", A, B, first(A, B), second(A, B), third(A, B), fourth(A, B));

    return 0;
}