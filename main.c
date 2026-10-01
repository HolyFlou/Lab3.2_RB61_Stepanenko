#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

// Визначення математичних функцій
double f(double x) {
    return pow(x, 3) - x - 4.0;
}

double f_prime(double x) {
    return 3.0 * pow(x, 2) - 1.0;
}

double f_double_prime(double x) {
    return 6.0 * x;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    double a = 1.0, b = 2.0;
    double eps = 0.00001;
    int max_iter = 10;
    int choice;
    int debug = 0;

    while (1) {
        printf("\nf(x) = x^3 - x - 4 = 0 на [%.1f, %.1f]\n", a, b);
        printf("1. Метод половинного ділення (не реалізовано)\n");
        printf("2. Метод хорд (не реалізовано)\n");
        printf("3. Порівняти обидва методи\n");
        printf("4. Змінити параметри (a, b, eps, max_iter)\n");
        if (debug == 1) {
            printf("5. Переключити режим налагодження (зараз: УВІМКНЕНО)\n");
        } else {
            printf("5. Переключити режим налагодження (зараз: ВИМКНЕНО)\n");
        }
        printf("0. Вихід\n");
        printf("Оберіть пункт: ");
        
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("\nМетод половинного ділення у розробці...\n");
                break;
            case 2:
                printf("\nМетод хорд у розробці...\n");
                break;
            case 3:
                printf("\nПорівняння у розробці...\n");
                break;
            case 4:
                printf("Введіть a: "); scanf("%lf", &a);
                printf("Введіть b: "); scanf("%lf", &b);
                printf("Введіть точноcть eps (наприклад 0.00001): "); scanf("%lf", &eps);
                printf("Введіть max_iter: "); scanf("%d", &max_iter);
                break;
            case 5:
                if (debug == 0) {
                    debug = 1;
                    printf("Режим налагодження увімкнено.\n");
                } else {
                    debug = 0;
                    printf("Режим налагодження вимкнено.\n");
                }
                break;
            case 0:
                printf("Завершення роботи програми.\n");
                return 0;
            default:
                printf("Некоректний вибір. Спробуйте ще раз.\n");
        }
    }

    return 0;
}