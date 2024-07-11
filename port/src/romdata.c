#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <PR/ultratypes.h>
#include <PR/ultratypes.h>
#include "game/setup.h"
#include "lib/rzip.h"
#include "romdata.h"
#include "fs.h"
#include "system.h"
#include "preprocess.h"
#include "platform.h"

/**
 * asset files and ROM segments can be replaced by optional external files,
 * but asset filenames still have to be either pulled from the ROM or from an
 * external file, so stuff can't be completely custom
 * 
 * all data is assumed to be big endian, so it has to be byteswapped
 * at load time, which is fucking terrible
 */

#define ROMDATA_FILEDIR "files"
#define ROMDATA_SEGDIR "segs"

#define ROMDATA_ROM_NAME "pd." VERSION_ROMID ".z64"
#define ROMDATA_ROM_SIZE 33554432

#if VERSION == VERSION_NTSC_FINAL
#define ROMDATA_ROM_TITLE "Perfect Dark"
#define ROMDATA_ROM_ID "NPDE"
#define ROMDATA_ROM_DESC "NTSC v1.1"
#define ROMDATA_FILES_OFS 0x28080
#define ROMDATA_DATA_OFS 0x39850
#elif VERSION == VERSION_PAL_FINAL
#define ROMDATA_ROM_TITLE "Perfect Dark"
#define ROMDATA_ROM_ID "NPDP"
#define ROMDATA_ROM_DESC "PAL"
#define ROMDATA_FILES_OFS 0x28910
#define ROMDATA_DATA_OFS 0x39850
#elif VERSION == VERSION_JPN_FINAL
#define ROMDATA_ROM_TITLE "PERFECT DARK"
#define ROMDATA_ROM_ID "NPDJ"
#define ROMDATA_ROM_DESC "JPN"
#define ROMDATA_FILES_OFS 0x28800
#define ROMDATA_DATA_OFS 0x39850
#else
#error "This ROM version is unsupported."
#endif

#define ROMDATA_MAX_FILES (2048 * 2)

u8 *g_RomFile;
u32 g_RomFileSize;
u8 *g_GexFile;
u32 g_GexFileSize;

static u8 *romDataSeg;
static u32 romDataSegSize;
static u8 *gexDataSeg;
static u32 gexDataSegSize;
static const char *romName = ROMDATA_ROM_NAME;
static const char *gexName = "gex.5e-clouds.z64";

enum loadsource {
	SRC_UNLOADED = 0,
	SRC_ROM,
	SRC_EXTERNAL
};

struct romfilepatch {
	u32 ofs;
	u32 len;
	const char *src;
	const char *dst;
};

struct romfile {
	u8 **segstart;
	u8 **segend;
	const char *name;
	u8 *data;
	u32 size;
	preprocessfunc preprocess;
	s32 source; // enum loadsource
	s32 preprocessed;
	const struct romfilepatch *patches;
	u32 numpatches;
};

/* patches for individual files; applied on file load, before preprocFuncs, but */
/* after unzip; only applied when loading from a ROM file                       */
static const struct romfilepatch filePatches[] = {
	/* FILE_USETUPLUE: fixes Jon's double "if what" in Infiltration outro */
	{ 0x92a2, 1, "\x6c", "\x99" },
	{ 0x92b0, 1, "\x6c", "\x99" },
};

static struct romfile fileSlots[ROMDATA_MAX_FILES] = {
	[FILE_USETUPLUE] = { .patches = &filePatches[0], .numpatches = 2 },
};

#define ROMSEG_START(n) _ ## n ## SegmentRomStart
#define ROMSEG_END(n) _ ## n ## SegmentRomEnd

