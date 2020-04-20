# coding='utf-8'

from time import time
import numpy as np
import matplotlib.pyplot as plt
import data
from arguments import get_args
from sklearn import datasets
from sklearn.manifold import TSNE
from models import Network
import torch
import torch.nn.functional as F


def plot_embedding(data, label, title):
    x_min, x_max = np.min(data, 0), np.max(data, 0)
    data = (data - x_min) / (x_max - x_min)

    for i in range(data.shape[0]):
        plt.text(data[i, 0], data[i, 1], str(label[i]),
                 color=plt.cm.Set1(label[i] / 10.),
                 fontdict={'weight': 'bold', 'size': 9})
    plt.xticks([])
    plt.yticks([])
    plt.title(title)


def forward(model, input):
    x = model.conv1(input)
    x = model.bn1(x)
    x = F.relu(x)
    x = model.pool1(x)
    x = model.conv2(x)
    x = model.bn2(x)
    x = F.relu(x)
    x = model.pool2(x)
    x = model.conv3(x)
    x = model.bn3(x)
    x = F.relu(x)
    x = model.pool3(x)
    x = model.conv4(x)
    x = model.bn4(x)
    x = F.relu(x)
    x = model.pool4(x)
    x = torch.flatten(x, 1)
    x = model.linear0(x)
    return x


def main():
    args = get_args()

    data_dir = "../data/"
    ## data preparation
    _, valid_loader = data.load_data(data_dir=data_dir, input_size=224, batch_size=args.batch_size,
                                     augmentation=args.augmentation)
    print('Computing t-SNE embedding')
    tsne = TSNE(n_components=2)
    t0 = time()
    pretrained_model = Network(20).to(args.device)
    pretrained_model.load_state_dict(torch.load('tsne.pt'))
    outputs = []
    label_list = []
    for inputs, labels in valid_loader:
        inputs = inputs.to(args.device)
        output = forward(pretrained_model, inputs)
        outputs.append(output.cpu().detach().numpy().astype(np.float64))
        label_list.append(labels)
    output = np.concatenate(outputs, axis=0)
    labels = np.concatenate(label_list, axis=0)
    result = tsne.fit_transform(output)

    plot_embedding(result, labels,
                   't-SNE embedding of the 20 classes (time %.2fs)'
                   % (time() - t0))


if __name__ == '__main__':
    ax = plt.subplot(111)
    main()
    plt.savefig('tsne.png')
