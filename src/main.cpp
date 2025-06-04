#include "paillier.h"
#include <iostream>
#include <gmp.h>

using namespace std;

int main()
{
    cout << "========= 开始进行测试 =========" << endl;
    Paillier paillier;
    unsigned long bitLen = 8;
    mpz_t m1, m2, res, c1, c2, mm, x;
    mpz_inits(m1, m2, res, c1, c2, mm, x, NULL);
    // 产生公私钥
    paillier.KeyGen(bitLen);
    cout << "模数 n  的大小是 : " << paillier.n << endl;
    mpz_t n_div2;
    mpz_init(n_div2);
    mpz_cdiv_q_ui(n_div2, paillier.n, 2);
    cout << "模数 n/2 的大小是 : " << n_div2 << endl;
    // 所有的数据均需要 < n/2
    printf("\n");

    cout << "========= 开始测试加法 =========" << endl;
    mpz_set_ui(m1, 36);
    mpz_set_ui(m2, 24);
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);

    cout << "情况1: 数据范围均 < n/2 " << endl;
    paillier.Add(res, c1, c2);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 + m2 = " << mm << endl;

    mpz_set_ui(m1, 30246);
    mpz_set_ui(m2, 30251);
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);

    cout << "情况2: 数据范围均 > n/2 " << endl;
    paillier.Add(res, c1, c2);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 + m2 = " << mm << endl;
    cout << "========= 结束测试加法 =========" << endl;
    printf("\n");

    cout << "========= 开始测试标量乘 =========" << endl;
    // 同态标量乘:"
    mpz_set_ui(m1, 36);
    paillier.Encrypt(c1, m1);
    mpz_set_ui(x, 3);
    
    paillier.Mul(res, c1, x);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; x is " << x << " ; m1 * x = " << mm << endl;
    cout << "========= 结束测试标量乘 =========" << endl;
    printf("\n");
    
    cout << "========= 开始测试减法 =========" << endl;
    cout << "情况1: 数据范围均 < n/2 " << endl;
    mpz_set_ui(m1, 36);
    mpz_set_ui(m2, 24);
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);
    paillier.Sub(res,c1,c2);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 - m2 = " << mm << endl;
    paillier.Sub(res,c2,c1);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m2 - m1 = " << mm << endl;

    cout << "情况2: 数据范围出现 > n/2 的情况  " << endl;
    mpz_set_ui(m1, 30248);
    mpz_set_ui(m2, 1); // >n/2
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);
    paillier.Sub(res,c1,c2);
    paillier.Decrypt(mm, res);
    // 无法通过 >n/2 来判断数据的正负
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 - m2 = " << mm << endl;

    mpz_set_ui(m1, 1);
    mpz_set_ui(m2, 30245); // >n/2
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);
    paillier.Sub(res,c1,c2);
    paillier.Decrypt(mm, res);
    // 无法通过 >n/2 来判断数据的正负
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 - m2 = " << mm << endl;
    cout << "========= 结束测试减法 =========" << endl;
    printf("\n");

    cout << "========= 开始测试比较 =========" << endl;
    cout << "情况1: 数据范围均 < n/2 " << endl;
    int result;
    mpz_set_ui(m1, 12);
    mpz_set_ui(m2, 30240);
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);

    result = paillier.Compare(c1,c2);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 和 m2的比较结果是 : " << endl;
    if (result == 1){ 
        cout << "m1 > m2" << endl;
    }else if (result ==0)
    {
        cout << "m1 = m2" << endl;
    }else
    {
        cout << "m1 < m2" << endl;
    }

    cout << "情况2: 数据范围出现 > n/2 的情况  " << endl;
    mpz_set_ui(m1, 1);
    mpz_set_ui(m2, 30246); // >n/2
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);

    result = paillier.Compare(c1,c2);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 和 m2的比较结果是 : " << endl;
    if (result == 1){ 
        cout << "m1 > m2" << endl;
    }else if (result ==0)
    {
        cout << "m1 = m2" << endl;
    }else
    {
        cout << "m1 < m2" << endl;
    }

    mpz_set_ui(m2, 30247); // >n/2
    paillier.Encrypt(c2, m2);

    result = paillier.Compare(c1,c2);
    cout << "m1 is " << m1 << " ; m2 is " << m2 << " ; m1 和 m2的比较结果是 : " << endl;
    if (result == 1){ 
        cout << "m1 > m2" << endl;
    }else if (result ==0)
    {
        cout << "m1 = m2" << endl;
    }else
    {
        cout << "m1 < m2" << endl;
    }
    cout << "========= 结束测试比较 =========" << endl;
    printf("\n");

    cout << "========= 开始测试除法 =========" << endl;
    mpz_t m3,c3;
    mpz_set_ui(m1, 36);
    paillier.Encrypt(c1, m1);
    mpz_inits(m3, c3, NULL);
    mpz_set_ui(m3, 4);

    paillier.Div(res,c1,m3);
    paillier.Decrypt(mm, res);
    cout << "m1 is " << m1 << " ; m3 is " << m3 << " ; m1 / m3 = " << mm << endl;
    cout << "========= 结束测试除法 =========" << endl;
    printf("\n");

    mpz_clears(m1, m2, m3,res, c1, c2, mm, x, NULL);

    return 0;
}
