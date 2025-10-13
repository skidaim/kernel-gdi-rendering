#pragma once
#include "stuff.h"
#include <WinDef.h>
#include <math.h>
#include <wingdi.h>
#include <winddi.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

HANDLE pid;
uintptr_t LocalPlayer;

NTSTATUS read(PVOID address, void* buffer, SIZE_T size);

ULONG ThreadProcessOffset;
KAPC_STATE apc_state;
HDC hdc;
HBRUSH brush;


#pragma pack(push, 1)

// PDB SuperBlock
struct SuperBlock
{
	CHAR    FileMagic[32];
	UINT32  BlockSize;
	UINT32  FreeBlockMapBlock;
	UINT32  NumBlocks;
	UINT32  NumDirectoryBytes;
	UINT32  Unknown;
	UINT32  BlockMapAddr;
};

// PDB DBI Header
struct DBIHeader
{
	INT32   VersionSignature;
	UINT32  VersionHeader;
	UINT32  Age;
	UINT16  GlobalStreamIndex;
	UINT16  BuildNumber;
	UINT16  PublicStreamIndex;
	UINT16  PdbDllVersion;
	UINT16  SymRecordStream;
	UINT16  PdbDllRbld;
	INT32   ModInfoSize;
	INT32   SectionContributionSize;
	INT32   SectionMapSize;
	INT32   SourceInfoSize;
	INT32   TypeServerSize;
	UINT32  MFCTypeServerIndex;
	INT32   OptionalDbgHeaderSize;
	INT32   ECSubstreamSize;
	UINT16  Flags;
	UINT16  Machine;
	UINT32  Padding;
};

// PDB Public Symbol Record
struct PUBSYM32
{
	UINT16 reclen;
	UINT16 rectyp;
	UINT32 pubsymflags;
	UINT32 off;     // Offset from the start of the section
	UINT16 seg;     // 1-based section index
	CHAR   name[1];
};
#pragma pack(pop)
#define S_PUB32 0x110e
static const char PDB_MAGIC[] = { 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x2F, 0x43, 0x2B, 0x2B, 0x20, 0x4D, 0x53, 0x46, 0x20, 0x37, 0x2E, 0x30, 0x30, 0x0D, 0x0A, 0x1A, 0x44, 0x53, 0x00, 0x00, 0x00 };


typedef struct _KERNEL_BUFFER { PVOID Buffer; SIZE_T Size; } KERNEL_BUFFER, * PKERNEL_BUFFER;
typedef HDC(NTAPI* pfnNtUserGetDC)(HWND hWnd);
typedef int (NTAPI* pfnNtUserReleaseDC)(HDC hDC);
typedef BOOL(APIENTRY* pfnNtGdiPatBlt)(_In_ HDC hdcDest, _In_ INT x, _In_ INT y, _In_ INT cx, _In_ INT cy, _In_ DWORD dwRop);
typedef HBRUSH(APIENTRY* pfnGreSelectBrush)(IN HDC hDC, IN HBRUSH hBrush);
typedef HBRUSH(APIENTRY* pfnNtGdiCreateSolidBrush)(_In_ COLORREF cr, _In_opt_ HBRUSH hbr);
typedef BOOL(APIENTRY* pfnNtGdiDeleteObjectApp)(HANDLE hobj);
typedef PVOID(NTAPI* pfnNtUserFindWindowEx)(PVOID, PVOID, PUNICODE_STRING, PUNICODE_STRING);
typedef BOOL(APIENTRY* pfnNtGdiExtTextOutW)(IN HDC hDC, IN INT XStart, IN INT YStart, IN UINT fuOptions, IN OPTIONAL LPRECT UnsafeRect, IN LPWSTR UnsafeString, IN INT Count, IN OPTIONAL LPINT UnsafeDx, IN DWORD dwCodePage);
typedef HBITMAP(APIENTRY* pfnNtGdiCreateCompatibleBitmap)(_In_ HDC hdc, _In_ INT cx, _In_ INT cy);
typedef HDC(APIENTRY* pfnNtGdiCreateCompatibleDC)(_In_opt_ HDC 	hdc);
typedef HBITMAP(APIENTRY* pfnNtGdiSelectBitmap)	(_In_ HDC hdc, _In_ HBITMAP hbm);
typedef HDC(APIENTRY* pfnNtUserGetDCEx)(HWND hwnd, HANDLE region, ULONG flags);
typedef BOOL(APIENTRY* pfnNtGdiBitBlt)(_In_ HDC hdcDst, _In_ INT x, _In_ INT 	y, _In_ INT 	cx, _In_ INT cy, _In_opt_ HDC hdcSrc, _In_ INT xSrc, _In_ INT ySrc, _In_ DWORD rop4, _In_ DWORD crBackColor, _In_ FLONG fl);
typedef BOOL(APIENTRY* pfnNtUserInvalidateRect)(HWND hWnd, CONST RECT* lpUnsafeRect, BOOL bErase);
typedef BOOL(APIENTRY* pfnNtUserRedrawWindow)(HWND hWnd, CONST RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags);
typedef BOOL(APIENTRY* pfnEngBitBlt)(_Inout_ SURFOBJ* psoTrg,_In_opt_ SURFOBJ* psoSrc,_In_opt_ SURFOBJ* psoMask,_In_opt_ CLIPOBJ* pco,_In_opt_ XLATEOBJ* pxlo,_In_ RECTL* prclTrg,_When_(psoSrc, _In_) POINTL* pptlSrc,_When_(psoMask, _In_) POINTL* pptlMask,_In_opt_ BRUSHOBJ* pbo,_When_(pbo, _In_) POINTL* pptlBrush,_In_ ROP4 	rop4);
typedef short (*pfnNtUserGetAsyncKeyState)(INT a);


NTKERNELAPI
PVOID
PsGetProcessSectionBaseAddress(
	__in PEPROCESS Process
);


PETHREAD OriginalWin32Thread;
PEPROCESS OriginalProcess;

PVOID GetModuleBase(PCHAR szModuleName);

ULONG GetActiveProcessLinksOffset();

PEPROCESS GetProcessByName(PCHAR szName);

PEPROCESS GetGameProcess(PWCH szName);

PETHREAD GetProcessMainThread(PEPROCESS Process);

PUNICODE_STRING unicodestrstr(PUNICODE_STRING Haystack, PUNICODE_STRING Needle);

void NtSleep(DWORD milliseconds);

int init();
int spoofthread();
int unspoofthread();

void drawbox(int x, int y, int w, int h, int border);
void refresh();
int getkey();

