#include<stdio.h>
#include<math.h>

#define ABS(x) ((x) < 0.0 ? -(x):(x)) // 絶対値を返す

typedef struct 
{
    double x;
    double a, b, c, d;

} FUNCTION;

// 計算する上で必要な環境変数
typedef struct
{
    int max_loop; // ループ処理の最大回数
    int period; // 計算結果を出力する周期

    double alpha; // 刻み幅
    double eps; // 終了判断基準

} ENV_VAR;

int main();

void Initialization_variable();
void Calculate_optimization();
void show_result_display();

// F(x)をxで微分した結果
void nablaLagragian_x(double *dx, double x, double a, double b, double c, double d);

// F(x)関数
double func(double x, double a, double b, double c, double d);

int main()
{
    FUNCTION function_variable;
    ENV_VAR environment_variable;

    // xの初期値が a - 1 のときの処理
    Initialization_variable(&function_variable, &environment_variable, 7.0, 8.0, 9.0, 11.0, 6.0);
    Calculate_optimization(&function_variable, &environment_variable, "result_4_1.log");
    printf("\n");

    // xの初期値が b + 1 のときの処理
    Initialization_variable(&function_variable, &environment_variable, 7.0, 8.0, 9.0, 11.0, 12.0);
    Calculate_optimization(&function_variable, &environment_variable, "result_4_2.log");

    return 0;
}

// 変数の初期化
void Initialization_variable(FUNCTION *is_function, ENV_VAR *is_env_var,
    double a, double b, double c, double d, double x)
{
    // functions's variable
    is_function->a = a;
    is_function->b = b;
    is_function->c = c;
    is_function->d = d;

    is_function->x = x;

    // environment variable
    is_env_var->max_loop = 1000;
    is_env_var->period = 50;

    is_env_var->alpha = 0.01;
    is_env_var->eps = 0.001;
}

// 計算処理
void Calculate_optimization(FUNCTION *is_function, ENV_VAR *is_env_var, char *file_name)
{
    int k = 0;
    double dx = 0.0;
    FILE *calc_log; // 計算結果を出力するためのファイル
    calc_log = fopen(file_name, "w"); // 書き込み用ファイルを開く

    while(1)
    {
        // F(x)をxで微分する
        nablaLagragian_x(&dx, is_function->x, is_function->a, is_function->b, is_function->c, is_function->d);

        // 計算終了条件
        if(!(k < is_env_var->max_loop)) break;
        if((ABS(dx) < is_env_var->eps))
            break;

        is_function->x = is_function->x + is_env_var->alpha * dx;

        if(k % is_env_var->period == 0)
            show_result_display(k, dx, is_function->x,
                is_function->a, is_function->b, is_function->c, is_function->d);
        k++;
    }
    show_result_display(k, dx, is_function->x,
        is_function->a, is_function->b, is_function->c, is_function->d);

    fprintf(calc_log, "a=%1.3le,b=%1.3le,c=%1.3le,d=%1.3le: x=%1.3le, F(x)=%1.3le\n", 
        is_function->a, is_function->b, is_function->c, is_function->d, is_function->x,
        func(is_function->x, is_function->a, is_function->b,is_function->c, is_function->d));

    fclose(calc_log);
}

// コンソールに計算（途中）結果を表示する
void show_result_display(int k, double dx, double x, double a, double b, double c, double d)
{
    fprintf(stdout, "k=%d, dx_k=%1.3le, ", k, dx);
    fprintf(stdout, "x_{k+1}=%1.3le, func(x_{k+1})=%1.3le\n", x, func(x, a, b, c, d));
}

// F(x)をxで微分する 解答 (6), (7)
// dF(x)/dx = b + c - 2*x + 2*(-a + x)^2 * (-d + x) + 2*(-a + x) * (-d + x)^2
// dF(x)/dx = b + c - 2*x + 2*(x - a)*(x - d)*(2*x - a - d)
void nablaLagragian_x(double *dx, double x, double a, double b, double c, double d)
{
    *dx = -(b + c -2.0 * x + 2.0*(x - a) * (x - d) * (2.0 * x - a - d));
}

//F(x) = (x - a)^2 * (x - d)^2 - (x - b) * (x - c)
double func(double x, double a, double b, double c, double d)
{
    return pow(x - a, 2.0) * pow(x - d, 2.0) - (x - b) * (x - c);
}