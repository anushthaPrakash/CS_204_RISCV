#include <bits/stdc++.h>
#include<iostream>
#include<queue>
using namespace std;

#define CACHE_SIZE 64 * 10246 // 64KB ICache // will be input from user
#define BLOCK_SIZE 16         // 16-byte block size //INPUT from user
long long CacheSz, BlockSz;
// no. of blocks
int no_ways;long long int blocks;int block_offset;int tag;int old_flag = 0;
int inst_cache_hit, inst_cache_miss, inst_cache_coldmiss, inst_cache_capacitymiss, inst_cache_conflictmiss, hitflag=0,hitflag2=0;
int misss = 2, misss2=0;
string format, Policy;
int no_adress_bits;
#define M 32
void write_data_memory();
static unsigned int X[32];     // 32 resitors
static unsigned int MEM[4000]; // Instruction memory
static int DMEM[1000000];      // give lui in range of 0x00010 (Data memory)
map<int, int> BrPr;
static unsigned int instruction_word;
static unsigned int operand1;
static unsigned int operand2;
// Global variables
char Type = '0';
// static bitset<M> inst;       // 32 bits instruction
static unsigned int des_reg; // destination resistor
static int des_res;          // destination result after ALU step
static string subtype;       // subtype of the instruction
static int imm;              // immediate value if a immediate instruction
static int pc = -4;          // PC value
unsigned int sz = 0;         // total number of instructions in the assembly program
static int stall = 0, stop = 0;
// static int IF_DE, DE_EX, EX_MA, MA_WB, WB_R; // pipeline registers
int cycle_no = 1,m =0, n =0, o=0;int Fetchflag = 0;int endflag = 0;int rflag = 0;
int inst_end = 1, stalls = 0, Tall = 0, Ld_St = 0, ALU = 0, Control = 0, Data_H = 0, Con_H = 0, Branch_Mis = 0, Stalls_D = 0, Stalls_C = 0;
int  T_inst=0, ALU_inst = 0, Control_inst = 0;
float CPI;
vector<int> dhcycle, chcycle;
// m and n are the row and column number of ICache 

typedef struct {
    string addrs;
    int mad;
    int nad;
} ADDR;

queue<ADDR> fifo;

struct fetch_decode{
    bitset<M> curr_inst;
    int progcont;
} IF_DE; //IF_DE Pipeline Register

struct decode_execute{   
    int rs1, rs2,rs1_val, rs2_val,rd2,immed;
    int progcont;
    char type;
    string Subtype;
} DE_EX; //DE_EX Pipeline Register

struct execute_mem{
    int rs1, rs2,rd_value,rd2;
    char type;
    string Subtype;
    char check;
} EX_MA; //EX_MA Pipeline Register

struct mem_write{
    int rd_value,rd2, rs1, rs2;
    char type;
    string Subtype;
} MA_WB; //MA_WB Pipeline Register

// Define Cache Block structure
void swi_exit() {   inst_end = 0;  }
void reset_proc()
{
    for (auto i : X)    {    i = 0; }
    X[2] = 999999;
    for (auto p : MEM)  {    p = 0;     }
    for (auto q : DMEM)     q = 0;
}

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
void write_data_memory()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("data_out.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening dataout.mem file for writing, ");
        return;
    }
    for (i = 0; i <= 4 * sz; i = i + 4)     {   fprintf(fp, "%u %u\n", i, read_word(MEM, i));   }
    fclose(fp);
}
void load_program_memory()
{
    FILE *fp;
    unsigned int address, instruction;
    fp = fopen("test_case.mem", "r"); // reading from the file
    if (fp == NULL)
    {
        printf("Error opening input mem file, ");
        exit(1);
    }
    while (fscanf(fp, "%x %x", &address, &instruction) != EOF) // instruction is stored in instruction variable and PC in address variable
    {
        write_word(MEM, address, instruction);
    }
    write_data_memory();
    fclose(fp);
}

int getRandomNumber(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL)); // seed the random number generator
        initialized = true;
    }
    int range = max - min + 1;
    return min + rand() % range;
}

typedef struct cache_block
{
    string tag;
    string B_off;
    string index; // can be set number bits
    string address;
    int valid;
} CacheBlock;

queue<CacheBlock> FetchData, FetchTotalData;

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
        default:   {cout<<"";}
    }
    return Func3;
}

//Loads all register values in register.mem file
void load_register()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("register.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening dataout.mem file for writing.\n");
        return;
    }
    for (i = 0; i < 32; i++)
    {
        fprintf(fp, "x%u - %u                                                                                 \n", i, X[i]);
    }
    fclose(fp);
}

void load_randata()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("CountData.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening CountData.mem file for writing.\n");
        return;
    }
    fprintf(fp, "Total cycles = %d                                                                \n", cycle_no);
    fprintf(fp, "Total instructions = %d                                                          \n", T_inst);
    fprintf(fp, "Cycles per instruction = %f                                                      \n", CPI);
    fprintf(fp, "Data transfer instructions = %d                                                  \n", Ld_St);
    fprintf(fp, "ALU instructions = %d                                                            \n", ALU_inst);
    fprintf(fp, "Cntrol instructions = %d                                                         \n", Control_inst);
    fprintf(fp, "Total stalls = %d                                                                \n", 0);
    fprintf(fp, "Data hazards found = %d                                                          \n", 0);
    fprintf(fp, "Control hazards found = %d                                                       \n", 0);
    fprintf(fp, "Branch mispredictions = %d                                                       \n", Branch_Mis);
    fprintf(fp, "Data hazard stalls = %d                                                          \n", 0);
    fprintf(fp, "Control hazard stalls = %d                                                       \n", 0);
    fclose(fp);
}

void load_Memory()
{
    FILE *fp;
    unsigned int i;
    fp = fopen("D_Memory.mem", "w");
    if (fp == NULL)
    {
        printf("Error opening D_Memory.mem file for writing.\n");
        return;
    }
    for (i = 0; i < 1000000; i += 4)
    {   fprintf(fp, "%u                                                                                  \n", DMEM[i]);}
    fclose(fp);
}

void load_dhazards() {
    FILE *fp;
    unsigned int i;
    fp = fopen("DH_Cycle.mem", "w");
    if (fp == NULL)
    {
        printf("no data hazards present\n");
        return;
    }
    int siz = dhcycle.size();
    for (i = 0; i < siz; i++)
    {   fprintf(fp, "Cycle number: %d\n", dhcycle[i]);}
    fclose(fp);
}

// void load_dcache() {
//     
// }

//Loads all cycles in which there are control hazards 
void load_chazards() {
    FILE *fp;
    unsigned int i;
    fp = fopen("CH_Cycle.mem", "w");
    if (fp == NULL)
    {
        printf("no control hazards\n");
        return;
    }
    int siz = chcycle.size();
    for (i = 0; i < siz; i++)
    {   fprintf(fp, "Cycle number: %d\n", chcycle[i]);}
    fclose(fp);
}

