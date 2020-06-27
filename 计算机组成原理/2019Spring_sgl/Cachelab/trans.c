/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

#define BLOCK_SIZE 8    
#define SUB_BLOCK_SIZE 4

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
   //BLOCK_SIZE是8,代表每个块的int数目:8,也代表我处理的基本单元:8*8矩阵
   //用于循环的变量x,y,i
   int i, x, y;
   //用于读取数据的8个变量
   int a0,a1,a2,a3,a4,a5,a6,a7;

    //情况1:32*32或者48*48
    if(M == 32 || M == 48)
    {
        for(x = 0; x < (N/BLOCK_SIZE); x ++)
        {
            for(y = 0; y < (M/BLOCK_SIZE); y ++)
            {    
		
		//遍历每个8*8的子块
                for(i = 0; i < BLOCK_SIZE; i ++)
                {
		    //读取A中对应行的数据
                    a0 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 0];
                    a1 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 1];
                    a2 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 2];
                    a3 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 3];
                    a4 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 4];
                    a5 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 5];
                    a6 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 6];
                    a7 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 7];

		    //写入到B中对应列
                    B[BLOCK_SIZE*y + 0][BLOCK_SIZE*x + i]= a0;
                    B[BLOCK_SIZE*y + 1][BLOCK_SIZE*x + i]= a1;
                    B[BLOCK_SIZE*y + 2][BLOCK_SIZE*x + i]= a2;
                    B[BLOCK_SIZE*y + 3][BLOCK_SIZE*x + i]= a3;
                    B[BLOCK_SIZE*y + 4][BLOCK_SIZE*x + i]= a4;
                    B[BLOCK_SIZE*y + 5][BLOCK_SIZE*x + i]= a5;
                    B[BLOCK_SIZE*y + 6][BLOCK_SIZE*x + i]= a6;
                    B[BLOCK_SIZE*y + 7][BLOCK_SIZE*x + i]= a7;
                }  
            }
        }
    }



    //情况2:64*64
    else if(M == 64)
    {
        //遍历每个8*8的子块
        for(x = 0; x < 8; x ++)
        {
            for(y = 0; y < 8; y ++)
            {    
                
		//处理A矩阵的前四行
                for(i = 0; i < SUB_BLOCK_SIZE; i ++)
                {
		    //读入一行8个数据
                    a0 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 0];
                    a1 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 1];
                    a2 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 2];
                    a3 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 3];
                    a4 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 4];
                    a5 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 5];
                    a6 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 6];
                    a7 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 7];
                    

		    //前四个数据写到B11的正确位置(左上角)
                    B[BLOCK_SIZE*y + 0][BLOCK_SIZE*x + i]= a0;
                    B[BLOCK_SIZE*y + 1][BLOCK_SIZE*x + i]= a1;
                    B[BLOCK_SIZE*y + 2][BLOCK_SIZE*x + i]= a2;
                    B[BLOCK_SIZE*y + 3][BLOCK_SIZE*x + i]= a3;

		    //后四个数据本应该写到B21(左下角),为了减少miss,我把它们写到B12(右上角)的对应位置
                    B[BLOCK_SIZE*y + 0][BLOCK_SIZE*x + i + 4]= a4;
                    B[BLOCK_SIZE*y + 1][BLOCK_SIZE*x + i + 4]= a5;
                    B[BLOCK_SIZE*y + 2][BLOCK_SIZE*x + i + 4]= a6;
                    B[BLOCK_SIZE*y + 3][BLOCK_SIZE*x + i + 4]= a7;
                }

                //处理A矩阵的后四行,这次是按列读取数据,每次先读第i列,再读i+4列
                for(i = 0; i <SUB_BLOCK_SIZE; i ++)
                {
                    //先读取第i列的四个数
                    a0 = A[BLOCK_SIZE*x + 4][BLOCK_SIZE*y + i];
                    a1 = A[BLOCK_SIZE*x + 5][BLOCK_SIZE*y + i];
                    a2 = A[BLOCK_SIZE*x + 6][BLOCK_SIZE*y + i];
                    a3 = A[BLOCK_SIZE*x + 7][BLOCK_SIZE*y + i];
                    
                    //先交换第i列的四个数和B12对应的行
		    //此时B12对应的行正确了,而a0-a3也成为B21对应行的正确数据
                    a4 = B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 4];
                    a5 = B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 5];
                    a6 = B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 6];
                    a7 = B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 7];
                    B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 4] = a0;
                    B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 5] = a1;
                    B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 6] = a2;
                    B[BLOCK_SIZE*y + i][BLOCK_SIZE*x + 7] = a3;
                    a0 = a4;
                    a1 = a5;
                    a2 = a6;
                    a3 = a7;

                    //然后读取第i+4列的四个数
                    a4 = A[BLOCK_SIZE*x + 4][BLOCK_SIZE*y + i + 4];
                    a5 = A[BLOCK_SIZE*x + 5][BLOCK_SIZE*y + i + 4];
                    a6 = A[BLOCK_SIZE*x + 6][BLOCK_SIZE*y + i + 4];
                    a7 = A[BLOCK_SIZE*x + 7][BLOCK_SIZE*y + i + 4];

                    //写B21,B22对应的行--因为这一行往上推4行的数据已经处理完毕,因此此时读取这一行替换是有道理的
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 0] = a0;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 1] = a1;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 2] = a2;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 3] = a3;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 4] = a4;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 5] = a5;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 6] = a6;
                    B[BLOCK_SIZE*y + 4 + i][BLOCK_SIZE*x + 7] = a7;
                }
            }    
        }
    
    }
	


    //情况3:61*67
    else if (M == 61)
    {
	//和情况1类似,还是读取每个8*8矩阵
       for(x = 0; x < 9; x ++)
        {
            for(y = 0; y < 8; y ++)
            {    
                for(i = 0; i < BLOCK_SIZE; i ++)
                {
		    //边界处理
                    if(BLOCK_SIZE*x + i < 67)
                    {
			//读
                        a0 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 0];
                        a1 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 1];
                        a2 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 2];
                        a3 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 3];
                        a4 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 4];
			//还是边界处理
                        if(BLOCK_SIZE*y+5 <61)
                        {
                           a5 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 5];
                           a6 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 6];
                           a7 = A[BLOCK_SIZE*x + i][BLOCK_SIZE*y + 7];
                        }

			//写
                        B[BLOCK_SIZE*y + 0][BLOCK_SIZE*x + i]= a0;
                        B[BLOCK_SIZE*y + 1][BLOCK_SIZE*x + i]= a1;
                        B[BLOCK_SIZE*y + 2][BLOCK_SIZE*x + i]= a2;
                        B[BLOCK_SIZE*y + 3][BLOCK_SIZE*x + i]= a3;
                        B[BLOCK_SIZE*y + 4][BLOCK_SIZE*x + i]= a4;
			//还是边界处理
                        if(BLOCK_SIZE*y+5 <61)
                        {
                           B[BLOCK_SIZE*y + 5][BLOCK_SIZE*x + i]= a5;
                           B[BLOCK_SIZE*y + 6][BLOCK_SIZE*x + i]= a6;
                           B[BLOCK_SIZE*y + 7][BLOCK_SIZE*x + i]= a7;
                        }
                    }
                }  
            }
        }
    }

    


       



   
}




/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

