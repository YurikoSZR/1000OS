#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define NUM_PROD 2   // 生产者数量
#define NUM_CONS 2   // 消费者数量

// 共享缓冲区结构
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;          // 生产者写入位置
    int out;         // 消费者读取位置
} shared_buffer;

shared_buffer sb = { {0}, 0, 0 };

// 信号量声明
sem_t mutex;        // 互斥信号量
sem_t empty;        // 空槽数量
sem_t full;         // 已填充数量

// 初始化信号量
void init_semaphores() {
    sem_init(&mutex, 0, 1);              // 互斥锁初始为1
    sem_init(&empty, 0, BUFFER_SIZE);    // 初始空槽数量
    sem_init(&full, 0, 0);               // 初始已填充数量
}

// 生产者线程
void *producer(void *arg) {
    int item = 0;
    while (1) {
        // 生产数据
        usleep(100000 + rand() % 150000); // 模拟生产时间
        
        sem_wait(&empty);  // 等待空槽
        sem_wait(&mutex);  // 进入临界区
        
        // 写入缓冲区
        sb.buffer[sb.in] = item++;
        sb.in = (sb.in + 1) % BUFFER_SIZE;
        
        printf("[Producer %ld] Produced: %d\n", (long)arg, item);
        fflush(stdout);
        
        sem_post(&mutex);  // 离开临界区
        sem_post(&full);   // 增加已填充计数
    }
    return NULL;
}

// 消费者线程
void *consumer(void *arg) {
    while (1) {
        sem_wait(&full);   // 等待已填充项
        sem_wait(&mutex);  // 进入临界区
        
        // 读取缓冲区
        int item = sb.buffer[sb.out];
        sb.out = (sb.out + 1) % BUFFER_SIZE;
        
        printf("[Consumer %ld] Consumed: %d\n", (long)arg, item);
        fflush(stdout);
        
        sem_post(&mutex);  // 离开临界区
        sem_post(&empty);  // 增加空槽计数
        
        // 消费数据
        usleep(200000 + rand() % 250000); // 模拟消费时间
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PROD], consumers[NUM_CONS];
    
    init_semaphores();
    
    // 创建生产者线程
    for (long i = 0; i < NUM_PROD; i++) {
        pthread_create(&producers[i], NULL, producer, (void*)i);
    }
    
    // 创建消费者线程
    for (long i = 0; i < NUM_CONS; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void*)i);
    }
    
    // 等待线程（实际不会返回）
    for (int i = 0; i < NUM_PROD; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONS; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    // 销毁信号量
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    return 0;
}