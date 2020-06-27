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

Marriage = {"single":1, "married":2, "divorced":3, "unknown":-1}
Education = {"primary":1, "secondary":2, "tertiary":3, "unknown":-1}
Month = {"jan":1, "feb":2, "mar":3, "apr":4, "may":5, "jun":6, "jul":7, "aug":8, "sep":9, "oct":10, "nov":11, "dec":12}
OutCome = {"success":1, "failure":0, "unknown":-1, "other": -2}
Bool = {"yes":1, "no":0, "unknown": -1}
Job = {}
Contact = {}

def CleanData(data):
    '''
    描述：将数据全部转化为数字
    '''
    #编码所有job
    for item in data["job"]:
        if item not in Job.keys():
            Value = len(Job.keys())
            Job[item] = Value
    #编码所有contact
    for item in data["contact"]:
        if item not in Contact.keys():
            Value = len(Contact.keys())
            Contact[item] = Value
    #清理job
    for i in range(len(data["job"])):
        value = data["job"][i]
        data["job"][i] = Job[value]
    #清理marital
    for i in range(len(data["marital"])):
        value = data["marital"][i]
        data["marital"][i] = Marriage[value]
    #清理education
    for i in range(len(data["education"])):
        value = data["education"][i]
        data["education"][i] = Education[value]
    #清理default
    for i in range(len(data["default"])):
        value = data["default"][i]
        data["default"][i] = Bool[value]
    #清理housing
    for i in range(len(data["housing"])):
        value = data["housing"][i]
        data["housing"][i] = Bool[value]
    #清理loan
    for i in range(len(data["loan"])):
        value = data["loan"][i]
        data["loan"][i] = Bool[value]
    #清理contact
    for i in range(len(data["contact"])):
        value = data["contact"][i]
        data["contact"][i] = Contact[value]
    #清理month
    for i in range(len(data["month"])):
        value = data["month"][i]
        data["month"][i] = Month[value]
    #清理pdays
    for i in range(len(data["pdays"])):
        value = data["pdays"][i]
        if value == -1:
            data["pdays"][i] = 999
    #清理poutcome
    for i in range(len(data["poutcome"])):
        value = data["poutcome"][i]
        data["poutcome"][i] = OutCome[value]
    return data

def ReadData():
    '''
    描述：用panda读取csv数据，并且进行清理
    参数：无
    返回：整个data集合
    '''
    data = pd.read_csv(constants.DataDir)
    #data = data.astype(int)
    data = CleanData(data).astype(int)
    return data

def SplitData(FeatureList, data):
    '''
    描述：读取csv数据，手动进行k折拆分
    参数：一个list，代表特征列表,还有数据
    返回：x训练集，x测试集，y训练集，y测试集---每个是一个list，list每个元素是一个list，代表实际数据
    '''
    
    #data = pd.read_csv(constants.DataDir)
    #data = CleanData(data)
    kf = KFold(n_splits = constants.KFolds ,shuffle = True)
    TrainX = []
    TestX = []
    TrainY = []
    TestY = []
    for train_index, test_index in kf.split(data["y"]):
        print('train_index %s, test_index %s'%(train_index,test_index))
        NewTrainX = {}
        NewTestX = {}
        NewTrainY = []
        NewTestY = []
        for item in FeatureList:
            NewTrainX[item] = []
            NewTestX[item] = []
            for i in train_index:
                NewTrainX[item].append(data[item][i])
            for i in test_index:
                NewTestX[item].append(data[item][i])
        NewTrainY = []
        NewTestY = []
        for i in train_index:
            NewTrainY.append(data["y"][i])
        for i in test_index:
            NewTestY.append(data["y"][i]) 
        TrainX.append(NewTrainX)
        TrainY.append(NewTrainY)
        TestX.append(NewTestX)
        TestY.append(NewTestY)
    return TrainX, TrainY, TestX, TestY
    
def ReadDataSelf(FeatureList):
    '''
    描述：手动读取CSV库，并且进行清洗
    参数：feature列表
    返回: x训练集，x测试集，y训练集，y测试集
    '''
    data = {}
    with open(constants.DataDir, "r") as f:
        reader = csv.DictReader(f)
        header = reader.fieldnames
        for item in header:
            data[item] = []
        for line in reader: #line 为 dict 类型 数据由line[key]的形式获得
            for item in header:
                try:
                    data[item].append(int(line[item]))
                except:    
                    data[item].append(line[item])
    data = CleanData(data)
    #print(data)

    TrainX, TrainY, TestX, TestY = SplitData(FeatureList, data)
    return TrainX, TrainY, TestX, TestY

class NNData(torch.utils.data.Dataset):
    '''
    pytorch方法读取csv数据
    '''
    def __init__(self, x, y):
        """
        初始化：记录x, y
        """
        self.x = x
        self.y = y
    def __len__(self):
        return len(self.y)

    def __getitem__(self, index):
        x = self.x[index].tolist()
        y = self.y[index].tolist()
        x = torch.Tensor(x)
        y = torch.Tensor(y)
        return x, y
        
def ReadNNData(FeatureList):
    '''
    pytorch方法读取csv数据
    '''
    data = pd.read_csv(constants.DataDir)
    data = CleanData(data)

    kf = KFold(n_splits = constants.KFolds ,shuffle = True)
    Loader = []
    for train_index, test_index in kf.split(data["y"]):
        TrainX = data[FeatureList].values[train_index]
        TestX = data[FeatureList].values[test_index]
        TrainY = data[["y"]].values[train_index]
        TestY = data[["y"]].values[test_index]
        TrainData = NNData(TrainX, TrainY)
        TestData = NNData(TestX, TestY)
        trainloader = DataLoader(TrainData, batch_size = constants.NNBatchs, shuffle = True)
        testloader = DataLoader(TestData, batch_size = constants.NNBatchs, shuffle = True)
        Loader.append({"train":trainloader, "test":testloader})
    return Loader
