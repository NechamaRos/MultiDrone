
#include <iostream>
#include "AES.h"
#include "API.h"

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
        throw invalid_argument("invalid AES key length");
    }
}
unsigned char* AES::EncryptCBC(const unsigned char in[], unsigned int inLen, const unsigned char key[], const unsigned char* iv) {
    CheckLength(inLen);
    unsigned char* out = new unsigned char[inLen];
    unsigned char block[blockBytesLen];
    unsigned char* roundKeys = new unsigned char[4 * NB * (NR + 1)];
    KeyExpansion(key, roundKeys);
    memcpy(block, iv, blockBytesLen);
    for (unsigned int i = 0; i < inLen; i += blockBytesLen)
    {
        XorBlocks(block, in + i, block, blockBytesLen);
        EncryptBlock(block, out + i, roundKeys);
        memcpy(block, out + i, blockBytesLen);

    }
    delete[] roundKeys;
    return out;
}

unsigned char* AES::EncryptECB(const unsigned char in[], unsigned int inLen, const unsigned char key[])
{
    CheckLength(inLen);
    unsigned char* out = new unsigned char[inLen];
    unsigned char* block=new unsigned char[blockBytesLen];
    unsigned char* roundKeys = new unsigned char[4 * NB * (NR + 1)];
    KeyExpansion(key, roundKeys);
    for (unsigned int i = 0; i < inLen; i+=blockBytesLen)
    {
        EncryptBlock(in + i, out + i, roundKeys);
    }
    delete[] roundKeys;
    return out;
}

unsigned char* AES::DecryptCBC(const unsigned char in[], unsigned int inLen, const unsigned char key[], const unsigned char* iv) {
    CheckLength(inLen);
    unsigned char* out = new unsigned char[inLen];
    // כאן תתבצע הפענוח בפועל
    std::memcpy(out, in, inLen); // לדוגמה בלבד, יש לממש פענוח בפועל
    return out;
}


void AES::CheckLength(unsigned int len)
{
    if (len % blockBytesLen != 0)
    {
        throw std::length_error("plaintest length must be divisble by " + std::to_string(blockBytesLen));
    }
}

void AES::KeyExpansion(const unsigned char key[], unsigned char w[])
{
    API::writeLog("AES::KeyExpansion");
    unsigned char temp[4];
    unsigned char rcon[4];
    unsigned int i = 0;
    while (i<4*NK)
    {
        w[i] = key[i];
        i++;
    }
    i = 4 * NK;
    while (i < 4 * NB * (NR + 1))
    {
        temp[0] = w[i - 4 + 0];
        temp[1] = w[i - 4 + 1];
        temp[2] = w[i - 4 + 2];
        temp[3] = w[i - 4 + 3];
        if (i / 4 % NK == 0)//בכל סיבוב יש 4 מילים. 11 סיבובים=44 מילים
        {
            RotWord(temp);//מזיזה את הביטים של המילה צעד אחד שמאלה מעגלי
            SubWord(temp);//מחליפה את הביטים של המילה בהתאם למטריצת בsbox
            Rcon(rcon, i / (NK * 4));//
            XorWords(temp, rcon, temp);//sbox^rcon
        }
        else if (NK > 6 && i / 4 % NK == 4)
        {
            SubWord(temp);
        }
        w[i + 0] = w[i - 4 * NK] ^ temp[0];
        w[i + 1] = w[i + 1 - 4 * NK] ^ temp[1];
        w[i + 2] = w[i + 2 - 4 * NK] ^ temp[2];
        w[i + 3] = w[i + 3 - 4 * NK] ^ temp[3];
        i += 4;
    }
    for (unsigned int j = 0; j < 4 * NB * (NR + 1); j++) {
        printf("%02x ", w[j]);
    }
    printf("\n");
}

void AES::RotWord(unsigned char* a)
{
    API::writeLog("AES::RotWord");
    unsigned char c = a[0];
    a[0] = a[1];
    a[1] = a[2];
    a[2] = a[3];
    a[3] = c;
}

void AES::SubWord(unsigned char* a)
{
    for (int i = 0; i < 4; ++i) {
        a[i] = sbox[a[i] / 16][a[i] % 16];
    }
}

