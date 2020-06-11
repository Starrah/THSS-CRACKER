import numpy as np
import operator
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt


def euclidean_distance(vector1, vector2):
    return np.sqrt(np.sum(np.power(vector1 - vector2, 2)))


def absolute_distance(vector1, vector2):
    return np.sum(np.absolute(vector1 - vector2))


class KNN(object):
    def __init__(self, k=5, dist_type='l2'):
        self.k = k
        if dist_type == 'l2':
            self.distance = euclidean_distance
        elif dist_type == 'l1':
            self.distance = absolute_distance
        else:
            raise NotImplementedError

    def neighbours(self, train_x, X_test_instance, k):
        distances = []
        neighbors = []
        for i in range(0, train_x.shape[0]):
            dist = self.distance(train_x[i], X_test_instance)
            distances.append((i, dist))
        distances.sort(key=operator.itemgetter(1))
        for x in range(k):
            neighbors.append(distances[x][0])
        return neighbors

    def _predict(self, output, train_y, weights=None):
        class_votes = {}
        for i in range(len(output)):
            if train_y[output[i]] in class_votes:
                class_votes[train_y[output[i]]] += 1
            else:
                class_votes[train_y[output[i]]] = 1
        sorted_votes = sorted(class_votes.items(), key=operator.itemgetter(1), reverse=True)
        return sorted_votes[0][0]

    def predict(self, train_x, test_x, train_y):
        output_classes = []
        for i in range(0, test_x.shape[0]):
            output = self.neighbours(train_x, test_x[i], self.k)
            p_class = self._predict(output, train_y)
            output_classes.append(p_class)
        return output_classes

    def confusion_matrix(self, test_y, pred_y, cmp=False):
        cm = confusion_matrix(test_y, pred_y, labels=range(2))
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        plt.imshow(cm, interpolation='nearest')
        plt.title('confusion matrix')
        plt.colorbar()
        labels_name = ['no', 'yes']
        num_local = np.array(range(2))
        plt.xticks(num_local, labels_name, rotation=90)
        plt.yticks(num_local, labels_name)
        plt.ylabel('True label')
        plt.xlabel('Predicted label')
        if cmp:
            plt.savefig('fig/confusion_matrix_knn.png')
        else:
            plt.savefig('fig/confusion_matrix.png')

    def confusion_matrix_cmp(self, train_x, test_x, train_y, test_y):
        from sklearn.neighbors import KNeighborsClassifier
        knn = KNeighborsClassifier(n_neighbors=5)
        knn.fit(train_x, train_y)
        pred_y = knn.predict(test_x)
        self.confusion_matrix(test_y, pred_y, cmp=True)


class WKNN(KNN):
    def __init__(self, k=5, dist_type='l2'):
        super(WKNN, self).__init__(k=k, dist_type=dist_type)

    def neighbours(self, train_x, X_test_instance, k):
        distances = []
        neighbors = []
        weights = []
        for i in range(0, train_x.shape[0]):
            dist = self.distance(train_x[i], X_test_instance)
            distances.append((i, dist))
        distances.sort(key=operator.itemgetter(1))
        for x in range(k):
            neighbors.append(distances[x][0])
            weights.append(1 / distances[x][1] + 1e-3)
        weights = np.array(weights)
        weights = (weights / weights.sum()) * self.k * 0.1
        return neighbors, weights

    def _predict(self, output, train_y, weights=None):
        class_votes = {}
        for i in range(len(output)):
            if train_y[output[i]] in class_votes:
                class_votes[train_y[output[i]]] += weights[i]
            else:
                class_votes[train_y[output[i]]] = weights[i]
        sorted_votes = sorted(class_votes.items(), key=operator.itemgetter(1), reverse=True)
        return sorted_votes[0][0]

    def predict(self, train_x, test_x, train_y):
        output_classes = []
        for i in range(0, test_x.shape[0]):
            output, weights = self.neighbours(train_x, test_x[i], self.k)
            p_class = self._predict(output, train_y, weights)
            output_classes.append(p_class)
        return output_classes

    def confusion_matrix(self, test_y, pred_y, cmp=False):
        cm = confusion_matrix(test_y, pred_y, labels=range(2))
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        plt.imshow(cm, interpolation='nearest')
        plt.title('confusion matrix')
        plt.colorbar()
        labels_name = ['no', 'yes']
        num_local = np.array(range(2))
        plt.xticks(num_local, labels_name, rotation=90)
        plt.yticks(num_local, labels_name)
        plt.ylabel('True label')
        plt.xlabel('Predicted label')
        if cmp:
            plt.savefig('fig/confusion_matrix_knn.png')
        else:
            plt.savefig('fig/confusion_matrix_w.png')
