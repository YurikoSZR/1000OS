#include <stdarg.h>
#include <stdio.h>
//va_arg(args,TYPE): 从可变参数列表args中取出类型为TYPE的下一个参数，并将其返回

// 函数原型：第一个参数是参数数量，后续为可变参数
int max(int count, ...) {
    if (count <= 0) return 0; // 参数数量需 ≥1

    va_list args;
    va_start(args, count);   // 初始化可变参数列表

    int max_val = va_arg(args, int); // 取第一个参数作为初始最大值

    // 遍历剩余参数
    for (int i = 1; i < count; i++) {
        int current = va_arg(args, int);
        if (current > max_val) {
            max_val = current;
        }
    }

    va_end(args); // 清理参数列表
    return max_val;
}

int main() {
    printf("max(3, 10, 20, 30) -> %d\n", max(3, 10, 20, 30));   // 30
    printf("max(5, -5, 0, 3, 1, 2) -> %d\n", max(5, -5, 0, 3, 1, 2)); // 3
    printf("max(1, 100) -> %d\n", max(1, 100));                 // 100
    return 0;
}