/* segment table for ntsc-final                                                     */
/* size will get calculated automatically if it is 0                                */
/* if there are replacement files in the data dir, they will be loaded instead      */
/* offsets are specified for ntsc-final, pal-final and jpn-final in that order      */
#define ROMSEG_LIST() \
	ROMSEG_DECL_SEG(fontjpnsingle,      0x194b20,  0x180330,  0x0,       0x0,      preprocessJpnFont       ) \
	ROMSEG_DECL_SEG(fontjpnmulti,       0x19fb40,  0x18b340,  0x0,       0x0,      preprocessJpnFont       ) \
	ROMSEG_DECL_SEG(animations,         0x1a15c0,  0x18cdc0,  0x190c50,  0x0,      preprocessAnimations    ) \
	ROMSEG_DECL_SEG(mpconfigs,          0x7d0a40,  0x7bc240,  0x7c00d0,  0x11e0,   preprocessMpConfigs     ) \
	ROMSEG_DECL_SEG(mpstringsE,         0x7d1c20,  0x7bd420,  0x7c12b0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsJ,         0x7d5320,  0x7c0b20,  0x7c49b0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsP,         0x7d8a20,  0x7c4220,  0x7c80b0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsG,         0x7dc120,  0x7c7920,  0x7cb7b0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsF,         0x7df820,  0x7cb020,  0x7ceeb0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsS,         0x7e2f20,  0x7ce720,  0x7d25b0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(mpstringsI,         0x7e6620,  0x7d1e20,  0x7d5cb0,  0x3700,   NULL                    ) \
	ROMSEG_DECL_SEG(firingrange,        0x7e9d20,  0x7d5520,  0x7d93b0,  0x1550,   NULL                    ) \
	ROMSEG_DECL_SEG(fonttahoma,         0x7f7860,  0x7e3060,  0x7e6ef0,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(fontnumeric,        0x7f8b20,  0x7e4320,  0x7e81b0,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(fonthandelgothicsm, 0x7f9d30,  0x7e5530,  0x7e93c0,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(fonthandelgothicxs, 0x7fbfb0,  0x7e87b0,  0x7ec640,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(fonthandelgothicmd, 0x7fdd80,  0x7eae20,  0x7eecb0,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(fonthandelgothiclg, 0x8008e0,  0x7eee70,  0x7f2d00,  0x0,      preprocessFont          ) \
	ROMSEG_DECL_SEG(sfxctl,             0x80a250,  0x7f87e0,  0x7fc670,  0x2fb80,  preprocessALBankFile    ) \
	ROMSEG_DECL_SEG(sfxtbl,             0x839dd0,  0x828360,  0x82c1f0,  0x4c2160, NULL                    ) \
	ROMSEG_DECL_SEG(seqctl,             0xcfbf30,  0xcea4c0,  0xcee350,  0xa060,   preprocessALBankFile    ) \
	ROMSEG_DECL_SEG(seqtbl,             0xd05f90,  0xcf4520,  0xcf83b0,  0x17c070, NULL                    ) \
	ROMSEG_DECL_SEG(sequences,          0xe82000,  0xe70590,  0xe74420,  0x563a0,  preprocessSequences     ) \
	ROMSEG_DECL_SEG(texturesdata,       0x1d65f40, 0x1d5ca20, 0x1d61f90, 0x0,      NULL                    ) \
	ROMSEG_DECL_SEG(textureslist,       0x1ff7ca0, 0x1fee780, 0x1ff68f0, 0x0,      preprocessTexturesList  ) \
	ROMSEG_DECL_SEG(copyright,          0x1ffea20, 0x1ff5500, 0x1ffd6b0, 0xb30,    NULL                    ) \
	ROMSEG_DECL_SEG(fontjpn,            0x0,       0x0,       0x178c40,  0x17920,  preprocessJpnFont       )

// declare the vars first

#undef ROMSEG_DECL_SEG
#define ROMSEG_DECL_SEG(name, ofs_ntsc, ofs_pal, ofs_jpn, size, preproc) u8 *ROMSEG_START(name), *ROMSEG_END(name);
ROMSEG_LIST()

u8 *_gextexturesdataSegmentRomStart, *_gextexturesdataSegmentRomEnd;
u8 *_gextextureslistSegmentRomStart, *_gextextureslistSegmentRomEnd;

// this is part of the animations seg and as such does not follow the naming convention
// these are set in preprocessAnimations
u8 *_animationsTableRomStart;
u8 *_animationsTableRomEnd;

// then build the table

#undef ROMSEG_DECL_SEG

#if VERSION == VERSION_NTSC_FINAL
#define ROMSEG_DECL_SEG(name, ofs_ntsc, ofs_pal, ofs_jpn, size, preproc) { &ROMSEG_START(name), &ROMSEG_END(name), #name, (u8 *)ofs_ntsc, size, preproc },
#elif VERSION == VERSION_PAL_FINAL
#define ROMSEG_DECL_SEG(name, ofs_ntsc, ofs_pal, ofs_jpn, size, preproc) { &ROMSEG_START(name), &ROMSEG_END(name), #name, (u8 *)ofs_pal, size, preproc },
#elif VERSION == VERSION_JPN_FINAL
#define ROMSEG_DECL_SEG(name, ofs_ntsc, ofs_pal, ofs_jpn, size, preproc) { &ROMSEG_START(name), &ROMSEG_END(name), #name, (u8 *)ofs_jpn, size, preproc },
#endif

static struct romfile romSegs[] = {
	ROMSEG_LIST()
	{ NULL, NULL, NULL, NULL, 0, NULL },
};

static struct romfile gexSegs[] = {
	{ &_gextexturesdataSegmentRomStart, &_gextexturesdataSegmentRomEnd, "texturesdata", (u8 *)0x1b330ae, 0x0, ((void *)0) },
	{ &_gextextureslistSegmentRomStart, &_gextextureslistSegmentRomEnd, "textureslist", (u8 *)0x1ff7ca0, 0x0, preprocessTexturesList }, // This offset is wrong.
	{ NULL, NULL, NULL, NULL, 0, NULL },
};

/* the game sets g_LoadType to the type of file it expects,              */
/* so we can hijack that in fileLoad and automatically byteswap the file */
static preprocessfilefunc filePreprocFuncs[] = {
	/* LOADTYPE_NONE  */ NULL,
	/* LOADTYPE_BG    */ NULL, // loaded in parts
	/* LOADTYPE_TILES */ preprocessTilesFile,
	/* LOADTYPE_LANG  */ preprocessLangFile,
	/* LOADTYPE_SETUP */ preprocessSetupFile,
	/* LOADTYPE_PADS  */ preprocessPadsFile,
	/* LOADTYPE_MODEL */ preprocessModelFile,
	/* LOADTYPE_GUN   */ preprocessGunFile,
};

static inline void romdataWrongRomError(const char *fmt, ...)
{
	char reason[1024];
	reason[0] = '\0';

	va_list args;
	va_start(args, fmt);
	vsnprintf(reason, sizeof(reason), fmt, args);
	va_end(args);

	sysFatalError("Wrong ROM file.\n%s\nEnsure that you have the correct " ROMDATA_ROM_DESC " ROM in z64 format.", reason);
}

static inline void romdataLoadRom(void)
{
	g_RomFile = fsFileLoad(romName, &g_RomFileSize);
	g_GexFile = fsFileLoad(gexName, &g_GexFileSize);

	if (!g_RomFile) {
		sysFatalError("Could not open the Perfect Dark ROM file %s.\nEnsure that it is in the %s directory.", romName, fsFullPath(""));
	} else {
		sysLogPrintf(LOG_NOTE, "Perfect Dark ROM file: %s", romName);
	}

	if (!g_GexFile) {
		sysLogPrintf(LOG_WARNING, "Could not open the GoldenEye X ROM file %s.\nEnsure that it is in the %s directory.", gexName, fsFullPath(""));
	} else {
		sysLogPrintf(LOG_NOTE, "GoldenEye X ROM file: %s", gexName);
	}

	// zips are not guaranteed to start with PK, but might as well at least try
	if (g_RomFileSize > 2 && (!memcmp(g_RomFile, "PK", 2) || !memcmp(g_RomFile, "Rar", 3) || !memcmp(g_RomFile, "7z", 2))) {
		romdataWrongRomError("Your ROM is in an archive file. Please extract it.");
	}

	if (g_RomFileSize != ROMDATA_ROM_SIZE) {
		romdataWrongRomError("ROM size does not match: expected: %u, got: %u.", ROMDATA_ROM_SIZE, g_RomFileSize);
	}

	if (memcmp(g_RomFile + 0x3b, ROMDATA_ROM_ID, 4) || memcmp(g_RomFile + 0x20, ROMDATA_ROM_TITLE, sizeof(ROMDATA_ROM_TITLE) - 1)) {
		romdataWrongRomError("ROM header does not match.");
	}

	// inflate the compressed data segment since that's where some useful stuff is

	u8 *zipped = g_RomFile + ROMDATA_DATA_OFS;
	if (!rzipIs1173(zipped)) {
		romdataWrongRomError("Data segment is not 1173-compressed.");
	}

	u32 dataSegLen = ((u32)zipped[2] << 16) | ((u32)zipped[3] << 8) | (u32)zipped[4];
	if (dataSegLen < ROMDATA_FILES_OFS) {
		romdataWrongRomError("Data segment too small (%u), need at least %u.", dataSegLen, ROMDATA_FILES_OFS);
	}

	u8 *dataSeg = sysMemAlloc(dataSegLen);
	if (!dataSeg) {
		sysFatalError("Could not allocate %u bytes for data segment.", dataSegLen);
	}

	u8 scratch[5 * 1024];
	if (rzipInflate(zipped, dataSeg, scratch) < 0) {
		free(dataSeg);
		sysFatalError("Could not inflate data segment.");
	}

	romDataSeg = dataSeg;
	romDataSegSize = dataSegLen;

	// inflate the GEX compressed data segment
	if (g_GexFile) {
		zipped = g_GexFile + ROMDATA_DATA_OFS;
		if (!rzipIs1173(zipped)) {
			romdataWrongRomError("GEX Data segment is not 1173-compressed.");
		}

		dataSegLen = ((u32)zipped[2] << 16) | ((u32)zipped[3] << 8) | (u32)zipped[4];
		if (dataSegLen < ROMDATA_FILES_OFS) {
			romdataWrongRomError("GEX Data segment too small (%u), need at least %u.", dataSegLen, ROMDATA_FILES_OFS);
		}

		dataSeg = sysMemAlloc(dataSegLen);
		if (!dataSeg) {
			sysFatalError("Could not allocate %u bytes for GEX data segment.", dataSegLen);
		}

		if (rzipInflate(zipped, dataSeg, scratch) < 0) {
			free(dataSeg);
			sysFatalError("Could not inflate GEX data segment.");
		}

		gexDataSeg = dataSeg;
		gexDataSegSize = dataSegLen;
	}
}

static inline void romdataInitSegment(struct romfile *seg, bool gex)
{
	if (!seg->data) {
		// unused in this ROM, skip it
		sysLogPrintf(LOG_NOTE, "skipping segment %s", seg->name);
		return;
	}

	if (!seg->size) {
		// size unknown
		if (seg[1].name) {
			// use next segment's base to calculate
			seg->size = seg[1].data - seg->data;
		} else {
			// this is the last segment, calculate based on rom size
			seg->size = (u8 *)g_RomFileSize - seg->data;
		}
	}

	// check if we have an external replacement and load it if so
	char tmp[FS_MAXPATH];
	snprintf(tmp, sizeof(tmp), ROMDATA_SEGDIR "/%s", seg->name);
	u8 *newData = NULL;
	if (!gex) {
		const s32 extFileSize = fsFileSize(tmp);
		if (extFileSize > 0) {
			newData = fsFileLoad(tmp, &seg->size);
		}
	}

	if (!newData) {
		// no external data, just make it point to the rom
		if (!gex && g_RomFile) {
			newData = g_RomFile + (uintptr_t)seg->data;
			seg->source = SRC_ROM;
			sysLogPrintf(LOG_NOTE, "loading segment %s from Perfect Dark ROM (offset %08x pointer %p)", seg->name, (u32)seg->data, newData);
		} else if (gex && g_GexFile) {
			newData = g_GexFile + (uintptr_t)seg->data;
			seg->source = SRC_ROM;
			sysLogPrintf(LOG_NOTE, "loading segment %s from GoldenEye X ROM (offset %08x pointer %p)", seg->name, (u32)seg->data, newData);
		} else {
			sysFatalError("No ROM or external file for segment:\n%s", seg->name);
		}
	} else {
		// loaded external data
		seg->source = SRC_EXTERNAL;
		sysLogPrintf(LOG_NOTE, "loading segment %s from file (pointer %p)", seg->name, newData);
	}

	seg->data = newData;

	if (seg->segstart) {
		*seg->segstart = seg->data;
	}

	if (seg->segend) {
		*seg->segend = seg->data + seg->size;
	}

	// call the post load function if any
	if (seg->preprocess && !seg->preprocessed) {
		seg->preprocess(seg->data, seg->size);
		seg->preprocessed = 1;
	}
}

static inline s32 romdataLoadExternalFileList(void)
{
	romDataSeg = fsFileLoad("filenames.lst", &romDataSegSize); // this null terminates the file by itself
	if (!romDataSeg || !romDataSegSize) {
		return 0;
	}

	s32 n = 1;
	char *p = (char *)romDataSeg;
	while (*p && n < ROMDATA_MAX_FILES) {
		// skip whitespace
		while (*p && isspace(*p)) ++p;
		if (*p) {
			const char *start = p;
			// skip to next whitespace or end of file
			while (*p && !isspace(*p)) ++p;
			// null terminate the name if needed
			if (*p) {
				*p++ = '\0';
			}
			fileSlots[n++].name = start;
		}
	}

	return n - 1;
}

#define GEX_FILE_OFFSET (FILE_GEX_BG_SEV_SEG - 1)

static inline void romdataInitFiles(void)
{
	if (!g_RomFile) {
		// no ROM; try to load the file name list from disk
		if (!romdataLoadExternalFileList()) {
			sysFatalError("No ROM file or external filename table found.");
		}
		return;
	}

	// the file offset table is in the data seg
	const u32 *offsets = (u32 *)(romDataSeg + ROMDATA_FILES_OFS);
	u32 i;
	for (i = 1; offsets[i]; ++i) {
		if (offsets + i + 1 < (u32 *)(romDataSeg + romDataSegSize)) {
			const u32 nextofs = PD_BE32(offsets[i + 1]);
			const u32 ofs = PD_BE32(offsets[i]);
			fileSlots[i].data = g_RomFile + ofs;
			fileSlots[i].size = nextofs - ofs;
			fileSlots[i].source = SRC_UNLOADED;
			fileSlots[i].preprocessed = 0;
		}
	}

	// last offset is to the name table
	const u32 *nameOffsets = (u32 *)(g_RomFile + PD_BE32(offsets[i - 1]));
	for (i = 1; nameOffsets[i]; ++i) {
		const u32 ofs = PD_BE32(nameOffsets[i]);
		fileSlots[i].name = (const char *)nameOffsets + ofs; // ofs is relative to the start of the name table
	}

	if (g_GexFile) {
		// the file offset table is in the data seg
		const u32 *gexOffsets = (u32 *)(gexDataSeg + ROMDATA_FILES_OFS);
		for (i = 1; gexOffsets[i]; ++i) {
			if (gexOffsets + i + 1 < (u32 *)(gexDataSeg + gexDataSegSize)) {
				const u32 nextofs = PD_BE32(gexOffsets[i + 1]);
				const u32 ofs = PD_BE32(gexOffsets[i]);
				fileSlots[i + GEX_FILE_OFFSET].data = g_GexFile + ofs;
				fileSlots[i + GEX_FILE_OFFSET].size = nextofs - ofs;
				fileSlots[i + GEX_FILE_OFFSET].source = SRC_UNLOADED;
				fileSlots[i + GEX_FILE_OFFSET].preprocessed = 0;
			}
		}

		// last offset is to the name table
		const u32 *gexNameOffsets = (u32 *)(g_GexFile + PD_BE32(gexOffsets[i - 1]));
		for (i = 1; gexNameOffsets[i]; ++i) {
			const u32 ofs = PD_BE32(gexNameOffsets[i]);
			fileSlots[i + GEX_FILE_OFFSET].name = (const char *)gexNameOffsets + ofs; // ofs is relative to the start of the name table
		}
	}
}

static inline struct romfile *romdataGetSeg(const char *name)
{
	struct romfile *seg = romSegs;
	while (seg->name && strcmp(name, seg->name)) {
		++seg;
	}
	return seg;
}

struct textureextractiondata
{
	u32 ofs;
	// Original GEX texture number
	s32 texturenum;
	u32 size;
};

static struct textureextractiondata texturesToExtract[] = {
	{ 0x1B330AE, 0x0000,  408 },
	{ 0x1B33246, 0x0001,   15 },
	{ 0x1B33B7F, 0x0005, 1159 },
	{ 0x1B361DB, 0x000e,  837 },
	{ 0x1B370AB, 0x0014,  436 },
	{ 0x1B40335, 0x004b,  694 },
	{ 0x1B41BEE, 0x0053,  627 },
	{ 0x1B41E61, 0x0054,  599 },
	{ 0x1B420B8, 0x0055, 1267 },
	{ 0x1B4CD20, 0x0082, 1145 },
	{ 0x1B58E41, 0x00b2,  550 },
	{ 0x1B5AEB5, 0x00bc,  720 },
	{ 0x1B5EF0B, 0x00d0,  511 },
	{ 0x1B66F4A, 0x00f2,  760 },
	{ 0x1B67242, 0x00f3,  444 },
	{ 0x1B6AB78, 0x0106,  984 },
	{ 0x1B6AF50, 0x0107, 1033 },
	{ 0x1B6B359, 0x0108, 1010 },
	{ 0x1B6B74B, 0x0109, 1033 },
	{ 0x1B6BB54, 0x010a, 1090 },
	{ 0x1B6BF96, 0x010b, 1085 },
	{ 0x1B6C3D3, 0x010c, 1014 },
	{ 0x1B6C7C9, 0x010d,  270 },
	{ 0x1B6C8D7, 0x010e, 1734 },
	{ 0x1B6CF9D, 0x010f,  551 },
	{ 0x1B6F473, 0x011b,  199 },
	{ 0x1B79BD3, 0x013e,  522 },
	{ 0x1BA5873, 0x01f5,  424 },
	{ 0x1BA7CD0, 0x0203, 1532 },
	{ 0x1BA82CC, 0x0204, 1461 },
	{ 0x1BA8881, 0x0205, 1220 },
	{ 0x1BA8FA7, 0x0208,  705 },
	{ 0x1BA9268, 0x0209,  854 },
	{ 0x1BEE4DB, 0x0329,  851 },
	{ 0x1BEE82E, 0x032a,  747 },
	{ 0x1BEEB19, 0x032b,  891 },
	{ 0x1BEEE94, 0x032c,  487 },
	{ 0x1BEF07B, 0x032d,  363 },
	{ 0x1BEF3D1, 0x032f,  746 },
	{ 0x1BFEC6E, 0x0386,  321 },
	{ 0x1BFEDAF, 0x0387,  147 },
	{ 0x1BFF049, 0x0389,  215 },
	{ 0x1BFF120, 0x038a,  730 },
	{ 0x1BFF3FA, 0x038b,  839 },
	{ 0x1BFF741, 0x038c,  479 },
	{ 0x1BFF920, 0x038d,  573 },
	{ 0x1BFFD5A, 0x038f, 1325 },
	{ 0x1C05E84, 0x03ab, 1881 },
	{ 0x1C065DD, 0x03ac, 1680 },
	{ 0x1C06C6D, 0x03ad, 1636 },
	{ 0x1C072D1, 0x03ae, 1715 },
	{ 0x1C788B1, 0x05ff,  461 },
	{ 0x1C8C8A1, 0x0643,  487 },
	{ 0x1C8CA88, 0x0644,  530 },
	{ 0x1C9E425, 0x0681, 1655 },
	{ 0x1C9EA9C, 0x0682, 1854 },
	{ 0x1CB0245, 0x06e2,  669 },
	{ 0x1CB7439, 0x0701,  991 },
	{ 0x1CBADBD, 0x070f,  593 },
	{ 0x1CBB535, 0x0712,  662 },
	{ 0x1CBB7CB, 0x0713,  479 },
	{ 0x1CBB9AA, 0x0714,  536 },
	{ 0x1CBBBC2, 0x0715,  866 },
	{ 0x1CBBF24, 0x0716,  875 },
	{ 0x1CC1B0D, 0x072e,  420 },
	{ 0x1CC41F8, 0x0738,  833 },
	{ 0x1D0A1B0, 0x0864,  381 },
	{ 0x1D0A32D, 0x0865,  135 },
	{ 0x1D5FE6F, 0x0951,  694 },
	{ 0x1D60125, 0x0952,  421 },
	{ 0x1D602CA, 0x0953,  403 },
	{ 0x1D6045D, 0x0954,  690 },
	{ 0x1D6070F, 0x0955,  225 },
	{ 0x1D607F0, 0x0956,  503 },
	{ 0x1D609E7, 0x0957,  410 },
	{ 0x1D60B81, 0x0958,  554 },
	{ 0x1D60DAB, 0x0959,  748 },
	{ 0x1D6B5D7, 0x0988,  191 },
	{ 0x1D6B995, 0x098f,   15 },
	{ 0x1D8AFEB, 0x0a24, 1081 },
	{ 0x1D8B424, 0x0a25,  889 },
	{ 0x1D8DF65, 0x0a34,  138 },
	{ 0x1D8E6BB, 0x0a37,  789 },
	{ 0x1D8F0A0, 0x0a3a,  180 },
	{ 0x1D8F154, 0x0a3b,  536 },
	{ 0x1D9253E, 0x0a4a, 1795 },
	{ 0x1DD0FB5, 0x0b3c, 2098 },
	{ 0x1DD17E7, 0x0b3d, 2023 },
	{ 0x1DD1FCE, 0x0b3e, 2003 },
	{ 0x1DD27A1, 0x0b3f, 1927 },
	{ 0x1DED0BC, 0x0bbc,  655 },
	{ 0x1E17375, 0x0cc7,  216 },
	{ 0x1E1744D, 0x0cc8,  401 },
	{ 0x1E175DE, 0x0cc9,  172 },
	{ 0x1E19D92, 0x0cd3,  496 },
};

s32 romdataInit(void)
{
	const char *altRomName = sysArgGetString("--rom-file");
	if (altRomName) {
		romName = altRomName;
	}

	romdataLoadRom();

	// set segments to point to the rom or load them externally
	for (struct romfile *seg = romSegs; seg->name; ++seg) {
		romdataInitSegment(seg, false);
	}

	if (g_GexFile) {
		// set segments to point to the rom or load them externally
		for (struct romfile *seg = gexSegs; seg->name; ++seg) {
			romdataInitSegment(seg, true);
		}

		char path[FS_MAXPATH + 1];
		for (int i = 0; i < ARRAYCOUNT(texturesToExtract); i++) {
			struct textureextractiondata ted = texturesToExtract[i];
			snprintf(path, sizeof(path), "data/textures/%04x.bin", ted.texturenum + NUM_ORIG_TEXTURES);
			FILE *fp = fopen(path, "wb");
			fwrite(g_GexFile + ted.ofs, sizeof(char), ted.size, fp);
			fclose(fp);
		}
	}

	// load file table from the files segment
	romdataInitFiles();

	sysLogPrintf(LOG_NOTE, "romdataInit: loaded rom, size = %u", g_RomFileSize);

	if (g_GexFile) {
		extern struct weaponfunc_melee* invfunc_falcon2_pistolwhip;
		extern struct guncmd invanim_falcon2_equip[];
		extern struct guncmd invanim_falcon2_unequip[];
		extern struct gunviscmd gunviscmds_falcon2[];
		extern struct gunviscmd gunviscmds_falcon2silencer[];
		extern struct modelpartvisibility invpartvisibility_falcon2[];
		extern struct modelpartvisibility invpartvisibility_falcon2silencer[];
		extern struct weaponfunc_shootsingle invfunc_pp9i_shoot;
		extern struct weaponfunc_shootsingle invfunc_pp9i_shoot_silenced;
		extern struct guncmd invanim_falcon2_shoot[];	

		// patch in GEX goodies:

		/* // Muzzle flash is not working correctly.
		// PP7 model
		g_Weapons[WEAPON_PP9I]->hi_model =  FILE_GEX_GWPPK;
		g_Weapons[WEAPON_PP9I]->lo_model = FILE_GEX_GWPPK;
		//g_Weapons[WEAPON_PP9I]->functions[1] = 	&invfunc_falcon2_pistolwhip; // Bugged
		g_Weapons[WEAPON_PP9I]->equip_animation = invanim_falcon2_equip;
		g_Weapons[WEAPON_PP9I]->unequip_animation = invanim_falcon2_unequip;
		g_Weapons[WEAPON_PP9I]->gunviscmds = gunviscmds_falcon2;
		g_Weapons[WEAPON_PP9I]->partvisibility = invpartvisibility_falcon2;
		g_Weapons[WEAPON_PP9I]->muzzlez = 2;
		g_Weapons[WEAPON_PP9I]->posx = 9;
		g_Weapons[WEAPON_PP9I]->posy = -15.7;
		g_Weapons[WEAPON_PP9I]->posz = -23.8;
		g_Weapons[WEAPON_PP9I]->flags = WEAPONFLAG_00000004 | WEAPONFLAG_ONEHANDED | WEAPONFLAG_AICANUSE | WEAPONFLAG_DUALFLIP | WEAPONFLAG_00000040 | WEAPONFLAG_TRACKTIMEUSED | WEAPONFLAG_00000400 | WEAPONFLAG_DUALWIELD | WEAPONFLAG_00002000 | WEAPONFLAG_00008000 | WEAPONFLAG_HASHANDS | WEAPONFLAG_GANGSTA;
		invfunc_pp9i_shoot.base.base.fire_animation = invanim_falcon2_shoot;
		*/

		// Silenced PP7 model
		g_Weapons[WEAPON_PP9I_SILENCER]->hi_model = FILE_GEX_GWPPK;
		g_Weapons[WEAPON_PP9I_SILENCER]->lo_model = FILE_GEX_GWPPK;
		//g_Weapons[WEAPON_PP9I_SILENCER]->functions[1] = &invfunc_falcon2_pistolwhip; // Bugged
		g_Weapons[WEAPON_PP9I_SILENCER]->equip_animation = invanim_falcon2_equip;
		g_Weapons[WEAPON_PP9I_SILENCER]->unequip_animation = invanim_falcon2_unequip;
		g_Weapons[WEAPON_PP9I_SILENCER]->gunviscmds = gunviscmds_falcon2silencer;
		g_Weapons[WEAPON_PP9I_SILENCER]->partvisibility = invpartvisibility_falcon2silencer;
		g_Weapons[WEAPON_PP9I_SILENCER]->muzzlez = 2;
		g_Weapons[WEAPON_PP9I_SILENCER]->posx = 9;
		g_Weapons[WEAPON_PP9I_SILENCER]->posy = -15.7;
		g_Weapons[WEAPON_PP9I_SILENCER]->posz = -23.8;
		g_Weapons[WEAPON_PP9I_SILENCER]->flags = WEAPONFLAG_00000004 | WEAPONFLAG_ONEHANDED | WEAPONFLAG_AICANUSE | WEAPONFLAG_DUALFLIP | WEAPONFLAG_00000040 | WEAPONFLAG_TRACKTIMEUSED | WEAPONFLAG_00000400 | WEAPONFLAG_DUALWIELD | WEAPONFLAG_00002000 | WEAPONFLAG_00008000 | WEAPONFLAG_HASHANDS | WEAPONFLAG_GANGSTA;
		invfunc_pp9i_shoot_silenced.base.base.fire_animation = invanim_falcon2_shoot;
	}

	return 0;
}

s32 romdataFileGetSize(s32 fileNum)
{
	if (fileNum < 1 || fileNum >= ROMDATA_MAX_FILES) {
		sysLogPrintf(LOG_ERROR, "romdataFileGetSize: invalid file num %d", fileNum);
		return -1;
	}

	// ensure any external files are loaded and we use their size
	if (romdataFileLoad(fileNum, NULL)) {
		return fileSlots[fileNum].size;
	}

	sysLogPrintf(LOG_ERROR, "romdataFileGetSize: could not load file num %d", fileNum);
	return -1;
}

u8 *romdataFileGetData(s32 fileNum)
{
	return romdataFileLoad(fileNum, NULL);
}

u8 *romdataFileLoad(s32 fileNum, u32 *outSize)
{
	if (fileNum < 1 || fileNum >= ROMDATA_MAX_FILES) {
		sysLogPrintf(LOG_ERROR, "romdataFileLoad: invalid file num %d", fileNum);
		return NULL;
	}

	u8 *out = NULL;

	// try to load external file
	if (fileSlots[fileNum].source == SRC_UNLOADED) {
		char tmp[FS_MAXPATH] = { 0 };
		const char* filename = fileSlots[fileNum].name;
		switch (fileNum) {
			case FILE_PCARDBOARDBOX: filename = "PcardboardboxZ"; break;
		}
		snprintf(tmp, sizeof(tmp), ROMDATA_FILEDIR "/%s", filename);
		if (fsFileSize(tmp) > 0) {
			u32 size = 0;
			out = fsFileLoad(tmp, &size);
			if (out && size) {
				sysLogPrintf(LOG_NOTE, "file %d (%s) loaded externally", fileNum, filename);
				fileSlots[fileNum].data = out;
				fileSlots[fileNum].size = size;
				fileSlots[fileNum].source = SRC_EXTERNAL;
				// external file; do not apply patches to this
				fileSlots[fileNum].numpatches = 0;
			}
		}
		// tried and failed, fall back to ROM
		fileSlots[fileNum].source = SRC_ROM;
	}

	if (!out) {
		out = fileSlots[fileNum].data;
	}

	if (out && outSize) {
		*outSize = fileSlots[fileNum].size;
	}

	return out;
}

void romdataFilePreprocess(s32 fileNum, s32 loadType, u8 *data, u32 size)
{
	if (fileNum < 1 || fileNum >= ROMDATA_MAX_FILES) {
		sysLogPrintf(LOG_ERROR, "romdataFilePreprocess: invalid file num %d", fileNum);
		return;
	}

	if (data && size /* && !fileSlots[fileNum].preprocessed*/) {
		if (loadType && loadType < (u32)ARRAYCOUNT(filePreprocFuncs) && filePreprocFuncs[loadType]) {
			// apply patches
			for (u32 i = 0; i < fileSlots[fileNum].numpatches; ++i) {
				const struct romfilepatch *p = &fileSlots[fileNum].patches[i];
				if (!memcmp(data + p->ofs, p->src, p->len)) {
					memcpy(data + p->ofs, p->dst, p->len);
					sysLogPrintf(LOG_NOTE, "file %d (%s) patched at offset 0x%x", fileNum, fileSlots[fileNum].name, p->ofs);
				}
			}
			// then preprocess
			if (loadType == LOADTYPE_SETUP) {
				setupLoadMappings(fileNum);
			}
			filePreprocFuncs[loadType](data, size, fileNum);
			// fileSlots[fileNum].preprocessed = 1;
		}
	}
}

void romdataFileFree(s32 fileNum)
{
	if (fileNum < 1 || fileNum >= ROMDATA_MAX_FILES) {
		sysLogPrintf(LOG_ERROR, "fsFileFree: invalid file num %d", fileNum);
		return;
	}

	if (fileSlots[fileNum].source == SRC_EXTERNAL) {
		sysMemFree(fileSlots[fileNum].data);
		fileSlots[fileNum].data = NULL;
	}

	fileSlots[fileNum].source = SRC_UNLOADED;
}

const char *romdataFileGetName(s32 fileNum)
{
	if (fileNum < 1 || fileNum >= ROMDATA_MAX_FILES) {
		return NULL;
	}
	return fileSlots[fileNum].name;
}

s32 romdataFileGetNumForName(const char *name)
{
	if (!name || !name[0]) {
		return -1;
	}

	for (s32 i = 0; i < ROMDATA_MAX_FILES; ++i) {
		if (fileSlots[i].name && !strcmp(fileSlots[i].name, name)) {
			return i;
		}
	}

	return -1;
}

u8 *romdataSegGetData(const char *segName)
{
	return romdataGetSeg(segName)->data;
}

u8 *romdataSegGetDataEnd(const char *segName)
{
	struct romfile *seg = romdataGetSeg(segName);
	return seg->data + seg->size;
}

u32 romdataSegGetSize(const char *segName)
{
	return romdataGetSeg(segName)->size;
}
