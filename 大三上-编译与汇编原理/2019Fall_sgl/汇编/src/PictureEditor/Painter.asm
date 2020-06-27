;文件名：Painter.asm
;描述：各种绘图函数的定义

.386 
.model flat,stdcall 
option casemap:none

include Define.inc

public CurrentMode
public CurrentX
public CurrentY
public StartX
public StartY
public EndX
public EndY
public MouseStatus
public ShowString
public WhetherDrawPolygon
public CurrentPointNum
public CurrentPointListX
public CurrentPointListY
public CurrentPointList
public SameThreshold

.data
; 模式
CurrentMode DWORD IDM_MODE_DRAW

;画线和擦除
;坐标
CurrentX DWORD 0
CurrentY DWORD 0
StartX DWORD 0
StartY DWORD 0
EndX DWORD 0
EndY DWORD 0
;鼠标状态
MouseStatus DWORD 0

;画多边形等
;画多边形是否已经结束
WhetherDrawPolygon DWORD 0

;当前使用的点信息
CurrentPointNum DWORD 0
CurrentPointListX DWORD 100 DUP(?)
CurrentPointListY DWORD 100 DUP(?)

;实际画多边形的参数
CurrentPointList DWORD 200 DUP(?)
;判断两点是否接近的Threshold
SameThreshold DWORD 4

;显示的文字
ShowString BYTE 100 dup(?)


.code
;画笔绘制函数
IPaint PROC, hdc:HDC
	INVOKE MoveToEx, hdc, StartX, StartY, NULL
	INVOKE LineTo, hdc, EndX, EndY
	INVOKE MoveToEx, hdc, 0, 0, NULL
	ret
IPaint ENDP

;橡皮擦除函数
IErase PROC, hdc:HDC
	extern EraserRadius:DWORD
	INVOKE GetStockObject, NULL_PEN
	INVOKE SelectObject, hdc, eax
	mov ecx, EraserRadius
	sub CurrentX, ecx
	sub CurrentY, ecx
	mov ebx, CurrentX
	mov edx, CurrentY
	add ebx, ecx
	add ebx, ecx
	add edx, ecx
	add edx, ecx
	INVOKE Rectangle, hdc, CurrentX, CurrentY, ebx, edx
	add CurrentX, ecx
	add CurrentY, ecx
	ret
IErase ENDP

;文字输入函数
IText PROC, hdc:HDC,hWnd:HWND
	extern hInstance:HINSTANCE
	mov edx, CurrentX
	mov ecx, CurrentY
	push edx
	push ecx
	invoke DialogBoxParam, hInstance, IDD_DIALOG1 ,hWnd, OFFSET ICallTextDialog, 0
	invoke crt_strlen, OFFSET ShowString
	pop ecx
	pop edx
	INVOKE TextOutA, hdc, edx, ecx, ADDR ShowString, eax
	mov ShowString, 0
	ret
IText ENDP

;画线函数
IPaintLine PROC, hdc:HDC
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE LineTo, hdc, edx, ecx
	INVOKE MoveToEx, hdc, 0, 0, NULL
	mov CurrentPointNum, 0
	ret
IPaintLine ENDP

;画矩形框函数
IPaintRectangleFrame PROC hdc:HDC
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE LineTo, hdc, edx, ecx
	INVOKE MoveToEx, hdc, 0, 0, NULL
	mov CurrentPointNum, 0
	ret
IPaintRectangleFrame ENDP

;画直角三角形(上方)框函数
IPaintTriangle0Frame PROC hdc:HDC
	LOCAL MiddleX:DWORD, MiddleY:DWORD
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov esi, DWORD PTR [CurrentPointListX + 4]
	mov edi, DWORD PTR [CurrentPointListY + 4]
	.IF ecx < edi
		mov MiddleY, ecx
		mov MiddleX, esi
	.ELSE
		mov MiddleY, edi
		mov MiddleX, edx
	.ENDIF
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, MiddleX
	mov ecx, MiddleY
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, MiddleX
	mov ecx, MiddleY
	INVOKE LineTo, hdc, edx, ecx
	INVOKE MoveToEx, hdc, 0, 0, NULL
	mov CurrentPointNum, 0
	ret
IPaintTriangle0Frame ENDP

;画直角三角形(下方)框函数
IPaintTriangle1Frame PROC hdc:HDC
LOCAL MiddleX:DWORD, MiddleY:DWORD
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov esi, DWORD PTR [CurrentPointListX + 4]
	mov edi, DWORD PTR [CurrentPointListY + 4]
	.IF ecx > edi
		mov MiddleY, ecx
		mov MiddleX, esi
	.ELSE
		mov MiddleY, edi
		mov MiddleX, edx
	.ENDIF
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, MiddleX
	mov ecx, MiddleY
	INVOKE LineTo, hdc, edx, ecx
	mov edx, DWORD PTR [CurrentPointListX + 4]
	mov ecx, DWORD PTR [CurrentPointListY + 4]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	mov edx, MiddleX
	mov ecx, MiddleY
	INVOKE LineTo, hdc, edx, ecx
	INVOKE MoveToEx, hdc, 0, 0, NULL
	mov CurrentPointNum, 0
	ret
