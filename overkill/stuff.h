#pragma once
#include <ntifs.h>
#include <wdf.h>
typedef enum _MEMORY_INFORMATION_CLASS_EX
{
    MemoryBasicInformationEx = 0,
    MemoryWorkingSetInformation = 1,
    MemoryMappedFilenameInformation = 2,
    MemoryRegionInformation = 3,
    MemoryWorkingSetExInformation = 4,
} MEMORY_INFORMATION_CLASS_EX;

typedef enum _PS_PROTECTED_SIGNER
{
    PsProtectedSignerNone = 0,
    PsProtectedSignerAuthenticode = 1,
    PsProtectedSignerCodeGen = 2,
    PsProtectedSignerAntimalware = 3,
    PsProtectedSignerLsa = 4,
    PsProtectedSignerWindows = 5,
    PsProtectedSignerWinTcb = 6,
    PsProtectedSignerMax = 7
} PS_PROTECTED_SIGNER;

typedef enum _PS_PROTECTED_TYPE
{
    PsProtectedTypeNone = 0,
    PsProtectedTypeProtectedLight = 1,
    PsProtectedTypeProtected = 2,
    PsProtectedTypeMax = 3
} PS_PROTECTED_TYPE;

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation = 0x0,
    SystemProcessorInformation = 0x1,
    SystemPerformanceInformation = 0x2,
    SystemTimeOfDayInformation = 0x3,
    SystemPathInformation = 0x4,
    SystemProcessInformation = 0x5,
    SystemCallCountInformation = 0x6,
    SystemDeviceInformation = 0x7,
    SystemProcessorPerformanceInformation = 0x8,
    SystemFlagsInformation = 0x9,
    SystemCallTimeInformation = 0xa,
    SystemModuleInformation = 0xb,
    SystemLocksInformation = 0xc,
    SystemStackTraceInformation = 0xd,
    SystemPagedPoolInformation = 0xe,
    SystemNonPagedPoolInformation = 0xf,
    SystemHandleInformation = 0x10,
    SystemObjectInformation = 0x11,
    SystemPageFileInformation = 0x12,
    SystemVdmInstemulInformation = 0x13,
    SystemVdmBopInformation = 0x14,
    SystemFileCacheInformation = 0x15,
    SystemPoolTagInformation = 0x16,
    SystemInterruptInformation = 0x17,
    SystemDpcBehaviorInformation = 0x18,
    SystemFullMemoryInformation = 0x19,
    SystemLoadGdiDriverInformation = 0x1a,
    SystemUnloadGdiDriverInformation = 0x1b,
    SystemTimeAdjustmentInformation = 0x1c,
    SystemSummaryMemoryInformation = 0x1d,
    SystemMirrorMemoryInformation = 0x1e,
    SystemPerformanceTraceInformation = 0x1f,
    SystemObsolete0 = 0x20,
    SystemExceptionInformation = 0x21,
    SystemCrashDumpStateInformation = 0x22,
    SystemKernelDebuggerInformation = 0x23,
    SystemContextSwitchInformation = 0x24,
    SystemRegistryQuotaInformation = 0x25,
    SystemExtendServiceTableInformation = 0x26,
    SystemPrioritySeperation = 0x27,
    SystemVerifierAddDriverInformation = 0x28,
    SystemVerifierRemoveDriverInformation = 0x29,
    SystemProcessorIdleInformation = 0x2a,
    SystemLegacyDriverInformation = 0x2b,
    SystemCurrentTimeZoneInformation = 0x2c,
    SystemLookasideInformation = 0x2d,
    SystemTimeSlipNotification = 0x2e,
    SystemSessionCreate = 0x2f,
    SystemSessionDetach = 0x30,
    SystemSessionInformation = 0x31,
    SystemRangeStartInformation = 0x32,
    SystemVerifierInformation = 0x33,
    SystemVerifierThunkExtend = 0x34,
    SystemSessionProcessInformation = 0x35,
    SystemLoadGdiDriverInSystemSpace = 0x36,
    SystemNumaProcessorMap = 0x37,
    SystemPrefetcherInformation = 0x38,
    SystemExtendedProcessInformation = 0x39,
    SystemRecommendedSharedDataAlignment = 0x3a,
    SystemComPlusPackage = 0x3b,
    SystemNumaAvailableMemory = 0x3c,
    SystemProcessorPowerInformation = 0x3d,
    SystemEmulationBasicInformation = 0x3e,
    SystemEmulationProcessorInformation = 0x3f,
    SystemExtendedHandleInformation = 0x40,
    SystemLostDelayedWriteInformation = 0x41,
    SystemBigPoolInformation = 0x42,
    SystemSessionPoolTagInformation = 0x43,
    SystemSessionMappedViewInformation = 0x44,
    SystemHotpatchInformation = 0x45,
    SystemObjectSecurityMode = 0x46,
    SystemWatchdogTimerHandler = 0x47,
    SystemWatchdogTimerInformation = 0x48,
    SystemLogicalProcessorInformation = 0x49,
    SystemWow64SharedInformationObsolete = 0x4a,
    SystemRegisterFirmwareTableInformationHandler = 0x4b,
    SystemFirmwareTableInformation = 0x4c,
    SystemModuleInformationEx = 0x4d,
    SystemVerifierTriageInformation = 0x4e,
    SystemSuperfetchInformation = 0x4f,
    SystemMemoryListInformation = 0x50,
    SystemFileCacheInformationEx = 0x51,
    SystemThreadPriorityClientIdInformation = 0x52,
    SystemProcessorIdleCycleTimeInformation = 0x53,
    SystemVerifierCancellationInformation = 0x54,
    SystemProcessorPowerInformationEx = 0x55,
    SystemRefTraceInformation = 0x56,
    SystemSpecialPoolInformation = 0x57,
    SystemProcessIdInformation = 0x58,
    SystemErrorPortInformation = 0x59,
    SystemBootEnvironmentInformation = 0x5a,
    SystemHypervisorInformation = 0x5b,
    SystemVerifierInformationEx = 0x5c,
    SystemTimeZoneInformation = 0x5d,
    SystemImageFileExecutionOptionsInformation = 0x5e,
    SystemCoverageInformation = 0x5f,
    SystemPrefetchPatchInformation = 0x60,
    SystemVerifierFaultsInformation = 0x61,
    SystemSystemPartitionInformation = 0x62,
    SystemSystemDiskInformation = 0x63,
    SystemProcessorPerformanceDistribution = 0x64,
    SystemNumaProximityNodeInformation = 0x65,
    SystemDynamicTimeZoneInformation = 0x66,
    SystemCodeIntegrityInformation = 0x67,
    SystemProcessorMicrocodeUpdateInformation = 0x68,
    SystemProcessorBrandString = 0x69,
    SystemVirtualAddressInformation = 0x6a,
    SystemLogicalProcessorAndGroupInformation = 0x6b,
    SystemProcessorCycleTimeInformation = 0x6c,
    SystemStoreInformation = 0x6d,
    SystemRegistryAppendString = 0x6e,
    SystemAitSamplingValue = 0x6f,
    SystemVhdBootInformation = 0x70,
    SystemCpuQuotaInformation = 0x71,
    SystemNativeBasicInformation = 0x72,
    SystemErrorPortTimeouts = 0x73,
    SystemLowPriorityIoInformation = 0x74,
    SystemBootEntropyInformation = 0x75,
    SystemVerifierCountersInformation = 0x76,
    SystemPagedPoolInformationEx = 0x77,
    SystemSystemPtesInformationEx = 0x78,
    SystemNodeDistanceInformation = 0x79,
    SystemAcpiAuditInformation = 0x7a,
    SystemBasicPerformanceInformation = 0x7b,
    SystemQueryPerformanceCounterInformation = 0x7c,
    SystemSessionBigPoolInformation = 0x7d,
    SystemBootGraphicsInformation = 0x7e,
    SystemScrubPhysicalMemoryInformation = 0x7f,
    SystemBadPageInformation = 0x80,
    SystemProcessorProfileControlArea = 0x81,
    SystemCombinePhysicalMemoryInformation = 0x82,
    SystemEntropyInterruptTimingInformation = 0x83,
    SystemConsoleInformation = 0x84,
    SystemPlatformBinaryInformation = 0x85,
    SystemThrottleNotificationInformation = 0x86,
    SystemHypervisorProcessorCountInformation = 0x87,
    SystemDeviceDataInformation = 0x88,
    SystemDeviceDataEnumerationInformation = 0x89,
    SystemMemoryTopologyInformation = 0x8a,
    SystemMemoryChannelInformation = 0x8b,
    SystemBootLogoInformation = 0x8c,
    SystemProcessorPerformanceInformationEx = 0x8d,
    SystemSpare0 = 0x8e,
    SystemSecureBootPolicyInformation = 0x8f,
    SystemPageFileInformationEx = 0x90,
    SystemSecureBootInformation = 0x91,
    SystemEntropyInterruptTimingRawInformation = 0x92,
    SystemPortableWorkspaceEfiLauncherInformation = 0x93,
    SystemFullProcessInformation = 0x94,
    SystemKernelDebuggerInformationEx = 0x95,
    SystemBootMetadataInformation = 0x96,
    SystemSoftRebootInformation = 0x97,
    SystemElamCertificateInformation = 0x98,
    SystemOfflineDumpConfigInformation = 0x99,
    SystemProcessorFeaturesInformation = 0x9a,
    SystemRegistryReconciliationInformation = 0x9b,
    MaxSystemInfoClass = 0x9c,
} SYSTEM_INFORMATION_CLASS;

