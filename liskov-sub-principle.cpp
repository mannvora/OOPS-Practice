#include<iostream>
using namespace std;

class vehicle {
	public:
		virtual int numberOfWheels() {
			return 2;
		}

		virtual bool hasEngine() {
			return true;
		}
};

class MotorCycle: public vehicle {};

class car: public vehicle {
	int numberOfWheels() {
		return 4;
	}
};

int main() {
	vehicle* vc = new MotorCycle;
	cout << vc->numberOfWheels() << "\n";
}