IPaintTriangle1Frame ENDP

;画多边形框函数
IPaintPolygonFrame PROC, hdc:HDC
	.IF WhetherDrawPolygon == 0
		INVOKE IIncreasePolygonLine, hdc
	.ELSEIF WhetherDrawPolygon == 1
		INVOKE IIncreasePolygonLastLine, hdc
		mov CurrentPointNum, 0
	.ENDIF
	ret
IPaintPolygonFrame ENDP

;画矩形函数
IPaintRectangle PROC, hdc:HDC
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov ebx, DWORD PTR [CurrentPointListX + 4]
	mov eax, DWORD PTR [CurrentPointListY + 4]
	INVOKE Rectangle, hdc, edx, ecx, ebx, eax
	mov CurrentPointNum, 0
	ret
IPaintRectangle ENDP

;画直角三角形(上方)函数
IPaintTriangle0 PROC hdc:HDC
	LOCAL MiddleX:DWORD, MiddleY:DWORD
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov esi, DWORD PTR [CurrentPointListX + 4]
	mov edi, DWORD PTR [CurrentPointListY + 4]
	.IF ecx < edi
		mov MiddleY, ecx
		mov MiddleX, esi
	.ELSE
		mov MiddleY, edi
		mov MiddleX, edx
	.ENDIF
	mov ebx, OFFSET CurrentPointList
	mov [ebx], edx
	add ebx, 4
	mov [ebx], ecx
	add ebx, 4
	mov [ebx], esi
	add ebx, 4
	mov [ebx], edi
	add ebx, 4
	mov eax, MiddleX
	mov [ebx], eax
	add ebx, 4
	mov eax, MiddleY
	mov [ebx], eax
	INVOKE Polygon, hdc, ADDR CurrentPointList, 3
	mov CurrentPointNum, 0
	ret
IPaintTriangle0 ENDP

;画直角三角形(下方)函数
IPaintTriangle1 PROC hdc:HDC
	LOCAL MiddleX:DWORD, MiddleY:DWORD
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov esi, DWORD PTR [CurrentPointListX + 4]
	mov edi, DWORD PTR [CurrentPointListY + 4]
	.IF ecx > edi
		mov MiddleY, ecx
		mov MiddleX, esi
	.ELSE
		mov MiddleY, edi
		mov MiddleX, edx
	.ENDIF
	mov ebx, OFFSET CurrentPointList
	mov [ebx], edx
	add ebx, 4
	mov [ebx], ecx
	add ebx, 4
	mov [ebx], esi
	add ebx, 4
	mov [ebx], edi
	add ebx, 4
	mov eax, MiddleX
	mov [ebx], eax
	add ebx, 4
	mov eax, MiddleY
	mov [ebx], eax
	INVOKE Polygon, hdc, ADDR CurrentPointList, 3
	mov CurrentPointNum, 0
	ret
IPaintTriangle1 ENDP

;画椭圆函数
IPaintEllipse PROC, hdc:HDC
	mov edx, DWORD PTR [CurrentPointListX]
	mov ecx, DWORD PTR [CurrentPointListY]
	mov ebx, DWORD PTR [CurrentPointListX + 4]
	mov eax, DWORD PTR [CurrentPointListY + 4]
	INVOKE Ellipse, hdc, edx, ecx, ebx, eax
	mov CurrentPointNum, 0
	ret
IPaintEllipse ENDP

;画多边形函数
IPaintPolygon PROC, hdc:HDC
	.IF WhetherDrawPolygon == 0
		INVOKE IIncreasePolygonLine, hdc
	.ELSEIF WhetherDrawPolygon == 1
		INVOKE IIncreasePolygonLastLine, hdc
		INVOKE IGetPolygonPointList
		INVOKE Polygon, hdc, ADDR CurrentPointList, CurrentPointNum
		mov CurrentPointNum, 0
	.ENDIF
	ret
IPaintPolygon ENDP

;在多边形绘制时增加一条线
IIncreasePolygonLine PROC, hdc:HDC
	push esi
	mov esi, OFFSET CurrentPointListX
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	sub esi, 4
	mov ebx, DWORD PTR [esi]
	sub esi, 4
	mov edx, DWORD PTR [esi]
	mov esi, OFFSET CurrentPointListY
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	sub esi, 4
	mov edi, DWORD PTR [esi]
	sub esi, 4
	mov ecx, DWORD PTR [esi]
	INVOKE MoveToEx, hdc, edx, ecx, NULL
	INVOKE LineTo, hdc, ebx, edi
	INVOKE MoveToEx, hdc, 0, 0, NULL
	ret
IIncreasePolygonLine ENDP

