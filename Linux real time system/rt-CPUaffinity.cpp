#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

// 스레드 함수
void* thread_function(void* arg) {
    int thread_num = *(int*)arg;


    //cpuset을 초기화
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);  // 초기화된 상태에서는 어떤 CPU 코어도 사용하지 않음

    // 스레드를 CPU 0에 고정
    CPU_SET(0, &cpuset);


    // 현재 스레드의 CPU Affinity 설정
    // pthread_self() 는 current_thread 에 현재 스레드의 thread ID 를 반환한다. 
    // cpu_set_t 구조체의 크기를 전달하는 이유는 CPU 프로세스와 관련된 내용이기 떄문에 좀더 정확하고 안전하게 하기 위해 전달한다고 한다.
    pthread_t current_thread = pthread_self();
    if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Error setting CPU affinity for thread " << thread_num << std::endl;
        return NULL;
    }


    // 스레드가 실행되는 CPU 확인
    // pthread_getaffinity_np 는 확인 하는 함수 위의set과 다름
    // CPU_SETSIZE 는 POSIX 표준에 정의된 상수
    CPU_ZERO(&cpuset);
    if (pthread_getaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) == 0) {
        std::cout << "Thread " << thread_num << " is running on CPU(s): ";
        for (int cpu = 0; cpu < CPU_SETSIZE; cpu++) {
            // CPU_ISSET 은 cpuset 에서 특정 CPU 코어가 설정되어있는 지 확인하는 함수래
            if (CPU_ISSET(cpu, &cpuset)) {
                std::cout << cpu << " ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Error getting CPU affinity for thread " << thread_num << std::endl;
    }

    // 간단한 작업 수행 (계속 실행)
    for (int i = 0; i < 10; i++) {
        std::cout << "Thread " << thread_num << " is working..." << std::endl;
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread1_num = 1;
    int thread2_num = 2;

    // 스레드 생성
    pthread_create(&thread1, NULL, thread_function, &thread1_num);
    pthread_create(&thread2, NULL, thread_function, &thread2_num);

    // 스레드 종료 대기
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}


// 만약 같은 함수 main 과 같이 thread1, thread2 가 이렇게 고정되면 그냥 두개가 우선순위를 두고 싸움
