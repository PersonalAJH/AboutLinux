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

void* medium_priority_task(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Medium priority task is running.\n");
        usleep(100000);  
    }
    return NULL;
}

void* High_priority_task(void* arg){
    for(int i = 0; i<1000000; i++){
        pthread_mutex_lock(&mutex);
        //some task if it exist;
        pthread_mutex_unlock(&mutex);
    }
    retrun NULL;
}

// prioirty 를 설정하는 함수, priority 의 값이 높으면 우선순위가 높으며 보통은 1~99 까지라고한다
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
    pthread_t low_thread, medium_thread, high_thread;
    pthread_mutexattr_t mutex_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &mutex_attr);



    // 우선순위는 숫자가 높을수록 높다(스레드가 더 높은 우선순위)
    pthread_create(&low_thread, NULL, low_priority_task, NULL);
    linux_realtime_priority_setting(low_thread, 10);  

    pthread_create(&medium_thread, NULL, medium_priority_task, NULL);
    linux_realtime_priority_setting(medium_thread, 15);

    pthread_create(&high_thread, NULL, High_priority_task, NULL);
    linux_realtime_priority_setting(high_thread, 20);  

    pthread_join(low_thread, NULL);
    pthread_join(medium_thread, NULL);
    pthread_join(high_thread, NULL);

    // **!!이 예시에서는 low_thread 가 돌고 mutex를 가지고 있기 떄문에 이 작업이 끝나게 된다면 medium을 하는것이 아니라 상속받은 high_thread 가 이 작업을 진행한다. **!!

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}




/*
 우선순위 역전방지? 의 예시 


A. 고속 모터 제어 작업 (High Priority Task):
이 작업은 로봇의 모터를 실시간으로 제어하는 중요한 역할을 합니다. 만약 이 작업이 지연된다면, 로봇의 움직임이 제어되지 않거나 충돌할 수 있습니다. 따라서 이 작업은 매우 높은 우선순위를 가져야 합니다.

B. 센서 데이터 수집 작업 (Low Priority Task):
이 작업은 로봇의 여러 센서에서 데이터를 수집하는 역할을 합니다. 이 작업도 중요하지만, 모터 제어만큼 즉각적인 응답을 요구하지는 않습니다. 따라서 이 작업은 낮은 우선순위를 가집니다.

C. 상태 모니터링 작업 (Medium Priority Task):
이 작업은 로봇의 현재 상태를 모니터링하고, 데이터 로깅 등을 수행합니다. 이 작업은 센서 데이터 수집보다는 중요하지만, 모터 제어보다는 덜 중요합니다. 따라서 중간 우선순위를 가집니다.


우선순위 역전 발생 상황:
센서 데이터 수집 작업이 뮤텍스를 소유:
낮은 우선순위의 센서 데이터 수집 작업이 데이터를 공유 메모리에 기록하기 위해 뮤텍스를 잠급니다.

고속 모터 제어 작업이 실행 중:
높은 우선순위의 모터 제어 작업이 실행됩니다. 이 작업도 동일한 공유 메모리에 접근해야 하므로 뮤텍스를 요청합니다. 하지만 이때 뮤텍스는 센서 데이터 수집 작업이 소유하고 있어, 모터 제어 작업은 뮤텍스가 해제될 때까지 대기해야 합니다.

상태 모니터링 작업이 개입:
중간 우선순위의 상태 모니터링 작업이 실행되려고 합니다. 기본적으로, 이 작업은 센서 데이터 수집 작업보다 높은 우선순위를 가지므로 실행됩니다. 하지만 이 작업이 계속 실행되면서 모터 제어 작업은 여전히 뮤텍스를 기다리며 대기하게 됩니다. 이로 인해 우선순위 역전이 발생하게 됩니다.

이 상황에서, 모터 제어 작업은 사실상 중간 우선순위의 작업에 의해 간접적으로 지연되며, 로봇의 모터 제어에 문제가 발생할 수 있습니다.



우선순위 상속 메커니즘:
우선순위 상속이 활성화되면, 낮은 우선순위를 가진 센서 데이터 수집 작업이 뮤텍스를 소유하고 있을 때, 높은 우선순위의 모터 제어 작업이 뮤텍스를 요청하면 센서 데이터 수집 작업이 임시로 높은 우선순위를 상속받습니다.
이렇게 되면 센서 데이터 수집 작업은 중간 우선순위의 상태 모니터링 작업보다 높은 우선순위로 실행되어, 빠르게 작업을 완료하고 뮤텍스를 해제할 수 있게 됩니다. 그 후 모터 제어 작업이 뮤텍스를 획득하여 제어 작업을 수행하게 됩니다.
결과적으로, 우선순위 상속을 통해 모터 제어 작업이 적시에 실행될 수 있도록 보장됩니다. 이는 중요한 실시간 작업이 우선순위 역전에 의해 지연되는 것을 방지합니다.



*/