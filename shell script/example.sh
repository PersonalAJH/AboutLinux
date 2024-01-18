#!/bin/bash
# 쉬뱅은 필수요소는 아니지만 다양한 환경에서 스크립트를 실행할 계획이라면 포함하는게 좋음, 리눅스는 bash 쉘만 하기 떄문에 없어도 될듯?

# 디스크 사용량 임계값 설정 (예: 90%)
THRESHOLD=90

# 'df' 명령어를 사용하여 디스크 사용량을 확인
# '-h': 인간이 읽기 쉬운 형태로 출력
# '--output=pcent,target': 사용량 퍼센트와 마운트 포인트만 출력
df -h --output=pcent,target | while read output;
do
  # 사용량과 마운트 포인트 분리
  usep=$(echo $output | awk '{ print $1}' | cut -d'%' -f1)
  partition=$(echo $output | awk '{ print $2 }')

  if [ $usep -ge $THRESHOLD ]; then
    echo "경고: $partition 파티션의 사용량이 $usep%입니다!"
  fi
done



# do 와 done 이 짝이고 if 와 fi 가 짝임 

# if [ $(($number % 2)) -eq 0 ]; then
#     echo "$number 은(는) 짝수입니다."
# else
#     echo "$number 은(는) 홀수입니다."
# fi

# 이런식으로 그 <> <\> 이것과 같이 앞과 뒤가 모두 있어야 함
