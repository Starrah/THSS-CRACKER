import torch
import torch.nn as nn
import numpy as np
from torch.autograd import Variable
import torch.optim as optim
import torch.nn.functional as F

class Net(nn.Module):
    def __init__(self, layer_num, state_num=115, device='cpu', orthogonal=False):
        super().__init__()
        self.device = device
        self.model = [
            nn.Linear(state_num, 512).to(device),
            nn.ReLU(inplace=True).to(device)]
        for i in range(layer_num):
            l = nn.Linear(512, 512).to(device)
            if orthogonal:
                nn.init.orthogonal(l.weight)
            self.model += [l, nn.ReLU(inplace=True).to(device)]
        self.model = nn.Sequential(*self.model)

    def forward(self, s):
        if not isinstance(s, torch.Tensor):
            s = torch.tensor(s, device=self.device, dtype=torch.float)
        logits = self.model(s.to(self.device))
        return logits


class Actor(nn.Module):
    def __init__(self, layer_num, state_num=115, action_num=19, device='cpu', orthogonal=False):
        super().__init__()
        self.preprocess = Net(layer_num, state_num, device, orthogonal)
        self.last = nn.Linear(512, action_num).to(device)
        if orthogonal:
            nn.init.orthogonal(self.last.weight)

    def forward(self, s, noise=None):
        logits = self.last(self.preprocess(s))
        return logits


class Critic(nn.Module):
    def __init__(self, layer_num, state_num=115, device='cpu', orthogonal=False):
        super().__init__()
        self.preprocess = Net(layer_num, state_num, device, orthogonal)
        self.last = nn.Linear(512, 1).to(device)
        if orthogonal:
            nn.init.orthogonal(self.last.weight)

    def forward(self, s, a=None):
        if a is not None:
            if len(a.shape) is 1:
                a = a.unsqueeze(1)
            s = torch.cat([s, a], 1)
        logits = self.preprocess(s)
        logits = self.last(logits)
        return logits


class DDPG(object):
    def __init__(self, batch_size=32, state_num=115, action_num=19, device='cpu', memory_capacity=100000,
                 eps=0.1, gamma=0.9, critic_lr=0.0001, actor_lr=0.0001, tau=0.005):

        self.actor = Actor(3, state_num, action_num, device)
        self.actor_target = Actor(3, state_num, action_num, device)
        self.critic = Critic(3, state_num + action_num, device)
        self.critic_target = Critic(3, state_num + action_num, device)
        self.actor_optim = optim.Adam(self.actor.parameters(), lr=actor_lr)
        self.critic_optim = optim.Adam(self.critic.parameters(), lr=critic_lr)
        self.actor_target.load_state_dict(self.actor.state_dict())
        self.critic_target.load_state_dict(self.critic.state_dict())
        self.tau = tau
        self.batch_size = batch_size
        self.memory_capacity = memory_capacity
        self.state_num = state_num
        self.action_num = action_num
        self.learn_step = 0
        self.memory_pts = 0
        self.eps = eps
        self.gamma = gamma
        self.memory = np.zeros((memory_capacity, state_num * 2 + action_num + 1))
        self.loss = nn.MSELoss()
        self.device = device

    def choose_action(self, s):
        s = Variable(torch.unsqueeze(torch.FloatTensor(s), 0))
        logits = self.actor(s)
        logits = F.gumbel_softmax(logits, dim=-1)
        if np.random.uniform() > self.eps:

            action = torch.distributions.Categorical(logits).sample()
            # action = torch.argmax(logits)
        else:
            action = torch.randint(0, self.action_num, ()).to(self.device)
        return action, logits.squeeze(0).detach().cpu().numpy()

    def store_transition(self, s, a, r, next_s):
        trans = np.hstack((s, a, r, next_s))

        i = self.memory_pts % self.memory_capacity
        self.memory[i, :] = trans
        self.memory_pts += 1

    def learn(self):
        self.learn_step += 1

        sample_index = np.random.choice(self.memory_capacity, self.batch_size)
        batch_memory = self.memory[sample_index, :]

        # in the memory, the 1st---4th column is state_now , the 5th is action , the 6th is reward
        # the final 4 column is state_next
        batch_s = Variable(torch.FloatTensor(batch_memory[:, :self.state_num])).to(self.device)
        batch_a = Variable(
            torch.LongTensor(batch_memory[:, self.state_num:self.state_num + self.action_num])).to(
            self.device)
        batch_r = Variable(torch.FloatTensor(
            batch_memory[:, self.state_num + self.action_num:self.state_num + self.action_num + 1])).to(self.device)
        batch_next_s = Variable(torch.FloatTensor(batch_memory[:, -self.state_num:])).to(self.device)

        batch_next_a_logits = self.actor_target(batch_next_s)
        batch_target_next_a = F.gumbel_softmax(batch_next_a_logits, dim=-1)

        y_true = batch_r + self.gamma * self.critic_target(batch_next_s, batch_target_next_a).detach()

        y_pred = self.critic(batch_s, batch_a.float())

        critic_loss = self.loss(y_pred, y_true)
        self.critic_optim.zero_grad()
        critic_loss.backward()
        self.critic_optim.step()

        batch_a_logits = self.actor(batch_s)
        batch_target_a = F.gumbel_softmax(batch_a_logits, dim=-1)

        actor_loss = -torch.mean(
            self.critic(batch_s, batch_target_a))
        self.actor_optim.zero_grad()
        actor_loss.backward()
        self.actor_optim.step()

        for target_param, param in zip(self.critic_target.parameters(), self.critic.parameters()):
            target_param.data.copy_(target_param.data * (1.0 - self.tau) + param.data * self.tau)

        for target_param, param in zip(self.actor_target.parameters(), self.actor.parameters()):
            target_param.data.copy_(target_param.data * (1.0 - self.tau) + param.data * self.tau)

        return critic_loss.item(), actor_loss.item()


