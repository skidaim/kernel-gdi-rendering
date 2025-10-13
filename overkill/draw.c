#include "utils.h"
HDC hdc;
HBRUSH brush;
int drawbox(int x, int y, int w, int h, int border) {
	if (!spoofthread())
	{
		return FALSE;
	}
	hdc = NtUserGetDC(0);
	if (!hdc)
	{
		KdPrint(("NtUserGetDC Failed\n"));
		return FALSE;
	}

	brush = NtGdiCreateSolidBrush(RGB(255, 0, 0), NULL);
	if (!brush)
	{
		KdPrint(("NtGdiCreateSolidBrush Failed\n"));
		NtUserReleaseDC(hdc);
		return FALSE;
	}

	NtGdiPatBlt(hdc, x, y, w, border, PATCOPY);
	NtGdiPatBlt(hdc, x, y, border, h, PATCOPY);
	NtGdiPatBlt(hdc, x + w, y, border, h, PATCOPY);
	NtGdiPatBlt(hdc, x, y - h, w, border, PATCOPY);

	NtGdiDeleteObjectApp(brush);
	NtUserReleaseDC(hdc);

	unspoofthread();
	return TRUE;
}