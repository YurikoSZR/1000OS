#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_PERMUTATIONS 1000000  // 预分配最大排列数（根据需求调整）

// 全局共享数据结构
typedef struct {
    int **permutations;   // 存储所有排列的二维数组
    int count;            // 当前已生成的排列数
    int max_elements;     // 排列元素数量
    pthread_mutex_t lock; // 互斥锁保护共享数据
} SharedData;

// 初始化共享数据
void init_shared_data(SharedData *data, int max_elements) {
    data->permutations = malloc(MAX_PERMUTATIONS * sizeof(int*));
    for (int i = 0; i < MAX_PERMUTATIONS; i++) {
        data->permutations[i] = malloc(max_elements * sizeof(int));
    }
    data->count = 0;
    data->max_elements = max_elements;
    pthread_mutex_init(&data->lock, NULL);
}

// 释放共享数据内存
void free_shared_data(SharedData *data) {
    for (int i = 0; i < MAX_PERMUTATIONS; i++) {
        free(data->permutations[i]);
    }
    free(data->permutations);
    pthread_mutex_destroy(&data->lock);
}

// 递归生成排列的核心函数
void generate_permutations(int *prefix, int prefix_len, int *remaining, int remaining_len, SharedData *data) {
    if (remaining_len == 0) {
        // 安全地将排列添加到全局列表
        pthread_mutex_lock(&data->lock);
        if (data->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < data->max_elements; i++) {
                data->permutations[data->count][i] = prefix[i];
            }
            data->count++;
        }
        pthread_mutex_unlock(&data->lock);
        return;
    }

    for (int i = 0; i < remaining_len; i++) {
        // 创建新的剩余数组副本（避免线程竞争）
        int *new_remaining = malloc((remaining_len - 1) * sizeof(int));
        for (int j = 0, k = 0; j < remaining_len; j++) {
            if (j != i) new_remaining[k++] = remaining[j];
        }

        // 扩展前缀
        int *new_prefix = malloc((prefix_len + 1) * sizeof(int));
        memcpy(new_prefix, prefix, prefix_len * sizeof(int));
        new_prefix[prefix_len] = remaining[i];

        // 递归生成
        generate_permutations(new_prefix, prefix_len + 1, new_remaining, remaining_len - 1, data);

        free(new_prefix);
        free(new_remaining);
    }
}

// 线程函数：处理以特定元素开头的排列
typedef struct {
        int start_element;
        int *elements;
        int n;
        SharedData *data;
} ThreadArg;


void* thread_func(void *arg) {
 

    ThreadArg *args = (ThreadArg*)arg;
    int n = args->n;

    // 创建初始前缀和剩余数组
    int *remaining = malloc((n - 1) * sizeof(int));
    for (int i = 0, j = 0; i < n; i++) {
        if (args->elements[i] != args->start_element) {
            remaining[j++] = args->elements[i];
        }
    }

    int prefix[1] = {args->start_element};
    generate_permutations(prefix, 1, remaining, n - 1, args->data);

    free(remaining);
    free(arg); // 释放线程参数内存
    return NULL;
}

int main() {
    int elements[] = {1, 2, 3, 4, 5, 6, 7, 8,9,10}; // 示例输入
    int n = sizeof(elements)/sizeof(elements[0]);

    // 初始化共享数据
    SharedData data;
    init_shared_data(&data, n);

    // 创建线程：每个线程处理一个起始元素
    pthread_t threads[n];
    for (int i = 0; i < n; i++) {
        ThreadArg *arg = malloc(sizeof(ThreadArg));
        arg->start_element = elements[i];
        arg->elements = elements;
        arg->n = n;
        arg->data = &data;
        pthread_create(&threads[i], NULL, thread_func, arg);
    }

    // 等待所有线程完成
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // 打印结果
    printf("Total permutations: %d\n", data.count);
    for (int i = 0; i < data.count; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", data.permutations[i][j]);
        }
        printf("\n");
    }

    // 清理资源
    free_shared_data(&data);
    return 0;
}