typedef enum _PROCESS_INFORMATION_CLASS {
    ProcessMemoryPriority,
    ProcessMemoryExhaustionInfo,
    ProcessAppMemoryInfo,
    ProcessInPrivateInfo,
    ProcessPowerThrottling,
    ProcessReservedValue1,           // Used to be for ProcessActivityThrottlePolicyInfo
    ProcessTelemetryCoverageInfo,
    ProcessProtectionLevelInfo,
    ProcessLeapSecondInfo,
    ProcessInformationClassMax
} PROCESS_INFORMATION_CLASS;

typedef enum _KAPC_ENVIRONMENT
{
    OriginalApcEnvironment,
    AttachedApcEnvironment,
    CurrentApcEnvironment,
    InsertApcEnvironment
} KAPC_ENVIRONMENT, * PKAPC_ENVIRONMENT;

typedef enum _MI_VAD_TYPE
{
    VadNone,
    VadDevicePhysicalMemory,
    VadImageMap,
    VadAwe,
    VadWriteWatch,
    VadLargePages,
    VadRotatePhysical,
    VadLargePageSection
} MI_VAD_TYPE, * PMI_VAD_TYPE;

typedef enum _MMSYSTEM_PTE_POOL_TYPE
{
    SystemPteSpace,
    NonPagedPoolExpansion,
    MaximumPtePoolTypes
} MMSYSTEM_PTE_POOL_TYPE;



#pragma once
#define _WIN10_

#pragma once


#define IMAGE_DOS_SIGNATURE                     0x5A4D      // MZ
#define IMAGE_NT_SIGNATURE                      0x00004550  // PE00

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC           0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC           0x20b

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES        16

#define IMAGE_DIRECTORY_ENTRY_EXPORT             0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT             1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE           2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION          3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY           4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC          5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG              6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT          7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE       7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR          8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS                9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG       10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT      11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT               12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT      13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR    14   // COM Runtime descriptor

#define IMAGE_REL_BASED_ABSOLUTE                0
#define IMAGE_REL_BASED_HIGH                    1
#define IMAGE_REL_BASED_LOW                     2
#define IMAGE_REL_BASED_HIGHLOW                 3
#define IMAGE_REL_BASED_HIGHADJ                 4
#define IMAGE_REL_BASED_MIPS_JMPADDR            5
#define IMAGE_REL_BASED_SECTION                 6
#define IMAGE_REL_BASED_REL32                   7
#define IMAGE_REL_BASED_MIPS_JMPADDR16          9
#define IMAGE_REL_BASED_IA64_IMM64              9
#define IMAGE_REL_BASED_DIR64                   10

#define IMAGE_SIZEOF_BASE_RELOCATION            8


#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved external references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Aggressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2  // ARM Thumb/Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_ARMNT             0x01c4  // ARM Thumb-2 Little-Endian
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE

#define IMAGE_ORDINAL_FLAG64 0x8000000000000000
#define IMAGE_ORDINAL_FLAG32 0x80000000
#define IMAGE_ORDINAL64(Ordinal) (Ordinal & 0xffff)
#define IMAGE_ORDINAL32(Ordinal) (Ordinal & 0xffff)
#define IMAGE_SNAP_BY_ORDINAL64(Ordinal) ((Ordinal & IMAGE_ORDINAL_FLAG64) != 0)
#define IMAGE_SNAP_BY_ORDINAL32(Ordinal) ((Ordinal & IMAGE_ORDINAL_FLAG32) != 0)

//
// Section characteristics.
//
//      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
//      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
//      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
//      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  // Reserved.
//      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
//      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.
//                                           0x00002000  // Reserved.
//      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
#define IMAGE_SCN_NO_DEFER_SPEC_EXC          0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define IMAGE_SCN_GPREL                      0x00008000  // Section content can be accessed relative to GP
#define IMAGE_SCN_MEM_FARDATA                0x00008000
//      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000  //
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000  //
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000  //
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000  //
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000  //
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000  //
#define IMAGE_SCN_ALIGN_128BYTES             0x00800000  //
#define IMAGE_SCN_ALIGN_256BYTES             0x00900000  //
#define IMAGE_SCN_ALIGN_512BYTES             0x00A00000  //
#define IMAGE_SCN_ALIGN_1024BYTES            0x00B00000  //
#define IMAGE_SCN_ALIGN_2048BYTES            0x00C00000  //
#define IMAGE_SCN_ALIGN_4096BYTES            0x00D00000  //
#define IMAGE_SCN_ALIGN_8192BYTES            0x00E00000  //
// Unused                                    0x00F00000
#define IMAGE_SCN_ALIGN_MASK                 0x00F00000

#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.

