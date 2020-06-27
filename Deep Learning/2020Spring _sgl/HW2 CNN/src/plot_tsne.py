'''
tsne可视化
'''

import os
import numpy as np
from time import time
import matplotlib.pyplot as plt
from sklearn import datasets #手写数据集要用到
from mpl_toolkits.mplot3d import Axes3D
from sklearn.manifold import TSNE
from torchvision import models
import torch
import torch.nn as nn
import data
import parameters as hyper

def get_color(label):
    '''
    获取对应的颜色
    '''
    color_list = ["black", "silver", "brown", "red", "sienna", "wheat", "gold", "yellow" ,"olivedrab", "green",
                  "seagreen", "darkcyan", "skyblue", "blue", "violet" ,"purple", "hotpink", "navy", "orange", "turquoise"]
    return color_list[label]

#该函数是关键，需要根据自己的数据加以修改，将图片存到一个np.array里面，并且制作标签
#因为是两类数据，所以我分别用0,1来表示
def get_data(model, valid_loader, device): #Input_path为你自己原始数据存储路径，我的路径就是上面的'./Images'
    '''
    代入模型，获取feature
    '''
    model.train(False)
    the_data = []
    the_label = []
    for inputs, labels in valid_loader:
        inputs = inputs.to(device)
        outputs = model.get_feature(inputs)
        n_features = len(outputs[0])

        for i in range(len(labels)):
            the_label.append(int(labels[i]))
            the_data.append(outputs[i].cpu().detach().numpy())
        #print(labels)
        #print(outputs[0].size())
    n_samples = len(the_data)
    print(the_data)
    print(the_label)
    print(n_samples)
    print(n_features)
    return the_data, the_label, n_samples, n_features

def plot_embedding_2D(data, label, title):
    '''
    绘制2D tsne图
    '''
    x_min, x_max = np.min(data, 0), np.max(data, 0)
    data = (data - x_min) / (x_max - x_min)
    fig = plt.figure()
    for i in range(data.shape[0]):
        plt.text(data[i, 0], data[i, 1], str(label[i]),
                 color=get_color(label[i]),
                 fontdict={'weight': 'bold', 'size': 9})
    plt.xticks([])
    plt.yticks([])
    plt.title(title)
    return fig

def plot_tsne(model_name, model, valid_loader, device):
    '''
    绘制t_sne可视化
    '''
    #读取模型，计算结果
    data, label, n_samples, n_features = get_data(model, valid_loader, device)
    
    #2d,3d可视化
    print('Begining......') #时间会较长，所有处理完毕后给出finished提示
    tsne_2D = TSNE(n_components=2, init='pca', random_state=0, perplexity = 10) #调用TSNE
    result_2D = tsne_2D.fit_transform(data)
    print('Finished......')
    #调用上面的函数进行可视化
    fig1 = plot_embedding_2D(result_2D, label,'2D t-SNE of model ' + model_name)
    plt.savefig("../result/visualizations/" + model_name + "_tsne_2D.jpg")
    plt.show(fig1)
