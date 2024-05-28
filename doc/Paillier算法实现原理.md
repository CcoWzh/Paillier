# Paillier算法实现原理

Paillier是一个支持加法同态的公钥密码系统，由Paillier在1999年的欧密会（EUROCRYPT）上首次提出。在众多PHE方案中，Paillier方案由于效率较高、安全性证明完备的特点，在各大顶会和实际应用中被广泛使用，是隐私计算场景中最常用的PHE实例化方案之一。

## 算法详情

### 密钥生成

- 随机选择两个大质数$p$，$q$；
- 计算 $n=pq$，和 $\lambda = lcm(p-1,q-1)$；
- 随机选择整数$g \in Z^*_{n^2}$ ；
- 计算 $\mu = (L(g^\lambda \ mod \ n^2 ))^{-1} \ mod \ n$ ，其中，$L$函数定义为：$L(x)=\frac{x-1}{n}$ ；
- 获得公钥$pk=(n,g)$，私钥$sk=(\lambda,\mu)$ 。

#### 关于符号的说明

- $gcm(a,b)$ ：表示两个数的最大公因数。
- $lcm(a,b)$：表示两个数的最小公倍数。

-  $ Z^*_{n}$：表示模 $n$ 意义下，$[0,n-1]$ 中所有与 $n$ 互质的元素的集合。
- $Z^*_{n^2}$：表示模 $n^2$ 意义下的可逆元素集合。即，对于正整数$n$ ，$Z^*_{n^2}$ 包含了模$n^2$ 意义下与$n^2$ 互质的所有元素。即，如果有$x \in Z^*_{n^2}$ ，则$x$ 满足以下条件：
  - $x$和$n^2$ 互质，即$gcd(x,n^2)=1$ 
  - $x$在模$n^2$ 意义下有逆元，即，存在$y$ ，使得 $xy≡1 \ (mod \ n^2)$

#### 关于取值和参数优化

> 注：一般取 $g=n+1$ ,则通过数学推导，有$ \mu = \lambda^{-1} mod \ n$。
>
> 因此，一般在编程时，只保存私钥为$sk=\lambda$

下面，对此进行证明：

在不影响算法正确性的前提下，为了简化运算，算法在密钥生成阶段，一般取 $g=n+1$ 。如果 $g=n+1$ ，则有如下推导：
$$
\begin{align}
   \mu &=(L(g^\lambda \ mod \ n^2 ))^{-1} \mod n \\\\ 
       &=(L((n+1))^\lambda \ mod \ n^2 ))^{-1} \mod n \\\\
       &=(\frac{(n+1)^\lambda \ mod \ n^2 -1}{n})^{-1} \mod n
\end{align}
$$
根据二项定理，知道：
$$
(n+1)^\lambda =  \sum_{k=0}^{\lambda} \binom{\lambda}{k} n^k
$$
根据上述公式，我们知道前面的$\lambda-1$ 项都是 $n^2$ 的倍数，在模$n^2$ 的时候都会是0，则有如下简约：
$$
\begin{align}
(n+1)^\lambda \ mod \ n^2 &=  \sum_{k=0}^{\lambda} \binom{\lambda}{k} n^k \ mod \ n^2 \\
                          &= \lambda n+1
\end{align}
$$
因此：
$$
\begin{align}
   \mu &=(\frac{(n+1)^\lambda \ mod \ n^2 -1}{n})^{-1} \ mod \ n \\
       &=(\frac{ \lambda n+1 -1}{n})^{-1} \ mod \ n  \\
       &=\lambda^{-1} \ mod \ n
\end{align}
$$
后续，加密过程中的计算加密内容 $g^mr^n \ mod \ n^2$ 的时候，也可简化计算量。

### 数据加密

- 输入明文消息 $m$，满足条件$0 \leq m < n$

- 选择随机数$r$，满足条件$0 \leq r < n$ ，且 $r \in Z^*_{n^2}$ 
- 计算密文 $c=g^mr^n \ mod \ n^2$

> 从上面的公式推导可以知道，如果 $g=n+1$ ，则有：

$$
\begin{align}
  g^m &= (n+1)^m \ mod \ n^2 \\
      &= \begin{pmatrix} m \\ 0 \end{pmatrix} n^m + \begin{pmatrix} m \\ 1 \end{pmatrix}n^{m-1} + ...+nm+1  \ mod \ n^2 \\
      &= nm+1  \ mod \ n^2
\end{align}
$$

这样，就加速了计算过程。

### 数据解密

- 输入密文$c$
- 计算明文消息 $m=L(c^{\lambda} \ mod \ n^2)\cdot \mu \ mod \ n$
