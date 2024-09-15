#include "RSA.h"

#include <boost/lexical_cast.hpp>

#include <CL/sycl.hpp>
#include "KeyPair.h"
#include "Logger.h"
using namespace sycl;

// Standard random number generator
std::mt19937 gen(static_cast<unsigned long>(time(0)));

RSA::RSA(const KeyPair& keyPair) : keyPair1(keyPair) {
   
}
// Function to perform modular exponentiation
//this function is requierd to miller_rabin_test
cpp_int RSA::mod_exp(cpp_int base, cpp_int exp, cpp_int mod) {
    cpp_int result = 1;
    base = base % mod;

    // Create a SYCL queue to offload computations to the accelerator
    sycl::queue q;

    // Allocate space for results on the host and device
    cpp_int* device_result = malloc_device<cpp_int>(1, q);
    cpp_int* device_base = malloc_device<cpp_int>(1, q);
    cpp_int* device_exp = malloc_device<cpp_int>(1, q);
    cpp_int* device_mod = malloc_device<cpp_int>(1, q);

    q.memcpy(device_base, &base, sizeof(cpp_int)).wait();
    q.memcpy(device_exp, &exp, sizeof(cpp_int)).wait();
    q.memcpy(device_mod, &mod, sizeof(cpp_int)).wait();

    // Use nd_range for parallel processing
    sycl::range<1> global_range(1);  // Global range
    sycl::range<1> local_range(1);   // Local range

    // Parallel modular exponentiation
    q.submit([&](sycl::handler& h) {
        h.parallel_for(sycl::nd_range<1>(global_range, local_range), [=](sycl::nd_item<1> item) {
            //h.single_task([=]() {
            cpp_int local_result = 1;
            cpp_int local_base = *device_base;
            cpp_int local_exp = *device_exp;
            cpp_int local_mod = *device_mod;

            while (local_exp > 0) {
                if (local_exp % 2 == 1) {
                    local_result = (local_result * local_base) % local_mod;
                }
                local_exp = local_exp >> 1;
                local_base = (local_base * local_base) % local_mod;
            }

            *device_result = local_result;
            });
        }).wait();

        // Copy the result back to host
        q.memcpy(&result, device_result, sizeof(cpp_int)).wait();

        // Free memory on the device
        sycl::free(device_result, q);
        sycl::free(device_base, q);
        sycl::free(device_exp, q);
        sycl::free(device_mod, q);

    return result;
}

// Function to perform the Miller-Rabin primality test
bool RSA::miller_rabin_test(const cpp_int& number_to_test, int num_tests) {
    if (number_to_test <= 1 || number_to_test == 4) return false;
    if (number_to_test <= 3) return true;

    cpp_int d_value = number_to_test - 1;
    int two_power_count = 0;
    while (d_value % 2 == 0) {
        d_value /= 2;
        two_power_count++;
    }
   
    // SYCL setup
    queue q;
    bool is_prime = true;

    //// Memory allocation
    // Allocate memory on device
    cpp_int* device_number = malloc_device<cpp_int>(1, q);
    cpp_int* device_d_value = malloc_device<cpp_int>(1, q);
    bool* device_is_prime = malloc_device<bool>(1, q);
    q.memcpy(device_number, &number_to_test, sizeof(cpp_int)).wait();
    q.memcpy(device_d_value, &d_value, sizeof(cpp_int)).wait();

    q.submit([&](handler& h) {
        h.single_task([=]() {
          /*  int index = item.get_global_id(0);*/
            cpp_int local_number = *device_number;
            cpp_int local_d_value = *device_d_value;

            cpp_int random_base = 2 + rand() % (local_number - 4);
            cpp_int power_mod_result = mod_exp(random_base, local_d_value, local_number);
            if (power_mod_result == 1 || power_mod_result == local_number - 1) return;

            bool continue_outer_loop = false;

  
        bool continue_outer_loop = false;
        for (int j = 0; j < two_power_count - 1; j++) {
            power_mod_result = (power_mod_result * power_mod_result) % local_number;
            if (power_mod_result == 1){
                *device_is_prime = false;
                return;
            }
        
            if (power_mod_result == local_number - 1) {
                continue_outer_loop = true;
                break;
            }
        }

        if (!continue_outer_loop) {
            *device_is_prime = false;
        }
            });
        }).wait();
        q.memcpy(&is_prime, device_is_prime, sizeof(bool)).wait();

        free(device_number, q);
        free(device_d_value, q);
        free(device_is_prime, q);


        return is_prime;
}

// Function to generate a random cpp_int of a given bit length
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