// Define Cache class
typedef struct
{
    int size;              // Cache size in bytes
    int block_size;        // Block size in bytes
    int num_block;         // number of blocks in ICache
    string assoc;          // Associativity
    int n_sets;            // k way set associative ? so store the value of k
    string policy;         // Replacement policy (LRU, FIFO, Random, LFU)
    int hit_time;          // Hit time in cycles
    int miss_penalty;      // Miss penalty in cycles
    int n_accesses;        // Number of ICache accesses
    int n_hits;            // Number of ICache hits
    int n_misses;          // Number of ICache misses
    int n_cold_misses;     // Number of cold misses
    int n_conflict_misses; // Number of conflict misses
    int n_capacity_misses; // Number of capacity misses
    int index_bits;        // number of index bits
    int offset_bits;
    int tag_bits;
    int set_bits;
    // CacheBlock *blocks;
    
    // Memory memory; // assume Memory struct is defined elsewhere
} Cache_pro;
map<int, map<int, CacheBlock>> ICACHE;
map<int, map<int, CacheBlock>> DCACHE;
Cache_pro ICache, DCache;

void load_icache() {
    unsigned int i;
    freopen("ICACHE.mem", "w",stdout);
    for (int i=0;i<CacheSz/BlockSz;i++) {
        for (int j=0;j<BlockSz/4;j++) {
            cout<<ICACHE[i][j].address<<" ";
        } cout<<endl;
    }
}

void load_dcache() {
    unsigned int i;
    freopen("DCACHE.mem", "w",stdout);
    for (int i=0;i<CacheSz/BlockSz;i++) {
        for (int j=0;j<BlockSz/4;j++) {
            cout<<DCACHE[i][j].address<<" ";
        } cout<<endl;
    }
}

void load_icachedata() {
    FILE *fp;
    unsigned int i;
    fp = fopen("ICacheData.mem", "w");
    if (fp == NULL)
    {
        printf("No file exists.\n");
        return;
    }
    fprintf(fp, "Number of accesses: %d\n", ICache.n_accesses);
    fprintf(fp, "Number of hits: %d\n", ICache.n_accesses-ICache.n_misses);
    fprintf(fp, "Number of cold misses: %d\n", ICache.n_cold_misses);
    fprintf(fp, "Number of conflict misses: %d\n", ICache.n_conflict_misses);
    fprintf(fp, "Number of capacity misses: %d\n", ICache.n_capacity_misses);
    fprintf(fp, "Total misses: %d\n", ICache.n_misses);
    fclose(fp);
}

void load_fetchmisscache() {
    freopen("FetchData.mem","w",stdout);
    while (!FetchData.empty()) {
        cout<<FetchData.front().index<<" "<<FetchData.front().B_off<<"\n";
        FetchData.pop();    
    }
}

void load_fetchcache() {
    freopen("FetchTotalData.mem","w",stdout);
    while (!FetchTotalData.empty()) {
        cout<<(FetchTotalData.front()).tag<<" "<<(FetchTotalData.front()).index<<" "<<(FetchTotalData.front()).B_off<<"\n";
        FetchTotalData.pop();    
    }
}

// Cache initialization function
void init_cache_inst()
{
    ICache.size = CacheSz / 4;
    ICache.block_size = BlockSz / 4;
    ICache.num_block = ICache.size / ICache.block_size;
    ICache.assoc = format;
    ICache.n_sets = no_ways;
    ICache.policy = "";
    if (ICache.assoc == "DM")   {  ICache.index_bits = log2(ICache.num_block);   }
    else if (ICache.assoc == "SA")
    {
        ICache.index_bits = log2(ICache.num_block / ICache.n_sets);
        ICache.set_bits = log2(ICache.n_sets);
    }
    else    {    ICache.index_bits = 0;  }
    if ((format == "SA") || (format == "FA"))   {ICache.policy = Policy;}
    
    ICache.hit_time = 0;      // given
    ICache.miss_penalty = 0; // given
    ICache.n_accesses = 0;
    ICache.n_hits = 0;
    ICache.n_misses = 0;
    ICache.n_cold_misses = 0;
    ICache.n_conflict_misses = 0;
    ICache.n_capacity_misses = 0;
    ICache.offset_bits = log2(ICache.block_size);
    ICache.tag_bits = no_adress_bits - ICache.index_bits - ICache.offset_bits;
    for (int i = 0; i < ICache.num_block; i++)
    {
        for (int j = 0; j < pow(2, ICache.offset_bits); j++)
        {
            ICACHE[i][j].valid = 0; // means garbage value here
            ICACHE[i][j].tag = "";
            ICACHE[i][j].B_off = "";
            ICACHE[i][j].index = "";
            ICACHE[i][j].address = "";
        }
    }
}

void init_cache_data()
{
    DCache.size = CacheSz / 4;
    DCache.block_size = BlockSz / 4;
    DCache.num_block = DCache.size / DCache.block_size;
    DCache.assoc = format;
    DCache.n_sets = no_ways;
    DCache.policy = "";
    if (DCache.assoc == "DM")   {  DCache.index_bits = log2(DCache.num_block);   }
    else if (DCache.assoc == "SA")
    {
        DCache.index_bits = log2(DCache.num_block / DCache.n_sets);
        DCache.set_bits = log2(DCache.n_sets);
    }
    else    {    DCache.index_bits = 0;  }
    if ((format == "SA") || (format == "FA"))   {DCache.policy = Policy;}
    
    DCache.hit_time = 0;      // given
    DCache.miss_penalty = 0; // given
    DCache.n_accesses = 0;
    DCache.n_hits = 0;
    DCache.n_misses = 0;
    DCache.n_cold_misses = 0;
    DCache.n_conflict_misses = 0;
    DCache.n_capacity_misses = 0;
    DCache.offset_bits = log2(DCache.block_size);
    DCache.tag_bits = no_adress_bits - DCache.index_bits - DCache.offset_bits;
    for (int i = 0; i < DCache.num_block; i++)
    {
        for (int j = 0; j < pow(2, DCache.offset_bits); j++)
        {
            DCACHE[i][j].valid = 0; // means garbage value here
            DCACHE[i][j].tag = "";
            DCACHE[i][j].B_off = "";
            DCACHE[i][j].index = "";
            DCACHE[i][j].address = "";
        }
    }
}

