import matplotlib.pyplot as plt
import seaborn as sns
import data
import pandas
TheData = data.ReadData()
a = TheData.corr()
print(a)
plt.subplots(figsize=(17, 17))
plt.rcParams['axes.unicode_minus']=False
sns.heatmap(a, annot=True, vmax=1, square=True, cmap="Blues")
plt.show()
