;文件名：main.asm
;描述：主函数

.386 
.model flat,stdcall 
option casemap:none

include Define.inc

WinMain proto :DWORD, :DWORD, :DWORD, :DWORD

public hInstance
public hMenu


.data
hInstance HINSTANCE ?
CommandLine LPSTR ?
hMenu HMENU ?

.code
;初始函数
main:
	INVOKE GetModuleHandle, NULL
	mov hInstance, eax
	INVOKE GetCommandLine
	mov CommandLine, eax
	INVOKE WinMain, hInstance, NULL, CommandLine, SW_SHOWDEFAULT
	INVOKE ExitProcess, eax

;主窗口函数
WinMain PROC,
	hInst: HINSTANCE, hPrevInst: HINSTANCE, CmdLine: LPSTR, CmdShow: DWORD
	LOCAL wc: WNDCLASSEX
	LOCAL msg: MSG
	LOCAL hwnd: HWND
	INVOKE ICreateMenu
	mov wc.cbSize, SIZEOF WNDCLASSEX
	mov wc.style, CS_HREDRAW or CS_VREDRAW
	mov wc.lpfnWndProc, OFFSET WndProc
	mov wc.cbClsExtra, NULL
	mov wc.cbWndExtra, NULL
	push hInst
	pop wc.hInstance
	mov wc.hbrBackground, COLOR_WINDOW+1
	mov wc.lpszMenuName, NULL
	mov wc.lpszClassName, OFFSET ClassName
	INVOKE LoadIcon, NULL, IDI_APPLICATION
	mov wc.hIcon, eax
	mov wc.hIconSm, eax
	INVOKE LoadCursor, NULL, IDC_ARROW
	mov wc.hCursor, eax
	INVOKE RegisterClassEx, ADDR wc
	INVOKE CreateWindowEx, NULL, ADDR ClassName, ADDR AppName, \
		WS_OVERLAPPEDWINDOW AND (NOT WS_SIZEBOX) AND (NOT WS_MAXIMIZEBOX) AND (NOT WS_MINIMIZEBOX), CW_USEDEFAULT, \
		CW_USEDEFAULT, 800, 600, NULL, hMenu, \
		hInst, NULL
	mov hwnd, eax
	INVOKE ShowWindow, hwnd, SW_SHOWNORMAL
	INVOKE UpdateWindow, hwnd
	.WHILE TRUE
		INVOKE GetMessage, ADDR msg, NULL, 0, 0
		.BREAK .IF (!eax)
			INVOKE TranslateMessage, ADDR msg
		INVOKE DispatchMessage, ADDR msg
	.ENDW
	mov eax, msg.wParam
	ret
WinMain ENDP

;事件处理函数
WndProc PROC USES ebx ecx edx,
	hWnd: HWND, uMsg: UINT, wParam: WPARAM, lParam: LPARAM
	LOCAL ps: PAINTSTRUCT

	.IF uMsg == WM_DESTROY
		INVOKE PostQuitMessage, NULL
	.ELSEIF uMsg == WM_COMMAND	
		INVOKE IHandleModeChange, hWnd, wParam, lParam
	.ELSEIF uMsg == WM_MOUSEMOVE
		INVOKE IHandleMouseMove, hWnd, wParam, lParam	
	.ELSEIF uMsg == WM_LBUTTONDOWN
		INVOKE IHandleButtonDown, hWnd, wParam, lParam
	.ELSEIF uMsg == WM_LBUTTONUP
		INVOKE IHandleButtonUp, hWnd, wParam, lParam
	.ELSEIF uMsg == WM_SETCURSOR
		INVOKE IHandleCursor, hWnd, wParam, lParam
	.ELSEIF uMsg == WM_PAINT
		INVOKE IHandlePaint, hWnd, wParam, lParam, ps
	.ELSE
		INVOKE DefWindowProc, hWnd, uMsg, wParam, lParam
		ret
	.ENDIF
	xor eax, eax
	ret
WndProc ENDP

end main