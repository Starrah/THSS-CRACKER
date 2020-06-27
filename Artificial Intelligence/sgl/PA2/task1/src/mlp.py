'''
主函数---训练模型
'''

import torch
import torch.nn as nn
import torch.optim as optim
import constants
import data
import os
import time
import math
import mlp_model
import numpy as np

## Note that: here we provide a basic solution for training and validation.
## You can directly change it if you find something wrong or not good enough.

def train(model, train_loader, optimizer, criterion, device):
    '''
    训练函数
    '''
    model.train(True)
    total_loss = 0.0
    total_correct = 0
    for inputs, labels in train_loader:
        labels = labels.squeeze().long()
        inputs = inputs.to(device)
        labels = labels.to(device)
        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        _, predictions = torch.max(outputs, 1)
        loss.backward()
        optimizer.step()
        total_loss += loss.item() * inputs.size(0)
        total_correct += torch.sum(predictions == labels.data)

    epoch_loss = total_loss / len(train_loader.dataset)
    epoch_acc = total_correct.double() / len(train_loader.dataset)
    return epoch_loss, epoch_acc.item()

def valid(model, valid_loader, criterion, device):
    '''
    验证函数
    '''
    model.train(False)
    total_loss = 0.0
    total_correct = 0
    TPNum = 0
    TNNum = 0
    FPNum = 0
    FNNum = 0
    for inputs, labels in valid_loader:
        labels = labels.squeeze().long()
        inputs = inputs.to(device)
        labels = labels.to(device)
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        _, predictions = torch.max(outputs, 1)
        total_loss += loss.item() * inputs.size(0)
        
        for i in range(len(predictions)):
            if predictions[i] == labels.data[i] and predictions[i] == 1:
                    TPNum += 1
            elif predictions[i] == labels.data[i] and predictions[i] == 0:
                    TNNum += 1
            elif predictions[i] != labels.data[i] and predictions[i] == 1:
                    FPNum += 1
            elif predictions[i] != labels.data[i] and predictions[i] == 0:
                    FNNum += 1
        total_correct += torch.sum(predictions == labels.data)
    TheResult = {}
    TheResult["TP"] = TPNum
    TheResult["TN"] = TNNum
    TheResult["FP"] = FPNum
    TheResult["FN"] = FNNum
    
    epoch_loss = total_loss / len(valid_loader.dataset)
    epoch_acc = total_correct.double() / len(valid_loader.dataset)
    return epoch_loss, epoch_acc.item(), TheResult
    

def TrainMain(model, train_loader, valid_loader, criterion, optimizer, device):
    '''
    训练的主函数：循环若干epoch
    '''
    acc_list = []
    best_acc = 0.0
    BestResult = {}
    for epoch in range(constants.NNEpochs):
        print('epoch:{:d}/{:d}'.format(epoch + 1, constants.NNEpochs))
        print('*' * 100)
                
        train_start = time.time()
        train_loss, train_acc = train(model, train_loader, optimizer, criterion, device)
        train_end = time.time()
        
        print("training: {:.4f}, {:.4f}".format(train_loss, train_acc))
        
        valid_start = time.time()
        valid_loss, valid_acc, TheResult = valid(model, valid_loader, criterion, device)
        valid_end = time.time()

        print("validation: {:.4f}, {:.4f}".format(valid_loss, valid_acc))
        print("train time: {:.4f} s".format(train_end - train_start))
        print("valid time: {:.4f} s".format(valid_end - valid_start))

        acc_list.append({"train": train_acc, "valid": valid_acc})
        if valid_acc > best_acc:
            best_acc = valid_acc
            BestResult = TheResult
    return best_acc, BestResult

def TrainNN(FeatureList):
    '''
    训练神经网络
    '''

    ## data preparation
    TheData = data.ReadNNData(FeatureList)
    best_accs = []
    Return = {}
    Return["TP"] = 0
    Return["TN"] = 0
    Return["FP"] = 0
    Return["FN"] = 0
    for i in range(constants.KFolds):
        ## model initialization
        Parameters = [len(FeatureList), 32, 32]
        model = mlp_model.MyModel(Parameters)
        print(model)
        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        model = model.to(device)

        ## optimizer  
        optimizer = optim.Adam(model.parameters(), lr = constants.LR, weight_decay=0)

        ## loss function
        criterion = nn.CrossEntropyLoss()
        acc, TheResult = TrainMain(model, TheData[i]["train"], TheData[i]["test"], criterion, optimizer, device)
        best_accs.append(acc)
        Return["TP"] += TheResult["TP"]
        Return["TN"] += TheResult["TN"]
        Return["FP"] += TheResult["FP"]
        Return["FN"] += TheResult["FN"]
        
    for i in range(constants.KFolds):
        print("The best valid accuracy in data number {:d} is {:.2f}%.".format(i + 1, best_accs[i] * 100))
    print("The average valid accuracy is {:.2f}%".format(np.mean(best_accs) * 100))
    return Return