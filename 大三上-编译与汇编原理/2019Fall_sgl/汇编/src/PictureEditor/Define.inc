;文件名：Define.inc
;描述：各种函数的PROTO定义，各种常量的定义

;引用各种头文件
include windows.inc 
include user32.inc 
include kernel32.inc 
include gdi32.inc
include msvcrt.inc
include comdlg32.inc
include comctl32.inc
include msvcrt.inc

includelib user32.lib 
includelib kernel32.lib 
includelib gdi32.lib
includelib msvcrt.lib
includelib comdlg32.lib
includelib comctl32.lib
includelib msvcrt.lib

;函数原形

;用于WndProc里，处理各种事件的函数，位于
IHandleModeChange			PROTO :HWND,:WPARAM,:LPARAM ;处理WM_COMMAND(菜单点击)导致的模式变化
IHandleMouseMove			PROTO :HWND,:WPARAM,:LPARAM ;处理鼠标移动事件(擦除和写入)
IHandleButtonDown			PROTO :HWND,:WPARAM,:LPARAM ;处理鼠标按下事件(擦除和写入)
IHandleButtonUp				PROTO :HWND,:WPARAM,:LPARAM	;处理鼠标收起来事件(擦除，写入，工具)
IHandleCursor				PROTO :HWND,:WPARAM,:LPARAM ;处理光标事件
IHandlePaint				PROTO :HWND,:WPARAM,:LPARAM,:PAINTSTRUCT ;处理绘图事件

;各种绘图函数
IPaint						PROTO :HDC
IErase						PROTO :HDC
IText						PROTO :HDC,:HWND
IPaintLine					PROTO :HDC
IPaintRectangleFrame		PROTO :HDC
IPaintTriangle0Frame		PROTO :HDC
IPaintTriangle1Frame		PROTO :HDC
IPaintPolygonFrame			PROTO :HDC
IPaintRectangle				PROTO :HDC
IPaintTriangle0				PROTO :HDC
IPaintTriangle1				PROTO :HDC
IPaintEllipse				PROTO :HDC
IPaintPolygon				PROTO :HDC

;绘图辅助函数
IGetCurrentPoint		PROTO :DWORD ;获得当前点
;判断多边形是否绘制完毕--已经有三个点以上，最后一个点和第一个点接近重合.
;接近：eax = 1 否则eax = 0
IJudgePolygonEnd			PROTO 
IAddGraphPoint				PROTO ;将当前点存储进点列
IGetPolygonPointList		PROTO ;根据点列得到绘制多边形的存储点列
IGetTriangle0PointList		PROTO ;根据点列得到绘制直角三角形(上方)的存储点列
IGetTriangle1PointList		PROTO ;根据点列得到绘制直角三角形(下方)的存储点列
IGetRectanglePointList		PROTO ;根据点列得到绘制矩形的存储点列
IIncreasePolygonLine		PROTO :HDC;在多边形绘制时增加一条线
IIncreasePolygonLastLine	PROTO, :HDC;在多边形绘制时增加最后一条线，连接开始和结束
ICallTextDialog				PROTO :HWND,:UINT,:WPARAM,:LPARAM ;弹出对话框输入文字
IHandleTextDialog			PROTO :HWND,:WPARAM,:LPARAM ;将对话框输入文字存储，用于绘制

;菜单，控制等函数
ICreateMenu					PROTO ;建立菜单

IHandleColor				PROTO :HWND, :DWORD
IHandleFont					PROTO :HWND
IHandleEraserSize			PROTO :HWND
ICallEraserDialog			PROTO :HWND, :UINT, :WPARAM, :LPARAM
IHandleEraserDialog			PROTO :HWND, :WPARAM, :LPARAM
IHandlePainterSize			PROTO :HWND
ICallPainterDialog			PROTO :HWND, :UINT, :WPARAM, :LPARAM
IHandlePainterDialog		PROTO :HWND, :WPARAM, :LPARAM

