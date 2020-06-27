grammar simpleC;

prog :(include)* (initialBlock|arrayInitBlock|structInitBlock|mStructDef|mFunction)*;
//prog : (forBlock)*;

//-------------语法规则----------------------------------------------
include : '#include' '<' mLIB '>';

//结构体
mStructDef : mStruct '{' (structParam)+ '}'';';

//结构体中参数
structParam : (mType|mStruct) (mID|mArray) (',' (mID|mArray))* ';';

//函数
mFunction : (mType|mVoid|mStruct) mID '(' params ')' '{' funcBody '}';

//函数参数
params : param (','param)* |;
param : mType mID;

//函数体
funcBody : body returnBlock;

//语句块/函数快
body : (block | func';')*;

//语句块
block : initialBlock | arrayInitBlock | structInitBlock | assignBlock | ifBlocks | whileBlock | forBlock | returnBlock;

//初始化语句
initialBlock : (mType) mID ('=' expr)? (',' mID ('=' expr)?)* ';';
arrayInitBlock : mType mID '[' mINT ']'';'; 
structInitBlock : mStruct (mID|mArray)';';


//赋值语句
assignBlock : ((arrayItem|mID|structMember) '=')+  expr ';';


//if 语句
ifBlocks : ifBlock (elifBlock)* (elseBlock)?;
ifBlock : 'if' '('condition')' '{' body '}';
elifBlock : 'else' 'if' '(' condition ')' '{' body '}';
elseBlock : 'else' '{' body '}';

condition :  expr;

//while 语句
whileBlock : 'while' '(' condition ')' '{' body '}';

//for 语句
forBlock : 'for' '(' for1Block  ';' condition ';' for3Block ')' ('{' body '}'|';');
for1Block :  mID '=' expr (',' for1Block)?|;
for3Block : mID '=' expr (',' for3Block)?|;

//return 语句
returnBlock : 'return' (mINT|mID)? ';';

expr
    : '(' expr ')'               #parens
    | op='!' expr                   #Neg
    | expr op=('*' | '/' | '%') expr   #MulDiv 
    | expr op=('+' | '-') expr   #AddSub
    | expr op=('==' | '!=' | '<' | '<=' | '>' | '>=') expr #Judge
    | expr '&&' expr             # AND
    | expr '||' expr             # OR
    | arrayItem                  #arrayitem
    | structMember               #structmember
    | (op='-')? mINT             #int                          
    | (op='-')? mDOUBLE          #double
    | mCHAR                       #char
    | mSTRING                     #string             
    | mID                         #identifier   
    | func                       #function                                     
    ;

mType : 'int'| 'double'| 'char'| 'string';

mArray : mID '[' mINT ']'; 

mVoid : 'void';

mStruct : 'struct' mID;

structMember: (mID | arrayItem)'.'(mID | arrayItem);

arrayItem : mID '[' expr ']';


//函数
func : (strlenFunc | atoiFunc | printfFunc | scanfFunc | getsFunc | selfDefinedFunc);

//strlen
strlenFunc : 'strlen' '(' mID ')';

//atoi
atoiFunc : 'atoi' '(' mID ')' ;

//printf
printfFunc 
    : 'printf' '(' (mSTRING | mID) (','expr)* ')';

//scanf
scanfFunc : 'scanf' '(' mSTRING (','('&')?(mID|arrayItem|structMember))* ')';

//gets
getsFunc : 'gets' '(' mID ')';

//Selfdefined
selfDefinedFunc : mID '('((argument|mID)(','(argument|mID))*)? ')';

argument : mINT | mDOUBLE | mCHAR | mSTRING;

//mID
mID : ID;

//mINT
mINT : INT;

//mDOUBLE
mDOUBLE : DOUBLE;

//mCHAR
mCHAR : CHAR;

//mSTRING
mSTRING : STRING;

//mLIB
mLIB : LIB;

//-------------词法规则----------------------------------------------

ID : [a-zA-Z_][0-9A-Za-z_]*;

INT : [0-9]+;

DOUBLE : [0-9]+'.'[0-9]+;

CHAR : '\''.'\'';

STRING : '"'.*?'"';


LIB : [a-zA-Z]+'.h'?;

Conjunction : '&&' | '||';

Operator : '!' | '+' | '-' | '*' | '/' | '==' | '!=' | '<' | '<=' | '>' | '>=';

//UnaryOperator :  '&' | '*' | '+' | '-' | '~' | '!';

LineComment: '//'.*?'\r'?'\n'   -> skip;

BlockComment:  '/*'.*?'*/'  -> skip;

WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines

