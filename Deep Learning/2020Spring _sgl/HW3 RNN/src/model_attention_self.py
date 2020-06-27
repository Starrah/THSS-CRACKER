'''
加入self attention机制的模型
'''

import torch
import torch.nn as nn
import torch.nn.functional as F
import math
import numpy as np
import constants

class LMModel(nn.Module):
    # Language model is composed of three parts: a word embedding layer, a rnn network and a output layer. 
    # The word embedding layer have input as a sequence of word index (in the vocabulary) and output a sequence of vector where each one is a word embedding. 
    # The rnn network has input of each word embedding and output a hidden feature corresponding to each word embedding.
    # The output layer has input as the hidden feature and output the probability of each word in the vocabulary.
    def __init__(self, device, network_type, nvoc,ninput, nhid, nlayers, nsql):
        super(LMModel, self).__init__()
        self.drop = nn.Dropout(0.5)
        self.encoder = nn.Embedding(nvoc, ninput)
        self.device = device
        self.network_type = network_type
        # WRITE CODE HERE witnin two '#' bar
        ########################################
        # Construct you RNN model here. You can add additional parameters to the function.
        self.ninput = ninput
        self.nhid = nhid
        self.nlayers = nlayers
        self.nsql = nsql
        self.rnn = nn.GRU(input_size = ninput, hidden_size = nhid, num_layers = nlayers)
        self.attention = nn.Linear(ninput, ninput)
        
        ########################################
        self.decoder = nn.Linear(nhid, nvoc)
        self.init_weights()


    def init_weights(self):
        init_uniform = 0.1
        self.encoder.weight.data.uniform_(-init_uniform, init_uniform)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-init_uniform, init_uniform)
        self.attention.bias.data.zero_()
        self.attention.weight.data.uniform_(-init_uniform, init_uniform)

    def forward(self, input):
        embeddings = self.drop(self.encoder(input))
        
        # WRITE CODE HERE within two '#' bar
        ########################################
        # With embeddings, you can get your output here.
        # Output has the dimension of sequence_length * batch_size * number of classes

        #计算attention score
        if self.network_type == constants.network_attention_self_matrix or self.network_type == constants.network_attention_self_matrix_modified:
            middle = self.attention(embeddings.permute(1, 0, 2))
            attention_score = torch.bmm(middle, embeddings.permute(1, 2, 0))
            attention_score = attention_score / math.sqrt(self.nsql)
        else:
            attention_score = torch.bmm(embeddings.permute(1, 0, 2), embeddings.permute(1, 2, 0)) / math.sqrt(self.nsql)
        
        
        if self.network_type == constants.network_attention_self_modified or self.network_type == constants.network_attention_self_matrix_modified:
            #取下三角部分，保证后面的元素不会影响前面的元素
            #上三角部分都是-inf，用于移除i > j情况
            nega_inf = torch.zeros(attention_score.size(0), attention_score.size(1), attention_score.size(2)).to(self.device)
            nega_inf = nega_inf - np.inf
            nega_inf = torch.triu(nega_inf, diagonal=1)
            attention_score = attention_score + nega_inf
        
        #求得attention，并且代入求得新的输入
        attention_score = F.softmax(attention_score, dim=1)
        new_input = torch.bmm(attention_score, embeddings.permute(1, 0, 2))
        new_input = new_input.permute(1, 0, 2)

        #代入RNN
        output, hidden = self.rnn(new_input)
        ########################################

        output = self.drop(output)
        decoded = self.decoder(output.view(output.size(0)*output.size(1), output.size(2)))
        return decoded.view(output.size(0), output.size(1), decoded.size(1)), hidden
    
