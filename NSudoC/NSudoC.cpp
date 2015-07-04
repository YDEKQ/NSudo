//NSudo 3.0 (Build 825)
//(C) 2015 NSudo Team. All rights reserved.

#include "stdafx.h"

#include <windows.h>

#include<Userenv.h>
#pragma comment(lib,"Userenv.lib")

#include "..\\NSudoAPI\\NSudoAPI.h"
#pragma comment(lib,"NSudoAPI.lib")

#define NSudo_Title L"NSudo"
#define NSudo_Version L"3.0 Internal Alpha 1(Build 811)"
#define NSudo_CopyRight L"\xA9 2015 NSudo Team. All rights reserved."

#define ReturnMessage(lpText) MessageBoxW(NULL, (lpText), NSudo_Title, NULL)


#define LOGONID_CURRENT     ((ULONG)-1)
#define SERVERNAME_CURRENT  ((HANDLE)NULL)

typedef	enum _WINSTATIONINFOCLASS
{
	WinStationCreateData,
	WinStationConfiguration,
	WinStationPdParams,
	WinStationWd,
	WinStationPd,
	WinStationPrinter,
	WinStationClient,
	WinStationModules,
	WinStationInformation,
	WinStationTrace,
	WinStationBeep,
	WinStationEncryptionOff,
	WinStationEncryptionPerm,
	WinStationNtSecurity,
	WinStationUserToken,
	WinStationUnused1,
	WinStationVideoData,
	WinStationInitialProgram,
	WinStationCd,
	WinStationSystemTrace,
	WinStationVirtualData,
	WinStationClientData,
	WinStationSecureDesktopEnter,
	WinStationSecureDesktopExit,
	WinStationLoadBalanceSessionTarget,
	WinStationLoadIndicator,
	WinStationShadowInfo,
	WinStationDigProductId,
	WinStationLockedState,
	WinStationRemoteAddress,
	WinStationIdleTime,
	WinStationLastReconnectType,
	WinStationDisallowAutoReconnect,
	WinStationUnused2,
	WinStationUnused3,
	WinStationUnused4,
	WinStationUnused5,
	WinStationReconnectedFromId,
	WinStationEffectsPolicy,
	WinStationType,
	WinStationInformationEx
} WINSTATIONINFOCLASS;

typedef struct _WINSTATIONINFORMATIONW {
	BYTE Reserved2[70];
	ULONG LogonId;
	BYTE Reserved3[1140];
} WINSTATIONINFORMATIONW, *PWINSTATIONINFORMATIONW;

typedef struct _WINSTATIONUSERTOKEN {
	HANDLE ProcessId;
	HANDLE ThreadId;
	HANDLE UserToken;
} WINSTATIONUSERTOKEN,
*PWINSTATIONUSERTOKEN;

BOOLEAN WinStationQueryInformationW(
	_In_   HANDLE hServer,
	_In_   ULONG LogonId,
	_In_   WINSTATIONINFOCLASS WinStationInformationClass,
	_Out_  PVOID pWinStationInformation,
	_In_   ULONG WinStationInformationLength,
	_Out_  PULONG pReturnLength
	)
{
	HINSTANCE hInstWinSta = LoadLibraryW(L"winsta.dll");
	if (hInstWinSta == NULL) return FALSE;

	typedef BOOLEAN(WINAPI * PWINSTATIONQUERYINFORMATIONW)(
		HANDLE, ULONG, WINSTATIONINFOCLASS, PVOID, ULONG, PULONG);

	PWINSTATIONQUERYINFORMATIONW WinStationQueryInformationW =
		(PWINSTATIONQUERYINFORMATIONW)GetProcAddress(hInstWinSta, "WinStationQueryInformationW");

	if (WinStationQueryInformationW == NULL) return FALSE;

	return WinStationQueryInformationW(
		hServer,
		LogonId,
		WinStationInformationClass,
		pWinStationInformation,
		WinStationInformationLength,
		pReturnLength);
}



//#include <Winternl.h>

#include<Wtsapi32.h>
#pragma comment(lib,"Wtsapi32.lib")

