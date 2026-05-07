#include <stdio.h>
#include <windows.h>
#include <math.h>

// \\ // \\ //

// Кастомная структура ("тип данных") для floating point number - числа с плавающей точкой
typedef struct {
    int znak;
    unsigned int exponent;
    double mantissa;
} fpn;

// Определение данных (структуры) для числа
fpn double_to_fpn(double double_num) {
    fpn fpn_num = {0, 0, 0};

    if (double_num == 0) return fpn_num;

    fpn_num.znak = (double_num < 0) ? 1 : 0;

    int E = 0;
    double M = fabs(double_num);

    while (M >= 1) {
        M /= 2;
        E++;
    }

    while (M < 0.5) {
        M *= 2;
        E--;
    }

    fpn_num.exponent = E - 1 + 1023;
    fpn_num.mantissa = M * 2 * (1ull << 52);

    return fpn_num;
}

double fpn_to_double(fpn fpn_num) {
    if (fpn_num.mantissa == 0) return 0;

    double Z = fpn_num.mantissa / (1ull << 52);
    double double_num = (fpn_num.znak == 1) ? -Z : Z;

    int E = fpn_num.exponent - 1023;

    if (E >= 0) {
        for (int i = 0; i < E; i++) {
            double_num *= 2;
        }
    } else {
        int n = -E;
        for (int i = 0; i < n; i++) {
            double_num /= 2;
        }
    }

    return double_num;
}

// \\ // \\ //

fpn umnozhenie(fpn A, fpn B) {
    unsigned long long M1 = A.mantissa;
    unsigned long long M2 = B.mantissa;

    if (M1 == 0 || M2 == 0) {
        printf("0 (S = 0, E = 0, M = 0)\n");
        fpn zero = {0, 0, 0};
        return zero;
        
    } else {
        int S3 = A.znak ^ B.znak;
        unsigned int E3 = A.exponent + B.exponent - 1023;

        unsigned long long M3 = 0;

        // Мл. СЧП
        for (int i = 0; i < 53; i++) {
            if (M2 & 1) M3 += M1;

            M2 >>= 1;

            if (i < 52) M3 >>= 1;
        }

        // Нормализуем, если 53-й бит установлен (т.е число >= 2)
        if (M3 >> 53) {
            M3 >>= 1;
            E3++;
        }

        fpn res = {S3, E3, M3};
        printf("%f (S = %d, E = %u, M = %llu)\n", fpn_to_double(res), S3, E3, M3);
        return res;
    }
}

// \\ // \\ //

fpn delenie(fpn A, fpn B) {
    unsigned long long M1 = A.mantissa;
    unsigned long long M2 = B.mantissa;

    if (M2 == 0) {
        printf("Делить на ноль нельзя!\n");

    } else if (M1 == 0) {
        printf("0 (S = 0, E = 0, M = 0)\n\n");
        fpn zero = {0, 0, 0};
        return zero;

    } else {
        int S3 = A.znak ^ B.znak;
        unsigned int E3 = A.exponent - B.exponent + 1023;

        unsigned long long M3 = 0;

        // С восст. ост.
        for (int i = 0; i < 53; i++) {
            M3 <<= 1;

            if (M1 >= M2) {
                M3 |= 1;
                M1 -= M2;
            }

            M1 <<= 1;
        }

        // Нормализация, если явная единица не в 52 бите
        if (!(M3 >> 52)) {
            M3 <<= 1;
            E3--;
        }

        fpn res = {S3, E3, M3};
        printf("%f (S = %d, E = %u, M = %llu)\n", fpn_to_double(res), S3, E3, M3);
        return res;
    }
}

// \\ // \\ //

int main() {
    SetConsoleOutputCP(CP_UTF8);

    double a, b;
    printf("A:");
    scanf("%lf", &a);
    printf("B:");
    scanf("%lf", &b);

    fpn A = double_to_fpn(a);
    fpn B = double_to_fpn(b);

    printf("- A: %f (S = %d, E = %u, M = %f)\n", fpn_to_double(A), A.znak, A.exponent, A.mantissa);
    printf("- B: %f (S = %d, E = %u, M = %f)\n", fpn_to_double(B), B.znak, B.exponent, B.mantissa);

    printf("\n- Умножение: ");
    umnozhenie(A, B);

    printf("- Деление: ");
    delenie(A, B);

    return 0;
}