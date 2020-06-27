'''
正常的RNN模型
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
    def __init__(self, device, network_type, dropout_type, init_type, weight_norm, nvoc,ninput, nhid, nlayers):
        super(LMModel, self).__init__()
        self.drop = nn.Dropout(0.5)
        self.encoder = nn.Embedding(nvoc, ninput)
        # WRITE CODE HERE witnin two '#' bar
        ########################################
        # Construct you RNN model here. You can add additional parameters to the function.
        self.ninput = ninput
        self.device = device
        self.nhid = nhid
        self.nlayers = nlayers
        self.dropout_type = dropout_type
        self.init_type = init_type
        self.network_type = network_type
        self.weight_norm = weight_norm
        
        #choose network type
        if network_type == constants.network_gru:
            self.rnn = nn.GRU(input_size = ninput, hidden_size = nhid, num_layers = nlayers)
        elif network_type == constants.network_lstm:
            self.rnn = nn.LSTM(input_size = ninput, hidden_size = nhid, num_layers = nlayers)
        elif network_type == constants.network_rnn:
            self.rnn = nn.RNN(input_size = ninput, hidden_size = nhid, num_layers = nlayers)

        ########################################
        self.decoder = nn.Linear(nhid, nvoc)
        self.init_weights()

        

    def init_weights(self):
        init_uniform = 0.1
        self.encoder.weight.data.uniform_(-init_uniform, init_uniform)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-init_uniform, init_uniform)     
        
        #initialize weight(orthogonal init or original uniform init)
        if self.init_type == constants.init_orth and self.network_type == constants.network_gru and self.nlayers == 2:
            #print(self.rnn)
            torch.nn.init.orthogonal_(self.rnn.weight_ih_l0.data)
            torch.nn.init.orthogonal_(self.rnn.weight_hh_l0.data)
            torch.nn.init.orthogonal_(self.rnn.weight_ih_l1.data)
            torch.nn.init.orthogonal_(self.rnn.weight_hh_l1.data)
            self.rnn.bias_ih_l0.data.zero_()
            self.rnn.bias_hh_l0.data.zero_()
            self.rnn.bias_ih_l1.data.zero_()
            self.rnn.bias_hh_l1.data.zero_()

    def forward(self, input):
        embeddings = self.drop(self.encoder(input))
        
        # WRITE CODE HERE within two '#' bar
        ########################################
        # With embeddings, you can get your output here.
        # Output has the dimension of sequence_length * batch_size * number of classes
        output, hidden = self.rnn(embeddings)
        ########################################
        
        #dropout or not
        if self.dropout_type == constants.dropout_true:
            output = self.drop(output)
            
            
        decoded = self.decoder(output.view(output.size(0)*output.size(1), output.size(2)))
        return decoded.view(output.size(0), output.size(1), decoded.size(1)), hidden
    
