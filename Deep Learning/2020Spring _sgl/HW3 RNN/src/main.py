# coding: utf-8
import argparse
import time
import math
import torch
import torch.nn as nn

import constants
import data
import model
import model_attention_self
import model_self
import os
import os.path as osp
import time


parser = argparse.ArgumentParser(description='PyTorch ptb Language Model')
parser.add_argument('--epochs', type=int, default=100,
                    help='upper epoch limit')
parser.add_argument('--train_batch_size', type=int, default=20, metavar='N',
                    help='batch size')
parser.add_argument('--eval_batch_size', type=int, default=10, metavar='N',
                    help='eval batch size')
parser.add_argument('--max_sql', type=int, default=35,
                    help='sequence length')

parser.add_argument('--seed', type=int, default=1234,
                    help='set random seed')
parser.add_argument('--cuda', action='store_true', help='use CUDA device')
parser.add_argument('--gpu_id', type=int, help='GPU device id used', default=0)


#自己设定的参数
#网络类型 0:GRU 1:LSTM 2:RNN
parser.add_argument('--network_type', type=int, default=constants.network_gru)
#学习率策略：0:10^-3 1:10^-2 2:10^-4 3: 10^-3慢速衰减 4:10^-4快速衰减
parser.add_argument('--lr_strategy', type=int, default=constants.lr_3)
#优化方法：0：adam带多余项 1：adam不带多余项 2：SGD
parser.add_argument('--optim_strategy', type=int, default=constants.optim_adam)
#初始化策略

#dropout策略 0:不dropout 1：dropout rnn
parser.add_argument('--dropout_strategy', type=int, default=constants.dropout_true)
#梯度clip策略 0:不clip 1：clip
parser.add_argument('--clip_strategy', type=int, default=constants.clip_false)
#初始化策略 0：默认初始化 1：正交初始化
parser.add_argument('--init_strategy', type=int, default=constants.init_original)
#weight_norm策略 0：不weight_norm 1：10 epoch一次 2:20 epoch一次
parser.add_argument('--weight_norm', type=int, default=constants.weight_norm_no)

args = parser.parse_args()

# Set the random seed manually for reproducibility.
torch.manual_seed(args.seed)

# Use gpu or cpu to train

use_gpu = True

if use_gpu:
    torch.cuda.set_device(args.gpu_id)
    device = torch.device(args.gpu_id)
else:
    device = torch.device("cpu")

#print(device)
# load data
train_batch_size = args.train_batch_size
eval_batch_size = args.eval_batch_size
batch_size = {'train': train_batch_size,'valid':eval_batch_size}
data_loader = data.Corpus("../data/ptb", batch_size, args.max_sql)
voc_size = data_loader.voc_size


        
# WRITE CODE HERE within two '#' bar
########################################
# Build LMModel model (bulid your language model here)
# choose model according to console inputs
if args.network_type == constants.network_self or args.network_type == constants.network_layer_norm:
    model = model_self.LMModel(device, args.network_type, voc_size, 50)
elif args.network_type == constants.network_attention:
    model = model_attention.LMModel(device, voc_size, 50)
elif args.network_type == constants.network_attention_self or args.network_type == constants.network_attention_self_matrix \
    or args.network_type == constants.network_attention_self_modified or args.network_type == constants.network_attention_self_matrix_modified:
    model = model_attention_self.LMModel(device, args.network_type, voc_size, 50, 50, 2, args.max_sql)
else:    
    model = model.LMModel(device, args.network_type, args.dropout_strategy, args.init_strategy, args.weight_norm, voc_size, 50, 50, 2)
#model = model_self.LMModel(device, voc_size, 50)
#model = model_attention.LMModel(voc_size, 50, 50, 2)
model = model.to(device)
########################################


#init learning rate
if args.lr_strategy == constants.lr_2:
    current_lr = 0.01
elif args.lr_strategy == constants.lr_4:
    current_lr = 0.0001
else:
    current_lr = 0.001
criterion = nn.CrossEntropyLoss()

#init optimizer
if args.optim_strategy == constants.optim_adam_decay:
    optimizer = torch.optim.Adam(model.parameters(), lr = current_lr, weight_decay=0.1)
elif args.optim_strategy == constants.optim_adam:
    optimizer = torch.optim.Adam(model.parameters(), lr = current_lr)
elif args.optim_strategy == constants.optim_sgd:
    optimizer = torch.optim.SGD(model.parameters(), lr = current_lr)
    
# WRITE CODE HERE within two '#' bar
########################################
# Evaluation Function
# Calculate the average cross-entropy loss between the prediction and the ground truth word.
# And then exp(average cross-entropy loss) is perplexity.