typedef struct _IMAGE_DOS_HEADER
{
    USHORT e_magic;
    USHORT e_cblp;
    USHORT e_cp;
    USHORT e_crlc;
    USHORT e_cparhdr;
    USHORT e_minalloc;
    USHORT e_maxalloc;
    USHORT e_ss;
    USHORT e_sp;
    USHORT e_csum;
    USHORT e_ip;
    USHORT e_cs;
    USHORT e_lfarlc;
    USHORT e_ovno;
    USHORT e_res[4];
    USHORT e_oemid;
    USHORT e_oeminfo;
    USHORT e_res2[10];
    LONG e_lfanew;
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_SECTION_HEADER
{
    UCHAR  Name[8];
    union
    {
        ULONG PhysicalAddress;
        ULONG VirtualSize;
    } Misc;
    ULONG VirtualAddress;
    ULONG SizeOfRawData;
    ULONG PointerToRawData;
    ULONG PointerToRelocations;
    ULONG PointerToLinenumbers;
    USHORT  NumberOfRelocations;
    USHORT  NumberOfLinenumbers;
    ULONG Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_FILE_HEADER // Size=20
{
    USHORT Machine;
    USHORT NumberOfSections;
    ULONG TimeDateStamp;
    ULONG PointerToSymbolTable;
    ULONG NumberOfSymbols;
    USHORT SizeOfOptionalHeader;
    USHORT Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY
{
    ULONG VirtualAddress;
    ULONG Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER64
{
    USHORT Magic;
    UCHAR MajorLinkerVersion;
    UCHAR MinorLinkerVersion;
    ULONG SizeOfCode;
    ULONG SizeOfInitializedData;
    ULONG SizeOfUninitializedData;
    ULONG AddressOfEntryPoint;
    ULONG BaseOfCode;
    ULONGLONG ImageBase;
    ULONG SectionAlignment;
    ULONG FileAlignment;
    USHORT MajorOperatingSystemVersion;
    USHORT MinorOperatingSystemVersion;
    USHORT MajorImageVersion;
    USHORT MinorImageVersion;
    USHORT MajorSubsystemVersion;
    USHORT MinorSubsystemVersion;
    ULONG Win32VersionValue;
    ULONG SizeOfImage;
    ULONG SizeOfHeaders;
    ULONG CheckSum;
    USHORT Subsystem;
    USHORT DllCharacteristics;
    ULONGLONG SizeOfStackReserve;
    ULONGLONG SizeOfStackCommit;
    ULONGLONG SizeOfHeapReserve;
    ULONGLONG SizeOfHeapCommit;
    ULONG LoaderFlags;
    ULONG NumberOfRvaAndSizes;
    struct _IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_OPTIONAL_HEADER32
{
    //
    // Standard fields.
    //

    USHORT  Magic;
    UCHAR   MajorLinkerVersion;
    UCHAR   MinorLinkerVersion;
    ULONG   SizeOfCode;
    ULONG   SizeOfInitializedData;
    ULONG   SizeOfUninitializedData;
    ULONG   AddressOfEntryPoint;
    ULONG   BaseOfCode;
    ULONG   BaseOfData;

    //
    // NT additional fields.
    //

    ULONG   ImageBase;
    ULONG   SectionAlignment;
    ULONG   FileAlignment;
    USHORT  MajorOperatingSystemVersion;
    USHORT  MinorOperatingSystemVersion;
    USHORT  MajorImageVersion;
    USHORT  MinorImageVersion;
    USHORT  MajorSubsystemVersion;
    USHORT  MinorSubsystemVersion;
    ULONG   Win32VersionValue;
    ULONG   SizeOfImage;
    ULONG   SizeOfHeaders;
    ULONG   CheckSum;
    USHORT  Subsystem;
    USHORT  DllCharacteristics;
    ULONG   SizeOfStackReserve;
    ULONG   SizeOfStackCommit;
    ULONG   SizeOfHeapReserve;
    ULONG   SizeOfHeapCommit;
    ULONG   LoaderFlags;
    ULONG   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, * PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_NT_HEADERS64
{
    ULONG Signature;
    struct _IMAGE_FILE_HEADER FileHeader;
    struct _IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_NT_HEADERS
{
    ULONG Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS;

typedef struct _IMAGE_EXPORT_DIRECTORY {
    ULONG   Characteristics;
    ULONG   TimeDateStamp;
    USHORT  MajorVersion;
    USHORT  MinorVersion;
    ULONG   Name;
    ULONG   Base;
    ULONG   NumberOfFunctions;
    ULONG   NumberOfNames;
    ULONG   AddressOfFunctions;     // RVA from base of image
    ULONG   AddressOfNames;         // RVA from base of image
    ULONG   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_BASE_RELOCATION {
    ULONG   VirtualAddress;
    ULONG   SizeOfBlock;
    //  USHORT  TypeOffset[1];
} IMAGE_BASE_RELOCATION;
typedef IMAGE_BASE_RELOCATION UNALIGNED* PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_IMPORT_BY_NAME {
    USHORT Hint;
    CHAR   Name[1];
} IMAGE_IMPORT_BY_NAME, * PIMAGE_IMPORT_BY_NAME;


// warning C4201: nonstandard extension used : nameless struct/union
#pragma warning (disable : 4201)

typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
    union {
        ULONG   Characteristics;            // 0 for terminating null import descriptor
        ULONG   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
    };
    ULONG   TimeDateStamp;                  // 0 if not bound,
    // -1 if bound, and real date\time stamp
    //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
    // O.W. date/time stamp of DLL bound to (Old BIND)

    ULONG   ForwarderChain;                 // -1 if no forwarders
    ULONG   Name;
    ULONG   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED* PIMAGE_IMPORT_DESCRIPTOR;


typedef struct _IMAGE_THUNK_DATA64
{
    union
    {
        ULONGLONG ForwarderString;  // PBYTE 
        ULONGLONG Function;         // PULONG
        ULONGLONG Ordinal;
        ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME
    } u1;
} IMAGE_THUNK_DATA64;
typedef IMAGE_THUNK_DATA64* PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_THUNK_DATA32
{
    union
    {
        ULONG ForwarderString;      // PBYTE 
        ULONG Function;             // PULONG
        ULONG Ordinal;
        ULONG AddressOfData;        // PIMAGE_IMPORT_BY_NAME
    } u1;
} IMAGE_THUNK_DATA32;
typedef IMAGE_THUNK_DATA32* PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_RESOURCE_DIRECTORY {
    ULONG   Characteristics;
    ULONG   TimeDateStamp;
    USHORT  MajorVersion;
    USHORT  MinorVersion;
    USHORT  NumberOfNamedEntries;
    USHORT  NumberOfIdEntries;
    //  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
} IMAGE_RESOURCE_DIRECTORY, * PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union {
        struct {
            INT NameOffset : 31;
            INT NameIsString : 1;
        } DUMMYSTRUCTNAME;
        ULONG   Name;
        USHORT  Id;
    } DUMMYUNIONNAME;
    union {
        ULONG   OffsetToData;
        struct {
            INT   OffsetToDirectory : 31;
            INT   DataIsDirectory : 1;
        } DUMMYSTRUCTNAME2;
    } DUMMYUNIONNAME2;
} IMAGE_RESOURCE_DIRECTORY_ENTRY, * PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
    ULONG OffsetToData;
    ULONG Size;
    ULONG CodePage;
    ULONG Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, * PIMAGE_RESOURCE_DATA_ENTRY;

typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY {
    ULONG BeginAddress;
    ULONG EndAddress;
    union {
        ULONG UnwindInfoAddress;
        ULONG UnwindData;
    } DUMMYUNIONNAME;
} _IMAGE_RUNTIME_FUNCTION_ENTRY, * _PIMAGE_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY32 {
    ULONG   Size;
    ULONG   TimeDateStamp;
    USHORT  MajorVersion;
    USHORT  MinorVersion;
    ULONG   GlobalFlagsClear;
    ULONG   GlobalFlagsSet;
    ULONG   CriticalSectionDefaultTimeout;
    ULONG   DeCommitFreeBlockThreshold;
    ULONG   DeCommitTotalFreeThreshold;
    ULONG   LockPrefixTable;                // VA
    ULONG   MaximumAllocationSize;
    ULONG   VirtualMemoryThreshold;
    ULONG   ProcessHeapFlags;
    ULONG   ProcessAffinityMask;
    USHORT  CSDVersion;
    USHORT  Reserved1;
    ULONG   EditList;                       // VA
    ULONG   SecurityCookie;                 // VA
    ULONG   SEHandlerTable;                 // VA
    ULONG   SEHandlerCount;
    ULONG   GuardCFCheckFunctionPointer;    // VA
    ULONG   Reserved2;
    ULONG   GuardCFFunctionTable;           // VA
    ULONG   GuardCFFunctionCount;
    ULONG   GuardFlags;
} IMAGE_LOAD_CONFIG_DIRECTORY32, * PIMAGE_LOAD_CONFIG_DIRECTORY32;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY64 {
    ULONG      Size;
    ULONG      TimeDateStamp;
    USHORT     MajorVersion;
    USHORT     MinorVersion;
    ULONG      GlobalFlagsClear;
    ULONG      GlobalFlagsSet;
    ULONG      CriticalSectionDefaultTimeout;
    ULONGLONG  DeCommitFreeBlockThreshold;
    ULONGLONG  DeCommitTotalFreeThreshold;
    ULONGLONG  LockPrefixTable;             // VA
    ULONGLONG  MaximumAllocationSize;
    ULONGLONG  VirtualMemoryThreshold;
    ULONGLONG  ProcessAffinityMask;
    ULONG      ProcessHeapFlags;
    USHORT     CSDVersion;
    USHORT     Reserved1;
    ULONGLONG  EditList;                    // VA
    ULONGLONG  SecurityCookie;              // VA
    ULONGLONG  SEHandlerTable;              // VA
    ULONGLONG  SEHandlerCount;
    ULONGLONG  GuardCFCheckFunctionPointer; // VA
    ULONGLONG  Reserved2;
    ULONGLONG  GuardCFFunctionTable;        // VA
    ULONGLONG  GuardCFFunctionCount;
    ULONG      GuardFlags;
} IMAGE_LOAD_CONFIG_DIRECTORY64, * PIMAGE_LOAD_CONFIG_DIRECTORY64;

typedef struct _IMAGE_TLS_DIRECTORY64 {
    ULONGLONG StartAddressOfRawData;
    ULONGLONG EndAddressOfRawData;
    ULONGLONG AddressOfIndex;         // PULONG
    ULONGLONG AddressOfCallBacks;     // PIMAGE_TLS_CALLBACK *;
    ULONG SizeOfZeroFill;
    union {
        ULONG Characteristics;
        struct {
            INT Reserved0 : 20;
            INT Alignment : 4;
            INT Reserved1 : 8;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

} IMAGE_TLS_DIRECTORY64;

typedef IMAGE_TLS_DIRECTORY64* PIMAGE_TLS_DIRECTORY64;

typedef struct _IMAGE_TLS_DIRECTORY32 {
    ULONG   StartAddressOfRawData;
    ULONG   EndAddressOfRawData;
    ULONG   AddressOfIndex;             // PULONG
    ULONG   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
    ULONG   SizeOfZeroFill;
    union {
        ULONG Characteristics;
        struct {
            INT Reserved0 : 20;
            INT Alignment : 4;
            INT Reserved1 : 8;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

} IMAGE_TLS_DIRECTORY32;
typedef IMAGE_TLS_DIRECTORY32* PIMAGE_TLS_DIRECTORY32;


#define ACTCTX_FLAG_PROCESSOR_ARCHITECTURE_VALID    (0x00000001)
#define ACTCTX_FLAG_LANGID_VALID                    (0x00000002)
#define ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID        (0x00000004)
#define ACTCTX_FLAG_RESOURCE_NAME_VALID             (0x00000008)
#define ACTCTX_FLAG_SET_PROCESS_DEFAULT             (0x00000010)
#define ACTCTX_FLAG_APPLICATION_NAME_VALID          (0x00000020)
#define ACTCTX_FLAG_SOURCE_IS_ASSEMBLYREF           (0x00000040)
#define ACTCTX_FLAG_HMODULE_VALID                   (0x00000080)

typedef struct tagACTCTXW
{
    ULONG  cbSize;
    ULONG  dwFlags;
    PWCH   lpSource;
    USHORT wProcessorArchitecture;
    USHORT wLangId;
    PWCH   lpAssemblyDirectory;
    PWCH   lpResourceName;
    PWCH   lpApplicationName;
    PVOID  hModule;
} ACTCTXW, * PACTCTXW;

typedef struct tagACTCTXW32
{
    ULONG  cbSize;
    ULONG  dwFlags;
    ULONG  lpSource;
    USHORT wProcessorArchitecture;
    USHORT wLangId;
    ULONG  lpAssemblyDirectory;
    ULONG  lpResourceName;
    ULONG  lpApplicationName;
    ULONG  hModule;
} ACTCTXW32, * PACTCTXW32;

#pragma warning (default : 4201)

#pragma once

//
// Native structures W10 technical preview x64, build 9841
//
#pragma warning(disable : 4214 4201)
#pragma pack(push, 1)

typedef struct _MM_AVL_NODE // Size=24
{
    struct _MM_AVL_NODE* LeftChild; // Size=8 Offset=0
    struct _MM_AVL_NODE* RightChild; // Size=8 Offset=8

    union ___unnamed1666 // Size=8
    {
        struct
        {
            __int64 Balance : 2; // Size=8 Offset=0 BitOffset=0 BitCount=2
        };
        struct _MM_AVL_NODE* Parent; // Size=8 Offset=0
    } u1;
} MM_AVL_NODE, * PMM_AVL_NODE, * PMMADDRESS_NODE;

typedef struct _RTL_AVL_TREE // Size=8
{
    PMM_AVL_NODE BalancedRoot;
    void* NodeHint;
    unsigned __int64 NumberGenericTableElements;
} RTL_AVL_TREE, * PRTL_AVL_TREE, MM_AVL_TABLE, * PMM_AVL_TABLE;

union _EX_PUSH_LOCK // Size=8
{
    struct
    {
        unsigned __int64 Locked : 1; // Size=8 Offset=0 BitOffset=0 BitCount=1
        unsigned __int64 Waiting : 1; // Size=8 Offset=0 BitOffset=1 BitCount=1
        unsigned __int64 Waking : 1; // Size=8 Offset=0 BitOffset=2 BitCount=1
        unsigned __int64 MultipleShared : 1; // Size=8 Offset=0 BitOffset=3 BitCount=1
        unsigned __int64 Shared : 60; // Size=8 Offset=0 BitOffset=4 BitCount=60
    };
    unsigned __int64 Value; // Size=8 Offset=0
    void* Ptr; // Size=8 Offset=0
};

struct _MMVAD_FLAGS // Size=4
{
    unsigned long VadType : 3; // Size=4 Offset=0 BitOffset=0 BitCount=3
    unsigned long Protection : 5; // Size=4 Offset=0 BitOffset=3 BitCount=5
    unsigned long PreferredNode : 6; // Size=4 Offset=0 BitOffset=8 BitCount=6
    unsigned long NoChange : 1; // Size=4 Offset=0 BitOffset=14 BitCount=1
    unsigned long PrivateMemory : 1; // Size=4 Offset=0 BitOffset=15 BitCount=1
    unsigned long Teb : 1; // Size=4 Offset=0 BitOffset=16 BitCount=1
    unsigned long PrivateFixup : 1; // Size=4 Offset=0 BitOffset=17 BitCount=1
    unsigned long ManySubsections : 1; // Size=4 Offset=0 BitOffset=18 BitCount=1
    unsigned long Spare : 12; // Size=4 Offset=0 BitOffset=19 BitCount=12
    unsigned long DeleteInProgress : 1; // Size=4 Offset=0 BitOffset=31 BitCount=1
};
struct _MMVAD_FLAGS1 // Size=4
{
    unsigned long CommitCharge : 31; // Size=4 Offset=0 BitOffset=0 BitCount=31
    unsigned long MemCommit : 1; // Size=4 Offset=0 BitOffset=31 BitCount=1
};

struct _MMVAD_FLAGS2 // Size=4
{
    unsigned long FileOffset : 24; // Size=4 Offset=0 BitOffset=0 BitCount=24
    unsigned long Large : 1; // Size=4 Offset=0 BitOffset=24 BitCount=1
    unsigned long TrimBehind : 1; // Size=4 Offset=0 BitOffset=25 BitCount=1
    unsigned long Inherit : 1; // Size=4 Offset=0 BitOffset=26 BitCount=1
    unsigned long CopyOnWrite : 1; // Size=4 Offset=0 BitOffset=27 BitCount=1
    unsigned long NoValidationNeeded : 1; // Size=4 Offset=0 BitOffset=28 BitCount=1
    unsigned long Spare : 3; // Size=4 Offset=0 BitOffset=29 BitCount=3
};

struct _MI_VAD_SEQUENTIAL_INFO // Size=8
{
    unsigned __int64 Length : 12; // Size=8 Offset=0 BitOffset=0 BitCount=12
    unsigned __int64 Vpn : 52; // Size=8 Offset=0 BitOffset=12 BitCount=52
};

union ___unnamed1951 // Size=4
{
    unsigned long LongFlags; // Size=4 Offset=0
    struct _MMVAD_FLAGS VadFlags; // Size=4 Offset=0
};

union ___unnamed1952 // Size=4
{
    unsigned long LongFlags1; // Size=4 Offset=0
    struct _MMVAD_FLAGS1 VadFlags1; // Size=4 Offset=0
};

union ___unnamed2047 // Size=4
{
    unsigned long LongFlags2; // Size=4 Offset=0
    struct _MMVAD_FLAGS2 VadFlags2; // Size=4 Offset=0
};

union ___unnamed2048 // Size=8
{
    struct _MI_VAD_SEQUENTIAL_INFO SequentialVa; // Size=8 Offset=0
    struct _MMEXTEND_INFO* ExtendedInfo; // Size=8 Offset=0
};

typedef struct _MMVAD_SHORT // Size=64
{
    union
    {
        struct _RTL_BALANCED_NODE VadNode; // Size=24 Offset=0
        struct _MMVAD_SHORT* NextVad; // Size=8 Offset=0
    };
    unsigned long StartingVpn; // Size=4 Offset=24
    unsigned long EndingVpn; // Size=4 Offset=28
    unsigned char StartingVpnHigh; // Size=1 Offset=32
    unsigned char EndingVpnHigh; // Size=1 Offset=33
    unsigned char CommitChargeHigh; // Size=1 Offset=34
    unsigned char SpareNT64VadUChar; // Size=1 Offset=35
    long ReferenceCount; // Size=4 Offset=36
    union _EX_PUSH_LOCK PushLock; // Size=8 Offset=40
    union ___unnamed1951 u; // Size=4 Offset=48
    union ___unnamed1952 u1; // Size=4 Offset=52
    struct _MI_VAD_EVENT_BLOCK* EventList; // Size=8 Offset=56
} MMVAD_SHORT, * PMMVAD_SHORT;


typedef struct _MMVAD // Size=128
{
    struct _MMVAD_SHORT Core; // Size=64 Offset=0
    union ___unnamed2047 u2; // Size=4 Offset=64
    unsigned long pad0;  // Size=4 Offset=68
    struct _SUBSECTION* Subsection; // Size=8 Offset=72
    struct _MMPTE* FirstPrototypePte; // Size=8 Offset=80
    struct _MMPTE* LastContiguousPte; // Size=8 Offset=88
    struct _LIST_ENTRY ViewLinks; // Size=16 Offset=96
    struct _EPROCESS* VadsProcess; // Size=8 Offset=112
    union ___unnamed2048 u4; // Size=8 Offset=120
    struct _FILE_OBJECT* FileObject; // Size=8 Offset=128
} MMVAD, * PMMVAD;
#pragma pack(pop)

typedef struct _HANDLE_TABLE
{
    ULONG NextHandleNeedingPool;
    long ExtraInfoPages;
    LONG_PTR TableCode;
    PEPROCESS QuotaProcess;
    LIST_ENTRY HandleTableList;
    ULONG UniqueProcessId;
    ULONG Flags;
    EX_PUSH_LOCK HandleContentionEvent;
    EX_PUSH_LOCK HandleTableLock;
    // More fields here...
} HANDLE_TABLE, * PHANDLE_TABLE;

typedef struct _API_SET_VALUE_ENTRY_10
{
    ULONG Flags;
    ULONG NameOffset;
    ULONG NameLength;
    ULONG ValueOffset;
    ULONG ValueLength;
} API_SET_VALUE_ENTRY_10, * PAPI_SET_VALUE_ENTRY_10;

typedef struct _API_SET_VALUE_ARRAY_10
{
    ULONG Flags;
    ULONG NameOffset;
    ULONG Unk;
    ULONG NameLength;
    ULONG DataOffset;
    ULONG Count;
} API_SET_VALUE_ARRAY_10, * PAPI_SET_VALUE_ARRAY_10;

typedef struct _API_SET_NAMESPACE_ENTRY_10
{
    ULONG Limit;
    ULONG Size;
} API_SET_NAMESPACE_ENTRY_10, * PAPI_SET_NAMESPACE_ENTRY_10;

typedef struct _API_SET_NAMESPACE_ARRAY_10
{
    ULONG Version;
    ULONG Size;
    ULONG Flags;
    ULONG Count;
    ULONG Start;
    ULONG End;
    ULONG Unk[2];
} API_SET_NAMESPACE_ARRAY_10, * PAPI_SET_NAMESPACE_ARRAY_10;

#pragma warning(default : 4214 4201)

#define GET_VAD_ROOT(Table) Table->BalancedRoot

#define MAKEINTRESOURCEW(i) ((PWCH)((ULONG_PTR)((USHORT)(i))))

typedef struct _SYSTEM_SERVICE_DESCRIPTOR_TABLE
{
    PULONG_PTR ServiceTableBase;
    PULONG ServiceCounterTableBase;
    ULONG_PTR NumberOfServices;
    PUCHAR ParamTableBase;
} SYSTEM_SERVICE_DESCRIPTOR_TABLE, * PSYSTEM_SERVICE_DESCRIPTOR_TABLE;

typedef NTSTATUS(NTAPI* NTPROC)();
typedef NTPROC* PNTPROC;

typedef struct _SYSTEM_SERVICE_TABLE {
    PNTPROC ServiceTable;
    PULONG CounterTable;
    ULONG ServiceLimit;
    PUCHAR ArgumentTable;
} SYSTEM_SERVICE_TABLE, * PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
    SYSTEM_SERVICE_TABLE ntoskrnl;
    SYSTEM_SERVICE_TABLE win32k;
    SYSTEM_SERVICE_TABLE Table3;
    SYSTEM_SERVICE_TABLE Table4;
}SERVICE_DESCRIPTOR_TABLE, * PSERVICE_DESCRIPTOR_TABLE;


typedef union _PS_PROTECTION
{
    UCHAR Level;
    struct
    {
        int Type : 3;
        int Audit : 1;
        int Signer : 4;
    } Flags;
} PS_PROTECTION, * PPS_PROTECTION;

typedef union _KEXECUTE_OPTIONS
{
    struct
    {
        int ExecuteDisable : 1;                     // 0x01
        int ExecuteEnable : 1;                      // 0x02
        int DisableThunkEmulation : 1;              // 0x04
        int Permanent : 1;                          // 0x08
        int ExecuteDispatchEnable : 1;              // 0x10
        int ImageDispatchEnable : 1;                // 0x20
        int DisableExceptionChainValidation : 1;    // 0x40
        int Spare : 1;
    } Flags;

    UCHAR ExecuteOptions;
} KEXECUTE_OPTIONS, * PKEXECUTE_OPTIONS;

typedef struct _EPROCESS_FLAGS2
{
    unsigned int JobNotReallyActive : 1;
    unsigned int AccountingFolded : 1;
    unsigned int NewProcessReported : 1;
    unsigned int ExitProcessReported : 1;
    unsigned int ReportCommitChanges : 1;
    unsigned int LastReportMemory : 1;
    unsigned int ForceWakeCharge : 1;
    unsigned int CrossSessionCreate : 1;
    unsigned int NeedsHandleRundown : 1;
    unsigned int RefTraceEnabled : 1;
    unsigned int DisableDynamicCode : 1;
    unsigned int EmptyJobEvaluated : 1;
    unsigned int DefaultPagePriority : 3;
    unsigned int PrimaryTokenFrozen : 1;
    unsigned int ProcessVerifierTarget : 1;
    unsigned int StackRandomizationDisabled : 1;
    unsigned int AffinityPermanent : 1;
    unsigned int AffinityUpdateEnable : 1;
    unsigned int PropagateNode : 1;
    unsigned int ExplicitAffinity : 1;
    unsigned int ProcessExecutionState : 2;
    unsigned int DisallowStrippedImages : 1;
    unsigned int HighEntropyASLREnabled : 1;
    unsigned int ExtensionPointDisable : 1;
    unsigned int ForceRelocateImages : 1;
    unsigned int ProcessStateChangeRequest : 2;
    unsigned int ProcessStateChangeInProgress : 1;
    unsigned int DisallowWin32kSystemCalls : 1;
} EPROCESS_FLAGS2, * PEPROCESS_FLAGS2;

typedef struct _MITIGATION_FLAGS
{
    unsigned int ControlFlowGuardEnabled : 1;
    unsigned int ControlFlowGuardExportSuppressionEnabled : 1;
    unsigned int ControlFlowGuardStrict : 1;
    unsigned int DisallowStrippedImages : 1;
    unsigned int ForceRelocateImages : 1;
    unsigned int HighEntropyASLREnabled : 1;
    unsigned int StackRandomizationDisabled : 1;
    unsigned int ExtensionPointDisable : 1;
    unsigned int DisableDynamicCode : 1;
    unsigned int DisableDynamicCodeAllowOptOut : 1;
    unsigned int DisableDynamicCodeAllowRemoteDowngrade : 1;
    unsigned int AuditDisableDynamicCode : 1;
    unsigned int DisallowWin32kSystemCalls : 1;
    unsigned int AuditDisallowWin32kSystemCalls : 1;
    unsigned int EnableFilteredWin32kAPIs : 1;
    unsigned int AuditFilteredWin32kAPIs : 1;
    unsigned int DisableNonSystemFonts : 1;
    unsigned int AuditNonSystemFontLoading : 1;
    unsigned int PreferSystem32Images : 1;
    unsigned int ProhibitRemoteImageMap : 1;
    unsigned int AuditProhibitRemoteImageMap : 1;
    unsigned int ProhibitLowILImageMap : 1;
    unsigned int AuditProhibitLowILImageMap : 1;
    unsigned int SignatureMitigationOptIn : 1;
    unsigned int AuditBlockNonMicrosoftBinaries : 1;
    unsigned int AuditBlockNonMicrosoftBinariesAllowStore : 1;
    unsigned int LoaderIntegrityContinuityEnabled : 1;
    unsigned int AuditLoaderIntegrityContinuity : 1;
    unsigned int EnableModuleTamperingProtection : 1;
    unsigned int EnableModuleTamperingProtectionNoInherit : 1;
    unsigned int RestrictIndirectBranchPrediction;
    unsigned int IsolateSecurityDomain;
} MITIGATION_FLAGS, * PMITIGATION_FLAGS;

typedef union _EXHANDLE
{
    struct
    {
        int TagBits : 2;
        int Index : 30;
    } u;
    void* GenericHandleOverlay;
    ULONG_PTR Value;
} EXHANDLE, * PEXHANDLE;

#pragma warning(disable : 4214 4201)

#pragma pack(push, 1)
typedef struct _POOL_HEADER // Size=16
{
    union
    {
        struct
        {
            unsigned long PreviousSize : 8; // Size=4 Offset=0 BitOffset=0 BitCount=8
            unsigned long PoolIndex : 8; // Size=4 Offset=0 BitOffset=8 BitCount=8
            unsigned long BlockSize : 8; // Size=4 Offset=0 BitOffset=16 BitCount=8
            unsigned long PoolType : 8; // Size=4 Offset=0 BitOffset=24 BitCount=8
        };
        unsigned long Ulong1; // Size=4 Offset=0
    };
    unsigned long PoolTag; // Size=4 Offset=4
    union
    {
        struct _EPROCESS* ProcessBilled; // Size=8 Offset=8
        struct
        {
            unsigned short AllocatorBackTraceIndex; // Size=2 Offset=8
            unsigned short PoolTagHash; // Size=2 Offset=10
        };
    };
} POOL_HEADER, * PPOOL_HEADER;
#pragma pack(pop)

/*
typedef struct _HANDLE_TABLE_ENTRY // Size=16
{
    union
    {
        ULONG_PTR VolatileLowValue; // Size=8 Offset=0
        ULONG_PTR LowValue; // Size=8 Offset=0
        struct _HANDLE_TABLE_ENTRY_INFO * InfoTable; // Size=8 Offset=0
        struct
        {
            ULONG_PTR Unlocked : 1; // Size=8 Offset=0 BitOffset=0 BitCount=1
            ULONG_PTR RefCnt : 16; // Size=8 Offset=0 BitOffset=1 BitCount=16
            ULONG_PTR Attributes : 3; // Size=8 Offset=0 BitOffset=17 BitCount=3
            ULONG_PTR ObjectPointerBits : 44; // Size=8 Offset=0 BitOffset=20 BitCount=44
        };
    };
    union
    {
        ULONG_PTR HighValue; // Size=8 Offset=8
        struct _HANDLE_TABLE_ENTRY * NextFreeHandleEntry; // Size=8 Offset=8
        union _EXHANDLE LeafHandleValue; // Size=8 Offset=8
        struct
        {
            ULONG GrantedAccessBits : 25; // Size=4 Offset=8 BitOffset=0 BitCount=25
            ULONG NoRightsUpgrade : 1; // Size=4 Offset=8 BitOffset=25 BitCount=1
            ULONG Spare : 6; // Size=4 Offset=8 BitOffset=26 BitCount=6
        };
    };
    ULONG TypeInfo; // Size=4 Offset=12
} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;
*/


//0x10 bytes (sizeof)
typedef union _HANDLE_TABLE_ENTRY
{
    volatile LONGLONG VolatileLowValue;                                        //0x0
    LONGLONG LowValue;                                                         //0x0
    struct
    {
        struct _HANDLE_TABLE_ENTRY_INFO* volatile InfoTable;                   //0x0
        LONGLONG HighValue;                                                        //0x8
        union _HANDLE_TABLE_ENTRY* NextFreeHandleEntry;                            //0x8
        EXHANDLE LeafHandleValue;                                      //0x8
    };
    LONGLONG RefCountField;                                                    //0x0
    ULONGLONG Unlocked : 1;                                                      //0x0
    ULONGLONG RefCnt : 16;                                                       //0x0
    ULONGLONG Attributes : 3;                                                    //0x0
    struct
    {
        ULONGLONG ObjectPointerBits : 44;                                        //0x0
        ULONG GrantedAccessBits : 25;                                                //0x8
        ULONG NoRightsUpgrade : 1;                                                   //0x8
        ULONG Spare1 : 6;                                                        //0x8
    };
    ULONG Spare2;                                                              //0xc
} HANDLE_TABLE_ENTRY, * PHANDLE_TABLE_ENTRY;



typedef struct _OBJECT_HEADER // Size=56
{
    ULONG_PTR PointerCount; // Size=8 Offset=0
    union
    {
        ULONG_PTR HandleCount; // Size=8 Offset=8
        void* NextToFree; // Size=8 Offset=8
    };
    void* Lock; // Size=8 Offset=16
    UCHAR TypeIndex; // Size=1 Offset=24
    union
    {
        UCHAR TraceFlags; // Size=1 Offset=25
        struct
        {
            UCHAR DbgRefTrace : 1; // Size=1 Offset=25 BitOffset=0 BitCount=1
            UCHAR DbgTracePermanent : 1; // Size=1 Offset=25 BitOffset=1 BitCount=1
        };
    };
    UCHAR InfoMask; // Size=1 Offset=26
    union
    {
        UCHAR Flags; // Size=1 Offset=27
        struct
        {
            UCHAR NewObject : 1; // Size=1 Offset=27 BitOffset=0 BitCount=1
            UCHAR KernelObject : 1; // Size=1 Offset=27 BitOffset=1 BitCount=1
            UCHAR KernelOnlyAccess : 1; // Size=1 Offset=27 BitOffset=2 BitCount=1
            UCHAR ExclusiveObject : 1; // Size=1 Offset=27 BitOffset=3 BitCount=1
            UCHAR PermanentObject : 1; // Size=1 Offset=27 BitOffset=4 BitCount=1
            UCHAR DefaultSecurityQuota : 1; // Size=1 Offset=27 BitOffset=5 BitCount=1
            UCHAR SingleHandleEntry : 1; // Size=1 Offset=27 BitOffset=6 BitCount=1
            UCHAR DeletedInline : 1; // Size=1 Offset=27 BitOffset=7 BitCount=1
        };
    };
    ULONG Spare; // Size=4 Offset=28
    union
    {
        struct _OBJECT_CREATE_INFORMATION* ObjectCreateInfo; // Size=8 Offset=32
        void* QuotaBlockCharged; // Size=8 Offset=32
    };
    void* SecurityDescriptor; // Size=8 Offset=40
    struct _QUAD Body; // Size=8 Offset=48
} OBJECT_HEADER, * POBJECT_HEADER;

typedef union _EX_FAST_REF // Size=8
{
    void* Object;
    struct
    {
        unsigned __int64 RefCnt : 4;
    };
    unsigned __int64 Value;
} EX_FAST_REF, * PEX_FAST_REF;

typedef struct _CONTROL_AREA // Size=120
{
    struct _SEGMENT* Segment;
    struct _LIST_ENTRY ListHead;
    unsigned __int64 NumberOfSectionReferences;
    unsigned __int64 NumberOfPfnReferences;
    unsigned __int64 NumberOfMappedViews;
    unsigned __int64 NumberOfUserReferences;
    unsigned long f1;
    unsigned long f2;
    EX_FAST_REF FilePointer;
    // Other fields
} CONTROL_AREA, * PCONTROL_AREA;

typedef struct _SUBSECTION // Size=56
{
    PCONTROL_AREA ControlArea;
    // Other fields
} SUBSECTION, * PSUBSECTION;

typedef struct _MEMORY_BASIC_INFORMATION_EX
{
    PVOID BaseAddress;
    PVOID AllocationBase;
    ULONG AllocationProtect;
    SIZE_T RegionSize;
    ULONG State;
    ULONG Protect;
    ULONG Type;
} MEMORY_BASIC_INFORMATION_EX, * PMEMORY_BASIC_INFORMATION_EX;

typedef struct _SYSTEM_CALL_COUNT_INFORMATION
{
    ULONG Length;
    ULONG NumberOfTables;
    ULONG limits[2];
} SYSTEM_CALL_COUNT_INFORMATION, * PSYSTEM_CALL_COUNT_INFORMATION;

typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    LONG BasePriority;
    ULONG ContextSwitches;
    ULONG ThreadState;
    KWAIT_REASON WaitReason;
}SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PVOID TebBaseAddress;
    CLIENT_ID ClientId;
    ULONG_PTR AffinityMask;
    LONG Priority;
    LONG BasePriority;
} THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFO
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize;
    ULONG HardFaultCount;
    ULONG NumberOfThreadsHighWatermark;
    ULONGLONG CycleTime;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR UniqueProcessKey;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    SYSTEM_THREAD_INFORMATION Threads[1];
}SYSTEM_PROCESS_INFO, * PSYSTEM_PROCESS_INFO;

typedef struct _SYSTEM_MODULE
{
    ULONG_PTR Reserved[2];
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT Unknown;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    CHAR ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;
typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG_PTR ulModuleCount;
    SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

#pragma warning(disable : 4214)
typedef struct _MMPTE_HARDWARE64
{
    ULONGLONG Valid : 1;
    ULONGLONG Dirty1 : 1;
    ULONGLONG Owner : 1;
    ULONGLONG WriteThrough : 1;
    ULONGLONG CacheDisable : 1;
    ULONGLONG Accessed : 1;
    ULONGLONG Dirty : 1;
    ULONGLONG LargePage : 1;
    ULONGLONG Global : 1;
    ULONGLONG CopyOnWrite : 1;
    ULONGLONG Unused : 1;
    ULONGLONG Write : 1;
    ULONGLONG PageFrameNumber : 36;
    ULONGLONG reserved1 : 4;
    ULONGLONG SoftwareWsIndex : 11;
    ULONGLONG NoExecute : 1;
} MMPTE_HARDWARE64, * PMMPTE_HARDWARE64;

typedef struct _MMPTE
{
    union
    {
        ULONG_PTR Long;
        MMPTE_HARDWARE64 Hard;
    } u;
} MMPTE;
typedef MMPTE* PMMPTE;

#pragma warning(default : 4214)

typedef struct _NT_PROC_THREAD_ATTRIBUTE_ENTRY
{
    ULONG Attribute;    // PROC_THREAD_ATTRIBUTE_XXX
    SIZE_T Size;
    ULONG_PTR Value;
    ULONG Unknown;
} NT_PROC_THREAD_ATTRIBUTE_ENTRY, * NT_PPROC_THREAD_ATTRIBUTE_ENTRY;

typedef struct _NT_PROC_THREAD_ATTRIBUTE_LIST
{
    ULONG Length;
    NT_PROC_THREAD_ATTRIBUTE_ENTRY Entry[1];
} NT_PROC_THREAD_ATTRIBUTE_LIST, * PNT_PROC_THREAD_ATTRIBUTE_LIST;


typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;         // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[MAXIMUM_FILENAME_LENGTH];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

#pragma warning(disable : 4214)
typedef union _MEMORY_WORKING_SET_EX_BLOCK
{
    ULONG_PTR Flags;
    struct
    {
        ULONG_PTR Valid : 1;
        ULONG_PTR ShareCount : 3;
        ULONG_PTR Win32Protection : 11;
        ULONG_PTR Shared : 1;
        ULONG_PTR Node : 6;
        ULONG_PTR Locked : 1;
        ULONG_PTR LargePage : 1;
        ULONG_PTR Reserved : 7;
        ULONG_PTR Bad : 1;

#if defined(_WIN64)
        ULONG_PTR ReservedUlong : 32;
#endif
    };
} MEMORY_WORKING_SET_EX_BLOCK, * PMEMORY_WORKING_SET_EX_BLOCK;

typedef struct _MEMORY_WORKING_SET_EX_INFORMATION
{
    PVOID VirtualAddress;
    MEMORY_WORKING_SET_EX_BLOCK VirtualAttributes;
} MEMORY_WORKING_SET_EX_INFORMATION, * PMEMORY_WORKING_SET_EX_INFORMATION;

#pragma warning(default : 4214)


typedef struct _PEB_LDR_DATA
{
    ULONG Length;
    UCHAR Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


typedef struct _PEB
{
    UCHAR InheritedAddressSpace;
    UCHAR ReadImageFileExecOptions;
    UCHAR BeingDebugged;
    UCHAR BitField;
    PVOID Mutant;
    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
    PVOID ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PVOID FastPebLock;
    PVOID AtlThunkSListPtr;
    PVOID IFEOKey;
    PVOID CrossProcessFlags;
    PVOID KernelCallbackTable;
    ULONG SystemReserved;
    ULONG AtlThunkSListPtr32;
    PVOID ApiSetMap;
} PEB, * PPEB;

typedef struct _PEB_LDR_DATA32
{
    ULONG Length;
    UCHAR Initialized;
    ULONG SsHandle;
    LIST_ENTRY32 InLoadOrderModuleList;
    LIST_ENTRY32 InMemoryOrderModuleList;
    LIST_ENTRY32 InInitializationOrderModuleList;
} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
    LIST_ENTRY32 InLoadOrderLinks;
    LIST_ENTRY32 InMemoryOrderLinks;
    LIST_ENTRY32 InInitializationOrderLinks;
    ULONG DllBase;
    ULONG EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING32 FullDllName;
    UNICODE_STRING32 BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    LIST_ENTRY32 HashLinks;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

typedef struct _PEB32
{
    UCHAR InheritedAddressSpace;
    UCHAR ReadImageFileExecOptions;
    UCHAR BeingDebugged;
    UCHAR BitField;
    ULONG Mutant;
    ULONG ImageBaseAddress;
    ULONG Ldr;
    ULONG ProcessParameters;
    ULONG SubSystemData;
    ULONG ProcessHeap;
    ULONG FastPebLock;
    ULONG AtlThunkSListPtr;
    ULONG IFEOKey;
    ULONG CrossProcessFlags;
    ULONG UserSharedInfoPtr;
    ULONG SystemReserved;
    ULONG AtlThunkSListPtr32;
    ULONG ApiSetMap;
} PEB32, * PPEB32;

typedef struct _WOW64_PROCESS
{
    PPEB32 Wow64;
} WOW64_PROCESS, * PWOW64_PROCESS;

typedef union _WOW64_APC_CONTEXT
{
    struct
    {
        ULONG Apc32BitContext;
        ULONG Apc32BitRoutine;
    };

    PVOID Apc64BitContext;

} WOW64_APC_CONTEXT, * PWOW64_APC_CONTEXT;

typedef struct _NON_PAGED_DEBUG_INFO
{
    USHORT      Signature;
    USHORT      Flags;
    ULONG       Size;
    USHORT      Machine;
    USHORT      Characteristics;
    ULONG       TimeDateStamp;
    ULONG       CheckSum;
    ULONG       SizeOfImage;
    ULONGLONG   ImageBase;
} NON_PAGED_DEBUG_INFO, * PNON_PAGED_DEBUG_INFO;

typedef struct _KLDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    // ULONG padding on IA64
    PVOID GpValue;
    PNON_PAGED_DEBUG_INFO NonPagedDebugInfo;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT __Unused5;
    PVOID SectionPointer;
    ULONG CheckSum;
    // ULONG padding on IA64
    PVOID LoadedImports;
    PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;


//
// This structure is used by the debugger for all targets
// It is the same size as DBGKD_DATA_HEADER on all systems
//
typedef struct _DBGKD_DEBUG_DATA_HEADER64 {

    //
    // Link to other blocks
    //

    LIST_ENTRY64 List;

    //
    // This is a unique tag to identify the owner of the block.
    // If your component only uses one pool tag, use it for this, too.
    //

    ULONG           OwnerTag;

    //
    // This must be initialized to the size of the data block,
    // including this structure.
    //

    ULONG           Size;

} DBGKD_DEBUG_DATA_HEADER64, * PDBGKD_DEBUG_DATA_HEADER64;


//
// This structure is the same size on all systems.  The only field
// which must be translated by the debugger is Header.List.
//

//
// DO NOT ADD OR REMOVE FIELDS FROM THE MIDDLE OF THIS STRUCTURE!!!
//
// If you remove a field, replace it with an "unused" placeholder.
// Do not reuse fields until there has been enough time for old debuggers
// and extensions to age out.
//
typedef struct _KDDEBUGGER_DATA64 {

    DBGKD_DEBUG_DATA_HEADER64 Header;

    //
    // Base address of kernel image
    //

    ULONG64   KernBase;

    //
    // DbgBreakPointWithStatus is a function which takes an argument
    // and hits a breakpoint.  This field contains the address of the
    // breakpoint instruction.  When the debugger sees a breakpoint
    // at this address, it may retrieve the argument from the first
    // argument register, or on x86 the eax register.
    //

    ULONG64   BreakpointWithStatus;       // address of breakpoint

    //
    // Address of the saved context record during a bugcheck
    //
    // N.B. This is an automatic in KeBugcheckEx's frame, and
    // is only valid after a bugcheck.
    //

    ULONG64   SavedContext;

    //
    // help for walking stacks with user callbacks:
    //

    //
    // The address of the thread structure is provided in the
    // WAIT_STATE_CHANGE packet.  This is the offset from the base of
    // the thread structure to the pointer to the kernel stack frame
    // for the currently active usermode callback.
    //

    USHORT  ThCallbackStack;            // offset in thread data

    //
    // these values are offsets into that frame:
    //

    USHORT  NextCallback;               // saved pointer to next callback frame
    USHORT  FramePointer;               // saved frame pointer

    //
    // pad to a quad boundary
    //
    USHORT  PaeEnabled;

    //
    // Address of the kernel callout routine.
    //

    ULONG64   KiCallUserMode;             // kernel routine

    //
    // Address of the usermode entry point for callbacks.
    //

    ULONG64   KeUserCallbackDispatcher;   // address in ntdll


    //
    // Addresses of various kernel data structures and lists
    // that are of interest to the kernel debugger.
    //

    ULONG64   PsLoadedModuleList;
    ULONG64   PsActiveProcessHead;
    ULONG64   PspCidTable;

    ULONG64   ExpSystemResourcesList;
    ULONG64   ExpPagedPoolDescriptor;
    ULONG64   ExpNumberOfPagedPools;

    ULONG64   KeTimeIncrement;
    ULONG64   KeBugCheckCallbackListHead;
    ULONG64   KiBugcheckData;

    ULONG64   IopErrorLogListHead;

    ULONG64   ObpRootDirectoryObject;
    ULONG64   ObpTypeObjectType;

    ULONG64   MmSystemCacheStart;
    ULONG64   MmSystemCacheEnd;
    ULONG64   MmSystemCacheWs;

    ULONG64   MmPfnDatabase;
    ULONG64   MmSystemPtesStart;
    ULONG64   MmSystemPtesEnd;
    ULONG64   MmSubsectionBase;
    ULONG64   MmNumberOfPagingFiles;

    ULONG64   MmLowestPhysicalPage;
    ULONG64   MmHighestPhysicalPage;
    ULONG64   MmNumberOfPhysicalPages;

    ULONG64   MmMaximumNonPagedPoolInBytes;
    ULONG64   MmNonPagedSystemStart;
    ULONG64   MmNonPagedPoolStart;
    ULONG64   MmNonPagedPoolEnd;

    ULONG64   MmPagedPoolStart;
    ULONG64   MmPagedPoolEnd;
    ULONG64   MmPagedPoolInformation;
    ULONG64   MmPageSize;

    ULONG64   MmSizeOfPagedPoolInBytes;

    ULONG64   MmTotalCommitLimit;
    ULONG64   MmTotalCommittedPages;
    ULONG64   MmSharedCommit;
    ULONG64   MmDriverCommit;
    ULONG64   MmProcessCommit;
    ULONG64   MmPagedPoolCommit;
    ULONG64   MmExtendedCommit;

    ULONG64   MmZeroedPageListHead;
    ULONG64   MmFreePageListHead;
    ULONG64   MmStandbyPageListHead;
    ULONG64   MmModifiedPageListHead;
    ULONG64   MmModifiedNoWritePageListHead;
    ULONG64   MmAvailablePages;
    ULONG64   MmResidentAvailablePages;

    ULONG64   PoolTrackTable;
    ULONG64   NonPagedPoolDescriptor;

    ULONG64   MmHighestUserAddress;
    ULONG64   MmSystemRangeStart;
    ULONG64   MmUserProbeAddress;

    ULONG64   KdPrintCircularBuffer;
    ULONG64   KdPrintCircularBufferEnd;
    ULONG64   KdPrintWritePointer;
    ULONG64   KdPrintRolloverCount;

    ULONG64   MmLoadedUserImageList;

    // NT 5.1 Addition

    ULONG64   NtBuildLab;
    ULONG64   KiNormalSystemCall;

    // NT 5.0 hotfix addition

    ULONG64   KiProcessorBlock;
    ULONG64   MmUnloadedDrivers;
    ULONG64   MmLastUnloadedDriver;
    ULONG64   MmTriageActionTaken;
    ULONG64   MmSpecialPoolTag;
    ULONG64   KernelVerifier;
    ULONG64   MmVerifierData;
    ULONG64   MmAllocatedNonPagedPool;
    ULONG64   MmPeakCommitment;
    ULONG64   MmTotalCommitLimitMaximum;
    ULONG64   CmNtCSDVersion;

    // NT 5.1 Addition

    ULONG64   MmPhysicalMemoryBlock;
    ULONG64   MmSessionBase;
    ULONG64   MmSessionSize;
    ULONG64   MmSystemParentTablePage;

    // Server 2003 addition

    ULONG64   MmVirtualTranslationBase;

    USHORT    OffsetKThreadNextProcessor;
    USHORT    OffsetKThreadTeb;
    USHORT    OffsetKThreadKernelStack;
    USHORT    OffsetKThreadInitialStack;

    USHORT    OffsetKThreadApcProcess;
    USHORT    OffsetKThreadState;
    USHORT    OffsetKThreadBStore;
    USHORT    OffsetKThreadBStoreLimit;

    USHORT    SizeEProcess;
    USHORT    OffsetEprocessPeb;
    USHORT    OffsetEprocessParentCID;
    USHORT    OffsetEprocessDirectoryTableBase;

    USHORT    SizePrcb;
    USHORT    OffsetPrcbDpcRoutine;
    USHORT    OffsetPrcbCurrentThread;
    USHORT    OffsetPrcbMhz;

    USHORT    OffsetPrcbCpuType;
    USHORT    OffsetPrcbVendorString;
    USHORT    OffsetPrcbProcStateContext;
    USHORT    OffsetPrcbNumber;

    USHORT    SizeEThread;

    ULONG64   KdPrintCircularBufferPtr;
    ULONG64   KdPrintBufferSize;

    ULONG64   KeLoaderBlock;

    USHORT    SizePcr;
    USHORT    OffsetPcrSelfPcr;
    USHORT    OffsetPcrCurrentPrcb;
    USHORT    OffsetPcrContainedPrcb;

    USHORT    OffsetPcrInitialBStore;
    USHORT    OffsetPcrBStoreLimit;
    USHORT    OffsetPcrInitialStack;
    USHORT    OffsetPcrStackLimit;

    USHORT    OffsetPrcbPcrPage;
    USHORT    OffsetPrcbProcStateSpecialReg;
    USHORT    GdtR0Code;
    USHORT    GdtR0Data;

    USHORT    GdtR0Pcr;
    USHORT    GdtR3Code;
    USHORT    GdtR3Data;
    USHORT    GdtR3Teb;

    USHORT    GdtLdt;
    USHORT    GdtTss;
    USHORT    Gdt64R3CmCode;
    USHORT    Gdt64R3CmTeb;

    ULONG64   IopNumTriageDumpDataBlocks;
    ULONG64   IopTriageDumpDataBlocks;

    // Longhorn addition

    ULONG64   VfCrashDataBlock;
    ULONG64   MmBadPagesDetected;
    ULONG64   MmZeroedPageSingleBitErrorsDetected;

    // Windows 7 addition

    ULONG64   EtwpDebuggerData;
    USHORT    OffsetPrcbContext;

    // Windows 8 addition

    USHORT    OffsetPrcbMaxBreakpoints;
    USHORT    OffsetPrcbMaxWatchpoints;

    ULONG     OffsetKThreadStackLimit;
    ULONG     OffsetKThreadStackBase;
    ULONG     OffsetKThreadQueueListEntry;
    ULONG     OffsetEThreadIrpList;

    USHORT    OffsetPrcbIdleThread;
    USHORT    OffsetPrcbNormalDpcState;
    USHORT    OffsetPrcbDpcStack;
    USHORT    OffsetPrcbIsrStack;

    USHORT    SizeKDPC_STACK_FRAME;

    // Windows 8.1 Addition

    USHORT    OffsetKPriQueueThreadListHead;
    USHORT    OffsetKThreadWaitReason;

    // Windows 10 RS1 Addition

    USHORT    Padding;
    ULONG64   PteBase;

    // Windows 10 RS5 Addition

    ULONG64 RetpolineStubFunctionTable;
    ULONG RetpolineStubFunctionTableSize;
    ULONG RetpolineStubOffset;
    ULONG RetpolineStubSize;

} KDDEBUGGER_DATA64, * PKDDEBUGGER_DATA64;


typedef struct _DUMP_HEADER
{
    ULONG Signature;
    ULONG ValidDump;
    ULONG MajorVersion;
    ULONG MinorVersion;
    ULONG_PTR DirectoryTableBase;
    ULONG_PTR PfnDataBase;
    PLIST_ENTRY PsLoadedModuleList;
    PLIST_ENTRY PsActiveProcessHead;
    ULONG MachineImageType;
    ULONG NumberProcessors;
    ULONG BugCheckCode;
    ULONG_PTR BugCheckParameter1;
    ULONG_PTR BugCheckParameter2;
    ULONG_PTR BugCheckParameter3;
    ULONG_PTR BugCheckParameter4;
    CHAR VersionUser[32];
    struct _KDDEBUGGER_DATA64* KdDebuggerDataBlock;
} DUMP_HEADER, * PDUMP_HEADER;

/*
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, Signature ) == 0 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, ValidDump ) == 4 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, MajorVersion ) == 8 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, MinorVersion ) == 0xc );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, DirectoryTableBase ) == 0x10 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, PfnDataBase ) == 0x18 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, PsLoadedModuleList ) == 0x20 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, PsActiveProcessHead ) == 0x28 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, MachineImageType ) == 0x30 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, NumberProcessors ) == 0x34 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, BugCheckCode ) == 0x38 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, BugCheckParameter1 ) == 0x40 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, BugCheckParameter2 ) == 0x48 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, BugCheckParameter3 ) == 0x50 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, BugCheckParameter4 ) == 0x58 );
C_ASSERT( FIELD_OFFSET( DUMP_HEADER, KdDebuggerDataBlock ) == 0x80 );
*/

extern KDDEBUGGER_DATA64 g_KdBlock;


EXTERN_C_START

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    IN PVOID SystemInformation,
    IN ULONG SystemInformationLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    IN  PULONG ReturnLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationThread(
    IN HANDLE ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress, OPTIONAL
    IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation,
    IN SIZE_T MemoryInformationLength,
    OUT PSIZE_T ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWriteVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _In_ PVOID BaseAddress,
    _In_reads_bytes_(BufferSize) PVOID Buffer,
    _In_ SIZE_T BufferSize,
    _Out_opt_ PSIZE_T NumberOfBytesWritten
);

NTSYSAPI
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID* BaseAddress,
    _Inout_ PSIZE_T NumberOfBytesToProtect,
    _In_ ULONG NewAccessProtection,
    _Out_ PULONG OldAccessProtection
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushInstructionCache(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID* BaseAddress,
    _Inout_ PSIZE_T Length
);



NTSTATUS
NTAPI
ZwCreateThreadEx(
    OUT PHANDLE hThread,
    IN ACCESS_MASK DesiredAccess,
    IN PVOID ObjectAttributes,
    IN HANDLE ProcessHandle,
    IN PVOID lpStartAddress,
    IN PVOID lpParameter,
    IN ULONG Flags,
    IN SIZE_T StackZeroBits,
    IN SIZE_T SizeOfStackCommit,
    IN SIZE_T SizeOfStackReserve,
    IN PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList
);

NTSTATUS
NTAPI
ZwTerminateThread(
    IN HANDLE ThreadHandle,
    IN NTSTATUS ExitStatus
);



NTKERNELAPI
NTSTATUS
NTAPI
MmCopyVirtualMemory(
    IN PEPROCESS FromProcess,
    IN PVOID FromAddress,
    IN PEPROCESS ToProcess,
    OUT PVOID ToAddress,
    IN SIZE_T BufferSize,
    IN KPROCESSOR_MODE PreviousMode,
    OUT PSIZE_T NumberOfBytesCopied
);

NTKERNELAPI
PPEB
NTAPI
PsGetProcessPeb(IN PEPROCESS Process);

NTKERNELAPI
PVOID
NTAPI
PsGetThreadTeb(IN PETHREAD Thread);

NTKERNELAPI
PVOID
NTAPI
PsGetProcessWow64Process(IN PEPROCESS Process);

NTKERNELAPI
PVOID
NTAPI
PsGetCurrentProcessWow64Process();

NTKERNELAPI
BOOLEAN
NTAPI
KeTestAlertThread(IN KPROCESSOR_MODE AlertMode);

NTKERNELAPI
BOOLEAN
NTAPI
PsIsProtectedProcess(IN PEPROCESS Process);

typedef VOID(NTAPI* PKNORMAL_ROUTINE)(
    PVOID NormalContext,
    PVOID SystemArgument1,
    PVOID SystemArgument2
    );

typedef VOID(NTAPI* PKKERNEL_ROUTINE)(
    PRKAPC Apc,
    PKNORMAL_ROUTINE* NormalRoutine,
    PVOID* NormalContext,
    PVOID* SystemArgument1,
    PVOID* SystemArgument2
    );

typedef VOID(NTAPI* PKRUNDOWN_ROUTINE)(PRKAPC Apc);

NTKERNELAPI
VOID
NTAPI
KeInitializeApc(
    IN PKAPC Apc,
    IN PKTHREAD Thread,
    IN KAPC_ENVIRONMENT ApcStateIndex,
    IN PKKERNEL_ROUTINE KernelRoutine,
    IN PKRUNDOWN_ROUTINE RundownRoutine,
    IN PKNORMAL_ROUTINE NormalRoutine,
    IN KPROCESSOR_MODE ApcMode,
    IN PVOID NormalContext
);

NTKERNELAPI
BOOLEAN
NTAPI
KeInsertQueueApc(
    PKAPC Apc,
    PVOID SystemArgument1,
    PVOID SystemArgument2,
    KPRIORITY Increment
);

NTSYSAPI
PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(PVOID Base);

NTSYSAPI
PVOID
NTAPI
RtlImageDirectoryEntryToData(
    PVOID ImageBase,
    BOOLEAN MappedAsImage,
    USHORT DirectoryEntry,
    PULONG Size
);


typedef BOOLEAN(*EX_ENUMERATE_HANDLE_ROUTINE)(
#if !defined(_WIN7_)
    IN PHANDLE_TABLE HandleTable,
#endif
    IN PHANDLE_TABLE_ENTRY HandleTableEntry,
    IN HANDLE Handle,
    IN PVOID EnumParameter
    );

NTKERNELAPI
BOOLEAN
ExEnumHandleTable(
    IN PHANDLE_TABLE HandleTable,
    IN EX_ENUMERATE_HANDLE_ROUTINE EnumHandleProcedure,
    IN PVOID EnumParameter,
    OUT PHANDLE Handle
);

NTKERNELAPI
VOID
FASTCALL
ExfUnblockPushLock(
    IN OUT PEX_PUSH_LOCK PushLock,
    IN OUT PVOID WaitBlock
);

NTKERNELAPI
PCHAR
PsGetProcessImageFileName(
    __in PEPROCESS Process
);

NTSYSAPI
NTSTATUS
NTAPI
NtTraceControl(
    _In_ ULONG FunctionCode,
    _In_reads_bytes_opt_(InBufferLen) PVOID InBuffer,
    _In_ ULONG InBufferLen,
    _Out_writes_bytes_opt_(OutBufferLen) PVOID OutBuffer,
    _In_ ULONG OutBufferLen,
    _Out_ PULONG ReturnLength);


NTKERNELAPI
PVOID
NTAPI
RtlFindExportedRoutineByName(
    PVOID ImageBase,
    PCCH RoutineNam);


NTKERNELAPI
PVOID
NTAPI
PsGetCurrentThreadWin32Thread();


NTSYSAPI
NTSTATUS
NTAPI
ZwGetNextThread(
    __in HANDLE ProcessHandle,
    __in HANDLE ThreadHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Flags,
    __out PHANDLE NewThreadHandle);

NTKERNELAPI
PVOID
NTAPI
PsGetThreadWin32Thread(
    _In_ PETHREAD Thread
);

NTKERNELAPI
VOID
PsSetThreadWin32Thread(
    IN OUT PETHREAD Thread,
    IN PVOID Win32Thread,
    IN PVOID PrevWin32Thread
);


EXTERN_C_END