void inst_cache_access()
{
    int ind = pc / 4;
    ICache.n_accesses++;
    bitset<32> address(ind);
    string add = address.to_string();
    string Tag, Block_Off, Index;
    ICache.miss_penalty = 2;
    Block_Off = add.substr(32 - (ICache.offset_bits), (ICache.offset_bits));
    Index = add.substr(32 - (ICache.offset_bits) - (ICache.index_bits), ICache.index_bits);
    Tag = add.substr(32 - no_adress_bits, ICache.tag_bits);
    bitset<32> tege(Tag);
    bitset<32> tege2(Block_Off);
    bitset<32> tege3(Index);
    int Tag_d = tege.to_ulong();
    int Block_d = tege2.to_ulong();
    int Index_d = tege3.to_ulong();
    ICache.assoc = format;
    if (ICache.assoc == "DM")
    {
        if(misss>0) misss--;
        else if (ICACHE[Index_d][Block_d].valid == 0)
        {
            ICACHE[Index_d][Block_d].address = add;
            ICACHE[Index_d][Block_d].tag = Tag;
            ICACHE[Index_d][Block_d].index = Index;
            ICACHE[Index_d][Block_d].B_off = Block_Off;
            ICACHE[Index_d][Block_d].valid = 1;
            FetchData.push(ICACHE[Index_d][Block_d]);
            FetchTotalData.push(ICACHE[Index_d][Block_d]);
            ICache.n_misses++;
            ICache.n_cold_misses++;
            hitflag = -1;
            misss = ICache.miss_penalty;
        }
        else if (ICACHE[Index_d][Block_d].valid == 1)
        {
            if (misss>0) {misss--;}
            else if (ICACHE[Index_d][Block_d].tag == Tag) {  ICache.n_hits++; hitflag=1; FetchTotalData.push(ICACHE[Index_d][Block_d]);}
            else if (ICACHE[Index_d][Block_d].tag != Tag)
            {
                ICACHE[Index_d][Block_d].address = add;
                ICACHE[Index_d][Block_d].tag = Tag;
                ICACHE[Index_d][Block_d].index = Index;
                ICACHE[Index_d][Block_d].B_off = Block_Off;
                FetchData.push(ICACHE[Index_d][Block_d]);
                FetchTotalData.push(ICACHE[Index_d][Block_d]);
                ICache.n_misses++;
                hitflag=-1;
                misss = ICache.miss_penalty;
                if (ICache.n_cold_misses == (ICache.size))  { ICache.n_capacity_misses++; }
                else{ICache.n_conflict_misses++;}
            }
        }
    }
    else if (ICache.assoc == "FA"){
        if (ICache.n_cold_misses < ICache.size ){  
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we get the requested block in ICache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if (misss>0) {misss--;}
            else if(!flag_flag){
                hitflag=-1;misss = ICache.miss_penalty;
                ICACHE[m][n].address = add;
                ICACHE[m][n].tag = Tag;
                ICACHE[m][n].B_off = Block_Off;
                ICACHE[m][n].valid = 1;
                FetchData.push(ICACHE[m][n]);
                FetchTotalData.push(ICACHE[m][n]);
                ICache.n_cold_misses++;
                ADDR tempadd;
                tempadd.addrs = add;
                tempadd.mad = m;
                tempadd.nad = n;
                fifo.push(tempadd);
                n++;
                if (n==ICache.block_size) {m++; n=0;}
                if (m==ICache.num_block) {m=0; n=0;}
            }
            else {
                FetchTotalData.push(ICACHE[m][n]);
                ICache.n_hits++;
            }
        }
        else {
             //all ICache blocks are filled we have to replace a block to store the new block from main memory only if that newly requested block is not in the cahe memory

            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we git the requested block in ICache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if(ICache.policy == "FIFO"){
                if (misss>0) {misss--;}
                else if(!flag_flag) {  // if the requested block is not in the ICache memory
                    ADDR tempadd;   
                    tempadd.addrs = add;
                    tempadd.mad = m;
                    tempadd.nad = n;
                    hitflag=-1; misss = ICache.miss_penalty;// -1 means there is a ICache miss
                    fifo.pop(); fifo.push(tempadd);
                    ICache.n_capacity_misses++;
                    ICACHE[m][n].address = add;
                    ICACHE[m][n].tag = Tag;
                    ICACHE[m][n].index = Index;
                    ICACHE[m][n].B_off = Block_Off;
                    FetchData.push(ICACHE[m][n]);
                    FetchTotalData.push(ICACHE[m][n]);
                    n++;
                    if (n==ICache.block_size) {m++; n=0;}
                    if (m==ICache.num_block) {m=0; n=0;}
                }
                else {
                    FetchTotalData.push(ICACHE[m][n]);
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "Random") {
                if (misss>0) {misss--;}
                else if (!flag_flag) {
                    cout<<"ABCD\n";
                    hitflag=-1; misss = ICache.miss_penalty;
                    m = getRandomNumber(0, ICache.num_block-1); // getting any random row number 
                    n = getRandomNumber(0, ICache.block_size-1); // getting any random column number
                    // just replacing the the block selected with the newly requested block
                    ICACHE[m][n].address = add;
                    ICACHE[m][n].tag = Tag;
                    ICACHE[m][n].index = Index;
                    ICACHE[m][n].B_off = Block_Off;
                    FetchTotalData.push(ICACHE[m][n]);
                    FetchData.push(ICACHE[m][n]);
                }
                else {
                    FetchTotalData.push(ICACHE[m][n]);
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "LRU") { // least recently used 
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = ICache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (fifo.front()).mad;
                    tempadd.nad = (fifo.front()).nad;
                    fifo.pop(); fifo.push(tempadd);
                    ICache.n_capacity_misses++;
                    ICACHE[tempadd.mad][tempadd.nad].address = add;
                    ICACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    ICACHE[tempadd.mad][tempadd.nad].index = Index;
                    ICACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    hitflag=1;
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "LFU") { // least frequently used
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag=-1;misss = ICache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (copy2.front()).mad;
                    tempadd.nad = (copy2.front()).nad;
                    copy.push(tempadd);
                    fifo = copy;
                    ICache.n_capacity_misses++; 
                    ICACHE[tempadd.mad][tempadd.nad].address = add;
                    ICACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    ICACHE[tempadd.mad][tempadd.nad].index = Index;
                    ICACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    fifo = copy2;
                    hitflag=1;
                    ICache.n_hits++;
                }
            }
            else {
                cout<<"There is no such policy! Please enter a correct policy.\n";
                exit(0);
            }
        }
    }
    else if (ICache.assoc == "SA") {
        Block_Off = add.substr(32 - (ICache.offset_bits)-(ICache.set_bits), (ICache.offset_bits)+(ICache.set_bits));
        bitset<32> tege4(Block_Off);
        Block_d = tege4.to_ullong();
        // where is the set bits calculated ? 
        if (ICache.n_cold_misses < ICache.size ){
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we git the requested block in ICache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if (misss>0) {misss--;}
            else if(!flag_flag) 
            {
                ICACHE[Index_d][Block_d].address = add;
                ICACHE[Index_d][Block_d].tag = Tag;
                ICACHE[Index_d][Block_d].B_off = Block_Off; // where are the set bits ... they needed to be stored as index tho
                ICACHE[Index_d][Block_d].valid = 1;
                FetchData.push(ICACHE[Index_d][Block_d]);
                FetchTotalData.push(ICACHE[Index_d][Block_d]);
                ICache.n_cold_misses++;
                ADDR tempadd;
                tempadd.addrs = add;
                tempadd.mad = Index_d;
                tempadd.nad = Block_d;
                fifo.push(tempadd);
                hitflag=-1;misss = ICache.miss_penalty;
            }
            else {  
                ICache.n_hits++;  
                FetchTotalData.push(ICACHE[Index_d][Block_d]);    
            }
        }
        else {
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if(ICache.policy == "FIFO"){
                ADDR tempadd;   
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = ICache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = m;
                    tempadd.nad = n;
                    fifo.pop(); fifo.push(tempadd);
                    ICache.n_capacity_misses++;
                    ICACHE[m][n].address = add;
                    ICACHE[m][n].tag = Tag;
                    ICACHE[m][n].index = Index;
                    ICACHE[m][n].B_off = Block_Off;
                    FetchData.push(ICACHE[m][n]);
                    FetchTotalData.push(ICACHE[m][n]);
                    n++;
                    if (n==ICache.block_size*ICache.n_sets) {m++; n=0;}
                    if (m==ICache.num_block) {m=0; n=0;}
                }
                else {
                    FetchTotalData.push(ICACHE[m][n]);
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "Random") {
                if (misss>0) {misss--;}
                else if (!flag_flag) {
                    hitflag=-1;misss = ICache.miss_penalty;
                    m = getRandomNumber(0, ICache.num_block-1);
                    n = getRandomNumber(0, ICache.block_size*ICache.n_sets-1);
                    ICACHE[m][n].address = add;
                    ICACHE[m][n].tag = Tag;
                    ICACHE[m][n].index = Index;
                    ICACHE[m][n].B_off = Block_Off;
                    FetchData.push(ICACHE[m][n]);
                    FetchTotalData.push(ICACHE[m][n]);
                }
                else {
                    FetchData.push(ICACHE[m][n]);
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "LRU") {
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = ICache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (fifo.front()).mad;
                    tempadd.nad = (fifo.front()).nad;
                    fifo.pop(); fifo.push(tempadd);
                    ICache.n_capacity_misses++;
                    ICACHE[tempadd.mad][tempadd.nad].address = add;
                    ICACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    ICACHE[tempadd.mad][tempadd.nad].index = Index;
                    ICACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(ICACHE[tempadd.mad][tempadd.nad]);   
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);   
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    hitflag=1;
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);   
                    ICache.n_hits++;
                }
            }
            else if (ICache.policy == "LFU") {
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag=-1;misss = ICache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (copy2.front()).mad;
                    tempadd.nad = (copy2.front()).nad;
                    copy.push(tempadd);
                    fifo = copy;
                    ICache.n_capacity_misses++; 
                    ICACHE[tempadd.mad][tempadd.nad].address = add;
                    ICACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    ICACHE[tempadd.mad][tempadd.nad].index = Index;
                    ICACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    FetchTotalData.push(ICACHE[tempadd.mad][tempadd.nad]);
                    ICache.n_hits++;
                    hitflag=1;
                }
            }
            else {
                cout<<"There is no such policy! Please enter a correct policy.\n";
                exit(0);
            }
        }
    }
    else {
        cout<<"There is no such associativity! Please enter a correct one.\n";
        exit(0);
    }
    
}

