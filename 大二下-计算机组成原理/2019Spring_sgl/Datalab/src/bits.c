/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) 
{
  int z = ~((~x)|(~y));
  return z;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) 
{
  int place = n<<3; //put what you want to the rightest
  int ans = (x >> place) & 0xff;//extract what you want
  return ans;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) 
{
  int anti_auxiliary = 1<<31; int sgn;
  sgn = (x & anti_auxiliary); //get the first bit of x
  sgn = (sgn >> n) << 1; //if x<0,let the first n bit of signal become 1, and the rest are 0
  x = x >> n;
  x = x ^ sgn;  //eliminate the 1s of the first n bits
  return x;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) 
{

  int get_every_two = 0x55,get_every_four = 0x33,get_every_eight = 0x0f,get_every_16 = 0xff,get_every_32;   

  int number_each_2, number_each_4,number_each_8,number_each_16,ans;


  get_every_two = get_every_two + (get_every_two<<8);
  get_every_two = get_every_two + (get_every_two<<16); //build 0101010101010101.....

  
  get_every_four = get_every_four + (get_every_four<<8);
  get_every_four = get_every_four + (get_every_four<<16); //build 0011001100110011....

  
  get_every_eight = get_every_eight + (get_every_eight<<8);
  get_every_eight = get_every_eight + (get_every_eight<<16); //build 0000111100001111....

  
  get_every_16 = get_every_16 + (get_every_16 << 16);//build 0*8,1*8,0*8,1*8

  get_every_32 = 0xff + (0xff<<8);   //build 0*16,1*16

  number_each_2 = (x & get_every_two) + ((x >> 1) & get_every_two);   //each two numbers: store the numbers of 1 in each 2 numbers in x
  number_each_4 = (number_each_2 & get_every_four) + ((number_each_2 >> 2) & get_every_four); //each four numbers:store the numbers of 1 in each 4 numbers in x
  number_each_8 = (number_each_4 & get_every_eight) + ((number_each_4 >> 4) & get_every_eight);
  number_each_16 = (number_each_8 & get_every_16) + ((number_each_8 >> 8) & get_every_16);
  ans = (number_each_16 & get_every_32) + ((number_each_16 >> 16) & get_every_32);
  return ans;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) 
{
  //core:only 0 and -0 has the same signal of 0, others not
  int reverse = ~x+1;   //get -x
  int max = 1<<31;    
  int first_place = (x|reverse)&max;
  return ((~first_place) >> 31)&1; 
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) 
{
  return 1<<31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) 
{
  int whether_32 = n >> 5;//if n = 32, always return 1
  int min = 1 << 31;
  int additive = (1 << n >> 1) & (~min);//get 2^(n-1)
  x += additive;  //if x is, let x be from 0 to 2^n -1
  return (!(x>>n))|whether_32;  //if n == 32,yes, else,if 0<=x + 2^(n-1)<2^n,yes
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) 
{
   int min, signal, signal_n;
   min = 1<<31;
   signal = x & min;      //get x's signal
   signal_n = n & (signal>>31); 
   x = x + (1<<signal_n) + (~0); //x<0: x+= 2^n - 1, else, x not change
    return x >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) 
{
  x = ~x;
  x += 1;
  return x;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) 
{
  int auxiliary = 1<<31;
  int a = !!(x & auxiliary);//get the signal of x,if x <0, then a = 1, else a = 0
  int whether = !x;  //if x = 0, whether = 1, else whether = 0
  return !(a + whether);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) 
{
  int reverse_y = ~y + 1;
  int substraction = x + reverse_y;
  int min = 1 << 31;

  //get the sign of x,y,x-y 
  int sign_x = x & min;
  int sign_y = y & min;
  int sign_z = substraction & min;

  int sign_yy = ~sign_y;
  int last = (sign_x& sign_yy)|(sign_x&sign_z)|(sign_yy&sign_z)|((!substraction)<<31);
  return (last>>31)&1;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) 
{
  int current,place;  //current: the current binary string you are handling; place:the least place of current
  int front,back;     //the first half and second half of current
  int whether,calculator; //whether:used to judge whether front is 0; calculator:used to renew current and place

  current = x; place = 0;

  //current.length = 32
  front = current>>16;
  back = current;
  whether = !front;
  calculator = (~whether) + 1;  
  current = front + (calculator & back);  //if whether==1, front = 0, back remains; else, back is eliminated, front remains 
  place = place + ((~calculator) & 16);   //if whether == 1, place remains unchanged, else, place += half_the_length

  //current.length = 16
  front = current>>8;
  back = current;
  whether = !front;
  calculator = (~whether) + 1;
  current = front + (calculator & back);
  place = place + ((~calculator) & 8);

  //current.length = 8
  front = current>>4;
  back = current;
  whether = !front;
  calculator = (~whether) + 1;
  current = front + (calculator & back);
  place = place + ((~calculator) & 4);

  //current.length = 4
  front = current>>2;
  back = current;
  whether = !front;
  calculator = (~whether) + 1;
  current = front + (calculator & back);
  place = place + ((~calculator) & 2);

  //current.length = 2
  front = current>>1;
  back = current;
  whether = !front;
  calculator = (~whether) + 1;
  current = front + (calculator & back);
  place = place + ((~calculator) & 1);

  return place;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) 
{

 int first;
 int decimal_judge,exp_judge;
 exp_judge = ((uf >> 23) ^ 0xff)<<24;     //if exp == 11111111, exp_judge = 0
 decimal_judge = uf << 9;   //if(decimal == all 0,decimal_judge = 0)
 first = 1<<31;      //sign
 if(!decimal_judge || exp_judge)
 {
 uf = uf ^ first;
 }
  return uf;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) 
{
  int sign;           //the sign of the float
  int exp,exp_show;   //exp:the 2^n of the first_1, exp_decimal:the 2^n of the second 1
  int a,ans;          //a:temp used to get exp, ans:result

  //handle min_int
  int min_int = 1<<31;      
  int min_int_float;
  int sign_decimal_up;
  int decimal,decimal_real,decimal_left;      //store decimal
  int right_place_decimal, place_minus1 ;    //the place of movement
  int round_1,round_2;  //all equals 1:decimal ++
  int removal;          //used to remove the first 1
  int bias = 127;

  //initialize
  ans = 0;
  exp = 126;
  //min_int = 1 << 31;

  //get and eliminate x's sign
  if(x >= 0) sign = 0;
  else
  {
    sign = 1;
    x = -x;
  } 
  
  if(!x) return 0;  //judge 0  
  if(x == min_int)  //judge MIN_INT
  {
    min_int_float = min_int;
    min_int_float = min_int_float + (158 << 23);
    return min_int_float;
  }

    a = x;

  while(a) //others
  {
    a = a >> 1;
    exp = exp +1;
  }

  exp_show = exp;
  exp = exp - bias;
  ans = sign << 31;
  ans = ans + (exp_show<<23);
  //get the exp place and sign


  removal = 1<<(exp);
  decimal = x^removal;
  //get decimal

  right_place_decimal = 23 - exp;   //>0:left shift, else, right_shift
  
  if(right_place_decimal < 0)
  {
    //judge round
    right_place_decimal = -right_place_decimal;
    place_minus1 = right_place_decimal - 1;
    sign_decimal_up = 1 << place_minus1;
    decimal_real = decimal >> right_place_decimal;
    decimal_left = decimal - (decimal_real<<right_place_decimal);
    

    round_2 = decimal_real & 1;                       //whether the decimal need to round to even
    if(round_2 + decimal_left>sign_decimal_up) decimal_real = decimal_real + 1;
  }
  else
  {

     decimal_real = decimal << right_place_decimal;
  }
  ans = ans + decimal_real;
  return ans;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) 
{
  int sign,exp,back;      
  int add_sign,back_sign;  //auxiliary
  int whether_less,whether_more;    //used to judge whether denormalized or overflow
  int whether_advance;    //when the exp == 0, used to judge whether becomes normalized

  //build constants
  back_sign = 0xff + (0xff<<8)+(0x7f<<16);
  add_sign = 1<<22;

  //get the three parts of a float
  sign = uf & (1<<31);
  exp = (uf>>23)&0xff;
  back = uf&back_sign;


  whether_less = !exp;          //denormalized
  whether_more = 0xff^exp;      //overflow
  whether_advance = back&add_sign;  //used when a denormalized becomes normalized

  //overflow originally
  if(!whether_more)
  {
      return uf;
  } 

  if(whether_less) 
  {
    back = (back << 1) & back_sign; //left shift
    if(whether_advance)
    {
      exp = 1;
    }
  }
  else
  {
    exp = exp + 1;
  }
  

  //renew whether overflow
  whether_more = 0xff^exp;
  if(!whether_more) 
  {
    return sign +(0xff<<23); //infinity
  }
  else 
  {
    return sign + (exp<<23)+back;  //normal
  }
}
