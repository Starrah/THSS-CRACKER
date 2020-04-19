import copy
from collections import defaultdict
import random


def action_left(cord, puzzle):
	state_map = puzzle.state_map
	next_state_collection = defaultdict(float)
	direction = lambda c: [c[0] - 1, c[1] - 1]
	inv_direction = lambda c: [c[0] + 1, c[1] + 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1/5

	direction = lambda c: [c[0], c[1] - 1]
	inv_direction = lambda c: [c[0], c[1] + 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 3/5

	direction = lambda c: [c[0] + 1, c[1] - 1]
	inv_direction = lambda c: [c[0] - 1, c[1] + 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1/5
	return next_state_collection


def action_up(cord, puzzle):
	state_map = puzzle.state_map
	next_state_collection = defaultdict(float)
	direction = lambda c: [c[0] - 1, c[1] - 1]
	inv_direction = lambda c: [c[0] + 1, c[1] + 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5

	direction = lambda c: [c[0] - 1, c[1]]
	inv_direction = lambda c: [c[0] + 1, c[1]]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 3 / 5

	direction = lambda c: [c[0] - 1, c[1] + 1]
	inv_direction = lambda c: [c[0] + 1, c[1] - 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5
	return next_state_collection


def action_right(cord, puzzle):
	state_map = puzzle.state_map
	next_state_collection = defaultdict(float)
	direction = lambda c: [c[0] - 1, c[1] + 1]
	inv_direction = lambda c: [c[0] + 1, c[1] - 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5

	direction = lambda c: [c[0], c[1] + 1]
	inv_direction = lambda c: [c[0], c[1] - 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 3 / 5

	direction = lambda c: [c[0] + 1, c[1] + 1]
	inv_direction = lambda c: [c[0] - 1, c[1] - 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5
	return next_state_collection


def action_down(cord, puzzle):
	state_map = puzzle.state_map
	next_state_collection = defaultdict(float)
	direction = lambda c: [c[0] + 1, c[1] - 1]
	inv_direction = lambda c: [c[0] - 1, c[1] + 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5

	direction = lambda c: [c[0] + 1, c[1]]
	inv_direction = lambda c: [c[0] - 1, c[1]]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 3 / 5

	direction = lambda c: [c[0] + 1, c[1] + 1]
	inv_direction = lambda c: [c[0] - 1, c[1] - 1]
	cord_temp = direction(cord)
	while puzzle.check_valid_cord(cord_temp) and state_map[cord_temp[0]][cord_temp[1]] == "S":
		cord_temp = direction(cord_temp)
	if (not puzzle.check_valid_cord(cord_temp)) or state_map[cord_temp[0]][cord_temp[1]] == "W":
		cord_temp = inv_direction(cord_temp)
	next_state_collection[tuple(cord_temp)] += 1 / 5
	return next_state_collection


action_collection = [action_left, action_up, action_right, action_down]
simple_action_symbol_dict = {0: "←", 1: "↑", 2: "→", 3: "↓", "@": "@"}


show_iter_time = [] # [1, 5, 20, 49]


map_dict = {"T": -1, "O": 0, "E": 1, "M": 0, "W": 0, "S": 0}


class Puzzle:
	def __init__(self, file_name=None, final_states=None):
		if not file_name:
			self.state_map = []
			self.reward_map = []
			self.height = 0
			self.width = 0
			self.final_states = []
			self.action_map = []
			return
		with open(file_name) as f:
			lines = f.readlines()
		self.state_map = list(map(lambda x: x[:-1].split(' '), lines))
		self.get_init_reward_map(self.state_map)
		self.height = len(self.state_map)
		self.width = len(self.state_map[0])
		self.action_map = [[random.randrange(0, 3) for ii in range(self.width)] for i in range(self.height)]
		self.final_states = final_states
		return

	def get_init_reward_map(self, state_map):
		self.reward_map = [[map_dict[ii] for ii in i]for i in state_map]
		return

	def read_map(self, file_name):
		with open(file_name) as f:
			lines = f.readlines()
		self.state_map = list(map(lambda x: list(map(int, x.split(' '))), lines))
		self.get_init_reward_map(self.state_map)
		self.height = len(self.state_map)
		self.width = len(self.state_map[0])
		return self.state_map, self.reward_map

	def check_valid_cord(self, c):
		return (0 <= c[0] < self.height) and (0 <= c[1] < self.width)

	def action_reward(self, state, next_state):
		if self.state_map[next_state[0]][next_state[1]] == "M":
			return -0.1
		return 0

	def update_value(self, actions, gamma):
		temp_reward_map = copy.deepcopy(self.reward_map)
		for i in range(self.height):
			for j in range(self.width):
				current_state = (i, j)
				if self.state_map[i][j] in self.final_states:
					continue
				max_expectation = float("-inf")
				for action in actions:
					next_state_collection = action(current_state, self)
					expectation = 0
					for (state, prob) in next_state_collection.items():
						expectation += prob * gamma * self.reward_map[state[0]][state[1]]
						expectation += prob * self.action_reward(current_state, state)
					max_expectation = [max_expectation, expectation][expectation > max_expectation]
				temp_reward_map[i][j] = max_expectation
		self.reward_map = temp_reward_map
		return self.reward_map

	def print_puzzle(self):
		print('\n'.join(' '.join("{:.4f}".format(r) for r in line) for line in self.reward_map))
		return self.reward_map

	def build_action_map(self, actions, action_symbol_dict):
		self.action_map = copy.deepcopy(self.reward_map)
		action_symbol_map = copy.deepcopy(self.reward_map)
		for i in range(self.height):
			for j in range(self.width):
				current_state = (i, j)
				if self.state_map[i][j] in self.final_states:
					self.action_map[i][j] = -1
					action_symbol_map[i][j] = "@"
					continue
				max_expectation = float("-inf")
				max_action = None
				for action in actions:
					next_state_collection = action(current_state, self)
					expectation = 0
					for (state, prob) in next_state_collection.items():
						expectation += prob * self.reward_map[state[0]][state[1]]
						expectation += prob * self.action_reward(current_state, state)
					max_action = [max_action, action][expectation > max_expectation]
					max_expectation = [max_expectation, expectation][expectation > max_expectation]
				self.action_map[i][j] = actions.index(max_action)
				action_symbol_map[i][j] = action_symbol_dict[self.action_map[i][j]]
		print('\n'.join(' '.join("{} ".format(a) for a in line) for line in action_symbol_map))
		return self.action_map

	def policy_evaluation(self, actions, gamma):
		temp_reward_map = copy.deepcopy(self.reward_map)
		while True:
			max_dif = 0
			for i in range(self.height):
				for j in range(self.width):
					current_state = (i, j)
					if self.state_map[i][j] in self.final_states:
						continue
					next_state_collection = actions[self.action_map[i][j]](current_state, self)
					expectation = 0
					for (state, prob) in next_state_collection.items():
						expectation += prob * gamma * self.reward_map[state[0]][state[1]]
						expectation += prob * self.action_reward(current_state, state)
					temp_reward_map[i][j] = expectation
					dif = abs(expectation - self.reward_map[i][j])
					max_dif = [max_dif, dif][dif > max_dif]
			if(max_dif < 1e-3):
				break
			self.reward_map = temp_reward_map
		return self.reward_map

	def policy_iteration(self, actions):
		self.action_map = copy.deepcopy(self.reward_map)
		for i in range(self.height):
			for j in range(self.width):
				current_state = (i, j)
				if self.state_map[i][j] in self.final_states:
					self.action_map[i][j] = "@"
					continue
				max_expectation = float("-inf")
				max_action = None
				for action in actions:
					next_state_collection = action(current_state, self)
					expectation = 0
					for (state, prob) in next_state_collection.items():
						expectation += prob * self.reward_map[state[0]][state[1]]
						expectation += prob * self.action_reward(current_state, state)
					max_action = [max_action, action][expectation > max_expectation]
					max_expectation = [max_expectation, expectation][expectation > max_expectation]
				self.action_map[i][j] = actions.index(max_action)
		return self.action_map

	def print_current_policy(self, action_symbol_dict):
		action_symbol_map = [list(map(lambda x: action_symbol_dict[x], i)) for i in self.action_map]
		print('\n'.join(' '.join("{} ".format(a) for a in line) for line in action_symbol_map))
