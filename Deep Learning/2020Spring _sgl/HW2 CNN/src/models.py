from torchvision import models
import torch
import torch.nn as nn
import torch.nn.functional as F
import re
import model_self
import parameters as hyper

def initNetParams(model):
    '''
    初始化网络参数
    '''
    for m in model.modules():
        if isinstance(m, nn.Conv2d):
            print(m)
            nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
        elif isinstance(m, nn.BatchNorm2d):
            print(m)
            nn.init.constant(m.weight, 1)
            nn.init.constant(m.bias, 0) 
                
def model_A(num_classes, pretrained = True):
    model_resnet = models.resnet18(pretrained=pretrained)
    num_features = model_resnet.fc.in_features
    model_resnet.fc = nn.Linear(num_features, num_classes)
    return model_resnet

def model_B(num_classes, pretrained = False):
    model_resnet = models.resnet18(pretrained = pretrained)
    num_features = model_resnet.fc.in_features
    model_resnet.fc = nn.Linear(num_features, num_classes)
    return model_resnet

def model_C(num_classes, input_size):
    conv_parameter = []
    conv0_parameter = {"channels": hyper.channel_0, "kernel_size" : 7, "stride": 2, "padding": 0, "bias" : True, "groups" : 1}
    conv1_parameter = {"channels": hyper.channel_1, "kernel_size" : 5, "stride": 2, "padding": 0, "bias" : True, "groups" : 1}
    conv2_parameter = {"channels": hyper.channel_2, "kernel_size" : 7, "stride": 2, "padding": 0, "bias" : True, "groups" : 1}
    conv3_parameter = {"channels": hyper.channel_3, "kernel_size" : 3, "stride": 2, "padding": 0, "bias" : True, "groups" : 1}

    conv_parameter.append(conv0_parameter)
    conv_parameter.append(conv1_parameter)
    conv_parameter.append(conv2_parameter)
    conv_parameter.append(conv3_parameter)

    the_model = model_self.MyModel(input_size, num_classes, conv_parameter)
    return the_model

def model_continue():
    model_con = torch.load("best_model.pt")
    return model_con



