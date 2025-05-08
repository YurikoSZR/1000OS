// #include <stdio.h>

// void hanoi(){
//     printf("A,B,C\n");
// }

// int main(){
//     // asm volatile(
//     //     "call  %0\n"
//     //     :
//     //     : "r"(&hanoi)     
//     // );
//     __asm__ __volatile__(
//         "call hanoi\n"
//     );
// }




#include <stdio.h>
#include <stdint.h>

typedef struct {
    uintptr_t rip, rsp;
    uintptr_t rbx, r12, r13, r14, r15;
} context_t;

// 保存当前上下文
void save_context(context_t *ctx) {
    asm volatile (
        "mov %%rip, %0\n"
        "mov %%rsp, %1\n"
        "mov %%rbx, %2\n"
        "mov %%r12, %3\n"
        "mov %%r13, %4\n"
        "mov %%r14, %5\n"
        "mov %%r15, %6"
        : "=m"(ctx->rip), "=m"(ctx->rsp),
          "=m"(ctx->rbx), "=m"(ctx->r12), "=m"(ctx->r13),
          "=m"(ctx->r14), "=m"(ctx->r15)
        : 
        : "memory"
    );
}

// 恢复目标上下文
void restore_context(context_t *ctx) {
    asm volatile (
        "mov %1, %%rsp\n"
        "mov %2, %%rbx\n"
        "mov %3, %%r12\n"
        "mov %4, %%r13\n"
        "mov %5, %%r14\n"
        "mov %6, %%r15\n"
        "jmp *%0"
        : 
        : "m"(ctx->rip), "m"(ctx->rsp),
          "m"(ctx->rbx), "m"(ctx->r12), "m"(ctx->r13),
          "m"(ctx->r14), "m"(ctx->r15)
        : "memory"
    );
}

// 协程入口函数
void coroutine_entry() {
    printf("Coroutine is running!\n");
    // 此处可切换回主上下文
}

context_t main_ctx, coro_ctx;

int main() {
    char coro_stack[4096];
    
    // 初始化协程上下文
    save_context(&coro_ctx);
    coro_ctx.rip = (uintptr_t)coroutine_entry;
    coro_ctx.rsp = (uintptr_t)(coro_stack + 4096 - 8); // 栈顶（16 字节对齐）

    // 切换到协程
    save_context(&main_ctx);
    restore_context(&coro_ctx);

    printf("Back to main!\n");
    return 0;
}