'''
读取csv数据，并且按照特征进行k折拆分，返回训练集和测试集
'''
import constants
import pandas as pd
import numpy as np
import time
import csv
import torch
from torch.utils.data import DataLoader
import torch.utils.data 
from sklearn.model_selection import KFold

data = pd.read_csv(constants.DataDir)
Family = {}
Genus = {}
Species = {}



def CleanData(data):
    '''
    描述：将数据全部转化为数字
    '''
    #编码所有科
    for item in data["Family"]:
        if item not in Family.keys():
            Value = len(Family.keys())
            Family[item] = Value
    #编码所有属
    for item in data["Genus"]:
        if item not in Genus.keys():
            Value = len(Genus.keys())
            Genus[item] = Value
    #编码所有种
    for item in data["Species"]:
        if item not in Species.keys():
            Value = len(Species.keys())
            Species[item] = Value
    #清理科
    for i in range(len(data["Family"])):
        value = data["Family"][i]
        data["Family"][i] = Family[value]
    #清理属
    for i in range(len(data["Genus"])):
        value = data["Genus"][i]
        data["Genus"][i] = Genus[value]
    #清理种
    for i in range(len(data["Species"])):
        value = data["Species"][i]
        data["Species"][i] = Species[value]
    #data = data.astype(float)
    return data

def ReadData():
    '''
    描述：用panda读取csv数据，并且进行清理
    参数：无
    返回：整个data集合
    '''
    data = pd.read_csv(constants.DataDir)
    return data

