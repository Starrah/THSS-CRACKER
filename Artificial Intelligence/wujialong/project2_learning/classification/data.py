import pandas as pd
import numpy as np
from sklearn.compose import ColumnTransformer
from sklearn.preprocessing import LabelEncoder, OneHotEncoder, StandardScaler

# 特征选择，剔除不在被选择中的特征
def feature_selection(data, select):
    for f in data.columns:
        if f not in select:
            del data[f]
    return data

# 特征工程，将非数值型特征转换为数值型
def feature_preprocessing(data):
    # education: ordered
    if 'education' in data.columns:
        dictUniq = {'unknown':0, 'primary':-1, 'secondary':0, 'tertiary':1}
        data.loc[:,'education'] = data.loc[:,'education'].apply(lambda x:dictUniq[x])

    # default, housing, loan: 01 code
    features = ['default', 'housing', 'loan']
    dictUniq = {'no':0, 'yes':1}
    for f in features:
        if f in data.columns:
            data.loc[:,f] = data.loc[:,f].apply(lambda x:dictUniq[x])

    # month
    if 'month' in data.columns:
        listUniq = ['jan', 'feb', 'mar', 'apr', 'may', 'jun', 'jul', 'aug', 'sep', 'oct', 'nov', 'dec']
        for j in range(len(listUniq)):
            data.loc[:,'month'] = data.loc[:,'month'].apply(lambda x:j if x == listUniq[j] else x)

    # pdays
    if 'pdays' in data.columns:
        data.loc[:,'pdays'] = data.loc[:,'pdays'].apply(lambda x:999 if x == -1 else x)

    # poutcome
    if 'poutcome' in data.columns:
        dictUniq = {'unknown':0, 'failure':-1, 'other':0, 'success':1}
        data.loc[:,'poutcome'] = data.loc[:,'poutcome'].apply(lambda x:dictUniq[x])

    columns = list(data.columns)
    data = data.values

    #  one hot encoder
    enc_index = []
    features = ['job', 'marital', 'contact']
    enc_index = [columns.index(f) for f in features if f in columns ]
    # print(enc_index)

    # scaler
    scale_index = []
    features = ['age', 'balance', 'day', 'duration', 'campaign', 'pdays', 'previous', 'month']
    scale_index = [columns.index(f) for f in features if f in columns]
    # print(scale_index)

    ct = ColumnTransformer(
        [
            ('oh_enc', OneHotEncoder(sparse=False), enc_index),
            ('scaler', StandardScaler(),scale_index) 
        ],
        remainder='passthrough' 
    )
    data = ct.fit_transform(data)
    
    return data

# 读取数据并预处理
def get_data(feature_select, csv_path = 'train_set.csv'):
    data = pd.read_csv(csv_path)
    label = np.array(data.loc[:,'y'].tolist())
    data = feature_selection(data, feature_select)
    data = feature_preprocessing(data)
    
    return data.astype(float), label.astype(float)

if __name__=='__main__':
    data, label = get_data()


    
