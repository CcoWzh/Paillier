# Paillier原语库

Paillier是一个支持加法同态的公钥密码系统，由Paillier在1999年的欧密会（EUROCRYPT）上首次提出。在众多PHE方案中，Paillier方案由于效率较高、安全性证明完备的特点，在各大顶会和实际应用中被广泛使用，是隐私计算场景中最常用的PHE实例化方案之一。

## 使用说明

下载本代码后，执行如下命令：

```
mkdir build
cd ./build
cmake ..
make
```







## 后续规划

需要考虑：

- 对密文的编码/解码：同态加密涉及到多方参与，可能会需要网络传输，这就需要将密文对象编码后才能传递给对方。
- 对公钥、私钥的持久化保存
