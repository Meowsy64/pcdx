#include <ultra64.h>
#include "constants.h"
#include "constants.h"
#include "game/camdraw.h"
#include "game/cheats.h"
#include "game/lang.h"
#include "game/player.h"
#include "game/savebuffer.h"
#include "game/bossfile.h"
#include "game/bg.h"
#include "game/challenge.h"
#include "game/training.h"
#include "game/gamefile.h"
#include "game/mplayer/mplayer.h"
#include "game/pak.h"
#include "game/options.h"
#include "game/utils.h"
#include "../src/lib/inih/ini.h"
#include "bss.h"
#include "lib/fault.h"
#include "lib/snd.h"
#include "lib/str.h"
#include "data.h"
#include "types.h"

u8 var800a22d0[0x5b];
u8 g_AltTitleUnlocked;
u8 g_AltTitleEnabled;

/**
 * Read a zero-terminated string from the buffer.
 */
void iniReadString(char *src, char *dst, bool addlinebreak)
{
	s32 i;

	for (i = 0; i < 10; i++) {
		s32 byte = src[i];
		if (byte == '\0') {
			break;
		} else {
			dst[i] = byte;
		}
	}

	if (addlinebreak) {
		i++;
		dst[i] = '\n';
	}

	i++;
	dst[i] = '\0';
}

static int handler(void* user, const char* section, const char* name, const char* value)
{
	if (strcasecmp(section, "bossfile") == 0) {
		if (strcasecmp(name, "language") == 0) {
			g_Vars.language = atoi(value);
			return 1;
		} else if (strcasecmp(name, "tracknum") == 0) {
			int tracknum = atoi(value);
			if (tracknum == 0xff) {
				g_BossFile.tracknum = -1;
			} else {
				g_BossFile.tracknum = tracknum;
			}
			return 1;
		} else if (strcasecmp(name, "usingmultipletunes") == 0) {
			g_BossFile.usingmultipletunes = atoi(value);
			return 1;
		} else if (strcasecmp(name, "alttitleunlocked") == 0) {
			g_AltTitleUnlocked = atoi(value);
			return 1;
		} else if (strcasecmp(name, "alttitleenabled") == 0) {
			g_AltTitleEnabled = atoi(value);
			return 1;
		} else {
			return 0;
		}
	} else if (strcasecmp(section, "teamnames") == 0) {
		int teamnum = atoi(name);
		if (teamnum >= ARRAYCOUNT(g_BossFile.teamnames)) {
			return 0;
		}
		iniReadString(value, g_BossFile.teamnames[teamnum], 1);
		return 1;
	} else if (strcasecmp(section, "multipletracknums") == 0) {
		int i = atoi(name);
		if (i >= ARRAYCOUNT(g_BossFile.multipletracknums)) {
			return 0;
		}
		g_BossFile.multipletracknums[i] = atoi(value);
		return 1;
	} else {
		return 0;
	}

    return 1;
}

bool bossfileLoadFull(void)
{
	bossfileLoad();

#if VERSION >= VERSION_PAL_BETA
	langSetEuropean(g_Vars.language);
#endif

	return true;
}

void bossfileLoad(void)
{
	bossfileSetDefaults();
	if (ini_parse("bossfile.ini", handler, NULL) < 0) {
		bossfileSave();
	}
}

void bossfileSave(void)
{
    FILE *file = fopen("bossfile.ini", "w");
    if (file != NULL) {
		fprintf(file, "[bossfile]\n");
		fprintf(file, "language=%i\n", g_Vars.language);
		fprintf(file, "tracknum=%i\n", g_BossFile.tracknum);
		fprintf(file, "usingmultipletunes=%i\n", g_BossFile.usingmultipletunes);
		fprintf(file, "alttitleunlocked=%i\n", g_AltTitleUnlocked);
		fprintf(file, "alttitleenabled=%i\n", g_AltTitleEnabled);
		fprintf(file, "[teamnames]\n");
		for (int i = 0; i < ARRAYCOUNT(g_BossFile.teamnames); i++) {
			fprintf(file, "%i=", i);
			for (int j = 0; j < 10; j++) {
				char c = g_BossFile.teamnames[i][j];
				if (c == '\0' || c == '\n') {
					break;
				} else {
					fprintf(file, "%c", c);
				}
			}
			fprintf(file, "\n");
		}
		fprintf(file, "[multipletracknums]\n");
		for (int i = 0; i < ARRAYCOUNT(g_BossFile.multipletracknums); i++) {
			fprintf(file, "%i=%i\n", i, g_BossFile.multipletracknums[i]);
		}
		fflush(file);
		fclose(file);
	}
}

void bossfileSetDefaults(void)
{
	g_BossFile.teamnames[0][0] = '\0';
	g_BossFile.teamnames[1][0] = '\0';
	g_BossFile.teamnames[2][0] = '\0';
	g_BossFile.teamnames[3][0] = '\0';
	g_BossFile.teamnames[4][0] = '\0';
	g_BossFile.teamnames[5][0] = '\0';
	g_BossFile.teamnames[6][0] = '\0';
	g_BossFile.teamnames[7][0] = '\0';

	g_BossFile.tracknum = -1;
	mpEnableAllMultiTracks();
	g_BossFile.usingmultipletunes = false;
	g_BossFile.locktype = MPLOCKTYPE_NONE;
	g_Vars.language = (PAL ? 7 : 0);
	g_AltTitleUnlocked = 0;
	g_AltTitleEnabled = false;

	bossfileSave();
}
