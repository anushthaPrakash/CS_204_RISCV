# import fetch
# import decode
# import execute
# import memory
# import Writeback
def assemble(input_list):
    clock=0
    instruction_dict={}    #dictionary storing instructions
    data_dict={}    #dictionary storing data in memory
    reg={} #registers
    reg[0]='0x0'
    for i in range(32):
        reg[i]='0x00000000'
        if i==2:
            reg[i]='0x7FFFFFF0'
        if i==3:
            reg[i]='0x10000000'
    file=open("simple_add.mem","r")