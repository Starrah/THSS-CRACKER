; ModuleID = ""
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

define i32 @"main"() 
{
main.entry:
  %"InputArray1" = alloca [1002 x i8]
  %"InputArray2" = alloca [1002 x i8]
  %"FollowIndex" = alloca [1002 x i32]
  %".2" = sub i32 0, 1
  %".3" = getelementptr inbounds [1002 x i32], [1002 x i32]* %"FollowIndex", i32 0, i32 0
  store i32 %".2", i32* %".3"
  %"lenInputArray1" = alloca i32
  %"lenInputArray2" = alloca i32
  %"i" = alloca i32
  %"j" = alloca i32
  %".5" = getelementptr inbounds [22 x i8], [22 x i8]* @".str0", i32 0, i32 0
  %".6" = call i32 (i8*, ...) @"printf"(i8* %".5")
  %".7" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray1", i32 0, i32 0
  %".8" = call i32 (...) @"gets"(i8* %".7")
  %".9" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray1", i32 0, i32 0
  %".10" = call i32 @"strlen"(i8* %".9")
  store i32 %".10", i32* %"lenInputArray1"
  %".12" = getelementptr inbounds [23 x i8], [23 x i8]* @".str1", i32 0, i32 0
  %".13" = call i32 (i8*, ...) @"printf"(i8* %".12")
  %".14" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 0
  %".15" = call i32 (...) @"gets"(i8* %".14")
  %".16" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 0
  %".17" = call i32 @"strlen"(i8* %".16")
  store i32 %".17", i32* %"lenInputArray2"
  store i32 1, i32* %"i"
  %".20" = sub i32 0, 1
  store i32 %".20", i32* %"j"
  br label %".22"
.22:
  %".26" = load i32, i32* %"i"
  %".27" = load i32, i32* %"lenInputArray2"
  %".28" = icmp slt i32 %".26", %".27"
  br i1 %".28", label %".23", label %".24"
.23:
  br label %".30"
.24:
  %"estimation" = alloca i32
  store i32 0, i32* %"estimation"
  store i32 0, i32* %"i"
  %".80" = sub i32 0, 1
  store i32 %".80", i32* %"j"
  br label %".82"
.30:
  %".34" = load i32, i32* %"j"
  %".35" = icmp sge i32 %".34", 0
  %".36" = load i32, i32* %"i"
  %".37" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".36"
  %".38" = load i8, i8* %".37"
  %".39" = load i32, i32* %"j"
  %".40" = add i32 %".39", 1
  %".41" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".40"
  %".42" = load i8, i8* %".41"
  %".43" = icmp ne i8 %".38", %".42"
  %".44" = and i1 %".35", %".43"
  br i1 %".44", label %".31", label %".32"
.31:
  %".46" = load i32, i32* %"j"
  %".47" = getelementptr inbounds [1002 x i32], [1002 x i32]* %"FollowIndex", i32 0, i32 %".46"
  %".48" = load i32, i32* %".47"
  store i32 %".48", i32* %"j"
  br label %".30"
.32:
  br label %".51"
.51:
  %".56" = load i32, i32* %"i"
  %".57" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".56"
  %".58" = load i8, i8* %".57"
  %".59" = load i32, i32* %"j"
  %".60" = add i32 %".59", 1
  %".61" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".60"
  %".62" = load i8, i8* %".61"
  %".63" = icmp eq i8 %".58", %".62"
  br i1 %".63", label %".54", label %".55"
.52:
  %".70" = load i32, i32* %"j"
  %".71" = load i32, i32* %"i"
  %".72" = getelementptr inbounds [1002 x i32], [1002 x i32]* %"FollowIndex", i32 0, i32 %".71"
  store i32 %".70", i32* %".72"
  %".74" = load i32, i32* %"i"
  %".75" = add i32 %".74", 1
  store i32 %".75", i32* %"i"
  br label %".22"
.54:
  %".65" = load i32, i32* %"j"
  %".66" = add i32 %".65", 1
  store i32 %".66", i32* %"j"
  br label %".52"
.55:
  br label %".52"
.82:
  %".86" = load i32, i32* %"i"
  %".87" = load i32, i32* %"lenInputArray1"
  %".88" = icmp slt i32 %".86", %".87"
  br i1 %".88", label %".83", label %".84"
.83:
  br label %".90"
.84:
  br label %".156"
.90:
  %".94" = load i32, i32* %"j"
  %".95" = icmp sge i32 %".94", 0
  %".96" = load i32, i32* %"i"
  %".97" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray1", i32 0, i32 %".96"
  %".98" = load i8, i8* %".97"
  %".99" = load i32, i32* %"j"
  %".100" = add i32 %".99", 1
  %".101" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".100"
  %".102" = load i8, i8* %".101"
  %".103" = icmp ne i8 %".98", %".102"
  %".104" = and i1 %".95", %".103"
  br i1 %".104", label %".91", label %".92"
.91:
  %".106" = load i32, i32* %"j"
  %".107" = getelementptr inbounds [1002 x i32], [1002 x i32]* %"FollowIndex", i32 0, i32 %".106"
  %".108" = load i32, i32* %".107"
  store i32 %".108", i32* %"j"
  br label %".90"
.92:
  br label %".111"
.111:
  %".116" = load i32, i32* %"i"
  %".117" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray1", i32 0, i32 %".116"
  %".118" = load i8, i8* %".117"
  %".119" = load i32, i32* %"j"
  %".120" = add i32 %".119", 1
  %".121" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"InputArray2", i32 0, i32 %".120"
  %".122" = load i8, i8* %".121"
  %".123" = icmp eq i8 %".118", %".122"
  br i1 %".123", label %".114", label %".115"
.112:
  br label %".130"
.114:
  %".125" = load i32, i32* %"j"
  %".126" = add i32 %".125", 1
  store i32 %".126", i32* %"j"
  br label %".112"
.115:
  br label %".112"
.130:
  %".135" = load i32, i32* %"j"
  %".136" = add i32 %".135", 1
  %".137" = load i32, i32* %"lenInputArray2"
  %".138" = icmp eq i32 %".136", %".137"
  br i1 %".138", label %".133", label %".134"
.131:
  %".152" = load i32, i32* %"i"
  %".153" = add i32 %".152", 1
  store i32 %".153", i32* %"i"
  br label %".82"
.133:
  store i32 1, i32* %"estimation"
  %".141" = getelementptr inbounds [4 x i8], [4 x i8]* @".str2", i32 0, i32 0
  %".142" = load i32, i32* %"i"
  %".143" = load i32, i32* %"j"
  %".144" = sub i32 %".142", %".143"
  %".145" = call i32 (i8*, ...) @"printf"(i8* %".141", i32 %".144")
  %".146" = load i32, i32* %"j"
  %".147" = getelementptr inbounds [1002 x i32], [1002 x i32]* %"FollowIndex", i32 0, i32 %".146"
  %".148" = load i32, i32* %".147"
  store i32 %".148", i32* %"j"
  br label %".131"
.134:
  br label %".131"
.156:
  %".161" = load i32, i32* %"estimation"
  %".162" = icmp eq i32 %".161", 0
  br i1 %".162", label %".159", label %".160"
.157:
  ret i32 0
.159:
  %".164" = getelementptr inbounds [7 x i8], [7 x i8]* @".str3", i32 0, i32 0
  %".165" = call i32 (i8*, ...) @"printf"(i8* %".164")
  br label %".157"
.160:
  br label %".157"
}

declare i32 @"printf"(i8* %".1", ...) 

@".str0" = constant [22 x i8] c"input source string: \00"
declare i32 @"gets"(...) 

declare i32 @"strlen"(i8* %".1") 

@".str1" = constant [23 x i8] c"input pattern string: \00"
@".str2" = constant [4 x i8] c"%d\0a\00"
@".str3" = constant [7 x i8] c"False\0a\00"