; ModuleID = ""
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@"nodes" = internal global [101 x {i32, i32, i32, i32}] zeroinitializer
define void @"initTree"() 
{
initTree.entry:
  %"i" = alloca i32
  store i32 1, i32* %"i"
  br label %".3"
.3:
  %".7" = load i32, i32* %"i"
  %".8" = icmp slt i32 %".7", 101
  br i1 %".8", label %".4", label %".5"
.4:
  %".10" = load i32, i32* %"i"
  %".11" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".10"
  %".12" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".11", i32 0, i32 0
  store i32 0, i32* %".12"
  %".14" = sub i32 0, 1
  %".15" = load i32, i32* %"i"
  %".16" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".15"
  %".17" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".16", i32 0, i32 1
  store i32 %".14", i32* %".17"
  %".19" = sub i32 0, 1
  %".20" = load i32, i32* %"i"
  %".21" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".20"
  %".22" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".21", i32 0, i32 2
  store i32 %".19", i32* %".22"
  %".24" = load i32, i32* %"i"
  %".25" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".24"
  %".26" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".25", i32 0, i32 3
  store i32 0, i32* %".26"
  %".28" = load i32, i32* %"i"
  %".29" = add i32 %".28", 1
  store i32 %".29", i32* %"i"
  br label %".3"
.5:
  ret void
}

define void @"addLeftNode"(i32 %"root", i32 %"value") 
{
addLeftNode.entry:
  %".4" = alloca i32
  store i32 %"root", i32* %".4"
  %".6" = alloca i32
  store i32 %"value", i32* %".6"
  br label %".8"
.8:
  %".13" = load i32, i32* %".4"
  %".14" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".13"
  %".15" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".14", i32 0, i32 3
  %".16" = load i32, i32* %".15"
  %".17" = icmp eq i32 %".16", 1
  br i1 %".17", label %".11", label %".12"
.9:
  ret void
.11:
  %"child" = alloca i32
  %".19" = load i32, i32* %".4"
  %".20" = mul i32 2, %".19"
  store i32 %".20", i32* %"child"
  %".22" = load i32, i32* %"child"
  %".23" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".22"
  %".24" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".23", i32 0, i32 3
  store i32 1, i32* %".24"
  %".26" = load i32, i32* %".6"
  %".27" = load i32, i32* %"child"
  %".28" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".27"
  %".29" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".28", i32 0, i32 0
  store i32 %".26", i32* %".29"
  %".31" = load i32, i32* %"child"
  %".32" = load i32, i32* %".4"
  %".33" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".32"
  %".34" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".33", i32 0, i32 1
  store i32 %".31", i32* %".34"
  br label %".9"
.12:
  %".37" = getelementptr inbounds [38 x i8], [38 x i8]* @".str0", i32 0, i32 0
  %".38" = call i32 (i8*, ...) @"printf"(i8* %".37")
  br label %".9"
}

declare i32 @"printf"(i8* %".1", ...) 

@".str0" = constant [38 x i8] c"Add node fail, Not a valid root node!\00"
define void @"addRightNode"(i32 %"root", i32 %"value") 
{
addRightNode.entry:
  %".4" = alloca i32
  store i32 %"root", i32* %".4"
  %".6" = alloca i32
  store i32 %"value", i32* %".6"
  br label %".8"
.8:
  %".13" = load i32, i32* %".4"
  %".14" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".13"
  %".15" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".14", i32 0, i32 3
  %".16" = load i32, i32* %".15"
  %".17" = icmp eq i32 %".16", 1
  br i1 %".17", label %".11", label %".12"
.9:
  ret void
.11:
  %"child" = alloca i32
  %".19" = load i32, i32* %".4"
  %".20" = mul i32 2, %".19"
  %".21" = add i32 %".20", 1
  store i32 %".21", i32* %"child"
  %".23" = load i32, i32* %"child"
  %".24" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".23"
  %".25" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".24", i32 0, i32 3
  store i32 1, i32* %".25"
  %".27" = load i32, i32* %".6"
  %".28" = load i32, i32* %"child"
  %".29" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".28"
  %".30" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".29", i32 0, i32 0
  store i32 %".27", i32* %".30"
  %".32" = load i32, i32* %"child"
  %".33" = load i32, i32* %".4"
  %".34" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".33"
  %".35" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".34", i32 0, i32 2
  store i32 %".32", i32* %".35"
  br label %".9"
.12:
  %".38" = getelementptr inbounds [38 x i8], [38 x i8]* @".str1", i32 0, i32 0
  %".39" = call i32 (i8*, ...) @"printf"(i8* %".38")
  br label %".9"
}

