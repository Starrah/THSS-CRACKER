import numpy as np
from data import get_data
from sklearn.cluster import KMeans, AgglomerativeClustering
from sklearn.metrics import calinski_harabasz_score, silhouette_score
from model import MyKMeans

from config import *
from utils import *
from visualize import *

# 以下函数用于在实验报告中出现的图表的计算

# 各种未知真实标签的评价指标的曲线
def do_plot():
    plot_metrics(features_modes=['feature1', 'feature2', 'feature3'], 
        model_names=['Agglomerative'],
        score_names=['SS'],
        save_path='figure/Agglomerative_SS.png')

# Agglomerative 的 linkage 超参选择，用 tsne 可视化
def do_agg_tuning():
    # data, label, origin_data = get_data('top', 22) # 特征组合1
    # data, label, origin_data = get_data('top', FEATURE_STD_TOP) # 特征组合2
    data, label, origin_data = get_data('select', feature_select=BEST_SELECT) # 特征组合3
    AgglomerativeDistanceTuning(data, label, k=4, save_path='figure/aggTuning.png')

# 已知真实标签的评价指标的计算
def do_evaluate_with_label():
    data, label, origin_data = get_data('top', 22) # 特征组合1
    print("feature combination 1")
    for model in ['KMeans','MyKMeans','Agglomerative']:
        res = evaluate_with_label(model, data, label)
        print(model, 'Entropy = ', res[0][0], 'Purity = ', res[1][0])
    
    print("feature combination 2")
    data, label, origin_data = get_data('top', FEATURE_STD_TOP) # 特征组合2
    for model in ['KMeans','MyKMeans','Agglomerative']:
        res = evaluate_with_label(model, data, label)
        print(model, 'Entropy = ', res[0][0], 'Purity = ', res[1][0])
    
    print("feature combination 3")
    data, label, origin_data = get_data('select', feature_select=BEST_SELECT) # 特征组合3
    for model in ['KMeans','MyKMeans','Agglomerative']:
        res = evaluate_with_label(model, data, label)
        print(model, 'Entropy = ', res[0][0], 'Purity = ', res[1][0])

# tsne 可视化
def do_tsne_plot():
    data, label, origin_data = get_data('select', feature_select=BEST_SELECT)
    k=4
    for model in ['KMeans','Agglomerative','MyKMeans']:
        if model=='KMeans':
            cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
        elif model=='Agglomerative':
            cluster = AgglomerativeClustering(n_clusters=k, linkage='ward').fit_predict(data)
        elif model=='MyKMeans':
            cluster = MyKMeans(n_clusters=k).fit_predict(data)
        tsne_visualize(data, cluster, 2, f'figure/tsne_{model}_2d.png')
        tsne_visualize(data, cluster, 3, f'figure/tsne_{model}_3d.png')

# 比较 KMeans 的运行效率
def do_kmeans_time_compare():
    data, label, origin_data = get_data('select', feature_select=BEST_SELECT)
    k=4

    import time
    start = time.clock()
    cluster = KMeans(n_clusters=k, random_state=0).fit_predict(data)
    elapsed = (time.clock() - start)
    print("KMeans Time used:", elapsed)

    start = time.clock()
    cluster = MyKMeans(n_clusters=k).fit_predict(data)
    elapsed = (time.clock() - start)
    print("MyKMeans Time used:", elapsed)

if __name__=='__main__':
    # do_plot()
    # do_agg_tuning()
    do_evaluate_with_label()
    # do_tsne_plot()
    # do_kmeans_time_compare()

    