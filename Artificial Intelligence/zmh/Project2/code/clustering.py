import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.distance import cdist


class ClusteringMethods(object):

    def __init__(self, show=True, show_type='bar'):
        self._show = show
        self._show_type = show_type
        self.names = ['KMeans', 'DBSCAN', 'OPTICS', 'SPEC']
        self.Clusterings = {'KMeans': self.kmeans_clustering(),
                            'DBSCAN': self.dbscan_clustering(),
                            'OPTICS': self.optics_clustering(),
                            'SPEC': self.spectral_clustering(),
                            }

    # KMeans clustering
    def kmeans_clustering(self):
        from sklearn.cluster import KMeans
        clt = KMeans(n_clusters=4, random_state=1)
        return clt

    # DBSCAN Clustering
    def dbscan_clustering(self):
        from sklearn.cluster import DBSCAN
        clt = DBSCAN(eps=0.5,
                     min_samples=5,
                     metric='euclidean',
                     metric_params=None,
                     algorithm='auto',
                     leaf_size=30,
                     p=None,
                     n_jobs=1)
        return clt

    # OPTICS Clustering
    def optics_clustering(self):
        from sklearn.cluster import OPTICS
        clt = OPTICS(eps=0.5,
                     min_samples=5,
                     metric='euclidean',
                     metric_params=None,
                     algorithm='auto',
                     leaf_size=30,
                     p=None,
                     n_jobs=1)
        return clt

    # Spectral Clustering
    def spectral_clustering(self):
        from sklearn.cluster import SpectralClustering
        clt = SpectralClustering(n_clusters=4)
        return clt

    def train_all(self, train_data, train_label):
        centroids = []
        for model in self.Clusterings.values():
            centroids.append(model.fit(train_data).labels_)
        scores = self.evaluation(train_data, train_label, centroids)
        if self._show:
            self.show(scores, train_data, centroids)

    def train_k(self, train_data, train_label):
        meandistortions = []
        from sklearn.cluster import KMeans
        for k in [3, 5, 8, 10]:
            kmeans = KMeans(k)
            kmeans.fit(train_data)
            meandistortions.append(sum(np.min(cdist(
                train_data, kmeans.cluster_centers_, "euclidean"), axis=1)) / train_data.shape[0])
        plt.plot([3, 5, 8, 10], meandistortions, 'bx-')
        plt.xlabel('k')
        plt.savefig('fig/eval_k.png')

    def train_feature(self, train_data, train_label):
        meandistortions = []
        from sklearn.cluster import KMeans
        kmeans = KMeans(5)
        for i in range(50):
            index = np.random.randint(0, 22, (20)).tolist()
            kmeans.fit(train_data[index])
            meandistortions.append(sum(np.min(cdist(
                train_data, kmeans.cluster_centers_, "euclidean"), axis=1)) / train_data.shape[0])
        plt.plot(range(50), meandistortions, 'bx-')
        plt.xlabel('k')
        plt.savefig('fig/eval_feature.png')

    def evaluation(self, train_data, train_label, centroids):
        from sklearn.metrics import davies_bouldin_score, calinski_harabasz_score, silhouette_score, completeness_score
        db_scores = []
        ch_scores = []
        silhouette_scores = []
        completeness_scores = []
        for centroid in centroids:
            db_scores.append(davies_bouldin_score(train_data, centroid))
            ch_scores.append(calinski_harabasz_score(train_data, centroid))
            silhouette_scores.append(silhouette_score(train_data, centroid))
            completeness_scores.append(completeness_score(train_label, centroid))
        return {'db_scores': db_scores, 'ch_scores': ch_scores, 'silhouette_scores': silhouette_scores,
                'completeness_scores': completeness_scores}

    def show(self, scores, train_data, centroids):
        if self._show_type == 'print':
            for score_name, score in scores.items():
                for name, s in zip(self.names, score):
                    print('clustering algorithm: ', name)
                    print(score_name + ': ', s)
                    print('=' * 50)
        elif self._show_type == 'bar':
            for score_name, score in scores.items():
                plt.bar(range(len(self.names)), score, color='rgb', tick_label=self.names)
                plt.savefig('fig/clustering_' + score_name + '.png')
                plt.clf()
        else:
            raise NotImplementedError

        from sklearn.manifold import TSNE
        tsne = TSNE(n_components=2, learning_rate=100)
        tsne_data = tsne.fit_transform(train_data)
        for name, centroid in zip(self.names, centroids):
            plt.scatter(tsne_data[:, 0], tsne_data[:, 1], c=centroid)
            plt.savefig('fig/tsne_' + name + '.png')
            plt.clf()