@".str1" = constant [38 x i8] c"Add node fail, Not a valid root node!\00"
define void @"preOrderTravel"(i32 %"root") 
{
preOrderTravel.entry:
  %".3" = alloca i32
  store i32 %"root", i32* %".3"
  br label %".5"
.5:
  %".10" = load i32, i32* %".3"
  %".11" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".10"
  %".12" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".11", i32 0, i32 3
  %".13" = load i32, i32* %".12"
  %".14" = icmp eq i32 %".13", 1
  br i1 %".14", label %".8", label %".9"
.6:
  ret void
.8:
  %"left" = alloca i32
  %".16" = load i32, i32* %".3"
  %".17" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".16"
  %".18" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".17", i32 0, i32 1
  %".19" = load i32, i32* %".18"
  store i32 %".19", i32* %"left"
  %"right" = alloca i32
  %".21" = load i32, i32* %".3"
  %".22" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".21"
  %".23" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".22", i32 0, i32 2
  %".24" = load i32, i32* %".23"
  store i32 %".24", i32* %"right"
  %".26" = getelementptr inbounds [5 x i8], [5 x i8]* @".str2", i32 0, i32 0
  %".27" = load i32, i32* %".3"
  %".28" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 %".27"
  %".29" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".28", i32 0, i32 0
  %".30" = load i32, i32* %".29"
  %".31" = call i32 (i8*, ...) @"printf"(i8* %".26", i32 %".30")
  %".32" = load i32, i32* %"left"
  call void @"preOrderTravel"(i32 %".32")
  %".34" = load i32, i32* %"right"
  call void @"preOrderTravel"(i32 %".34")
  br label %".6"
.9:
  ret void
}

@".str2" = constant [5 x i8] c"%d->\00"
define i32 @"main"() 
{
main.entry:
  call void @"initTree"()
  %".3" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 1
  %".4" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".3", i32 0, i32 0
  store i32 1, i32* %".4"
  %".6" = getelementptr inbounds [101 x {i32, i32, i32, i32}], [101 x {i32, i32, i32, i32}]* @"nodes", i32 0, i32 1
  %".7" = getelementptr inbounds {i32, i32, i32, i32}, {i32, i32, i32, i32}* %".6", i32 0, i32 3
  store i32 1, i32* %".7"
  call void @"addLeftNode"(i32 1, i32 2)
  call void @"addRightNode"(i32 1, i32 3)
  call void @"addLeftNode"(i32 2, i32 4)
  call void @"addRightNode"(i32 2, i32 5)
  call void @"addLeftNode"(i32 4, i32 8)
  call void @"addRightNode"(i32 4, i32 9)
  %".15" = getelementptr inbounds [27 x i8], [27 x i8]* @".str3", i32 0, i32 0
  %".16" = call i32 (i8*, ...) @"printf"(i8* %".15")
  call void @"preOrderTravel"(i32 1)
  %".18" = getelementptr inbounds [3 x i8], [3 x i8]* @".str4", i32 0, i32 0
  %".19" = call i32 (i8*, ...) @"printf"(i8* %".18")
  ret i32 0
}

@".str3" = constant [27 x i8] c"pre-order traval result: \0a\00"
@".str4" = constant [3 x i8] c"$\0a\00"