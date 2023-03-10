import sys
file = open('input.txt', 'r')
sys.stdout = open('output.txt','w')
# This will print every line one by one in the file
i =0 
for each in file:
    print(hex(i),end=" ")
    print(each,end="")
    i += 4