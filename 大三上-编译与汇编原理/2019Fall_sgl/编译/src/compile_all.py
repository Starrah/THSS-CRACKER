from Generator.Generator import generate
import sys
import os

print(os.getcwd())
c_files = []
for root,dirs,files in os.walk('test'):
    for file in files:
        suffix = os.path.splitext(file)[1]
        if suffix == '.c':
            input_filename = 'test/'+file
            print("compiling ",input_filename)
            output_filename = input_filename.split(".")[0]+".ll"
            generate(input_filename, output_filename)