import numpy as np
import sklearn
from sklearn.model_selection import train_test_split, cross_validate
from imblearn.over_sampling import RandomOverSampler
from imblearn.pipeline import Pipeline

from config import *
from models import *
from utils import *
from data import get_data

# 交叉验证
def classify(model, data, label, cv=5): 
    scoring = {'accuracy': 'accuracy',
            'precision': 'precision',
            'recall': 'recall',
            'f1': 'f1',
            'roc_auc': 'roc_auc'}

    scores=cross_validate(model, data, label, cv=cv, scoring=scoring)

    return {'accuracy': scores['test_accuracy'].mean(), 
        'precision:': scores['test_precision'].mean(),
        'recall:': scores['test_recall'].mean(),
        'f1': scores['test_f1'].mean(),
        'roc_auc': scores['test_roc_auc'].mean()}


if __name__=='__main__':
    # 各种评价指标的交叉验证
    # data,label = get_data(MANUAL) # 特征组合 1
    data,label = get_data(BEST) # 特征组合 2
    print('knn:', classify(get_knn(), data, label))
    print('dtc:', classify(get_dtc(), data, label))
    print('rfc:', classify(get_rfc(), data, label))
    print('svc:', classify(get_svc(), data, label))
    print('mlp:', classify(get_mlp(), data, label))
    print('mymlp:', classify(get_mymlp(data.shape[1]), data, label))

    # 比较 MLP 与自己实现的时间效率差异
    # data,label = get_data(BEST)
    # mlp_time_compare(data, label)

    # knn 超参选择
    # data,label = get_data(BEST)
    # knn_tuning(data, label)

