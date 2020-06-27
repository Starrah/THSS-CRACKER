from Generator.Generator import generate
import sys

if __name__ == '__main__':
    if len(sys.argv) >= 2:
        input_filename = sys.argv[1]
        output_filename = input_filename.split(".")[0]+".ll"
        generate(input_filename, output_filename)
    else:
        print("Usage: python main.py (input_filename)")
