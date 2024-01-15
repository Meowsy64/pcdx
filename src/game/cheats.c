#include <ultra64.h>
#include "constants.h"
#include "lib/sched.h"
#include "lib/str.h"
#include "game/bondgun.h"
#include "game/camdraw.h"
#include "game/cheats.h"
#include "game/inv.h"
#include "game/playermgr.h"
#include "game/training.h"
#include "game/gamefile.h"
#include "game/lang.h"
#include "game/pak.h"
#include "bss.h"
#include "data.h"
#include "string.h"
#include "types.h"

struct menuitem g_CheatsBuddiesMenuItems[];
struct menudialogdef g_CheatsBuddiesMenuDialog;

#define TIME(mins, secs) (mins * 60 + secs)
#define m
#define s

struct cheat g_Cheats[] = {
	{ L_MPWEAPONS_075,              TIME(2 m,  3 s),   SOLOSTAGEINDEX_EXTRACTION,     DIFF_A,  CHEATFLAG_TIMED | CHEATFLAG_TRANSFERPAK,                                                       { CHEAT_NONE           } }, // Hurricane Fists
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_076,              TIME(1 m, 40 s),   SOLOSTAGEINDEX_G5BUILDING,     DIFF_A,  CHEATFLAG_TIMED | CHEATFLAG_TRANSFERPAK,                                                       { CHEAT_NONE           } }, // Cloaking Device
#else
	{ L_MPWEAPONS_076,              TIME(0 m, 59 s),   SOLOSTAGEINDEX_G5BUILDING,     DIFF_A,  CHEATFLAG_TIMED | CHEATFLAG_TRANSFERPAK,                                                       { CHEAT_NONE           } }, // Cloaking Device
