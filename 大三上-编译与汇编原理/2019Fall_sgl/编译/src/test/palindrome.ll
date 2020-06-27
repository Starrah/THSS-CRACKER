; ModuleID = ""
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

define i32 @"main"() 
{
main.entry:
  %".2" = getelementptr inbounds [47 x i8], [47 x i8]* @".str0", i32 0, i32 0
  %".3" = call i32 (i8*, ...) @"printf"(i8* %".2")
  %"StringGet" = alloca [1002 x i8]
  %".4" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"StringGet", i32 0, i32 0
  %".5" = call i32 (...) @"gets"(i8* %".4")
  %"len" = alloca i32
  %"i" = alloca i32
  %".6" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"StringGet", i32 0, i32 0
  %".7" = call i32 @"strlen"(i8* %".6")
  store i32 %".7", i32* %"len"
  br label %".9"
.9:
  %".14" = load i32, i32* %"len"
  %".15" = icmp slt i32 %".14", 0
  %".16" = load i32, i32* %"len"
  %".17" = icmp sgt i32 %".16", 1000
  %".18" = or i1 %".15", %".17"
  br i1 %".18", label %".12", label %".13"
.10:
  ret i32 0
.12:
  %".20" = getelementptr inbounds [36 x i8], [36 x i8]* @".str1", i32 0, i32 0
  %".21" = call i32 (i8*, ...) @"printf"(i8* %".20")
  %".22" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"StringGet", i32 0, i32 0
  %".23" = call i32 (...) @"gets"(i8* %".22")
  br label %".10"
.13:
  %"IsPLD" = alloca i32
  %".25" = sub i32 0, 1
  store i32 %".25", i32* %"IsPLD"
  store i32 0, i32* %"i"
  br label %".28"
.28:
  %".32" = load i32, i32* %"i"
  %".33" = load i32, i32* %"i"
  %".34" = add i32 %".32", %".33"
  %".35" = load i32, i32* %"len"
  %".36" = icmp slt i32 %".34", %".35"
  %".37" = load i32, i32* %"IsPLD"
  %".38" = icmp ne i32 %".37", 1
  %".39" = and i1 %".36", %".38"
  br i1 %".39", label %".29", label %".30"
.29:
  br label %".41"
.30:
  br label %".66"
.41:
  %".46" = load i32, i32* %"len"
  %".47" = sub i32 %".46", 1
  %".48" = load i32, i32* %"i"
  %".49" = sub i32 %".47", %".48"
  %".50" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"StringGet", i32 0, i32 %".49"
  %".51" = load i8, i8* %".50"
  %".52" = load i32, i32* %"i"
  %".53" = getelementptr inbounds [1002 x i8], [1002 x i8]* %"StringGet", i32 0, i32 %".52"
  %".54" = load i8, i8* %".53"
  %".55" = icmp ne i8 %".51", %".54"
  br i1 %".55", label %".44", label %".45"
.42:
  %".62" = load i32, i32* %"i"
  %".63" = add i32 %".62", 1
  store i32 %".63", i32* %"i"
  br label %".28"
.44:
  %".57" = getelementptr inbounds [7 x i8], [7 x i8]* @".str2", i32 0, i32 0
  %".58" = call i32 (i8*, ...) @"printf"(i8* %".57")
  store i32 1, i32* %"IsPLD"
  br label %".42"
.45:
  br label %".42"
.66:
  %".71" = load i32, i32* %"IsPLD"
  %".72" = icmp ne i32 %".71", 1
  br i1 %".72", label %".69", label %".70"
.67:
  br label %".10"
.69:
  %".74" = getelementptr inbounds [6 x i8], [6 x i8]* @".str3", i32 0, i32 0
  %".75" = call i32 (i8*, ...) @"printf"(i8* %".74")
  br label %".67"
.70:
  br label %".67"
}

declare i32 @"printf"(i8* %".1", ...) 

@".str0" = constant [47 x i8] c"Please input Char Array Under 1000 characters\0a\00"
declare i32 @"gets"(...) 

declare i32 @"strlen"(i8* %".1") 

@".str1" = constant [36 x i8] c"Input Error, only one more chance:\0a\00"
@".str2" = constant [7 x i8] c"False\0a\00"
@".str3" = constant [6 x i8] c"True\0a\00"