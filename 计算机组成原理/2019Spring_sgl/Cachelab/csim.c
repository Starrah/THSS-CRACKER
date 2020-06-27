//沈冠霖,2017013569
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>
#include "cachelab.h"
//data:2^b bytes
//64 bit address,t,s,b,t = 64 - s - b

int s = 0, E = 0, b = 0, t = 0;
int miss = 0,hit = 0, evict = 0,current_operation = 0;
//Block's functions
typedef struct
{
    bool m_Valid;
    int m_Tag;
    //bool* m_Data;
    int m_Usage;

}Block;

typedef struct
{
    Block* m_Block;
}Set;

typedef struct
{
    Set* m_Set;
}Cache;
Cache* the_cache = NULL;

void Initiate_Block(Block* the);
bool Compare_Tag(int inquery,Block* the);
void Write_Block(bool* pointer,Block* the);
void Initiate_Set(Set* the);
void Destroy_Set(Set* the);
void Initiate_Cache(Cache* the);
void Destroy_Cache(Cache* the);
Set* Find_Set(Cache* the,bool* dest);
Block* Find_Block(Set* the,bool* dest, int* operation);
void OperateCache(Block* the,bool* dest,int operation);
void OperateInstruction(int type,bool* address);


void Initiate_Block(Block* the)
{
        the->m_Valid = 0;
        the->m_Tag = 0;
        the->m_Usage = 0;
}



bool Compare_Tag(int inquery,Block* the)
{
    if(inquery != the->m_Tag)
    {
            return 0;
    }
    return 1;
}




void Initiate_Set(Set* the)
{
    the->m_Block = (Block*)malloc(sizeof(Block)*E);
    for(int i = 0; i < E; i ++)
    {
        Initiate_Block(&(the->m_Block[i]));
    }
}

void Destroy_Set(Set* the)
{
    for(int i = 0; i < E; i ++)
    {
        free(&(the->m_Block[i]));
    }
}



void Initiate_Cache(Cache* the)
{
    unsigned int S = 1<<s;
    the->m_Set = (Set*)malloc(sizeof(Set)*S);
    for(int i = 0; i < S; i ++)
    {
        Initiate_Set(&(the->m_Set[i]));
    }
}

void Destroy_Cache(Cache* the)
{
    int S = 1<<s;
    for(int i = 0; i < S; i ++)
    {
        Destroy_Set(&(the->m_Set[i]));
        free(&(the->m_Set[i]));
    }
}


Set* Find_Set(Cache* the,bool* dest)
{
    //long long int mask_tt = ((1 << t) - 1)<<(s+b);
    long long int mask_s = ((1<<s)-1)<<b;
    //long long int mask_b = (1<<b)-1;
    //long long int t_num = ((long long int)dest & mask_tt)>>(s+b);
    long long int s_num = ((long long int)dest & mask_s)>>b;
    //long long int b_num = (long long int)dest & mask_b);
    return &(the->m_Set[s_num]);
}

//operation = 0: hit 1:miss, not evict, 2:miss, evict
Block* Find_Block(Set* the,bool* dest, int* operation)
{
    long long int mask_tt = ((1 << t) - 1)<<(s+b);
    //long long int mask_s = ((1<<s)-1)<<b;
    //long long int mask_b = (1<<b)-1;
    long long int t_num = ((long long int)dest & mask_tt)>>(s+b);
    //long long int s_num = ((long long int)dest & mask_s)>>s;
    //long long int b_num = (long long int)dest & mask_b);
    bool whether_success = 0; //hit
    int success_place = -1;
    bool whether_has_empty = 0; //has empty
    int first_empty_place = -1;
    int least_recent_num = 1919810;
    int least_recent_place = -1;
    for(int i = 0; i < E; i ++)
    {
        Block* the_block = &(the->m_Block[i]);
        if(the_block->m_Valid == 1 && Compare_Tag(t_num,the_block)==1)
        {
            whether_success = 1;
            success_place = i;
            break;
        }
        else if(the_block->m_Valid == 0)
        {
            if(whether_has_empty == 0)
            {
                whether_has_empty = 1;
                first_empty_place = i;
            }
        }
        else if(the_block->m_Usage < least_recent_num)
        {
            least_recent_num = the_block->m_Usage;
            least_recent_place = i;
        }
    }
    //hit
    if(whether_success == 1)
    {
        *operation = 0;
        return &(the->m_Block[success_place]);
    }
    //miss, no evict
    else if(whether_has_empty == 1)
    {
        *operation = 1;
        return &(the->m_Block[first_empty_place]);
    }
    //miss evict
    else
    {
        *operation = 2;
        return &(the->m_Block[least_recent_place]);
    }
}

void OperateCache(Block* the,bool* dest,int operation)
{
    long long int mask_tt = ((1 << t) - 1)<<(s+b);
    //long long int mask_s = ((1<<s)-1)<<b;
    //long long int mask_b = (1<<b)-1;
    long long int t_num = ((long long int)dest & mask_tt)>>(s+b);
    //long long int s_num = ((long long int)dest & mask_s)>>s;
    //long long int b_num = (long long int)dest & mask_b;
    if(operation == 0)
    {
        the->m_Usage = current_operation;
        return;
    }
    else
    {
        the->m_Valid = 1;
        the->m_Usage = current_operation;

        the->m_Tag = t_num;
       /* for(int i = 0; i < (1<<b); i ++)
        {
            the->m_Data[i]=new_start[i];
        }*/
        
    }
}

void OperateInstruction(int type,bool* address)
{

        current_operation ++;

        Set* current_set;
        Block* current_block;
        int current_status = -1;
        if(type == 0)
        {
            
        }
        else
        {
            current_set = Find_Set(the_cache,address);
            current_block = Find_Block(current_set,address,&current_status);
            OperateCache(current_block,address,current_status);
            if(current_status == 0) //hit
            {
                hit ++;
            }
            else if(current_status == 1)//miss
            {
                miss ++;
            }
            else//miss evict
            {
                miss ++;
                evict ++;
            }
            if(type == 1)//M
            {
                hit ++;
            }
        }
}

int main(int argc, char **argv)
{

    //used in reading parameters
    char* filename;
    char c;
    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1)
	{
        switch(c)
		{
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            filename = optarg;
            break;
        case 'h':
            exit(0);
        default:
            exit(1);
        }
    }
    //s = 2; E = 1; b = 4;  filename = "traces/dave.trace";
    t = 64 - s - b;
    the_cache = (Cache*)malloc(sizeof(Cache));
    Initiate_Cache(the_cache);

    FILE* the_file = NULL; 
    the_file  = fopen(filename, "r");
    if (the_file != NULL) 
    {
        long long int address = 0;
        int length = 0;
        while (1)
        {
            char c1 = 0,c2 = 0,c3 = 0;                
            fscanf(the_file, "%c", &c1);
            fscanf(the_file, "%c", &c2);
            fscanf(the_file, "%c", &c3);

            if(c1 <= 0 && c2 <= 0 && c3 <= 0) break;
            fscanf(the_file, "%llx,%d", &address, &length);
            c3 = 0;
            while(1)
            {
                fscanf(the_file, "%c", &c3);
                if(c3 == '\n') break;
            }
            int type = 0;
            switch(c2)
		    {
            case 'I':
                type = 0;
                break;
            case 'M':
                type = 1;
                break;
            case 'L':
                type = 2;
                break;
            case 'S':
                type = 3;
                break;
            }
            OperateInstruction(type,(bool*)address);
        }
    }
    printSummary(hit, miss, evict);
    //Destroy_Cache(the_cache);
    return 0;
}
