"""
可视化cnn的feature
"""
import os
import numpy as np

import torch
from torch.optim import Adam
from torchvision import models

import cnn_layer_visualization as cnn_layer
import deep_dream as deep_dream
import parameters as hyper

def plot_cnn_visualize(model):
    '''
    调用cnn_layer_visualization进行可视化
    '''
    for i in range(hyper.channel_0):
        pretrained_model = model
        layer_vis = cnn_layer.CNNLayerVisualization(pretrained_model, 0, i)
        layer_vis.visualise_layer_with_hooks()

    for i in range(hyper.channel_1):
        pretrained_model = model
        layer_vis = cnn_layer.CNNLayerVisualization(pretrained_model, 2, i)
        layer_vis.visualise_layer_with_hooks()

    for i in range(hyper.channel_2):
        pretrained_model = model
        layer_vis = cnn_layer.CNNLayerVisualization(pretrained_model, 3, i)
        layer_vis.visualise_layer_with_hooks()

def plot_deep_dream(model):
    '''
    调用deepdream进行可视化
    '''
    for j in range(15, 16):
        im_path = "../data/valid/" + str(j) + "/0000.jpg"
        save_name = str(j)
        for i in range(hyper.channel_0):
            pretrained_model = model
            dd = deep_dream.DeepDream(pretrained_model, 0, i, im_path, save_name)
            dd.dream()
        for i in range(hyper.channel_1):
            pretrained_model = model
            dd = deep_dream.DeepDream(pretrained_model, 2, i, im_path, save_name)
            dd.dream()

        for i in range(hyper.channel_2):
            pretrained_model = model
            dd = deep_dream.DeepDream(pretrained_model, 3, i, im_path, save_name)
            dd.dream()
    
