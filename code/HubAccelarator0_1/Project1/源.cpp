#include<iostream>
using namespace std;
int main() {
	double a = -228955 / 12.0;
	double b = 419751 / 2.0;
	double c = -782263.4176;
	double d = 1163856.5;
	double e = -572355.9991;
	for (int i = 1; i < 6; i++) {
		cout << a * i * i * i * i + b * i * i * i + c * i * i + d * i + e << endl;
	}
}