// Function to generate a prime number with a specified bit length
cpp_int RSA::generate_prime(int bit_length) {
    cpp_int prime;
    
    bool is_prime = false;

    // SYCL setup
    queue q;

    while (!is_prime) {
        cpp_int* device_prime = malloc_device<cpp_int>(1, q);
        bool* device_is_prime = malloc_device<bool>(1, q);

        // Generate a random number and copy to device
        cpp_int random_prime = generate_random_cpp_int(bit_length);
        q.memcpy(device_prime, &random_prime, sizeof(cpp_int)).wait();

        // Check primality on the device
        q.submit([&](handler& h) {
            h.single_task([=]() {
                cpp_int local_prime = *device_prime;
                bool local_is_prime = miller_rabin_test(local_prime, num_tests);
                *device_is_prime = local_is_prime;
                });
            }).wait();

            q.memcpy(&is_prime, device_is_prime, sizeof(bool)).wait();

            free(device_prime, q);
            free(device_is_prime, q);

            if (is_prime) {
                prime = random_prime;
            }
    }

    return prime;
}

// Function to set the private and public keys for RSA
void RSA::set_private_and_public_keys() {
    cpp_int prime1 = generate_prime(1024);
    cpp_int prime2 = generate_prime(1024);
    n = prime1 * prime2;
    cpp_int t = (prime1 - 1) * (prime2 - 1); // Definition of t
    cpp_int e = 65537; // Common choice for e

    if (gcd(e, t) != 1) {
        throw std::runtime_error("gcd(e, t) != 1, choose another prime pair");
    }

    keyPair1.setPublicKey_RSA(e);

    cpp_int d = 0, k = 1;
    while ((1 + k * t) % e != 0) {
        k++;
    }

    keyPair1.setPrivateKey((1 + k * t) / e);

    Logger::getInstance().log("Public and Private keys set.\n");

    // Log the keys
    std::string publicKeyMessage = "Public Key: " + boost::lexical_cast<std::string>(keyPair1.getPublicKey_RSA()) + "\n";
    Logger::getInstance().log(publicKeyMessage);

    std::string privateKeyMessage = "Private Key: " + boost::lexical_cast<std::string>(keyPair1.getPrivateKey()) + "\n";
    Logger::getInstance().log(privateKeyMessage);
}
// Function to encrypt the given number
cpp_int RSA::encrypt(cpp_int  message) {

    cpp_int encrypted_message = mod_exp(message,keyPair1.getPublicKey_RSA(), n);

    // Adding message to the log

    Logger::getInstance().log("Encrypted message: " + boost::lexical_cast<std::string>(message) + " to " + boost::lexical_cast<std::string>(encrypted_message) + "\n");



    return encrypted_message;
}

// Function to decrypt the given number
cpp_int RSA::decrypt(cpp_int  encrpyted_text) {

    cpp_int decrypted_message = mod_exp(encrpyted_text,keyPair1.getPrivateKey(), n);

    // Adding message to the log
    Logger::getInstance().log("Decrypted message: " + boost::lexical_cast<std::string>(encrpyted_text) + " to " + boost::lexical_cast<std::string>(decrypted_message) + "\n");

    return decrypted_message;
}

//AES key encryption and decryption:

//A function that prepares the AES key for encryption.
cpp_int RSA::convert_aes_key_to_cpp_int(const unsigned char* aes_key, size_t key_length) {
    cpp_int aes_key_as_int = 0;
    for (size_t i = 0; i < key_length; i++) {
        aes_key_as_int <<= 8;
        aes_key_as_int |= aes_key[i];
    }
    return aes_key_as_int;
}
//A function that encrypts an AES key with RSA encryption.
cpp_int RSA::encrypt_aes_key(const unsigned char* aes_key, size_t key_length) {
    cpp_int aes_key_as_int = convert_aes_key_to_cpp_int(aes_key, key_length);
    return encrypt(aes_key_as_int); //Using the existing function of encryption
}
// A function that decrypts the AES key.
std::vector<unsigned char> RSA::decrypt_aes_key(cpp_int encrypted_aes_key) {
    cpp_int decrypted_aes_key_as_int = decrypt(encrypted_aes_key);
    std::vector<unsigned char> decrypted_key_bytes;
    while (decrypted_aes_key_as_int != 0) {
        decrypted_key_bytes.push_back(static_cast<unsigned char>(decrypted_aes_key_as_int & 0xFF));
        decrypted_aes_key_as_int >>= 8;
    }

    //Reverse the order to get the original key.
    std::reverse(decrypted_key_bytes.begin(), decrypted_key_bytes.end());
    return decrypted_key_bytes;
}




//String encryption functions not suitable for AES
//we don't use in encoder and decoder function in our code
vector<cpp_int> RSA::encoder(string message) {
    vector<cpp_int > encrpyted_text;

    // Each individual letter is converted to a number and then encrypted.
    for (auto& letter : message)
        encrpyted_text.push_back(encrypt(static_cast<int>(letter)));
    // Adding message to the log
    Logger::getInstance().log("Encoded message: " + message + "\n");

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
    Logger::getInstance().log("Decoded message.\n");

    return s;
}
