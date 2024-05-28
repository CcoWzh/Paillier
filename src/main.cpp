#include "paillier.h"
#include <iostream>
#include <gmp.h>

using namespace std;

int main()
{
    cout << "start ......" << endl;
    Paillier paillier;
    unsigned long bitLen = 1024;
    paillier.KeyGen(bitLen);

    mpz_t m1, m2, res, c1, c2, mm, x;
    mpz_inits(m1, m2, res, c1, c2, mm, x, NULL);
    mpz_set_ui(m1, 4);
    mpz_set_ui(m2, 5);

    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);

    paillier.Add(res, c1, c2);
    paillier.Decrypt(mm, res);

    cout << "4+5 =" << mm << endl;

    mpz_set_ui(res, 0);
    mpz_set_ui(mm, 0);
    mpz_set_ui(x, 3);

    paillier.Mul(res, c1, x);
    paillier.Decrypt(mm, res);
    cout << "4*3 =" << mm << endl;

    return 0;
}
