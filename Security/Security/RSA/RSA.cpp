#include "RSA.h"


cpp_int publicKey;
cpp_int privateKey;
cpp_int n;

// Adding a global variable to the log file
ofstream logfile("logfile.txt");

void RSA::closeLogFile() {
	if (logfile.is_open()) {
		logfile.close();
	}
}

// Standard random number generator
std::mt19937 gen(static_cast<unsigned long>(time(0)));

// Function to calculate the bit length of a cpp_int number
int  RSA::bit_length(const cpp_int& n) {
	int bits = 0;
	cpp_int temp = n;
	while (temp != 0) {
		bits++;
		temp >>= 1;
	}
	return bits;
}

// Function to check if n is at least 2048 bits
bool RSA::is_key_at_least_2048_bits(cpp_int n) {
	return bit_length(n) >= 2048;
}

// Function to perform modular exponentiation
//this function is requierd to miller_rabin_test
cpp_int RSA::mod_exp(cpp_int base, cpp_int exp, cpp_int mod) {
	cpp_int result = 1;
	base = base % mod;
	while (exp > 0) {
		if (exp % 2 == 1) {
			result = (result * base) % mod;
		}
		exp = exp >> 1;
		base = (base * base) % mod;
	}
	return result;
}

// Function to perform the Miller-Rabin primality test
//this test cheack if large number is prime.
bool RSA::miller_rabin_test(const cpp_int& n, int k) {
	if (n <= 1 || n == 4) return false;
	if (n <= 3) return true;

	cpp_int d = n - 1;
	int r = 0;
	while (d % 2 == 0) {
		d /= 2;
		r++;
	}

	for (int i = 0; i < k; i++) {
		cpp_int a = 2 + rand() % (n - 4); // Random integer in [2, n-2]
		cpp_int x = mod_exp(a, d, n);
		if (x == 1 || x == n - 1) continue;

		bool continue_outer_loop = false;
		for (int j = 0; j < r - 1; j++) {
			x = (x * x) % n;
			if (x == 1) return false;
			if (x == n - 1) {
				continue_outer_loop = true;
				break;
			}
		}

		if (!continue_outer_loop) return false;
	}

	return true;
}
//Custom function to generate a random cpp_int of a given bit length
cpp_int RSA::generate_random_cpp_int(int bit_length) {
	std::uniform_int_distribution<unsigned int> dist(0, std::numeric_limits<unsigned int>::max());
	cpp_int result = 0;

	for (int i = 0; i < bit_length; i += 32) {
		result <<= 32;
		result |= dist(gen);
	}

	result |= (cpp_int(1) << (bit_length - 1)); // Ensure the number has the correct bit length
	result |= 1; // Ensure the number is odd
	return result;
}
//Function to generate a prime number with a specified bit length
cpp_int RSA::generate_prime(int bit_length) {
	cpp_int prime;
	do {
		prime = generate_random_cpp_int(bit_length);
	} while (!miller_rabin_test(prime, 25)); // Using Miller-Rabin primality test
	return prime;
}
// Function to find the greatest common divisor of two numbers
cpp_int RSA::gcd(cpp_int a, cpp_int b) {
	while (b != 0)
	{
		cpp_int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

void RSA::set_private_and_public_keys() {
	cpp_int  prime1 = generate_prime(1024);
	cpp_int  prime2 = generate_prime(1024);
	n = prime1 * prime2;
	cpp_int  t = (prime1 - 1) * (prime2 - 1);//הגדרת t
	cpp_int  e = 65537;//Common choice for e

	if (gcd(e, t) != 1) {
		throw std::runtime_error("gcd(e, t) != 1, choose another prime pair");
	}

	publicKey = e;

	cpp_int  d = 0, k = 1;
	while ((1 + k * t) % e != 0)//הגדרת d
	{
		k++;
	}
	privateKey = (1 + k * t) / e;


	// Print bit lengths for verification
	cout << "Bit length of prime1: " << bit_length(prime1) << endl;
	cout << "Bit length of prime2: " << bit_length(prime2) << endl;
	cout << "Bit length of n: " << bit_length(n) << endl;

	// Adding messages to the log
	if (logfile.is_open()) {
		logfile << "Public and Private keys set.\n";
		logfile << "Public Key: " << publicKey << "\n";
		logfile << "Private Key: " << privateKey << "\n";
	}
}

// Function to encrypt the given number
cpp_int RSA::encrypt(cpp_int  message) {

	cpp_int encrypted_message = mod_exp(message, publicKey, n);

	// Adding message to the log
	if (logfile.is_open()) {
		logfile << "Encrypted message: " << message << " to " << encrypted_message << "\n";
	}

	return encrypted_message;
}

// Function to decrypt the given number
cpp_int RSA::decrypt(cpp_int  encrpyted_text) {

	cpp_int decrypted_message = mod_exp(encrpyted_text, privateKey, n);

	// Adding message to the log
	if (logfile.is_open()) {
		logfile << "Decrypted message: " << encrpyted_text << " to " << decrypted_message << "\n";
	}

	return decrypted_message;
}

vector<cpp_int> RSA::encoder(string message) {
	vector<cpp_int > encrpyted_text;

// Each individual letter is converted to a number and then encrypted.
	for (auto& letter : message)
		encrpyted_text.push_back(encrypt(static_cast<int>(letter)));
	// Adding message to the log
	if (logfile.is_open()) {
		logfile << "Encoded message: " << message << "\n";
	}
	return encrpyted_text;
}
string  RSA::decoder(vector<cpp_int> encoded)
{
	string s;
	// Calling the decrypting function decoding function
	for (auto& num : encoded)
		// Each number in the ciphertext is decoded, then concatenated into a string
		s += static_cast<char>(decrypt(num).convert_to<int>());
	// Adding message to the log
	if (logfile.is_open()) {
		logfile << "Decoded message.\n";
	}
	return s;
}
// Function to check if a number is prime using Miller-Rabin test
bool  RSA::isPrime(const cpp_int& n, int k = 25) {
	return miller_rabin_test(n, k);
}
// A function that finds a factor of any number
cpp_int RSA::pollards_rho(const cpp_int& n) {
	if (n % 2 == 0) return 2;
	cpp_int x = 2;
	cpp_int y = 2;
	cpp_int d = 1;
	cpp_int one = 1;
	// Boost random number generation
	boost::random::mt19937 gen(static_cast<unsigned long>(time(0)));
	boost::random::uniform_int_distribution<cpp_int> dist(1, n - 1);
	cpp_int c = dist(gen);
	auto f = [&](cpp_int x) { return (x * x + c) % n; };
	while (d == 1) {
		x = f(x);
		y = f(f(y));
		d = gcd((x > y) ? (x - y) : (y - x), n);
	}
	return d;
}
//A function that returns for any number, a pair of its prime factors, and an empty pair if none exist.
std::pair<cpp_int, cpp_int> RSA::factorize(const cpp_int& n) {
	if (n <= 1) return { 0, 0 };
	if (isPrime(n)) return { n, 1 };
	cpp_int factor = pollards_rho(n);
	cpp_int other_factor = n / factor;
	if (isPrime(factor) && isPrime(other_factor)) {
		return { factor, other_factor };
	}
	return { 0, 0 };
}