;定义各种常量
.data

	;屏幕和工作区信息
	ScreenLength			WORD 541
	ScreenWidth				WORD 784
	WorkRegion				RECT <0, 0, 800, 600>

	;字符上限
	MAX_LENGTH				WORD 100

	;资源文件
	IDD_DIALOG1				WORD 104
	IDD_DIALOG2				WORD 106
	IDD_DIALOG3				WORD 107
	IDC_EDIT1				WORD 1001
	IDC_EDIT2				WORD 1003
	IDC_EDIT3				WORD 1004
	IDC_TEXTCURSOR			DWORD 103
	IDC_ERASERCURSOR		DWORD 101
	IDC_PAINTCURSOR			DWORD 102


	;上方菜单栏各种编号
	IDM_FILE_MENU			WORD 301
	IDM_DRAW_MENU			WORD 302
	IDM_GRAPH_TOOL_MENU		WORD 303
	IDM_GRAPH_FRAME_MENU	WORD 304
	IDM_SETTINGS_MENU		WORD 305

	;菜单栏具体选项的编号, word
	IDM_DRAW				WORD 400
	IDM_ERASE				WORD 401
	IDM_TEXT				WORD 402
	IDM_LINE				WORD 403
	IDM_SOLID_LINE			WORD 4031
	IDM_DASH_LINE			WORD 4032
	IDM_DOT_LINE			WORD 4033
	IDM_DASHDOT_LINE		WORD 4034
	IDM_DASHDOT2_LINE		WORD 4035
	IDM_INSIDEFRAME_LINE	WORD 4036
	IDM_TRIANGLE0			WORD 4040
	IDM_TRIANGLE1			WORD 4041
	IDM_RECTANGLE			WORD 405
	IDM_ELLIPSE				WORD 406
	IDM_POLYGON				WORD 407
	IDM_RECTANGLE_FRAME		WORD 408
	IDM_TRIANGLE0_FRAME		WORD 4090
	IDM_TRIANGLE1_FRAME		WORD 4091
	IDM_POLYGON_FRAME		WORD 410
	;File
	IDM_SAVE				WORD 411
	IDM_LOAD				WORD 412
	;Color
	IDM_BRUSH_COLOR			WORD 413
	IDM_PEN_COLOR			WORD 414
	IDM_FONT				WORD 415

	IDM_BRUSH 				WORD 416
	IDM_SOLID_BRUSH			WORD 4160
	IDM_BDIAG_BRUSH			WORD 4161
	IDM_FDIAG_BRUSH			WORD 4162
	IDM_DCROSS_BRUSH		WORD 4163
	IDM_CROSS_BRUSH 		WORD 4164
	IDM_HORIZ_BRUSH			WORD 4165
	IDM_VERTI_BRUSH			WORD 4166

	;设置线宽和橡皮大小
	IDM_LINE_SIZE			WORD 417
 	IDM_ERASER_SIZE         WORD 418		

	;当前模式的编号
	IDM_MODE_DRAW				DWORD 500
	IDM_MODE_ERASE				DWORD 501
	IDM_MODE_TEXT				DWORD 502
	IDM_MODE_LINE				DWORD 503
	IDM_MODE_TRIANGLE0			DWORD 5040
	IDM_MODE_TRIANGLE1			DWORD 5041
	IDM_MODE_RECTANGLE			DWORD 505
	IDM_MODE_ELLIPSE			DWORD 506
	IDM_MODE_POLYGON			DWORD 507
	IDM_MODE_RECTANGLE_FRAME	DWORD 508
	IDM_MODE_TRIANGLE0_FRAME	DWORD 5090
	IDM_MODE_TRIANGLE1_FRAME	DWORD 5091
	IDM_MODE_POLYGON_FRAME		DWORD 510
	IDM_MODE_BACKGROUBD_COLOR	DWORD 511
	IDM_MODE_FRAME_COLOR		DWORD 512
	IDM_MODE_FONT				DWORD 513


	; 类名以及程序名字符串
	ClassName				db "DrawingWinClass", 0
	AppName					db "画图", 0

	; 菜单字符串
	FileMenuString			BYTE "文件", 0
		LoadMenuString			BYTE "打开", 0
		SaveMenuString			BYTE "保存", 0

	DrawMenuString			BYTE "绘图", 0
		PaintMenuString			BYTE "画图", 0
		EraseMenuString			BYTE "擦除", 0
		TextMenuString			BYTE "文字", 0
		LineSizeString			BYTE "画笔宽度", 0
		EraserSizeString		BYTE "橡皮擦大小", 0

	FrameMenuString			BYTE "框架绘制工具", 0
		LineMenuString			BYTE "直线", 0
		RectangleFrameMenuString BYTE "矩形框", 0
		Triangle0FrameMenuString BYTE "直角三角形框（上方）", 0
		Triangle1FrameMenuString BYTE "直角三角形框（下方）", 0
		PolygonFrameMenuString	BYTE "多边形框", 0
		BrushMenuString			BYTE "填充方式", 0

			SolidLineMenuString		BYTE "实线", 0
			DashLineMenuString		BYTE "段线", 0
			DotLineMenuString		BYTE "点线", 0
			DashDotLineMenuString	BYTE "线、点",0
			DashDot2LineMenuString	BYTE "线、点、点", 0
			InsideFrameLineMenuString BYTE "内线宽", 0
			SolidBrushMenuString	BYTE "实心", 0
			BDiagonalBrushMenuString  BYTE "右斜线", 0
			FDiagonalBrushMenuString  BYTE "左斜线", 0
			DiagCrossBrushMenuString  BYTE "斜交叉", 0
			CrossBrushMenuString	BYTE "交叉", 0
			HorizontalBrushMenuString BYTE "平行", 0
			VerticalBrushMenuString	  BYTE "竖直", 0


	ColorMenuString			BYTE "颜色", 0
		ColorBrushMenuString	BYTE "填充", 0
		ColorPenMenuString		BYTE "线条", 0
		
	
	FontMenuString			BYTE "字体", 0
		FontChooseMenuString	BYTE "选择字体", 0 

	ToolMenuString			BYTE "图形绘制工具", 0
		RectangleMenuString		BYTE "矩形", 0
		Triangle0MenuString		BYTE "直角三角形（上方）", 0
		Triangle1MenuString		BYTE "直角三角形（下方）", 0
		EllipseMenuString		BYTE "椭圆", 0
		PolygonMenuString		BYTE "多边形", 0

	SettingsMenuString		BYTE "帮助", 0

	PenColor 				DWORD 0ffh
	PenStyle				DWORD PS_SOLID ;PS_SOLID == 0
	BrushColor 				DWORD 0ffffffh ;Fill Rect with BrushColor
	BrushMode				DWORD 0
		SOLID_BRUSH 		DWORD 0
		HATCH_BRUSH			DWORD 1
		HatchStyle			DWORD HS_BDIAGONAL
	ArrayCustom_Color		DWORD 16 DUP(0)
	
	LogicFont				LOGFONT <> 
    CurrentFont				HFONT	0

	
	; 保存有关
	fileType byte "BMP(*.bmp)", 0, 0
	fileType2 byte "BMP(*.bmp)", 0 ,"*.bmp", 0, 0
	extenName byte "bmp", 0


	rsFileName	db	1000 DUP (?)
	rsTitleName	db	1000 DUP (?)
	rsFile OPENFILENAME <>



