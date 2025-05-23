#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define NUM_PROD 2   // 生产者数量
#define NUM_CONS 2   // 消费者数量

// 共享缓冲区结构
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;          // 生产者写入位置
    int out;         // 消费者读取位置
    int count;       // 当前元素数量
    int spin_lock;   // 自旋锁（0=解锁，1=加锁）
} shared_buffer;

shared_buffer sb = { {0}, 0, 0, 0, 0 };

// 自旋锁实现（x86汇编）
/* 内联汇编阅读：
    3f: 向下读取标签3
    2b 1b:向上读取标签2，标签1
*/
void spin_lock(int *lock) {
    asm volatile (
        "1:\n"
        "mov $1, %%eax\n"     // 将1放入eax寄存器
        "xchg %%eax, %0\n"   // 原子交换eax和lock的值
        "test %%eax, %%eax\n" // 测试eax是否为0
        "jz 3f\n"            // 如果为0则获取锁成功
        "2:\n"
        "pause\n"            // 提示CPU进入忙等待优化状态
        "cmpl $0, %0\n"      // 检查锁是否释放
        "jnz 2b\n"           // 如果仍被锁定则继续等待
        "jmp 1b\n"           // 重新尝试获取锁
        "3:\n"
        : "+m" (*lock)
        :
        : "eax", "memory"
    );
}

void spin_unlock(int *lock) {
    asm volatile (
        "mov $0, %0"  // 将锁置为0
        : "+m" (*lock)
        :
        : "memory"
    );
}

// 生产者线程
void *producer(void *arg) {
    int item = 0;
    while (1) {
        // 生产数据
        usleep(100000 + rand() % 150000); // 模拟生产时间
        
        spin_lock(&sb.spin_lock);
        // 等待缓冲区有空位
        while (sb.count == BUFFER_SIZE) {
            spin_unlock(&sb.spin_lock);
            usleep(1000);  // 避免忙等待消耗过多CPU
            spin_lock(&sb.spin_lock);
        }
        
        // 写入缓冲区
        sb.buffer[sb.in] = item++;
        sb.in = (sb.in + 1) % BUFFER_SIZE;
        sb.count++;
        
        printf("[Producer %ld] Produced: %d\n", (long)arg, item);
        fflush(stdout);
        
        spin_unlock(&sb.spin_lock);
    }
    return NULL;
}

// 消费者线程
void *consumer(void *arg) {
    while (1) {
        spin_lock(&sb.spin_lock);
        // 等待缓冲区有数据
        while (sb.count == 0) {
            spin_unlock(&sb.spin_lock);
            usleep(1000);  // 避免忙等待消耗过多CPU
            spin_lock(&sb.spin_lock);
        }
        
        // 读取缓冲区
        int item = sb.buffer[sb.out];
        sb.out = (sb.out + 1) % BUFFER_SIZE;
        sb.count--;
        
        printf("[Consumer %ld] Consumed: %d\n", (long)arg, item);
        fflush(stdout);
        
        spin_unlock(&sb.spin_lock);
        
        // 消费数据
        usleep(200000 + rand() % 250000); // 模拟消费时间
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PROD], consumers[NUM_CONS];
    
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
    
    return 0;
}