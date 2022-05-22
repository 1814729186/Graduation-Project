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
    // 1.�������������ڷ���
    long** M2 = new long* [rowSize];
    for (long i = 0; i < rowSize; i++)
        M2[i] = new long[colSize];

    // 2.����CPU��Դ
    double* h_M1 = (double*)malloc(rowSize * colSize * sizeof(double));
    double* h_M2 = (double*)malloc(rowSize * colSize * sizeof(double));
    // ��ʼ��h_M1
    for (long i = 0; i < rowSize; i++) {
        for (long j = 0; j < colSize; j++) {
            h_M1[i * colSize + j] = (double)M1[i][j];
        }
    }

    // 3.����GPU��Դ
    double* d_M1;
    double* d_M2;
    cudaMalloc((void**)&d_M1, rowSize * colSize * sizeof(double));
    cudaMalloc((void**)&d_M2, rowSize * colSize * sizeof(double));
    // ��CPU���ݿ�����GPU��
    cudaMemcpy(d_M1, h_M1, rowSize * colSize * sizeof(double), cudaMemcpyHostToDevice);

    // 4.����kernel��ִ������
    int blockSize = 1024;
    int minGridSize128;
    int gridSize;
    // ��ȡGPU����Ϣ�����������Ų���
    //cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, mod_kernel, 0, rowSize * colSize);
    gridSize = (rowSize * colSize + blockSize - 1) / blockSize;

    // 5.ִ�к˺���
    // ȡģ
    mod_kernel <<< gridSize, blockSize >>> (d_M1, d_M2, q, rowSize * colSize);

    // 6.��GPU���ݿ�����CPU��
    cudaMemcpy(h_M2, d_M2, rowSize * colSize * sizeof(double), cudaMemcpyDeviceToHost);

    // 7.��ֵ���������
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            M2[i][j] = static_cast<long>(h_M2[i * colSize + j]);
        }
    }

    // 8.�����ʹ�ù����ڴ�
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
    // 1.����cuda��Ϣ������
    cublasHandle_t cuHandle;
    cublasStatus_t status = cublasCreate(&cuHandle);

    // 2.����Ҫȡģ�ľ���
    long q = 3; // ȡģ��
    long rowSize = 4; // ��������
    long colSize = 6; // ��������
    long** M1 = uniformMat(rowSize, colSize, 5, 9); // ����һ��rowSize*colSize���������Ԫ��ֵ��[5, 9]�����ѡȡ

    // 3.���о���Ԫ��ȡģ����
    long** M2 = mat_mod_cu(M1, rowSize, colSize, q, cuHandle);

    // 4.���ԭ����M1��ȡģ��ľ���M2
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

    // 5.����cuda��Ϣ������
    cublasDestroy(cuHandle);

    return 0;
}