'''
全局常量，超参数所在位置
'''
import numpy as np
import math

DataDir = "../data/Frogs_MFCCs.csv"

TestRound = 5
NClusters = 4
NIterations = 20

#Eucilid,Cosine两个选项
DistanceType = "Cosine"
#DistanceType = "Eucilid"

#每个聚类的颜色
ColorCluster = ["black", "red", "green", "blue"]
#每个科的颜色
Family = ["Leptodactylidae", "Dendrobatidae", "Hylidae", "Bufonidae"]
ColorFamily = ["black", "red", "green", "blue"]

#能用于分类的feature
UsefulFeatures = ["MFCCs_ 1", "MFCCs_ 2", "MFCCs_ 3", "MFCCs_ 4", "MFCCs_ 5", "MFCCs_ 6", "MFCCs_ 7", "MFCCs_ 8" \
    , "MFCCs_ 9", "MFCCs_10", "MFCCs_11", "MFCCs_12", "MFCCs_13", "MFCCs_14", "MFCCs_15", "MFCCs_16", "MFCCs_17" \
    , "MFCCs_18", "MFCCs_19", "MFCCs_20", "MFCCs_21", "MFCCs_22"]

def GetDistance(Vec1, Vec2):
    '''
    描述：求两个向量的距离,根据全局设置自动选择
    参数：两个向量
    返回：距离
    '''
    if DistanceType == "Cosine":
        return GetCosineDistance(Vec1, Vec2)
    else:
        return GetEucilidDistance(Vec1, Vec2)

def GetCosineDistance(Vec1, Vec2):
    '''
    描述：求两个向量的余弦距离 2(1-cos(x,y))
    参数：两个向量
    返回：余弦距离
    '''
    try:
        InnerProduct = np.dot(Vec1, Vec2)
        Length1 = math.sqrt(sum(np.array(Vec1) ** 2))
        Length2 = math.sqrt(sum(np.array(Vec2) ** 2))
        Cosine = InnerProduct / (Length1 * Length2)
        Dist = 2 * (1 - Cosine)
        return Dist
    except:
        return 2

def GetEucilidDistance(Vec1, Vec2):
    '''
    描述：求两个向量的欧式距离
    参数：两个向量
    返回：欧式距离
    '''
    try:
        Difference =  np.array(Vec1) - np.array(Vec2)
        Dist = sum(Difference ** 2)
        return Dist
    except:
        return 2