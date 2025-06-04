#include "paillier.h"

using namespace std;

extern gmp_randstate_t gmp_rand;
gmp_randstate_t gmp_rand;

void Paillier::KeyGen(unsigned long bitLen)
{
    gmp_randinit_default(gmp_rand);
    mpz_t r;
    mpz_init(r);
    mpz_rrandomb(r, gmp_rand, bitLen); // r <--- rand
    mpz_nextprime(p, r);               // p是大素数
    mpz_set(r, p);
    mpz_nextprime(q, r); // q是大素数

    mpz_mul(n, p, q);       // n = p*q
    mpz_add_ui(g, n, 1);    // g = n+1
    mpz_mul(nsquare, n, n); // nsqaure = n * n;

    mpz_sub_ui(p, p, 1); // p = p-1
    mpz_sub_ui(q, q, 1); // q = q-1
    // mpz_lcm(lambda, p, q);         // lambda = lcm(p-1, q-1)
    mpz_mul(lambda, p, q);
    mpz_invert(lmdInv, lambda, n); // lmdInv = lambda^{-1} mod n

    mpz_clear(r);
}

void Paillier::Encrypt(mpz_t c, mpz_t m)
{
    if (mpz_cmp(m, n) >= 0)
    {

        throw("m must be less than n");
        return;
    }

    mpz_t r;
    mpz_init(r);
    gmp_randinit_default(gmp_rand);
    mpz_urandomm(r, gmp_rand, n); // r <--- rand

    mpz_powm(c, g, m, nsquare); // c = g^m mod n^2
    mpz_powm(r, r, n, nsquare); // r = r^n mod n^2
    mpz_mul(c, c, r);           // c = c*r
    mpz_mod(c, c, nsquare);     // c = c mod n^2

    mpz_clear(r);
}

void Paillier::Decrypt(mpz_t m, mpz_t c)
{
    if (mpz_cmp(c, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }
    mpz_powm(m, c, lambda, nsquare); // c = c^lambda mod n^2
    // m = (c - 1) / n * lambda^(-1) mod n

    mpz_sub_ui(m, m, 1);   // c=c-1
    mpz_fdiv_q(m, m, n);   // c=(c-1)/n
    mpz_mul(m, m, lmdInv); // c=c*lambda^(-1)
    mpz_mod(m, m, n);      // m=c mod n
}

// res = c1 + c2
void Paillier::Add(mpz_t res, mpz_t c1, mpz_t c2)
{

    if (mpz_cmp(c1, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }
    if (mpz_cmp(c2, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }
    mpz_mul(res, c1, c2);
    mpz_mod(res, res, nsquare);
}

// 只能是同态标量乘
// res = c * e
void Paillier::Mul(mpz_t res, mpz_t c, mpz_t e)
{
    if (mpz_cmp(c, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }
    if (mpz_cmp(e, n) >= 0)
    {
        throw("exponent must be less than n");
    }
    mpz_powm(res, c, e, nsquare);
}

// res = c1 - c2
void Paillier::Sub(mpz_t res, mpz_t c1, mpz_t c2)
{
    if (mpz_cmp(c1, nsquare) >= 0 || mpz_cmp(c2, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }

    mpz_t c2_inv;
    mpz_init(c2_inv);

    // 计算 c2 的模逆
    if (mpz_invert(c2_inv, c2, nsquare) == 0)
    {
        throw("c2 has no modular inverse mod nsquare");
        mpz_clear(c2_inv);
        return;
    }

    // c1 * c2^{-1} mod nsquare
    mpz_mul(res, c1, c2_inv);
    mpz_mod(res, res, nsquare);

    mpz_clear(c2_inv);
}

int Paillier::Compare(mpz_t c1, mpz_t c2)
{
    mpz_t c_diff, m_diff, n_half;
    mpz_inits(c_diff, m_diff, n_half, NULL);

    // 差值密文
    Sub(c_diff, c1, c2);
    // 解密得到明文差值
    Decrypt(m_diff, c_diff);

    // 计算 n/2
    mpz_fdiv_q_ui(n_half, n, 2);

    int cmp;
    if (mpz_cmp(m_diff, n_half) > 0)
    {
        cmp = -1; // m1 - m2 < 0, 说明 m1 < m2
    }
    else if (mpz_cmp_ui(m_diff, 0) == 0)
    {
        cmp = 0; // 相等
    }
    else
    {
        cmp = 1; // m1 > m2
    }

    mpz_clears(c_diff, m_diff, n_half, NULL);
    return cmp;
}

// 标量除法（c为密文，k为明文）
// 要求：c是k的倍数. res = c / k
void Paillier::Div(mpz_t res, mpz_t c, mpz_t k)
{
    if (mpz_cmp(c, nsquare) >= 0)
    {
        throw("ciphertext must be less than n^2");
        return;
    }

    mpz_t k_inv;
    mpz_init(k_inv);

    // 计算 k 在 mod n 下的逆元
    if (mpz_invert(k_inv, k, n) == 0)
    {
        throw("k has no inverse modulo n");
        mpz_clear(k_inv);
        return;
    }

    // c^{k^{-1}} mod n^2
    mpz_powm(res, c, k_inv, nsquare);

    mpz_clear(k_inv);
}