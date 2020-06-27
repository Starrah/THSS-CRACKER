import torch 
import torch.nn as nn
import torchvision
import parameters as hyper
class MyModel(nn.Module):    
    def __init__(self, input_size, num_classes, parameter_conv):
        '''
        描述：初始化模型
        参数：输入图片大小（默认224，输入图片是224*224*3），分类种类，一组图片数目，卷积层超参数数组
        卷积层超参数数组包括：channels，kernel_size，stride，padding，bias（true false），groups
        mlp超参数
        '''
        super(MyModel, self).__init__()
        
        #计算各种大小
        self.input_size = input_size
        self.input_channel = 3
        self.size0 = 1 + (self.input_size - parameter_conv[0]["kernel_size"] + 2 * parameter_conv[0]["padding"]) \
            // parameter_conv[0]["stride"]
        self.channel0 = parameter_conv[0]["channels"]
        self.size_pool = 1 + (self.size0 - hyper.pooling_kernel) \
            //  hyper.pooling_stride
        self.size1 = 1 + (self.size_pool - parameter_conv[1]["kernel_size"] + 2 * parameter_conv[1]["padding"]) \
            // parameter_conv[1]["stride"]
        self.channel1 = parameter_conv[1]["channels"]
        self.size2 = 1 + (self.size1 - parameter_conv[2]["kernel_size"] + 2 * parameter_conv[2]["padding"]) \
            // parameter_conv[2]["stride"]
        self.channel2 = parameter_conv[2]["channels"]
        self.size_end = 1 + (self.size2 - hyper.pooling_kernel) // hyper.pooling_stride
        '''self.size3 = 1 + (self.size2 - parameter_conv[3]["kernel_size"] + 2 * parameter_conv[3]["padding"]) \
            // parameter_conv[3]["stride"]
        self.channel3 = parameter_conv[3]["channels"]'''
        
        self.layer0 = nn.Sequential()
        self.layer0.add_module("conv0", nn.Conv2d(in_channels = self.input_channel, out_channels = parameter_conv[0]["channels"], \
            kernel_size = parameter_conv[0]["kernel_size"], \
            stride = parameter_conv[0]["stride"], padding = parameter_conv[0]["padding"], bias = parameter_conv[0]["bias"], \
            groups = parameter_conv[0]["groups"]))
        self.layer0.add_module("relu0", torch.nn.ReLU())
        self.layer0.add_module("bn0", nn.BatchNorm2d(self.channel0))
        
        self.pooling = nn.MaxPool2d(kernel_size = hyper.pooling_kernel, stride = hyper.pooling_stride)

        
        self.layer1 = nn.Sequential()
        self.layer1.add_module("conv1", nn.Conv2d(in_channels = self.channel0, out_channels = parameter_conv[1]["channels"], \
            kernel_size = parameter_conv[1]["kernel_size"], \
            stride = parameter_conv[1]["stride"], padding = parameter_conv[1]["padding"], bias = parameter_conv[1]["bias"], \
            groups = parameter_conv[1]["groups"]))
        self.layer1.add_module("relu1", torch.nn.ReLU())
        self.layer1.add_module("bn1", nn.BatchNorm2d(self.channel1))
        
        self.layer2 = nn.Sequential()
        self.layer2.add_module("conv2", nn.Conv2d(in_channels = self.channel1, out_channels = parameter_conv[2]["channels"], \
            kernel_size = parameter_conv[2]["kernel_size"], \
            stride = parameter_conv[2]["stride"], padding = parameter_conv[2]["padding"], bias = parameter_conv[2]["bias"], \
            groups = parameter_conv[2]["groups"]))
        self.layer2.add_module("relu2", torch.nn.ReLU())
        self.layer2.add_module("bn2", nn.BatchNorm2d(self.channel2))    
        
        '''self.dense1 = nn.Sequential()
        self.dense1.add_module("fc1", nn.Linear(in_features = self.size2 * self.size2 * self.channel2, out_features = 512))
        self.dense1.add_module("relu", torch.nn.ReLU())'''
        
        self.dense = nn.Sequential()
        self.dense.add_module("fc",nn.Linear(in_features = self.size2 * self.size2 * self.channel2, out_features = num_classes, bias = True))

    def forward(self, x):
        '''
        描述：模型,三层卷积一层全连接，relu
        '''
        out = self.layer0(x)
        out = self.pooling(out)
        out = self.layer1(out)
        out = self.layer2(out)
        
        #将卷积的四维张量转化成全连接的二维，不能丢，丢了就gg
        out = out.view(out.size(0), -1)
        
        #out = self.dense1(out)
        out = self.dense(out)
        return out
    
    def get_feature(self, x):
        '''
        描述：获得最后一层全连接之外的情况
        '''
        out = self.layer0(x)
        out = self.pooling(out)
        out = self.layer1(out)
        out = self.layer2(out)

        #将卷积的四维张量转化成全连接的二维，不能丢，丢了就gg
        out = out.view(out.size(0), -1)
        #out = self.dense1(out)
        return out
    