void AES::XorWords(unsigned char* a, unsigned char* b, unsigned char* c)
{
    API::writeLog("AES::XorWords");
    for (int i = 0; i < 4; i++)
    {
        c[i] = a[i] ^ b[i];
    }
}
/// <summary>
/// הפונקציה מחשבת את הערך עבור a[0] על ידי כפל ב-2 n-1 פעמים) 
/// </summary>
/// <param name="a">Rconערך ה</param>
/// <param name="n">הסיבוב</param>
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
// הפונקציה מבצעת את פעולת הכפל ב-2 בתוך השדה הגלואה (GF(2^8)), על ידי שימוש בפולינום 
unsigned char AES::xtime(unsigned char b)
{
    //cout << (((b << 1)) ^ ((((b >> 7) & 1) * 0x1b)))<<endl;
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
    unsigned char state[4][NB];
    unsigned int i, j, round;
    for(i=0;i<4;i++)
        for (j = 0; j < NB; j++)
            state[i][j] = in[i + 4 * j];
    AddRoundKey(state, roundKeys);
    //בכל סיבוב חוץ מהאחרון מתבצעים השלבים הבאים:
    for (round = 1; round <= NR - 1; round++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys + round * 4 * NB);
    }
    //בסיבוב האחרון מתבצע:
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys + NR * 4 * NB);
    for (i = 0; i < 4; i++)
        for (j = 0; j < NB; j++)
            out[i + 4 * j] = state[i][j];
}
/// <summary>
/// החלפה של הביטים במטריצת המצב בהתאמה בביטים שבמטריצה הקבועה sbox
/// </summary>
/// <param name="state"></param>
void AES::SubBytes(unsigned char state[4][NB])
{
    unsigned int i, j;
    unsigned char t;
    for(i=0;i<4;i++)
        for (j = 0; j < NB; j++)
        {
            t = state[i][j];
            state[i][j] = sbox[t/16][t%16];
        }
}
void AES::FindInSBox(int r, int c)
{
    cout << sbox[r][c]<<endl;
}
/// <summary>
/// הפונקציה מקבלת את מטריצת המצב ומבצעת הזזה של השורות.שורה שניה-1, שורה שלישית-2,שורה רביעית-3
/// </summary>
/// <param name="state"></param>
void AES::ShiftRows(unsigned char state[4][NB])
{
    ShiftRow(state, 1, 1);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 3);
    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 4; j++)
    //    {
    //        cout << hex << static_cast<int>(state[i][j]) << " ";
    //    }
    //    cout << endl;
    //}
}
/// <summary>
/// המטריצה מבצעת הזזה של שורה במספר צעדים שמאלה במטריצה.
/// </summary>
/// <param name="state"></param>
/// <param name="i">השורה</param>
/// <param name="n">מספר הצעדים שצריך לזוז</param>
void AES::ShiftRow(unsigned char state[4][NB], unsigned int i, unsigned int n)
{
    unsigned char tmp[NB];
    for (unsigned int j = 0; j < NB; j++)
    {
        tmp[j] = state[i][(j + n) % NB];
    }
    memcpy(state[i], tmp, NB * sizeof(unsigned char));
}
/// <summary>
/// הפונקציה מבצעת טרנספורמציה על עמודות מטריצת המצב באמצעות פעולות חשבון של שדה גלואה
/// </summary>
/// <param name="state"></param>
void AES::MixColumns(unsigned char state[4][NB])
{
    unsigned char temp_state[4][NB];
    for(int i=0;i<4;i++)
        memset(temp_state[i], 0, 4);
    for (size_t i = 0; i < 4; ++i) {
        for (size_t k = 0; k < 4; ++k) {
            for (size_t j = 0; j < 4; ++j) {
                if (CMDS[i][k] == 1)
                    temp_state[i][j] ^= state[k][j];
                else
                    temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
            }
        }
    }
    //דוגמא:
    //static const unsigned char CMDS[4][4] = {
    //{2, 3, 1, 1}, 
    //{1, 2, 3, 1},
    //{1, 1, 2, 3},
    //{3, 1, 1, 2} };
    //unsigned char state[4][4] = {
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

    for (size_t i = 0; i < 4; ++i) {
        memcpy(state[i], temp_state[i], 4);
    }
}
/// <summary>
/// הפונקציה מקבצת XOR בין המטריצה הקבועה למפתח של הסיבוב הנוכחי
/// </summary>
/// <param name="state"></param>
/// <param name="key"></param>
void AES::AddRoundKey(unsigned char state[4][NB], unsigned char* key)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < NB; j++)
            state[i][j] = state[i][j] ^ key[i + j * 4];
}