;在多边形绘制时增加最后一条线，连接开始和结束
IIncreasePolygonLastLine PROC, hdc:HDC
	push esi
	mov esi, OFFSET CurrentPointListX
	mov edx, DWORD PTR [esi]
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	sub esi, 4
	mov ebx, DWORD PTR [esi]

	mov esi, OFFSET CurrentPointListY
	mov ecx, DWORD PTR [esi]
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	add esi, CurrentPointNum
	sub esi, 4
	mov edi, DWORD PTR [esi]

	INVOKE MoveToEx, hdc, edx, ecx, NULL
	INVOKE LineTo, hdc, ebx, edi
	INVOKE MoveToEx, hdc, 0, 0, NULL
	ret
IIncreasePolygonLastLine ENDP 

;获得当前点信息
IGetCurrentPoint PROC, Place:DWORD
	push ebx
	push edx
	mov ebx, Place
	mov edx, 0
	mov dx, bx
	sar ebx, 16
	mov CurrentX, edx
	mov CurrentY, ebx
	pop edx
	pop ebx
	ret
IGetCurrentPoint ENDP

;判断多边形是否绘制完毕--已经有三个点以上，最后一个点和第一个点接近重合.
;接近：eax = 1 否则eax = 0
IJudgePolygonEnd PROC
	push edx
	push ebx
	push ecx
	push esi
	.IF CurrentPointNum < 3
		mov eax, 0
	jmp EndJudge
	.ENDIF
	mov edx, OFFSET CurrentPointListX
	mov ecx, [edx]
	.IF ecx > CurrentX
		mov esi, ecx
		sub esi, CurrentX
	.ELSE
		mov esi, CurrentX
		sub esi, ecx
	.ENDIF
	.IF esi > SameThreshold
		mov eax, 0
		jmp EndJudge
	.ENDIF
	mov edx, OFFSET CurrentPointListY
	mov ecx, [edx]
	.IF ecx > CurrentY
		mov esi, ecx
		sub esi, CurrentY
	.ELSE
		mov esi, CurrentY
		sub esi, ecx
	.ENDIF
	.IF esi > SameThreshold
		mov eax, 0
		jmp EndJudge
	.ENDIF
	mov eax, 1
EndJudge:
	mov WhetherDrawPolygon, eax
	pop esi
	pop ecx
	pop ebx
	pop edx
	ret
IJudgePolygonEnd ENDP

;将当前点存储进点列
IAddGraphPoint PROC
	LOCAL PointerX:DWORD
	LOCAL PointerY:DWORD
	push edx
	push ebx
	mov edx, OFFSET CurrentPointListX
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	mov PointerX, edx
	mov edx, OFFSET CurrentPointListY
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	add edx, CurrentPointNum
	mov PointerY, edx
	mov ebx, CurrentX
	mov edx, PointerX
	mov [edx], ebx
	mov ebx, CurrentY
	mov edx, PointerY
	mov [edx], ebx
	inc CurrentPointNum
	pop ebx
	pop edx
	ret
IAddGraphPoint ENDP

;根据点列得到绘制多边形的存储点列
;将x，y两个数组转化为连续x，y的数组
IGetPolygonPointList PROC
	push esi
	push edx
	push ecx
	push ebx
	push edi
	mov esi, 0
	.WHILE esi < CurrentPointNum
		mov edx, OFFSET CurrentPointListX
		add edx, esi
		add edx, esi
		add edx, esi
		add edx, esi

		mov ecx, OFFSET CurrentPointListY
		add ecx, esi
		add ecx, esi
		add ecx, esi
		add ecx, esi
		
		mov ebx, OFFSET CurrentPointList
		add ebx, esi
		add ebx, esi
		add ebx, esi
		add ebx, esi
		add ebx, esi
		add ebx, esi
		add ebx, esi
		add ebx, esi

		mov edi, [edx]
		mov [ebx], edi
		add ebx, 4
		mov edi, [ecx]
		mov [ebx], edi
		inc esi
	.ENDW
	pop edi
	pop ebx
	pop ecx
	pop edx
	pop esi
	ret
IGetPolygonPointList ENDP

;弹出对话框输入文字
ICallTextDialog PROC hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    mov ebx,uMsg
    .IF ebx == WM_COMMAND
        invoke IHandleTextDialog,hWnd,wParam,lParam
    .ELSE 
		;默认处理
        invoke DefWindowProc,hWnd,uMsg,wParam,lParam 
        ret 
    .ENDIF 
    xor eax,eax 
    ret
ICallTextDialog endp

;将对话框输入文字存储，用于绘制
IHandleTextDialog PROC hWnd:HWND,wParam:WPARAM,lParam:LPARAM
    mov ebx,wParam
    and ebx,0ffffh
    .IF ebx == IDOK
        invoke GetDlgItemText,hWnd,IDC_EDIT1,addr ShowString, MAX_LENGTH
        invoke EndDialog,hWnd,wParam
    .ELSEIF ebx == IDCANCEL
        invoke EndDialog,hWnd,wParam
        mov eax,TRUE
    .ENDIF
    ret
IHandleTextDialog ENDP

end

