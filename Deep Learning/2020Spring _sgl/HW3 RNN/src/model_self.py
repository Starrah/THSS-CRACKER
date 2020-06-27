'''
自己实现的GRU模型
'''

import torch
import torch.nn as nn
import math
import constants

class LMModel(nn.Module):
    # Language model is composed of three parts: a word embedding layer, a rnn network and a output layer. 
    # The word embedding layer have input as a sequence of word index (in the vocabulary) and output a sequence of vector where each one is a word embedding. 
    # The rnn network has input of each word embedding and output a hidden feature corresponding to each word embedding.
    # The output layer has input as the hidden feature and output the probability of each word in the vocabulary.
    def __init__(self, device, network_type, nvoc, ninput):
        super(LMModel, self).__init__()
        self.drop = nn.Dropout(0.5)
        self.encoder = nn.Embedding(nvoc, ninput)
        # WRITE CODE HERE witnin two '#' bar
        ########################################
        # Construct you RNN model here. You can add additional parameters to the function.
        #ninput must = nhid
        self.ninput = ninput
        self.device = device
        self.network_type = network_type
        #self.rnn = nn.GRU(input_size = ninput, hidden_size = nhid, num_layers = nlayers)
        #第一层全连接
        self.sigmoid = nn.Sigmoid()
        self.tanh = nn.Tanh()
        self.xr1 = nn.Linear(in_features = ninput, out_features = ninput)
        self.hr1 = nn.Linear(in_features = ninput, out_features = ninput)
        self.xz1 = nn.Linear(in_features = ninput, out_features = ninput)        
        self.hz1 = nn.Linear(in_features = ninput, out_features = ninput)
        self.xc1 = nn.Linear(in_features = ninput, out_features = ninput)
        self.hc1 = nn.Linear(in_features = ninput, out_features = ninput)


        #第二层全连接
        self.xr2 = nn.Linear(in_features = ninput, out_features = ninput)
        self.hr2 = nn.Linear(in_features = ninput, out_features = ninput)
        self.xz2 = nn.Linear(in_features = ninput, out_features = ninput)        
        self.hz2 = nn.Linear(in_features = ninput, out_features = ninput)
        self.xc2 = nn.Linear(in_features = ninput, out_features = ninput)
        self.hc2 = nn.Linear(in_features = ninput, out_features = ninput)
        ########################################
        self.decoder = nn.Linear(ninput, nvoc)
        self.init_weights()


    def init_weights(self):
        init_uniform = 0.1
        self.encoder.weight.data.uniform_(-init_uniform, init_uniform)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-init_uniform, init_uniform)

    def rnn_forward(self, sequence_length, batch_size, input):
        '''
        自己实现rnn（2层GRU）的前向传播
        '''
        h1 = torch.rand(batch_size, self.ninput)
        h2 = torch.rand(batch_size, self.ninput)
        h1 = h1.to(self.device)
        h2 = h2.to(self.device)

        output = None
        for i in range(sequence_length):
            #计算第一层
            input_current = input[i]
            r_1_current = self.sigmoid(self.xr1(input_current) \
            + self.hr1(h1))
            z_1_current = self.sigmoid(self.xz1(input_current) \
            + self.hz1(h1))
            hrough_1_current = self.tanh(self.xc1(input_current) \
            + self.hc1(torch.mul(r_1_current, h1)))
            h1 = torch.mul(1 - z_1_current, h1) + torch.mul(z_1_current, hrough_1_current)
            
            #计算第二层
            input_current = h1
            r_2_current = self.sigmoid(self.xr2(input_current) \
            + self.hr2(h2))
            z_2_current = self.sigmoid(self.xz2(input_current) \
            + self.hz2(h2))
        
            hrough_2_current = self.tanh(self.xc2(input_current) \
            + self.hc2(torch.mul(r_2_current, h2)))
            h2 = torch.mul(1 - z_2_current, h2) + torch.mul(z_2_current, hrough_2_current)
        
            #结果拼接
            if i == 0:
                output = h2.view(1, h2.size(0), h2.size(1))
            else:
                output_raw = h2.view(1, h2.size(0), h2.size(1))
                output = torch.cat((output, output_raw),0)
        return output
    
    
    def layer_norm(self, x):
        '''
        Layer Normalization操作
        '''
        xmean = x.mean(1)
        xmean = xmean.view(xmean.size(0), 1)
        xmean = xmean.expand(x.size())
        xstd = x.std(1)
        xstd = xstd.view(xstd.size(0), 1)
        xstd = xstd.expand(x.size())
        eps = 1e-5
        return ((x - xmean) / (xstd + eps))
        
    
    def rnn_forward_layer_norm(self, sequence_length, batch_size, input):
        '''
        带layer norm的rnn的前向传播
        '''
        h1 = torch.rand(batch_size, self.ninput)
        h2 = torch.rand(batch_size, self.ninput)
        h1 = h1.to(self.device)
        h2 = h2.to(self.device)
        output = None
        for i in range(sequence_length):
            #计算第一层
            input_current = input[i]
            r_1_current = self.sigmoid(self.layer_norm(self.xr1(input_current)) \
            + self.layer_norm(self.hr1(h1)))
            z_1_current = self.sigmoid(self.layer_norm(self.xz1(input_current)) \
            + self.layer_norm(self.hz1(h1)))
            hrough_1_current = self.tanh(self.layer_norm(self.xc1(input_current)) \
            + self.layer_norm(self.hc1(torch.mul(r_1_current, h1))))
            h1 = torch.mul(1 - z_1_current, h1) + torch.mul(z_1_current, hrough_1_current)
            #计算第二层
            input_current = h1
            r_2_current = self.sigmoid(self.layer_norm(self.xr2(input_current)) \
            + self.layer_norm(self.hr2(h2)))
            z_2_current = self.sigmoid(self.layer_norm(self.xz2(input_current)) \
            + self.layer_norm(self.hz2(h2)))
        
            hrough_2_current = self.tanh(self.layer_norm(self.xc2(input_current)) \
            + self.layer_norm(self.hc2(torch.mul(r_2_current, h2))))
            h2 = torch.mul(1 - z_2_current, h2) + torch.mul(z_2_current, hrough_2_current)
        
            #结果拼接
            if i == 0:
                output = h2.view(1, h2.size(0), h2.size(1))
            else:
                output_raw = h2.view(1, h2.size(0), h2.size(1))
                output = torch.cat((output, output_raw),0)
        return output

    def forward(self, input):
        embeddings = self.drop(self.encoder(input))
        
        # WRITE CODE HERE within two '#' bar
        ########################################
        # With embeddings, you can get your output here.
        # Output has the dimension of sequence_length * batch_size * number of classes
        sequence_length = input.size(0)
        batch_length = input.size(1)
        
        #判断是否进行layer norm
        if self.network_type == constants.network_layer_norm:
            output = self.rnn_forward_layer_norm(sequence_length, batch_length, embeddings)
        else:
            output = self.rnn_forward(sequence_length, batch_length, embeddings)
        ########################################

        output = self.drop(output)
        decoded = self.decoder(output.view(output.size(0)*output.size(1), output.size(2)))
        return decoded.view(output.size(0), output.size(1), decoded.size(1)), None
    
