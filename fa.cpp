#include <cstdio> 
#include <iostream>
#define POSITIVE_OVERFLOW 1
#define NEGATIVE_OVERFLOW -1
#define NO_OVERFLOW 0
#define INT_SIGN 0x80000000
#define INT_SIGN 0x8000000000000000L

/*
			  s
			  ^
			  |
		-------------
		|			|
c2  <---| full add  |<--- c1
		|			|
		-------------
			^	^
			|	|
			a	b
*/

void fullAdd(bool a, bool b, bool c1, bool& c2, bool& s) {
	/*
	0^0^0=0 0|0|0=0
	0^0^1=1 0&0&1=0
	0^1^1=0 0|1|1=1
	1^1^1=1 1&1&1=1
	*/

	if (a^b^c1) {
		if (a&b&c1) {
			c2 = true;
		} else {
			c2 = false;
		}
		s = true;
	} else {
		if (a|b|c1) {
			c2 = true;
		} else {
			c2 = false;
		}
		s = false;
	}
}

int unsignedAdd(unsigned a, unsigned b, unsigned& ans) {
	ans = 0;
	unsigned p = 1;
	bool c1 = false, c2 = false, s = false;
	for (unsigned i=0; i<32; i++) {
		fullAdd(a&p, b&p, c1, c2, s);
		c1 = c2;
		ans |= s<<i;
		p<<=1;
	}
	if (c2) return POSITIVE_OVERFLOW;
	return NO_OVERFLOW;
}

int intAdd(int a, int b, int& ans) {
	bool a_sign = INT_SIGN&a;
	bool b_sign = INT_SIGN&b;
	ans = 0;
	unsigned p = 1;
	bool c1 = false, c2 = false, s = false;
	for (unsigned i=0; i<32; i++) {
		fullAdd(a&p, b&p, c1, c2, s);
		c1 = c2;
		ans |= s<<i;
		p<<=1;
	}
	bool s_bk = s;
	fullAdd(a_sign, b_sign, c1, c2, s);
	if (s&&!s_bk) return NEGATIVE_OVERFLOW;
	if (!s&&s_bk) return POSITIVE_OVERFLOW;
	return NO_OVERFLOW;
}

void statusPrint(int a) {
	switch (a) {
		case -1:
			std::cout << "NEGATIVE_OVERFLOW" << std::endl;
			break;
		case 0:
			std::cout << "NO_OVERFLOW" << std::endl;
			break;
		case 1:
			std::cout << "POSITIVE_OVERFLOW" << std::endl;
			break;
		default: break;
	}
}

void anserPrint(int a, int b, int c) {
	std::cout << std::dec << a << " + " << b << " = " << c << std::endl;
	std::cout << std::hex << a << " + " << b << " = " << c << std::endl << std::endl;
}
void anserPrint(unsigned a, unsigned b, unsigned c) {
	std::cout << std::dec << a << " + " << b << " = " << c << std::endl;
	std::cout << std::hex << a << " + " << b << " = " << c << std::endl << std::endl;
}
void Demo1() {
	std::cout << "Demo unsigned" << std::endl;
	// no overflow
	unsigned a = 0x12345678, b = 0xedcba987, c;
	statusPrint(unsignedAdd(a, b, c));
	anserPrint(a, b, c);
	
	//no overflow
	a = 0xffffffff, b = 0x1, c;
	std::cout << a << std::endl;
	statusPrint(unsignedAdd(a, b, c));
	anserPrint(a, b, c);
	
	//overflow
	a = 0xffffffff, b = 0x2, c;
	statusPrint(unsignedAdd(a, b, c));
	anserPrint(a, b, c);
}

void Demo2() {
	std::cout << "Demo int" << std::endl;
	// equals 0x ffff ffff
	int a = 0x12345678, b = 0xedcba987, c;
	statusPrint(intAdd(a, b, c));
	anserPrint(a, b, c);
	
	// 10000...0 + 10000...0 = 00000...0 negative overflow
	a = 0x80000000, b = 0x80000000, c;
	statusPrint(intAdd(a, b, c));
	anserPrint(a, b, c);
	
	// 01000...0 + 01000...0 = 10000...0 positive overflow
	a = 0x70000000, b = 0x70000000, c;
	statusPrint(intAdd(a, b, c));
	anserPrint(a, b, c);
}

main() {
	Demo1();
	Demo2();
}

