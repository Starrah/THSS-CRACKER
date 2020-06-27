'''
可视化的主函数
'''
import plot_curves
import plot_confusion_matrix
import plot_tsne
import plot_cnn_visualize
from torchvision import models
import torch
import torch.nn as nn
import data
import parameters as hyper


if __name__ == '__main__':
    #读取模型，数据
    model_name = "C"
    model = torch.load("../result/models/best_model.pt")
    _, valid_loader, _ = data.load_data \
    (data_dir = hyper.data_dir,input_size = hyper.input_size)
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    
    the_model = nn.Sequential(*list(model.children()))[:]

    plot_cnn_visualize.plot_cnn_visualize(the_model)
    plot_cnn_visualize.plot_deep_dream(the_model)
    model = model.to(device)
    plot_confusion_matrix.plot_matrix(model_name, model, valid_loader, device)
    plot_tsne.plot_tsne(model_name, model, valid_loader, device)
    
    new_model_name = "C_2layers"
    new_model = torch.load("../result/models/best_model_2layer.pt")
    new_model = new_model.to(device)
    plot_tsne.plot_tsne(new_model_name, new_model, valid_loader, device)

