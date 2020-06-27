'''
绘制一个模型的训练，测试模型
'''

import matplotlib.pyplot as plt
import os
import numpy as np
from tensorboardX import SummaryWriter

class PlotCurves:
    def GetAccuracy(self, file):
        '''
        描述：读取记录文件，获取训练测试准确率
        '''
        y_train = []
        y_valid = []
        with open(file) as f:
            lines = f.readlines()
            for item in lines:
                item_split = item.split()
                if(len(item_split) <= 0):
                    continue
                if item_split[0] == "training_pp:":
                    y_train.append(float(item_split[-1]))
                elif item_split[0] == "validation_pp:":
                    y_valid.append(float(item_split[-1]))
        return y_train, y_valid            

    def PlotModel(self, y_train, y_valid, model_name, save_place):
        '''
        描述：绘制并且保存curve图片
        '''
        x = range(len(y_train)) 
        plt.plot(x, y_train, label='Train Accuracy', linewidth = 3, color = 'r')
        plt.plot(x, y_valid, label='Validate Accuracy', linewidth = 3, color = 'b')
        plt.xlabel('Epoch Number')
        plt.ylabel('Accuracy')
        plt.title('Accuracy curve of model ' + model_name)
        plt.legend()
        plt.savefig(save_place)
        plt.show() 

    def PlotTensorboard(self, y_train, y_valid):
        writer = SummaryWriter()
        for epoch in range(len(y_train)):
            writer.add_scalars('scalar/test', {"train" : y_train[epoch], "valid" : y_valid[epoch]}, epoch)
            #writer.add_scalar('scalar/test', y_valid, epoch)
        writer.close()
        
    def PlotContrast(self, ScalarList, length):
        writer = SummaryWriter()
        for epoch in range(length):
            TheScalar = {}
            for name in ScalarList.keys():
                TheScalar[name] = ScalarList[name][epoch]
            writer.add_scalars('scalar/test', TheScalar, epoch)
            #writer.add_scalar('scalar/test', y_valid, epoch)
        writer.close()
        
    def __init__(self, model_list):
        super(PlotCurves, self).__init__()
        ScalarList = {}
        length = -1
        if len(model_list) == 1:
            item = model_list[0]
            model_name = item["model_name"]
            model_dir = item["model_dir"]
            log_place = "../result/" + model_dir + ".txt"
            y_train, y_valid = self.GetAccuracy(log_place)
            ScalarList["train"] = y_train
            ScalarList["valid"] = y_valid
            length = len(y_train)
            self.PlotContrast(ScalarList, length)        
        else:
            for item in model_list:
                model_name = item["model_name"]
                model_dir = item["model_dir"]
                log_place = "../result/" + model_dir + ".txt"
                y_train, y_valid = self.GetAccuracy(log_place)
                if length < 0:
                    length = len(y_train)
                ScalarList[model_name] = y_valid
            #self.PlotModel(y_train, y_valid, model_name, save_place)
            self.PlotContrast(ScalarList, length)
        
