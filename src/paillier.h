#ifndef __PAILLIER_H__ 
#define __PAILLIER_H__

#include <iostream>
#include <gmp.h>

// struct Paillier_Private_Key
// {
//     mpz_t p, q, g, n, nsquare; // nsquare = n^2
//     mpz_t lambda, lmdInv;      // lmdInv = lambda^{-1} mod n
// };

// struct Paillier_Public_Key
// {
//     mpz_t g;
//     mpz_t n;
// };

class Paillier
{
public:
    // 成员函数声明
    mpz_t p, q, g, n, nsquare; // nsquare = n^2
    mpz_t lambda, lmdInv;      // lmdInv = lambda^{-1} mod n
    Paillier(){
        mpz_inits(p, q, g, n, nsquare, lambda, lmdInv, NULL);
    }
    ~Paillier(){
        mpz_clears(p, q, g, n, nsquare, lambda, lmdInv, NULL);
        std::cout << "释放内存并推出程序......" << std::endl;
    }

    void KeyGen(unsigned long bitLen);
    void Encrypt(mpz_t c, mpz_t m); // 加密
    void Decrypt(mpz_t m, mpz_t c); // 解密
    void Add(mpz_t res, mpz_t c1, mpz_t c2);
    void Mul(mpz_t resc, mpz_t c, mpz_t e);
};


#endif