#endif
	{ L_MPWEAPONS_077,              TIME(3 m, 50 s),   SOLOSTAGEINDEX_ESCAPE,         DIFF_A,  CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Invincible
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_078,              TIME(5 m, 31 s),   SOLOSTAGEINDEX_SKEDARRUINS,    DIFF_PA, CHEATFLAG_TIMED | CHEATFLAG_TRANSFERPAK,                                                       { CHEAT_NONE           } }, // All Guns in Solo
	{ L_MPWEAPONS_079,              TIME(7 m,  7 s),   SOLOSTAGEINDEX_PELAGIC,        DIFF_SA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo
	{ L_MPWEAPONS_080,              TIME(3 m, 11 s),   SOLOSTAGEINDEX_AIRBASE,        DIFF_SA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo, No Reloads
#else
	{ L_MPWEAPONS_078,              TIME(4 m,  7 s),   SOLOSTAGEINDEX_SKEDARRUINS,    DIFF_PA, CHEATFLAG_TIMED | CHEATFLAG_TRANSFERPAK,                                                       { CHEAT_NONE           } }, // All Guns in Solo
	{ L_MPWEAPONS_079,              TIME(5 m, 50 s),   SOLOSTAGEINDEX_PELAGIC,        DIFF_SA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo
	{ L_MPWEAPONS_080,              TIME(2 m, 59 s),   SOLOSTAGEINDEX_AIRBASE,        DIFF_SA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo, No Reloads
#endif
	{ L_MPWEAPONS_081,              0,                 SOLOSTAGEINDEX_INVESTIGATION,  DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Slo-mo Single Player
	{ L_MPWEAPONS_082,              0,                 SOLOSTAGEINDEX_CHICAGO,        DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // DK Mode
	{ L_MPWEAPONS_083,              TIME(2 m, 50 s),   SOLOSTAGEINDEX_CRASHSITE,      DIFF_A,  CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Trent's Magnum
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_084,              TIME(7 m, 27 s),   SOLOSTAGEINDEX_DEEPSEA,        DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // FarSight
#else
	{ L_MPWEAPONS_084,              TIME(5 m, 13 s),   SOLOSTAGEINDEX_DEEPSEA,        DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // FarSight
#endif
	{ L_MPWEAPONS_085,              0,                 SOLOSTAGEINDEX_G5BUILDING,     DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Small Jo
	{ L_MPWEAPONS_086,              0,                 SOLOSTAGEINDEX_INFILTRATION,   DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Small Characters
	{ L_MPWEAPONS_087,              0,                 SOLOSTAGEINDEX_DEFENSE,        DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Enemy Shields
	{ L_MPWEAPONS_088,              0,                 SOLOSTAGEINDEX_DEEPSEA,        DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Jo Shield
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_089,              TIME(1 m, 45 s),   SOLOSTAGEINDEX_DEFENSE,        DIFF_A,  CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Super Shield
#else
	{ L_MPWEAPONS_089,              TIME(1 m, 12 s),   SOLOSTAGEINDEX_DEFENSE,        DIFF_A,  CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Super Shield
#endif
	{ L_MPWEAPONS_090,              0,                 SOLOSTAGEINDEX_DEFECTION,      DIFF_A,  CHEATFLAG_COMPLETION | CHEATFLAG_NONINVALIDATING,                                              { CHEAT_NONE           } }, // Classic Sight
	{ L_MPWEAPONS_091,              0,                 SOLOSTAGEINDEX_AIRBASE,        DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Team Heads Only
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_092,              TIME(7 m, 59 s),   SOLOSTAGEINDEX_RESCUE,         DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Play as Elvis
#else
	{ L_MPWEAPONS_092,              TIME(7 m,  0 s),   SOLOSTAGEINDEX_RESCUE,         DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Play as Elvis
#endif
	{ L_MPWEAPONS_093,              0,                 SOLOSTAGEINDEX_PELAGIC,        DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_MARQUIS        } }, // Enemy Rockets
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_094,              TIME(3 m, 55 s),   SOLOSTAGEINDEX_AIRFORCEONE,    DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo - Laptop Sentry Gun
#else
	{ L_MPWEAPONS_094,              TIME(2 m, 59 s),   SOLOSTAGEINDEX_AIRFORCEONE,    DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Unlimited Ammo - Laptop Sentry Gun
#endif
	{ L_MPWEAPONS_095,              TIME(1 m, 30 s),   SOLOSTAGEINDEX_DEFECTION,      DIFF_SA, CHEATFLAG_TIMED,                                                                               { CHEAT_ENEMYROCKETS   } }, // Marquis of Queensbury Rules
	{ L_MPWEAPONS_096,              0,                 SOLOSTAGEINDEX_CRASHSITE,      DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Perfect Darkness
	{ L_MPWEAPONS_117,              0,                 0,                             0,       CHEATFLAG_ALWAYSAVAILABLE | CHEATFLAG_BUDDY | CHEATFLAG_NONINVALIDATING,                       { CHEAT_NONE           } }, // Velvet Dark
	{ L_MPWEAPONS_097,              TIME(6 m, 30 s),   SOLOSTAGEINDEX_INVESTIGATION,  DIFF_PA, CHEATFLAG_TIMED | CHEATFLAG_BUDDY,                                                             { CHEAT_NONE           } }, // Pugilist
	{ L_MPWEAPONS_098,              TIME(5 m,  0 s),   SOLOSTAGEINDEX_INFILTRATION,   DIFF_SA, CHEATFLAG_TIMED | CHEATFLAG_BUDDY,                                                             { CHEAT_NONE           } }, // Hotshot
	{ L_MPWEAPONS_099,              TIME(2 m, 30 s),   SOLOSTAGEINDEX_VILLA,          DIFF_SA, CHEATFLAG_TIMED | CHEATFLAG_BUDDY,                                                             { CHEAT_NONE           } }, // Hit and Run
	{ L_MPWEAPONS_100,              TIME(5 m, 17 s),   SOLOSTAGEINDEX_ATTACKSHIP,     DIFF_SA, CHEATFLAG_TIMED | CHEATFLAG_BUDDY,                                                             { CHEAT_NONE           } }, // Alien
	{ L_MPWEAPONS_101,              0,                 SOLOSTAGEINDEX_SKEDARRUINS,    DIFF_A,  CHEATFLAG_COMPLETION | CHEATFLAG_TRANSFERPAK | CHEATFLAG_SOLOONLY,                             { CHEAT_NONE           } }, // R-Tracker/Weapon Cache Locations
	{ L_MPWEAPONS_102,              0,                 SOLOSTAGEINDEX_EXTRACTION,     DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Rocket Launcher
	{ L_MPWEAPONS_103,              0,                 SOLOSTAGEINDEX_VILLA,          DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Sniper Rifle
	{ L_MPWEAPONS_104,              0,                 SOLOSTAGEINDEX_RESCUE,         DIFF_A,  CHEATFLAG_COMPLETION | CHEATFLAG_SOLOONLY,                                                     { CHEAT_NONE           } }, // X-Ray Scanner
	{ L_MPWEAPONS_105,              0,                 SOLOSTAGEINDEX_ESCAPE,         DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // SuperDragon
	{ L_MPWEAPONS_106,              0,                 SOLOSTAGEINDEX_AIRFORCEONE,    DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Laptop Gun
	{ L_MPWEAPONS_107,              0,                 SOLOSTAGEINDEX_ATTACKSHIP,     DIFF_A,  CHEATFLAG_COMPLETION,                                                                          { CHEAT_NONE           } }, // Phoenix
#if VERSION >= VERSION_NTSC_1_0
	{ L_MPWEAPONS_108,              TIME(2 m,  0 s),   SOLOSTAGEINDEX_CHICAGO,        DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Psychosis Gun
#else
	{ L_MPWEAPONS_108,              TIME(1 m, 44 s),   SOLOSTAGEINDEX_CHICAGO,        DIFF_PA, CHEATFLAG_TIMED,                                                                               { CHEAT_NONE           } }, // Psychosis Gun
#endif
	{ L_MPWEAPONS_109,              WEAPON_PP9I,       0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_FALCON2,      FRDIFFICULTY_GOLD, }, { WEAPON_FALCON2_SCOPE, FRDIFFICULTY_GOLD, }, { WEAPON_FALCON2_SILENCER, FRDIFFICULTY_GOLD, },                                                                                         }, }, // PP9i
	{ L_MPWEAPONS_110,              WEAPON_CC13,       0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_MAGSEC4,      FRDIFFICULTY_GOLD, }, { WEAPON_MAULER,        FRDIFFICULTY_GOLD, }, { WEAPON_PHOENIX,          FRDIFFICULTY_GOLD, }, { WEAPON_DY357MAGNUM, FRDIFFICULTY_GOLD, }, { WEAPON_DY357LX,     FRDIFFICULTY_GOLD, }, }, }, // CC13
	{ L_MPWEAPONS_111,              WEAPON_KL01313,    0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_CMP150,       FRDIFFICULTY_GOLD, }, { WEAPON_CYCLONE,       FRDIFFICULTY_GOLD, }, { WEAPON_CALLISTO,         FRDIFFICULTY_GOLD, }, { WEAPON_RCP120,      FRDIFFICULTY_GOLD, },                                             }, }, // KL01313
	{ L_MPWEAPONS_112,              WEAPON_KF7SPECIAL, 0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_LAPTOPGUN,    FRDIFFICULTY_GOLD, }, { WEAPON_DRAGON,        FRDIFFICULTY_GOLD, }, { WEAPON_K7AVENGER,        FRDIFFICULTY_GOLD, }, { WEAPON_AR34,        FRDIFFICULTY_GOLD, }, { WEAPON_SUPERDRAGON, FRDIFFICULTY_GOLD, }, }, }, // KF7 Special
	{ L_MPWEAPONS_113,              WEAPON_ZZT,        0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_SHOTGUN,      FRDIFFICULTY_GOLD, }, { WEAPON_SNIPERRIFLE,   FRDIFFICULTY_GOLD, }, { WEAPON_ROCKETLAUNCHER,   FRDIFFICULTY_GOLD, }, { WEAPON_SLAYER,      FRDIFFICULTY_GOLD, },                                             }, }, // ZZT (9mm)
	{ L_MPWEAPONS_114,              WEAPON_DMC,        0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_TIMEDMINE,    FRDIFFICULTY_GOLD, }, { WEAPON_PROXIMITYMINE, FRDIFFICULTY_GOLD, }, { WEAPON_REMOTEMINE,       FRDIFFICULTY_GOLD, },                                                                                         }, }, // DMC
	{ L_MPWEAPONS_115,              WEAPON_AR53,       0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_FARSIGHT,     FRDIFFICULTY_GOLD, }, { WEAPON_CROSSBOW,      FRDIFFICULTY_GOLD, }, { WEAPON_COMBATKNIFE,      FRDIFFICULTY_GOLD, }, { WEAPON_GRENADE,     FRDIFFICULTY_GOLD, },                                             }, }, // AR53
	{ L_MPWEAPONS_116,              WEAPON_RCP45,      0,                             0,       CHEATFLAG_FIRINGRANGE,                                                                         { CHEAT_NONE           }, { { WEAPON_TRANQUILIZER, FRDIFFICULTY_GOLD, }, { WEAPON_REAPER,        FRDIFFICULTY_GOLD, }, { WEAPON_DEVASTATOR,       FRDIFFICULTY_GOLD, },                                                                                         }, }, // RC-P45
	{ L_MPWEAPONS_UNLOCKALLCONTENT, 0,                 0,                             0,       CHEATFLAG_ALWAYSAVAILABLE | CHEATFLAG_NONINVALIDATING,                                         { CHEAT_NONE           } }, // CHEAT_UNLOCKALLCONTENT
	{ L_MPWEAPONS_ULTRAVISIBLE,     0,                 0,                             0,       CHEATFLAG_ALWAYSAVAILABLE,                                                                     { CHEAT_NONE           } }, // CHEAT_ULTRAVISIBLE
	{ L_MPWEAPONS_CLASSICMODE,      0,                 0,                             0,       CHEATFLAG_ALWAYSAVAILABLE,                                                                     { CHEAT_NONE           } }, // CHEAT_CLASSICMODE
};

u32 cheatIsUnlocked(s32 cheat_id)
{
	struct cheat *cheat;
	u32 unlocked = 0;

	if (cheat_id == CHEAT_NONE) {
		return true;
	}

	if (cheatIsInvalid(cheat_id)) {
		return false;
	}

	if (cheatIsAllContentUnlocked()) {
		return true;
	}

	cheat = &g_Cheats[cheat_id];

	if (cheat->flags & CHEATFLAG_ALWAYSAVAILABLE) {
		return true;
	} else if (cheat->flags & CHEATFLAG_FIRINGRANGE) {
		if (frIsCheatUnlocked(cheat)) {
			unlocked++;
		}
	} else if (cheat->flags & CHEATFLAG_COMPLETION) {
		if (g_GameFile.besttimes[cheat->stage_index][0]) {
			unlocked++;
		}
		if (g_GameFile.besttimes[cheat->stage_index][1]) {
			unlocked++;
		}
		if (g_GameFile.besttimes[cheat->stage_index][2]) {
			unlocked++;
		}
	} else {
		if (g_GameFile.besttimes[cheat->stage_index][cheat->difficulty] &&
				g_GameFile.besttimes[cheat->stage_index][cheat->difficulty] <= cheat->time) {
			unlocked++;
		}
	}

	if ((cheat->flags & CHEATFLAG_TRANSFERPAK) && gamefileHasFlag(GAMEFILEFLAG_USED_TRANSFERPAK)) {
		unlocked++;
	}

	return unlocked;
}

bool cheatIsActive(s32 cheat_id)
{
	if (cheat_id == CHEAT_NONE) {
		return true;
	}

	if (cheatIsInvalid(cheat_id)) {
		return false;
	}

	return g_Cheats[cheat_id].active;
}

void cheatActivate(s32 cheat_id)
{
	u32 prevplayernum;
	s32 playernum;
	bool isMulti = g_Vars.normmplayerisrunning == true;

	if (cheat_id == CHEAT_NONE) {
		return;
	}

	if (cheatIsInvalid(cheat_id)) {
		return;
	}

	if ((isMulti && g_Cheats[cheat_id].flags & CHEATFLAG_SOLOONLY) || (!isMulti && g_Cheats[cheat_id].flags & CHEATFLAG_MULTIONLY)) {
		g_Cheats[cheat_id].active = false;
		return;
	}

	switch (cheat_id) {
	case CHEAT_INVINCIBLE:
		// Make all players invincible
		prevplayernum = g_Vars.currentplayernum;

		for (playernum = 0; playernum < PLAYERCOUNT(); playernum++) {
			setCurrentPlayerNum(playernum);
			g_Vars.currentplayer->invincible = 1;
		}

		setCurrentPlayerNum(prevplayernum);
		break;
	case CHEAT_ALLGUNS:
		prevplayernum = g_Vars.currentplayernum;

		for (playernum = 0; playernum < PLAYERCOUNT(); playernum++) {
			setCurrentPlayerNum(playernum);
			invSetAllGuns(true);
		}

		setCurrentPlayerNum(prevplayernum);
		break;
	}

	g_Cheats[cheat_id].active = true;
}

void cheatDeactivate(s32 cheat_id)
{
	u32 prevplayernum;
	s32 playernum;

	switch (cheat_id) {
	case CHEAT_INVINCIBLE:
		prevplayernum = g_Vars.currentplayernum;

		for (playernum = 0; playernum < PLAYERCOUNT(); playernum++) {
			setCurrentPlayerNum(playernum);
			g_Vars.currentplayer->invincible = false;
		}

		setCurrentPlayerNum(prevplayernum);
		break;
	case CHEAT_ALLGUNS:
		if (PLAYERCOUNT() == 1 && g_Vars.normmplayerisrunning == false) {
			prevplayernum = g_Vars.currentplayernum;

			for (playernum = 0; playernum < PLAYERCOUNT(); playernum++) {
				setCurrentPlayerNum(playernum);
				invSetAllGuns(false);
			}

			setCurrentPlayerNum(prevplayernum);
		}
		break;
	case CHEAT_UNLOCKALLCONTENT:
		cheatsReset();
		break;
	}

	g_Cheats[cheat_id].active = false;
}

void cheatsInit(void)
{
	struct cheat *cheat;

	s32 cheat_id;
	for (cheat_id = 0; cheat_id < NUM_CHEATS; cheat_id++) {
		g_Cheats[cheat_id].active = false;
		g_Cheats[cheat_id].enabled = false;
	}

	// TODO: Apply cheats from save file.
	g_Cheats[CHEAT_UNLOCKALLCONTENT].enabled = true;
	g_Cheats[CHEAT_UNLOCKALLCONTENT].active = true;
	cheatActivate(CHEAT_UNLOCKALLCONTENT);
}

/**
 * Apply cheats at level startup.
 */
void cheatsReset(void)
{
	s32 cheat_id;

	// Copy enabled cheats to active cheats, unless in CI training
	// or weapon cheats not in solo
	for (cheat_id = 0; cheat_id < NUM_CHEATS; cheat_id++) {
		if (g_Vars.stagenum == STAGE_CITRAINING && cheat_id != CHEAT_UNLOCKALLCONTENT) {
			continue;
		}

		// Copy enabled cheats to active cheats
		g_Cheats[cheat_id].active = g_Cheats[cheat_id].enabled;
		if (g_Cheats[cheat_id].active) {
			cheatActivate(cheat_id);
		}
	}

	// Set any "always on" cheats to active and properly activate all active cheats
	for (cheat_id = 0; cheat_id != NUM_CHEATS; cheat_id++) {
		if (g_Cheats[cheat_id].flags & CHEATFLAG_ALWAYSON) {
			g_Cheats[cheat_id].active = cheatIsUnlocked(cheat_id);
		}

		if (g_Cheats[cheat_id].active) {
			cheatActivate(cheat_id);
		}
	}
}

MenuItemHandlerResult cheatCheckboxMenuHandler(s32 operation, struct menuitem *item, union handlerdata *data)
{
	s32 i;

	switch (operation) {
	case MENUOP_GET:
		if (g_Cheats[item->param].flags & CHEATFLAG_BUDDY) {
			return cheatIsUnlocked(item->param);
		}
		return g_Cheats[item->param].enabled;
	case MENUOP_SET:
		if (g_Cheats[item->param].flags & CHEATFLAG_BUDDY) {
			return false;
		}

		if (!cheatIsUnlocked(item->param)) {
			return false;
		}

		if (g_Cheats[item->param].enabled) {
			g_Cheats[item->param].enabled = false;
			cheatDeactivate(item->param);
		} else {
			g_Cheats[item->param].enabled = true;
			// Disable incompatible cheats.
			for (i = 0; i < ARRAYCOUNT(g_Cheats[item->param].incompatiblecheats); i++) {
				if (g_Cheats[item->param].incompatiblecheats[i] != CHEAT_NONE) {
					g_Cheats[g_Cheats[item->param].incompatiblecheats[i]].enabled = false;
					cheatDeactivate(g_Cheats[item->param].incompatiblecheats[i]);
				}
			}
			cheatActivate(item->param);
		}

		for (i = 0; i < NUM_CHEATS; i++) {
			if (g_Cheats[i].enabled) {
				cheatActivate(item->param);
			}
		}

		return false;
	}

	return 0;
}

char *cheatGetNameIfUnlocked(struct menuitem *item)
{
	if (cheatIsUnlocked(item->param)) {
		return langGet(g_Cheats[item->param].nametextid);
	}

	return langGet(L_MPWEAPONS_074); // "----------"
}

MenuDialogHandlerResult cheatMenuHandleDialog(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data)
{
	if (operation == MENUOP_OPEN) {
		func0f14a52c();

		if (gbpakIsAnyPerfectDark()) {
			gamefileSetFlag(GAMEFILEFLAG_USED_TRANSFERPAK);
		}

#if PIRACYCHECKS
		{
			u32 *ptr = (u32 *)&__scHandleTasks;
			u32 *end = (u32 *)&__scHandleRSP;
			u32 checksum = 0;

			while (ptr < end) {
				checksum ^= ~*ptr;
				ptr++;
			}

			if (checksum != CHECKSUM_PLACEHOLDER) {
				ptr = (u32 *)&__scHandleTasks + 20;
				if (1);
				end = &ptr[4];

				while (ptr < end) {
					*ptr = 0x00000012;
					ptr++;
				}
			}
		}
#endif
	}

	if (operation == MENUOP_CLOSE) {
		if (gbpakIsAnyPerfectDark()) {
			gamefileSetFlag(GAMEFILEFLAG_USED_TRANSFERPAK);
		}

		func0f14a560();
	}

	return 0;
}

struct menuitem g_CheatsWarningMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		0,
		L_MPMENU_479, // "If you activate any cheats, then you will be unable to progress further in the game while those cheats are active."
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_480, // "OK"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_481, // "Cancel"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsWarningMenuDialog = {
	MENUDIALOGTYPE_SUCCESS,
	L_MPMENU_478, // "Warning"
	g_CheatsWarningMenuItems,
	NULL,
	0,
	NULL,
};

/**
 * NTSC Beta uses g_StringPointer while newer versions use g_CheatMarqueeString.
 *
 * PAL Final introduces a string length check which ultimately doesn't do
 * anything.
 *
 * JPN final removes the colon characters from the format strings.
 */
char *cheatGetMarquee(struct menuitem *arg0)
{
	u32 cheat_id;
	char *ptr;
	char difficultyname[256];
	char cheatname[256];

#if VERSION >= VERSION_JPN_FINAL
	s32 len;
	static s32 var80074020pf = 0;
	static s32 var80074024pf = 0;

	if (g_Menus[g_MpPlayerNum].curdialog
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem->type == MENUITEMTYPE_CHECKBOX) {
		cheat_id = g_Menus[g_MpPlayerNum].curdialog->focuseditem->param;

		if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog
				&& g_Menus[g_MpPlayerNum].curdialog->focuseditem == &g_CheatsBuddiesMenuItems[0]) {
			// Velvet
			sprintf(g_CheatMarqueeString, "%s %s", langGet(L_MPWEAPONS_143), langGet(L_MPWEAPONS_117)); // "Buddy Available", "Velvet Dark"
		} else if (cheatIsUnlocked(cheat_id)) {
			// Show cheat name
			sprintf(g_CheatMarqueeString, "%s %s\n",
					g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog ? langGet(L_MPWEAPONS_143) : langGet(L_MPWEAPONS_136), // "Buddy Available", "Cheat available"
					langGet(g_Cheats[cheat_id].nametextid)
			);
		} else {
			// Locked
			strcpy(cheatname, langGet(g_Cheats[cheat_id].nametextid));
			ptr = cheatname;

			while (*ptr != '\n') {
				ptr++;
			}

			*ptr = '\0';

			if (g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION) {
				sprintf(g_CheatMarqueeString, "%s %s %s %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			} else {
				// Timed
				strcpy(difficultyname, langGet(L_OPTIONS_251 + g_Cheats[cheat_id].difficulty));
				ptr = difficultyname;

				while (*ptr != '\n') {
					ptr++;
				}

				*ptr = '\0';

				sprintf(g_CheatMarqueeString, "%s %s %s %s %s %s %d:%02d %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_139), // "on"
						&difficultyname,
						langGet(L_MPWEAPONS_140), // "in under"
						g_Cheats[cheat_id].time / 60,
						g_Cheats[cheat_id].time % 60,
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			}

			if (g_Cheats[cheat_id].flags & CHEATFLAG_TRANSFERPAK) {
				strcat(g_CheatMarqueeString, langGet(L_MPWEAPONS_141)); // " or insert Game Boy ..."
			}

			strcat(g_CheatMarqueeString, "\n");
		}

		len = strlen(g_CheatMarqueeString);

		if (var80074024pf != len) {
			var80074024pf = len;

			if (len > var80074020pf) {
				var80074020pf = len;
			}
		}

		return g_CheatMarqueeString;
	}
#elif VERSION >= VERSION_PAL_FINAL
	s32 len;
	static s32 var80074020pf = 0;
	static s32 var80074024pf = 0;

	if (g_Menus[g_MpPlayerNum].curdialog
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem->type == MENUITEMTYPE_CHECKBOX) {
		cheat_id = g_Menus[g_MpPlayerNum].curdialog->focuseditem->param;

		if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog
				&& g_Menus[g_MpPlayerNum].curdialog->focuseditem == &g_CheatsBuddiesMenuItems[0]) {
			// Velvet
			sprintf(g_CheatMarqueeString, "%s: %s", langGet(L_MPWEAPONS_143), langGet(L_MPWEAPONS_117)); // "Buddy Available", "Velvet Dark"
		} else if (cheatIsUnlocked(cheat_id)) {
			// Show cheat name
			sprintf(g_CheatMarqueeString, "%s: %s\n",
					g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog ? langGet(L_MPWEAPONS_143) : langGet(L_MPWEAPONS_136), // "Buddy Available", "Cheat available"
					langGet(g_Cheats[cheat_id].nametextid)
			);
		} else {
			// Locked
			strcpy(cheatname, langGet(g_Cheats[cheat_id].nametextid));
			ptr = cheatname;

			while (*ptr != '\n') {
				ptr++;
			}

			*ptr = '\0';

			if (g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION) {
				sprintf(g_CheatMarqueeString, "%s %s: %s %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			} else {
				// Timed
				strcpy(difficultyname, langGet(L_OPTIONS_251 + g_Cheats[cheat_id].difficulty));
				ptr = difficultyname;

				while (*ptr != '\n') {
					ptr++;
				}

				*ptr = '\0';

				sprintf(g_CheatMarqueeString, "%s %s: %s %s %s %s %d:%02d %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_139), // "on"
						&difficultyname,
						langGet(L_MPWEAPONS_140), // "in under"
						g_Cheats[cheat_id].time / 60,
						g_Cheats[cheat_id].time % 60,
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			}

			if (g_Cheats[cheat_id].flags & CHEATFLAG_TRANSFERPAK) {
				strcat(g_CheatMarqueeString, langGet(L_MPWEAPONS_141)); // " or insert Game Boy ..."
			}

			strcat(g_CheatMarqueeString, "\n");
		}

		len = strlen(g_CheatMarqueeString);

		if (var80074024pf != len) {
			var80074024pf = len;

			if (len > var80074020pf) {
				var80074020pf = len;
			}
		}

		return g_CheatMarqueeString;
	}
#elif VERSION >= VERSION_NTSC_1_0
	if (g_Menus[g_MpPlayerNum].curdialog
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem->type == MENUITEMTYPE_CHECKBOX) {
		cheat_id = g_Menus[g_MpPlayerNum].curdialog->focuseditem->param;

		if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog
				&& g_Menus[g_MpPlayerNum].curdialog->focuseditem == &g_CheatsBuddiesMenuItems[0]) {
			// Velvet
			sprintf(g_CheatMarqueeString, "%s: %s", langGet(L_MPWEAPONS_143), langGet(L_MPWEAPONS_117)); // "Buddy Available", "Velvet Dark"
		} else if (cheatIsUnlocked(cheat_id)) {
			// Show cheat name
			sprintf(g_CheatMarqueeString, "%s: %s\n",
					g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog ? langGet(L_MPWEAPONS_143) : langGet(L_MPWEAPONS_136), // "Buddy Available", "Cheat available"
					langGet(g_Cheats[cheat_id].nametextid)
			);
		} else {
			// Locked
			strcpy(cheatname, langGet(g_Cheats[cheat_id].nametextid));
			ptr = cheatname;

			while (*ptr != '\n') {
				ptr++;
			}

			*ptr = '\0';

			if (g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION) {
				sprintf(g_CheatMarqueeString, "%s %s: %s %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			} else {
				// Timed
				strcpy(difficultyname, langGet(L_OPTIONS_251 + g_Cheats[cheat_id].difficulty));
				ptr = difficultyname;

				while (*ptr != '\n') {
					ptr++;
				}

				*ptr = '\0';

				sprintf(g_CheatMarqueeString, "%s %s: %s %s %s %s %d:%02d %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_139), // "on"
						&difficultyname,
						langGet(L_MPWEAPONS_140), // "in under"
						g_Cheats[cheat_id].time / 60,
						g_Cheats[cheat_id].time % 60,
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			}

			if (g_Cheats[cheat_id].flags & CHEATFLAG_TRANSFERPAK) {
				strcat(g_CheatMarqueeString, langGet(L_MPWEAPONS_141)); // " or insert Game Boy ..."
			}

			strcat(g_CheatMarqueeString, "\n");
		}

		return g_CheatMarqueeString;
	}
#else
	if (g_Menus[g_MpPlayerNum].curdialog
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem
			&& g_Menus[g_MpPlayerNum].curdialog->focuseditem->type == MENUITEMTYPE_CHECKBOX) {
		cheat_id = g_Menus[g_MpPlayerNum].curdialog->focuseditem->param;

		if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog
				&& g_Menus[g_MpPlayerNum].curdialog->focuseditem == &g_CheatsBuddiesMenuItems[0]) {
			// Velvet
			sprintf(g_StringPointer, "%s: %s", langGet(L_MPWEAPONS_143), langGet(L_MPWEAPONS_117)); // "Buddy Available", "Velvet Dark"
		} else if (cheatIsUnlocked(cheat_id)) {
			// Show cheat name
			sprintf(g_StringPointer, "%s: %s\n",
					g_Menus[g_MpPlayerNum].curdialog->definition == &g_CheatsBuddiesMenuDialog ? langGet(L_MPWEAPONS_143) : langGet(L_MPWEAPONS_136), // "Buddy Available", "Cheat available"
					langGet(g_Cheats[cheat_id].nametextid)
			);
		} else {
			// Locked
			strcpy(cheatname, langGet(g_Cheats[cheat_id].nametextid));
			ptr = cheatname;

			while (*ptr != '\n') {
				ptr++;
			}

			*ptr = '\0';

			if (g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION) {
				sprintf(g_StringPointer, "%s %s: %s %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			} else {
				// Timed
				strcpy(difficultyname, langGet(L_OPTIONS_251 + g_Cheats[cheat_id].difficulty));
				ptr = difficultyname;

				while (*ptr != '\n') {
					ptr++;
				}

				*ptr = '\0';

				sprintf(g_StringPointer, "%s %s: %s %s %s %s %d:%02d %s %s",
						langGet(L_MPWEAPONS_137), // "Complete"
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name1),
						langGet(g_SoloStages[g_Cheats[cheat_id].stage_index].name2),
						langGet(L_MPWEAPONS_139), // "on"
						&difficultyname,
						langGet(L_MPWEAPONS_140), // "in under"
						g_Cheats[cheat_id].time / 60,
						g_Cheats[cheat_id].time % 60,
						langGet(L_MPWEAPONS_138), // "for cheat:"
						&cheatname
				);
			}

			if (g_Cheats[cheat_id].flags & CHEATFLAG_TRANSFERPAK) {
				strcat(g_StringPointer, langGet(L_MPWEAPONS_141)); // " or insert Game Boy ..."
			}

			strcat(g_StringPointer, "\n");
		}

		return g_StringPointer;
	}
#endif

	// No cheat selected
	return langGet(L_MPWEAPONS_142); // "Select cheat for information"
}

MenuItemHandlerResult cheatMenuHandleTurnOffAllCheats(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
		s32 cheat_id;
		for (cheat_id = 0; cheat_id < NUM_CHEATS; cheat_id++) {
			g_Cheats[cheat_id].enabled = false;
			g_Cheats[cheat_id].active = false;
		}
	}

	return false;
}

#if VERSION >= VERSION_NTSC_1_0
s32 cheatGetByTimedStageIndex(s32 stage_index, s32 difficulty)
{
	s32 cheat_id;

	for (cheat_id = 0; cheat_id < ARRAYCOUNT(g_Cheats); cheat_id++) {
		if (g_Cheats[cheat_id].stage_index == stage_index &&
				g_Cheats[cheat_id].difficulty == difficulty &&
				(g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION) == 0 &&
				(g_Cheats[cheat_id].flags & CHEATFLAG_FIRINGRANGE) == 0) {
			return cheat_id;
		}
	}

	return -1;
}
#endif

#if VERSION >= VERSION_NTSC_1_0
s32 cheatGetByCompletedStageIndex(s32 stage_index)
{
	s32 cheat_id;

	for (cheat_id = 0; cheat_id < ARRAYCOUNT(g_Cheats); cheat_id++) {
		if (g_Cheats[cheat_id].stage_index == stage_index && (g_Cheats[cheat_id].flags & CHEATFLAG_COMPLETION)) {
			return cheat_id;
		}
	}

	return -1;
}
#endif

#if VERSION >= VERSION_NTSC_1_0
s32 cheatGetTime(s32 cheat_id)
{
	return g_Cheats[cheat_id].time;
}
#endif

#if VERSION >= VERSION_NTSC_1_0
char *cheatGetName(s32 cheat_id)
{
	return langGet(g_Cheats[cheat_id].nametextid);
}
#endif

s32 cheatAreInvalidatingCheatsActive()
{
	s32 i;
	for (i = 0; i < ARRAYCOUNT(g_Cheats); i++) {
		if (g_Cheats[i].active && !(g_Cheats[i].flags & CHEATFLAG_NONINVALIDATING)) {
			return true;
		}
	}
	return false;
}

s32 cheatAreSoloInvalidatingCheatsEnabled()
{
	s32 i;
	for (i = 0; i < ARRAYCOUNT(g_Cheats); i++) {
		if (g_Cheats[i].enabled && !(g_Cheats[i].flags & CHEATFLAG_NONINVALIDATING) && !(g_Cheats[i].flags & CHEATFLAG_MULTIONLY)) {
			return true;
		}
	}
	return false;
}

s32 cheatAreMultiInvalidatingCheatsEnabled()
{
	s32 i;
	for (i = 0; i < ARRAYCOUNT(g_Cheats); i++) {
		if (g_Cheats[i].enabled && !(g_Cheats[i].flags & CHEATFLAG_NONINVALIDATING) && !(g_Cheats[i].flags & CHEATFLAG_SOLOONLY)) {
			return true;
		}
	}
	return false;
}

#ifndef PLATFORM_N64

static MenuItemHandlerResult menuhandlerUnlockEverything(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
		gamefileUnlockEverything();
	}
	return 0;
}

struct menuitem g_CheatsConfirmUnlockMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LITERAL_TEXT,
		(uintptr_t)"Are you sure?\n\nThis will overwrite any progress\nsaved to the current profile.\n",
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000082,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES | MENUITEMFLAG_LITERAL_TEXT,
		(uintptr_t)"Unlocks all cheats, weapons, missions, challenges and combat simulator items.\n",
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000082,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_OPTIONS_191, // "No"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_OPTIONS_190, // "Yes"
		0,
		menuhandlerUnlockEverything,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsConfirmUnlockMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_OPTIONS_188, // "Warning"
	g_CheatsConfirmUnlockMenuItems,
	NULL,
	0,
	NULL,
};

#endif

struct menuitem g_CheatsFunMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_DKMODE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SMALLJO,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SMALLCHARACTERS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_TEAMHEADSONLY,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PLAYASELVIS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SLOMO,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		(uintptr_t)&cheatGetMarquee,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsFunMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_118, // "Fun"
	g_CheatsFunMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsGameplayMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_INVINCIBLE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_CLOAKINGDEVICE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_MARQUIS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_JOSHIELD,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SUPERSHIELD,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ENEMYSHIELDS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ENEMYROCKETS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PERFECTDARKNESS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ULTRAVISIBLE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_CLASSICMODE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		(uintptr_t)&cheatGetMarquee,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsGameplayMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_119, // "Gameplay"
	g_CheatsGameplayMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsSoloWeaponsMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ROCKETLAUNCHER,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SNIPERRIFLE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_SUPERDRAGON,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_LAPTOPGUN,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PHOENIX,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PSYCHOSISGUN,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_TRENTSMAGNUM,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_FARSIGHT,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_CLASSICMODE,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		(uintptr_t)&cheatGetMarquee,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsSoloWeaponsMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_122, // "Weapons for Jo in Solo"
	g_CheatsSoloWeaponsMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsClassicWeaponsMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PP9I,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_CC13,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_KL01313,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_KF7SPECIAL,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ZZT,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_DMC,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_AR53,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_RCP45,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x000000c8,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		L_MPWEAPONS_144, // "Win Golds on the firing range to enable classic guns."
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x000000c8,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsClassicWeaponsMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_123, // "Classic Weapons for Jo in Solo"
	g_CheatsClassicWeaponsMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsWeaponsMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_CLASSICSIGHT,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_UNLIMITEDAMMOLAPTOP,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_HURRICANEFISTS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_UNLIMITEDAMMO,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_UNLIMITEDAMMONORELOADS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_XRAYSCANNER,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_RTRACKER,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ALLGUNS,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		(uintptr_t)&cheatGetMarquee,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsWeaponsMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_120, // "Weapons"
	g_CheatsWeaponsMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsBuddiesMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_VELVETDARK,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_PUGILIST,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_HOTSHOT,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_HITANDRUN,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_ALIEN,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_MARQUEE,
		0,
		MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_MARQUEE_FADEBOTHSIDES,
		(uintptr_t)&cheatGetMarquee,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsBuddiesMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPWEAPONS_121, // "Buddies"
	g_CheatsBuddiesMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};

struct menuitem g_CheatsMenuItems[] = {
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_118, // "Fun"
		0,
		(void *)&g_CheatsFunMenuDialog,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_119, // "Gameplay"
		0,
		(void *)&g_CheatsGameplayMenuDialog,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_122, // "Weapons for Jo in Solo"
		0,
		(void *)&g_CheatsSoloWeaponsMenuDialog,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_123, // "Classic Weapons for Jo in Solo"
		0,
		(void *)&g_CheatsClassicWeaponsMenuDialog,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_120, // "Weapons"
		0,
		(void *)&g_CheatsWeaponsMenuDialog,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_OPENSDIALOG,
		L_MPWEAPONS_121, // "Buddies"
		0,
		(void *)&g_CheatsBuddiesMenuDialog,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		0,
		L_MPWEAPONS_217, // "Turn off all Cheats"
		0,
		cheatMenuHandleTurnOffAllCheats,
	},
#ifndef PLATFORM_N64
	{
		MENUITEMTYPE_CHECKBOX,
		CHEAT_UNLOCKALLCONTENT,
		0,
		(uintptr_t)&cheatGetNameIfUnlocked,
		0,
		cheatCheckboxMenuHandler,
	},
#endif
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0x00000096,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPMENU_477, // "Done"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_CheatsMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPMENU_476, // "Cheats"
	g_CheatsMenuItems,
	cheatMenuHandleDialog,
	0,
	NULL,
};
