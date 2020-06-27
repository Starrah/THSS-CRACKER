'''
调用sklearn库进行k分类
'''
from sklearn.cluster import KMeans
from sklearn import preprocessing
import numpy as np
import pandas as pd
import constants
import data

def RunKMeans(FeatureList, TheData):
    '''
    描述：调用sklearn库进行kmeans分类
    参数：feature列表， 数据
    返回：原始数据+聚类结果
    '''
    UseData = TheData[FeatureList]    
    estimator = KMeans(n_clusters = constants.NClusters, max_iter = constants.NIterations)
    estimator.fit(UseData)
    
    OutputData = pd.concat([TheData, pd.Series(estimator.labels_, index = TheData.index, name = "labels")], axis = 1)
    return OutputData

    

