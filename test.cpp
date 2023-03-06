# include<bits/stdc++.h>
using namespace std;

#define M 32
static unsigned int X[32];
static unsigned int MEM[4000]; 
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;
static bitset<M> inst;

// checking the instruction set
char op_R_type(bitset<7> op){ bitset<7> opr("0110011");
  if(op == opr) return 'R';
  else return '0';
}
char op_I_type(bitset<7> op){ bitset<7> opi1("0010011"); bitset<7> opi2("0000011");
  if(op == opi1 || op == opi2) return 'I';
  else return '0';
}
char op_J_type(bitset<7> op){ bitset<7> opj("1101111");
  if(op == opj) return 'J';
  else return '0';
}
char op_B_type(bitset<7> op){ bitset<7> opb("1100011");
  if(op == opb) return 'B';
  else return '0';
}
char op_S_type(bitset<7> op){ bitset<7> ops("0100011");
  if(op == ops) return 'S';
  else return '0';
}
char op_U_type(bitset<7> op){ bitset<7> opu1("0110111"); bitset<7> opu2("0010111");
  if(op == opu1 || op == opu2) return 'U';
  else return '0';
}
//end checking 

void reset_proc() {
  for(auto i : X){
    i = 0;
  }
  for(auto p : MEM){
    p = 0;
  }
}

//read and write in file start
int read_word(unsigned int *mem, unsigned int address) 
{
  int *data;
  data =  (int*) (mem + address);
  return *data;
}

void write_word(unsigned int *mem, unsigned int address, unsigned int data) 
{
  int *data_p;
  data_p = (int*) (mem + address);
  *data_p = data;
}
//read and write ends


void write_data_memory() {
  FILE *fp;
  unsigned int i;
  fp = fopen("data_out.mem", "w");
  if(fp == NULL) {
    printf("Error opening dataout.mem file for writing\n");
    return;
  }
  
  for(i=0; i <= 12; i = i+4){
    fprintf(fp, "%u %u\n", i, read_word(MEM, i));
  }
  fclose(fp);
}

void load_program_memory() 
{
  FILE *fp;
  unsigned int address, instruction;
  fp = fopen("simple_add.mem", "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    write_word(MEM, address, instruction);
  }
  write_data_memory();
  fclose(fp);
}

//reads from the instruction memory and updates the instruction register
void fetch() {
  inst = MEM[4];
  cout<<inst;
}
//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode() {
  bitset<7> op,func3,func7,op1,op2;
  for(int i =0 ;i<7; i++){
    op[i] = inst[i];
  }
  cout<<"\n"<<op;
  char s = '0';
  while(1){
    if(s == '0') {s = op_R_type(op);}
    if(s == '0') s = op_I_type(op);
    if(s == '0') s = op_J_type(op);
    if(s == '0') s = op_B_type(op);
    if(s == '0') s = op_S_type(op);
    if(s == '0') s = op_U_type(op);
    break;
  }
  cout<<"\n"<<s;
}
//executes the ALU operation based on ALUop
void execute() {
}
//perform the memory operation
void mem() {
}
//writes the results back to register file
void write_back() {
}

//should be called when instruction is swi_exit
// void swi_exit() {
//   write_data_memory();
//   exit(0);
// }


void run_riscvsim() {
  while(1) {
    // bitset<M> bset;
    fetch();
    decode();
    // execute();
    // mem();
    // write_back();
    break;
  }
}


int main(){
  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory();
  //run the simulator
  run_riscvsim();
  return 0;
}