#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

void* thread_function(void* arg) {
    pthread_mutex_lock(&lock); // 뮤텍스 잠금


    /*
        // 뮤텍스가 잠겨있는 동안 작업을 수행하는 곳 
        printf("Thread %d is running\n", *((int*)arg));




    */
    pthread_mutex_unlock(&lock); // 뮤텍스 잠금 해제
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread1_id = 1;
    int thread2_id = 2;

    // 뮤텍스 초기화
    pthread_mutex_init(&lock, NULL);

    // 두 개의 스레드 생성
    pthread_create(&thread1, NULL, thread_function, &thread1_id);
    pthread_create(&thread2, NULL, thread_function, &thread2_id);

    // 스레드가 종료될 때까지 대기
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 뮤텍스 해제
    pthread_mutex_destroy(&lock);

    return 0;
}
