;文件名：WindowsManager.asm
;描述：Win32相关函数定义，包括鼠标，光标，对话框，绘图事件的处理

.386 
.model flat,stdcall 
option casemap:none

include Define.inc

public PenWidth

.data
PenWidth DWORD 1		   ;If PenStyle != PS_SOLID, PenWidth <= 1

.code
;处理WM_COMMAND(菜单点击)导致的模式变化
IHandleModeChange PROC USES ebx ecx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM
	LOCAL hdc: HDC
	LOCAL hdcBM: HDC
	LOCAL hbm: HBITMAP
	LOCAL bm: BITMAP
	LOCAL bmInfo: BITMAPINFOHEADER;位图文件信息头
	LOCAL bmFile: BITMAPFILEHEADER;位图文件头
	LOCAL numColor: DWORD;调色盘颜色种类
	LOCAL rgbSize:DWORD;调色板大小
	LOCAL bitmapSize:DWORD;位图大小
	LOCAL hg:HGLOBAL;分配空间句柄
	LOCAL spacePtr:DWORD;储存空间的首地址指针
	LOCAL numByte:DWORD;写入字节数
	LOCAL fileHandle:DWORD;文件句柄
	push ebx
	push ecx
	extern CurrentMode:DWORD
	extern CurrentPointNum:DWORD

	mov ebx, wParam
	mov CurrentPointNum, 0
	mov ecx, CurrentMode
	.IF bx == IDM_DRAW  ; 画图
		mov ecx, IDM_MODE_DRAW
	.ELSEIF bx == IDM_ERASE ; 擦除
		mov ecx, IDM_MODE_ERASE
	.ELSEIF bx == IDM_TEXT ;文字
		mov ecx, IDM_MODE_TEXT
	.ELSEIF bx == IDM_SOLID_LINE || bx == IDM_DASH_LINE || bx == IDM_DOT_LINE || bx == IDM_DASHDOT_LINE || bx == IDM_DASHDOT2_LINE || bx == IDM_INSIDEFRAME_LINE;画线
		.IF bx == IDM_SOLID_LINE
			mov ecx, IDM_MODE_LINE
			push PS_SOLID
		.ELSEIF bx == IDM_DASH_LINE ;画线
			mov ecx, IDM_MODE_LINE
			push PS_DASH
		.ELSEIF bx == IDM_DOT_LINE ;画线
			mov ecx, IDM_MODE_LINE
			push PS_DOT
		.ELSEIF bx == IDM_DASHDOT_LINE ;画线
			mov ecx, IDM_MODE_LINE
			push PS_DASHDOT
		.ELSEIF bx == IDM_DASHDOT2_LINE ;画线
			mov ecx, IDM_MODE_LINE
			push PS_DASHDOTDOT
		.ELSEIF bx == IDM_INSIDEFRAME_LINE ;画线
			mov ecx, IDM_MODE_LINE
			push PS_INSIDEFRAME
		.ENDIF
		pop PenStyle
	.ELSEIF bx == IDM_TRIANGLE0_FRAME ;上三角形边框
		mov ecx, IDM_MODE_TRIANGLE0_FRAME
	.ELSEIF bx == IDM_TRIANGLE1_FRAME ;下三角形边框
		mov ecx, IDM_MODE_TRIANGLE1_FRAME
	.ELSEIF bx == IDM_RECTANGLE_FRAME ;矩形边框
		mov ecx, IDM_MODE_RECTANGLE_FRAME
	.ELSEIF bx == IDM_POLYGON_FRAME ;多边形边框
		mov ecx, IDM_MODE_POLYGON_FRAME
	.ELSEIF bx == IDM_TRIANGLE0 ;上三角形
		mov ecx, IDM_MODE_TRIANGLE0
	.ELSEIF bx == IDM_TRIANGLE1 ;下三角形
		mov ecx, IDM_MODE_TRIANGLE1
	.ELSEIF bx == IDM_RECTANGLE ;矩形
		mov ecx, IDM_MODE_RECTANGLE
	.ELSEIF bx == IDM_ELLIPSE ;椭圆
		mov ecx, IDM_MODE_ELLIPSE
	.ELSEIF bx == IDM_POLYGON ;多边形
		mov ecx, IDM_MODE_POLYGON
	.ELSEIF bx == IDM_BRUSH_COLOR
		INVOKE IHandleColor, hWnd, 0
	.ELSEIF bx == IDM_PEN_COLOR
		INVOKE IHandleColor, hWnd, 1
	.ELSEIF bx == IDM_FONT
		INVOKE IHandleFont, hWnd
	.ELSEIF bx == IDM_SOLID_BRUSH
		push SOLID_BRUSH
		pop BrushMode
	.ELSEIF bx == IDM_BDIAG_BRUSH || bx == IDM_FDIAG_BRUSH || bx == IDM_DCROSS_BRUSH || bx == IDM_CROSS_BRUSH || bx == IDM_HORIZ_BRUSH || bx == IDM_VERTI_BRUSH
		push HATCH_BRUSH
		pop BrushMode
		.IF bx == IDM_BDIAG_BRUSH
			push HS_BDIAGONAL
		.ELSEIF bx == IDM_FDIAG_BRUSH
			push HS_FDIAGONAL
		.ELSEIF bx == IDM_DCROSS_BRUSH
			push HS_DIAGCROSS
		.ELSEIF bx == IDM_CROSS_BRUSH
			push HS_CROSS
		.ELSEIF bx == IDM_HORIZ_BRUSH
			push HS_HORIZONTAL
		.ELSEIF bx == IDM_VERTI_BRUSH
			push HS_VERTICAL
		.ENDIF
		pop HatchStyle
	.ELSEIF bx == IDM_LINE_SIZE
		INVOKE IHandlePainterSize, hWnd
	.ELSEIF bx == IDM_ERASER_SIZE
		INVOKE IHandleEraserSize, hWnd

	.ELSEIF bx == IDM_LOAD
		push edx
		mov rsFile.hwndOwner, NULL
		mov rsFile.nFilterIndex, 1
		mov rsFile.lpstrFileTitle, NULL
		mov rsFile.nMaxFileTitle, 0
		mov rsFile.lpstrInitialDir, NULL ;
		mov rsFile.Flags,  OFN_PATHMUSTEXIST AND OFN_FILEMUSTEXIST

		mov edx, sizeof rsFile
		mov rsFile.lStructSize, edx
		mov rsFile.lpstrFile, OFFSET rsFileName;			
		mov edx, sizeof rsFileName
		mov rsFile.nMaxFile, edx
		mov rsFile.lpstrFilter, OFFSET fileType2
		pop edx
		INVOKE GetOpenFileName, ADDR rsFile
		INVOKE GetDC, hWnd
		mov hdc, eax
		invoke CreateCompatibleDC, hdc
		mov hdcBM, eax
		invoke LoadImage, NULL, ADDR rsFileName, IMAGE_BITMAP, ScreenWidth, ScreenLength, LR_LOADFROMFILE
		mov hbm, eax
		invoke SelectObject, hdcBM, eax
		invoke BitBlt, hdc, 0, 0,  ScreenWidth, ScreenLength, hdcBM, 0, 0, SRCCOPY
		invoke DeleteObject, hbm
		invoke DeleteDC, hdcBM

	.ELSEIF bx == IDM_SAVE
		pushad
		mov edx, sizeof rsFile
		mov rsFile.lStructSize, edx ;文件大小
		mov rsFile.lpstrFile, OFFSET rsFileName ;文件名,包含驱动器和路径
		mov rsFile.lpstrFileTitle, OFFSET rsTitleName ;仅包含文件名和扩展名
		mov edx, sizeof rsFileName
		mov rsFile.nMaxFile, edx;文件名最大长度
		mov rsFile.lpstrFilter, OFFSET fileType ;过滤器（扩展名）
		mov rsFile.lpstrDefExt, OFFSET extenName ;默认扩展名
		mov rsFile.Flags, OFN_OVERWRITEPROMPT
		popad
		
		pushad
		INVOKE GetSaveFileName, ADDR rsFile;获取文件路径和信息，文件可以不存在
		INVOKE GetDC, hWnd ;获取当前窗口句柄
		mov hdc, eax

		INVOKE CreateCompatibleBitmap, hdc, ScreenWidth, ScreenLength;创建位图
		mov hbm, eax
		INVOKE CreateCompatibleDC, hdc;创建上下文环境
		mov hdcBM, eax

		INVOKE SelectObject, hdcBM, hbm ;将位图移到该上下文环境中
		INVOKE BitBlt, hdcBM, 0, 0, ScreenWidth, ScreenLength, hdc, 0, 0, SRCCOPY;将原窗口环境中的像素块整体拷贝到新建立的环境中
	
		INVOKE GetObject, hbm, (sizeof BITMAP), ADDR bm ;获取位图信息

		
		popad

		;位图信息头的初始化
		pushad
		mov bmInfo.biSize, (sizeof bmInfo);结构大小
		mov edx, bm.bmWidth
		mov bmInfo.biWidth, edx;位图文件的宽度
		mov edx, bm.bmHeight
		mov bmInfo.biHeight, edx;位图文件的高度
		mov bmInfo.biPlanes, 1;位图层数，总为1
		mov dx, bm.bmBitsPixel
		mov bmInfo.biBitCount, dx;位图色深
		mov bmInfo.biCompression, BI_RGB;压缩方式
		mov bmInfo.biSizeImage, 0;位图大小
		mov bmInfo.biXPelsPerMeter, 0;水平分辨率
		mov bmInfo.biYPelsPerMeter, 0;垂直分辨率
		mov bmInfo.biClrUsed, 0 ;使用所有颜色
		mov bmInfo.biClrImportant, 0 ;所有颜色重要
		popad

		;位图文件由位图文件头、位图信息头、调色板和位图数据组成

		;计算位图数据大小
		pushad
		mov eax, bm.bmWidth
		mov edx, 0
		mov dx, bmInfo.biBitCount
		imul eax, edx
		;保证是四字节的整数倍
		mov ebx, 32
		mov edx, 0
		add eax, 31
		idiv ebx
		imul eax, bm.bmHeight
		imul eax, 4
		mov bitmapSize, eax
		popad

		;计算调色板大小
		;色深小于等于8位，调色板颜色数是2的色深次方，大于8位无调色板
		pushad
		.IF bm.bmBitsPixel > 8
			push edx
			mov edx, 0
			mov numColor, edx
			pop edx
		.ELSE
			pushad
			mov edx, 1
			mov ecx, 0
			mov cx, bm.bmBitsPixel
			shl edx, cl
			mov numColor, edx
			popad
		.ENDIF
		mov edx, numColor
		imul edx, sizeof RGBQUAD
		mov rgbSize, edx
		popad

		;存储空间分配与数据转移
		pushad

		mov edx, bitmapSize
		add edx, sizeof bmInfo
		add edx, rgbSize
		;分配edx大小的动态空间
		INVOKE GlobalAlloc, GHND, edx
		mov hg, eax
		;锁定动态空间，返回首地址
		INVOKE GlobalLock, hg
		mov spacePtr, eax

		mov eax, sizeof bmInfo
		lea ebx, bmInfo
		mov edx, spacePtr

	copydata:
		mov eax, [ebx]
		mov [edx], eax
		add ebx, TYPE DWORD
		add edx, TYPE DWORD
		sub eax, TYPE DWORD
		cmp eax, 0
		jne copydata

		popad
	
		pushad
		mov edx, spacePtr
		add edx, sizeof bmInfo
		add edx, rgbSize
		INVOKE GetDIBits, hdc, hbm, 0, bmInfo.biHeight, edx, spacePtr, DIB_RGB_COLORS
		popad

		;设置位图文件头的信息
		pushad
		mov bmFile.bfType, 4D42h
		mov edx, sizeof bmFile
		add edx, sizeof bmInfo
		add edx, rgbSize
		mov bmFile.bfOffBits, edx
		add edx, bitmapSize
		mov bmFile.bfSize, edx
		mov bmFile.bfReserved1, 0
		mov bmFile.bfReserved2, 0

		;建立保存文件
		INVOKE CreateFile,
			ADDR rsFileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		mov fileHandle, eax
		popad


		pushad
		
		;将内容写入文件
		INVOKE WriteFile, fileHandle, ADDR bmFile, sizeof bmFile, ADDR numByte, NULL
		popad
		pushad
		mov edx, bmFile.bfSize
		sub edx, sizeof bmFile
		INVOKE WriteFile, fileHandle, spacePtr, edx, ADDR numByte, NULL
		popad
		pushad
		INVOKE GlobalFree, hg
		INVOKE CloseHandle, fileHandle
		popad
	.ENDIF
	mov CurrentMode, ecx
	pop ecx
	pop ebx
	ret
