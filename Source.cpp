#include <iostream>
#include <iomanip>
#include <cinttypes>
#include <cmath>
#include <time.h>

#include <cstdlib>

void helpMessage();
template<typename T> T inverse(T input, T mod);
template<typename T> bool isPrime(T input);
template<typename T> T gcd_rec(T a, T b, const uint32_t& spacing);
template<typename T> T gcd(T a, T b);
template<typename T> T gcdExtendedRec(T a, T b, T& x, T& y, uint32_t& spacing);
template<typename T> T gcdExtended(T a, T b, T& x, T& y);
template<typename T> T chineseRemainder(T a, T b, T p, T q);
template<typename T> T exponentiation(uint64_t input, T power, T mod);
template<typename T> void rsa(T message);

uint32_t main(int argc, char* argv[]) {

	if (argc == 3) {

		if (argv[1][0] == '-' && argv[1][1] == 'r') {

			int64_t rsa_message = atoll(argv[2]);
			rsa(rsa_message);

		}
		else if (argv[1][0] == '-')
			helpMessage();
		else {

			int64_t gcd_input1 = atoll(argv[1]);
			int64_t gcd_input2 = atoll(argv[2]);

			std::cout << "GCD(" << gcd_input1 << ", " << gcd_input2 << ") = " << gcd(gcd_input1, gcd_input2) << '\n' << std::flush;

		}

	}
	//	eGCD or exponentiation
	else if (argc == 4) {

		if (argv[1][0] == '-' && argv[1][1] == 'e') {

			int64_t gcd_input1 = atoll(argv[2]);
			int64_t gcd_input2 = atoll(argv[3]);
			int64_t gcd_x = 0;
			int64_t gcd_y = 0;
			int64_t output = 0;

			if (gcd_input1 < gcd_input2) {

				gcd_input1 ^= gcd_input2;
				gcd_input2 ^= gcd_input1;
				gcd_input1 ^= gcd_input2;

			}

			std::cout << "gcdExtended:\n";
			output = gcdExtended(gcd_input1, gcd_input2, gcd_x, gcd_y);

			std::cout << "eGCD(" << gcd_input1 << ", " << gcd_input2 << ") = " << output << '\n' << std::flush;
			std::cout << " = (" << gcd_input1 << " * " << gcd_x << ") + (" << gcd_input2 << " * " << gcd_y << ") = " << (gcd_input1 * gcd_x) + (gcd_input2 * gcd_y) << '\n' << std::flush;

		}
		else if (argv[1][0] == '-' && argv[1][1] == 'i') {

			int32_t input = atol(argv[2]);
			int32_t mod = atol(argv[3]);

			std::cout << "Inverse(" << input << ", " << mod << ") = " << inverse(input, mod) << '\n' << std::flush;

		}
		else if (argv[1][0] != '-') {

			int32_t exp_input = atol(argv[1]);
			int32_t exp_pow = atol(argv[2]);
			int32_t exp_mod = atol(argv[3]);

			std::cout << "Exponentiation(" << exp_input << ", " << exp_pow << ", " << exp_mod << ") = " << exponentiation(exp_input, exp_pow, exp_mod) << '\n' << std::flush;

		}
		else
			helpMessage();

	}
	//	Chinese Remainder
	else if (argc == 5) {

		int32_t input1 = atol(argv[1]);
		int32_t mod1 = atol(argv[2]);
		int32_t input2 = atol(argv[3]);
		int32_t mod2 = atol(argv[4]);

		std::cout << "Chinese Remainder(" << input1 << ", " << input2 << ", " << mod1 << ", " << mod2 << ") = " << chineseRemainder(input1, mod1, input2, mod2) << '\n' << std::flush;

	}
	else
		helpMessage();

	return EXIT_SUCCESS;

}

void helpMessage() {

	std::cout << "For GCD, please enter 2 integer values: gcd a b\n";
	std::cout << "For GCD Extended, please enter: gcd -e a b\n";
	std::cout << "For calculating the modular inverse, please enter: gcd -i a mod\n";
	std::cout << "For Exponentiatiation, please enter: gcd a pow mod\n";
	std::cout << "For Chinese Remaidner, please enter: gcd a mod1 b mod2\n";
	std::cout << "For RSA, please enter: gcd -r message(currently only accepting number values)\n" << std::flush;

}

template<typename T>
T inverse(T input, T mod) {

	return ((input % mod) + mod) % mod;

}

template<typename T>
bool isPrime(T input) {

	if (input == 0)
		return false;

	if (input % 2 == 0)		//	check if it is even, if so, it's not prime
		return false;

	for (T val_start = 3; val_start * val_start <= input; val_start++)
		if (input % val_start == 0)
			return false;

	return true;

}

