# coding: utf-8
import argparse
import torch
import torch.nn as nn
import data
import model
from tensorboardX import SummaryWriter
from datetime import datetime

parser = argparse.ArgumentParser(description='PyTorch ptb Language Model')
parser.add_argument('--epochs', type=int, default=1000,
                    help='upper epoch limit')
parser.add_argument('--train_batch_size', type=int, default=150, metavar='N',
                    help='batch size')
parser.add_argument('--eval_batch_size', type=int, default=10, metavar='N',
                    help='eval batch size')
parser.add_argument('--max_sql', type=int, default=35,
                    help='sequence length')
parser.add_argument('--seed', type=int, default=1234,
                    help='set random seed')
parser.add_argument('--cuda', action='store_true', help='use CUDA device')
parser.add_argument('--gpu_id', default=0, type=int, help='GPU device id used')
parser.add_argument('--lr', default=8e-3, type=float, help='learning rate')

parser.add_argument('--nvoc', type=int, default=10000)
parser.add_argument('--ninput', type=int, default=256)
parser.add_argument('--nhid', type=int, default=512)
parser.add_argument('--nlayers', type=int, default=1)
parser.add_argument('--rnn_type', type=str, default='rnn')
parser.add_argument('--save', type=bool, default=False, help='use tensorboard to save curves')
parser.add_argument('--bidirectional', type=bool, default=False, help='use bidirectional rnn')
parser.add_argument('--bias', type=bool, default=True, help='bias for rnn')
parser.add_argument('--orthogonal', type=bool, default=True, help='use orthogonal initialization')
parser.add_argument('--temporal_attention', type=bool, default=False, help='use conv as temporal attention mechanism')
parser.add_argument('--self_attention', type=bool, default=False, help='use self attention mechanism')

args = parser.parse_args()

# Set the random seed manually for reproducibility.
torch.manual_seed(args.seed)

# Use gpu or cpu to train
use_gpu = True

if use_gpu:
    device = torch.device('cuda:' + str(args.gpu_id))
else:
    device = torch.device("cpu")

# load data
train_batch_size = args.train_batch_size
eval_batch_size = args.eval_batch_size
batch_size = {'train': train_batch_size, 'valid': eval_batch_size}
data_loader = data.Corpus("../data/ptb", batch_size, args.max_sql)

# WRITE CODE HERE within two '#' bar
########################################
# Build LMModel model (bulid your language model here)
net = model.LMModel(nvoc=args.nvoc, ninput=args.ninput, nhid=args.nhid, nlayers=args.nlayers,
                    rnn_type=args.rnn_type, bidirectional=args.bidirectional, bias=args.bias,
                    temporal_attention=args.temporal_attention, self_attention=args.self_attention, device=device).to(device)

if args.orthogonal:
    net.init_weights_orthogonal()

if args.save:

    writer = SummaryWriter(
        'logdir/' + args.rnn_type + '_bidirectional_' + str(args.bidirectional) + '_bias_' + str(
            args.bias) + '_orthogonal_' + str(args.orthogonal) + '_' + datetime.now().strftime(
            '%b%d_%H-%M-%S'))
    '''
    writer = SummaryWriter(
        'logdir/' + args.rnn_type + '_' + datetime.now().strftime(
            '%b%d_%H-%M-%S'))
    '''

train_hidden, eval_hidden = net.init_hidden(use_gpu, args)
if args.rnn_type == 'lstm':
    train_c, eval_c = net.init_hidden(use_gpu, args)
    train_hidden = (train_hidden, train_c)
    eval_hidden = (eval_hidden, eval_c)
optimizer = torch.optim.Adam(net.parameters(), args.lr)
########################################

criterion = nn.CrossEntropyLoss()


# WRITE CODE HERE within two '#' bar
########################################
# Evaluation Function
# Calculate the average cross-entropy loss between the prediction and the ground truth word.
# And then exp(average cross-entropy loss) is perplexity.

def evaluate(eval_hidden, epoch):
    net.eval()
    data_loader.set_valid()
    data, labels, end = data_loader.get_batch()
    data = data.to(device)
    labels = labels.to(device)

    output, eval_hidden = net(data, eval_hidden)
    loss = criterion(output, labels)
    pp = torch.exp(loss)
    if args.save:
        writer.add_scalar('eval loss', loss, epoch)
        writer.add_scalar('eval pp', pp, epoch)

    return eval_hidden, loss, pp


########################################


# WRITE CODE HERE within two '#' bar
########################################
# Train Function
def train(train_hidden, epoch):
    net.train()
    data_loader.set_train()
    data, labels, end = data_loader.get_batch()
    data = data.to(device)
    labels = labels.to(device)

    output, train_hidden = net(data, train_hidden)
    optimizer.zero_grad()
    loss = criterion(output, labels)
    loss.backward()
    optimizer.step()

    pp = torch.exp(loss)
    if args.save:
        writer.add_scalar('train loss', loss, epoch)
        writer.add_scalar('train pp', pp, epoch)
    if args.rnn_type == 'lstm':
        train_hidden = (train_hidden[0].detach(), train_hidden[1].detach())
    else:
        train_hidden = train_hidden.detach()
    return train_hidden, loss, pp


########################################

if __name__ == '__main__':

    # Loop over epochs.
    for epoch in range(1, args.epochs + 1):
        train_hidden, loss, pp = train(train_hidden, epoch)
        eval_hidden, val_loss, val_pp = evaluate(eval_hidden, epoch)

        print("Epoch: {}/{}...".format(epoch, args.epochs),
              "Loss: {:.6f}...".format(loss.item()),
              "Val Loss: {:.6f}".format(val_loss.item()),
              "PP: {:.6f}...".format(pp.item()),
              "Val PP: {:.6f}".format(val_pp.item()))
