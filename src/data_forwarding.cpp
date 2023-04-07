/*
The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date:
*/
/* myRISCVSim.cpp
   Purpose of this file: implementation file for myRISCVSim
*/
#include <bits/stdc++.h>
using namespace std;

#define M 32
void write_data_memory();
static unsigned int X[32];     // 32 resitors
static unsigned int MEM[4000]; // Instruction memory
static int DMEM[1000000];      // give lui in range of 0x00010 (Data memory)
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;
// Global variables
char Type = '0';
// static bitset<M> inst;       // 32 bits instruction
static unsigned int des_reg; // destination resistor
static int des_res;          // destination result after ALU step
static string subtype;              // subtype of the instruction
static int imm;              // immediate value if a immediate instruction
static int pc = -4;           // PC value
unsigned int sz = 0;         // total number of instructions in the assembly program
static int stall = 0, stop = 0;
// static int IF_DE, DE_EX, EX_MA, MA_WB, WB_R; // pipeline registers
int cycle_no = 1;
int Fetchflag = 0;
int endflag = 0;
int rflag = 0;
int inst_end = 1, stalls = 0,Tall = 0, CPI, Ld_St = 0, ALU = 0, Control = 0, Data_H = 0, Con_H = 0, Branch_Mis = 0, Stalls_D = 0, Stalls_C = 0;
struct fetch_decode{
    bitset<M> curr_inst;
} IF_DE;

struct decode_execute{   
    int Stall,rs1, rs2,rs1_val, rs2_val,rd2,immed;
    char type;
    string Subtype;
} DE_EX;

struct execute_mem{
    int Stall,rs1, rs2,rd_value,rd2;
    char type;
    string Subtype;
    char check;
} EX_MA;

struct mem_write{
    int Stall,rd_value,rd2, rs1, rs2;
    char type;
    string Subtype;
} MA_WB;

// determing the type pf the instruction
char op_R_type(bitset<7> op)
{
    bitset<7> opr("0110011");
    if (op == opr)  return 'R';
    else    return '0';
}
char op_I_type(bitset<7> op)
{
    bitset<7> opi1("0010011");
    bitset<7> opi2("1100111");
    bitset<7> opi3("0000011");
    if (op == opi1 || op == opi2 || op == opi3) return 'I';
    else     return '0';
}
char op_J_type(bitset<7> op)
{
    bitset<7> opj("1101111");
    if (op == opj)  return 'J';
    else    return '0';
}
char op_B_type(bitset<7> op)
{
    bitset<7> opb("1100011");
    if (op == opb)  return 'B';
    else    return '0';
}
char op_S_type(bitset<7> op)
{
    bitset<7> ops("0100011");
    if (op == ops)  return 'S';
    else return '0';
}
char op_U_type(bitset<7> op)
{
    bitset<7> opu1("0110111");
    bitset<7> opu2("0010111");
    if (op == opu1 || op == opu2)  return 'U';
    else  return '0';
}
// end checking
string findTwoscomplement(string str) // for the finding the twos complement as requied in calculation of pc after sign extension of immediate
{
    int n = str.length();
    int i = 0;
    // Traverse the string to get first '1' from
    // the last of string
    for (i = n - 1; i >= 0; i--)  if (str[i] == '1')  break;
    // If there exists no '1' concatenate 1 at the
    // starting of string
    if (i == -1)    return '1' + str;
    for (int k = i - 1; k >= 0; k--){
        if (str[k] == '1')  str[k] = '0';
        else str[k] = '1';
    }
    return str;
}

