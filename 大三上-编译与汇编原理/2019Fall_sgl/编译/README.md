# C2LLVM Compiler

## 环境

* 系统：Ubuntu16.04
* 语言：Python3.6



## 安装

安装[antlr4](https://www.antlr.org/)。

安装antlr4-python3-runtime

```
pip install antlr4-python3-runtime
```

安装llvmlite

```
pip install llvmlite
```



## 使用

**注意以下命令运行路径为`/src`目录下，即与`main.py`同级**

生成某个源代码的LLVM IR代码

```shell
python main.py test/xxx.c   #该命令会在源代码同级目录下生成xxx.ll文件
```

快速编译test文件夹下所有源代码

```shell
python compile_all.py       #该命令会编译test文件夹下所有源代码
```

运行LLVM IR代码

```shell
lli xxx.ll
```



## 测试用例

1、`src/test/palindrome.c`回文检测

2、`src/test/kmp.c`字符串搜索

3、`src/test/cal.c`四则运算计算器

4、`src/test/biTree.c`二叉树

5、`src/test/quickSort.c`快排