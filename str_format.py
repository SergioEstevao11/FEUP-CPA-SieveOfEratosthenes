input_string = input("Enter a string: ")

# Replace spaces with tabs, and remove newlines
output_string = input_string.replace(' ', '\t').replace('\n', '')

# Remove all characters that are not digits
output_string = ''.join(filter(str.isdigit, output_string))

print(output_string)