class TD3(DDPG):

    def __init__(self, batch_size=32, state_num=115, action_num=19, device='cpu', memory_capacity=100000,
                 update_freq=2, eps=0.1, gamma=0.9, critic_lr=0.0001, actor_lr=0.0001, policy_noise=0.2,
                 noise_clip=0.5, tau=0.005):

        super(TD3, self).__init__(batch_size, state_num, action_num, device, memory_capacity,
                                  eps, gamma, critic_lr, actor_lr, tau)
        self.critic_2 = Critic(3, state_num + action_num, device)
        self.critic_target_2 = Critic(3, state_num + action_num, device)
        self.critic_optim_2 = optim.Adam(self.critic_2.parameters(), lr=critic_lr)
        self.critic_target_2.load_state_dict(self.critic_2.state_dict())
        self.policy_noise = policy_noise
        self.update_freq = update_freq
        self.noise_clip = noise_clip
        self.a_loss = 0
        self.learn_step = 0
        self.device = device

    def learn(self):
        self.learn_step += 1

        sample_index = np.random.choice(self.memory_capacity, self.batch_size)
        batch_memory = self.memory[sample_index, :]

        # in the memory, the 1st---4th column is state_now , the 5th is action , the 6th is reward
        # the final 4 column is state_next
        batch_s = Variable(torch.FloatTensor(batch_memory[:, :self.state_num])).to(self.device)
        batch_a = Variable(torch.LongTensor(batch_memory[:, self.state_num:self.state_num + self.action_num])).to(
            self.device)
        batch_r = Variable(torch.FloatTensor(
            batch_memory[:, self.state_num + self.action_num:self.state_num + self.action_num + 1])).to(self.device)
        batch_next_s = Variable(torch.FloatTensor(batch_memory[:, -self.state_num:])).to(self.device)
        batch_next_a_logits = self.actor_target(batch_next_s)
        batch_target_next_a = F.gumbel_softmax(batch_next_a_logits, dim=-1)

        noise = torch.randn(size=batch_next_a_logits.shape, device=self.device) * self.policy_noise
        if self.noise_clip >= 0:
            noise = noise.clamp(-self.noise_clip, self.noise_clip)
            batch_target_next_a += noise

        target_q = torch.min(
            self.critic_target(batch_next_s, batch_target_next_a),
            self.critic_target_2(batch_next_s, batch_target_next_a))

        y_true = batch_r + self.gamma * target_q.detach()

        y_pred = self.critic(batch_s, batch_a.float())
        y_pred_2 = self.critic_2(batch_s, batch_a.float())

        critic_loss = self.loss(y_pred, y_true)
        self.critic_optim.zero_grad()
        critic_loss.backward()
        self.critic_optim.step()

        critic_loss_2 = self.loss(y_pred_2, y_true)
        self.critic_optim_2.zero_grad()
        critic_loss_2.backward()
        self.critic_optim_2.step()

        if self.learn_step % self.update_freq == 0:

            batch_a_logits = self.actor(batch_s)
            batch_target_a = F.gumbel_softmax(batch_a_logits, dim=-1)

            actor_loss = -torch.mean(
                self.critic(batch_s, batch_target_a))
            self.actor_optim.zero_grad()
            actor_loss.backward()
            self.actor_optim.step()

            for target_param, param in zip(self.critic_target.parameters(), self.critic.parameters()):
                target_param.data.copy_(target_param.data * (1.0 - self.tau) + param.data * self.tau)

            for target_param, param in zip(self.critic_target_2.parameters(), self.critic_2.parameters()):
                target_param.data.copy_(target_param.data * (1.0 - self.tau) + param.data * self.tau)

            for target_param, param in zip(self.actor_target.parameters(), self.actor.parameters()):
                target_param.data.copy_(target_param.data * (1.0 - self.tau) + param.data * self.tau)
            self.a_loss = actor_loss.item()

        return critic_loss.item(), critic_loss_2.item(), self.a_loss


