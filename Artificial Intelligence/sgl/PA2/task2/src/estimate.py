'''
评价聚类结果的函数
'''

import pandas as pd
import numpy as np
import constants
import data
import math

def GetPurity(TheData):
    '''
    描述：求纯度（有的cluster分到0，求不了熵）
    参数：带标签的数据
    返回：纯度
    '''
    NewData = TheData[["Family","labels"]].values
    Entropy = 0
    Purity = 0
    for i in range(constants.NClusters):
        #对每个cluster计数
        CountNum = [0, 0, 0, 0]
        for j in range(len(NewData)):
            if NewData[j][1] == i:
                TheFamily = NewData[j][0]
                TheNum = constants.Family.index(TheFamily)
                CountNum[TheNum] += 1
        Frac = (sum(CountNum))/(len(NewData))
        #求纯度
        MaxNum = max(CountNum)
        if sum(CountNum) != 0:
            ThePurity = MaxNum * Frac / (sum(CountNum))
        else:
            ThePurity = Frac
        Purity += ThePurity
    return Purity
            
        
def GetSSEAndBSS(TheData):
    '''
    描述：SSE,BSS
    参数：带标签的数据
    返回：SSE,BSS
    '''
    #求各种m
    CalculateData = TheData[constants.UsefulFeatures].values
    LabelData = TheData["labels"].values
    TotalAverage = []
    
    #m_total
    for j in range(22):
        TotalAverage.append(0)
    for j in range(len(LabelData)):
        TotalAverage += CalculateData[j]  
    TotalAverage = TotalAverage / len(LabelData)
    
    #m_i
    AverageList = []
    NumList = []
    for i in range(constants.NClusters):
        Average = []
        Num = 0
        for j in range(22):
            Average.append(0)
        for j in range(len(LabelData)):
            if LabelData[j] == i:
                Num += 1
                Average += CalculateData[j]
        if Num != 0:
            Average = Average / Num
        else:
            Average = 0
        AverageList.append(Average)
        NumList.append(Num)
    
    #SSE
    SSE = 0
    for i in range(constants.NClusters):
        for j in range(len(LabelData)):
            if LabelData[j] == i:
                TheSSE = constants.GetDistance(CalculateData[j], AverageList[i])
                SSE += TheSSE

    #BSS
    BSS = 0
    for i in range(constants.NClusters):
        TheBSS = constants.GetDistance(AverageList[i], TotalAverage)
        BSS += (TheBSS * NumList[i])
        
    return SSE, BSS            
    