IHandleModeChange ENDP 

;处理鼠标移动事件(擦除和写入)
IHandleMouseMove PROC USES ebx ecx edx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM
	push ebx
	push ecx
	push edx
	extern CurrentMode: DWORD
	extern MouseStatus: DWORD
	extern StartX: DWORD
	extern StartY: DWORD
	extern EndX: DWORD
	extern EndY: DWORD
	extern CurrentX: DWORD
	extern CurrentY: DWORD
	extern CurrentPointNum: DWORD
	extern WhetherDrawPolygon: DWORD
	;获取当前位置
	mov ebx, lParam
	INVOKE IGetCurrentPoint, ebx
	mov edx, CurrentX
	mov ebx, CurrentY
	mov ecx, CurrentMode
	.IF MouseStatus == 1
		.IF ecx == IDM_MODE_DRAW    ;画线
			;更新画线位置 
			.IF EndX == 0  ;第一次画线
				mov StartX, edx
			.ELSE
				mov eax, EndX
				mov StartX, eax
			.ENDIF
			
			.IF EndY == 0 ;第一次画线
				mov StartY, ebx
			.ELSE
				mov eax, EndY
				mov StartY, eax
			.ENDIF

			mov EndX, edx
			mov EndY, ebx
			INVOKE InvalidateRect, hWnd, ADDR WorkRegion,0
		.ELSEIF ecx == IDM_MODE_ERASE ;擦除
			INVOKE InvalidateRect, hWnd, ADDR WorkRegion, 0
		.ENDIF
	.ENDIF
	pop edx
	pop ecx
	pop ebx
	ret