template<typename T>
T gcd_rec(T a, T b, const uint32_t& spacing) {

	//	calculate the quotient and remainder of the current iteration of GCD
	T quotient = (T)a / (T)b;
	T remainder = a % b;

	//	output all relevant data
	std::cout << std::setw(spacing) << a << std::setw(spacing) << b << std::setw(spacing) << quotient << std::setw(spacing) << remainder << '\n' << std::flush;

	//	check if a was divisible by b, if so, return the value of b, otherwise, we need to go another iteration deeper
	if (remainder == 0)
		return b;
	else
		return gcd_rec(b, remainder, spacing);

}

template<typename T>
T gcd(T a, T b) {

	//	we need to know the number of digits from both numbers for proper spacing(based on highest digit count)
	uint32_t spacing = a > b ? (uint32_t)std::floor(std::log10(a)) + 2 : (uint32_t)std::floor(std::log10(b)) + 2;

	//	output table header of data: first argument, second arguement, quotient, and remainder
	std::cout << std::setw(spacing) << 'a' << std::setw(spacing) << 'b' << std::setw(spacing) << 'q' << std::setw(spacing) << 'r' << '\n' << std::flush;

	//	call recursive function to calcuate and output GCD data
	return gcd_rec(a, b, spacing);

}

//	for maintaining x and y values outside of function
template<typename T>
T gcdExtendedRec(T a, T b, T& x, T& y, uint32_t& spacing) {

	if (a == 0) {

		x = 0;
		y = 1;
		std::cout << std::setw(spacing) << a << std::setw(spacing) << b << std::setw(spacing) << x << std::setw(spacing) << y << '\n' << std::flush;
		return b;

	}

	T x1, y1;
	T gcd = gcdExtendedRec(b % a, a, x1, y1, spacing);

	x = y1 - (b / a) * x1;
	y = x1;

	std::cout << std::setw(spacing) << a << std::setw(spacing) << b << std::setw(spacing) << x << std::setw(spacing) << y << '\n' << std::flush;
	return gcd;

}

template<typename T>
T gcdExtended(T a, T b, T& x, T& y) {

	//	since there are now 4 numbers to measure, lets just take the max possible digit size
	uint32_t spacing = (uint32_t)std::floor(std::log10(ULLONG_MAX)) + 2;

	//	output table header of data: first argument, second arguement, quotient, and remainder
	std::cout << std::setw(spacing) << 'a' << std::setw(spacing) << 'b' << std::setw(spacing) << 'x' << std::setw(spacing) << 'y' << '\n' << std::flush;

	//	call recursive function to calcuate and output GCD data
	return gcdExtendedRec(a, b, x, y, spacing);

}

template<typename T>
T chineseRemainder(T input_1, T mod_1, T input_2, T mod_2) {

	T pInv = 0;
	T qInv = 0;

	T gcd = gcdExtended(mod_1, mod_2, pInv, qInv);

	if (gcd != 1)	//	if it isn't 1, it doesn't work
		return -1;

	T p1 = inverse(pInv, mod_2);
	T q1 = inverse(qInv, mod_1);

	return ((input_1 * (mod_2 * q1)) + (input_2 * (mod_1 * p1))) % (mod_1 * mod_2);

}

template<typename T>
T exponentiation(uint64_t input, T power, T mod) {

	T res = 1;
	input = input % mod;

	//	check if x was divisible by mod, if so, return 0
	if (!input)
		return 0;

	for (; power; power >>= 1) {

		if (power & 1)	//	check if the first bit of mod is 1, if so, we recalculate
			res = (res * input) % mod;

		input = (input * input) % mod;

	}

	return res;

}

template<typename T> void rsa(T message) {

	srand(time(NULL));

	T p = 0, q = 0, n, e = 0, d;
	T x = 0, y = 0;
	while (!isPrime(p))
		p = rand();
	while (!isPrime(q))
		q = rand();

	n = p * q;

	while (!isPrime(e))
		e = rand();

	if (e > (p - 1) * (q - 1)) {

		std::cout << "eGCD(" << e << ", " << (p - 1) * (q - 1) << ")" << '\n' << std::flush;
		gcdExtended(e, (p - 1) * (q - 1), x, y);
		d = inverse(x, (p - 1) * (q - 1));

	}
	else {

		std::cout << "eGCD(" << (p - 1) * (q - 1) << ", " << e << ")" << '\n' << std::flush;
		gcdExtended((p - 1) * (q - 1), e, x, y);
		d = inverse(y, (p - 1) * (q - 1));

	}

	std::cout << "p: " << p << '\n';
	std::cout << "q: " << q << '\n';
	std::cout << "n: " << n << '\n';
	std::cout << "e: " << e << '\n';
	std::cout << "d: " << d << '\n' << std::flush;

	T cipher = exponentiation(message, e, n);
	std::cout << "Encrypted: " << cipher << '\n' << std::flush;

	std::cout << "Decrypted: " << exponentiation(cipher, d, n) << '\n' << std::flush;

}