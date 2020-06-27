import torch 
import torch.nn as nn
import torchvision
class MyModel(nn.Module):    
    def __init__(self, Parameters):
        '''
        描述：初始化模型
        参数：输入参数，是一个list, 长度为3，每个元素代表每一层输入
        '''
        super(MyModel, self).__init__()
        self.relu = nn.ReLU()
        self.layer0 = nn.Linear(Parameters[0], Parameters[1])
        self.layer1 = nn.Linear(Parameters[1], Parameters[2])
        self.layer2 = nn.Linear(Parameters[2], 2)
        
    def forward(self, x):
        '''
        描述：模型,三层卷积一层全连接，relu
        '''
        out = self.relu(self.layer0(x))
        out = self.relu(self.layer1(out))
        out = self.layer2(out)
        return out
    
