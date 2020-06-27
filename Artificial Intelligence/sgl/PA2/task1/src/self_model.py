import constants
import data
def GetConditionalProb(x, y, feature, result):
    '''
    描述：计算条件概率P(x_i|y)
    参数：训练数据集x，y，被求的feature(x_i)，result(y)
    返回：条件概率
    '''
    Total = 0
    Right = 0 
    for i in range(len(x)):
        if y[i] == result:
            Total = Total + 1
        if y[i] == result and x[i] == feature:
            Right = Right + 1
    
    return Right / Total

def GetTotalProb(y, result):
    '''
    描述：计算概率P(y)
    参数：训练数据集y，result(y)
    返回：条件概率
    '''
    Total = len(y)
    Right = 0
    for i in range(len(y)):
        if y[i] == result:
            Right = Right + 1
    return Right / Total

def SelfBayes(TrainX, TrainY, TestX, TestY):
    '''
    描述：自己实现的naive bayes分类器
    参数：训练x，训练y，测试x，测试y
    返回：四种情况
    '''
    TPNum = 0
    TNNum = 0
    FPNum = 0
    FNNum = 0
    TotalProbTrue = GetTotalProb(TrainY, 1)
    TotalProbFalse = GetTotalProb(TrainY, 0)
    for i in range(len(TestY)):
        ProbTrue = TotalProbTrue
        ProbFalse = TotalProbFalse
        for key in TestX.keys():
            PartX = TrainX[key]
            PartY = TrainY
            feature = TestX[key][i]
            FeatureTrue = GetConditionalProb(PartX, PartY, feature, 1)
            FeatureFalse = GetConditionalProb(PartX, PartY, feature, 0)
            ProbTrue = ProbTrue * FeatureTrue
            ProbFalse = ProbFalse * FeatureFalse
        if ProbTrue >= ProbFalse:
            Predict = 1
        else:
            Predict = 0
            
        
        if Predict == TestY[i] and Predict == 1:
            TPNum += 1
        elif Predict == TestY[i] and Predict == 0:
            TNNum += 1
        elif Predict != TestY[i] and Predict == 1:
            FPNum += 1
        elif Predict != TestY[i] and Predict == 0:
            FNNum += 1
         
    return TPNum, TNNum, FPNum, FNNum

def BayesTrain(TrainX, TrainY, TestX, TestY):
    '''
    描述：主函数
    参数：训练x，训练y，测试x，测试y（k组）
    返回：准确率
    '''
    Return = {}
    Return["TP"] = 0
    Return["TN"] = 0
    Return["FP"] = 0
    Return["FN"] = 0
    for i in range(len(TrainX)):
        TPNum, TNNum, FPNum, FNNum = SelfBayes(TrainX[i], TrainY[i], TestX[i], TestY[i])
        Return["TP"] += TPNum
        Return["TN"] += TNNum
        Return["FP"] += FPNum
        Return["FN"] += FNNum
    return Return
            