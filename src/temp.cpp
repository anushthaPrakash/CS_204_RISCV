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

void data_cache_access()
{
    int ind = pc / 4;
    DCache.n_accesses++;
    bitset<32> address(ind);
    string add = address.to_string();
    string Tag, Block_Off, Index;
    DCache.miss_penalty = 2;
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
        if (misss>0) {misss--;}
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
            hitflag = -1;
            misss = DCache.miss_penalty;
        }
        else if (DCACHE[Index_d][Block_d].valid == 1)
        {
            if (misss>0) {misss--;}
            else if (DCACHE[Index_d][Block_d].tag == Tag) {  DCache.n_hits++; hitflag=1; FetchTotalData.push(DCACHE[Index_d][Block_d]);}
            else if (DCACHE[Index_d][Block_d].tag != Tag)
            {
                DCACHE[Index_d][Block_d].address = add;
                DCACHE[Index_d][Block_d].tag = Tag;
                DCACHE[Index_d][Block_d].index = Index;
                DCACHE[Index_d][Block_d].B_off = Block_Off;
                FetchData.push(DCACHE[Index_d][Block_d]);
                FetchTotalData.push(DCACHE[Index_d][Block_d]);
                DCache.n_misses++;
                hitflag=-1;
                misss = DCache.miss_penalty;
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
            if (misss>0) {misss--;}
            else if(!flag_flag){
                hitflag=-1;misss = DCache.miss_penalty;
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
                if (misss>0) {misss--;}
                else if(!flag_flag) {  // if the requested block is not in the DCache memory
                    ADDR tempadd;   
                    tempadd.addrs = add;
                    tempadd.mad = m;
                    tempadd.nad = n;
                    hitflag=-1; misss = DCache.miss_penalty;// -1 means there is a DCache miss
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
                if (misss>0) {misss--;}
                else if (!flag_flag) {
                    cout<<"ABCD\n";
                    hitflag=-1; misss = DCache.miss_penalty;
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
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = DCache.miss_penalty;
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
                    hitflag=1;
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LFU") { // least frequently used
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag=-1;misss = DCache.miss_penalty;
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
                    hitflag=1;
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
            if (misss>0) {misss--;}
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
                hitflag=-1;misss = DCache.miss_penalty;
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
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = DCache.miss_penalty;
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
                if (misss>0) {misss--;}
                else if (!flag_flag) {
                    hitflag=-1;misss = DCache.miss_penalty;
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
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    hitflag=-1;misss = DCache.miss_penalty;
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
                    hitflag=1;
                    FetchTotalData.push(DCACHE[tempadd.mad][tempadd.nad]);   
                    DCache.n_hits++;
                }
            }
            else if (DCache.policy == "LFU") {
                ADDR tempadd;
                if (misss>0) {misss--;}
                else if(!flag_flag) {
                    for (int i=0;i<copy2.size()-1;i++) {
                        p = copy2.front();
                        copy2.pop();
                        copy.push(p);
                    }
                    hitflag=-1;misss = DCache.miss_penalty;
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
