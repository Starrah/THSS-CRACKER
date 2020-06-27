from sklearn.cluster import AgglomerativeClustering
import numpy as np
import pandas as pd
import constants
import data

def RunHierarchy(FeatureList, TheData):
    '''
    描述：调用sklearn库进行层次聚类
    参数：feature列表， 数据
    返回：原始数据+聚类结果
    '''
    UseData = TheData[FeatureList]
    if constants.DistanceType == "Cosine":
        estimator = AgglomerativeClustering(n_clusters = constants.NClusters, affinity = "cosine", linkage = "average") 
    else:
        estimator = AgglomerativeClustering(n_clusters = constants.NClusters)
    estimator.fit(UseData)
    
    OutputData = pd.concat([TheData, pd.Series(estimator.labels_, index = TheData.index, name = "labels")], axis = 1)
    return OutputData