if __name__ == '__main__':
    PlotModel = []
    ModelRNN = {"model_name":"RNN", "model_dir":"rnn"}
    ModelLSTM = {"model_name":"LSTM", "model_dir":"lstm"}
    ModelGRU = {"model_name":"GRU", "model_dir":"basic"}
    ModelSelf = {"model_name":"self_GRU", "model_dir":"self"}
    PlotModel.append(ModelRNN)
    PlotModel.append(ModelLSTM)
    PlotModel.append(ModelGRU)
    PlotModel.append(ModelSelf)
    
    PlotLR = []
    LRSmall = {"model_name":"10^-4", "model_dir":"10-4"}
    LRBig = {"model_name":"10^-2", "model_dir":"10-2"}
    LRNormal = {"model_name":"10^-3","model_dir":"basic"}
    LRDecaySlow = {"model_name":"10^-3, with slow decay rate","model_dir":"slow_decay"}
    LRDecayFast = {"model_name":"10^-3, with fast decay rate","model_dir":"fast_decay"}
    PlotLR.append(LRNormal)
    PlotLR.append(LRSmall)
    PlotLR.append(LRBig)
    PlotLR.append(LRDecaySlow)
    PlotLR.append(LRDecayFast)
    
    PlotOptimize = []
    OptimizeAdam = {"model_name":"Adam", "model_dir":"basic"}
    OptimizeDecay = {"model_name":"Adam with weight decay", "model_dir":"adam_decay"}
    OptimizeSGD = {"model_name":"SGD", "model_dir":"sgd"}
    PlotOptimize.append(OptimizeAdam)
    PlotOptimize.append(OptimizeDecay)
    PlotOptimize.append(OptimizeSGD)

    PlotInitialize = []
    InitNormal = {"model_name":"Normal", "model_dir":"basic"}
    InitOrth = {"model_name":"Orthogonal", "model_dir":"orth_init"}
    PlotInitialize.append(InitNormal)
    PlotInitialize.append(InitOrth)
    
    PlotDropout = []
    DropNo = {"model_name":"No dropout", "model_dir":"no_dropout"}
    DropYes = {"model_name":"Dropout RNN", "model_dir":"basic"}
    PlotDropout.append(DropNo)
    PlotDropout.append(DropYes)
    
    PlotClip = []
    ClipNo = {"model_name":"No gradient clip", "model_dir":"clip"}
    ClipYes = {"model_name":"Gradient clip", "model_dir":"basic"}
    PlotClip.append(ClipNo)
    PlotClip.append(ClipYes)
    
    PlotWeightNorm = []
    WeightNormYes = {"model_name":"Weight Norm", "model_dir":"weight_norm"}
    WeightNormNo = {"model_name":"No Weight Norm", "model_dir":"basic"}
    PlotWeightNorm.append(WeightNormNo)
    PlotWeightNorm.append(WeightNormYes)

    PlotLayerNorm = []
    LayerNormYes = {"model_name":"Layer Norm", "model_dir":"layer_norm"}
    LayerNormNo = {"model_name":"No Layer Norm", "model_dir":"self"}
    PlotLayerNorm.append(LayerNormNo)
    PlotLayerNorm.append(LayerNormYes)

    PlotAttention_1 = []
    AttentionMatrix_1 = {"model_name":"Attention With Matrix", "model_dir":"attention_matrix"}
    AttentionInnerProduct_1 = {"model_name":"Attention With Inner Product", "model_dir":"attention"}
    AttentionNo_1 = {"model_name":"No Attention", "model_dir":"basic"}
    PlotAttention_1.append(AttentionNo_1)
    PlotAttention_1.append(AttentionMatrix_1)
    PlotAttention_1.append(AttentionInnerProduct_1)
    
    PlotAttention_2 = []
    AttentionMatrix_2 = {"model_name":"Attention With Matrix", "model_dir":"attention_matrix_refracted"}
    AttentionInnerProduct_2 = {"model_name":"Attention With Inner Product", "model_dir":"attention_refracted"}
    AttentionNo_2 = {"model_name":"No Attention", "model_dir":"basic"}
    PlotAttention_2.append(AttentionNo_2)
    PlotAttention_2.append(AttentionMatrix_2)
    PlotAttention_2.append(AttentionInnerProduct_2)  
    
    PlotBest = [{"model_name":"best", "model_dir":"basic"}]
    
    print("please input the type you want")
    print("0:Network types")
    print("1:LR")
    print("2:Optimize")
    print("3:Initialize")
    print("4:Dropout")
    print("5:Gradient Clip")
    print("6:Weight Norm")
    print("7:Layer Norm")
    print("8:Attention_1")
    print("9:Attention_2")
    print("10:Plot the train and valid pp of the best model")

    num = int(input())
    if num == 0:
        PlotCurves(PlotModel)
    elif num == 1:
        PlotCurves(PlotLR)
    elif num == 2:
        PlotCurves(PlotOptimize)
    elif num == 3:
        PlotCurves(PlotInitialize)    
    elif num == 4:
        PlotCurves(PlotDropout)
    elif num == 5:
        PlotCurves(PlotClip)
    elif num == 6:
        PlotCurves(PlotWeightNorm)
    elif num == 7:
        PlotCurves(PlotLayerNorm)      
    elif num == 8:
        PlotCurves(PlotAttention_1) 
    elif num == 9:
        PlotCurves(PlotAttention_2) 
    elif num == 10:
        PlotCurves(PlotBest)