IHandleMouseMove ENDP

;处理鼠标按下事件(擦除和写入)
IHandleButtonDown PROC USES ecx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM
	push ecx
	extern CurrentMode: DWORD
	extern MouseStatus:DWORD
	mov ecx, CurrentMode
	.IF ecx == IDM_MODE_DRAW || ecx == IDM_MODE_ERASE
		;画笔或者橡皮
		mov MouseStatus, 1
	.ENDIF
	pop ecx
	ret
IHandleButtonDown ENDP 


;处理鼠标收起来事件(擦除，写入，工具)
IHandleButtonUp PROC USES ebx ecx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM
    local hdc:HDC
    local tempDC:HDC
    local tempBitmap:HBITMAP
	extern hInstance:HINSTANCE
	push ebx
	push ecx
	extern CurrentMode: DWORD
	extern MouseStatus: DWORD
	extern StartX: DWORD
	extern StartY: DWORD
	extern EndX: DWORD
	extern EndY: DWORD
	extern CurrentPointNum: DWORD
 	mov ecx, CurrentMode
	.IF ecx == IDM_MODE_DRAW || ecx == IDM_MODE_ERASE
		;画笔或者橡皮
		mov MouseStatus, 0
		mov StartX, 0
		mov StartY, 0
		mov EndX, 0
		mov EndY, 0
	.ELSEIF ecx == IDM_MODE_TEXT
		;文字
		mov ebx, lParam
		INVOKE IGetCurrentPoint, ebx
		INVOKE InvalidateRect, hWnd, ADDR WorkRegion, 0

	.ELSEIF ecx == IDM_MODE_POLYGON || ecx == IDM_MODE_POLYGON_FRAME
		;多边形或者边框
		mov ebx, lParam
		INVOKE IGetCurrentPoint, ebx
		INVOKE IJudgePolygonEnd
		.IF WhetherDrawPolygon == 0
			INVOKE IAddGraphPoint
		.ENDIF
		.IF CurrentPointNum >= 2
			INVOKE InvalidateRect, hWnd, ADDR WorkRegion, 0
		.ENDIF
