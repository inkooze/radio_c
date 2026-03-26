#include <stdio.h>
#include <windows.h>

// Первый алгоритм (с восстановлением остатка)
void division1(int RG1, int RG2) {
    int RG3 = 0;
    int CT = 16 + 1;

    printf("Первый алгоритм (с восстановлением остатка)\nНачальные значения: RG1 = %i, RG2 = %i, RG3 = %i\n", RG1, RG2, RG3);

    while (CT) {
        RG1 -= RG2 << 16;

        RG3 <<= 1;
        if (RG1 >= 0) {
            RG3 |= 1;
        } else {
            RG1 += RG2 << 16;
        }

        RG1 <<= 1;

        CT--;
        printf("\t- RG1 = %i, RG2 = %i, RG3 = %i\n", RG1, RG2, RG3);
    }

    if (RG3 > 0x0000FFFF) {
        printf("Ошибка вычисления: переполнение!\n");
    } else {
        printf("Результат второго алгоритма (без восстановления остатка): %i, %i (ост.)\n\n", RG3, RG1 >> 17);
    }
}

// Второй алгоритм (без восстановления)
void division2(int RG1, int RG2) {
    int RG3 = 0;
    int CT = 16 + 1;

    printf("Второй алгоритм (без восстановления остатка)\nНачальные значения: RG1 = %i, RG2 = %i, RG3 = %i\n", RG1, RG2, RG3);

    while (CT) {
        if (RG1 >= 0) {
            RG1 -= RG2 << 16;
        }
        else {
            RG1 += RG2 << 16;
        }

        RG3 <<= 1;
        if (RG1 >= 0) {
            RG3 |= 1;
        }

        // Сдвиг всего регистра
        if (RG1 < 0) {
            RG1 = -RG1;
            RG1 <<= 1;
            RG1 = -RG1;
        } else {
            RG1 <<= 1;
        }

        CT--;
        printf("\t- RG1 = %i, RG2 = %i, RG3 = %i\n", RG1, RG2, RG3);
    }

    if (RG3 > 0x0000FFFF) {
        printf("Ошибка вычисления: переполнение!\n");
    } else {
        printf("Результат второго алгоритма (без восстановления остатка): %i, %i (ост.)\n\n", RG3, (RG1 < 0) ? (RG1 >> 17) + RG2 : RG1 >> 17);
    }
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);

    int A, B;

    printf("Делимое: "); scanf("%i", &A);
    printf("Делитель: "); scanf("%i", &B);

    if (A >= 0 && A <= 65535 && B > 0 && B <= 256) {
        printf("Вычисляем выражение %i / %i\n\n", A, B);
        division1(A, B);
        division2(A, B);
    } else {
        printf("A должно быть в диапазоне [0; 65535], а В в диапазоне [1; 256]!");
    }
    return 0;
}