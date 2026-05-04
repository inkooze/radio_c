#include <stdio.h>
#include <windows.h>
#include <math.h>

// \\ // \\ //

// Кастомная структура ("тип данных") для floating point number - числа с плавающей точкой
typedef struct {
    int znak;
    unsigned int exponent;
    unsigned long long mantissa;
} fpn;

// Определение данных (структуры) для числа
fpn double_to_fpn(double db_num) {
    fpn fp_num = {0, 0, 0};

    if (db_num == 0.0) return fp_num;

    // Знак
    fp_num.znak = (db_num < 0.0) ? 1 : 0;

    //
    int exp = 0;
    double mant = fabs(db_num);

    while (mant >= 1.0) {
        mant /= 2.0;
        exp++;
    }

    while (mant < 0.5) {
        mant *= 2.0;
        exp--;
    }

    fp_num.exponent = (unsigned int)(exp - 1 + 1023);
    fp_num.mantissa = (unsigned long long)(mant * 2.0 * (1ULL << 52) + 0.5);

    return fp_num;
}

double fpn_to_double(fpn fp_num) {
    if (fp_num.mantissa == 0) return 0.0;

    double znak = (double)fp_num.mantissa / (double)(1ULL << 52);
    
    double db_num = (fp_num.znak == 1) ? -znak : znak;
    
    int exp = (int)fp_num.exponent - 1023;
    
    if (exp >= 0) {
        for (int i = 0; i < exp; i++) {
            db_num *= 2.0;
        }
    } else {
        int n = -exp;
        for (int i = 0; i < n; i++) {
            db_num /= 2.0;
        }
    }

    return db_num;
}

// \\ // \\ //

fpn slozhenie(fpn A, fpn B) {
    printf("- A: %.10g (S = %d; E = 0x%04X; M = 0x%013llX)\n", fpn_to_double(A), A.znak, A.exponent, A.mantissa);
    printf("- B: %.10g (S = %d; E = 0x%04X; M = 0x%013llX)\n", fpn_to_double(B), B.znak, B.exponent, B.mantissa);

    unsigned long long M1 = A.mantissa;
    unsigned long long M2 = B.mantissa;

    // Если один из операндов 0, значит возвращаем другой
    if (A.mantissa == 0) return B;
    if (B.mantissa == 0) return A;

    unsigned int E3 = (A.exponent > B.exponent) ? A.exponent : B.exponent;
    long dE = (long)A.exponent - (long)B.exponent;

    if (dE != 0) {
        if (dE > 0) {
            M2 >>= (unsigned int)dE;
        } else {
            M1 >>= (unsigned int)(-dE);
        }
        printf("    - Уравнивание: dE = %ld\n", dE);
    } else {
        printf("    - Уравнивания не требуется\n");
    }
    printf("    - M1 = 0x%013llX\n    - M2 = 0x%013llX\n", M1, M2);

    unsigned long long M3;
    int S3;

    if (A.znak == B.znak) {
        M3 = M1 + M2;
        S3 = A.znak;

        if (M3 & (1ull << 53)) {
            M3 >>= 1;
            E3++;
            printf("    - Переполнение мантиссы, сдвиг вправо, E3 + 1 = 0x%04X\n", E3);
        }
    } else {
        if (M1 >= M2) {
            M3 = M1 - M2;
            S3 = A.znak;
        } else {
            M3 = M2 - M1;
            S3 = B.znak;
        }

        if (M3 != 0) {
            int k = 0;
            while ((M3 & (1ull << 52)) == 0 || k > 60) {
                M3 <<= 1;
                k++;
            }
            E3 -= k;
            printf("    - Нормализация влево на %d разрядов, E3 = 0x%04X\n", k, E3);
        } else {
            S3 = 0;
        }
    }

    if (M3 == 0) {
        S3 = 0;
        E3 = 0;
    }

    fpn result = {S3, E3, M3};
    printf("- Результат: %.10g (S = %d; E = 0x%04X; M = 0x%013llX)\n", fpn_to_double(result),
        result.znak, result.exponent, result.mantissa);

    return result;
}

// \\ // \\ //

fpn vichitanie(fpn A, fpn B) {
    B.znak = (B.mantissa != 0) ? 1 - B.znak : 0; // Учитываем, чтобы 0 не был "отрицательным" (через sign)

    return slozhenie(A, B);
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

    printf("Сложение:\n");
    fpn sum = slozhenie(A, B);

    printf("\nВычитание:\n");
    fpn diff = vichitanie(A, B);

    return 0;
}