typedef struct _UNICODE_STRING
{
	WORD Length;
	WORD MaximumLength;
	WORD * Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;


typedef struct _TEB_ACTIVE_FRAME_CONTEXT
{
	ULONG Flags;
	CHAR * FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME
{
	ULONG Flags;
	/*PTEB_ACTIVE_FRAME*/ int Previous;
	PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	UCHAR Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

typedef struct _STRING
{
	WORD Length;
	WORD MaximumLength;
	CHAR * Buffer;
} STRING, *PSTRING;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
	WORD Flags;
	WORD Length;
	ULONG TimeStamp;
	STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	PVOID ConsoleHandle;
	ULONG ConsoleFlags;
	PVOID StandardInput;
	PVOID StandardOutput;
	PVOID StandardError;
	CURDIR CurrentDirectory;
	UNICODE_STRING DllPath;
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
	PVOID Environment;
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING WindowTitle;
	UNICODE_STRING DesktopInfo;
	UNICODE_STRING ShellInfo;
	UNICODE_STRING RuntimeData;
	RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];
	ULONG EnvironmentSize;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_FREE_BLOCK
{
	/*PPEB_FREE_BLOCK*/ int Next;
	ULONG Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef struct _PEB
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	ULONG ImageUsesLargePages : 1;
	ULONG IsProtectedProcess : 1;
	ULONG IsLegacyProcess : 1;
	ULONG IsImageDynamicallyRelocated : 1;
	ULONG SpareBits : 4;
	PVOID Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID SubSystemData;
	PVOID ProcessHeap;
	PRTL_CRITICAL_SECTION FastPebLock;
	PVOID AtlThunkSListPtr;
	PVOID IFEOKey;
	ULONG CrossProcessFlags;
	ULONG ProcessInJob : 1;
	ULONG ProcessInitializing : 1;
	ULONG ReservedBits0 : 30;
	union
	{
		PVOID KernelCallbackTable;
		PVOID UserSharedInfoPtr;
	};
	ULONG SystemReserved[1];
	ULONG SpareUlong;
	PPEB_FREE_BLOCK FreeList;
	ULONG TlsExpansionCounter;
	PVOID TlsBitmap;
	ULONG TlsBitmapBits[2];
	PVOID ReadOnlySharedMemoryBase;
	PVOID HotpatchInformation;
	VOID * * ReadOnlyStaticServerData;
	PVOID AnsiCodePageData;
	PVOID OemCodePageData;
	PVOID UnicodeCaseTableData;
	ULONG NumberOfProcessors;
	ULONG NtGlobalFlag;
	LARGE_INTEGER CriticalSectionTimeout;
	ULONG HeapSegmentReserve;
	ULONG HeapSegmentCommit;
	ULONG HeapDeCommitTotalFreeThreshold;
	ULONG HeapDeCommitFreeBlockThreshold;
	ULONG NumberOfHeaps;
	ULONG MaximumNumberOfHeaps;
	VOID * * ProcessHeaps;
	PVOID GdiSharedHandleTable;
	PVOID ProcessStarterHelper;
	ULONG GdiDCAttributeList;
	PRTL_CRITICAL_SECTION LoaderLock;
	ULONG OSMajorVersion;
	ULONG OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	ULONG OSPlatformId;
	ULONG ImageSubsystem;
	ULONG ImageSubsystemMajorVersion;
	ULONG ImageSubsystemMinorVersion;
	ULONG ImageProcessAffinityMask;
	ULONG GdiHandleBuffer[34];
	PVOID PostProcessInitRoutine;
	PVOID TlsExpansionBitmap;
	ULONG TlsExpansionBitmapBits[32];
	ULONG SessionId;
	//ULARGE_INTEGER AppCompatFlags;
	//ULARGE_INTEGER AppCompatFlagsUser;
	//PVOID pShimData;
	//PVOID AppCompatInfo;
	//UNICODE_STRING CSDVersion;
	//_ACTIVATION_CONTEXT_DATA * ActivationContextData;
	//_ASSEMBLY_STORAGE_MAP * ProcessAssemblyStorageMap;
	//_ACTIVATION_CONTEXT_DATA * SystemDefaultActivationContextData;
	//_ASSEMBLY_STORAGE_MAP * SystemAssemblyStorageMap;
	//ULONG MinimumStackCommit;
	//_FLS_CALLBACK_INFO * FlsCallback;
	//LIST_ENTRY FlsListHead;
	//PVOID FlsBitmap;
	//ULONG FlsBitmapBits[4];
	//ULONG FlsHighIndex;
	//PVOID WerRegistrationData;
	//PVOID WerShipAssertPtr;
} PEB, *PPEB;

typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME
{
	/*PRTL_ACTIVATION_CONTEXT_STACK_FRAME*/ int Previous;
	_ACTIVATION_CONTEXT * ActivationContext;
	ULONG Flags;
} RTL_ACTIVATION_CONTEXT_STACK_FRAME, *PRTL_ACTIVATION_CONTEXT_STACK_FRAME;

typedef struct _ACTIVATION_CONTEXT_STACK
{
	PRTL_ACTIVATION_CONTEXT_STACK_FRAME ActiveFrame;
	LIST_ENTRY FrameListCache;
	ULONG Flags;
	ULONG NextCookieSequenceNumber;
	ULONG StackId;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

typedef struct _GDI_TEB_BATCH
{
	ULONG Offset;
	ULONG HDC;
	ULONG Buffer[310];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _TEB
{
	NT_TIB NtTib;
	PVOID EnvironmentPointer;
	CLIENT_ID ClientId;
	PVOID ActiveRpcHandle;
	PVOID ThreadLocalStoragePointer;
	PPEB ProcessEnvironmentBlock;
	ULONG LastErrorValue;
	ULONG CountOfOwnedCriticalSections;
	PVOID CsrClientThread;
	PVOID Win32ThreadInfo;
	ULONG User32Reserved[26];
	ULONG UserReserved[5];
	PVOID WOW32Reserved;
	ULONG CurrentLocale;
	ULONG FpSoftwareStatusRegister;
	VOID * SystemReserved1[54];
	LONG ExceptionCode;
	PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
	UCHAR SpareBytes1[36];
	ULONG TxFsContext;
	GDI_TEB_BATCH GdiTebBatch;
	CLIENT_ID RealClientId;
	PVOID GdiCachedProcessHandle;
	ULONG GdiClientPID;
	ULONG GdiClientTID;
	PVOID GdiThreadLocalInfo;
	ULONG Win32ClientInfo[62];
	VOID * glDispatchTable[233];
	ULONG glReserved1[29];
	PVOID glReserved2;
	PVOID glSectionInfo;
	PVOID glSection;
	PVOID glTable;
	PVOID glCurrentRC;
	PVOID glContext;
	ULONG LastStatusValue;
	UNICODE_STRING StaticUnicodeString;
	WCHAR StaticUnicodeBuffer[261];
	PVOID DeallocationStack;
	VOID * TlsSlots[64];
	LIST_ENTRY TlsLinks;
	PVOID Vdm;
	PVOID ReservedForNtRpc;
	VOID * DbgSsReserved[2];
	ULONG HardErrorMode;
	VOID * Instrumentation[9];
	GUID ActivityId;
	PVOID SubProcessTag;
	PVOID EtwLocalData;
	PVOID EtwTraceData;
	PVOID WinSockData;
	ULONG GdiBatchCount;
	UCHAR SpareBool0;
	UCHAR SpareBool1;
	UCHAR SpareBool2;
	UCHAR IdealProcessor;
	ULONG GuaranteedStackBytes;
	PVOID ReservedForPerf;
	PVOID ReservedForOle;
	ULONG WaitingOnLoaderLock;
	PVOID SavedPriorityState;
	ULONG SoftPatchPtr1;
	PVOID ThreadPoolData;
	VOID * * TlsExpansionSlots;
	ULONG ImpersonationLocale;
	ULONG IsImpersonating;
	PVOID NlsCache;
	PVOID pShimData;
	ULONG HeapVirtualAffinity;
	PVOID CurrentTransactionHandle;
	PTEB_ACTIVE_FRAME ActiveFrame;
	PVOID FlsData;
	PVOID PreferredLanguages;
	PVOID UserPrefLanguages;
	PVOID MergedPrefLanguages;
	ULONG MuiImpersonation;
	WORD CrossTebFlags;
	ULONG SpareCrossTebBits : 16;
	WORD SameTebFlags;
	ULONG DbgSafeThunkCall : 1;
	ULONG DbgInDebugPrint : 1;
	ULONG DbgHasFiberData : 1;
	ULONG DbgSkipThreadAttach : 1;
	ULONG DbgWerInShipAssertCode : 1;
	ULONG DbgRanProcessInit : 1;
	ULONG DbgClonedThread : 1;
	ULONG DbgSuppressDebugMsg : 1;
	ULONG SpareSameTebBits : 8;
	PVOID TxnScopeEnterCallback;
	PVOID TxnScopeExitCallback;
	PVOID TxnScopeContext;
	ULONG LockCount;
	ULONG ProcessRundown;
	UINT64 LastSwitchTime;
	UINT64 TotalSwitchOutTime;
	LARGE_INTEGER WaitReasonBitMap;
} TEB, *PTEB;


int _tmain(int argc, _TCHAR* argv[])
{
	if (!(SetCurrentProcessPrivilege(SE_ASSIGNPRIMARYTOKEN_NAME, true)
		&& SetCurrentProcessPrivilege(SE_INCREASE_QUOTA_NAME, true)
		&& SetCurrentProcessPrivilege(SE_DEBUG_NAME, true)))
	{
		ReturnMessage(L"进程调试权限获取失败");
		return -1;
	}

	PPEB PEB = NtCurrentTeb()->ProcessEnvironmentBlock;

	return 0;
}

void Backup()
{
	HANDLE hSystemToken;
	//if (GetSystemToken(&hSystemToken))
	{
		if (ImpersonateLoggedOnUser(hSystemToken))
		{
			WINSTATIONUSERTOKEN WinStaUserToken = { 0 };
			DWORD ccbInfo = NULL;

			WinStationQueryInformationW(SERVERNAME_CURRENT, LOGONID_CURRENT, WinStationUserToken, &WinStaUserToken, sizeof(WINSTATIONUSERTOKEN), &ccbInfo);

			HRESULT hr = GetLastError();

			STARTUPINFOW StartupInfo = { 0 };
			PROCESS_INFORMATION ProcessInfo = { 0 };
			StartupInfo.lpDesktop = L"WinSta0\\Default";

			wchar_t szCMDLine[] = L"cmd /c start cmd";

			if (CreateProcessAsUserW(WinStaUserToken.UserToken, NULL, szCMDLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
			{

			}


			RevertToSelf();

			ReturnMessage(L"OK");
		}

		CloseHandle(hSystemToken);
	}
}