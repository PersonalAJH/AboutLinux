#include <iostream>
#include <math.h>

// 해당예제는 엔비디아 CUDA를 사용한 간단한 예시로 두 벡터의 각 요소를 더하는 기본적인 병렬 덧셈 프로그램


// CUDA 커널 함수 정의
__global__ void add(int n, float *x, float *y) {            // __global__ 키워드는 이 함수가 GPU에서 실행될 CUDA 커널임을 나타낸다. 
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    for (int i = index; i < n; i += stride)
        y[i] = x[i] + y[i];
}

int main(void) {
    int N = 1<<20; // 1M 요소
    float *x, *y, *d_x, *d_y;

    // 호스트 메모리 할당 -> CPU에 메모리를 할당한다. 
    x = (float*)malloc(N*sizeof(float));
    y = (float*)malloc(N*sizeof(float));

    // GPU 메모리 할당 -> 필요한 공간을 할당하고 초기화 시킨다. d_x, d_y 는 GPU 메모리 내의 배열을 가리키는 포인터 
    cudaMalloc(&d_x, N*sizeof(float)); 
    cudaMalloc(&d_y, N*sizeof(float));

    // 호스트에서 데이터 초기화
    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    // 데이터를 GPU로 복사 -> 초기화된 데이터를 호스트(CPU)에서 디바이스(GPU)로 이동시킨다. 
    cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

    // CUDA 커널 실행
    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;
    add<<<numBlocks, blockSize>>>(N, d_x, d_y);

    // 결과를 호스트로 복사
    cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);

    // 결과 확인
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(y[i]-3.0f));
    std::cout << "Max error: " << maxError << std::endl;

    // 메모리 해제
    cudaFree(d_x);
    cudaFree(d_y);
    free(x);
    free(y);
    
    return 0;
}



// CUDA는 딥 러닝 분야에서 주로 대규모 신경망 모델의 훈련과 추론에 사용됩니다. 딥 러닝 모델, 특히 깊은 신경망과 컨볼루션 신경망은 많은 양의 데이터와 복잡한 계산이 필요하기 때문에, CUDA를 사용한 GPU 가속은 이러한 모델을 훈련하고 실행하는 데 있어서 중요한 역할을 합니다.
// torch 와 연계해서도 사용할 수 있는것 같음 