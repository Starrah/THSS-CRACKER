import torch
import torch.nn as nn
import torch.nn.functional as F


class LMModel(nn.Module):
    # Language model is composed of three parts: a word embedding layer, a rnn network and a output layer. 
    # The word embedding layer have input as a sequence of word index (in the vocabulary) and output a sequence of vector where each one is a word embedding. 
    # The rnn network has input of each word embedding and output a hidden feature corresponding to each word embedding.
    # The output layer has input as the hidden feature and output the probability of each word in the vocabulary.
    def __init__(self, nvoc, ninput, nhid, nlayers, rnn_type='gru', bidirectional=False, bias=True,
                 temporal_attention=False, self_attention=False, device='cpu'):
        super(LMModel, self).__init__()
        self.drop = nn.Dropout(0.3)
        self.encoder = nn.Embedding(nvoc, ninput)
        # WRITE CODE HERE witnin two '#' bar
        ########################################
        # Construct you RNN model here. You can add additional parameters to the function.
        flag = 1 if bidirectional is False else 2

        if rnn_type == 'rnn':
            self.rnn = nn.RNN(ninput, nhid, nlayers, bidirectional=bidirectional, bias=bias)
        elif rnn_type == 'gru':
            self.rnn = nn.GRU(ninput, nhid, nlayers, bidirectional=bidirectional, bias=bias)
        elif rnn_type == 'lstm':
            self.rnn = nn.LSTM(ninput, nhid, nlayers, bidirectional=bidirectional, bias=bias)
        else:
            raise NotImplementedError

        if temporal_attention:
            self.conv = [nn.Conv1d(ninput, ninput, kernel_size=1).to(device),
                         nn.Conv1d(ninput, ninput, kernel_size=3).to(device),
                         nn.Conv1d(ninput, ninput, kernel_size=5).to(device)]
        if self_attention:
            self.fc_attn = nn.Linear(nhid, nhid)
            self.v = nn.Parameter(torch.rand(nhid), requires_grad=True)

        ########################################
        self.decoder = nn.Linear(nhid * flag, nvoc)
        self.ta = temporal_attention
        self.sa = self_attention
        self.init_weights()
        self.nhid = nhid
        self.nlayers = nlayers
        self.device = device

    def init_weights(self):
        init_uniform = 0.1
        if self.ta:
            [x.weight.data.uniform_(-init_uniform, init_uniform) for x in self.conv]

        self.encoder.weight.data.uniform_(-init_uniform, init_uniform)
        self.decoder.bias.data.zero_()
        self.decoder.weight.data.uniform_(-init_uniform, init_uniform)
        if self.sa:
            self.fc_attn.weight.data.uniform_(-init_uniform, init_uniform)

    def init_weights_orthogonal(self):
        for m in self.modules():
            if isinstance(m, (nn.Embedding, nn.Linear, nn.Conv1d)):
                nn.init.orthogonal(m.weight)

    def forward(self, input, hidden):
        embeddings = self.drop(self.encoder(input))

        # WRITE CODE HERE within two '#' bar
        ########################################
        # With embeddings, you can get your output here.
        # Output has the dimension of sequence_length * batch_size * number of classes

        if self.ta:
            embeddings = self.temporal_attention(embeddings)

        output, hidden = self.rnn(embeddings, hidden)
        if self.sa:
            z = torch.tanh(self.fc_attn(output))
            z = z.permute(1, 2, 0)  # [B*H*T]
            v = self.v.repeat(output.size(1), 1).unsqueeze(1)  # [B*1*H]
            z = torch.bmm(v, z).squeeze(1)  # [B*T]
            attn_scores = F.softmax(z, dim=1)
            attn_scores = attn_scores.transpose(0, 1).flatten().unsqueeze(1)

            output = output.view(-1, self.nhid) * attn_scores

        ########################################

        output = self.drop(output)
        if self.sa:
            decoded = self.decoder(output)
        else:
            decoded = self.decoder(output.view(output.size(0) * output.size(1), output.size(2)))
        return decoded, hidden

    def init_hidden(self, use_gpu, args):

        weight = next(self.parameters()).data
        flag = 1 if args.bidirectional is False else 2
        if use_gpu:
            hidden = (
                weight.new(self.nlayers * flag, args.train_batch_size, self.nhid).zero_().to(
                    'cuda:' + str(args.gpu_id)),
                weight.new(self.nlayers * flag, args.eval_batch_size, self.nhid).zero_().to('cuda:' + str(args.gpu_id)))
        else:
            hidden = (weight.new(self.nlayers * flag, args.train_batch_size, self.nhid).zero_(),
                      weight.new(self.nlayers * flag, args.eval_batch_size, self.nhid).zero_())

        return hidden

    def temporal_attention(self, x):
        seq_len, batch_size, ninput = x.shape
        if seq_len < 5:
            return x
        x = x.permute(1, 2, 0)
        x_list = [F.relu(conv(x)) for conv in self.conv]
        xx_list = [x.view(-1, batch_size, ninput) for x in x_list]
        xx_list[1] = torch.cat(
            [torch.zeros((1, batch_size, ninput)).to(self.device), torch.zeros((1, batch_size, ninput)).to(self.device),
             xx_list[1]], 0)
        xx_list[2] = torch.cat(
            [torch.zeros((1, batch_size, ninput)).to(self.device), torch.zeros((1, batch_size, ninput)).to(self.device),
             torch.zeros((1, batch_size, ninput)).to(self.device), torch.zeros((1, batch_size, ninput)).to(self.device),
             xx_list[2]], 0)
        x = xx_list[0] + xx_list[1] + xx_list[2]
        return x
