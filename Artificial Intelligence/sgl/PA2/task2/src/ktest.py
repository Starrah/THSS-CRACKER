'''
测试k值
'''

import constants
import data
import tsne
import kmeans
import self_model
import estimate
import hierarchy

TheData = data.ReadData()
KNumList = [2, 3, 4, 5, 6, 7, 8]
FeatureList = ["MFCCs_ 6", "MFCCs_11", "MFCCs_19"]

print("Test K Values")
for item in KNumList:
    constants.KClusters = item
    Purity = 0
    SSE = 0
    BSS = 0
    for i in range(constants.TestRound):
        Result = self_model.RunSelf(FeatureList, TheData)
        ThePurity = estimate.GetPurity(Result)
        TheSSE, TheBSS = estimate.GetSSEAndBSS(Result)
        Purity += ThePurity
        SSE += TheSSE
        BSS += TheBSS
    Purity = Purity / constants.TestRound
    SSE = SSE / constants.TestRound
    BSS = BSS / constants.TestRound
    print("KNum:", item)
    print("Purity: {:.2f}".format(Purity))
    print("SSE: {:.2f}".format(SSE))
    print("BSS: {:.2f}".format(BSS))


