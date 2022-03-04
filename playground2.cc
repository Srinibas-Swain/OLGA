
#include <iostream>

using namespace std;

template <class T>
class Vehicle {
public:
	virtual void hello() {
		cout << "veh.hello" << endl;
		hi();
	}
	virtual void hi() {
		cout << "veh.hi" << endl;
	}
	virtual ~Vehicle() {
		hi();
	}
};

template <class T>
class Car : public Vehicle<T> {
public:
	void hi() {
		cout << "car.hi" << endl;
	}
	~Car() { cout << "~car" << endl; }
};

class Blue {};

template<>
class Car<Blue> {
	
};

int main()
{
	Car<Blue> *car = new Car<Blue>();
	car->hello();
	delete(car);
}
