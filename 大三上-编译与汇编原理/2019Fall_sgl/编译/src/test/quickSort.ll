; ModuleID = ""
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@"a" = internal global [1001 x i32] undef
@"InputArray" = internal global i32 undef
define void @"QuickSort"(i32 %"left", i32 %"right") 
{
QuickSort.entry:
  %".4" = alloca i32
  store i32 %"left", i32* %".4"
  %".6" = alloca i32
  store i32 %"right", i32* %".6"
  %"i" = alloca i32
  %"j" = alloca i32
  %"temp" = alloca i32
  %"t" = alloca i32
  br label %".8"
.8:
  %".13" = load i32, i32* %".4"
  %".14" = load i32, i32* %".6"
  %".15" = icmp sgt i32 %".13", %".14"
  br i1 %".15", label %".11", label %".12"
.9:
  %".19" = load i32, i32* %".4"
  %".20" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".19"
  %".21" = load i32, i32* %".20"
  store i32 %".21", i32* %"temp"
  %".23" = load i32, i32* %".4"
  store i32 %".23", i32* %"i"
  %".25" = load i32, i32* %".6"
  store i32 %".25", i32* %"j"
  br label %".27"
.11:
  ret void
.12:
  br label %".9"
.27:
  %".31" = load i32, i32* %"i"
  %".32" = load i32, i32* %"j"
  %".33" = icmp ne i32 %".31", %".32"
  br i1 %".33", label %".28", label %".29"
.28:
  br label %".35"
.29:
  %".97" = load i32, i32* %"i"
  %".98" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".97"
  %".99" = load i32, i32* %".98"
  %".100" = load i32, i32* %".4"
  %".101" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".100"
  store i32 %".99", i32* %".101"
  %".103" = load i32, i32* %"temp"
  %".104" = load i32, i32* %"i"
  %".105" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".104"
  store i32 %".103", i32* %".105"
  %"temp1" = alloca i32
  %".107" = load i32, i32* %"i"
  %".108" = sub i32 %".107", 1
  store i32 %".108", i32* %"temp1"
  %"temp2" = alloca i32
  %".110" = load i32, i32* %"i"
  %".111" = add i32 %".110", 1
  store i32 %".111", i32* %"temp2"
  %".113" = load i32, i32* %".4"
  %".114" = load i32, i32* %"temp1"
  call void @"QuickSort"(i32 %".113", i32 %".114")
  %".116" = load i32, i32* %"temp2"
  %".117" = load i32, i32* %".6"
  call void @"QuickSort"(i32 %".116", i32 %".117")
  ret void
.35:
  %".39" = load i32, i32* %"j"
  %".40" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".39"
  %".41" = load i32, i32* %".40"
  %".42" = load i32, i32* %"temp"
  %".43" = icmp sge i32 %".41", %".42"
  %".44" = load i32, i32* %"i"
  %".45" = load i32, i32* %"j"
  %".46" = icmp slt i32 %".44", %".45"
  %".47" = and i1 %".43", %".46"
  br i1 %".47", label %".36", label %".37"
.36:
  %".49" = load i32, i32* %"j"
  %".50" = sub i32 %".49", 1
  store i32 %".50", i32* %"j"
  br label %".35"
.37:
  br label %".53"
.53:
  %".57" = load i32, i32* %"i"
  %".58" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".57"
  %".59" = load i32, i32* %".58"
  %".60" = load i32, i32* %"temp"
  %".61" = icmp sle i32 %".59", %".60"
  %".62" = load i32, i32* %"i"
  %".63" = load i32, i32* %"j"
  %".64" = icmp slt i32 %".62", %".63"
  %".65" = and i1 %".61", %".64"
  br i1 %".65", label %".54", label %".55"
.54:
  %".67" = load i32, i32* %"i"
  %".68" = add i32 %".67", 1
  store i32 %".68", i32* %"i"
  br label %".53"
.55:
  br label %".71"
.71:
  %".76" = load i32, i32* %"i"
  %".77" = load i32, i32* %"j"
  %".78" = icmp slt i32 %".76", %".77"
  br i1 %".78", label %".74", label %".75"
.72:
  br label %".27"
.74:
  %".80" = load i32, i32* %"i"
  %".81" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".80"
  %".82" = load i32, i32* %".81"
  store i32 %".82", i32* %"t"
  %".84" = load i32, i32* %"j"
  %".85" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".84"
  %".86" = load i32, i32* %".85"
  %".87" = load i32, i32* %"i"
  %".88" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".87"
  store i32 %".86", i32* %".88"
  %".90" = load i32, i32* %"t"
  %".91" = load i32, i32* %"j"
  %".92" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".91"
  store i32 %".90", i32* %".92"
  br label %".72"
.75:
  br label %".72"
}

