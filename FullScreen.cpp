#include <Novice.h>

// フルスクリーンにする関数
void SetFullScreen(HWND hwnd) {
	// 画面解像度をフルスクリーンに合わせる
	DEVMODE devMode = {};
	devMode.dmSize = sizeof(DEVMODE);
	devMode.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN); // 画面幅
	devMode.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN); // 画面高さ
	devMode.dmBitsPerPel = 32; // 色深度
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	// 解像度を変更する
	ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

	// ウィンドウをフルスクリーンにする
	SetWindowLong(hwnd, GWL_STYLE, 0); // タイトルバーや枠を非表示
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, devMode.dmPelsWidth, devMode.dmPelsHeight, SWP_SHOWWINDOW);
}