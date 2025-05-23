创建线程：
````
pthread_t thread_id;
int ret = pthread_create(
    &thread_id,    // 线程ID指针
    NULL,          // 线程属性（默认NULL）
    thread_function, // 线程函数
    (void*)arg     // 传递给线程的参数（可以是任意类型指针）
);

if (ret != 0) {
    perror("pthread_create failed");
}
````
阻塞主线程(进程），直到特定进程结束：
````
void* thread_result;
pthread_join(thread_id, &thread_result); // 获取线程返回值
````

Mutex
````
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 加锁
pthread_mutex_lock(&mutex);
// 操作共享资源...
// 解锁
pthread_mutex_unlock(&mutex);

// 销毁锁
pthread_mutex_destroy(&mutex);
````


