#include <stdio.h>
#include <conio.h>
#include <Windows.h>

int main(int argc, char **argv)
{
	OpenClipboard(NULL);
	HBITMAP hBmp = (HBITMAP)GetClipboardData(CF_BITMAP);
	CloseClipboard();

	if(hBmp == NULL) return 1;

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	BITMAPINFOHEADER bi = {};
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmp.bmWidth;
	bi.biHeight = bmp.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;

	DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;

	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char *lpBmp = (char *)GlobalLock(hDIB);
	GetDIBits(GetDC(NULL), hBmp, 0, (UINT)bmp.bmHeight, lpBmp, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	BITMAPFILEHEADER bh = {};
	bh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	bh.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bh.bfType = 0x4D42;

	HANDLE STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWritten = 0;
	WriteFile(STDOUT, (LPSTR)&bh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(STDOUT, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(STDOUT, (LPSTR)lpBmp, dwBmpSize, &dwBytesWritten, NULL);

	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	return 0;
}