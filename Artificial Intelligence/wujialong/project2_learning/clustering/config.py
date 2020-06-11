# 所有特征
FEATURES = ['MFCCs_ 1','MFCCs_ 2','MFCCs_ 3','MFCCs_ 4','MFCCs_ 5','MFCCs_ 6','MFCCs_ 7','MFCCs_ 8','MFCCs_ 9','MFCCs_10','MFCCs_11','MFCCs_12','MFCCs_13','MFCCs_14','MFCCs_15','MFCCs_16','MFCCs_17','MFCCs_18','MFCCs_19','MFCCs_20','MFCCs_21','MFCCs_22']

# 科属
FAMILY = ['Bufonidae', 'Dendrobatidae', 'Hylidae', 'Leptodactylidae']
GENUS = ['Hypsiboas', 'Leptodactylus', 'Adenomera', 'Scinax', 'Ameerega', 'Rhinella', 'Dendropsophus', 'Osteocephalus']
SPECIES = ['AdenomeraAndre', 'LeptodactylusFuscus', 'ScinaxRuber', 'HylaMinuta', 'Rhinellagranulosa', 'HypsiboasCinerascens', 'OsteocephalusOophagus', 'Ameeregatrivittata', 'HypsiboasCordobae', 'AdenomeraHylaedactylus']

# 根据标准差从大到小选择前若干个特征
FEATURE_STD_TOP = 17

# 前向选择算法结果
# forward_feature_selection(4, FEATURES, target_num = FEATURE_STD_TOP)
BEST_SELECT = ['MFCCs_ 1', 'MFCCs_22', 'MFCCs_10', 'MFCCs_ 7', 'MFCCs_15', 'MFCCs_12', 'MFCCs_ 6', 'MFCCs_ 5', 'MFCCs_11', 'MFCCs_17', 'MFCCs_ 9', 'MFCCs_20', 'MFCCs_21', 'MFCCs_18', 'MFCCs_19', 'MFCCs_13', 'MFCCs_ 2']