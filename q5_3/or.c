#include<stdio.h>
#include<math.h>

#define MAX(x, y) ((x) > (y) ? (x):(y)) // 大きい方を返す
#define ABS(x) ((x) < 0.0 ? -(x):(x)) // 絶対値を返す

typedef struct 
{
    double x;
    double lambda;
    double a, b, c;

} FUNCTION;

typedef struct
{
    int max_loop; // ループ処理の最大回数
    int max_id;
    int period; // 計算結果を出力する周期

    double alpha; // 刻み幅
    double eps; // 計算終了基準

} ENV_VAR;

int main();

void Initialization_variable();
void Calculate_optimization();
void show_result_display();

// dF(x, λ)/dx = 2 * (x - a) + b + λ 
void nablaLagragian_x(double *dx, double x, double lambda, double a, double b);

// dF(x, λ)/dλ = x - c 
void nablaLagragian_lambda(double *dlambda, double x, double c);

// f(x) = (x - a)^2 + b * x
double func(double x, double a, double b);

int main()
{
    FUNCTION function_variable;
    ENV_VAR environment_variable;

    // (1)の計算
    Initialization_variable(&function_variable, &environment_variable, 1.0, 0.0, 0.0);
    Calculate_optimization(&function_variable, &environment_variable, "result_1.log");
    printf("\n");

    // (2)の計算処理
    Initialization_variable(&function_variable, &environment_variable, 11.0, 1.0, 1.0);
    Calculate_optimization(&function_variable, &environment_variable, "result_2.log");
    printf("\n");

    // (3)の計算処理
    Initialization_variable(&function_variable, &environment_variable, 21.0, 2.0, 2.0);
    Calculate_optimization(&function_variable, &environment_variable, "result_3.log");

    return 0;
}

// 変数の初期化
void Initialization_variable(FUNCTION *is_function, ENV_VAR *is_env_var, double a, double b, double c)
{
    // functions's variable
    is_function->x = 0.0;
    is_function->lambda = 1.0;

    is_function->a = a;
    is_function->b = b;
    is_function->c = c;

    // environment variable
    is_env_var->max_loop = 1000;
    is_env_var->period = 50;

    is_env_var->alpha = 0.1;
    is_env_var->eps = 0.001;
}

// 計算処理
void Calculate_optimization(FUNCTION *is_function, ENV_VAR *is_env_var, char *file_name)
{
    int k = 0;
    double dx = 0.0, dlambda = 0.0;
    FILE *calc_log; // 計算結果を出力するためのファイル
    calc_log = fopen(file_name, "w"); // 書き込み用ファイルを開く

    // 問6 (1)
    while(1)
    {
        // F(x)をxで微分する 問6 (2)
        nablaLagragian_x(&dx, is_function->x, is_function->lambda, is_function->a, is_function->b);
        nablaLagragian_lambda(&dlambda, is_function->x, is_function->c);

        // 計算終了条件 解答(2)
        if(!(k < is_env_var->max_loop)) break;
        if((ABS(dx) < is_env_var->eps && ABS(dlambda) < is_env_var->eps))
            break;

        is_function->x = is_function->x + is_env_var->alpha * dx;
        // lambdaの計算 解答(1)
        is_function->lambda = is_function->lambda - is_env_var->alpha * dlambda;

        if(k % is_env_var->period == 0)
            show_result_display(k, dx, dlambda,
                is_function->x, is_function->lambda, is_function->a, is_function->c);
        k++;
    }
    show_result_display(k, dx, dlambda,
        is_function->x, is_function->lambda, is_function->a, is_function->b);

    // 解答(3)
    fprintf(calc_log, "a=%1.3le,c=%1.3le,c=%1.3le: x=%1.3le, F(x)=%1.3le\n", 
        is_function->a, is_function->b, is_function->c, is_function->x,
        func(is_function->x, is_function->a, is_function->b)); // func

    fclose(calc_log);
}

// 計算（途中）結果をコンソールに表示する
void show_result_display(int k, double dx, double dlambda, double x, double lambda,double a, double b)
{
    fprintf(stdout, "k=%d, dx_k=%1.3le, dlambda_k=%1.3le, lambda=%1.3le, ", k, dx, dlambda, lambda);
    fprintf(stdout, "x_{k+1}=%1.3le, func(x_{k+1})=%1.3le\n", x, func(x, a, b)); // func
}

// L(x, λ)をxで微分する
void nablaLagragian_x(double *dx, double x, double lambda, double a, double b)
{
    *dx = -(2.0 * (x - a) + b + lambda); // 解答(4)
}

// L(x, λ)をλで微分する
void nablaLagragian_lambda(double *dlambda, double x, double c)
{
    *dlambda = -(x - c);
}

// F(x)関数
double func(double x, double a, double b)
{
    return pow(x - a, 2.0) + (b * x); // 解答(5)
}