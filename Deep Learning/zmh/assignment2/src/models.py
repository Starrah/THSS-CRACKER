from torchvision import models
import torch.nn as nn
import torch.nn.functional as F
import torch


def model_A(num_classes, pretrained=True):
    model_resnet = models.resnet18(pretrained=pretrained)
    num_features = model_resnet.fc.in_features
    model_resnet.fc = nn.Linear(num_features, num_classes)
    return model_resnet


def model_B(num_classes, pretrained=False):
    ## your code here
    model_resnet = models.resnet18(pretrained=pretrained)
    num_features = model_resnet.fc.in_features
    model_resnet.fc = nn.Linear(num_features, num_classes)
    return model_resnet


def model_C(num_classes, pretrained=False):
    ## your code here

    net = Network(num_classes)
    return net


class Network(nn.Module):
    def __init__(self, num_classes):
        super(Network, self).__init__()

        self.conv1 = nn.Conv2d(3, 64, 3, stride=1, padding=1)
        self.bn1 = nn.BatchNorm2d(64)
        self.pool1 = nn.MaxPool2d((3, 3), stride=3)

        self.conv2 = nn.Conv2d(64, 128, 3, stride=1, padding=1)
        self.bn2 = nn.BatchNorm2d(128)

        self.pool2 = nn.MaxPool2d((3, 3), stride=2)

        self.conv3 = nn.Conv2d(128, 256, 3, stride=1, padding=1)
        self.bn3 = nn.BatchNorm2d(256)

        self.pool3 = nn.MaxPool2d((3, 3), stride=3)
        self.conv4 = nn.Conv2d(256, 512, 3, stride=1, padding=1)
        self.bn4 = nn.BatchNorm2d(512)

        self.pool4 = nn.MaxPool2d((2, 2), stride=2)
        self.linear0 = nn.Linear(18432, num_classes)

    def forward(self, input):
        # input size: B x C x H x W
        x = self.conv1(input)
        x = self.bn1(x)
        x = F.relu(x)
        x = self.pool1(x)
        x = self.conv2(x)
        x = self.bn2(x)
        x = F.relu(x)
        x = self.pool2(x)
        x = self.conv3(x)
        x = self.bn3(x)
        x = F.relu(x)
        x = F.dropout(x, 0.3)
        x = self.pool3(x)
        x = self.conv4(x)
        x = self.bn4(x)
        x = F.relu(x)
        x = self.pool4(x)
        x = torch.flatten(x, 1)
        x = self.linear0(x)

        return x


if __name__ == '__main__':
    net = Network(20)
    input = torch.rand(32, 3, 224, 224)
    out = net(input)
