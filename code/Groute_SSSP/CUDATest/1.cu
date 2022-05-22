#include <iostream>
#include <cuda_runtime.h>
#include "main.cuh"
int main(void) {
	kernel<<<1,1>>> ();
	std::cout << "finished" << std::endl;
	return 0;
}