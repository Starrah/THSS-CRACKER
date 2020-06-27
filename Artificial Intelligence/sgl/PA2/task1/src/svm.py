'''
调用sklearn的SVM库
'''
from sklearn.model_selection import KFold
from sklearn import svm
import numpy as np
import data
import constants


def SVMTrain(data, FeatureList):
    '''
    描述：调用sklearn的贝叶斯库训练模型
    参数：数据，特征
    返回：无
    '''
    #实例化分类器
    model = svm.SVC()
    kf = KFold(n_splits = constants.KFolds ,shuffle = True)
    Return = {}
    Return["TP"] = 0
    Return["TN"] = 0
    Return["FP"] = 0
    Return["FN"] = 0
    for TrainLabels, TestLabels in kf.split(data):
        TrainX = data[FeatureList].values[TrainLabels]
        TrainY = data["y"][TrainLabels]
        TestX = data[FeatureList].values[TestLabels]
        TestY = data["y"][TestLabels]  
        model.fit(TrainX, TrainY)  
        PredY = model.predict(TestX)
        
        TPNum = 0
        TNNum = 0
        FPNum = 0
        FNNum = 0
        TestY = np.array(TestY)
        TestY = TestY.reshape(-1)
        for i in range(len(TestY)):
            if PredY[i] == 1 and TestY[i] == 1:
                TPNum += 1
            elif PredY[i] == 0 and TestY[i] == 0:
                TNNum += 1       
            elif PredY[i] == 1 and TestY[i] == 0:
                FPNum += 1
            elif PredY[i] == 0 and TestY[i] == 1:
                FNNum += 1
        Return["TP"] += TPNum
        Return["TN"] += TNNum
        Return["FP"] += FPNum
        Return["FN"] += FNNum
    return Return
    