def evaluate(model, data_loader, criterion):
    model.train(False)
    EndFlag = False
    data_loader.set_valid()
    total_loss = 0.0
    total_correct = 0
    total_num = 0
    while EndFlag == False:
        valid, target, EndFlag = data_loader.get_batch()
        if(EndFlag == True):
            continue
        total_num += valid.size(0) * valid.size(1)
        valid = valid.to(device)
        target = target.to(device)
        outputs, _ = model(valid)
        outputs = outputs.view(outputs.size(0) * outputs.size(1), -1)
        _, predictions = torch.max(outputs, 1)
        loss = criterion(outputs, target)
        total_loss += loss.item() * valid.size(0) * valid.size(1)
        total_correct += torch.sum(predictions == target.data)
    epoch_pp = math.exp(total_loss / total_num)
    epoch_acc = total_correct.double() / total_num
    return epoch_pp, epoch_acc.item()
########################################


# WRITE CODE HERE within two '#' bar
########################################
# Train Function
def train(model, data_loader, optimizer, criterion):
    model.train(True)
    EndFlag = False
    data_loader.set_train()
    total_loss = 0.0
    total_correct = 0
    total_num = 0
    while EndFlag == False:
        train, target, EndFlag = data_loader.get_batch()
        if(EndFlag == True):
            continue
        total_num += train.size(0) * train.size(1)
        train = train.to(device)
        target = target.to(device)
        optimizer.zero_grad()
        outputs, _ = model(train)
        outputs = outputs.view(outputs.size(0) * outputs.size(1), -1)
        _, predictions = torch.max(outputs, 1)   
        loss = criterion(outputs, target)
        loss.backward()
        
        #gradient clip
        if args.clip_strategy == constants.clip_true:
            nn.utils.clip_grad_norm_(model.parameters(), max_norm=20)
        
        optimizer.step()
        total_loss += loss.item() * train.size(0) * train.size(1)
        total_correct += torch.sum(predictions == target.data)
    epoch_pp = math.exp(total_loss / total_num)
    epoch_acc = total_correct.double() / total_num
    return epoch_pp, epoch_acc.item()
########################################

def adjust_learning_rate_fast(original_lr, optimizer, epoch):
    """Sets the learning rate to the initial LR decayed by 2 every 5 epochs"""
    lr = original_lr * (0.5 ** (epoch // 5))
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr
    return lr

def adjust_learning_rate_slow(original_lr, optimizer, epoch):
    """Sets the learning rate to the initial LR decayed by 2 every 10 epochs"""
    lr = original_lr * (0.5 ** (epoch // 10))
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr
    return lr
# Loop over epochs.
acc_list = []
original_lr = current_lr        
#判断是否进行weight_norm
if args.weight_norm == constants.weight_norm_yes and args.network_type == constants.network_gru:
    model._modules['rnn'].flatten_parameters()
    nn.utils.weight_norm(model._modules['rnn'], name='weight_ih_l0')
    nn.utils.weight_norm(model._modules['rnn'], name='weight_hh_l0')
    nn.utils.weight_norm(model._modules['rnn'], name='weight_ih_l1')
    nn.utils.weight_norm(model._modules['rnn'], name='weight_hh_l1')



for epoch in range(1, args.epochs+1):
    print('epoch:{:d}/{:d}'.format(epoch, args.epochs))
    print('*' * 100)
    
    #learning rate decay
    if args.lr_strategy == constants.lr_slow:    
        current_lr = adjust_learning_rate_slow(original_lr, optimizer, epoch)
    elif args.lr_strategy == constants.lr_fast:
        current_lr = adjust_learning_rate_fast(original_lr, optimizer, epoch)
    print('current lr: ', current_lr)
        
    train_start = time.time()
    train_pp, train_acc = train(model, data_loader,optimizer,criterion)
    train_end = time.time()
        
    print("training: {:.4f}, {:.4f}".format(train_pp, train_acc))
        
    valid_start = time.time()
    valid_pp, valid_acc = evaluate(model, data_loader,criterion)
    valid_end = time.time()

    print("validation: {:.4f}, {:.4f}".format(valid_pp, valid_acc))
    print("train time: {:.4f} s".format(train_end - train_start))
    print("valid time: {:.4f} s".format(valid_end - valid_start))

    acc_list.append({"train": train_acc, "valid": valid_acc, "train_pp": train_pp, "valid_pp":valid_pp})
    
#输出信息
log_filename = "../result/C.txt"
with open(log_filename,"w") as f:
    for item in acc_list:
        f.write("training: {:.4f}\n".format(item["train"]))
        f.write("validation: {:.4f}\n".format(item["valid"]))
        f.write("training_pp: {:.4f}\n".format(item["train_pp"]))
        f.write("validation_pp: {:.4f}\n".format(item["valid_pp"]))
