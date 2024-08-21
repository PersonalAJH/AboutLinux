#include "rt.h"

pthread_mutex_t mutex;

void* low_priority_task(void* arg){
    pthread_mutex_lock(&mutex);
    printf("Low priority task acquired the mutex.\n");
    // 우선순위는 낮지만 계~~속 하는 함수
    for (int i = 0; i < 1000000000; i++);
    pthread_mutex_unlock(&mutex);
    printf("Low priority task released the mutex.\n");
    return NULL;
}

void* high_priority_task(void* arg){
    // 우선순위도 높고 짧게 계속 가는 함수
    for (int i = 0; i < 100000000; i++);

    pthread_mutex_lock(&mutex);
    printf("High priority task acquired the mutex.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// prioirty 를 설정하는 함수
int linux_realtime_priority_setting(pthread_t thread, int priority) {
    struct sched_param param;
    param.sched_priority = priority;

    if (pthread_setschedparam(thread, SCHED_FIFO, &param) != 0) {
        perror("Failed to set thread priority");
        return -1;
    }

    return 0;
}

int main(){
    pthread_t low_thread, high_thread;
    pthread_mutexattr_t mutex_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &mutex_attr);



    // 우선순위는 숫자가 높을수록 높다(스레드가 더 높은 우선순위)
    pthread_create(&low_thread, NULL, low_priority_task, NULL);
    linux_realtime_priority_setting(low_thread, 10);  

    pthread_create(&high_thread, NULL, high_priority_task, NULL);
    linux_realtime_priority_setting(high_thread, 20);  

    pthread_join(low_thread, NULL);
    pthread_join(high_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}
