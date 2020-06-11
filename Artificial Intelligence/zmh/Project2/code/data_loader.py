from data_mapping import *
import numpy as np
from sklearn.model_selection import train_test_split


def normalization(data):
    _range = np.max(abs(data))
    return data / _range


def classification_data_preprocess(data, dim):
    for d in range(data.shape[1]):
        if data.keys()[d] + '_mapping' in mapping_keys:
            data[data.keys()[d]] = data[data.keys()[d]].map(mapping_mapping[data.keys()[d] + '_mapping'])
        else:
            data[data.keys()[d]] = data[data.keys()[d]].astype(float)
    return data


def clustering_data_selection(data):
    data['Family'] = data['Family'].map(family_mapping)
    x, y = data.values[:, :22], data.values[:, -4]
    train_x, test_x, train_y, test_y = train_test_split(x, y, test_size=0.2)
    return train_x, test_x, train_y, test_y


def classification_data_selection(data, dim):
    assert isinstance(dim, list)
    data = classification_data_preprocess(data, dim)
    x, y = data.values[:, dim], data.values[:, -1]
    train_x, test_x, train_y, test_y = train_test_split(x, y, test_size=0.2)
    train_x = normalization(train_x)
    test_x = normalization(test_x)
    return train_x, test_x, train_y, test_y
