#include <iostream>
#include <cuda_runtime.h>

__global__ void kernel(void) {

}

int main(void) {
	kernel <<<1, 1 >>> ();
	std::cout << "finished" << std::endl;
	return 0;
}