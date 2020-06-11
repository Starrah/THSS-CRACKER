import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans as KM


class KMeans(object):
    def __init__(self, k=3, tolerance=0.0001, max_iterations=500, show_type='bar'):
        self.k = k
        self.tolerance = tolerance
        self.max_iterations = max_iterations
        self.centroids = dict()
        self._show_type = show_type
        self.names = ['my kmeans', 'sklearn kmeans']

    def fit(self, data):
        data = np.array(data)
        labels_ = np.zeros(len(data))
        for i in range(self.k):
            self.centroids[i] = data[i]
        for i in range(self.max_iterations):
            self.classes = {}
            for j in range(self.k):
                self.classes[j] = []
            # find the distance between the point and cluster; choose the nearest centroid
            for k, features in enumerate(data):
                distances = [np.linalg.norm(features - self.centroids[centroid]) for centroid in self.centroids]
                classification = distances.index(min(distances))
                self.classes[classification].append(features)
                labels_[k] = classification

            previous = dict(self.centroids)
            flag = True
            # average the cluster data points to re-calculate the centroids
            for classification in self.classes:
                self.centroids[classification] = np.average(self.classes[classification], axis=0)
            for centroid in self.centroids:
                original_centroid = previous[centroid]
                curr = self.centroids[centroid]

                if np.sum((curr - original_centroid) / (original_centroid + 1e-8) * 100.0) > self.tolerance:
                    flag = False
            if flag:
                break
        return labels_.astype(int)

    def evaluation(self, train_data, train_label, centroids, cmp=False):
        from sklearn.metrics import davies_bouldin_score, calinski_harabasz_score, silhouette_score, completeness_score

        db_scores = [davies_bouldin_score(train_data, centroids)]
        ch_scores = [calinski_harabasz_score(train_data, centroids)]
        silhouette_scores = [silhouette_score(train_data, centroids)]
        completeness_scores = [completeness_score(train_label, centroids)]

        if cmp:
            km = KM(n_clusters=4, random_state=1)
            km_centroids = km.fit(train_data).labels_
            db_scores.append(davies_bouldin_score(train_data, km_centroids))
            ch_scores.append(calinski_harabasz_score(train_data, km_centroids))
            silhouette_scores.append(silhouette_score(train_data, km_centroids))
            completeness_scores.append(completeness_score(train_label, km_centroids))

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
                plt.savefig('fig/cmp_clustering_' + score_name + '.png')
                plt.clf()

        km = KM(n_clusters=4, random_state=1)
        km_centroid = km.fit(train_data).labels_
        from sklearn.manifold import TSNE
        tsne = TSNE(n_components=2, learning_rate=100)
        tsne_data = tsne.fit_transform(train_data)
        plt.scatter(tsne_data[:, 0], tsne_data[:, 1], c=centroids)
        plt.savefig('fig/tsne.png')
        plt.clf()
        plt.scatter(tsne_data[:, 0], tsne_data[:, 1], c=km_centroid)
        plt.savefig('fig/tsne_cmp.png')
        plt.clf()


if __name__ == '__main__':
    kmeans = KMeans()
    train_x = [[1., 3., 4., 5., 9., ], [3., 7., 5., 5., 9., ], [0., 8., 6., 5., 9., ],
               [2., 3., 4., 6., 6., ], [0., 10., 3., 4., 1., ], [4., 2., 2., 6., 3., ], ]
    labels = kmeans.fit(train_x)
    print(labels)
