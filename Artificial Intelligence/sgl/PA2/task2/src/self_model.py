import pandas as pd
import numpy as np
import constants
import data
import random
    

def GetRandomCenters(TheData):
    '''
    描述：找到两个随机的质心
    参数：数据
    返回：center1，center2
    '''
    n = len(TheData[0])
    Center1 = []
    Center2 = []
    for i in range(n):#create random cluster centers, within bounds of each dimension
        CurrentMin = min(TheData[ : ][i]) 
        CurrentMax = max(TheData[ : ][i])
        New1 = random.uniform(CurrentMin, CurrentMax)
        New2 = random.uniform(CurrentMin, CurrentMax)
        Center1.append(New1)
        Center2.append(New2)
    return Center1, Center2



def CalculateDist(ThePoint, TheCenter):
    '''
    描述：求得当前情况这个点与中心的距离（余弦）
    参数：点，质心
    返回：loss
    '''
    return constants.GetDistance(ThePoint, TheCenter)


def CalculateLoss(TheData, TheCenter):
    '''
    描述：求得当前情况这个cluster的loss(余弦)
    参数：数据，质心
    返回：loss
    '''
    Loss = 0
    for item in TheData:
        TheLoss = constants.GetDistance(item, TheCenter)
        Loss += TheLoss
    return Loss

    
def RenewCenter(TheData):
    '''
    描述：更新一个cluster的质心为平均
    参数：数据
    返回：center
    '''
    Center = np.mean(TheData, axis=0)
    return Center

def AllocatePoints(Center1, Center2, Original1, Original2, OldTag1, OldTag2):
    '''
    描述：给每个点分配质心
    参数：两个质心, 原有的cluster1， cluster2
    返回：data1，data2，是否有变化
    '''
    WhetherChanged = False
    Data1 = []
    Data2 = []
    Tag1 = []
    Tag2 = []
    
    for i in range(len(Original1)):
        item = Original1[i]
        Loss1 = CalculateDist(item, Center1)
        Loss2 = CalculateDist(item, Center2)
        if Loss1 <= Loss2:
            Data1.append(item)
            Tag1.append(OldTag1[i])
        else:
            Data2.append(item)
            Tag2.append(OldTag1[i])
            WhetherChanged = True
            
    for i in range(len(Original2)):
        item = Original2[i]
        Loss1 = CalculateDist(item, Center1)
        Loss2 = CalculateDist(item, Center2)
        if Loss1 <= Loss2:
            Data1.append(item)
            Tag1.append(OldTag2[i])
            WhetherChanged = True       
        else:
            Data2.append(item)
            Tag2.append(OldTag2[i])
            
    return Data1, Data2, Tag1, Tag2, WhetherChanged
    
    
def Kmeans2(TheData, TheTag, MaxIterTime):
    '''
    描述：将一组数据切分成2个cluster
    参数：数据，迭代次数
    返回：data1，tag1, center1，data2，tag2, center2
    '''
    i = 0
    WhetherChanged = True
    Center1, Center2 = GetRandomCenters(TheData)
    Data1 = TheData
    Data2 = []
    Tag1 = TheTag
    Tag2 = []
    while i < MaxIterTime and WhetherChanged == True:
        Data1, Data2, Tag1, Tag2, WhetherChanged = AllocatePoints(Center1, Center2, Data1, Data2, Tag1, Tag2)
        Center1 = RenewCenter(Data1)
        Center2 = RenewCenter(Data2)
        i += 1
    return Data1, Tag1, Center1, Data2, Tag2, Center2
    
    
    
def KmeansBinary(TheData, TheTag):
    '''
    描述：二叉KMeans算法
    参数：数据
    返回：Label--每个数据的分类号
    '''
    ClusterList = []
    ClusterLoss = []
    GlobalLength = len(TheData)
    Labels = []
    for i in range(GlobalLength):
        Labels.append(-1)
    NowCenter = RenewCenter(TheData)
    ClusterList.append({"Data":TheData, "Tag":TheTag, "Center":NowCenter})
    ClusterLoss.append(CalculateLoss(TheData, NowCenter))
    while len(ClusterList) < constants.NClusters:
        #找到拆分最小的位置
        LossList = []
        StorageList = []
        for i in range(len(ClusterList)):
            CurrentData = ClusterList[i]["Data"]
            CurrentTag = ClusterList[i]["Tag"]
            Data1, Tag1, Center1, Data2, Tag2, Center2 = Kmeans2(CurrentData, CurrentTag, constants.NIterations)
            Loss1 = CalculateLoss(Data1, Center1)
            Loss2 = CalculateLoss(Data2, Center2)
            NewLoss = ClusterLoss[:i] + [Loss1, Loss2] + ClusterLoss[i + 1:]
            LossList.append(NewLoss)
            StorageList.append({"Data1":Data1, "Data2":Data2, "Tag1":Tag1, "Tag2":Tag2, \
                "Center1":Center1, "Center2":Center2, "Loss1":Loss1, "Loss2":Loss2})
        MinPlace = LossList.index(min(LossList))
        
        #拆分
        TheData1 = StorageList[MinPlace]["Data1"]
        TheData2 = StorageList[MinPlace]["Data2"]
        TheTag1 = StorageList[MinPlace]["Tag1"]
        TheTag2 = StorageList[MinPlace]["Tag2"]
        TheCenter1 = StorageList[MinPlace]["Center1"]
        TheCenter2 = StorageList[MinPlace]["Center2"]
        TheLoss1 = StorageList[MinPlace]["Loss1"]
        TheLoss2 = StorageList[MinPlace]["Loss2"]
        NewCluster = [{"Data":TheData1, "Tag": TheTag1, "Center": TheCenter1}, \
            {"Data":TheData2, "Tag":TheTag2, "Center": TheCenter2}]
        NewLoss = [TheLoss1, TheLoss2]
        #print(ClusterList[:MinPlace])
        ClusterList = ClusterList[ :MinPlace] + NewCluster + ClusterList[MinPlace + 1 : ]
        ClusterLoss = ClusterLoss[ :MinPlace] + NewLoss + ClusterLoss[MinPlace + 1: ]
    
    #得到最终标签
    for i in range(len(ClusterList)):
        TheTag = ClusterList[i]["Tag"]
        for j in range(len(TheTag)):
            Labels[TheTag[j]] = i
    return Labels        

    
def RunSelf(FeatureList, TheData):
    '''
    描述：自己进行k分类
    参数：feature列表， 数据
    返回：原始数据+聚类结果
    '''
    UsedData = TheData[FeatureList].values
    UsedTag = []
    for i in range(len(UsedData)):
        UsedTag.append(i)
    Labels = KmeansBinary(UsedData, UsedTag)
    OutputData = pd.concat([TheData, pd.Series(Labels, index = TheData.index, name = "labels")], axis = 1)
    return OutputData
    