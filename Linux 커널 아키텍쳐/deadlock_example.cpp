#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock1, lock2;


//실행 순서
// 1, thread1_function 의 pthread_mutex_lock(&lock1); 실행
// 2. thread2_function의 pthread_mutex_lock(&lock2); 실행
// 3. 둘다 lock 되어있어서 실행이 불가능(pthread_mutex_lock 함수는 닫혀있는 경우 뮤텍스가 풀릴떄까지 대기하게 된다. 그렇기 떄문에 unlock으로 넘어가는게 아니라 계속 대기 상태)


void* thread1_function(void* arg) {
    pthread_mutex_lock(&lock1); // 첫 번째 뮤텍스 잠금
    printf("Thread 1 has locked lock1\n");

    sleep(1); // 다른 스레드에게 실행 기회 제공

    pthread_mutex_lock(&lock2); // 두 번째 뮤텍스 잠금
    printf("Thread 1 has locked lock2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

void* thread2_function(void* arg) {
    pthread_mutex_lock(&lock2); // 두 번째 뮤텍스 잠금
    printf("Thread 2 has locked lock2\n");

    sleep(1); // 다른 스레드에게 실행 기회 제공

    pthread_mutex_lock(&lock1); // 첫 번째 뮤텍스 잠금
    printf("Thread 2 has locked lock1\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
