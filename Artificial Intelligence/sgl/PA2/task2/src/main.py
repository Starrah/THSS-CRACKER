import constants
import data
import tsne
import kmeans
import self_model
import estimate
import hierarchy

AllFeatureChoices = [["MFCCs_ 4", "MFCCs_11", "MFCCs_19"], ["MFCCs_ 4", "MFCCs_13", "MFCCs_19"], \
    ["MFCCs_ 6", "MFCCs_11", "MFCCs_19"], ["MFCCs_ 6", "MFCCs_13", "MFCCs_19"]]

def RunKmeansTrain(TheData, TSNEResult):
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Purity = 0
        SSE = 0
        BSS = 0
        for j in range(constants.TestRound):
            Result = kmeans.RunKMeans(FeatureList, TheData)
            ThePurity = estimate.GetPurity(Result)
            TheSSE, TheBSS = estimate.GetSSEAndBSS(Result)
            Purity += ThePurity
            SSE += TheSSE
            BSS += TheBSS
        Purity = Purity / constants.TestRound
        SSE = SSE / constants.TestRound
        BSS = BSS / constants.TestRound
        print("Feature", i)
        print("Purity: {:.2f}".format(Purity))
        print("SSE: {:.2f}".format(SSE))
        print("BSS: {:.2f}".format(BSS))
        print("*" * 100)
        tsne.PlotTsnePredict(Result, TSNEResult, i, "KMeans")

def RunSelfTrain(TheData, TSNEResult):
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Purity = 0
        SSE = 0
        BSS = 0
        for j in range(constants.TestRound):
            Result = self_model.RunSelf(FeatureList, TheData)
            ThePurity = estimate.GetPurity(Result)
            TheSSE, TheBSS = estimate.GetSSEAndBSS(Result)
            Purity += ThePurity
            SSE += TheSSE
            BSS += TheBSS
        Purity = Purity / constants.TestRound
        SSE = SSE / constants.TestRound
        BSS = BSS / constants.TestRound
        print("Feature", i)
        print("Purity: {:.2f}".format(Purity))
        print("SSE: {:.2f}".format(SSE))
        print("BSS: {:.2f}".format(BSS))
        print("*" * 100)
        tsne.PlotTsnePredict(Result, TSNEResult, i, "Self")
        
def RunHierTrain(TheData, TSNEResult):
    for i in range(len(AllFeatureChoices)):
        FeatureList = AllFeatureChoices[i]
        Purity = 0
        SSE = 0
        BSS = 0
        for j in range(constants.TestRound):
            Result = hierarchy.RunHierarchy(FeatureList, TheData)
            ThePurity = estimate.GetPurity(Result)
            TheSSE, TheBSS = estimate.GetSSEAndBSS(Result)
            Purity += ThePurity
            SSE += TheSSE
            BSS += TheBSS
        Purity = Purity / constants.TestRound
        SSE = SSE / constants.TestRound
        BSS = BSS / constants.TestRound
        print("Feature", i)
        print("Purity: {:.2f}".format(Purity))
        print("SSE: {:.2f}".format(SSE))
        print("BSS: {:.2f}".format(BSS))
        print("*" * 100)
        tsne.PlotTsnePredict(Result, TSNEResult, i, "Hier")

def main():
    Result = []
    FeatureList = []
    TheData = data.ReadData()
    TSNEResult = tsne.GetTsneResult(TheData)
    
    print("please input the type you want")
    print("1:K means")
    print("2:K means implemented by myself(slow)")
    print("3:Hierarchical")
    num = -1
    while True:
        try:
            num = int(input())
        except:
            continue
        if num == 1:
            RunKmeansTrain(TheData, TSNEResult)
            break
        elif num == 2:
            RunSelfTrain(TheData, TSNEResult)
            break
        elif num == 3:
            RunHierTrain(TheData, TSNEResult)
            break   
    tsne.PlotTsneGroundTruth(TheData, TSNEResult, num)

main()