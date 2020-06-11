# 所有特征
FEATURES = ['age', 'job', 'marital', 'education', 'default', 'balance', 'housing', 'loan', 'contact', 'day', 'month', 'duration', 'campaign', 'pdays', 'previous', 'poutcome']

# 特征组合1：手工选择特征
MANUAL = ['job', 'education', 'default', 'balance', 'housing', 'loan', 'month', 'pdays', 'previous', 'poutcome']
# 特征组合2：特征子集前向选择算法结果
BEST = ['age', 'education', 'housing', 'loan', 'contact', 'day', 'month', 'duration', 'pdays', 'poutcome']