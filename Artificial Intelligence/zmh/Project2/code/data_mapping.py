# data preprocess
job_mapping = {
    'management': 0.,
    'technician': 1.,
    'admin.': 2.,
    'services': 3.,
    'retired': 4.,
    'student': 5.,
    'blue-collar': 6.,
    'housemaid': 7.,
    'unemployed': 8.,
    'entrepreneur': 9.,
    'self-employed': 10.,
    'unknown': 11.,
}

marital_mapping = {
    'married': 0.,
    'single': 1.,
    'divorced': 2.,
    'unknown': 3.,
}

education_mapping = {
    'tertiary': 0.,
    'primary': 1.,
    'secondary': 2.,
    'unknown': 3.,
}

default_mapping = {
    'no': 0.,
    'yes': 1.,
    'unknown': 2.,
}
housing_mapping = {
    'no': 0.,
    'yes': 1.,
    'unknown': 2.,
}

loan_mapping = {
    'no': 0.,
    'yes': 1.,
    'unknown': 2.,
}

contact_mapping = {
    'cellular': 0.,
    'telephone': 1.,
    'unknown': 2.,
}

month_mapping = {
    'jan': 0.,
    'feb': 1.,
    'mar': 2.,
    'apr': 3.,
    'may': 4.,
    'jun': 5.,
    'jul': 6.,
    'aug': 7.,
    'sep': 8.,
    'oct': 9.,
    'nov': 10.,
    'dec': 11.,
    'unknown': 12.,
}

poutcome_mapping = {
    'failure': 0.,
    'success': 1.,
    'other': 2.,
    'unknown': 3.,
}

mapping_keys = ['job_mapping', 'marital_mapping', 'education_mapping', 'default_mapping',
                'housing_mapping', 'loan_mapping', 'contact_mapping', 'month_mapping', 'poutcome_mapping']

mapping_mapping = {'job_mapping': job_mapping, 'marital_mapping': marital_mapping,
                   'education_mapping': education_mapping, 'default_mapping': default_mapping,
                   'housing_mapping': housing_mapping, 'loan_mapping': loan_mapping, 'contact_mapping': contact_mapping,
                   'month_mapping': month_mapping, 'poutcome_mapping': poutcome_mapping}

family_mapping = {
    'Leptodactylidae': 0,
    'Dendrobatidae': 1,
    'Hylidae': 2,
    'Bufonidae': 3
}
