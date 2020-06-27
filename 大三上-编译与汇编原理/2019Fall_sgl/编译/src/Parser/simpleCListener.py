# Generated from simpleC.g4 by ANTLR 4.7.2
from antlr4 import *
if __name__ is not None and "." in __name__:
    from .simpleCParser import simpleCParser
else:
    from simpleCParser import simpleCParser

# This class defines a complete listener for a parse tree produced by simpleCParser.
class simpleCListener(ParseTreeListener):

    # Enter a parse tree produced by simpleCParser#prog.
    def enterProg(self, ctx:simpleCParser.ProgContext):
        pass

    # Exit a parse tree produced by simpleCParser#prog.
    def exitProg(self, ctx:simpleCParser.ProgContext):
        pass


    # Enter a parse tree produced by simpleCParser#include.
    def enterInclude(self, ctx:simpleCParser.IncludeContext):
        pass

    # Exit a parse tree produced by simpleCParser#include.
    def exitInclude(self, ctx:simpleCParser.IncludeContext):
        pass


    # Enter a parse tree produced by simpleCParser#mStructDef.
    def enterMStructDef(self, ctx:simpleCParser.MStructDefContext):
        pass

    # Exit a parse tree produced by simpleCParser#mStructDef.
    def exitMStructDef(self, ctx:simpleCParser.MStructDefContext):
        pass


    # Enter a parse tree produced by simpleCParser#structParam.
    def enterStructParam(self, ctx:simpleCParser.StructParamContext):
        pass

    # Exit a parse tree produced by simpleCParser#structParam.
    def exitStructParam(self, ctx:simpleCParser.StructParamContext):
        pass


    # Enter a parse tree produced by simpleCParser#mFunction.
    def enterMFunction(self, ctx:simpleCParser.MFunctionContext):
        pass

    # Exit a parse tree produced by simpleCParser#mFunction.
    def exitMFunction(self, ctx:simpleCParser.MFunctionContext):
        pass


    # Enter a parse tree produced by simpleCParser#params.
    def enterParams(self, ctx:simpleCParser.ParamsContext):
        pass

    # Exit a parse tree produced by simpleCParser#params.
    def exitParams(self, ctx:simpleCParser.ParamsContext):
        pass


    # Enter a parse tree produced by simpleCParser#param.
    def enterParam(self, ctx:simpleCParser.ParamContext):
        pass

    # Exit a parse tree produced by simpleCParser#param.
    def exitParam(self, ctx:simpleCParser.ParamContext):
        pass


    # Enter a parse tree produced by simpleCParser#funcBody.
    def enterFuncBody(self, ctx:simpleCParser.FuncBodyContext):
        pass

    # Exit a parse tree produced by simpleCParser#funcBody.
    def exitFuncBody(self, ctx:simpleCParser.FuncBodyContext):
        pass


    # Enter a parse tree produced by simpleCParser#body.
    def enterBody(self, ctx:simpleCParser.BodyContext):
        pass

    # Exit a parse tree produced by simpleCParser#body.
    def exitBody(self, ctx:simpleCParser.BodyContext):
        pass


    # Enter a parse tree produced by simpleCParser#block.
    def enterBlock(self, ctx:simpleCParser.BlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#block.
    def exitBlock(self, ctx:simpleCParser.BlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#initialBlock.
    def enterInitialBlock(self, ctx:simpleCParser.InitialBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#initialBlock.
    def exitInitialBlock(self, ctx:simpleCParser.InitialBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#arrayInitBlock.
    def enterArrayInitBlock(self, ctx:simpleCParser.ArrayInitBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#arrayInitBlock.
    def exitArrayInitBlock(self, ctx:simpleCParser.ArrayInitBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#structInitBlock.
    def enterStructInitBlock(self, ctx:simpleCParser.StructInitBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#structInitBlock.
    def exitStructInitBlock(self, ctx:simpleCParser.StructInitBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#assignBlock.
    def enterAssignBlock(self, ctx:simpleCParser.AssignBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#assignBlock.
    def exitAssignBlock(self, ctx:simpleCParser.AssignBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#ifBlocks.
    def enterIfBlocks(self, ctx:simpleCParser.IfBlocksContext):
        pass

    # Exit a parse tree produced by simpleCParser#ifBlocks.
    def exitIfBlocks(self, ctx:simpleCParser.IfBlocksContext):
        pass


    # Enter a parse tree produced by simpleCParser#ifBlock.
    def enterIfBlock(self, ctx:simpleCParser.IfBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#ifBlock.
    def exitIfBlock(self, ctx:simpleCParser.IfBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#elifBlock.
    def enterElifBlock(self, ctx:simpleCParser.ElifBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#elifBlock.
    def exitElifBlock(self, ctx:simpleCParser.ElifBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#elseBlock.
    def enterElseBlock(self, ctx:simpleCParser.ElseBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#elseBlock.
    def exitElseBlock(self, ctx:simpleCParser.ElseBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#condition.
    def enterCondition(self, ctx:simpleCParser.ConditionContext):
        pass

    # Exit a parse tree produced by simpleCParser#condition.
    def exitCondition(self, ctx:simpleCParser.ConditionContext):
        pass


    # Enter a parse tree produced by simpleCParser#whileBlock.
    def enterWhileBlock(self, ctx:simpleCParser.WhileBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#whileBlock.
    def exitWhileBlock(self, ctx:simpleCParser.WhileBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#forBlock.
    def enterForBlock(self, ctx:simpleCParser.ForBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#forBlock.
    def exitForBlock(self, ctx:simpleCParser.ForBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#for1Block.
    def enterFor1Block(self, ctx:simpleCParser.For1BlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#for1Block.
    def exitFor1Block(self, ctx:simpleCParser.For1BlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#for3Block.
    def enterFor3Block(self, ctx:simpleCParser.For3BlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#for3Block.
    def exitFor3Block(self, ctx:simpleCParser.For3BlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#returnBlock.
    def enterReturnBlock(self, ctx:simpleCParser.ReturnBlockContext):
        pass

    # Exit a parse tree produced by simpleCParser#returnBlock.
    def exitReturnBlock(self, ctx:simpleCParser.ReturnBlockContext):
        pass


    # Enter a parse tree produced by simpleCParser#identifier.
    def enterIdentifier(self, ctx:simpleCParser.IdentifierContext):
        pass

    # Exit a parse tree produced by simpleCParser#identifier.
    def exitIdentifier(self, ctx:simpleCParser.IdentifierContext):
        pass


    # Enter a parse tree produced by simpleCParser#parens.
    def enterParens(self, ctx:simpleCParser.ParensContext):
        pass

    # Exit a parse tree produced by simpleCParser#parens.
    def exitParens(self, ctx:simpleCParser.ParensContext):
        pass


    # Enter a parse tree produced by simpleCParser#OR.
    def enterOR(self, ctx:simpleCParser.ORContext):
        pass

    # Exit a parse tree produced by simpleCParser#OR.
    def exitOR(self, ctx:simpleCParser.ORContext):
        pass


    # Enter a parse tree produced by simpleCParser#string.
    def enterString(self, ctx:simpleCParser.StringContext):
        pass

    # Exit a parse tree produced by simpleCParser#string.
    def exitString(self, ctx:simpleCParser.StringContext):
        pass


    # Enter a parse tree produced by simpleCParser#MulDiv.
    def enterMulDiv(self, ctx:simpleCParser.MulDivContext):
        pass

    # Exit a parse tree produced by simpleCParser#MulDiv.
    def exitMulDiv(self, ctx:simpleCParser.MulDivContext):
        pass


    # Enter a parse tree produced by simpleCParser#AddSub.
    def enterAddSub(self, ctx:simpleCParser.AddSubContext):
        pass

    # Exit a parse tree produced by simpleCParser#AddSub.
    def exitAddSub(self, ctx:simpleCParser.AddSubContext):
        pass


    # Enter a parse tree produced by simpleCParser#double.
    def enterDouble(self, ctx:simpleCParser.DoubleContext):
        pass

    # Exit a parse tree produced by simpleCParser#double.
    def exitDouble(self, ctx:simpleCParser.DoubleContext):
        pass


    # Enter a parse tree produced by simpleCParser#int.
    def enterInt(self, ctx:simpleCParser.IntContext):
        pass

    # Exit a parse tree produced by simpleCParser#int.
    def exitInt(self, ctx:simpleCParser.IntContext):
        pass


    # Enter a parse tree produced by simpleCParser#Neg.
    def enterNeg(self, ctx:simpleCParser.NegContext):
        pass

    # Exit a parse tree produced by simpleCParser#Neg.
    def exitNeg(self, ctx:simpleCParser.NegContext):
        pass


    # Enter a parse tree produced by simpleCParser#arrayitem.
    def enterArrayitem(self, ctx:simpleCParser.ArrayitemContext):
        pass

    # Exit a parse tree produced by simpleCParser#arrayitem.
    def exitArrayitem(self, ctx:simpleCParser.ArrayitemContext):
        pass


    # Enter a parse tree produced by simpleCParser#function.
    def enterFunction(self, ctx:simpleCParser.FunctionContext):
        pass

    # Exit a parse tree produced by simpleCParser#function.
    def exitFunction(self, ctx:simpleCParser.FunctionContext):
        pass


    # Enter a parse tree produced by simpleCParser#AND.
    def enterAND(self, ctx:simpleCParser.ANDContext):
        pass

    # Exit a parse tree produced by simpleCParser#AND.
    def exitAND(self, ctx:simpleCParser.ANDContext):
        pass


    # Enter a parse tree produced by simpleCParser#char.
    def enterChar(self, ctx:simpleCParser.CharContext):
        pass

    # Exit a parse tree produced by simpleCParser#char.
    def exitChar(self, ctx:simpleCParser.CharContext):
        pass


    # Enter a parse tree produced by simpleCParser#structmember.
    def enterStructmember(self, ctx:simpleCParser.StructmemberContext):
        pass

    # Exit a parse tree produced by simpleCParser#structmember.
    def exitStructmember(self, ctx:simpleCParser.StructmemberContext):
        pass


    # Enter a parse tree produced by simpleCParser#Judge.
    def enterJudge(self, ctx:simpleCParser.JudgeContext):
        pass

    # Exit a parse tree produced by simpleCParser#Judge.
    def exitJudge(self, ctx:simpleCParser.JudgeContext):
        pass


    # Enter a parse tree produced by simpleCParser#mType.
    def enterMType(self, ctx:simpleCParser.MTypeContext):
        pass

    # Exit a parse tree produced by simpleCParser#mType.
    def exitMType(self, ctx:simpleCParser.MTypeContext):
        pass


    # Enter a parse tree produced by simpleCParser#mArray.
    def enterMArray(self, ctx:simpleCParser.MArrayContext):
        pass

    # Exit a parse tree produced by simpleCParser#mArray.
    def exitMArray(self, ctx:simpleCParser.MArrayContext):
        pass


    # Enter a parse tree produced by simpleCParser#mVoid.
    def enterMVoid(self, ctx:simpleCParser.MVoidContext):
        pass

    # Exit a parse tree produced by simpleCParser#mVoid.
    def exitMVoid(self, ctx:simpleCParser.MVoidContext):
        pass


    # Enter a parse tree produced by simpleCParser#mStruct.
    def enterMStruct(self, ctx:simpleCParser.MStructContext):
        pass

    # Exit a parse tree produced by simpleCParser#mStruct.
    def exitMStruct(self, ctx:simpleCParser.MStructContext):
        pass


    # Enter a parse tree produced by simpleCParser#structMember.
    def enterStructMember(self, ctx:simpleCParser.StructMemberContext):
        pass

    # Exit a parse tree produced by simpleCParser#structMember.
    def exitStructMember(self, ctx:simpleCParser.StructMemberContext):
        pass


    # Enter a parse tree produced by simpleCParser#arrayItem.
    def enterArrayItem(self, ctx:simpleCParser.ArrayItemContext):
        pass

    # Exit a parse tree produced by simpleCParser#arrayItem.
    def exitArrayItem(self, ctx:simpleCParser.ArrayItemContext):
        pass


    # Enter a parse tree produced by simpleCParser#func.
    def enterFunc(self, ctx:simpleCParser.FuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#func.
    def exitFunc(self, ctx:simpleCParser.FuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#strlenFunc.
    def enterStrlenFunc(self, ctx:simpleCParser.StrlenFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#strlenFunc.
    def exitStrlenFunc(self, ctx:simpleCParser.StrlenFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#atoiFunc.
    def enterAtoiFunc(self, ctx:simpleCParser.AtoiFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#atoiFunc.
    def exitAtoiFunc(self, ctx:simpleCParser.AtoiFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#printfFunc.
    def enterPrintfFunc(self, ctx:simpleCParser.PrintfFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#printfFunc.
    def exitPrintfFunc(self, ctx:simpleCParser.PrintfFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#scanfFunc.
    def enterScanfFunc(self, ctx:simpleCParser.ScanfFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#scanfFunc.
    def exitScanfFunc(self, ctx:simpleCParser.ScanfFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#getsFunc.
    def enterGetsFunc(self, ctx:simpleCParser.GetsFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#getsFunc.
    def exitGetsFunc(self, ctx:simpleCParser.GetsFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#selfDefinedFunc.
    def enterSelfDefinedFunc(self, ctx:simpleCParser.SelfDefinedFuncContext):
        pass

    # Exit a parse tree produced by simpleCParser#selfDefinedFunc.
    def exitSelfDefinedFunc(self, ctx:simpleCParser.SelfDefinedFuncContext):
        pass


    # Enter a parse tree produced by simpleCParser#argument.
    def enterArgument(self, ctx:simpleCParser.ArgumentContext):
        pass

    # Exit a parse tree produced by simpleCParser#argument.
    def exitArgument(self, ctx:simpleCParser.ArgumentContext):
        pass


    # Enter a parse tree produced by simpleCParser#mID.
    def enterMID(self, ctx:simpleCParser.MIDContext):
        pass

    # Exit a parse tree produced by simpleCParser#mID.
    def exitMID(self, ctx:simpleCParser.MIDContext):
        pass


    # Enter a parse tree produced by simpleCParser#mINT.
    def enterMINT(self, ctx:simpleCParser.MINTContext):
        pass

    # Exit a parse tree produced by simpleCParser#mINT.
    def exitMINT(self, ctx:simpleCParser.MINTContext):
        pass


    # Enter a parse tree produced by simpleCParser#mDOUBLE.
    def enterMDOUBLE(self, ctx:simpleCParser.MDOUBLEContext):
        pass

    # Exit a parse tree produced by simpleCParser#mDOUBLE.
    def exitMDOUBLE(self, ctx:simpleCParser.MDOUBLEContext):
        pass


    # Enter a parse tree produced by simpleCParser#mCHAR.
    def enterMCHAR(self, ctx:simpleCParser.MCHARContext):
        pass

    # Exit a parse tree produced by simpleCParser#mCHAR.
    def exitMCHAR(self, ctx:simpleCParser.MCHARContext):
        pass


    # Enter a parse tree produced by simpleCParser#mSTRING.
    def enterMSTRING(self, ctx:simpleCParser.MSTRINGContext):
        pass

    # Exit a parse tree produced by simpleCParser#mSTRING.
    def exitMSTRING(self, ctx:simpleCParser.MSTRINGContext):
        pass


    # Enter a parse tree produced by simpleCParser#mLIB.
    def enterMLIB(self, ctx:simpleCParser.MLIBContext):
        pass

    # Exit a parse tree produced by simpleCParser#mLIB.
    def exitMLIB(self, ctx:simpleCParser.MLIBContext):
        pass


