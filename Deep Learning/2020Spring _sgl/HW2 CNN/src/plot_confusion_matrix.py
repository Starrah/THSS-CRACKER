'''
绘制一个模型的混淆矩阵
'''

import matplotlib.pyplot as plt    # 绘图库
import numpy as np
from torchvision import models
import itertools
import torch
import torch.nn as nn
import data
import parameters as hyper

def get_confusion_matrix(model, valid_loader, device):
    '''
    代入模型，获取混淆矩阵
    '''
    confusion_matrix = torch.zeros(hyper.num_classes, hyper.num_classes)
    model.train(False)
    for inputs, labels in valid_loader:
        inputs = inputs.to(device)
        labels = labels.to(device)
        outputs = model(inputs)
        _, predictions = torch.max(outputs, 1)
    
        for i in range(len(predictions)):
            x = int(predictions[i])
            y = int(labels.data[i])
            confusion_matrix[x][y] += 1
    return confusion_matrix

def plot_confusion_matrix(cm, classes, model_name, normalize=False, cmap=plt.cm.Blues):
    '''
    绘制混淆矩阵
    参数：
    - cm : 计算出的混淆矩阵的值
    - classes : 混淆矩阵中每一行每一列对应的列
    - model_name: 模型名称，用于设置标题和保存文件
    - normalize : True:显示百分比, False:显示个数
    ''' 
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    title = "Confusion model of model " + model_name
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=90)
    plt.yticks(tick_marks, classes)
    
    # x,y轴长度一致
    plt.axis("equal")
    # x轴处理一下，如果x轴或者y轴两边有空白的话
    ax = plt.gca()  # 获得当前axis
    left, right = plt.xlim()  # 获得x轴最大最小值
    ax.spines['left'].set_position(('data', left))
    ax.spines['right'].set_position(('data', right))
    for edge_i in ['top', 'bottom', 'right', 'left']:
        ax.spines[edge_i].set_edgecolor("white")

    #对齐
    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        num = '{:.2f}'.format(cm[i, j]) if normalize else int(cm[i, j])
        plt.text(j, i, num,
             verticalalignment='center',
             horizontalalignment="center",
             color="white" if num > thresh else "black")
    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    save_place = "../result/visualizations/" + model_name + "_confusion_matrix.jpg"
    plt.savefig(save_place)
    plt.show()


def plot_matrix(model_name, model, valid_loader, device):
    '''
    绘制混淆矩阵
    '''
    '''model = torch.load("result/best_model.pt")
    _, valid_loader, _ = data.load_data \
    (data_dir = hyper.data_dir,input_size = hyper.input_size)
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model = model.to(device)'''
    confusion_matrix = get_confusion_matrix(model, valid_loader, device)
    classes = []
    for i in range(hyper.num_classes):
        classes.append(str(i))
    plot_confusion_matrix(confusion_matrix, classes, model_name)
