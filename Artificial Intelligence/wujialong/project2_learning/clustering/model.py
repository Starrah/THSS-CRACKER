import numpy as np
import copy
from utils import SSE

class MyKMeans(object):
    def __init__(self, n_clusters):
        self.n_clusters = n_clusters
    
    # 初始随机选择 k 个点作为中心点
    def rand_select_cent(self, data):
        n,dim = data.shape
        centroids = np.zeros((self.n_clusters,dim))
        for j in range(self.n_clusters):
            centroids[j] = data[np.random.randint(n)]
        return centroids

    def fit_predict(self, data, T=10):
        n = data.shape[0]
        cluster = np.zeros(n)

        best_sse = 1e30
        best = None
        
        # 重复 T 次根据 SSE 选最优结果 
        for t in range(T):
            centroids = self.rand_select_cent(data)
            clusterChanged = True
            while clusterChanged:
                clusterChanged = False
                # 归类到最近的中心点
                for i in range(n):
                    dist=np.sum((centroids[:,:]-data[i,:])**2,axis=1)
                    
                    argMin = np.argmin(dist)
                    if cluster[i] != argMin: 
                        clusterChanged = True
                    cluster[i] = argMin
                
                # 求新的中心点
                for c in range(self.n_clusters):
                    points = data[cluster==c]
                    if points.size!=0:
                        centroids[c] = np.mean(points, axis=0)
                    else:
                        centroids[c] = data[np.random.randint(n)]
            
            sse = SSE(data, cluster)
            if best is None or sse < best_sse:
                best = copy.deepcopy(cluster)
                best_sse = sse

        return best
