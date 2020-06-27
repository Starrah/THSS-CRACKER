import matplotlib.pyplot as plt
import seaborn as sns
import data
import pandas


def PaintCorrelation():
    TheData = data.CleanData(data.ReadData()).astype(float)
    TheData = TheData[["MFCCs_ 4", "MFCCs_ 6", "MFCCs_11", "MFCCs_13", "MFCCs_15", "MFCCs_17", "MFCCs_19", "MFCCs_20", "MFCCs_22", "Family"]]
    a = TheData.corr()
    print(a)
    plt.subplots(figsize=(17, 17))
    plt.rcParams['axes.unicode_minus'] = False
    sns.heatmap(a, annot=True, vmax=1, square=True, cmap="Blues")
    plt.show()
PaintCorrelation()