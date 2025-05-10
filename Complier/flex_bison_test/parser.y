%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 声明词法分析器函数和错误函数
extern int yylex();
extern void yyerror(const char *s);

// 用于传递数值的语义类型（与Flex的yylval对应）
typedef union {
    int int_val;      // 存储数值的实际整数
    char str_val[32]; // 存储原始字符串（如"0x1F"）
} YYSTYPE;

// 声明语义值类型
#define YYSTYPE YYSTYPE

%}

/* Token 声明 */
%token NUMBER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
%token EOL

/* 运算符优先级和结合性 */
%left ADD SUB    // 加减法，左结合
%left MUL DIV    // 乘除法，优先级高于加减，左结合
%precedence UMINUS  // 处理负号（此处未实现，仅示例）

%%

/* 语法规则 */
input:
    /* 空 */
    | input line
;

line:
    EOL                { /* 空行 */ }
    | expression EOL   { printf("Result: %d\n", $1.int_val); }
;

expression:
    term               { $$ = $1; }              // expression → term
    | expression ADD term { $$.int_val = $1.int_val + $3.int_val; } // 加法
    | expression SUB term { $$.int_val = $1.int_val - $3.int_val; } // 减法
;

term:
    factor             { $$ = $1; }              // term → factor
    | term MUL factor  { $$.int_val = $1.int_val * $3.int_val; } // 乘法
    | term DIV factor  { 
                        if ($3.int_val == 0) {
                            yyerror("Division by zero!");
                            exit(1);
                        }
                        $$.int_val = $1.int_val / $3.int_val; 
                      } // 除法
;

factor:
    NUMBER             { $$.int_val = $1.int_val; } // 直接使用数值
    | LPAREN expression RPAREN { $$ = $2; }       // 括号优先级
;

%%

/* 自定义错误报告函数 */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* 主函数 */
int main() {
    yyparse();  // 启动语法分析器
    return 0;
}
