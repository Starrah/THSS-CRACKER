from sklearn.model_selection import cross_val_score, cross_validate
from sklearn.metrics import classification_report


class ClassifyMethods(object):

    def __init__(self, k_fold_num=5, show=True, show_type='print'):
        self._show = show
        self._show_type = show_type
        self.k_fold_num = k_fold_num
        self.names = ['KNN', 'LR', 'RF', 'DT', 'SVM', 'GBDT']
        self.classifiers = {'KNN': self.knn_classifier(),
                            'LR': self.logistic_regression_classifier(),
                            'RF': self.random_forest_classifier(),
                            'DT': self.decision_tree_classifier(),
                            'SVM': self.svm_classifier(),
                            'GBDT': self.gradient_boosting_classifier()
                            }

    # KNN Classifier
    def knn_classifier(self):
        from sklearn.neighbors import KNeighborsClassifier
        clf = KNeighborsClassifier(n_neighbors=5)
        return clf

    # Logistic Regression Classifier
    def logistic_regression_classifier(self):
        from sklearn.linear_model import LogisticRegression
        clf = LogisticRegression(penalty='l2', max_iter=1000)
        return clf

    # Random Forest Classifier
    def random_forest_classifier(self):
        from sklearn.ensemble import RandomForestClassifier
        clf = RandomForestClassifier(n_estimators=8)
        return clf

    # Decision Tree Classifier
    def decision_tree_classifier(self):
        from sklearn import tree
        clf = tree.DecisionTreeClassifier()
        return clf

    # GBDT(Gradient Boosting Decision Tree) Classifier
    def gradient_boosting_classifier(self):
        from sklearn.ensemble import GradientBoostingClassifier
        clf = GradientBoostingClassifier(n_estimators=200)
        return clf

    # SVM Classifier
    def svm_classifier(self):
        from sklearn.svm import SVC
        clf = SVC(kernel='rbf', probability=True)
        return clf

    def train_all(self, train_x, test_x, train_y, test_y):
        scores = []
        for name, model in self.classifiers.items():
            scores.append(cross_val_score(model, train_x, train_y, cv=self.k_fold_num, scoring='accuracy').mean())
            print('classification algorithm: ', name)
            model.fit(train_x, train_y)
            pred_y = model.predict(test_x)
            print(classification_report(y_true=test_y, y_pred=pred_y))
        if self._show:
            self.show(scores)

    def cv_all(self, train_x, train_y):
        scoring = {'accuracy': 'accuracy',
                   'precision': 'precision',
                   'recall': 'recall',
                   'f1': 'f1',
                   'roc_auc': 'roc_auc'}
        for name, model in self.classifiers.items():
            print('classification algorithm: ', name)
            print(cross_validate(model, train_x, train_y, cv=self.k_fold_num, scoring=scoring))

    def show(self, scores):
        if self._show_type == 'print':
            for name, score in zip(self.names, scores):
                print('classification algorithm: ', name)
                print('accuracy: {:.3f}'.format(score))
                print('=' * 50)
        elif self._show_type == 'bar':
            import matplotlib.pyplot as plt
            plt.bar(range(len(self.names)), scores, color='rgb', tick_label=self.names)
            plt.savefig('fig/classification.png')
        else:
            raise NotImplementedError

