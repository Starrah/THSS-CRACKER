# Generated from simpleC.g4 by ANTLR 4.7.2
from antlr4 import *
if __name__ is not None and "." in __name__:
    from .simpleCParser import simpleCParser
else:
    from simpleCParser import simpleCParser

# This class defines a complete generic visitor for a parse tree produced by simpleCParser.

class simpleCVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by simpleCParser#prog.
    def visitProg(self, ctx:simpleCParser.ProgContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#include.
    def visitInclude(self, ctx:simpleCParser.IncludeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mStructDef.
    def visitMStructDef(self, ctx:simpleCParser.MStructDefContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#structParam.
    def visitStructParam(self, ctx:simpleCParser.StructParamContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mFunction.
    def visitMFunction(self, ctx:simpleCParser.MFunctionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#params.
    def visitParams(self, ctx:simpleCParser.ParamsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#param.
    def visitParam(self, ctx:simpleCParser.ParamContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#funcBody.
    def visitFuncBody(self, ctx:simpleCParser.FuncBodyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#body.
    def visitBody(self, ctx:simpleCParser.BodyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#block.
    def visitBlock(self, ctx:simpleCParser.BlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#initialBlock.
    def visitInitialBlock(self, ctx:simpleCParser.InitialBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#arrayInitBlock.
    def visitArrayInitBlock(self, ctx:simpleCParser.ArrayInitBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#structInitBlock.
    def visitStructInitBlock(self, ctx:simpleCParser.StructInitBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#assignBlock.
    def visitAssignBlock(self, ctx:simpleCParser.AssignBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#ifBlocks.
    def visitIfBlocks(self, ctx:simpleCParser.IfBlocksContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#ifBlock.
    def visitIfBlock(self, ctx:simpleCParser.IfBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#elifBlock.
    def visitElifBlock(self, ctx:simpleCParser.ElifBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#elseBlock.
    def visitElseBlock(self, ctx:simpleCParser.ElseBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#condition.
    def visitCondition(self, ctx:simpleCParser.ConditionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#whileBlock.
    def visitWhileBlock(self, ctx:simpleCParser.WhileBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#forBlock.
    def visitForBlock(self, ctx:simpleCParser.ForBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#for1Block.
    def visitFor1Block(self, ctx:simpleCParser.For1BlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#for3Block.
    def visitFor3Block(self, ctx:simpleCParser.For3BlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#returnBlock.
    def visitReturnBlock(self, ctx:simpleCParser.ReturnBlockContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#identifier.
    def visitIdentifier(self, ctx:simpleCParser.IdentifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#parens.
    def visitParens(self, ctx:simpleCParser.ParensContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#OR.
    def visitOR(self, ctx:simpleCParser.ORContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#string.
    def visitString(self, ctx:simpleCParser.StringContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#MulDiv.
    def visitMulDiv(self, ctx:simpleCParser.MulDivContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#AddSub.
    def visitAddSub(self, ctx:simpleCParser.AddSubContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#double.
    def visitDouble(self, ctx:simpleCParser.DoubleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#int.
    def visitInt(self, ctx:simpleCParser.IntContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#Neg.
    def visitNeg(self, ctx:simpleCParser.NegContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#arrayitem.
    def visitArrayitem(self, ctx:simpleCParser.ArrayitemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#function.
    def visitFunction(self, ctx:simpleCParser.FunctionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#AND.
    def visitAND(self, ctx:simpleCParser.ANDContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#char.
    def visitChar(self, ctx:simpleCParser.CharContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#structmember.
    def visitStructmember(self, ctx:simpleCParser.StructmemberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#Judge.
    def visitJudge(self, ctx:simpleCParser.JudgeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mType.
    def visitMType(self, ctx:simpleCParser.MTypeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mArray.
    def visitMArray(self, ctx:simpleCParser.MArrayContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mVoid.
    def visitMVoid(self, ctx:simpleCParser.MVoidContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mStruct.
    def visitMStruct(self, ctx:simpleCParser.MStructContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#structMember.
    def visitStructMember(self, ctx:simpleCParser.StructMemberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#arrayItem.
    def visitArrayItem(self, ctx:simpleCParser.ArrayItemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#func.
    def visitFunc(self, ctx:simpleCParser.FuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#strlenFunc.
    def visitStrlenFunc(self, ctx:simpleCParser.StrlenFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#atoiFunc.
    def visitAtoiFunc(self, ctx:simpleCParser.AtoiFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#printfFunc.
    def visitPrintfFunc(self, ctx:simpleCParser.PrintfFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#scanfFunc.
    def visitScanfFunc(self, ctx:simpleCParser.ScanfFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#getsFunc.
    def visitGetsFunc(self, ctx:simpleCParser.GetsFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#selfDefinedFunc.
    def visitSelfDefinedFunc(self, ctx:simpleCParser.SelfDefinedFuncContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#argument.
    def visitArgument(self, ctx:simpleCParser.ArgumentContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mID.
    def visitMID(self, ctx:simpleCParser.MIDContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mINT.
    def visitMINT(self, ctx:simpleCParser.MINTContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mDOUBLE.
    def visitMDOUBLE(self, ctx:simpleCParser.MDOUBLEContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mCHAR.
    def visitMCHAR(self, ctx:simpleCParser.MCHARContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mSTRING.
    def visitMSTRING(self, ctx:simpleCParser.MSTRINGContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by simpleCParser#mLIB.
    def visitMLIB(self, ctx:simpleCParser.MLIBContext):
        return self.visitChildren(ctx)



del simpleCParser