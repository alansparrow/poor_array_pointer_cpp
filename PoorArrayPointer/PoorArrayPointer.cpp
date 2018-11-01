// PoorArrayPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

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

int main()
{
	printf("sizeof(Fruit): %d\nsizeof(Apple): %d\n", sizeof(Fruit), sizeof(Apple));

	Apple a[20];

	PoorFunction(a, 20);
    return 0;
}

