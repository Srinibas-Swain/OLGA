

#include <array>
#include <vector>
#include <iostream>

using namespace std;

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
		//hi();
	}
};

class Car : public Vehicle {
public:
	int ix;
	Car(int _ix) {
		ix = _ix;
	}
	void hi() {
		cout << "car.hi: " << ix << endl;
	}
	~Car() { cout << "~car" << ix << endl; }
};

typedef std::vector<Car*> Cars;

Cars* init(const int n)
{
	Cars* cars;
	cars = new Cars[n];
	for(int ix=0; ix<n; ix++) {
		cars[ix] = std::vector<Car*>();
		for(int jx=0; jx<n; jx++) {
			cars[ix].push_back(new Car(ix*100 + jx));
		}
	}
	return cars;
}


int main()
{
	/* checking for persistence of vector<custom-class> pointers across function boundaries
	 * 
	int n=5;
	Cars* cars = init(n);
	for(int ix=0; ix<n; ix++) {
		for(int jx=0; jx<n; jx++) {
			cars[ix][jx]->hi();
		}
	}
	
	for(int ix=0; ix<n; ix++) {
		for(int jx=0; jx<n; jx++) {
			delete(cars[ix][jx]);
		}
	}
	delete[](cars);
	*/
}











