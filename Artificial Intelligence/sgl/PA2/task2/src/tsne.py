'''
tsne可视化
'''

import os
import numpy as np
import pandas as pd
from time import time
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE
import constants

def plot_embedding_2D(data, label, title):
    '''
    绘制2D tsne图
    '''
    x_min, x_max = np.min(data, 0), np.max(data, 0)
    data = (data - x_min) / (x_max - x_min)
    fig = plt.figure()
    for i in range(data.shape[0]):
        plt.text(data[i, 0], data[i, 1], str(label[i]),
                 color = get_color(label[i]),
                 fontdict = {'weight': 'bold', 'size': 9})
    plt.xticks([])
    plt.yticks([])
    plt.title(title)
    return fig

def GetTsneResult(TheData):
    '''
    描述：计算Tsne结果
    参数：带上labels的原始数据
    '''
    #计算TSNE结果
    UsefulData = TheData[constants.UsefulFeatures]
    tsne = TSNE()
    tsne.fit_transform(UsefulData) #进行数据降维,并返回结果
    Result = pd.DataFrame(tsne.embedding_, index = UsefulData.index) #转换数据格式
    
    #正规化
    x_min, x_max = np.min(Result, 0), np.max(Result, 0)
    Result = (Result - x_min) / (x_max - x_min)
    return Result

def PlotTsnePredict(TheData, Result, FeatureNum, MethodName):
    '''
    描述：绘制t_sne可视化（按照自己的分类）
    参数：带上labels的原始数据
    '''  
    plt.rcParams['font.sans-serif'] = ['SimHei'] #用来正常显示中文标签
    plt.rcParams['axes.unicode_minus'] = False #用来正常显示负号

    #不同类别用不同颜色和样式绘图
    for i in range(constants.NClusters):
        d = Result[TheData["labels"] == i]
        plt.plot(d[0], d[1], 'o', color = constants.ColorCluster[i - 1])
    plt.title("t-SNE plot of clusters")
    #plt.legend()
    FileName = "../result/" + MethodName + "_feature" + str(FeatureNum) + ".png"
    plt.savefig(FileName)
    #plt.show()
    
def PlotTsneGroundTruth(TheData, Result, Num):
    '''
    描述：绘制t_sne可视化（按照科）
    参数：原始数据
    '''

    plt.rcParams['font.sans-serif'] = ['SimHei'] #用来正常显示中文标签
    plt.rcParams['axes.unicode_minus'] = False #用来正常显示负号

    #不同类别用不同颜色和样式绘图
    for i in range(len(constants.Family)):
        d = Result[TheData["Family"] == constants.Family[i]]
        plt.plot(d[0], d[1], 'o', color = constants.ColorFamily[i], label = constants.Family[i])
    plt.title("t-SNE plot of families")
    plt.legend()
    
    MethodName = ["KMeans", "Self", "Hier"]
    FileName = "../result/" + MethodName[Num - 1] + "_groundtruth.png"
    plt.savefig(FileName)
    #plt.show()
