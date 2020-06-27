#include <stdio.h>
#include <string.h>

int main()
{
    char InputExpression[2000];
    char OperationUpdated[2000];
    int NumUpdated[2000];
    int PresentNum = -1, PresentOperator = -1, DecimalProcess = 1, StrLength;
    int num = 0, i;
    printf("please input an calculate expression: ");
    gets(InputExpression);
    StrLength = strlen(InputExpression);
    for(i = StrLength - 1; i >= 0; i = i - 1) 
    {
        InputExpression[i + 1] = InputExpression[i];
    }
    i = StrLength + 1;
    InputExpression[0] = '(';
    InputExpression[StrLength + 1] = ')';
    StrLength = StrLength + 2;
    while(i >= 0){
        if(InputExpression[i] == '+')
        {
            while(PresentOperator >= 0 && ((OperationUpdated[PresentOperator] == '*') || (OperationUpdated[PresentOperator] == '/'))){
                if(OperationUpdated[PresentOperator] == '*') {
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] * NumUpdated[PresentNum - 1];
                }
                else {
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] / NumUpdated[PresentNum - 1];
                }
                PresentOperator = PresentOperator - 1;
                PresentNum = PresentNum - 1;
            }
            PresentOperator = PresentOperator + 1;
            OperationUpdated[PresentOperator] = '+';
            i = i - 1;
        }
        else if(InputExpression[i] == '-')
        {
            while(PresentOperator >= 0 && ((OperationUpdated[PresentOperator] == '*') || (OperationUpdated[PresentOperator] == '/'))){
                if(OperationUpdated[PresentOperator] == '*'){
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] * NumUpdated[PresentNum - 1];
                }
                else{
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] / NumUpdated[PresentNum - 1];
                }
                PresentNum = PresentNum - 1;
                PresentOperator = PresentOperator - 1;
            }
            PresentOperator = PresentOperator + 1;
            OperationUpdated[PresentOperator] = '-';
            i = i - 1;
        }
        else if(InputExpression[i] == '*')
        {
            PresentOperator = PresentOperator + 1;
            OperationUpdated[PresentOperator] = '*';
            i = i - 1;
        }
        else if(InputExpression[i] == '/')
        {
            PresentOperator = PresentOperator + 1;
            OperationUpdated[PresentOperator] = '/';
            i = i - 1;
        }
        else if(InputExpression[i] == ')'){
            PresentOperator = PresentOperator + 1;
            OperationUpdated[PresentOperator] = ')';
            i = i - 1;
        }
        else if(InputExpression[i] == '(')
        {
            while(OperationUpdated[PresentOperator] != ')')
            {
                char OperatorGet = OperationUpdated[PresentOperator];
                PresentOperator = PresentOperator - 1;
                if(OperatorGet == '/'){
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] / NumUpdated[PresentNum - 1];
                }
                else if(OperatorGet == '+'){
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] + NumUpdated[PresentNum - 1];
                }
                else if(OperatorGet == '-'){
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] - NumUpdated[PresentNum - 1];
                }
                else if(OperatorGet == '*'){
                    NumUpdated[PresentNum - 1] = NumUpdated[PresentNum] * NumUpdated[PresentNum - 1];
                }
                PresentNum = PresentNum - 1;
            }
            PresentOperator = PresentOperator - 1;
            i = i - 1;
        }
        else
        {
            num = 0;
            DecimalProcess = 1;
            while(i >= 0 && InputExpression[i] <= '9' && InputExpression[i] >= '0')
            {
                num = num + (InputExpression[i] - '0') * DecimalProcess;
                DecimalProcess = DecimalProcess * 10;
                i = i - 1;
            }
            NumUpdated[PresentNum + 1] = num;
            PresentNum = PresentNum + 1;
        }
    }
    printf("result: %d\n", NumUpdated[0]);
    return 0;
}
