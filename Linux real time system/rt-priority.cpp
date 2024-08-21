#include "rt.h"

int linux_realtime_priority_setting()
{
	struct sched_param sp = { .sched_priority = 1 };
    int policy;
	policy = SCHED_FIFO;        //FIFO 실시간 정책입니다. 같은 우선순위를 가진 프로세스 간에는, 먼저 대기열에 들어온 프로세스가 먼저 실행됩니다.
    policy = SCHED_RR;          //Round-Robin 실시간 정책입니다. 이 정책은 동일한 우선순위를 가진 프로세스들에게 시간 할당량을 순환식으로 분배합니다.
    policy = SCHED_DEADLINE;    //각 프로세스에게 실행 마감 시간을 부여합니다. 이 정책은 마감 시간이 가장 임박한 작업에게 우선순위를 부여합니다.
    policy = SCHED_OTHER;       //이것은 기본적인 타임쉐어링 스케줄링 정책입니다. 동적 우선순위 할당을 사용하며, 일반 사용자 프로세스에 적합합니다.
 


    //int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param); 함수 원형
    // pid : 프로세서의 id 이고 0 일시는 현재 프로세스에 대한 정책임, policy는 위에 4개 
	int error = sched_setscheduler(0, policy, &sp);

	if( error != 0 ) {
        // 실패시 1을 반환 
		perror("sched_setcheduler error \n");
	}
}
