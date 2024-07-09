#include <ultra64.h>
#include "constants.h"
#include "bss.h"
#include "lib/dma.h"
#include "lib/memp.h"
#include "data.h"
#include "types.h"

void texInit(void)
{
	extern u8 EXT_SEG _textureslistSegmentRomStart;
	extern u8 EXT_SEG _textureslistSegmentRomEnd;
	extern u8 EXT_SEG _gextextureslistSegmentRomStart;
	extern u8 EXT_SEG _gextextureslistSegmentRomEnd;

	u32 len = ((REF_SEG _textureslistSegmentRomEnd - REF_SEG _textureslistSegmentRomStart) + 15) & -16;
	u32 len2 = ((REF_SEG _gextextureslistSegmentRomEnd - REF_SEG _gextextureslistSegmentRomStart) + 15) & -16;

	g_Textures = mempAlloc(len + len2, MEMPOOL_PERMANENT);

	dmaExec(g_Textures, (romptr_t) REF_SEG _textureslistSegmentRomStart, len);
	//dmaExec(g_Textures + len, (romptr_t) REF_SEG _gextextureslistSegmentRomStart, len2);
	dmaExec(g_Textures + len, (romptr_t) REF_SEG _textureslistSegmentRomStart, len); // Reuse the PD textureslist until we find the right address
}
