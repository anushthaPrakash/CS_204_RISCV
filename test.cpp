#include <bits/stdc++.h>
using namespace std;

#define M 32
void write_data_memory();
static unsigned int X[32];
static unsigned int MEM[4000];//only 4000?
static int DMEM[4000];
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;
char Type = '0';
static bitset<M> inst;
static unsigned int des_reg;
static int des_res;
string subtype;
static int imm;
static int  pc = 0;
// -----------DOUBT------------
// should the instuction memory be in byte format
// like the first 8 bits(1 byte) of instruction in index 0 of MEM then next 8 bits in MEM[1] so that a single instruction will be 32 bits and will take
// MEM[0]-MEM[3] , then next instruction will me from MEM[4] so the pc will be adjusted to move 4 at one time 
// like when they give recursion type code we have to keep in mind that the space there while craeting stack is sp-4 i.e handle the recursion 

// does this handle the msb extension and arithmetic and logical sign extension?
// In provided RISCV refernce card immediate value bit is not correct in U type instruction?
// test >> with Arithmatic shift and logical shift

// checking the instruction set
char op_R_type(bitset<7> op)
{
  bitset<7> opr("0110011");
  if (op == opr)
    return 'R';
  else
    return '0';
}
char op_I_type(bitset<7> op)
{
  bitset<7> opi1("0010011");
  bitset<7> opi2("1100111");
  if (op == opi1 || op == opi2)
    return 'I';
  else
    return '0';
}
char op_J_type(bitset<7> op)
{
  bitset<7> opj("1101111");
  if (op == opj)
    return 'J';
  else
    return '0';
}
char op_B_type(bitset<7> op)
{
  bitset<7> opb("1100011");
  if (op == opb)
    return 'B';
  else
    return '0';
}
char op_S_type(bitset<7> op)
{
  bitset<7> ops("0100011");
  if (op == ops)
    return 'S';
  else
    return '0';
}
char op_U_type(bitset<7> op)
{
  bitset<7> opu1("0110111");
  bitset<7> opu2("0010111");
  if (op == opu1 || op == opu2)
    return 'U';
  else
    return '0';
}
// end checking
string subtype_select(bitset<5> func3, bitset<5> func7, char Type)
{
  
}
void swi_exit() {
   write_data_memory();
   exit(0);
 }

void reset_proc()
{
  for (auto i : X)
  {
    i = 0;
  }
  X[2]=3999;
  for (auto p : MEM)
  {
    p = 0;
  }
  for (auto q : DMEM)
  {
    q = 0;
  }
}

// read and write in file start
int read_word(unsigned int *mem, unsigned int address)
{
  int *data;
  data = (int *)(mem + address);
  return *data;
}

void write_word(unsigned int *mem, unsigned int address, unsigned int data)
{
  int *data_p;
  data_p = (int *)(mem + address);
  *data_p = data;
}
// read and write ends