;这里添加不同的Mode的判断，从而调用Painter.asm中的函数
	
	.ELSE
		;画线，矩形，椭圆，三角形等多种情况
		mov ebx, lParam
		INVOKE IGetCurrentPoint, ebx
		INVOKE IAddGraphPoint
		.IF CurrentPointNum == 2
			INVOKE InvalidateRect, hWnd, ADDR WorkRegion, 0
		.ENDIF
	.ENDIF
	pop ecx
	pop ebx
	ret
IHandleButtonUp ENDP

;处理光标事件
IHandleCursor PROC USES eax ebx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM
	extern hInstance:HINSTANCE
	push eax
	push ebx
	mov eax,lParam
    and eax,0ffffh
    .IF eax!=HTCLIENT
        ret
    .ENDIF

    mov eax,CurrentMode
    .IF eax == IDM_MODE_ERASE
        mov ebx,IDC_ERASERCURSOR
    .ELSEIF eax == IDM_MODE_TEXT
        mov ebx,IDC_TEXTCURSOR
    .ELSE
        mov ebx,IDC_PAINTCURSOR
    .ENDIF
    invoke LoadCursor,hInstance,ebx
    invoke SetCursor,eax
	pop ebx
	pop eax
    ret
IHandleCursor ENDP

;处理绘图事件
IHandlePaint PROC USES ecx,
	hWnd:HWND,wParam:WPARAM,lParam:LPARAM,ps:PAINTSTRUCT		
	local hPen: HPEN
	local hBrush: HBRUSH
	extern CurrentMode:DWORD
	push ecx
	INVOKE BeginPaint, hWnd, ADDR ps
	mov ecx, CurrentMode
	.IF ecx == IDM_MODE_DRAW || ecx==IDM_MODE_LINE || ecx==IDM_MODE_RECTANGLE_FRAME || ecx==IDM_MODE_POLYGON_FRAME || ecx==IDM_MODE_TRIANGLE0_FRAME || ecx==IDM_MODE_TRIANGLE1_FRAME
		push ecx
		;mov edi, PenWidth
		INVOKE CreatePen, PenStyle, PenWidth, PenColor
		mov hPen, eax
		INVOKE SelectObject, ps.hdc, hPen
		pop ecx
		.IF ecx == IDM_MODE_DRAW
			INVOKE IPaint, ps.hdc
		.ELSEIF ecx == IDM_MODE_LINE
			INVOKE IPaintLine, ps.hdc
		.ELSEIF ecx == IDM_MODE_RECTANGLE_FRAME
			INVOKE IPaintRectangleFrame, ps.hdc
		.ELSEIF ecx == IDM_MODE_TRIANGLE0_FRAME
			INVOKE IPaintTriangle0Frame, ps.hdc
		.ELSEIF ecx == IDM_MODE_TRIANGLE1_FRAME
			INVOKE IPaintTriangle1Frame, ps.hdc
		.ELSEIF ecx == IDM_MODE_POLYGON_FRAME
			INVOKE IPaintPolygonFrame, ps.hdc
		.ENDIF
		INVOKE DeleteObject, hPen
	.ELSEIF ecx == IDM_MODE_RECTANGLE || ecx==IDM_MODE_POLYGON || ecx==IDM_MODE_TRIANGLE0 || ecx==IDM_MODE_TRIANGLE1 || ecx==IDM_MODE_ELLIPSE
		push ecx
		mov ecx, BrushMode
		.IF ecx == SOLID_BRUSH
			INVOKE CreateSolidBrush, BrushColor
		;.ELSE
		.ELSEIF ecx == HATCH_BRUSH
			INVOKE CreateHatchBrush, HatchStyle, BrushColor
		.ENDIF
		mov hBrush, eax
		INVOKE SelectObject, ps.hdc, hBrush
		INVOKE CreatePen, PenStyle, PenWidth, PenColor
		mov hPen, eax
		INVOKE SelectObject, ps.hdc, hPen
		pop ecx
		.IF ecx == IDM_MODE_RECTANGLE
			INVOKE IPaintRectangle, ps.hdc
		.ELSEIF ecx == IDM_MODE_TRIANGLE0
			INVOKE IPaintTriangle0, ps.hdc
		.ELSEIF ecx == IDM_MODE_TRIANGLE1
			INVOKE IPaintTriangle1, ps.hdc
		.ELSEIF ecx == IDM_MODE_ELLIPSE
			INVOKE IPaintEllipse, ps.hdc
		.ELSEIF ecx == IDM_MODE_POLYGON
			INVOKE IPaintPolygon, ps.hdc
		.ENDIF
		INVOKE DeleteObject, hBrush
		INVOKE DeleteObject, hPen
	.ELSEIF ecx == IDM_MODE_ERASE
		INVOKE IErase, ps.hdc
	.ELSEIF ecx == IDM_MODE_TEXT
		push ecx		
		INVOKE SelectObject,ps.hdc, CurrentFont
		pop ecx
		INVOKE IText, ps.hdc, hWnd
	.ENDIF
	INVOKE EndPaint, hWnd, ADDR ps
	pop ecx
	ret