void data_cache_access(int ind)
{
    DCache.n_accesses++;
    bitset<32> address(ind);
    string add = address.to_string();
    string Tag, Block_Off, Index;
    DCache.miss_penalty = 5;
    Block_Off = add.substr(32 - (DCache.offset_bits), (DCache.offset_bits));
    Index = add.substr(32 - (DCache.offset_bits) - (DCache.index_bits), DCache.index_bits);
    Tag = add.substr(32 - no_adress_bits, DCache.tag_bits);
    bitset<32> tege(Tag);
    bitset<32> tege2(Block_Off);
    bitset<32> tege3(Index);
    int Tag_d = tege.to_ulong();
    int Block_d = tege2.to_ulong();
    int Index_d = tege3.to_ulong();
    DCache.assoc = format;
    if (DCache.assoc == "DM")
    {
        if (misss2>0) {misss2--;}
        else if (DCACHE[Index_d][Block_d].valid == 0)
        {
            DCACHE[Index_d][Block_d].address = add;
            DCACHE[Index_d][Block_d].tag = Tag;
            DCACHE[Index_d][Block_d].index = Index;
            DCACHE[Index_d][Block_d].B_off = Block_Off;
            DCACHE[Index_d][Block_d].valid = 1;
            FetchData.push(DCACHE[Index_d][Block_d]);
            // cout<<"check: "<<DCACHE[Index_d][Block_d].in<<"\n";
            FetchTotalData.push(DCACHE[Index_d][Block_d]);
            DCache.n_misses++;
            DCache.n_cold_misses++;
            hitflag2 = -1;
            misss2 = DCache.miss_penalty;
        }
        else if (DCACHE[Index_d][Block_d].valid == 1)
        {
            if (misss2>0) {misss2--;}
            else if (DCACHE[Index_d][Block_d].tag == Tag) {  DCache.n_hits++; hitflag2=1; FetchTotalData.push(DCACHE[Index_d][Block_d]);}
            else if (DCACHE[Index_d][Block_d].tag != Tag)
            {
                DCACHE[Index_d][Block_d].address = add;
                DCACHE[Index_d][Block_d].tag = Tag;
                DCACHE[Index_d][Block_d].index = Index;
                DCACHE[Index_d][Block_d].B_off = Block_Off;
                FetchData.push(DCACHE[Index_d][Block_d]);
                FetchTotalData.push(DCACHE[Index_d][Block_d]);
                DCache.n_misses++;
                hitflag2=-1;
                misss2 = DCache.miss_penalty;
                if (DCache.n_cold_misses == (DCache.size))  { DCache.n_capacity_misses++; }
                else{DCache.n_conflict_misses++;}
            }
        }
    }
    else if (DCache.assoc == "FA"){
        cout<<"Tag: "<<add.substr(32 - no_adress_bits, DCache.tag_bits)<<endl;
        if (DCache.n_cold_misses < DCache.size ){  
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we get the requested block in DCache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if (misss2>0) {misss2--;}
            else if(!flag_flag){
                hitflag2=-1;misss2 = DCache.miss_penalty;
                DCACHE[m][n].address = add;
                DCACHE[m][n].tag = Tag;
                DCACHE[m][n].B_off = Block_Off;
                DCACHE[m][n].valid = 1;
                FetchData.push(DCACHE[m][n]);
                FetchTotalData.push(DCACHE[m][n]);
                DCache.n_cold_misses++;
                ADDR tempadd;
                tempadd.addrs = add;
                tempadd.mad = m;
                tempadd.nad = n;
                fifo.push(tempadd);
                n++;
                if (n==DCache.block_size) {m++; n=0;}
                if (m==DCache.num_block) {m=0; n=0;}
            }
            else {
                FetchTotalData.push(DCACHE[m][n]);
                DCache.n_hits++;
            }
        }
        else {
             //all DCache blocks are filled we have to replace a block to store the new block from main memory only if that newly requested block is not in the cahe memory

            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we git the requested block in DCache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if(DCache.policy == "FIFO"){
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {  // if the requested block is not in the DCache memory
                    ADDR tempadd;   
                    tempadd.addrs = add;
                    tempadd.mad = m;
                    tempadd.nad = n;
                    hitflag2=-1; misss2 = DCache.miss_penalty;// -1 means there is a DCache miss
                    fifo.pop(); fifo.push(tempadd);
                    DCache.n_capacity_misses++;
                    DCACHE[m][n].address = add;
                    DCACHE[m][n].tag = Tag;
                    DCACHE[m][n].index = Index;
                    DCACHE[m][n].B_off = Block_Off;
                    FetchData.push(DCACHE[m][n]);
                    FetchTotalData.push(DCACHE[m][n]);
                    n++;
                    if (n==DCache.block_size) {m++; n=0;}
                    if (m==DCache.num_block) {m=0; n=0;}
                }
                else {
                    FetchTotalData.push(DCACHE[m][n]);
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "Random") {
                if (misss2>0) {misss2--;}
                else if (!flag_flag) {
                    cout<<"ABCD\n";
                    hitflag2=-1; misss2 = DCache.miss_penalty;
                    m = getRandomNumber(0, DCache.num_block-1); // getting any random row number 
                    n = getRandomNumber(0, DCache.block_size-1); // getting any random column number
                    // just replacing the the block selected with the newly requested block
                    DCACHE[m][n].address = add;
                    DCACHE[m][n].tag = Tag;
                    DCACHE[m][n].index = Index;
                    DCACHE[m][n].B_off = Block_Off;
                    FetchTotalData.push(DCACHE[m][n]);
                    FetchData.push(DCACHE[m][n]);
                }
                else {
                    FetchTotalData.push(DCACHE[m][n]);
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LRU") { // least recently used 
                ADDR tempadd;
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (fifo.front()).mad;
                    tempadd.nad = (fifo.front()).nad;
                    fifo.pop(); fifo.push(tempadd);
                    DCache.n_capacity_misses++;
                    DCACHE[tempadd.mad][tempadd.nad].address = add;
                    DCACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    DCACHE[tempadd.mad][tempadd.nad].index = Index;
                    DCACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    hitflag2=1;
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LFU") { // least frequently used
                ADDR tempadd;
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (copy2.front()).mad;
                    tempadd.nad = (copy2.front()).nad;
                    copy.push(tempadd);
                    fifo = copy;
                    DCache.n_capacity_misses++; 
                    DCACHE[tempadd.mad][tempadd.nad].address = add;
                    DCACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    DCACHE[tempadd.mad][tempadd.nad].index = Index;
                    DCACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    fifo = copy2;
                    hitflag2=1;
                    DCache.n_hits++;
                }
            }
            else {
                cout<<"There is no such policy! Please enter a correct policy.\n";
                exit(0);
            }
        }
    }
    else if (DCache.assoc == "SA") {
        Block_Off = add.substr(32 - (DCache.offset_bits)-(DCache.set_bits), (DCache.offset_bits)+(DCache.set_bits));
        bitset<32> tege4(Block_Off);
        Block_d = tege4.to_ullong();
        // where is the set bits calculated ? 
        if (DCache.n_cold_misses < DCache.size ){
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;  // we git the requested block in DCache memory
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if (misss2>0) {misss2--;}
            else if(!flag_flag) 
            {
                DCACHE[Index_d][Block_d].address = add;
                DCACHE[Index_d][Block_d].tag = Tag;
                DCACHE[Index_d][Block_d].B_off = Block_Off; // where are the set bits ... they needed to be stored as index tho
                DCACHE[Index_d][Block_d].valid = 1;
                FetchData.push(DCACHE[Index_d][Block_d]);
                FetchTotalData.push(DCACHE[Index_d][Block_d]);
                DCache.n_cold_misses++;
                ADDR tempadd;
                tempadd.addrs = add;
                tempadd.mad = Index_d;
                tempadd.nad = Block_d;
                fifo.push(tempadd);
                hitflag2=-1;misss2 = DCache.miss_penalty;
            }
            else {  
                DCache.n_hits++;  
                FetchTotalData.push(DCACHE[Index_d][Block_d]);    
            }
        }
        else {
            ADDR p;
            queue<ADDR> copy = fifo; // make a copy of the original queue
            queue<ADDR> copy2;
            bool flag_flag = false;
            while (!copy.empty()) {
                if ((copy.front()).addrs == add) {
                    flag_flag = true;
                    p = copy.front();
                }
                else {  copy2.push(copy.front());   }
                copy.pop();
            }
            if(DCache.policy == "FIFO"){
                ADDR tempadd;   
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = m;
                    tempadd.nad = n;
                    fifo.pop(); fifo.push(tempadd);
                    DCache.n_capacity_misses++;
                    DCACHE[m][n].address = add;
                    DCACHE[m][n].tag = Tag;
                    DCACHE[m][n].index = Index;
                    DCACHE[m][n].B_off = Block_Off;
                    FetchData.push(DCACHE[m][n]);
                    FetchTotalData.push(DCACHE[m][n]);
                    n++;
                    if (n==DCache.block_size*DCache.n_sets) {m++; n=0;}
                    if (m==DCache.num_block) {m=0; n=0;}
                }
                else {
                    FetchTotalData.push(DCACHE[m][n]);
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "Random") {
                if (misss2>0) {misss2--;}
                else if (!flag_flag) {
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    m = getRandomNumber(0, DCache.num_block-1);
                    n = getRandomNumber(0, DCache.block_size*DCache.n_sets-1);
                    DCACHE[m][n].address = add;
                    DCACHE[m][n].tag = Tag;
                    DCACHE[m][n].index = Index;
                    DCACHE[m][n].B_off = Block_Off;
                    FetchData.push(DCACHE[m][n]);
                    FetchTotalData.push(DCACHE[m][n]);
                }
                else {
                    FetchData.push(DCACHE[m][n]);
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LRU") {
                ADDR tempadd;
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (fifo.front()).mad;
                    tempadd.nad = (fifo.front()).nad;
                    fifo.pop(); fifo.push(tempadd);
                    DCache.n_capacity_misses++;
                    DCACHE[tempadd.mad][tempadd.nad].address = add;
                    DCACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    DCACHE[tempadd.mad][tempadd.nad].index = Index;
                    DCACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(DCACHE[tempadd.mad][tempadd.nad]);   
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);   
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    hitflag2=1;
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);   
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LFU") {
                ADDR tempadd;
                if (misss2>0) {misss2--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag2=-1;misss2 = DCache.miss_penalty;
                    tempadd.addrs = add;
                    tempadd.mad = (copy2.front()).mad;
                    tempadd.nad = (copy2.front()).nad;
                    copy.push(tempadd);
                    fifo = copy;
                    DCache.n_capacity_misses++; 
                    DCACHE[tempadd.mad][tempadd.nad].address = add;
                    DCACHE[tempadd.mad][tempadd.nad].tag = Tag;
                    DCACHE[tempadd.mad][tempadd.nad].index = Index;
                    DCACHE[tempadd.mad][tempadd.nad].B_off = Block_Off;
                    FetchData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                }
                else {
                    copy2.push(p);
                    fifo = copy2;
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);
                    DCache.n_hits++;
                    hitflag2=1;
                }
            }
            else {
                cout<<"There is no such policy! Please enter a correct policy.\n";
                exit(0);
            }
        }
    }
    else {
        cout<<"There is no such associativity! Please enter a correct one.\n";
        exit(0);
    }
}


