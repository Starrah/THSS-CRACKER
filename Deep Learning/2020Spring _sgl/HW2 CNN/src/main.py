'''
主函数---训练模型
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

## Note that: here we provide a basic solution for training and validation.
## You can directly change it if you find something wrong or not good enough.

def train_model(model,train_loader, valid_loader, criterion, optimizer, num_epochs=20):


    def adjust_learning_rate_fast(optimizer, epoch):
        """Sets the learning rate to the initial LR decayed by 2 every 10 epochs"""
        lr = hyper.lr * (0.5 ** (epoch // 10))
        for param_group in optimizer.param_groups:
            param_group['lr'] = lr
        return lr
    
    def adjust_learning_rate_slow(optimizer, epoch):
        """Sets the learning rate to the initial LR decayed by 2 every 10 epochs"""
        lr = hyper.lr * (0.5 ** (epoch // 20))
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

    def valid(model, valid_loader,criterion):
        model.train(False)
        total_loss = 0.0
        total_correct = 0
        for inputs, labels in valid_loader:
            inputs = inputs.to(device)
            labels = labels.to(device)
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            _, predictions = torch.max(outputs, 1)
            total_loss += loss.item() * inputs.size(0)
            total_correct += torch.sum(predictions == labels.data)
            
        epoch_loss = total_loss / len(valid_loader.dataset)
        epoch_acc = total_correct.double() / len(valid_loader.dataset)
        return epoch_loss, epoch_acc.item()

    acc_list = []
    best_acc = 0.0
    for epoch in range(num_epochs):
        print('epoch:{:d}/{:d}'.format(epoch, num_epochs))
        print('*' * 100)
        
        #current_lr = adjust_learning_rate_slow(optimizer, epoch)
        #print('current lr: ', current_lr)
        
        train_start = time.time()
        train_loss, train_acc = train(model, train_loader,optimizer,criterion)
        train_end = time.time()
        
        print("training: {:.4f}, {:.4f}".format(train_loss, train_acc))
        
        valid_start = time.time()
        valid_loss, valid_acc = valid(model, valid_loader,criterion)
        valid_end = time.time()

        print("validation: {:.4f}, {:.4f}".format(valid_loss, valid_acc))
        print("train time: {:.4f} s".format(train_end - train_start))
        print("valid time: {:.4f} s".format(valid_end - valid_start))

        if valid_acc > best_acc:
            best_acc = valid_acc
            best_model = model
            torch.save(best_model, 'best_model.pt')
        acc_list.append({"train": train_acc, "valid": valid_acc})
    return acc_list

if __name__ == '__main__':
    os.environ["CUDA_VISIBLE_DEVICES"] = "5"


    ## model initialization
    #model = models.model_A(num_classes = hyper.num_classes)
    #model = models.model_B(num_classes = hyper.num_classes)
    model = models.model_C(num_classes = hyper.num_classes, input_size = hyper.input_size)
    print(model)
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model = model.to(device)

    ## data preparation
    train_loader, valid_loader, debug_loader = data.load_data \
    (data_dir = hyper.data_dir,input_size = hyper.input_size, batch_size = hyper.batch_size)

    ## optimizer  
    optimizer = optim.Adam(model.parameters(), lr = hyper.lr, weight_decay=0)

    ## loss function
    criterion = nn.CrossEntropyLoss()
    
    
    acc_list = train_model(model,train_loader, valid_loader, criterion, optimizer, num_epochs = hyper.num_epochs)

    log_filename = "../result/reports/C.txt"
    with open(log_filename,"w") as f:
        for item in acc_list:
            f.write("training: {:.4f}\n".format(item["train"]))
            f.write("validation: {:.4f}\n".format(item["valid"]))