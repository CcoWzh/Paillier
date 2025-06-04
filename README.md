# README

本代码库是使用`C++`对Paillier的简单实现，具备加、减、乘（标量）、除（标量）、比较等功能。

> Paillier是一个支持加法同态的公钥密码系统，由Paillier在1999年的欧密会（EUROCRYPT）上首次提出。在众多PHE方案中，Paillier方案由于效率较高、安全性证明完备的特点，在各大顶会和实际应用中被广泛使用，是隐私计算场景中最常用的PHE实例化方案之一。
>
> 算法详情见`./doc` 文件夹内的：[Paillier算法原理和实现详细说明](./doc/Paillier算法原理和实现详细说明.pdf) 。

## 使用

### 前提条件

- 需要有GMP大数库，官网链接：https://gmplib.org/

在Linux下执行如下命令即可：

```
apt install libgmp-dev
```

### 运行

执行如下命令：

```
mkdir build
cd ./build
cmake ..
make
```

## 程序说明

```c++
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
    void Sub(mpz_t res, mpz_t c1, mpz_t c2);  // 减法
    int Compare(mpz_t c1, mpz_t c2); // 比较
    void Div(mpz_t res, mpz_t c, mpz_t k); //除法
};
```

调用：

```c++
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

```

测试结果为：

```
hui@hui-virtual-machine:~/Desktop/SC-test/Paillier/build$ ./paillier 
========= 开始进行测试 =========
模数 n  的大小是 : 60491
模数 n/2 的大小是 : 30246

========= 开始测试加法 =========
情况1: 数据范围均 < n/2 
m1 is 36 ; m2 is 24 ; m1 + m2 = 60
情况2: 数据范围均 > n/2 
m1 is 30246 ; m2 is 30251 ; m1 + m2 = 6
========= 结束测试加法 =========

========= 开始测试标量乘 =========
m1 is 36 ; x is 3 ; m1 * x = 108
========= 结束测试标量乘 =========

========= 开始测试减法 =========
情况1: 数据范围均 < n/2 
m1 is 36 ; m2 is 24 ; m1 - m2 = 12
m1 is 36 ; m2 is 24 ; m2 - m1 = 60479
情况2: 数据范围出现 > n/2 的情况  
m1 is 30248 ; m2 is 1 ; m1 - m2 = 30247
m1 is 1 ; m2 is 30245 ; m1 - m2 = 30247
========= 结束测试减法 =========

========= 开始测试比较 =========
情况1: 数据范围均 < n/2 
m1 is 12 ; m2 is 30240 ; m1 和 m2的比较结果是 : 
m1 < m2
情况2: 数据范围出现 > n/2 的情况  
m1 is 1 ; m2 is 30246 ; m1 和 m2的比较结果是 : 
m1 < m2
m1 is 1 ; m2 is 30247 ; m1 和 m2的比较结果是 : 
m1 > m2
========= 结束测试比较 =========

========= 开始测试除法 =========
m1 is 36 ; m3 is 4 ; m1 / m3 = 9
========= 结束测试除法 =========

释放内存并推出程序......
```

