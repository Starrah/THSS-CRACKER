'''
主函数，负责选择feature调用分类器
'''
import data
import bayes
import svm
import self_model
import mlp


AllFeatureChoices = [["balance", "duration", "pdays"],
               ["balance", "duration", "previous"],
               ["balance", "duration", "poutcome"],
               ["duration", "pdays"],
               ["duration", "previous"],
               ["duration", "poutcome"]]

def RunBayesTrain():
    '''
    用bayes库训练6个feature
    '''
    TheData = data.ReadData()
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Result = bayes.BayesTrain(TheData, FeatureList)
        Accuracy = (Result["TP"] + Result["TN"]) \
        / (Result["TP"] + Result["FP"] + Result["TN"] + Result["FN"])
        Precision = (Result["TP"]) \
        / (Result["TP"] + Result["FP"])
        Recall = (Result["TP"]) \
        / (Result["TP"] + Result["FN"])
        F1 = (2 * Precision * Recall) \
        / (Precision + Recall)
        print("Accuracy: {:.2f}".format(100 * Accuracy))
        print("Precision: {:.2f}".format(100 * Precision))
        print("Recall: {:.2f}".format(100 * Recall))
        print("F1: {:.2f}".format(100 * F1))
        print("*" * 100)
        
def RunSelfBayesTrain():
    '''
    用自带bayes库训练6个feature
    '''
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        TheTrainX, TheTrainY, TheTestX, TheTestY = data.ReadDataSelf(FeatureList)
        Result = self_model.BayesTrain(TheTrainX, TheTrainY, TheTestX, TheTestY)
        Accuracy = (Result["TP"] + Result["TN"]) \
        / (Result["TP"] + Result["FP"] + Result["TN"] + Result["FN"])
        Precision = (Result["TP"]) \
        / (Result["TP"] + Result["FP"])
        Recall = (Result["TP"]) \
        / (Result["TP"] + Result["FN"])
        F1 = (2 * Precision * Recall) \
        / (Precision + Recall)
        print("Accuracy: {:.2f}".format(100 * Accuracy))
        print("Precision: {:.2f}".format(100 * Precision))
        print("Recall: {:.2f}".format(100 * Recall))
        print("F1: {:.2f}".format(100 * F1))
        print("*" * 100)

def RunSVMTrain():
    '''
    用SVM库训练6个feature
    '''
    TheData = data.ReadData()
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Result = svm.SVMTrain(TheData, FeatureList)
        Accuracy = (Result["TP"] + Result["TN"]) \
        / (Result["TP"] + Result["FP"] + Result["TN"] + Result["FN"])
        Precision = (Result["TP"]) \
        / (Result["TP"] + Result["FP"])
        Recall = (Result["TP"]) \
        / (Result["TP"] + Result["FN"])
        F1 = (2 * Precision * Recall) \
        / (Precision + Recall)
        print("Accuracy: {:.2f}".format(100 * Accuracy))
        print("Precision: {:.2f}".format(100 * Precision))
        print("Recall: {:.2f}".format(100 * Recall))
        print("F1: {:.2f}".format(100 * F1))
        print("*" * 100)

def RunNNTrain():
    '''
    用神经网络训练6个feature
    '''
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Result = mlp.TrainNN(FeatureList)
        Accuracy = (Result["TP"] + Result["TN"]) \
        / (Result["TP"] + Result["FP"] + Result["TN"] + Result["FN"])
        Precision = (Result["TP"]) \
        / (Result["TP"] + Result["FP"])
        Recall = (Result["TP"]) \
        / (Result["TP"] + Result["FN"])
        F1 = (2 * Precision * Recall) \
        / (Precision + Recall)
        print("Accuracy: {:.2f}".format(100 * Accuracy))
        print("Precision: {:.2f}".format(100 * Precision))
        print("Recall: {:.2f}".format(100 * Recall))
        print("F1: {:.2f}".format(100 * F1))
        print("*" * 100)

def main():

    Result = []
    FeatureList = []
    
    print("please input the type you want")
    print("1:Bayes")
    print("2:Bayes implemented by myself(slow)")
    print("3:SVM")
    print("4:NN")
    while True:
        try:
            num = int(input())
        except:
            continue
        if num == 1:
            RunBayesTrain()
            break
        elif num == 2:
            RunSelfBayesTrain()
            break
        elif num == 3:
            RunSVMTrain()
            break   
        elif num == 4:
            RunNNTrain()
            break
        
main()

