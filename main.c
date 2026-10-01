 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


// Визначення функції
double f(double x);

// Перша похідна: f'(x) = 3x^2 - 1
double f_prime(double x);

// Друга похідна: f''(x) = 6x
double f_double_prime(double x);

/* Запит користувачу при перевищенні кількості ітерацій
   Повертає: 1 - продовжити (+max_iter), 2 - безліміт, 0 - вийти */
int handle_max_iter_exceeded(int current_iter, double current_x);

// Метод половинного ділення
void solve_bisection(double a, double b, double eps, int max_iter, int debug);

// Метод хорд
void solve_chord(double a, double b, double eps, int max_iter, int debug);

int main() {

    double a = 1.0, b = 2.0;
    double eps = 0.00001;
    int max_iter = 10;
    int choice;
    int debug = 0;

    while (1) {
        printf("  f(x) = x^3 - x - 4 = 0 на [%.1f, %.1f]\n", a, b);
        printf("1. Метод половинного ділення\n");
        printf("2. Метод хорд\n");
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
                solve_bisection(a, b, eps, max_iter, debug);
                break;
            case 2:
                solve_chord(a, b, eps, max_iter, debug);
                break;
            case 3:
                printf("\nВиконання методу половинного ділення");
                solve_bisection(a, b, eps, max_iter, debug);
                printf("\nВиконання методу хорд");
                solve_chord(a, b, eps, max_iter, debug);
                break;
            case 4:
                printf("Введіть a: ");
                scanf("%lf", &a);
                printf("Введіть b: ");
                scanf("%lf", &b);
                printf("Введіть точноcть eps (наприклад 0.00001): ");
                scanf("%lf", &eps);
                printf("Введіть max_iter: ");
                scanf("%d", &max_iter);
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

// Визначення функції
double f(double x) {
    return pow(x, 3) - x - 4.0;
}
// Перша похідна: f'(x) = 3x^2 - 1
double f_prime(double x) {
    return 3.0 * pow(x, 2) - 1.0;
}

// Друга похідна: f''(x) = 6x
double f_double_prime(double x) {
    return 6.0 * x;
}

/* Запит користувачу при перевищенні кількості ітерацій
   Повертає: 1 - продовжити (+max_iter), 2 - безліміт, 0 - вийти */
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

// Метод половинного ділення
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

// Метод хорд
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