void fetch()
{
    cycle_no++;
    cout << "fetching : " << endl;
    if (Fetchflag != 2)
    {
        pc += 4;
        Tall = 0;
        // if(misss>0)
        // {
        //     misss--;
        //     if(misss2>0) misss2--;
        //     pc-=4;
        //     return;
        // }
        // else if(misss2>0)
        // {
        //     misss2--;
        //     if(misss>0) misss--;
        //     pc-=4;
        //     return;
        // }
        inst_cache_access();
        if (misss>0) {pc-=4; return;}
        IF_DE.curr_inst = MEM[pc];
        string insth;
        bitset<32> pac(pc);
        cout << "Fetch instruction : 0x" << hex << (IF_DE.curr_inst).to_ulong() << " from address 0x" << hex << pac.to_ulong() << endl;
        bitset<32> exitcode("11101111000000000000000000010001");
        // end when come across 0xEF000011 as the instruction
        // cout<<endflag<<endl
        if (endflag == 1 && EX_MA.check == 'T') {   endflag = 0; }
        else if (endflag == 1 && EX_MA.check == 'F') {    swi_exit();  }
        if (IF_DE.curr_inst == exitcode && (DE_EX.type == 'B' || DE_EX.type == 'J' || DE_EX.Subtype == "jalr"))
        {
            endflag = 1;
            Fetchflag = 2;
        }
        else if (IF_DE.curr_inst == exitcode)   { swi_exit(); }
    }
    else if (Fetchflag == 2 && EX_MA.check == 'T')
    {
        Fetchflag = 0;
        cout << "Branch Stall\n";
    }
    else if (Fetchflag == 2 && EX_MA.check == 'F')
    {
        Fetchflag = 0;
        cout << "Branch Stall\n";
        pc -= 4;
    }
    else    {    cout << "STALLED  : " << stall << endl;    }
}