define i32 @"main"() 
{
main.entry:
  %".2" = getelementptr inbounds [24 x i8], [24 x i8]* @".str0", i32 0, i32 0
  %".3" = call i32 (i8*, ...) @"printf"(i8* %".2")
  %".4" = getelementptr inbounds [3 x i8], [3 x i8]* @".str1", i32 0, i32 0
  %".5" = call i32 (i8*, ...) @"scanf"(i8* %".4", i32* @"InputArray")
  %".6" = getelementptr inbounds [27 x i8], [27 x i8]* @".str2", i32 0, i32 0
  %".7" = call i32 (i8*, ...) @"printf"(i8* %".6")
  %"i" = alloca i32
  %"j" = alloca i32
  %"t" = alloca i32
  store i32 0, i32* %"i"
  br label %".9"
.9:
  %".13" = load i32, i32* %"i"
  %".14" = load i32, i32* @"InputArray"
  %".15" = icmp slt i32 %".13", %".14"
  br i1 %".15", label %".10", label %".11"
.10:
  %".17" = getelementptr inbounds [3 x i8], [3 x i8]* @".str3", i32 0, i32 0
  %".18" = load i32, i32* %"i"
  %".19" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".18"
  %".20" = call i32 (i8*, ...) @"scanf"(i8* %".17", i32* %".19")
  %".21" = load i32, i32* %"i"
  %".22" = add i32 %".21", 1
  store i32 %".22", i32* %"i"
  br label %".9"
.11:
  %"temp" = alloca i32
  %".25" = load i32, i32* @"InputArray"
  %".26" = sub i32 %".25", 1
  store i32 %".26", i32* %"temp"
  %".28" = load i32, i32* %"temp"
  call void @"QuickSort"(i32 0, i32 %".28")
  store i32 0, i32* %"i"
  br label %".31"
.31:
  %".35" = load i32, i32* %"i"
  %".36" = load i32, i32* @"InputArray"
  %".37" = icmp slt i32 %".35", %".36"
  br i1 %".37", label %".32", label %".33"
.32:
  %".39" = getelementptr inbounds [4 x i8], [4 x i8]* @".str4", i32 0, i32 0
  %".40" = load i32, i32* %"i"
  %".41" = getelementptr inbounds [1001 x i32], [1001 x i32]* @"a", i32 0, i32 %".40"
  %".42" = load i32, i32* %".41"
  %".43" = call i32 (i8*, ...) @"printf"(i8* %".39", i32 %".42")
  %".44" = load i32, i32* %"i"
  %".45" = add i32 %".44", 1
  store i32 %".45", i32* %"i"
  br label %".31"
.33:
  ret i32 0
}

declare i32 @"printf"(i8* %".1", ...) 

@".str0" = constant [24 x i8] c"Input the array length:\00"
declare i32 @"scanf"(i8* %".1", ...) 

@".str1" = constant [3 x i8] c"%d\00"
@".str2" = constant [27 x i8] c"Input every array element:\00"
@".str3" = constant [3 x i8] c"%d\00"
@".str4" = constant [4 x i8] c"%d \00"