import numpy as np
from sklearn.metrics import calinski_harabasz_score, silhouette_score

from data import get_data
import copy

from model import MyKMeans
from sklearn.cluster import KMeans, AgglomerativeClustering
from utils import evaluate_without_label
from config import *
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt 

import os

# tsne 可视化
def tsne_visualize(data, cluster, dim=2, save_path=None):
    tsne=TSNE(n_components=dim, random_state=0)
    embedding = tsne.fit_transform(data)
    if dim==3:
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        ax.scatter(embedding[:, 0], embedding[:, 1], embedding[:, 2], c=cluster)
    elif dim==2:
        plt.scatter(embedding[:,0],embedding[:,1],c=cluster)
    if save_path is None:
        plt.show()
    else:
        plt.savefig(save_path, dpi=300)
    plt.clf()


# AgglomerativeClustering 超参选择
def AgglomerativeDistanceTuning(data, label, k, save_path=None):
    tsne=TSNE(n_components=2, random_state=0)
    embedding = tsne.fit_transform(data)
    for i, link in enumerate(['ward','average','complete','single']):
        plt.subplot(2,2,i+1)
        cluster = AgglomerativeClustering(n_clusters=k, linkage=link).fit_predict(data)
        plt.title(link)
        plt.scatter(embedding[:,0],embedding[:,1],c=cluster)
    plt.subplots_adjust(wspace =0.2, hspace =0.3)
    if save_path is None:
        plt.show()
    else:
        plt.savefig(save_path, dpi=300)
    plt.clf()
    

# 不同评价指标的曲线
def plot_metrics(features_modes, model_names, score_names, save_path=None):
    import matplotlib.pyplot as plt 

    score_dict = {'CH':0,'SS':1,'SSE':2}
    score_name_dict = {'CH':'calinski harabasz score','SS':'silhouette score','SSE':'SSE'}
    model_dict = {'KMeans':0,'Agglomerative':1,'MyKMeans':2}
    feature_dict = {'feature1':0,'feature2':1,'feature3':2}

    if os.path.exists('metrics.npy'):
        print("metrics.npy exists! load it.")
        metrics = np.load('metrics.npy')
    else:
        k_range = range(2,10)
        print("can not find metrics.npy, re-evluate ...")
        metrics = np.zeros((3,3,3,len(k_range)))

        for i, features_mode in enumerate(['all','top','select']):
            print(f"feature mode: feature{i+1}:", features_mode)
            if features_mode=='top':
                data, label,_ = get_data('top', FEATURE_STD_TOP)
            elif features_mode=='all':
                data, label,_ = get_data('top', len(FEATURES))
            elif features_mode=='select':
                data, label,_ = get_data('select', feature_select=BEST_SELECT)

            for j, model_name in enumerate(['KMeans','Agglomerative','MyKMeans']):
                print("model:", model_name)
                scores = evaluate_without_label(model_name, data, k_range)
                for s in range(0,3):
                    metrics[i,j,s,:] = scores[s]
        np.save('metrics.npy', metrics)
        print('metrics.npy saved!')

    k_range = range(2,10)
    for f in features_modes:
        for m in model_names:
            for s in score_names:
                plt.plot(k_range, metrics[feature_dict[f],model_dict[m],score_dict[s]], '*-', label=f'{f}_{m}_{s}')

    plt.xlabel('k')
    plt.ylabel(score_name_dict[score_names[0]])
    plt.legend(loc='lower left')

    if save_path is None:
        plt.show()
    else:
        plt.savefig(save_path, dpi=300)
    