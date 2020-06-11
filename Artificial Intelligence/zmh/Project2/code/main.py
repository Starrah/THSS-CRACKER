import pandas as pd
import argparse
from sklearn.metrics import classification_report
from classification import ClassifyMethods
from clustering import ClusteringMethods
from algorithms.KNN import KNN, WKNN
from algorithms.KMeans import KMeans
from data_loader import *

parser = argparse.ArgumentParser()
parser.add_argument('--work', type=str, default='test_clustering')
parser.add_argument('--dim', type=list, default=[-6, -5, -4, -3, -2])  # only for classification task

args = parser.parse_args()
classification_dir = '../data/classification/train_set.csv'
clustering_dir = '../data/clustering/Frogs_MFCCs.csv'

if __name__ == "__main__":

    if args.work == 'classification':
        data = pd.read_csv(classification_dir)
        train_x, test_x, train_y, test_y = classification_data_selection(data, dim=args.dim)
        clf = ClassifyMethods()
        clf.train_all(train_x, test_x, train_y, test_y)
        # clf.train_all(train_x, train_x, train_y, train_y)
        # clf.cv_all(train_x, train_y)

    elif args.work == 'clustering':
        data = pd.read_csv(clustering_dir)
        train_x, test_x, train_y, test_y = clustering_data_selection(data)
        clt = ClusteringMethods(show_type='print')
        # label won't be used while training
        clt.train_all(train_x, train_y)
        # clt.train_k(train_x, train_y)
        # clt.train_feature(train_x, train_y)

    elif args.work == 'test_classification':
        data = pd.read_csv(classification_dir)
        train_x, test_x, train_y, test_y = classification_data_selection(data, dim=args.dim)
        clf = WKNN()
        pred_y = clf.predict(train_x, test_x, train_y)
        print(classification_report(y_true=test_y, y_pred=pred_y))
        clf.confusion_matrix(test_y, pred_y)
        # clf.confusion_matrix_cmp(train_x, test_x, train_y, test_y)

    elif args.work == 'test_clustering':
        data = pd.read_csv(clustering_dir)
        train_x, test_x, train_y, test_y = clustering_data_selection(data)
        clt = KMeans()
        centroids = clt.fit(train_x)
        scores = clt.evaluation(train_x, train_y, centroids)
        clt.show(scores, train_x, centroids)
    else:
        raise NotImplementedError
