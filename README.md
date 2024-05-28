# Paillier原语库

本代码库是使用`C++`对Paillier的简单实现。

> Paillier是一个支持加法同态的公钥密码系统，由Paillier在1999年的欧密会（EUROCRYPT）上首次提出。在众多PHE方案中，Paillier方案由于效率较高、安全性证明完备的特点，在各大顶会和实际应用中被广泛使用，是隐私计算场景中最常用的PHE实例化方案之一。
>
> 算法详情见`./doc` 文件夹内的：[Paillier算法实现原理](./doc/Paillier算法实现原理.md) 。

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
    void KeyGen(unsigned long bitLen); //产生密钥
    void Encrypt(mpz_t c, mpz_t m);    // 加密
    void Decrypt(mpz_t m, mpz_t c);    // 解密
    void Add(mpz_t res, mpz_t c1, mpz_t c2); // 同态加
    void Mul(mpz_t resc, mpz_t c, mpz_t e);  // 同态标量乘
```

调用：

```c++
int main()
{
    cout << "start ......" << endl;
    Paillier paillier;
    unsigned long bitLen = 1024;
    paillier.KeyGen(bitLen);
	// 初始化参数
    mpz_t m1, m2, res, c1, c2, mm, x;
    mpz_inits(m1, m2, res, c1, c2, mm, x, NULL);
    mpz_set_ui(m1, 4);   // 设置 m1 = 4
    mpz_set_ui(m2, 5);   // 设置 m2 = 5
	// 加密
    paillier.Encrypt(c1, m1);
    paillier.Encrypt(c2, m2);
	// 密文相加
    paillier.Add(res, c1, c2);
    paillier.Decrypt(mm, res);
    cout << "4+5 =" << mm << endl;

    mpz_set_ui(res, 0);
    mpz_set_ui(mm, 0);
    // 密文（标量）相乘
    mpz_set_ui(x, 3);
    paillier.Mul(res, c1, x);
    paillier.Decrypt(mm, res);
    cout << "4*3 =" << mm << endl;

    return 0;
}
```

## 后续规划

需要考虑：

- 对密文的编码/解码：同态加密涉及到多方参与，可能会需要网络传输，这就需要将密文对象编码后才能传递给对方。
- 对公钥、私钥的持久化保存
