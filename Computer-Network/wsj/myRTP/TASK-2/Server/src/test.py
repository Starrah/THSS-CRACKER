import os
msg = ''
for root,dirs,files in os.walk('./video'):
    for file in files:
        suffix = os.path.splitext(file)[1]
        if suffix == '.mp4' or suffix == '.avi' or suffix == '.iso' or suffix == '.mov':
            msg += file
            msg += ' '
msg = msg.strip()
print(msg)