class PPO(object):
    def __init__(self, batch_size=32, state_num=115, action_num=19, device='cpu', memory_capacity=2048,
                 eps=0.1, gamma=0.9, lr=0.0001, clip=0.2, loss_coeff_value=0.5, loss_coeff_entropy=0.01, lamda=0.95,
                 clip_grad_norm=0.5, optim_epoch=10):

        self.actor = Actor(3, state_num, action_num, device, orthogonal=False)
        self.critic = Critic(3, state_num + action_num, device, orthogonal=False)
        self.optim = optim.Adam([{'params': self.actor.parameters()},
                                 {'params': self.critic.parameters()}], lr=lr)

        self.batch_size = batch_size
        self.state_num = state_num
        self.action_num = action_num
        self.memory_pts = 0
        self.eps = eps
        self.gamma = gamma
        self.clip = clip
        self.clip_grad_norm = clip_grad_norm
        self.loss_coeff_value = loss_coeff_value
        self.loss_coeff_entropy = loss_coeff_entropy
        self.lamda = lamda
        self.memory = np.zeros((memory_capacity, state_num + action_num + 4))
        self.memory_capacity = memory_capacity
        self.device = device
        self.loss = nn.MSELoss()
        self.optim_epoch = optim_epoch

    def choose_action(self, s):
        s = Variable(torch.unsqueeze(torch.FloatTensor(s), 0)).to(self.device)
        log_prob = -2.94
        logits = self.actor(s)
        logits = F.gumbel_softmax(logits, dim=-1)
        # logits = F.softmax(logits, dim=-1)
        if np.random.uniform() > self.eps:
            dist = torch.distributions.Categorical(logits)
            action = dist.sample()
            # action = torch.argmax(logits)
            log_prob = dist.log_prob(action).detach()
            # log_prob = torch.log(logits.squeeze(0)[action]).detach().cpu().numpy()
        else:
            action = torch.randint(0, self.action_num, ()).to(self.device)
        return action, log_prob, logits.squeeze(0).detach().cpu().numpy()

    def get_logproba(self, states, actions):
        logits = self.actor(states)
        logits = F.gumbel_softmax(logits, dim=-1)
        # logits = F.softmax(logits, dim=-1)
        dist = torch.distributions.Categorical(logits)
        if len(actions.shape) is 2:
            logproba = dist.log_prob(actions.squeeze(0))
        else:
            logproba = dist.log_prob(actions).detach().cpu().numpy()
        return logproba

    def store_transition(self, s, a, logits, r, done, logprob):
        trans = np.hstack((s, a, logits, r, done, logprob))

        i = self.memory_pts % self.memory_capacity
        self.memory[i, :] = trans
        self.memory_pts += 1

    def if_full(self):
        return self.memory_pts >= self.memory_capacity

    def reset_buffer(self):
        self.memory_pts = 0
        self.memory = np.zeros((self.memory_capacity, self.state_num + self.action_num + 4))

    def learn(self):

        batch_memory = self.memory
        total_size = batch_memory.shape[0]
        # in the memory, the 1st---4th column is state_now , the 5th is action , the 6th is reward
        # the final 4 column is state_next
        batch_s = Variable(torch.FloatTensor(batch_memory[:, :self.state_num])).to(self.device)
        batch_a = Variable(
            torch.LongTensor(batch_memory[:, self.state_num:self.state_num + 1])).to(
            self.device)
        batch_logits = Variable(
            torch.LongTensor(batch_memory[:, self.state_num + 1:self.state_num + self.action_num + 1])).to(
            self.device).float()
        batch_r = Variable(torch.FloatTensor(
            batch_memory[:, self.state_num + self.action_num + 1:self.state_num + self.action_num + 2])).to(self.device)
        batch_v = self.critic(batch_s, batch_logits).detach()

        batch_mask = Variable(torch.FloatTensor(
            batch_memory[:, self.state_num + self.action_num + 2:self.state_num + self.action_num + 3])).to(self.device)
        batch_old_logprob = Variable(torch.FloatTensor(
            batch_memory[:, self.state_num + self.action_num + 3:self.state_num + self.action_num + 4])).to(
            self.device)

        batch_returns = torch.Tensor(total_size).to(self.device)
        batch_delta = torch.Tensor(total_size).to(self.device)
        batch_advantage = torch.Tensor(total_size).to(self.device)

        prev_return = 0
        prev_value = 0
        prev_advantage = 0
        for i in reversed(range(total_size)):
            batch_returns[i] = batch_r[i] + self.gamma * prev_return * batch_mask[i]
            batch_delta[i] = batch_r[i] + self.gamma * prev_value * batch_mask[i] - batch_v[i]
            batch_advantage[i] = batch_delta[i] + self.gamma * self.lamda * prev_advantage * batch_mask[i]

            prev_return = batch_returns[i]
            prev_value = batch_v[i]
            prev_advantage = batch_advantage[i]

        batch_advantage = (batch_advantage - batch_advantage.mean()) / (batch_advantage.std() + 1e-10)

        return_loss = 0
        for _ in range(self.optim_epoch):
            for _ in range(int(total_size / self.batch_size)):
                # sample from current batch
                minibatch_ind = np.random.choice(total_size, self.batch_size, replace=False)
                minibatch_states = batch_s[minibatch_ind]
                minibatch_actions = batch_a[minibatch_ind]
                minibatch_logits = batch_logits[minibatch_ind]
                minibatch_oldlogproba = batch_old_logprob[minibatch_ind]
                minibatch_newlogproba = self.get_logproba(minibatch_states, minibatch_actions)
                minibatch_advantage = batch_advantage[minibatch_ind]
                minibatch_returns = batch_returns[minibatch_ind]
                minibatch_newvalues = self.critic(minibatch_states, minibatch_logits)

                ratio = torch.exp(minibatch_newlogproba - minibatch_oldlogproba).squeeze(1)
                surr1 = ratio * minibatch_advantage
                surr2 = ratio.clamp(1 - self.clip, 1 + self.clip) * minibatch_advantage
                loss_surr = - torch.mean(torch.min(surr1, surr2))

                loss_value = torch.mean((minibatch_newvalues - minibatch_returns).pow(2))

                loss_entropy = torch.mean(torch.exp(minibatch_newlogproba) * minibatch_newlogproba)

                total_loss = loss_surr + self.loss_coeff_value * loss_value + self.loss_coeff_entropy * loss_entropy
                self.optim.zero_grad()
                total_loss.backward()
                torch.nn.utils.clip_grad_norm_(self.actor.parameters(), self.clip_grad_norm)
                torch.nn.utils.clip_grad_norm_(self.critic.parameters(), self.clip_grad_norm)

                self.optim.step()
                return_loss += total_loss.item()

        return return_loss
