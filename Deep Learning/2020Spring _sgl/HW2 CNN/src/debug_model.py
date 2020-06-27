'''
模型初步验证---用少量样本判断模型是否能够过拟合训练数据
'''

import torch
import torch.nn as nn
import torch.optim as optim
import parameters as hyper
import data
import models
import os
import time
import model_self

def debug_model(model, loader, criterion, optimizer, num_epochs=20):
    
    def adjust_learning_rate(optimizer, epoch):
        """Sets the learning rate to the initial LR decayed by 2 every 10 epochs"""
        lr = hyper.lr * (0.5 ** (epoch // 10))
        for param_group in optimizer.param_groups:
            param_group['lr'] = lr
        return lr

    def train(model, train_loader,optimizer,criterion):
        model.train(True)
        total_loss = 0.0
        total_correct = 0
        for inputs, labels in train_loader:
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
    
    max_rate = 0
    for epoch in range(num_epochs):
        print('epoch:{:d}/{:d}'.format(epoch, num_epochs))
        print('*' * 100)
        
        #current_lr = adjust_learning_rate(optimizer, epoch)
        #print('current lr: ', current_lr)

        train_start = time.time()
        train_loss, train_acc = train(model, loader,optimizer,criterion)
        train_end = time.time()
        print("training: {:.4f}, {:.4f}".format(train_loss, train_acc))
        print("train time: {:.4f} s".format(train_end - train_start))
        
        if max_rate < train_acc:
            max_rate = train_acc
    if max_rate >= hyper.debug_threshold:
        print("The model can overfit the training data, valid.")
    else:
        print("The model cannot overfit the training data, invalid.")  
    
    
if __name__ == '__main__':
    os.environ["CUDA_VISIBLE_DEVICES"] = "5"


    ## model initialization
    model = models.model_C(num_classes = hyper.num_classes, input_size = hyper.input_size)

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model = model.to(device)

    ## data preparation
    _, _, debug_loader = data.load_data \
    (data_dir = hyper.data_dir,input_size = hyper.input_size)

    ## optimizer
    optimizer = optim.Adam(model.parameters(), lr = hyper.lr)

    ## loss function
    criterion = nn.CrossEntropyLoss()
    
    debug_model(model, debug_loader, criterion, optimizer, num_epochs = hyper.num_epochs)

