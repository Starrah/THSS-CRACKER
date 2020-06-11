from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier

from sklearn.pipeline import make_pipeline
from sklearn.model_selection import train_test_split, cross_validate
import sklearn

from imblearn.over_sampling import RandomOverSampler
from imblearn.pipeline import Pipeline

import numpy as np

class MyMLP(object):
    def __init__(self, input_dim, output_dim, hidden_dim=100, batch_size = 512, lr=1e-2, weight_decay=1e-4, max_iter = 50000):
        # 超参
        self.input_dim = input_dim
        self.hidden_dim = hidden_dim
        self.output_dim = output_dim
        self.lr = lr
        self.weight_decay = weight_decay
        self.max_iter = max_iter
        self.batch_size = batch_size

        # 参数
        self.weight1 = np.random.randn(input_dim, hidden_dim)
        self.bias1 = np.random.randn(1,hidden_dim)
        self.weight2 = np.random.randn(hidden_dim, output_dim)
        self.bias2 = np.random.randn(1,output_dim)

        # 动量
        self.beta = 0.9
        self.v_w1 = np.zeros_like(self.weight1)
        self.v_b1 = np.zeros_like(self.bias1)
        self.v_w2 = np.zeros_like(self.weight2)
        self.v_b2 = np.zeros_like(self.bias2)

        self._estimator_type = 'classifier'

    def get_params(self, deep = False):
        return {'lr':self.lr,'weight_decay':self.weight_decay,'max_iter':self.max_iter,'batch_size':self.batch_size,
            'input_dim':self.input_dim, 'output_dim':self.output_dim, 'hidden_dim':self.hidden_dim}
    
    def fit(self, X, Y):
        X, Y = sklearn.utils.shuffle(X, Y)
        Y = Y.reshape((Y.shape[0],1))
        lr = self.lr
        start=0

        for t in range(self.max_iter):
            # minibatch
            end = min(start+self.batch_size,X.shape[0])
            x = X[start:end]
            y = Y[start:end]
            
            # 前向传播
            m = x.shape[0]
            out1 = np.matmul(x,self.weight1)+self.bias1
            act1 = np.maximum(out1,0) # relu
            out2 = np.matmul(act1,self.weight2)+self.bias2
            act2 = 1/(1+np.exp(-out2)) # sigmoid
            loss = -np.mean(y*np.log(act2+1e-8)+(1-y)*np.log(1-act2+1e-8))
            
            # if t%500==0:
            #     print(f"iter {t}: loss = {loss}")

            # 反向传播
            grad_out2 = -y*(1-act2)+(1-y)*(act2)
            grad_weight2 = np.matmul(act1.T, grad_out2) / m
            grad_bias2 = np.sum(grad_out2) / m
            grad_act1 = np.matmul(grad_out2, self.weight2.T)
            grad_out1 = grad_act1.copy()
            grad_out1[out1<0] = 0
            grad_weight1 = np.matmul(x.T, grad_out1) / m
            grad_bias1 = np.sum(grad_out1) / m

            # 带动量的梯度下降
            ratio = 1-self.lr*self.weight_decay/m
            self.v_w1 = self.beta*self.v_w1 + (1-self.beta)*grad_weight1
            self.v_b1 = self.beta*self.v_b1 + (1-self.beta)*grad_bias1
            self.v_w2 = self.beta*self.v_w2 + (1-self.beta)*grad_weight2
            self.v_b2 = self.beta*self.v_b2 + (1-self.beta)*grad_bias2

            self.weight1 = self.weight1*ratio - lr * self.v_w1
            self.bias1 = self.bias1*ratio - lr * self.v_b1
            self.weight2 = self.weight2*ratio - lr * self.v_w2
            self.bias2 = self.bias2*ratio - lr * self.v_b2

            # learning rate decay
            lr = self.lr * (1 - t/self.max_iter * 0.9)
            start += self.batch_size
            if start>=X.shape[0]:
                start = 0

    def predict(self, x):
        proba = self.predict_proba(x)
        return np.argmax(proba,axis=-1).astype(float)

    def predict_proba(self, x):
        out1 = np.matmul(x,self.weight1)+self.bias1
        act1 = np.maximum(out1,0) # relu
        out2 = np.matmul(act1,self.weight2)+self.bias2
        act2 = 1/(1+np.exp(-out2)) # sigmoid
        act2 = np.squeeze(act2)
        return np.vstack([1 - act2, act2]).T
    


def get_mymlp(input_dim):
    ros = RandomOverSampler(random_state=0)
    mymlp = MyMLP(input_dim, 1)
    pipeline = Pipeline([('ros',ros), ('mymlp',mymlp)])
    return pipeline


def get_svc():
    ros = RandomOverSampler(random_state=0)
    svc = SVC(kernel='rbf', gamma='auto', C=1, random_state=0)
    pipeline = Pipeline([('ros',ros), ('svc',svc)])
    return pipeline

def get_mlp():
    ros = RandomOverSampler(random_state=0)
    mlp = MLPClassifier(random_state=0, max_iter=500, learning_rate_init=0.001)
    pipeline = Pipeline([('ros',ros), ('mlp',mlp)])
    return pipeline

def get_knn(k=5):
    ros = RandomOverSampler(random_state=0)
    knn=KNeighborsClassifier(n_neighbors=k)
    pipeline = Pipeline([('ros',ros), ('knn',knn)])
    return pipeline

def get_rfc():
    ros = RandomOverSampler(random_state=0)
    rfc = RandomForestClassifier(random_state=0)
    pipeline = Pipeline([('ros',ros), ('rfc',rfc)])
    return pipeline

def get_dtc():
    ros = RandomOverSampler(random_state=0)
    rfc = DecisionTreeClassifier(random_state=0)
    pipeline = Pipeline([('ros',ros), ('rfc',rfc)])
    return pipeline
