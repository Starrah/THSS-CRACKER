from main import classify
from data import get_data
import copy

# 特征子集前向选择算法
def forward_feature_selection(model, features_names, score_name = 'roc_auc'):
    current = []
    best_select = None
    for i in range(len(features_names)-len(current)):
        stage_best = None
        for f in features_names:
            if f not in current:
                current.append(f)
                print(current)
                data,label = get_data(current)
                scores = classify(model, data, label)
                print(f"try {current}, ", scores)
                if stage_best is None or stage_best[1]<scores[score_name]:
                    stage_best = (f, scores[score_name])
                current.pop()
        current.append(stage_best[0])
        if best_select is None or stage_best[1]>best_select[1]:
            best_select = (copy.deepcopy(current), stage_best[1])
        print(f"k = {i}, best_{score_name} = {best_select[1]}, ", best_select[0])

    return best_select

# KNN 超参选择
def knn_tuning(data, label, k_range=range(1,11)):
    from models import get_knn

    k_f1=[]
    for k in k_range:
        knn = get_knn(k=k)
        scores = classify(knn, data, label)
        k_f1.append(scores['f1'])
        print(f'k = {k} done! f1 = {k_f1[-1]}')

    print(k_f1)

    import matplotlib.pyplot as plt
    plt.figure()
    plt.plot(k_range,k_f1)
    plt.xlabel('Value of k for KNN')
    plt.ylabel('CrossValidation f1')
    plt.show()

# MLP 时间效率对比
def mlp_time_compare(data, label):
    from models import get_mlp, get_mymlp
    import time
    start = time.clock()
    mymlp = get_mymlp(data.shape[1])
    mymlp.fit(data, label)
    elapsed = (time.clock() - start)
    print("MyMLP Time used:", elapsed)

    start = time.clock()
    mlp = get_mlp()
    mlp.fit(data, label)
    elapsed = (time.clock() - start)
    print("MLP Time used:", elapsed)

