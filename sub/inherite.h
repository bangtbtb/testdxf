#ifndef _INHERITED_TEST_
#define  _INHERITED_TEST_
#include <iostream>

class Base {
public:
	Base()  {}

	virtual void print() = 0;
};


class Derrived : public Base {
public:
	Derrived() : Base() {

	}
	virtual void print();

};

class Derrived2 : public Derrived {
public:
	Derrived2() : Derrived() {

	}
	virtual void print();
};


class Derrived3 : public Derrived2 {
public:
	Derrived3() : Derrived2() {

	}
	virtual void print();
};


#endif // !_INHERITED_TEST_
