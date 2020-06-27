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
result文件夹：tsne可视化结果，其中groundtruth的代表用青蛙的科直接当做标签可视化的结果
src下：
- constants.py 全局常量
- data.py 数据读取相关函数
- correlation.py 求相关性，用于特征选取
- kmeans.py 调用sklearn库的Kmeans算法
- self_model.py 自己实现的Kmeans算法
- hierarchy.py 调用sklearn库的凝聚层次聚类算法
- estimate.py 计算评估指标的函数
- tsne.py 可视化相关代码
- ktest.py 测试不同k下的表现
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

测试不同k值

```
python ktest.py
```

注意：如果在主函数里运行自己的模型，或者测试不同的k值，有小概率因为边界条件报错，重新运行即可。
