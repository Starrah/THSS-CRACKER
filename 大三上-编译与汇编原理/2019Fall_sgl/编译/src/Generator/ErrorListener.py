from antlr4.error.ErrorListener import ErrorListener
from antlr4 import *

class SemanticError(Exception):
    """语义错误基类"""
    def __init__(self, msg, ctx=None):
        super().__init__()
        if ctx:
            self.line = ctx.start.line  #错误出现位置
            self.column = ctx.start.column
        else:
            self.line = 0
            self.column = 0
        self.msg = msg

    def __str__(self):
        return "SemanticError: " + str(self.line) + ":" + str(self.column) + " " + self.msg

# 语法错误检查与反馈
class syntaxErrorListener(ErrorListener):
    
    # 打印语法错误
    def syntaxError(self, recognizer, offendingSymbol, row, column, msg, e):
        exception = "(row:" + str(row) + ",column:" + str(column) + ") " + msg
        print('SyntaxError: ' + exception)