//  Selection the subtype of instruction for example addi,jal,beg etc
//  Function will take func3 , func7 and op as input and return subtype as a string from the type of instruction
string subtype_select(bitset<3> func3, bitset<7> func7, bitset<7> op)
{
    string Func3 = func3.to_string(), Func7 = func7.to_string(), Op = op.to_string();
    switch (Type)
    {
        case 'R':
        {
            if (Func3 == "000" && Func7 == "0000000")   {   subtype = "add";    }
            else if (Func3 == "111" && Func7 == "0000000")  {   subtype = "and";    }
            else if (Func3 == "110" && Func7 == "0000000")  {   subtype = "or"; }
            else if (Func3 == "001" && Func7 == "0000000")  {   subtype = "sll";    }
            else if (Func3 == "010" && Func7 == "0000000")  { subtype = "slt";    }
            else if (Func3 == "101" && Func7 == "0100000")  {  subtype = "sra";  }
            else if (Func3 == "101" && Func7 == "0000000")  {  subtype = "srl"; }
            else if (Func3 == "000" && Func7 == "0100000")  { subtype = "sub"; }
            else if (Func3 == "100" && Func7 == "0000000")  { subtype = "xor"; }
            break;
        }
        case 'I':
        {
            if (Func3 == "000" && Op == "0010011")   {  subtype = "addi";  }
            else if (Func3 == "111")  {   subtype = "andi";   }
            else if (Func3 == "110")  {  subtype = "ori"; }
            else if (Func3 == "000" && Op == "0000011")   {  subtype = "lb";  }
            else if (Func3 == "001" && Op == "0000011")   {  subtype = "lh"; }
            else if (Func3 == "010" && Op == "0000011")   {  subtype = "lw";  }
            else if (Func3 == "000" && Op == "1100111")   {  subtype = "jalr";  }
            else if (Func3 == "001" && Op == "0010011")   {  subtype = "slli";  }
            break;
        }
        case 'B':
        {
            if (Func3 == "000")  { subtype = "beq"; }
            else if (Func3 == "001")  {  subtype = "bne";  }
            else if (Func3 == "101")  {   subtype = "bge";  }
            else if (Func3 == "100")  {  subtype = "blt";}
            break;
        }
        case 'J':
        {   subtype = "jal";
            break;
        }
        case 'S':
        {
            if (Func3 == "000") {   subtype = "sb";  }
            else if (Func3 == "001") {  subtype = "sh";}
            else if (Func3 == "010") { subtype = "sw"; }
            break;
        }
        case 'U':{
            if (Op == "0010111") {   subtype = "auipc"; }
            else if (Op == "0110111") {  subtype = "lui"; }
            break;
        }
        default:    { cout << "error" << endl; }
    }
    return Func3;
}

void load_resister()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("register.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening dataout.mem file for writing, ");
        return;
    }
    for (i = 0; i < 32; i++)
    {
        fprintf(fp, "x%u - %u \t\t\t\t\t                                           \n", i, X[i]);
    }
    fclose(fp);
}

