import argparse
import re
import os
import sys

def transpile_line(line):
    """Applies basic regex replacements to translate C# syntax to Python."""
    # Replace comments
    line = re.sub(r'//', r'#', line)
    
    # Replace printing
    line = re.sub(r'Console\.WriteLine\((.*)\)', r'print(\1)', line)
    line = re.sub(r'Console\.Write\((.*)\)', r'print(\1, end="")', line)
    
    # Replace booleans
    line = re.sub(r'\btrue\b', 'True', line)
    line = re.sub(r'\bfalse\b', 'False', line)
    line = re.sub(r'\bnull\b', 'None', line)
    
    # Replace conditionals/loops
    line = re.sub(r'else\s+if', 'elif', line)
    line = re.sub(r'foreach\s*\((.*?)\s+in\s+(.*?)\)', r'for \1 in \2:', line)
    
    # Remove semicolons
    line = line.replace(';', '')
    
    # Basic attempt to strip types from variable assignments (e.g., int x = 5 -> x = 5)
    line = re.sub(r'^(?:int|double|float|string|bool|var)\s+(\w+)\s*=', r'\1 =', line)
    
    return line.strip()

def convert_csharp_to_python(csharp_code):
    """Reads C# text and formats it into Pythonic indentation and syntax."""
    lines = csharp_code.split('\n')
    python_lines = []
    
    indent_level = 0
    indent_space = "    "

    for line in lines:
        stripped = line.strip()

        # 1. Skip empty lines and C# specific boilerplate
        if not stripped or stripped.startswith('using ') or stripped.startswith('namespace '):
            continue

        # 2. Handle closing braces (Decrease indent)
        if stripped.startswith('}'):
            indent_level = max(0, indent_level - 1)
            continue

        # 3. Handle standalone opening braces (Increase indent)
        if stripped == '{':
            continue 

        # 4. Handle Class declarations
        if stripped.startswith('class '):
            class_name = stripped.split()[1]
            python_lines.append(f"{indent_space * indent_level}class {class_name}:")
            indent_level += 1
            continue

        # 5. Handle Method declarations (Very basic regex)
        # Looks for patterns like "public static void Main(string[] args)"
        if re.match(r'.*(public|private|protected|internal|static|void|int|string|bool|double).*\(.*\)', stripped) and not stripped.startswith('print'):
            match = re.search(r'(\w+)\s*\((.*)\)', stripped)
            if match:
                func_name = match.group(1)
                args = match.group(2)
                
                # Clean up args (remove C# types, keep variable names)
                clean_args = ", ".join([arg.strip().split()[-1] for arg in args.split(',') if arg.strip()])
                
                if func_name == "Main":
                    python_lines.append(f"{indent_space * indent_level}if __name__ == '__main__':")
                else:
                    python_lines.append(f"{indent_space * indent_level}def {func_name}({clean_args}):")
                
                indent_level += 1
                
                # If the brace is on the same line as the method
                if stripped.endswith('{'):
                    pass # Indent already increased
            continue

        # 6. Handle standard lines of code
        transpiled = transpile_line(stripped)
        
        if transpiled:
            # Handle inline opening braces for if/for/while loops
            if transpiled.endswith('{'):
                transpiled = transpiled[:-1].strip() + ':'
                python_lines.append(f"{indent_space * indent_level}{transpiled}")
                indent_level += 1
            else:
                # Add colon to blocks if missing (like if, elif, else, while)
                if re.match(r'^(if|elif|else|while|for)\b.*[^:]$', transpiled):
                    transpiled += ':'
                    indent_level += 1 # Assume next line will be inside the block
                
                python_lines.append(f"{indent_space * indent_level}{transpiled}")

    return '\n'.join(python_lines)

def main():
    parser = argparse.ArgumentParser(description="A rudimentary C# to Python script converter.")
    parser.add_argument("input", help="Path to the input .cs file")
    parser.add_argument("-o", "--output", help="Path to the output .py file", default="output.py")
    
    args = parser.parse_args()

    if not os.path.exists(args.input):
        print(f"Error: Input file '{args.input}' not found.")
        sys.exit(1)

    print(f"Reading C# file: {args.input}...")
    with open(args.input, 'r', encoding='utf-8') as f:
        csharp_code = f.read()

    print("Transpiling to Python...")
    python_code = convert_csharp_to_python(csharp_code)

    with open(args.output, 'w', encoding='utf-8') as f:
        f.write(python_code)

    print(f"Success! Python code written to: {args.output}")

if __name__ == "__main__":
    main()
