
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include "AES.h"
#include "API.h"
namespace MyAES {

    AES::AES(const AESKeyLength keyLength)
    {
        switch (keyLength)
        {
        case AESKeyLength::AES_128:
            this->NK = 4;
            this->NR = 10;
            break;
        case AESKeyLength::AES_192:
            this->NK = 6;
            this->NR = 12;
            break;
        case AESKeyLength::AES_256:
            this->NK = 8;
            this->NR = 14;
            break;
        default:
            this->NK = 8;
            this->NR = 14;
        }
    }
    unsigned char* AES::EncryptCBC(const unsigned char in[], unsigned int& inLen, const unsigned char key[], const unsigned char* iv) {
        API::writeLog("AES::EncryptCBC");
        // Adjust the input length and get the padded input
        unsigned char* paddedIn = CheckLength(in, inLen);
        unsigned char* out = new unsigned char[inLen];
        unsigned char block[blockBytesLen];
        unsigned char* roundKeys = new unsigned char[NUM_WORDS * WORD * (NR + 1)];
        KeyExpansion(key, roundKeys);
        // Initialize block with IV
        memcpy(block, iv, blockBytesLen);
        // Encrypt each block
        for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
            XorBlocks(block, paddedIn + i, block, blockBytesLen);
            EncryptBlock(block, out + i, roundKeys);
            memcpy(block, out + i, blockBytesLen);
        }
        delete[] roundKeys;
        delete[] paddedIn;
        return out;
    }

    unsigned char* AES::DecryptCBC(const unsigned char in[], unsigned int inLen, const unsigned char key[], const unsigned char* iv) {
        API::writeLog("AES::DecryptCBC");
        unsigned char* out = new unsigned char[inLen];
        unsigned char block[blockBytesLen];
        unsigned char* roundKeys = new unsigned char[NUM_WORDS * WORD * (NR + 1)];
        KeyExpansion(key, roundKeys);
        memcpy(block, iv, blockBytesLen);

        for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
            DecryptBlock(in + i, out + i, roundKeys);
            XorBlocks(block, out + i, out + i, blockBytesLen);
            memcpy(block, in + i, blockBytesLen);
        }
        delete[] roundKeys;
        // Remove padding from the decrypted output
        unsigned char* unpaddedOut = RemovePadding(out, inLen);
        delete[] out;

        return unpaddedOut;
    }
    unsigned char* AES::EncryptECB(const unsigned char in[], unsigned int& inLen, const unsigned char key[])
    {
        API::writeLog("AES::EncryptECB");
        // Check and pad the input length
        unsigned int originalLen = inLen;
        unsigned char* paddedIn = CheckLength(in, inLen);
        unsigned char* out = new unsigned char[inLen];
        unsigned char* roundKeys = new unsigned char[NUM_WORDS * WORD * (NR + 1)];
        KeyExpansion(key, roundKeys);
        // Encrypt each block
        for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
            EncryptBlock(paddedIn + i, out + i, roundKeys);
        }
        delete[] roundKeys;
        delete[] paddedIn; // Clean up padded input
        return out;
    }
    unsigned char* AES::DecryptECB(const unsigned char in[], unsigned int inLen, const unsigned char key[]) {
        API::writeLog("AES::DecryptECB");
        unsigned char* out = new unsigned char[inLen];
        unsigned char* roundKeys = new unsigned char[NUM_WORDS * WORD * (NR + 1)];
        KeyExpansion(key, roundKeys);
        // Decrypt each block
        for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
            DecryptBlock(in + i, out + i, roundKeys);
        }
        delete[] roundKeys;
        // Remove padding from the decrypted output
        unsigned char* unpaddedOut = RemovePadding(out, inLen);
        delete[] out;
        return unpaddedOut;
    }
    unsigned char* AES::CheckLength(const unsigned char in[], unsigned int& inLen)
    {
        if (inLen % blockBytesLen == 0)
        {
            // No padding needed, create a copy to return
            unsigned char* paddedIn = new unsigned char[inLen];
            memcpy(paddedIn, in, inLen);
            return paddedIn;
        }
        // Calculate padding length
        unsigned int paddingLen = blockBytesLen - (inLen % blockBytesLen);
        unsigned int paddedLen = inLen + paddingLen;
        // Allocate new input with padding
        unsigned char* paddedIn = new unsigned char[paddedLen];
        for (unsigned int i = 0; i < inLen; ++i) {
            paddedIn[i] = in[i];
        }
        // Add PKCS#7 padding
        for (unsigned int i = inLen; i < paddedLen; ++i) {
            paddedIn[i] = static_cast<unsigned char>(paddingLen);
        }
        // Update the length of the input
        inLen = paddedLen;
        API::writeLog("new input length allocated : "+to_string(paddedLen) +" origin length: "+to_string(inLen));
        return paddedIn;
    }

    void AES::KeyExpansion(const unsigned char key[], unsigned char w[])
    {
        unsigned char temp[WORD];
        unsigned char rcon[WORD];
        unsigned int i = 0;
        while (i < WORD * NK)
        {
            w[i] = key[i];
            i++;
        }
        i = WORD * NK;
        while (i < NUM_WORDS * WORD * (NR + 1))
        {
            temp[0] = w[i - WORD + 0];
            temp[1] = w[i - WORD + 1];
            temp[2] = w[i - WORD + 2];
            temp[3] = w[i - WORD + 3];
            //each roundKey has 4 words. word=4 bytes. 11 rounds=44 words
            if (i / WORD % NK == 0)
            {
                RotWord(temp);//Moves the bytes of the word one circular step to the left.
                SubWord(temp);//Replaces the bytes of the word according to the sbox matrix.
                Rcon(rcon, i / (NK * NUM_WORDS));
                XorWords(temp, rcon, temp);//sbox^rcon
            }
            else if (NK > 6 && i / NUM_WORDS % NK == WORD)
            {
                SubWord(temp);
            }
            w[i + 0] = w[i - NUM_WORDS * NK] ^ temp[0];
            w[i + 1] = w[i + 1 - NUM_WORDS * NK] ^ temp[1];
            w[i + 2] = w[i + 2 - NUM_WORDS * NK] ^ temp[2];
            w[i + 3] = w[i + 3 - NUM_WORDS * NK] ^ temp[3];
            i += NUM_WORDS;
        }
        string s = "";
        for (unsigned int j = 0;NR + 1; j++) 
        {
            for (unsigned int i = 0; i < WORD*NUM_WORDS; i++)
            {
            printf("%02x ", w[j]);
            s += w[j];
            }
            API::writeLog(s);
            s = "";
            printf("\n");
        }
        printf("\n");
    }

    void AES::RotWord(unsigned char* a)
    {
        unsigned char c = a[0];
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = c;
    }

    void AES::SubWord(unsigned char* a)
    {
        for (int i = 0; i < NUM_WORDS; ++i) {
            a[i] = sbox[a[i] / 16][a[i] % 16];
        }
    }

    void AES::XorWords(unsigned char* a, unsigned char* b, unsigned char* c)
    {
        API::writeLog("AES::XorWords");
        for (int i = 0; i < NUM_WORDS; i++)
        {
            c[i] = a[i] ^ b[i];
        }
    }
    /// <summary>
    ///The function calculates the value for a[0] by multiplying by 2 n-1 times 
    /// </summary>
    /// <param name="a">Rcon value</param>
    /// <param name="n">num round</param>
    void AES::Rcon(unsigned char* a, unsigned int n)
    {
        unsigned char c = 1;
        for (unsigned int i = 0; i < n - 1; i++)
        {
            c = xtime(c);
        }
        a[0] = c;
        a[1] = a[2] = a[3] = 0;
    }
    // The function performs the operation of multiplying by 2 in the Galois field (GF(2^8)), by using a polynomial
    unsigned char AES::xtime(unsigned char b)
    {
        return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
    }
    void AES::XorBlocks(const unsigned char* a, const unsigned char* b, unsigned char* c, unsigned int len)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            c[i] = a[i] ^ b[i];
        }
    }
    void AES::EncryptBlock(const unsigned char in[], unsigned char out[], unsigned char* roundKeys)
    {
        unsigned char state[NUM_WORDS][WORD];
        unsigned int i, j, round;
        for (i = 0; i < NUM_WORDS; i++)
            for (j = 0; j < WORD; j++)
                state[i][j] = in[i + WORD * j];
        AddRoundKey(state, roundKeys);
        //In every round except the last, the following steps are performed:
        for (round = 1; round <= NR - 1; round++)
        {
            SubBytes(state);
            ShiftRows(state);
            MixColumns(state);
            AddRoundKey(state, roundKeys + round * NUM_WORDS * WORD);
        }
        //בסיבוב האחרון מתבצע:
        SubBytes(state);
        ShiftRows(state);
        AddRoundKey(state, roundKeys + NR * NUM_WORDS * WORD);
        for (i = 0; i < NUM_WORDS; i++)
            for (j = 0; j < WORD; j++)
                out[i + NUM_WORDS * j] = state[i][j];
    }
    void AES::DecryptBlock(const unsigned char in[], unsigned char out[], unsigned char* roundKeys) {
        unsigned char state[NUM_WORDS][WORD];
        unsigned int i, j, round;

        for (i = 0; i < NUM_WORDS; i++) {
            for (j = 0; j < WORD; j++) {
                state[i][j] = in[i + NUM_WORDS * j];
            }
        }

        AddRoundKey(state, roundKeys + NR * NUM_WORDS * WORD);

        for (round = NR - 1; round >= 1; round--) {
            InvSubBytes(state);
            InvShiftRows(state);
            AddRoundKey(state, roundKeys + round * NUM_WORDS * WORD);
            InvMixColumns(state);
        }

        InvSubBytes(state);
        InvShiftRows(state);
        AddRoundKey(state, roundKeys);

        for (i = 0; i < NUM_WORDS; i++) {
            for (j = 0; j < WORD; j++) {
                out[i + NUM_WORDS * j] = state[i][j];
            }
        }
    }

    unsigned char* AES::RemovePadding(const unsigned char* in, unsigned int& outLen)
    {
        unsigned char* out = new unsigned char[outLen];
        memcpy(out, in, outLen);
        // Get the padding length from the last byte of the decrypted text
        unsigned int paddingLen = in[outLen - 1];
        // the last byte is no padding byte
        if (paddingLen > blockBytesLen || paddingLen == 0) {
            return out;
        }
        // Validate that the padding bytes are correct
        for (unsigned int i = 0; i < paddingLen; ++i) {
            if (in[outLen - 1 - i] != paddingLen) {
                return out;
            }
        }
        delete[] out;
        outLen -= paddingLen;
        unsigned char* out = new unsigned char[outLen];
        memcpy(out, in, outLen);
        return out;
    }

    /// <summary>
    /// Replacement of the bits in the respective state matrix with the bits in the fixed matrix sbox.
    /// </summary>
    /// <param name="state">the state matrix</param>
    void AES::SubBytes(unsigned char state[NUM_WORDS][WORD])
    {
        unsigned int i, j;
        unsigned char t;
        for (i = 0; i < NUM_WORDS; i++)
            for (j = 0; j < WORD; j++)
            {
                t = state[i][j];
                state[i][j] = sbox[t / 16][t % 16];
            }
    }
    //The function receives the state matrix and moves the rows. Second row-1, third row-2, fourth row-3
    void AES::ShiftRows(unsigned char state[NUM_WORDS][WORD])
    {
        ShiftRow(state, 1, 1);
        ShiftRow(state, 2, 2);
        ShiftRow(state, 3, 3);
        //for (int i = 0; i < NUM_WORDS; i++)
        //{
        //    for (int j = 0; j < NUM_WORDS; j++)
        //    {
        //        cout << hex << static_cast<int>(state[i][j]) << " ";
        //    }
        //    cout << endl;
        //}
    }
    /// <summary>
    /// The matrix moves a row a number of steps to the left in the matrix.
    /// </summary>
    /// <param name="state">the state matrix</param>
    /// <param name="i">line</param>
    /// <param name="n">num steps</param>
    void AES::ShiftRow(unsigned char state[NUM_WORDS][WORD], unsigned int i, unsigned int n)
    {
        unsigned char tmp[WORD];
        for (unsigned int j = 0; j < WORD; j++)
        {
            tmp[j] = state[i][(j + n) % WORD];
        }
        memcpy(state[i], tmp, WORD * sizeof(unsigned char));
    }
    /// <summary>
    /// The function performs a transformation on the columns of the state matrix by Galois field arithmetic operations.
    /// </summary>
    /// <param name="state">the state matrix</param>
    void AES::MixColumns(unsigned char state[NUM_WORDS][WORD])
    {
        unsigned char temp_state[NUM_WORDS][WORD];
        for (int i = 0; i < NUM_WORDS; i++)
            memset(temp_state[i], 0, NUM_WORDS);
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            for (size_t k = 0; k < NUM_WORDS; ++k) {
                for (size_t j = 0; j < NUM_WORDS; ++j) {
                    if (CMDS[i][k] == 1)
                        temp_state[i][j] ^= state[k][j];
                    else
                        temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
                }
            }
        }
        //דוגמא:
        //static const unsigned char CMDS[NUM_WORDS][NUM_WORDS] = {
        //{2, 3, 1, 1}, 
        //{1, 2, 3, 1},
        //{1, 1, 2, 3},
        //{3, 1, 1, 2} };
        //unsigned char state[NUM_WORDS][NUM_WORDS] = {
        //{0x87, 0xf2, 0x4d, 0x97},
        //{0x6e, 0x4c, 0x90, 0xec},
        //{0x46, 0xe7, 0x4a, 0xc3},
        //{0xa6, 0x8c, 0xd8, 0x95}
        //};
        //{
        //    0x87^2 ^3 ^1 ^1 ^CMD[1] ^CMD[2] ^CMD[3], 0xf2 ^ 2 ^ 3 ^ 1 ^ 1 ^ CMD[1] ^ CMD[2] ^ CMD[3], 0x4d ^ 2 ^ 3 ^ 1 ^ 1 ^ CMD[1] ^ CMD[2] ^ CMD[3], 0x97 ^ 2 ^ 3 ^ 1 ^ 1 ^ CMD[1] ^ CMD[2] ^ CMD[3],
        //    0x6e^CMD[0] ^CMD[1] ^ CMD[2] ^ CMD[3], 0x4c ^ CMD[0] ^ CMD[1] ^ CMD[2] ^ CMD[3], 0x90^CMD[0] ^ CMD[1] ^ CMD[2] ^ CMD[3],... ,...
        //    0x46,...,...,...
        //    0xa6,...,...,...
        //}

        for (size_t i = 0; i < NUM_WORDS; ++i) {
            memcpy(state[i], temp_state[i], NUM_WORDS);
        }
    }
    // The function groups XOR between the constant matrix and the key of the current rotation.
    void AES::AddRoundKey(unsigned char state[NUM_WORDS][WORD], unsigned char* key)
    {
        for (int i = 0; i < NUM_WORDS; i++)
            for (int j = 0; j < WORD; j++)
                state[i][j] = state[i][j] ^ key[i + j * NUM_WORDS];
    }
    void AES::InvSubBytes(unsigned char state[NUM_WORDS][WORD]) {
        unsigned int i, j;
        unsigned char t;
        for (i = 0; i < NUM_WORDS; i++) {
            for (j = 0; j < WORD; j++) {
                t = state[i][j];
                state[i][j] = inv_sbox[t / 16][t % 16];
            }
        }
    }
    void AES::InvShiftRows(unsigned char state[NUM_WORDS][WORD]) {
        ShiftRow(state, 1, WORD - 1);
        ShiftRow(state, 2, WORD - 2);
        ShiftRow(state, 3, WORD - 3);
    }
    void AES::InvMixColumns(unsigned char state[NUM_WORDS][WORD]) {
        unsigned char temp_state[NUM_WORDS][WORD];
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            memset(temp_state[i], 0, NUM_WORDS);
        }
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            for (size_t k = 0; k < NUM_WORDS; ++k) {
                for (size_t j = 0; j < NUM_WORDS; ++j) {
                    temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
                }
            }
        }
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            memcpy(state[i], temp_state[i], NUM_WORDS);
        }
    }

}