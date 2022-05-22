/*mod_test.cu*/
#include "cuda_runtime.h"
#include "cublas_v2.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <stdio.h>

using namespace std;

__global__ void mod_kernel(double* d_M1, double* d_M2, long q, int n) {
    // index
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    long x;
    if (idx < n) {
        x = d_M1[idx];
        // printf("x: %d\n", x)
        d_M2[idx] = x % q;
    }
}

long** mat_mod_cu(long** M1, long rowSize, long colSize, long q, cublasHandle_t cuHandle) {
    // 1.定义结果矩阵，用于返回
    long** M2 = new long* [rowSize];
    for (long i = 0; i < rowSize; i++)
        M2[i] = new long[colSize];

    // 2.分配CPU资源
    double* h_M1 = (double*)malloc(rowSize * colSize * sizeof(double));
    double* h_M2 = (double*)malloc(rowSize * colSize * sizeof(double));
    // 初始化h_M1
    for (long i = 0; i < rowSize; i++) {
        for (long j = 0; j < colSize; j++) {
            h_M1[i * colSize + j] = (double)M1[i][j];
        }
    }

    // 3.分配GPU资源
    double* d_M1;
    double* d_M2;
    cudaMalloc((void**)&d_M1, rowSize * colSize * sizeof(double));
    cudaMalloc((void**)&d_M2, rowSize * colSize * sizeof(double));
    // 将CPU数据拷贝到GPU上
    cudaMemcpy(d_M1, h_M1, rowSize * colSize * sizeof(double), cudaMemcpyHostToDevice);

    // 4.定义kernel的执行配置
    int blockSize = 1024;
    int minGridSize128;
    int gridSize;
    // 获取GPU的信息，并配置最优参数
    //cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, mod_kernel, 0, rowSize * colSize);
    gridSize = (rowSize * colSize + blockSize - 1) / blockSize;

    // 5.执行核函数
    // 取模
    mod_kernel <<< gridSize, blockSize >>> (d_M1, d_M2, q, rowSize * colSize);

    // 6.将GPU数据拷贝到CPU上
    cudaMemcpy(h_M2, d_M2, rowSize * colSize * sizeof(double), cudaMemcpyDeviceToHost);

    // 7.赋值给结果矩阵
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            M2[i][j] = static_cast<long>(h_M2[i * colSize + j]);
        }
    }

    // 8.清理掉使用过的内存
    free(h_M1); free(h_M2);
    cudaFree(d_M1); cudaFree(d_M2);

    return M2;
}

long** uniformMat(long rowSize, long colSize, long minValue, long maxValue) {
    long** mat = new long* [rowSize];
    for (long i = 0; i < rowSize; i++)
        mat[i] = new long[colSize];

    srand((unsigned)time(NULL));
    for (long i = 0; i < rowSize; i++) {
        for (long j = 0; j < colSize; j++) {
            mat[i][j] = (long)(rand() % (maxValue - minValue + 1)) + minValue;
        }
    }

    return mat;
}

int main(void) {
    // 1.定义cuda消息处理器
    cublasHandle_t cuHandle;
    cublasStatus_t status = cublasCreate(&cuHandle);

    // 2.定义要取模的矩阵
    long q = 3; // 取模数
    long rowSize = 4; // 矩阵行数
    long colSize = 6; // 矩阵列数
    long** M1 = uniformMat(rowSize, colSize, 5, 9); // 生成一个rowSize*colSize的随机矩阵，元素值在[5, 9]内随机选取

    // 3.进行矩阵元素取模运算
    long** M2 = mat_mod_cu(M1, rowSize, colSize, q, cuHandle);

    // 4.输出原矩阵M1和取模后的矩阵M2
    cout << "M1: " << endl;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            cout << M1[i][j] << " ";
        }
        cout << endl;
    }
    cout << "M2: " << endl;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            cout << M2[i][j] << " ";
        }
        cout << endl;
    }

    // 5.销毁cuda消息处理器
    cublasDestroy(cuHandle);

    return 0;
}