void write_data_memory()
{
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if (fp == NULL)
  {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }

  for (i = 0; i <= 12; i = i + 4)
  {
    fprintf(fp, "%u %u\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

void load_program_memory()
{
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen("simple_add.mem", "r");
  if (fp == NULL)
  {
    printf("Error opening input mem file\n");
    exit(1);
  }

  while (fscanf(fp, "%x %x", &address, &instruction) != EOF)
  {
    write_word(MEM, address, instruction);
  }
  write_data_memory();
  fclose(fp);
}

// reads from the instruction memory and updates the instruction register
void fetch()
{
  inst = MEM[pc];
  cout<<inst<<endl;
  bitset<32> exitcode("11101111000000000000000000010001");
  if(inst == exitcode ){
    swi_exit();
  }
}
// reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode()
{
  bitset<7> op;
  bitset<7> func7;
  bitset<3> func3;
  bitset<5> rs1, rs2, rd;

  for (int i = 0; i < 7; i++)
  {
    op[i] = inst[i];
  }
  // cout<<op<<endl;
  while (1)
  {
    if (Type == '0')
    {
      Type = op_R_type(op);
    }
    if (Type == '0')
      Type = op_I_type(op);
    if (Type == '0')
      Type = op_J_type(op);
    if (Type == '0')
      Type = op_B_type(op);
    if (Type == '0')
      Type = op_S_type(op);
    if (Type == '0')
      Type = op_U_type(op);
    break;
  }
  // cout<< Type<<endl;
  // cout<<inst<<endl;
  // cout << operand1 << "  " << des_reg << endl;
      int j = 0;
    for (int i = 25; i < 32; i++)
    {
      func7[j] = inst[i];
      j++;
    }
    j = 0;
    for (int i = 12; i < 15; i++)
    {
      func3[j] = inst[i];
      j++;
    }
    j = 0;
    for (int i = 15; i < 20; i++)
    {
      rs1[j] = inst[i];
      j++;
    }
    operand1 = rs1.to_ulong();
    j = 0;
    for (int i = 20; i < 25; i++)
    {
      rs2[j] = inst[i];
      j++;
    }
    operand2 = rs2.to_ulong();
  switch (Type)
  {
  case 'R':
  {

    j = 0;
    for (int i = 7; i < 12; i++)
    {
      rd[j] = inst[i];
      j++;
    }
    des_reg = rd.to_ulong();
  } 
  case 'I':{
    bitset<12> immb;
     j = 0;
    for (int i = 7; i < 12; i++)
    {
      rd[j] = inst[i];
      j++;
    }
    des_reg = rd.to_ulong();
    j=0;
    for (int i = 20; i < 32; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    imm = immb.to_ulong();
    if(immb[11]==1){
      imm = -1*imm;
    }

  }
  case 'S':{
    bitset<12> immb;
     j=0;
    for (int i = 7; i < 12; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    for (int i = 25; i < 32; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    imm = immb.to_ulong();
    if(immb[11]==1){
      imm = -1*imm;
    }

  }
  case 'B':{
    bitset<13> immb;
     j=0;
     immb[0]=0;
    for (int i = 8; i < 12; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    for (int i = 25; i < 31; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    immb[j]=inst[7];
    j++;
    immb[j]= inst[31];
    imm = immb.to_ulong();
    if(immb[12]==1){
      imm = -1*imm;
    }
  }
  case 'U':{
        j = 0;
    for (int i = 7; i < 12; i++)
    {
      rd[j] = inst[i];
      j++;
    }
    des_reg = rd.to_ulong();
    // bitset<32> immb;
    // for (int j = 0; j < 12; j++)
    // {
    //   immb[j] = 0;
    // }
    // for (int i = 12; i < 32; i++)
    // {
    //   immb[j] = inst[i];
    //   j++;
    // }
    // imm = immb.to_ulong();
    // if(immb[31]==1){
    //   imm = -1*imm;
    // }
    bitset<20> immb;
    j=0;
    for (int i = 12; i < 32; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    cout<<immb<<endl;
    imm = immb.to_ulong();
    if(immb[19]==1){
      imm = -1*imm;
    }
     }
  case 'J':{
    j=0;
    bitset<21> immb;
    j=0;
    immb[0]=0;
    j=12;
     for (int i = 12; i < 20; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    immb[11]=inst[20];
    j=1;
     for (int i = 21; i < 31; i++)
    {
      immb[j] = inst[i];
      j++;
    }
    immb[20]=inst[31];
     imm = immb.to_ulong();
    if(immb[20]==1){
      imm = -1*imm;
    }
  }
  default:{
    cout<<"error"<<endl;
  }
   

  }
}
// executes the ALU operation based on ALUop
void execute()
{
    if(Type == 'R'){ //add, and, or, sll, slt, sra, srl, sub, xor
    if(subtype == "add")  des_res = X[operand1] + X[operand2];
    else if(subtype == "sub") des_res = X[operand1] - X[operand2];
    else if(subtype == "and") des_res = X[operand1] & X[operand2];
    else if(subtype == "or")  des_res = X[operand1] | X[operand2];
    else if(subtype == "sll") des_res = X[operand1] << X[operand2];
    else if(subtype == "slt") des_res = (X[operand1] < X[operand2])?1:0;
    else if(subtype == "sra") des_res = X[operand1] << X[operand2];
    else if(subtype == "xor") des_res = X[operand1] ^ X[operand2];
    else if(subtype == "srl") des_res = X[operand1] << X[operand2];
  }
  else if(Type == 'I'){ //addi, andi, ori, lb, lh, lw, jalr
    if(subtype == "addi") des_res = X[operand1]+imm; 
    else if(subtype == "andi") des_res = X[operand1]&imm; 
    else if(subtype == "ori") des_res = X[operand1]|imm; 
    else if(subtype == "lb" || subtype == "lh" || subtype == "lw") des_res = X[operand1]+imm; 
    else if(subtype == "jalr") {des_res =pc + 4; pc = X[operand1]+imm;} 
  }
  else if(Type == 'B'){//beq, bne, bge, blt
    if(subtype == "beq") if(X[operand1] == X[operand2]) pc += imm;
    else if(subtype == "bne") if(X[operand1] != X[operand2]) pc += imm;
    else if(subtype == "bge") if(X[operand1] >= X[operand2]) pc += imm;
    else if(subtype == "blt") if(X[operand1] < X[operand2]) pc += imm;
  }
  else if(Type == 'J'){//jal
    des_res = pc+4; pc += imm;
  }
  else if(Type == 'S'){//sb, sw, sh
    X[operand2] = X[operand1]+imm;
  }
  else if(Type == 'U'){//auipc, lui
    if(subtype == "auipc") des_res = pc + (imm<<12) ;
    else if(subtype == "lui") des_res = imm<<12;
  }
}
// perform the memory operation
void mem()
{
  
}
// writes the results back to register file
void write_back()
{
}

// should be called when instruction is swi_exit


void run_riscvsim()
{
  while (1)
  {
    fetch();
    decode();
    // execute();
    // mem();
    // write_back();
    break;
  }
}

int main()
{
  // reset the processor
  reset_proc();
  // load the program memory
  load_program_memory();
  // run the simulator
  run_riscvsim();
  return 0;
}