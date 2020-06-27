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
                if item_split[0] == "training:":
                    y_train.append(float(item_split[-1]))
                elif item_split[0] == "validation:":
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
        for item in model_list:
            model_name = item["model_name"]
            model_dir = item["model_dir"]
            log_place = "../result/reports/" + model_dir + ".txt"
            y_train, y_valid = self.GetAccuracy(log_place)
            if length < 0:
                length = len(y_train)
            ScalarList[model_name] = y_valid
        #self.PlotModel(y_train, y_valid, model_name, save_place)
        self.PlotContrast(ScalarList, length)
        
if __name__ == '__main__':
    PlotLR = []
    LRNormal = {"model_name":"10^-4", "model_dir":"C_optimal"}
    LRSmall = {"model_name":"10^-5", "model_dir":"C_lr_small"}
    LRBig = {"model_name":"10^-3","model_dir":"C_lr_big"}
    LRDecaySlow = {"model_name":"10^-4, with slow decay rate","model_dir":"C_decay_slow"}
    LRDecayFast = {"model_name":"10^-4, with fast decay rate","model_dir":"C_decay_fast"}
    PlotLR.append(LRNormal)
    PlotLR.append(LRSmall)
    PlotLR.append(LRBig)
    PlotLR.append(LRDecaySlow)
    PlotLR.append(LRDecayFast)
    
    PlotOptimize = []
    OptimizeAdam = {"model_name":"Adam", "model_dir":"C_optimal"}
    OptimizeDecay = {"model_name":"Adam with weight decay", "model_dir":"C_adam_weight_decay"}
    OptimizeSGD = {"model_name":"SGD", "model_dir":"C_SGD"}
    PlotOptimize.append(OptimizeAdam)
    PlotOptimize.append(OptimizeDecay)
    PlotOptimize.append(OptimizeSGD)

    PlotInitialize = []
    InitXavier = {"model_name":"Xavier", "model_dir":"C_optimal"}
    InitKaiming = {"model_name":"Kaiming", "model_dir":"C_kaiming"}
    PlotInitialize.append(InitXavier)
    PlotInitialize.append(InitKaiming)
    
    PlotDropout = []
    DropNo = {"model_name":"No dropout", "model_dir":"C_optimal"}
    Drop1 = {"model_name":"Dropout layer 1", "model_dir":"C_dropout1"}
    Drop3 = {"model_name":"Dropout layer 3", "model_dir":"C_dropout3"}
    Dropall = {"model_name":"Dropout all layers", "model_dir":"C_dropout_all"}
    PlotDropout.append(DropNo)
    PlotDropout.append(Drop1)
    PlotDropout.append(Drop3)
    PlotDropout.append(Dropall)
    
    PlotCrop = []
    CropMiddle = {"model_name":"Crop Middle(0.64)", "model_dir":"C_optimal"}
    CropSmall = {"model_name":"Crop Middle(0.08)", "model_dir":"C_0.08"}
    CropNo = {"model_name":"Crop Middle(1)", "model_dir":"C_0.99"}
    CropNoConstrain = {"model_name":"Crop without constrain", "model_dir":"C_Crop_unbounded"}
    PlotCrop.append(CropMiddle)
    PlotCrop.append(CropSmall)
    PlotCrop.append(CropNo)
    PlotCrop.append(CropNoConstrain)

    PlotColor = []
    ColorUnchanged = {"model_name":"Unchanged", "model_dir":"C_optimal"}
    ColorBright = {"model_name":"Brightness", "model_dir":"C_brightness"}
    ColorContrast = {"model_name":"Contrast", "model_dir":"C_contrast"}
    ColorSatuation = {"model_name":"Satuation", "model_dir":"C_satuation"}
    ColorHue = {"model_name":"Hue", "model_dir":"C_hue"}
    PlotColor.append(ColorUnchanged)
    PlotColor.append(ColorBright)
    PlotColor.append(ColorContrast)
    PlotColor.append(ColorSatuation)
    PlotColor.append(ColorHue)

    PlotRotate = []
    RotateNo = {"model_name":"Unchanged", "model_dir":"C_optimal"}
    Rotate30 = {"model_name":"Rotate 30", "model_dir":"C_rotation"}
    PlotRotate.append(RotateNo)
    PlotRotate.append(Rotate30)
    
    PlotGrey = []
    GreyNo = {"model_name":"Unchanged", "model_dir":"C_optimal"}
    GreyYes = {"model_name":"grey", "model_dir":"C_grey"}
    PlotGrey.append(GreyNo)
    PlotGrey.append(GreyYes)
    
    PlotNewCrop = []
    CropNormal = {"model_name":"Normal crop", "model_dir":"C_optimal"}
    CropFive = {"model_name":"5 crop", "model_dir":"C_crop5"}
    CropTen = {"model_name":"10 crop", "model_dir":"C_crop10"}
    PlotNewCrop.append(CropNormal)
    PlotNewCrop.append(CropFive)
    PlotNewCrop.append(CropTen)
    
    PlotLayer = []
    Layer1 = {"model_name":"One MLP layer", "model_dir":"C_optimal"}
    Layer2 = {"model_name":"2 MLP layers", "model_dir":"C_2layers"}
    PlotLayer.append(Layer1)
    PlotLayer.append(Layer2)
    
    PlotFlips = []
    HorizontalFlip = {"model_name":"Horizontal Flip", "model_dir":"C_optimal"}
    VerticalFlip = {"model_name":"Vertical Flip", "model_dir":"C_vertical"}
    NoFlip = {"model_name":"No Flip", "model_dir":"C_noflip"}
    PlotFlips.append(HorizontalFlip)
    PlotFlips.append(VerticalFlip)
    PlotFlips.append(NoFlip)
    
    print("please input the type you want")
    print("0:LR")
    print("1:Optimize")
    print("2:Initialize")
    print("3:Dropout")
    print("4:Crop")
    print("5:Color")
    print("6:Rotate")
    print("7:Grey")
    print("8:NewCrop")
    print("9:DifferentLayers")
    print("10:Flips")

    num = int(input())
    if num == 0:
        PlotCurves(PlotLR)
    elif num == 1:
        PlotCurves(PlotOptimize)
    elif num == 2:
        PlotCurves(PlotInitialize)    
    elif num == 3:
        PlotCurves(PlotDropout)
    elif num == 4:
        PlotCurves(PlotCrop)
    elif num == 5:
        PlotCurves(PlotColor)
    elif num == 6:
        PlotCurves(PlotRotate)
    elif num == 7:
        PlotCurves(PlotGrey)      
    elif num == 8:
        PlotCurves(PlotNewCrop) 
    elif num == 9:
        PlotCurves(PlotLayer) 
    elif num == 10:
        PlotCurves(PlotFlips)