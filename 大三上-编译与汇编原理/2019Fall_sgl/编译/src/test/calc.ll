; ModuleID = ""
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

define i32 @"main"() 
{
main.entry:
  %"InputExpression" = alloca [2000 x i8]
  %"OperationUpdated" = alloca [2000 x i8]
  %"NumUpdated" = alloca [2000 x i32]
  %"PresentNum" = alloca i32
  %".2" = sub i32 0, 1
  store i32 %".2", i32* %"PresentNum"
  %"PresentOperator" = alloca i32
  %".4" = sub i32 0, 1
  store i32 %".4", i32* %"PresentOperator"
  %"DecimalProcess" = alloca i32
  store i32 1, i32* %"DecimalProcess"
  %"StrLength" = alloca i32
  %"num" = alloca i32
  store i32 0, i32* %"num"
  %"i" = alloca i32
  %".8" = getelementptr inbounds [39 x i8], [39 x i8]* @".str0", i32 0, i32 0
  %".9" = call i32 (i8*, ...) @"printf"(i8* %".8")
  %".10" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 0
  %".11" = call i32 (...) @"gets"(i8* %".10")
  %".12" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 0
  %".13" = call i32 @"strlen"(i8* %".12")
  store i32 %".13", i32* %"StrLength"
  %".15" = load i32, i32* %"StrLength"
  %".16" = sub i32 %".15", 1
  store i32 %".16", i32* %"i"
  br label %".18"
.18:
  %".22" = load i32, i32* %"i"
  %".23" = icmp sge i32 %".22", 0
  br i1 %".23", label %".19", label %".20"
.19:
  %".25" = load i32, i32* %"i"
  %".26" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".25"
  %".27" = load i8, i8* %".26"
  %".28" = load i32, i32* %"i"
  %".29" = add i32 %".28", 1
  %".30" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".29"
  store i8 %".27", i8* %".30"
  %".32" = load i32, i32* %"i"
  %".33" = sub i32 %".32", 1
  store i32 %".33", i32* %"i"
  br label %".18"
.20:
  %".36" = load i32, i32* %"StrLength"
  %".37" = add i32 %".36", 1
  store i32 %".37", i32* %"i"
  %".39" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 0
  store i8 40, i8* %".39"
  %".41" = load i32, i32* %"StrLength"
  %".42" = add i32 %".41", 1
  %".43" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".42"
  store i8 41, i8* %".43"
  %".45" = load i32, i32* %"StrLength"
  %".46" = add i32 %".45", 2
  store i32 %".46", i32* %"StrLength"
  br label %".48"
.48:
  %".52" = load i32, i32* %"i"
  %".53" = icmp sge i32 %".52", 0
  br i1 %".53", label %".49", label %".50"
.49:
  br label %".55"
.50:
  %".419" = getelementptr inbounds [12 x i8], [12 x i8]* @".str1", i32 0, i32 0
  %".420" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 0
  %".421" = load i32, i32* %".420"
  %".422" = call i32 (i8*, ...) @"printf"(i8* %".419", i32 %".421")
  ret i32 0
.55:
  %".60" = load i32, i32* %"i"
  %".61" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".60"
  %".62" = load i8, i8* %".61"
  %".63" = icmp eq i8 %".62", 43
  br i1 %".63", label %".58", label %".59"
.56:
  br label %".48"
.58:
  br label %".65"
.59:
  %".137" = load i32, i32* %"i"
  %".138" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".137"
  %".139" = load i8, i8* %".138"
  %".140" = icmp eq i8 %".139", 45
  br i1 %".140", label %".135", label %".136"
.65:
  %".69" = load i32, i32* %"PresentOperator"
  %".70" = icmp sge i32 %".69", 0
  %".71" = load i32, i32* %"PresentOperator"
  %".72" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".71"
  %".73" = load i8, i8* %".72"
  %".74" = icmp eq i8 %".73", 42
  %".75" = load i32, i32* %"PresentOperator"
  %".76" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".75"
  %".77" = load i8, i8* %".76"
  %".78" = icmp eq i8 %".77", 47
  %".79" = or i1 %".74", %".78"
  %".80" = and i1 %".70", %".79"
  br i1 %".80", label %".66", label %".67"
.66:
  br label %".82"
.67:
  %".125" = load i32, i32* %"PresentOperator"
  %".126" = add i32 %".125", 1
  store i32 %".126", i32* %"PresentOperator"
  %".128" = load i32, i32* %"PresentOperator"
  %".129" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".128"
  store i8 43, i8* %".129"
  %".131" = load i32, i32* %"i"
  %".132" = sub i32 %".131", 1
  store i32 %".132", i32* %"i"
  br label %".56"
.82:
  %".87" = load i32, i32* %"PresentOperator"
  %".88" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".87"
  %".89" = load i8, i8* %".88"
  %".90" = icmp eq i8 %".89", 42
  br i1 %".90", label %".85", label %".86"
.83:
  %".118" = load i32, i32* %"PresentOperator"
  %".119" = sub i32 %".118", 1
  store i32 %".119", i32* %"PresentOperator"
  %".121" = load i32, i32* %"PresentNum"
  %".122" = sub i32 %".121", 1
  store i32 %".122", i32* %"PresentNum"
  br label %".65"
.85:
  %".92" = load i32, i32* %"PresentNum"
  %".93" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".92"
  %".94" = load i32, i32* %".93"
  %".95" = load i32, i32* %"PresentNum"
  %".96" = sub i32 %".95", 1
  %".97" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".96"
  %".98" = load i32, i32* %".97"
  %".99" = mul i32 %".94", %".98"
  %".100" = load i32, i32* %"PresentNum"
  %".101" = sub i32 %".100", 1
  %".102" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".101"
  store i32 %".99", i32* %".102"
  br label %".83"
.86:
  %".105" = load i32, i32* %"PresentNum"
  %".106" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".105"
  %".107" = load i32, i32* %".106"
  %".108" = load i32, i32* %"PresentNum"
  %".109" = sub i32 %".108", 1
  %".110" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".109"
  %".111" = load i32, i32* %".110"
  %".112" = sdiv i32 %".107", %".111"
  %".113" = load i32, i32* %"PresentNum"
  %".114" = sub i32 %".113", 1
  %".115" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".114"
  store i32 %".112", i32* %".115"
  br label %".83"
.135:
  br label %".142"
.136:
  %".214" = load i32, i32* %"i"
  %".215" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".214"
  %".216" = load i8, i8* %".215"
  %".217" = icmp eq i8 %".216", 42
  br i1 %".217", label %".212", label %".213"
.142:
  %".146" = load i32, i32* %"PresentOperator"
  %".147" = icmp sge i32 %".146", 0
  %".148" = load i32, i32* %"PresentOperator"
  %".149" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".148"
  %".150" = load i8, i8* %".149"
  %".151" = icmp eq i8 %".150", 42
  %".152" = load i32, i32* %"PresentOperator"
  %".153" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".152"
  %".154" = load i8, i8* %".153"
  %".155" = icmp eq i8 %".154", 47
  %".156" = or i1 %".151", %".155"
  %".157" = and i1 %".147", %".156"
  br i1 %".157", label %".143", label %".144"
.143:
  br label %".159"
.144:
  %".202" = load i32, i32* %"PresentOperator"
  %".203" = add i32 %".202", 1
  store i32 %".203", i32* %"PresentOperator"
  %".205" = load i32, i32* %"PresentOperator"
  %".206" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".205"
  store i8 45, i8* %".206"
  %".208" = load i32, i32* %"i"
  %".209" = sub i32 %".208", 1
  store i32 %".209", i32* %"i"
  br label %".56"
.159:
  %".164" = load i32, i32* %"PresentOperator"
  %".165" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".164"
  %".166" = load i8, i8* %".165"
  %".167" = icmp eq i8 %".166", 42
  br i1 %".167", label %".162", label %".163"
.160:
  %".195" = load i32, i32* %"PresentNum"
  %".196" = sub i32 %".195", 1
  store i32 %".196", i32* %"PresentNum"
  %".198" = load i32, i32* %"PresentOperator"
  %".199" = sub i32 %".198", 1
  store i32 %".199", i32* %"PresentOperator"
  br label %".142"
.162:
  %".169" = load i32, i32* %"PresentNum"
  %".170" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".169"
  %".171" = load i32, i32* %".170"
  %".172" = load i32, i32* %"PresentNum"
  %".173" = sub i32 %".172", 1
  %".174" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".173"
  %".175" = load i32, i32* %".174"
  %".176" = mul i32 %".171", %".175"
  %".177" = load i32, i32* %"PresentNum"
  %".178" = sub i32 %".177", 1
  %".179" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".178"
  store i32 %".176", i32* %".179"
  br label %".160"
.163:
  %".182" = load i32, i32* %"PresentNum"
  %".183" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".182"
  %".184" = load i32, i32* %".183"
  %".185" = load i32, i32* %"PresentNum"
  %".186" = sub i32 %".185", 1
  %".187" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".186"
  %".188" = load i32, i32* %".187"
  %".189" = sdiv i32 %".184", %".188"
  %".190" = load i32, i32* %"PresentNum"
  %".191" = sub i32 %".190", 1
  %".192" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".191"
  store i32 %".189", i32* %".192"
  br label %".160"
.212:
  %".219" = load i32, i32* %"PresentOperator"
  %".220" = add i32 %".219", 1
  store i32 %".220", i32* %"PresentOperator"
  %".222" = load i32, i32* %"PresentOperator"
  %".223" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".222"
  store i8 42, i8* %".223"
  %".225" = load i32, i32* %"i"
  %".226" = sub i32 %".225", 1
  store i32 %".226", i32* %"i"
  br label %".56"
.213:
  %".231" = load i32, i32* %"i"
  %".232" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".231"
  %".233" = load i8, i8* %".232"
  %".234" = icmp eq i8 %".233", 47
  br i1 %".234", label %".229", label %".230"
.229:
  %".236" = load i32, i32* %"PresentOperator"
  %".237" = add i32 %".236", 1
  store i32 %".237", i32* %"PresentOperator"
  %".239" = load i32, i32* %"PresentOperator"
  %".240" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".239"
  store i8 47, i8* %".240"
  %".242" = load i32, i32* %"i"
  %".243" = sub i32 %".242", 1
  store i32 %".243", i32* %"i"
  br label %".56"
.230:
  %".248" = load i32, i32* %"i"
  %".249" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".248"
  %".250" = load i8, i8* %".249"
  %".251" = icmp eq i8 %".250", 41
  br i1 %".251", label %".246", label %".247"
.246:
  %".253" = load i32, i32* %"PresentOperator"
  %".254" = add i32 %".253", 1
  store i32 %".254", i32* %"PresentOperator"
  %".256" = load i32, i32* %"PresentOperator"
  %".257" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".256"
  store i8 41, i8* %".257"
  %".259" = load i32, i32* %"i"
  %".260" = sub i32 %".259", 1
  store i32 %".260", i32* %"i"
  br label %".56"
.247:
  %".265" = load i32, i32* %"i"
  %".266" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".265"
  %".267" = load i8, i8* %".266"
  %".268" = icmp eq i8 %".267", 40
  br i1 %".268", label %".263", label %".264"
.263:
  br label %".270"
.264:
  store i32 0, i32* %"num"
  store i32 1, i32* %"DecimalProcess"
  br label %".375"
.270:
  %".274" = load i32, i32* %"PresentOperator"
  %".275" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".274"
  %".276" = load i8, i8* %".275"
  %".277" = icmp ne i8 %".276", 41
  br i1 %".277", label %".271", label %".272"
.271:
  %"OperatorGet" = alloca i8
  %".279" = load i32, i32* %"PresentOperator"
  %".280" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"OperationUpdated", i32 0, i32 %".279"
  %".281" = load i8, i8* %".280"
  store i8 %".281", i8* %"OperatorGet"
  %".283" = load i32, i32* %"PresentOperator"
  %".284" = sub i32 %".283", 1
  store i32 %".284", i32* %"PresentOperator"
  br label %".286"
.272:
  %".366" = load i32, i32* %"PresentOperator"
  %".367" = sub i32 %".366", 1
  store i32 %".367", i32* %"PresentOperator"
  %".369" = load i32, i32* %"i"
  %".370" = sub i32 %".369", 1
  store i32 %".370", i32* %"i"
  br label %".56"
.286:
  %".291" = load i8, i8* %"OperatorGet"
  %".292" = icmp eq i8 %".291", 47
  br i1 %".292", label %".289", label %".290"
.287:
  %".362" = load i32, i32* %"PresentNum"
  %".363" = sub i32 %".362", 1
  store i32 %".363", i32* %"PresentNum"
  br label %".270"
.289:
  %".294" = load i32, i32* %"PresentNum"
  %".295" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".294"
  %".296" = load i32, i32* %".295"
  %".297" = load i32, i32* %"PresentNum"
  %".298" = sub i32 %".297", 1
  %".299" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".298"
  %".300" = load i32, i32* %".299"
  %".301" = sdiv i32 %".296", %".300"
  %".302" = load i32, i32* %"PresentNum"
  %".303" = sub i32 %".302", 1
  %".304" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".303"
  store i32 %".301", i32* %".304"
  br label %".287"
.290:
  %".309" = load i8, i8* %"OperatorGet"
  %".310" = icmp eq i8 %".309", 43
  br i1 %".310", label %".307", label %".308"
.307:
  %".312" = load i32, i32* %"PresentNum"
  %".313" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".312"
  %".314" = load i32, i32* %".313"
  %".315" = load i32, i32* %"PresentNum"
  %".316" = sub i32 %".315", 1
  %".317" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".316"
  %".318" = load i32, i32* %".317"
  %".319" = add i32 %".314", %".318"
  %".320" = load i32, i32* %"PresentNum"
  %".321" = sub i32 %".320", 1
  %".322" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".321"
  store i32 %".319", i32* %".322"
  br label %".287"
.308:
  %".327" = load i8, i8* %"OperatorGet"
  %".328" = icmp eq i8 %".327", 45
  br i1 %".328", label %".325", label %".326"
.325:
  %".330" = load i32, i32* %"PresentNum"
  %".331" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".330"
  %".332" = load i32, i32* %".331"
  %".333" = load i32, i32* %"PresentNum"
  %".334" = sub i32 %".333", 1
  %".335" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".334"
  %".336" = load i32, i32* %".335"
  %".337" = sub i32 %".332", %".336"
  %".338" = load i32, i32* %"PresentNum"
  %".339" = sub i32 %".338", 1
  %".340" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".339"
  store i32 %".337", i32* %".340"
  br label %".287"
.326:
  %".345" = load i8, i8* %"OperatorGet"
  %".346" = icmp eq i8 %".345", 42
  br i1 %".346", label %".343", label %".344"
.343:
  %".348" = load i32, i32* %"PresentNum"
  %".349" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".348"
  %".350" = load i32, i32* %".349"
  %".351" = load i32, i32* %"PresentNum"
  %".352" = sub i32 %".351", 1
  %".353" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".352"
  %".354" = load i32, i32* %".353"
  %".355" = mul i32 %".350", %".354"
  %".356" = load i32, i32* %"PresentNum"
  %".357" = sub i32 %".356", 1
  %".358" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".357"
  store i32 %".355", i32* %".358"
  br label %".287"
.344:
  br label %".287"
.375:
  %".379" = load i32, i32* %"i"
  %".380" = icmp sge i32 %".379", 0
  %".381" = load i32, i32* %"i"
  %".382" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".381"
  %".383" = load i8, i8* %".382"
  %".384" = icmp sle i8 %".383", 57
  %".385" = and i1 %".380", %".384"
  %".386" = load i32, i32* %"i"
  %".387" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".386"
  %".388" = load i8, i8* %".387"
  %".389" = icmp sge i8 %".388", 48
  %".390" = and i1 %".385", %".389"
  br i1 %".390", label %".376", label %".377"
.376:
  %".392" = load i32, i32* %"num"
  %".393" = load i32, i32* %"i"
  %".394" = getelementptr inbounds [2000 x i8], [2000 x i8]* %"InputExpression", i32 0, i32 %".393"
  %".395" = load i8, i8* %".394"
  %".396" = sub i8 %".395", 48
  %".397" = load i32, i32* %"DecimalProcess"
  %".398" = sext i8 %".396" to i32
  %".399" = mul i32 %".398", %".397"
  %".400" = add i32 %".392", %".399"
  store i32 %".400", i32* %"num"
  %".402" = load i32, i32* %"DecimalProcess"
  %".403" = mul i32 %".402", 10
  store i32 %".403", i32* %"DecimalProcess"
  %".405" = load i32, i32* %"i"
  %".406" = sub i32 %".405", 1
  store i32 %".406", i32* %"i"
  br label %".375"
.377:
  %".409" = load i32, i32* %"num"
  %".410" = load i32, i32* %"PresentNum"
  %".411" = add i32 %".410", 1
  %".412" = getelementptr inbounds [2000 x i32], [2000 x i32]* %"NumUpdated", i32 0, i32 %".411"
  store i32 %".409", i32* %".412"
  %".414" = load i32, i32* %"PresentNum"
  %".415" = add i32 %".414", 1
  store i32 %".415", i32* %"PresentNum"
  br label %".56"
}

declare i32 @"printf"(i8* %".1", ...) 

@".str0" = constant [39 x i8] c"please input an calculate expression: \00"
declare i32 @"gets"(...) 

declare i32 @"strlen"(i8* %".1") 

@".str1" = constant [12 x i8] c"result: %d\0a\00"