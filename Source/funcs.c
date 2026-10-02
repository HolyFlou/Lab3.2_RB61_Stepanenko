#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "funcs.h"

double f(double x) {
    return pow(x, 3) - x - 4.0;
}

double f_prime(double x) {
    return 3.0 * pow(x, 2) - 1.0;
}

double f_double_prime(double x) {
    return 6.0 * x;
}

int handle_max_iter_exceeded(int current_iter, double current_x) {
    int choice;
    printf("\nДосягнуто ліміт ітерацій (%d).\n", current_iter);
    printf("Поточне проміжне значення x = %.8f, f(x) = %.8f\n", current_x, f(current_x));
    printf("Оберіть подальшу дію:\n");
    printf("  1. Продовжити обчислення (ще на таку ж кількість ітерацій)\n");
    printf("  2. Виконувати до кінця (поки не досягнемо точності eps)\n");
    printf("  0. Перервати обчислення та вийти з переглядом результату\n");
    printf("Ваш вибір: ");
    scanf("%d", &choice);

    return choice;
}

void solve_bisection(double a, double b, double eps, int max_iter, int debug) {
    if (f(a) * f(b) >= 0) {
        printf("\nПомилка: f(a) та f(b) повинні мати різні знаки на кінцях відрізка!\n");
        return;
    }

    int iter = 0;
    int iter_limit = max_iter;
    double c = a;
    double prev_c;
    clock_t start_time = clock();

    if (debug) {
        printf("\nРежим налагодження (Половинне ділення)\n");
        printf("%-5s | %-12s | %-12s | %-12s | %-12s | %-12s\n", "Iter", "a", "b", "c (mid)", "f(c)", "Half-Width");
    }

    while (1) {
        prev_c = c;
        c = (a + b) / 2.0;
        iter++;

        if (debug) {
            printf("%-5d | %-12.8f | %-12.8f | %-12.8f | %-12.8f | %-12.8f\n", iter, a, b, c, f(c), (b - a) / 2.0);
        }

        if (f(c) == 0.0 || (b - a) / 2.0 < eps) {
            break;
        }

        if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }

        if (iter_limit > 0 && iter >= iter_limit) {
            int action = handle_max_iter_exceeded(iter, c);

            if (action == 1) {
                iter_limit += max_iter;
            } else if (action == 2) {
                iter_limit = -1; // Безлімітний режим
            } else {
                printf("\nОбчислення зупинено користувачем.\n");
                break;
            }
        }
    }

    clock_t end_time = clock();
    double time_spent = (end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nРезультат (Метод половинного ділення)\n");
    printf("Знайдений корінь x      = %.8f\n", c);
    printf("Значення функції f(x)   = %.8e\n", f(c));
    printf("Кількість ітерацій      = %d\n", iter);
    printf("Затрачений час          = %.6f сек.\n", time_spent);
}

void solve_chord(double a, double b, double eps, int max_iter, int debug) {
    if (f(a) * f(b) >= 0) {
        printf("\nПомилка: f(a) та f(b) повинні мати різні знаки на кінцях відрізка!\n");
        return;
    }

    double fixed_point, x0;
   
    // Перевірка умови збіжності: f(x) * f''(x) > 0 визначає нерухому точку
    if (f(a) * f_double_prime(a) > 0) {
        fixed_point = a;
        x0 = b;
    } else {
        fixed_point = b;
        x0 = a;
    }

    double x_curr = x0;
    double x_next;
    int iter = 0;
    int iter_limit = max_iter;
    clock_t start_time = clock();

    if (debug) {
        printf("\nРежим налагодження (Метод хорд)\n");
        printf("Нерухома точка: %.4f, Початкове наближення x0: %.4f\n", fixed_point, x0);
        printf("%-5s | %-14s | %-14s | %-14s\n", "Iter", "x_k", "f(x_k)", "|x_k - x_{k-1}|");
    }

    do {
        // Формула методу хорд з рухомою точкою x_curr та нерухомою fixed_point
        x_next = x_curr - (f(x_curr) * (fixed_point - x_curr)) / (f(fixed_point) - f(x_curr));
        iter++;
        double diff = fabs(x_next - x_curr);

        if (debug) {
            printf("%-5d | %-14.8f | %-14.8f | %-14.8f\n", iter, x_next, f(x_next), diff);
        }

        if (diff < eps) {
            x_curr = x_next;
            break;
        }

        x_curr = x_next;

        if (iter_limit > 0 && iter >= iter_limit) {
            int action = handle_max_iter_exceeded(iter, x_curr);
            if (action == 1) {
                iter_limit += max_iter;
            } else if (action == 2) {
                iter_limit = -1;
            } else {
                printf("\nОбчислення зупинено користувачем.\n");
                break;
            }
        }
    } while (1);

    clock_t end_time = clock();
    double time_spent = (end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nРезультат (Метод хорд)\n");
    printf("Знайдений корінь x      = %.8f\n", x_curr);
    printf("Значення функції f(x)   = %.8e\n", f(x_curr));
    printf("Кількість ітерацій      = %d\n", iter);
    printf("Затрачений час          = %.6f сек.\n", time_spent);

} 