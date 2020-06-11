import numpy as np
from sklearn.metrics import calinski_harabasz_score, silhouette_score

from data import get_data
import copy

# 类内凝聚度
def SSE(data, cluster):
    sse = 0
    for j in list(set(cluster)):
        c = data[cluster==j]
        mean = np.mean(c,0)
        sse += np.sum((c-mean)**2)
    return sse

# 熵
def entropy(label, cluster):
    c = len(list(set(cluster)))
    D = label.shape[0]
    Ent = 0
    ents = []
    for i in range(c):
        clu = cluster[cluster==i]
        lab = label[cluster==i]
        Di = clu.shape[0]
        ent = 0
        for j in range(c):
            cj = np.sum(lab==j)/Di
            if cj!=0:
               ent += -cj*np.log(cj)
        Ent += ent*Di/D
        ents.append(ent)
    return Ent, ents

# 纯度
def purity(label, cluster):
    c = len(list(set(cluster)))
    D = label.shape[0]
    Pure = 0
    pures = []
    for i in range(c):
        clu = cluster[cluster==i]
        lab = label[cluster==i]
        Di = clu.shape[0]
        pure = 0
        for j in range(c):
            cj = np.sum(lab==j)/Di
            if cj!=0:
               pure = max(pure, cj)
        Pure += pure*Di/D
        pures.append(pure)
    return Pure, pures

# 未知真实标签的评价指标：CH, SS, SSE
def evaluate_without_label(model : str, data, k_range=range(2,10)):
    from sklearn.cluster import KMeans, AgglomerativeClustering
    from model import MyKMeans
    
    ch, ss, sse = [],[],[]
    for k in k_range:
        if model=='KMeans':
            cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
        elif model=='Agglomerative':
            cluster = AgglomerativeClustering(n_clusters=k, linkage='ward').fit_predict(data)
        elif model=='MyKMeans':
            cluster = MyKMeans(n_clusters=k).fit_predict(data)
        else:
            raise NotImplementedError
        ch.append(calinski_harabasz_score(data, cluster))
        ss.append(silhouette_score(data, cluster))
        sse.append(SSE(data, cluster))
        print(k, ch[-1], ss[-1], sse[-1])
    return ch, ss, sse

# 已知真实标签的评价指标：熵，纯度
def evaluate_with_label(model : str, data, label):
    from sklearn.cluster import KMeans, AgglomerativeClustering
    from model import MyKMeans

    k = len(list(set(label)))
    if model=='KMeans':
        cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
    elif model=='Agglomerative':
        cluster = AgglomerativeClustering(n_clusters=k, linkage='ward').fit_predict(data)
    elif model=='MyKMeans':
        cluster = MyKMeans(n_clusters=k).fit_predict(data)
    else:
        raise NotImplementedError

    return entropy(label, cluster), purity(label, cluster)

# 前向选择算法
def forward_feature_selection(k, features_names, target_num, score_func = silhouette_score):
    current = []
    best_select = None
    for i in range(target_num):
        stage_best = None
        for f in features_names:
            if f not in current:
                current.append(f)
                # print(current)
                data,label,origin_data = get_data('select', feature_select = current)
                cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
                score = score_func(origin_data, cluster)
                # print(f"try {current}, ", score)
                if stage_best is None or stage_best[1]<score:
                    stage_best = (f, score)
                current.pop()
        current.append(stage_best[0])
        if best_select is None or stage_best[1]>best_select[1]:
            best_select = (copy.deepcopy(current), stage_best[1])
        print(f"k = {i}, best_score = {best_select[1]}, ", len(best_select[0]), best_select[0])

    return best_select[0]

# 后向选择算法
def backward_feature_selection(k, features_names, target_num, score_func = silhouette_score):
    current = features_names
    best_select = None
    for i in range(len(current)-target_num):
        stage_best = None
        checkpoint = copy.deepcopy(current)
        for f in features_names:
            if f in current:
                current.remove(f)
                # print(current)
                data,label,origin_data = get_data('select', feature_select = current)
                cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
                score = score_func(origin_data, cluster)
                # print(f"try {current}, ", score)
                if stage_best is None or stage_best[1]<score:
                    stage_best = (f, score)
                current = copy.deepcopy(checkpoint)
        current.remove(stage_best[0])
        if best_select is None or stage_best[1]>best_select[1]:
            best_select = (copy.deepcopy(current), stage_best[1])
        print(f"k = {i}, best_score = {best_select[1]}, ", len(best_select[0]), best_select[0])

    return best_select[0]