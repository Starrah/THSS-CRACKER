import pandas as pd
import numpy as np
from config import *
from sklearn.compose import ColumnTransformer
from sklearn.preprocessing import LabelEncoder, OneHotEncoder, StandardScaler

# 特征选择
def feature_selection(data, select):
    for f in data.columns:
        if f not in select:
            del data[f]
    return data

# 读取数据
# 实验报告中三种特征组合分别为：
# 特征组合1 (all)：data, label, origin_data = get_data('top', 22) 
# 特征组合2 (std top)：data, label, origin_data = get_data('top', FEATURE_STD_TOP) 
# 特征组合3 (forward select)：data, label, origin_data = get_data('select', feature_select=BEST_SELECT) 
def get_data(mode, top_k=None, feature_select=None, csv_path = 'data/Frogs_MFCCs.csv'):
    data = pd.read_csv(csv_path)
    
    for j in range(len(FAMILY)):
            data.loc[:,'Family'] = data.loc[:,'Family'].apply(lambda x:j if x == FAMILY[j] else x)
    label = np.array(data.loc[:,'Family'].tolist())

    for f in ['Family', 'Genus', 'Species', 'RecordID']:
        del data[f]

    origin_data = data.values.astype(float)

    if mode=='select':
        data = feature_selection(data, feature_select)

    data, label = data.values.astype(float), label.astype(float)

    if mode=='top':
        mean, std = np.mean(data, 0), np.std(data, 0)
        top = np.argsort(std)[-top_k:]
        data = data[:,top]

    return data, label, origin_data

if __name__=='__main__':
    data,label = get_data()
    


    
