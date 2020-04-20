import torch
import torch.nn as nn
import numpy as np
from torch.autograd import Variable


class QNetwork(nn.Module):
    def __init__(self, layer_num, state_num=115, action_num=20, device='cpu'):
        super().__init__()
        self.device = device
        self.model = [
            nn.Linear(state_num, 512),
            nn.ReLU(inplace=True)]
        for i in range(layer_num):
            self.model += [nn.Linear(512, 512), nn.ReLU(inplace=True)]
        if action_num:
            self.model += [nn.Linear(512, action_num)]
        self.model = nn.Sequential(*self.model)

    def forward(self, s):
        if not isinstance(s, torch.Tensor):
            s = torch.tensor(s, device=self.device, dtype=torch.float)
        logits = self.model(s.cuda())
        return logits


class DuelQNetwork(nn.Module):
    def __init__(self, layer_num, state_num=115, action_num=20, device='cpu'):
        super().__init__()
        self.device = device
        self.model = [
            nn.Linear(state_num, 512),
            nn.ReLU(inplace=True)]
        for i in range(layer_num):
            self.model += [nn.Linear(512, 512), nn.ReLU(inplace=True)]

        self.out1 = nn.Linear(512, action_num)
        self.out2 = nn.Linear(512, 1)

        self.model = nn.Sequential(*self.model)

    def forward(self, s):
        if not isinstance(s, torch.Tensor):
            s = torch.tensor(s, device=self.device, dtype=torch.float)
        s.to(self.device)
        h = self.model(s.cuda())
        V = self.out1(h)
        A = self.out2(h)
        logits = V.expand_as(A) + (A - A.mean(1).expand_as(A))
        return logits


class DQN(object):
    def __init__(self, layer_num, batch_size=32, state_num=115, action_num=20, device='cpu', memory_capacity=320,
                 update_freq=100
                 , eps=0.95, gamma=0.9, lr=0.01, Dueling=None):
        self.batch_size = batch_size
        if not Dueling:
            self.online_net = QNetwork(layer_num, state_num, action_num, device).to(device)
            self.target_net = QNetwork(layer_num, state_num, action_num, device).to(device)
        self.memory_capacity = memory_capacity
        self.state_num = state_num
        self.action_num = action_num
        self.update_freq = update_freq
        self.learn_step = 0
        self.memory_pts = 0
        self.eps = eps
        self.gamma = gamma
        self.memory = np.zeros((memory_capacity, state_num * 2 + 2))
        self.optimizer = torch.optim.Adam(self.online_net.parameters(), lr=lr)
        self.loss = nn.MSELoss()

    def choose_action(self, s):
        s = Variable(torch.unsqueeze(torch.FloatTensor(s), 0))
        if np.random.uniform() < self.eps:
            logits = self.online_net.forward(s)
            action = torch.argmax(logits)
        else:  # random
            action = np.random.randint(0, self.action_num)
        return action

    def store_transition(self, s, a, r, next_s):
        trans = np.hstack((s, [a, r], next_s))

        i = self.memory_pts % self.memory_capacity
        # save the info_this_step into memory
        self.memory[i, :] = trans
        self.memory_pts += 1

    def learn(self):
        # target parameter update
        if self.learn_step % self.update_freq == 0:
            self.target_net.load_state_dict(self.online_net.state_dict())
        self.learn_step += 1

        sample_index = np.random.choice(self.memory_capacity, self.batch_size)
        batch_memory = self.memory[sample_index, :]

        # in the memory, the 1st---4th column is state_now , the 5th is action , the 6th is reward
        # the final 4 column is state_next
        batch_s = Variable(torch.FloatTensor(batch_memory[:, :self.state_num])).cuda()
        batch_a = Variable(torch.LongTensor(batch_memory[:, self.state_num:self.state_num + 1].astype(int))).cuda()
        batch_r = Variable(torch.FloatTensor(batch_memory[:, self.state_num + 1:self.state_num + 2])).cuda()
        batch_next_s = Variable(torch.FloatTensor(batch_memory[:, -self.state_num:])).cuda()
        # q_eval w.r.t the action in experience
        q_eval = self.online_net(batch_s).gather(1, batch_a)  # shape (batch, 1)
        q_next = self.target_net(batch_next_s).detach()  # detach from graph, don't backpropagate

        q_target = batch_r + self.gamma * q_next.max(1)[0].unsqueeze(1)  # shape (batch, 1)
        loss = self.loss(q_eval, q_target)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        return loss.item()


class DoubleDQN(DQN):

    def __init__(self, layer_num, batch_size=32, state_num=115, action_num=20, device='cpu', memory_capacity=320,
                 update_freq=100
                 , eps=0.95, gamma=0.9, lr=0.01):
        super(DoubleDQN, self).__init__(layer_num, batch_size, state_num, action_num, device, memory_capacity,
                                        update_freq
                                        , eps, gamma, lr)

    def learn(self):
        # target parameter update
        if self.learn_step % self.update_freq == 0:
            self.target_net.load_state_dict(self.online_net.state_dict())
        self.learn_step += 1

        # by random , choose the row's number from memory_capacity , total row's number is batch_size(32)
        sample_index = np.random.choice(self.memory_capacity, self.batch_size)
        batch_memory = self.memory[sample_index, :]

        # in the memory, the 1st---4th column is state_now , the 5th is action , the 6th is reward
        # the final 4 column is state_next
        batch_s = Variable(torch.FloatTensor(batch_memory[:, :self.state_num])).cuda()
        batch_a = Variable(torch.LongTensor(batch_memory[:, self.state_num:self.state_num + 1].astype(int))).cuda()
        batch_r = Variable(torch.FloatTensor(batch_memory[:, self.state_num + 1:self.state_num + 2])).cuda()
        batch_next_s = Variable(torch.FloatTensor(batch_memory[:, -self.state_num:])).cuda()

        # q_eval w.r.t the action in experience
        q_eval = self.online_net(batch_s).gather(1, batch_a)  # shape (batch, 1)

        q_eval_test = self.online_net(batch_s)
        # argmax axis = 0 means column , 1 means row
        # we choose the max acion value , the action is column , so axis = 1
        q_argmax = torch.argmax(q_eval_test, axis=1)

        q_next = self.target_net(batch_next_s).detach()

        q_update = torch.zeros((self.batch_size, 1))
        for i in range(self.batch_size):
            q_update[i] = q_next[i, q_argmax[i]]

        q_update = self.gamma * q_update
        q_update = Variable(torch.FloatTensor(q_update)).cuda()

        q_target = batch_r + q_update
        loss = self.loss(q_eval, q_target)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        return loss.item()


class DuelingDQN(DQN):

    def __init__(self, layer_num, batch_size=32, state_num=115, action_num=20, device='cpu', memory_capacity=320,
                 update_freq=100
                 , eps=0.95, gamma=0.9, lr=0.01):
        self.online_net = QNetwork(layer_num, state_num, action_num, device).to(device)
        self.target_net = QNetwork(layer_num, state_num, action_num, device).to(device)
        super(DuelingDQN, self).__init__(layer_num, batch_size, state_num, action_num, device, memory_capacity,
                                         update_freq
                                         , eps, gamma, lr)

