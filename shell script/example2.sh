#!/bin/bash

# 새 사용자 추가
add_user() {
    sudo adduser $1
}

# 사용자 삭제
delete_user() {
    sudo deluser $1
}

# 사용법: ./script.sh add/delete 사용자명
if [ "$1" == "add" ]; then
    add_user $2
elif [ "$1" == "delete" ]; then
    delete_user $2
else
    echo "Usage: $0 add/delete username"
fi



# $은 reference 역할을 하며 그 함수 add_user [a] [b] [c] 일떄 a = $1 b = $2 c = $3 을 의미함 
# shell 에는 main 문이 따로없고 그냥 쓰면됨 