void decode()
{
    cout << "Decode Instruction:\n";
    if (misss>0) { return;}
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
    cout << "Format of instruction: " << Type <<"\n";
    int j = 0;
    for (int i = 25; i < 32; i++){ func7[j] = inst[i]; j++; }   j = 0;
    for (int i = 12; i < 15; i++){ func3[j] = inst[i]; j++;}    j = 0;       
    for (int i = 15; i < 20; i++){rs1[j] = inst[i];j++;}
    operand1 = rs1.to_ulong(); // converting the binary string to unsigned decimal to get the register 1
    j = 0;
    for (int i = 20; i < 25; i++){ rs2[j] = inst[i]; j++;}
    operand2 = rs2.to_ulong(); // converting the binary string to unsigned decimal to get the register 2
    switch (Type)
    {
        case 'R':{
            j = 0;
            for (int i = 7; i < 12; i++) {   rd[j] = inst[i];    j++;  } // Determining destination register (rd) in binary form
            des_reg = rd.to_ulong(); // converting the binary string to unsigned decimal to get the destination register
            cout << "Operand1: 0x" << operand1 << ", "<< "Operand2: 0x" << operand2 << ", "<< "RD: 0x" << des_reg << endl;
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && cycle_no > 2 && DE_EX.rd2 != 0)  {stall = 4;}
            break;
        }
        case 'I':{
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
            cout << "Immediate Value: " << imm << ", "<< "Operand1: 0x" << operand1 << ", "<< "RD: 0x" << des_reg << endl;
            if (operand1 == DE_EX.rd2 && cycle_no > 2 && DE_EX.rd2 != 0) {stall = 4;}
            break;
        }
        case 'S':{
            bitset<12> immb;
            j = 0;
            for (int i = 7; i < 12; i++){immb[j] = inst[i];j++;}
            for (int i = 25; i < 32; i++){ immb[j] = inst[i]; j++; }
            // Sign extension of the immediate
            imm = immb.to_ulong();
            if (immb[11] == 1)  {  imm = -1 * imm; }
            cout << "Immediate Value: " << imm << ", "<< "Operand1: 0x" << operand1 << ", "<< "Operand2: 0x" << operand2 << endl; 
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && DE_EX.rd2 != 0)      { stall = 4; }
            break;
        }
        case 'B':
        {
            int addr = IF_DE.progcont;
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
            cout << "Immediate Value: " << imm << ", " << "Operand1: 0x" << operand1 << ", "<< "Operand2: 0x" << operand2 << endl;
            if ((operand1 == DE_EX.rd2 || operand2 == DE_EX.rd2) && DE_EX.rd2 != 0)   { stall = 4;  }
            if (BrPr[addr]) {pc += (imm - 4);cout<<"Branch predicted\n"; endflag = 0;}
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
            // the immediate value in the LUI (Load Upper Immediate) and AUIPC (Add Upper Immediate to PC) instructions in RISC-V cannot be negative. So Sign extension required.
            cout << "Immediate Value: " << imm << ", "<< "RD: 0x" << des_reg << endl;
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
            cout << "Immediate Value: " << imm << ", "<< "RD: 0x" << des_reg << endl;
            break;
        }
        default:{cout << "Error! No instruction found!\n";}
    }
    DE_EX.rs1 = operand1; DE_EX.rs2 = operand2;
    DE_EX.immed = imm; DE_EX.type = Type;
    DE_EX.rd2 = des_reg; 
    subtype_select(func3, func7, op);
    DE_EX.Subtype = subtype;
    DE_EX.progcont = IF_DE.progcont;
    cout << "The operation is " << subtype << ".\n";
}
// executes the ALU operation based on ALUop
void execute()
{
    char ch = 'O';
    cout << "Execute Instruction:\n";
    if (misss>0) { return;}
    // cout<<"pc during execute: "<<pc<<"\n";
    operand1 = DE_EX.rs1;operand2 = DE_EX.rs2;Type = DE_EX.type;des_reg = DE_EX.rd2;imm = DE_EX.immed; 
    subtype = DE_EX.Subtype;
    if (Type == 'R'){ // add, and, or, sll, slt, sra, srl, sub, xor
        if(stall == 4){
            if(EX_MA.rd2==operand1) {X[operand1]=EX_MA.rd_value;cout<<"hgfashklfa "<<X[operand1]<<endl; }
            else if(EX_MA.rd2==operand2){ X[operand2]=EX_MA.rd_value;}stall = 0;
        }
        if (subtype == "add"){
            des_res = X[operand1] + X[operand2];
            cout << "Adding " << X[operand1] << " and " << X[operand2] << endl;
        }
        else if (subtype == "sub"){
            des_res = X[operand1] - X[operand2];
            cout << "Subtracting " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "and"){
            des_res = X[operand1] & X[operand2];
            cout << "Bitwise AND " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "or"){
            des_res = X[operand1] | X[operand2];
            cout << "Bitwise OR " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "sll"){
            des_res = X[operand1] << X[operand2];
            cout << "Shift Left " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "slt"){
            des_res = (X[operand1] < X[operand2]) ? 1 : 0;
            cout << "Set Less Than in " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "sra"){
            des_res = X[operand1] >> X[operand2];
            cout << "Shift Right Arithmatic" << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "xor"){
            des_res = X[operand1] ^ X[operand2];
            cout << "Bitwise XOR " << X[operand1]  << " and " << X[operand2] << endl;
        }
        else if (subtype == "srl"){
            des_res = X[operand1] >> X[operand2];
            cout << "Shift Right Logical " << X[operand1]  << " and " << X[operand2] << endl;
        }
        ALU_inst++;
    }
    else if (Type == 'I'){ // addi, andi, ori, lb, lh, lw, jalr
        // if(stall==4){ X[operand1]=EX_MA.rd_value; cout<<"safskdhfkds\n";stall = 0;}
        if (subtype == "addi"){
            des_res = X[operand1] + imm;
            cout << "Adding " << X[operand1] << " and " << imm << endl;
            ALU_inst++;
        }
        else if (subtype == "andi"){
            des_res = X[operand1] & imm;
            cout << "Bitwise AND " << X[operand1] << " and " << imm << endl;
            ALU_inst++;
        }
        else if (subtype == "ori"){
            des_res = X[operand1] | imm;
            cout << "Bitwise OR " << X[operand1] << " and " << imm << endl;
            ALU_inst++;
        }
        else if (subtype == "lb" || subtype == "lh" || subtype == "lw"){
            des_res = X[operand1] + imm;
            cout << "Calculating net memory address by Adding " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "jalr"){
            des_res = pc;
            pc-=4;
            Fetchflag=2;
            DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;
            Control_inst++;
            ch = 'T';
            pc = X[operand1] + imm - 4;
            cout << "Calculating net memory address by adding " << X[operand1] << " and " << imm << endl;
        }
        else if (subtype == "slli"){
            des_res = X[operand1] << imm;
            cout << "Shift Left " << X[operand1] << "and" << operand2 << endl;
            ALU_inst++;
        }
    }
    else if (Type == 'B'){ // beq, bne, bge, blt
        if(stall == 4){
            if(EX_MA.rd2==operand1 ) {X[operand1]=EX_MA.rd_value;}
            else if(EX_MA.rd2==operand2){X[operand2]=EX_MA.rd_value;}stall = 0;
        }
        if (subtype == "beq"){
            if (X[operand1] == X[operand2]){
                if (BrPr[DE_EX.progcont]==0) {  pc -= 4;
                    BrPr[DE_EX.progcont]=1; Fetchflag=2;    ch='T';
                    cout << "if operand1 and operand2 are equal then Adding " << pc << " and " << imm << endl;  pc += imm-4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }    
                else {  ch='T';
                    cout << "if operand1 and operand2 are equal then Adding " << pc-imm << " and " << imm << "Already Predicted\n";
                }           
            }
            else{   ch='F';
                if (BrPr[DE_EX.progcont]==0) {
                    cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << 4 << endl;
                    des_res = 0; des_reg = 0;
                }
                else {
                    Fetchflag=2;    BrPr[DE_EX.progcont]=0; pc-=imm; 
                    cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << 4 << "Predicted Wrong!\n"; Branch_Mis++; pc+=4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }
            }
        }
        else if (subtype == "bne"){
            if (X[operand1] != X[operand2]){
                if (BrPr[DE_EX.progcont]==0) { pc -= 4;
                    BrPr[DE_EX.progcont]=1; Fetchflag=2;    ch='T';
                    cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << imm << endl;  pc += imm-4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }    
                else {  ch='T';
                    cout << "if operand1 and operand2 are not equal then Adding " << pc << " and " << imm <<"Already Predicted\n";
                }     
            }
            else{
                ch='F';
                if (BrPr[DE_EX.progcont]==0) {
                    cout << "if operand1 and operand2 are equal then Adding " << pc << " and " << 4 << endl;
                    des_res = 0; des_reg = 0;
                }
                else {
                    Fetchflag=2;    BrPr[DE_EX.progcont]=0; pc-=imm; 
                    cout << "if operand1 and operand2 are equal then Adding " << pc << " and " << 4 << "Predicted Wrong!\n"; Branch_Mis++; pc+=4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }
            }
        }
        else if (subtype == "bge"){
            if (X[operand1] >= X[operand2]){
                if (BrPr[DE_EX.progcont]==0) { pc -= 4;
                    BrPr[DE_EX.progcont]=1; Fetchflag=2;    ch='T'; pc += imm-4;
                    cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " << imm << endl;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }    
                else {  ch='T';
                    cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " <<imm <<"Already Predicted\n";
                } 
            }
            else{
                ch='F';
                if (BrPr[DE_EX.progcont]==0) {
                    cout << "if operand1 less than operand2 then Adding " << pc << " and " << 4 << endl;
                    des_res = 0; des_reg = 0;
                }
                else {Fetchflag=2;  BrPr[DE_EX.progcont]=0; pc-=imm; 
                    cout << "if operand1 less than operand2 then Adding " << pc << " and " << 4 <<  "Predicted Wrong!\n"; Branch_Mis++; pc+=4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }
            }
        }
        else if (subtype == "blt"){
            if (X[operand1] < X[operand2]){
                if (BrPr[DE_EX.progcont]==0) { pc -= 4;
                    BrPr[DE_EX.progcont]=1; Fetchflag=2;    ch='T';
                    cout << "if operand1 less than operand2 then Adding " << pc << " and " << imm << endl;  pc += imm-4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }    
                else {  ch='T';
                    cout << "if operand1 less than operand2 then Adding " << pc << " and " << imm <<"Already Predicted\n";
                } 
            }
            else{   ch='F';
                if (BrPr[DE_EX.progcont]==0) {
                    cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " << 4 << endl;
                    des_res = 0; des_reg = 0;
                }
                else {
                    Fetchflag=2;    BrPr[DE_EX.progcont]=0; pc-=imm; 
                    cout << "if operand1 greater than and equal to operand2 then Adding " << pc << " and " << 4 <<  "Predicted Wrong!\n"; Branch_Mis++; pc+=4;
                    DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;des_res = 0;des_reg = 0;
                }
            }
        }
    }
    else if (Type == 'J'){ // jal
        des_res = pc;   pc-=4;
        cout << "Calculating net memory address by adding " << operand1 << " and " << imm << endl;
        pc += imm - 4; Fetchflag=2; ch = 'T';
        DE_EX.rs1 = 0; DE_EX.rs2 = 0; DE_EX.rd2 = 0; DE_EX.type = '0'; DE_EX.Subtype = "", DE_EX.immed = 0, IF_DE.curr_inst=0;
        Control_inst++;
    }
    else if (Type == 'S'){ // sb, sw, sh
        des_res = X[operand1] + imm;
        cout << "Calculating net memory address by Adding " << operand1 << " and " << imm << endl;
    }
    else if (Type == 'U'){ // auipc, lui
        if (subtype == "auipc"){
            des_res = (pc - 4) + (imm << 12);
            cout << "Adding pc and Shifted Immediate " << pc << " and " << imm << endl;
        }
        else if (subtype == "lui"){des_res = imm << 12;}
        ALU_inst++;
    }
    EX_MA.rs1 = operand1;EX_MA.rs2 = operand2;EX_MA.rd_value = des_res;EX_MA.rd2 = des_reg;EX_MA.type = Type;
    EX_MA.Subtype = subtype; EX_MA.check = ch;
    Type = '0';
}
// perform the memory operation
void mem()
{
    cout << "Memory Instruction:\n";
    if (misss>0) { return;}
    operand1 = EX_MA.rs1; operand2 = EX_MA.rs2; des_reg = EX_MA.rd2; des_res = EX_MA.rd_value; Type = EX_MA.type; subtype = EX_MA.Subtype;
    
    // Memory operation is required only in Load and store instructions
    if (subtype == "lw" || subtype == "lh" || subtype == "lb")
    {
        data_cache_access(des_res);
        cout << "Loading from data memory: " << des_res << endl;
        des_res = DMEM[des_res]; // Loading value from memory to register
        MA_WB.rd_value = des_res; MA_WB.rd2 = des_reg;MA_WB.type = Type; MA_WB.Subtype = subtype;
        Ld_St++;
    }
    else if (subtype == "sw" || subtype == "sh" || subtype == "sb"){
        data_cache_access(des_res);
        DMEM[des_res] = X[operand2]; // Loading value from register to memory
        MA_WB.rd_value = DMEM[des_res]; MA_WB.rd2 = 0;MA_WB.type = Type; MA_WB.Subtype = subtype;
        cout << "Storing " << X[operand2] << " into data memory: " << des_res <<" "<< endl;
        Ld_St++;
    }
    else{
        cout << "No Memory operation.\n"; 
        MA_WB.rd_value = des_res; MA_WB.rd2 = des_reg; MA_WB.type = Type; MA_WB.Subtype = subtype;
    }
    MA_WB.rs1 = operand1;
    MA_WB.rs2 = operand2;
    if(stall == 4){
        EX_MA.rs1 = operand1;EX_MA.rs2 = operand2;EX_MA.rd_value = des_res;EX_MA.rd2 = des_reg;EX_MA.type = Type;
        EX_MA.Subtype = subtype;
    }
    cout<<"x14="<<X[14]<<endl;
}
// writes the results back to register file
void write_back()
{
    
    cout << "WriteBack Instruction:\n";
    if (misss>0) {return;}
    operand1 = MA_WB.rs1; operand2 = MA_WB.rs2; des_reg = MA_WB.rd2; des_res = MA_WB.rd_value; Type = MA_WB.type;
    if (Type != 'S' && Type != 'B'){
        X[des_reg] = des_res;
        cout << "Storing " << des_res << " into 0x" << des_reg << endl;
    }
    else cout << "No WriteBack Operation.\n";
    Type = '0'; X[0] = 0;// Ensuring that X0 will always be 0 and wont get overwritten.
}


// Define main program
int main()
{
    // reset the processor
    reset_proc();
    // load the program memory
    load_program_memory();
    // we have to specify the input format.. power of 2 or GB/MB
    cout << "Enter Cache size in Bytes: " ;
    cin >> CacheSz;
    cout << "Enter Block Size in Bytes: " ;
    cin >> BlockSz;
    cout << "Direct mapped (DM)/Full Assoc (FA)/ Set Assoc(SA) : " ;
    cin >> format;
    no_ways = 1; // in case of direct mapping
    if (format == "SA")
    {
        cout << "Number of ways for SA " << endl;
        cin >> no_ways;
    }
    if ((format == "SA") || (format == "FA"))
    {
        cout << "LRU/FIFO/Random/LFU replacement policies" << endl;
        cin >> Policy;
    }
    no_adress_bits = ceil(log2(sz));
    #ifndef ONLINE_JUDGE
        freopen("output_log.mem", "w", stdout);
    #endif
    Cache_pro *ICache, *DCache;
    init_cache_inst();
    // init_cache(DCache);
    while (1)
    {
        if (inst_end > 0){
            cout<<endl; cout << "CYCLE NUMBER: " <<dec << cycle_no << endl;

            // if(misss>0)
            // {
            //     cout<<"Cache Stalled for "<<misss<<" cycles\n";
            //     hitflag =0;
            // }
            // else if(misss2>0)
            // {
            //     cout<<"Cache Stalled for "<<misss2<<" cycles\n";
            //     hitflag2=0;
            // }
            // else
            // {
            //     if (cycle_no > 4) {write_back(); T_inst++;}
            //     if (cycle_no > 3){mem(); T_inst++;}
            //     if (cycle_no > 2){execute(); T_inst++;}
            //     if (cycle_no > 1){decode(); T_inst++;}
            //     T_inst++;
            // }
            
            if (misss <= 0){
                if (cycle_no > 4) {write_back(); T_inst++;}
                if (cycle_no > 3){mem(); T_inst++;}
                if (cycle_no > 2){execute(); T_inst++;}
                if (cycle_no > 1){decode(); T_inst++;}
                T_inst++;
            }
            else {
                //cache_stalls
                cout<<"Cache Stalled for "<<misss<<" cycles\n";
                hitflag =0;
                // hitflag2=0;
                }
            fetch();
        }
        else{
            if (cycle_no<=4) {cycle_no++; rflag++;}
            int finish = cycle_no + 3 ; if (rflag==1) {cycle_no--; rflag--;}
            while (finish != cycle_no)
            { cout<<endl; cout << "CYCLE NUMBER: " <<dec << cycle_no << endl;
                if (cycle_no < finish && cycle_no>4) {write_back(); T_inst++;}
                if (cycle_no < finish - 1) {mem(); T_inst++;}
                if (cycle_no < finish - 2) {execute(); T_inst++;}
                if (cycle_no < finish - 3) {decode(); T_inst++;}
                cycle_no++;
            }
            cycle_no--;
            break;
        }
    }
    cout<<(FetchData.front()).index<<"\n";
    CPI = float(T_inst)/float(cycle_no);
    load_randata(); load_register(); load_Memory();
    load_dhazards(); load_chazards();
    load_icache(); load_icachedata(); load_fetchmisscache(); load_fetchcache(); 
    load_dcache();
    // for (int i = 0; i < sz; i++)
    // {
    //     fetch();
    //     if (misss>0) {
    //         //cache_stalls
    //         cout<<"Cache Stalled for "<<misss<<" cycles\n";
    //         hitflag =0;
    //         i--;
    //     }
    // }
    return 0;
}              