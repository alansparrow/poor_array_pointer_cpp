// PoorArrayPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include<vector>

using namespace std;

class Fruit {
public:
	int x, y;
	Fruit() : x{ 0 }, y{ 0 } {}
	Fruit(int xx, int yy) : x{ xx }, y{ yy } {}
};

class Apple : public Fruit {
public:
	int x1, y1;
	Apple() : Fruit(1, 1), x1{ 0 }, y1{ 0 } {}
	Apple(int xx1, int yy1) : Fruit(1, 1), x1{ xx1 }, y1{ yy1 } {}
};

void PoorFunction(Fruit *f, int sz) {
	printf("%p %p %p\n", &f[0], &f[1], &f[2]);
	printf("%p %p %p\n", f, (f+1), (f+2));

	Apple* a = static_cast<Apple*>(f);
	printf("casting...\n");

	printf("%p %p %p\n", &a[0], &a[1], &a[2]);
	printf("%p %p %p\n", a, (a + 1), (a + 2));

}


namespace NS1 {
	template<typename T>
	class Array_ref {
	public:
		template<typename Q>
		operator const Array_ref<const Q>() {
			static_cast<Q>(*static_cast<T*>(nullptr)); // check element conversion

			return Array_ref<const Q>{reinterpret_cast<Q*>(p), sz};
		}
		//Array_ref(Array_ref&& ar) : p{ ar.p }, sz{ ar.sz } {}
		Array_ref(T* pp, int s) : p{ pp }, sz{ s } {}
		T& operator[](int n) { return p[n]; }
		const T& operator[](int n) const { return p[n]; }

		bool assign(Array_ref a) {
			if (a.sz != sz) return false;
			for (int i = 0; i < sz; i++) p[i] = a.p[i];
			return true;
		}

		void reset(Array_ref a) { reset(a.p, a.sz); }
		void reset(T* pp, int s) { p = pp; sz = s; }

		int size() const { return sz; }

		// default copy operations:
		// Array_ref doesn't own any resources
		// Array_ref has reference semantics
	private:
		T * p;
		int sz;
	};

	template<typename T>
	Array_ref<T> make_ref(T* pp, int s) {
		return (pp) ? Array_ref<T>{pp, s} : Array_ref<T>{ nullptr, 0 };
	}

	template<typename T>
	Array_ref<T> make_ref(vector<T>& v) {
		return (v.size()) ? Array_ref<T>{&v[0], v.size()} : Array_ref<T>{ nullptr, 0 };
	}

	template<typename T, int s>
	Array_ref<T> make_ref(T(&pp)[s]) {
		return Array_ref<T>{pp, s};
	}

	
};

using namespace NS1;

void f1() {
	Apple a[20];

	PoorFunction(a, 20);
}

void better(Array_ref<Fruit> a) {

}

void better2(const Array_ref<Fruit* const> a) {

}


void f2() {
	Fruit fr[20];
	Apple a[30];

	NS1::make_ref<Fruit>(fr);
	better(NS1::make_ref<Fruit>(fr));
	NS1::make_ref<Apple>(a);
	//better(NS1::make_ref<Apple>(a));

	Fruit* fptr = new Fruit();
	NS1::make_ref(fptr, 1); // OK: one element
	delete fptr;
	fptr = 0;
	NS1::Array_ref<Fruit> r1{ NS1::make_ref(fptr, 1) }; // OK: no element

	cout << endl;
}

void f3() {
	Apple* a2[20];
	Fruit* f1[10];
	
	Fruit* f2 = new Fruit();

	better2(make_ref(f1));
	better2(make_ref(a2));
	//better2(make_ref(f2));
}

int main()
{
	printf("sizeof(Fruit): %d\nsizeof(Apple): %d\n", sizeof(Fruit), sizeof(Apple));
	f2();
	f3();
    return 0;
}

