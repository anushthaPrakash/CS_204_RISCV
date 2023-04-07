import sys
file = open('input.mem', 'r')
sys.stdout = open('test_case.mem','w')
# This will print every line one by one in the file
i =0 
for each in file:
    print(hex(i),end=" ")
    print(each,end="")
    i += 4
print()
print(hex(i),"0xEF000011")