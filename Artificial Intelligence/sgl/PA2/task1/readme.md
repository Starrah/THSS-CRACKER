#### 实验环境：

- ubuntu 18.04
- python 3.6.9
- numpy 1.18.3
- pandas 1.0.3
- scikit-learn 0.22.2.post1
- seaborn 0.10.1
- matplotlib 3.2.1
- pytorch 1.4.0

#### 文件描述：

- constants.py 全局常量
- data.py 数据读取相关函数
- correlation.py 求相关性，用于特征选取
- bayes.py 调用sklearn库的朴素贝叶斯分类器
- self_model.py 自己实现的朴素贝叶斯分类器
- svm.py 调用sklearn库的svm分类器
- mlp_model.py, mlp.py 调用pytorch的全连接神经网络分类器
- main.py 主函数

#### 运行方法：

主函数

```
python main.py
```

求相关性

```
python correlation.py
```