void load_Memory()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("D_Memory.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening dataout.mem file for writing, ");
        return;
    }
    for (i = 0; i < 1000000; i += 4)
    {   fprintf(fp, "%u \t\t\t\t\t                                           \n", DMEM[i]);}
    fclose(fp);
}
// function to exit the program
void swi_exit()
{
    //  write_data_memory();
    inst_end = 0;
}
// before running any set of instructions initializing the values of resistors, Instruction and Data memory
//  every resitor to 0 except the stack pointer whick will point to the last index of data memory as for the recurssion to work
void reset_proc()
{
    for (auto i : X)    {  i = 0; }
    X[2] = 999999;
    for (auto p : MEM) {   p = 0;}
    for (auto q : DMEM) q = 0;
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
    sz++; // Determing how many instructions are there
}
// read and write ends
void write_data_memory()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("data_out.mem", "w");
    if (fp == NULL) { printf("Error opening dataout.mem file for writing, "); return; }
    for (i = 0; i <= 4 * sz; i = i + 4) { fprintf(fp, "%u %u\n", i, read_word(MEM, i)); }
    fclose(fp);
}
void load_program_memory()
{
    FILE *fp;
    unsigned int address, instruction;
    fp = fopen("test_case.mem", "r"); // reading from the file
    if (fp == NULL){   
        printf("Error opening input mem file, ");   exit(1);
    }
    while (fscanf(fp, "%x %x", &address, &instruction) != EOF) // instruction is stored in instruction variable and PC in address variable
    {   write_word(MEM, address, instruction);  }
    write_data_memory();
    fclose(fp);
}
// reads from the instruction memory and updates the instruction register
void fetch()
{
    cycle_no++;
    cout<<"fetching : "<<endl;
    if (Fetchflag != 2)
    {
        pc += 4; 
        Tall = 0;
        IF_DE.curr_inst = MEM[pc];
        string insth;
        bitset<32> pac(pc);
        cout << "Fetch instruction : 0x" << hex << (IF_DE.curr_inst).to_ulong() << " from address 0x" << hex << pac.to_ulong() << endl;
        bitset<32> exitcode("11101111000000000000000000010001"); 
        // end when come across 0xEF000011 as the instruction
                // cout<<endflag<<endl
        if (endflag==1 && EX_MA.check=='T') {
            // swi_exit();
            endflag=0;
        }
        else if (endflag==1 && EX_MA.check=='F') {
            swi_exit();
        }
        if (IF_DE.curr_inst == exitcode && (DE_EX.type=='B' || DE_EX.type=='J' || DE_EX.Subtype=="jalr")) {
            endflag = 1;
            Fetchflag = 2;
            // cout<<endflag<<endl;
        }
        else if (IF_DE.curr_inst == exitcode){   swi_exit(); }
    }
   else if (Fetchflag==2 && EX_MA.check=='T') {Fetchflag=0; cout<<"Branch Stall\n";}
    else if (Fetchflag==2 && EX_MA.check=='F') {Fetchflag=0; cout<<"Branch Stall\n"; pc-=4;}
    else {cout<<"STALLED  : "<<stall <<endl;}
}
// reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode()
{
    cout << "Decode :" << endl;
    bitset<7> op;
    bitset<7> func7;
    bitset<3> func3;
    bitset<5> rs1, rs2, rd;
    bitset<M> inst = IF_DE.curr_inst;
    for (int i = 0; i < 7; i++) // determining the OP Code
    {   op[i] = inst[i];    }
    while (1) // Determining the Type of the instruction using the OP Code
    {
        if (Type == '0') {Type = op_R_type(op);}
        if (Type == '0') Type = op_I_type(op);
        if (Type == '0') Type = op_J_type(op);
        if (Type == '0') Type = op_B_type(op);
        if (Type == '0')    Type = op_S_type(op);
        if (Type == '0')    Type = op_U_type(op);
        break;
    }
    cout << "Format of instruction : " << Type << endl;
    // Determing the Func3 , Func 7 and value of rs1 an rs2 in binary form
    int j = 0;
    for (int i = 25; i < 32; i++){ func7[j] = inst[i]; j++; }
    j = 0;
    for (int i = 12; i < 15; i++){ func3[j] = inst[i]; j++;}
    j = 0;
    for (int i = 15; i < 20; i++){rs1[j] = inst[i];j++;}
    operand1 = rs1.to_ulong(); // converting the binary string to unsigned decimal to get the registor 1
    j = 0;
    for (int i = 20; i < 25; i++){ rs2[j] = inst[i]; j++;}
    operand2 = rs2.to_ulong(); // converting the binary string to unsigned decimal to get the registor 2
    switch (Type)
    {
        case 'R':
        {
            j = 0;
            for (int i = 7; i < 12; i++) // Determining destination registor (rd) in binary form
            {   rd[j] = inst[i];    j++;  }
            des_reg = rd.to_ulong(); // converting the binary string to unsigned decimal to get the destination resistor
            cout << "Operand1 : " << operand1 << ", "<< "Operand2 : " << operand2 << ", "<< "RD : " << des_reg << endl;
    
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && cycle_no > 2 && DE_EX.rd2 != 0)       {stall = 4;Tall = 4; DE_EX.Stall = stall-1;}
            // else if ((operand1 == EX_MA.rd2 || operand2 == EX_MA.rd2) && cycle_no > 3 && DE_EX.rd2 != 0 )    {stall = 3; Tall = 3;}
            else if ((operand1 == MA_WB.rd2 || operand2 == MA_WB.rd2)&& cycle_no > 4 && MA_WB.rd2 != 0)    {stall = 3; Tall = 3; EX_MA.Stall = stall-1;}
            break;
        }
        case 'I':
        {
            bitset<12> immb;
            j = 0;
            bool isneg = false;
            for (int i = 7; i < 12; i++){   rd[j] = inst[i];    j++;}
            des_reg = rd.to_ulong();
            j = 0;
            // determining the immediate of 12 bits
            for (int i = 20; i < 32; i++){  immb[j] = inst[i];  j++; }
            // Sign extension of the immediate
            if (immb[11] == 1){                 // if MSB of 12 bits immediate is 1
                isneg = true; // Immediate is negative and should be extended to 32 bits accordingly (arithematically)
            }
            if (immb[11] == 1){
                string s1 = immb.to_string();
                string s = findTwoscomplement(s1);
                bitset<12> opl(s);
                immb = opl;
            }
            imm = immb.to_ulong();
            if (isneg)  {imm = -1 * imm;}
            cout << "immediate : " << imm << ", "<< "Operand1 : " << operand1 << ", "<< "RD : " << des_reg << endl;
            if (operand1 == DE_EX.rd2 && cycle_no > 2 && DE_EX.rd2 != 0)       {stall = 4;Tall = 4; DE_EX.Stall = stall-1;}
            else if (operand1 == MA_WB.rd2 && cycle_no > 4 && MA_WB.rd2 != 0)    {stall = 3;Tall = 3; EX_MA.Stall = stall-1;}
            break;
        }
        case 'S':
        {
            bitset<12> immb;
            j = 0;
            for (int i = 7; i < 12; i++){immb[j] = inst[i];j++;}
            for (int i = 25; i < 32; i++){ immb[j] = inst[i]; j++; }
            // Sign extension of the immediate
            imm = immb.to_ulong();
            if (immb[11] == 1)
            {
                imm = -1 * imm;
            }
            cout << "immediate : " << imm << ", "<< "Operand1 : " << operand1 << ", "<< "operand2 : " << operand2 << endl;
            //pass the types in pipeline reg
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && DE_EX.rd2 != 0)      { stall = 4; Tall = 4; DE_EX.Stall = stall-1;}
            else if (operand1 == MA_WB.rd2 || operand2 == MA_WB.rd2 && MA_WB.rd2 != 0)    {stall = 3; Tall = 3; EX_MA.Stall = stall-1;}
            break;
        }
        case 'B':
        {
            bitset<13> immb;
            bool isneg = false;
            immb[0] = 0;
            j = 1;
            for (int i = 8; i < 12; i++){ immb[j] = inst[i];j++;}
            for (int i = 25; i < 31; i++){immb[j] = inst[i];j++;}
            immb[j] = inst[7];
            j++;
            immb[j] = inst[31];
            // Sign extension of the immediate
            // if MSB of 12 bits immediate is 1
            // Immediate is negative and should be extended to 32 bits accordingly (arithematically)
            if (immb[12] == 1) {isneg = true;}
            if (immb[12] == 1){
                string s1 = immb.to_string();
                string s = findTwoscomplement(s1);
                bitset<13> opl(s);
                immb = opl;
            }
            imm = immb.to_ulong();
            if (isneg){ imm = -1 * imm; }
            cout << "immediate : " << imm << ", " << "Operand1 : " << operand1 << ", "<< "operand2 : " << operand2 << endl;
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && DE_EX.rd2 != 0)   { stall = 4; Tall = 4; DE_EX.Stall = stall-1;}
            else if ((operand1 == MA_WB.rd2 || operand2 == MA_WB.rd2) && MA_WB.rd2 != 0)   {stall = 3; Tall = 3; EX_MA.Stall = stall-1;}
            break;
        }
        case 'U':
        {
            j = 0;
            for (int i = 7; i < 12; i++){ rd[j] = inst[i]; j++; }
            des_reg = rd.to_ulong();
            bitset<20> immb;
            j = 0;
            for (int i = 12; i < 32; i++){ immb[j] = inst[i]; j++; }
            imm = immb.to_ulong();
            // the immediate value in the LUI (Load Upper Immediate) and AUIPC (Add Upper Immediate to PC) instructions in RISC-V cannot be negative.So Sign extension required.
            cout << "immediate : " << imm << ", "<< "RD : " << des_reg << endl;
            break;
        }
        case 'J':
        {
            j = 0;
            bitset<21> immb;
            bool isneg = false;
            j = 0;
            immb[0] = 0;
            j = 12;
            for (int i = 12; i < 20; i++){ immb[j] = inst[i]; j++;}
            immb[11] = inst[20];
            j = 1;
            for (int i = 21; i < 31; i++){ immb[j] = inst[i]; j++;  }
            immb[20] = inst[31];
            if (immb[20] == 1){isneg = true;}
            if (immb[20] == 1){
                string s1 = immb.to_string();
                string s = findTwoscomplement(s1);
                bitset<21> opl(s);
                immb = opl;
            }
            imm = immb.to_ulong();
            if (isneg){imm = -1 * imm;}
            j = 0;
            for (int i = 7; i < 12; i++){
                rd[j] = inst[i];
                j++;
            }
            des_reg = rd.to_ulong();
            cout << "immediate : " << imm << ", "<< "RD : " << des_reg << endl;
            break;
        }
        default:{cout << "error" << endl;}
    }
    DE_EX.rs1 = operand1; DE_EX.rs2 = operand2;
    // DE_EX.rs1_val=X[operand1];
    // DE_EX.rs2_val=X[operand2];
    DE_EX.immed = imm; DE_EX.type = Type;
    // Type = '0';
    DE_EX.rd2 = des_reg; 
    DE_EX.Stall = stall;
    subtype_select(func3, func7, op);
    DE_EX.Subtype = subtype;
    cout << "Operation is " << subtype << endl;
}
// executes the ALU operation based on ALUop
void execute()
{
    char ch = 'O';
    cout << "Execute : " << endl;
    operand1 = DE_EX.rs1;operand2 = DE_EX.rs2;Type = DE_EX.type;des_reg = DE_EX.rd2;imm = DE_EX.immed; 
    subtype = DE_EX.Subtype;
    if (Type == 'R'){ // add, and, or, sll, slt, sra, srl, sub, xor
        if(stall == 4){
            if(EX_MA.rd2==operand1 ) {X[operand1]=EX_MA.rd_value;cout<<"hgfashklfa "<<X[operand1]<<endl; }
            else if(EX_MA.rd2==operand2){X[operand2]=EX_MA.rd_value; }stall = 0;
        }
        if (subtype == "add"){
            des_res = X[operand1] + X[operand2];
            cout << "Adding " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "sub"){
            des_res = X[operand1] - X[operand2];
            cout << "Subtracting " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "and"){
            des_res = X[operand1] & X[operand2];
            cout << "Bitwise AND " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "or"){
            des_res = X[operand1] | X[operand2];
            cout << "Bitwise OR " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "sll"){
            des_res = X[operand1] << X[operand2];
            cout << "Shift Left " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "slt"){
            des_res = (X[operand1] < X[operand2]) ? 1 : 0;
            cout << "Set Less Than in " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "sra"){
            des_res = X[operand1] >> X[operand2];
            cout << "Shift Right Arithmatic" << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "xor"){
            des_res = X[operand1] ^ X[operand2];
            cout << "Bitwise XOR " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "srl"){
            des_res = X[operand1] >> X[operand2];
            cout << "Shift Right Logical " << X[operand1] << " and " << X[operand2] << endl;
        }
    }
    else if (Type == 'I'){ // addi, andi, ori, lb, lh, lw, jalr
        if(stall==4){ X[operand1]=EX_MA.rd_value; stall = 0;}
        if (subtype == "addi"){
            des_res = X[operand1] + imm;
            cout << "Adding " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "andi"){
            des_res = X[operand1] & imm;
            cout << "Bitwise AND " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "ori"){
            des_res = X[operand1] | imm;
            cout << "Bitwise OR " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "lb" || subtype == "lh" || subtype == "lw"){
            des_res = X[operand1] + imm;
            cout << "Calculating net memory address by Adding " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "jalr"){
            des_res = pc;
            pc = X[operand1] + imm - 4;
            cout << "Calculating net memory address by adding " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "slli"){
            des_res = X[operand1] << imm;
            cout << "Shift Left " << operand1 << "and" << operand2 << endl;
        }
    }
    else if (Type == 'B'){ // beq, bne, bge, blt
        if(stall == 4){
            if(EX_MA.rd2==operand1 ) {X[operand1]=EX_MA.rd_value;}
            else if(EX_MA.rd2==operand2){X[operand2]=EX_MA.rd_value;}stall = 0;
        }
        if (subtype == "beq"){pc -= 4;
            if (X[operand1] == X[operand2]){
                ch='T';
                cout << "if operand1 and operand2 are equal then Adding " << pc << " and " << imm << endl;
                pc += imm-4; Fetchflag=2;
                DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                //control hazard
            }
            else{ch='F';cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << 4 << endl;des_res = 0;des_reg = 0;}
        }
        else if (subtype == "bne"){pc -= 4;
            if (X[operand1] != X[operand2]){
                ch='T';
                 cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << imm << endl;
                pc += imm-4; Fetchflag=2;
                DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                //control hazard
            }
            else{ch='F';cout << "if operand1 and operand2 are equal then Adding " << pc << " and " << 4 << endl;des_res = 0;des_reg = 0;}
        }
        else if (subtype == "bge"){pc -= 4;
            if (X[operand1] >= X[operand2]){
                ch='T';
                cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " << imm << endl;
                pc += imm-4; Fetchflag=2;
                DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                //control hazard
            }
            else{ch='F';cout << "if operand1 less than operand2 then Adding " << pc << " and " << 4 << endl;des_res = 0;des_reg = 0;}
        }
        else if (subtype == "blt"){pc -= 4;
            if (X[operand1] < X[operand2]){
                ch='T';
                cout << "if operand1 less than operand2 then Adding " << pc << " and " << imm << endl;
                pc += imm-4; Fetchflag=2;
                DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                //control hazard
            }
            else{ch='F';cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " << 4 << endl;des_res = 0;des_reg = 0;}
        }
    }
    else if (Type == 'J'){ // jal
        des_res = pc;
        pc-=4; cout << "Calculating net memory address by adding " << operand1 << " and " << imm << endl;
        pc += imm - 4; Fetchflag=2;
        DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;
    }
    else if (Type == 'S'){ // sb, sw, sh
        if(stall == 4){
            if(EX_MA.rd2==operand1 ) {X[operand1]=EX_MA.rd_value;}
            else if(EX_MA.rd2==operand2){X[operand2]=EX_MA.rd_value;}stall = 0;
        }
        des_res = X[operand1] + imm;
        cout << "Calculating net memory address by Adding " << X[operand1] << " and " << imm << endl;
    }
    else if (Type == 'U'){ // auipc, lui
        if (subtype == "auipc"){
            des_res = (pc - 4) + (imm << 12);
            cout << "Adding pc and Shifted Imm " << pc << " and " << (imm << 12) << endl;
        }
        else if (subtype == "lui"){des_res = imm << 12;cout << "Shifted Imm " << (imm << 12) << endl;}
    }
    EX_MA.rs1 = operand1;EX_MA.rs2 = operand2;EX_MA.rd_value = des_res;EX_MA.rd2 = des_reg;EX_MA.type = Type;
    EX_MA.Subtype = subtype;
    EX_MA.check = ch;
    // cout<<"desres "<< des_res<<endl;
    Type = '0';
}
// perform the memory operation
void mem()
{
    cout << "Memory :\n";
    operand1 = EX_MA.rs1; operand2 = EX_MA.rs2; des_reg = EX_MA.rd2; des_res = EX_MA.rd_value; Type = EX_MA.type; subtype = EX_MA.Subtype;
    // Memory operation is required only in Load and store instructions
    if (subtype == "lw" || subtype == "lh" || subtype == "lb")
    {
        cout << "Loading from data memory : " << des_res << endl;
        des_res = DMEM[des_res]; // Loading value from memory to resister
        MA_WB.rd_value = des_res; MA_WB.rd2 = des_reg;MA_WB.type = Type; MA_WB.Subtype = subtype;
    }
    else if (subtype == "sw" || subtype == "sh" || subtype == "sb"){
        DMEM[des_res] = X[operand2]; // Loading value from resistor to memory
        MA_WB.rd_value = DMEM[des_res]; MA_WB.rd2 = 0;MA_WB.type = Type; MA_WB.Subtype = subtype;
        cout << "Storing " << X[operand2] << " into data memory : " << des_res <<" "<< endl;
    }
    else{
        cout << "No Memory operation " << endl; cout<<"des_res "<<des_res<<" des_reg "<<des_reg<<endl;
        MA_WB.rd_value = des_res; MA_WB.rd2 = des_reg; MA_WB.type = Type; MA_WB.Subtype = subtype;
    }
    MA_WB.rs1 = operand1;
    MA_WB.rs2 = operand2;
    if(stall == 4){
        EX_MA.rs1 = operand1;EX_MA.rs2 = operand2;EX_MA.rd_value = des_res;EX_MA.rd2 = des_reg;EX_MA.type = Type;
        EX_MA.Subtype = subtype;
    }
}
// writes the results back to register file
void write_back()
{
    cout << "WriteBack :" << endl;
    operand1 = MA_WB.rs1; operand2 = MA_WB.rs2; des_reg = MA_WB.rd2; des_res = MA_WB.rd_value; Type = MA_WB.type;
    if (Type != 'S' && Type != 'B'){
        X[des_reg] = des_res;
        cout << "Storing " << des_res << " into x" << des_reg << endl;
    }
    else    cout << "No WriteBack Operation \n";
    X[0] = 0;// Ensuring that X0 will always be 0 and wont get overwritten.
    Type = '0';
}
void run_riscvsim()
{
    while (1)
    {
        if (inst_end > 0){
            cout<<endl; cout << "CYCLE NUMBER: " <<dec << cycle_no << endl;
            if (cycle_no > 4) {write_back();}
            if (cycle_no > 3){mem();}
            if (cycle_no > 2){execute();}
            if (cycle_no > 1){decode();}
            fetch();
        }
        else{
            if (cycle_no<=4) {cycle_no++; rflag++;}
            int finish = cycle_no + 3 ; if (rflag==1) {cycle_no--; rflag--;}
            while (finish != cycle_no)
            { cout<<endl; cout << "CYCLE NUMBER: " <<dec << cycle_no << endl;
                if (cycle_no < finish && cycle_no>4) {write_back();}
                if (cycle_no < finish - 1) {mem();}
                if (cycle_no < finish - 2) {execute();}
                if (cycle_no < finish - 3) {decode();}
                cycle_no++;
            }
            cycle_no--;
            break;
        }
    }
    load_resister();
    load_Memory();
}