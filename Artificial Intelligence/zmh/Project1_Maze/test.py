import utils
import time

puzzle = utils.Puzzle(file_name="map.txt", final_states=["E", "T"])
print("Initial value map:")
puzzle.print_puzzle()
print("")

# Value iteration
start_time = time.time()
print("Value maps during iteration:")
for iter_time in range(50):
	if iter_time in utils.show_iter_time:
		print("iter time:{}".format(iter_time))
		puzzle.print_puzzle()
		print("")
	puzzle.update_value(utils.action_collection, gamma=0.9)

print("Value iteration result:\n")
puzzle.build_action_map(utils.action_collection, utils.simple_action_symbol_dict)
end_time = time.time()
print("Time consumption: {}s".format(end_time - start_time))

puzzle = utils.Puzzle(file_name="map.txt", final_states=["E", "T"])
# Policy iteration
start_time = time.time()
policy_map = 0
for iter_time in range(50):
	if iter_time in utils.show_iter_time:
		print("iter time:{}".format(iter_time))
		puzzle.print_current_policy(utils.simple_action_symbol_dict)
		print("")
	puzzle.policy_evaluation(utils.action_collection, gamma=0.9)
	new_policy_map = puzzle.policy_iteration(utils.action_collection)
	if(policy_map == new_policy_map):
		break
	policy_map = new_policy_map
print("Iteration ended, current time: {}".format(iter_time))
puzzle.print_current_policy(utils.simple_action_symbol_dict)
end_time = time.time()
print("Time consumption: {}s".format(end_time - start_time))