IHandlePaint ENDP

IHandleColor PROC hWnd:HWND, Command:DWORD
	local cc:CHOOSECOLOR
	extern hInstance:HINSTANCE
	push eax
	push ecx
    mov cc.lStructSize,sizeof cc
    mov eax,hWnd
    mov cc.hwndOwner,eax
    mov eax,hInstance
    mov cc.hInstance,eax
    mov cc.rgbResult,0
    mov eax,offset ArrayCustom_Color
    mov cc.lpCustColors,eax
    mov cc.Flags,CC_FULLOPEN or CC_RGBINIT
    mov cc.lCustData,0
    mov cc.lpfnHook,0
    mov cc.lpTemplateName,0
    INVOKE ChooseColor,addr cc
    mov eax,cc.rgbResult
    .IF Command == 0
        mov BrushColor, eax
    .ELSEIF Command == 1
        mov PenColor, eax
    .ENDIF
	pop ecx
	pop eax
    ret
IHandleColor ENDP


IHandleFont PROC hWnd:HWND
    local cc:CHOOSEFONT
    extern hInstance:HINSTANCE

    mov cc.lStructSize,sizeof cc
    mov eax,hWnd
    mov cc.hwndOwner,eax
    mov cc.hDC, 0
    push offset LogicFont
    pop cc.lpLogFont
    mov cc.Flags, 0
    mov cc.rgbColors, 0
    mov cc.lCustData, 0
    mov cc.lpfnHook, 0
    mov cc.lpTemplateName, 0
    mov eax,hInstance
    mov cc.hInstance,eax
    mov cc.lpszStyle, 0
    mov cc.nFontType, 0
    mov cc.nSizeMin, 0
    mov cc.nSizeMax, 0
    
    invoke ChooseFont,addr cc
    invoke CreateFontIndirect, offset LogicFont
    mov CurrentFont, eax
    ret
IHandleFont endp

end