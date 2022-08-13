#include <ultra64.h>
#include "constants.h"
#include "game/camdraw.h"
#include "game/game_006900.h"
#include "game/body.h"
#include "game/objectives.h"
#include "game/quaternion.h"
#include "game/bondgun.h"
#include "game/tex.h"
#include "game/camera.h"
#include "game/player.h"
#include "game/modeldef.h"
#include "game/savebuffer.h"
#include "game/menugfx.h"
#include "game/menuitem.h"
#include "game/menu.h"
#include "game/filemgr.h"
#include "game/credits.h"
#include "game/game_1531a0.h"
#include "game/file.h"
#include "game/lv.h"
#include "game/mplayer/setup.h"
#include "game/music.h"
#include "game/texdecompress.h"
#include "game/mplayer/setup.h"
#include "game/challenge.h"
#include "game/training.h"
#include "game/game_1a78b0.h"
#include "game/gamefile.h"
#include "game/gfxmemory.h"
#include "game/lang.h"
#include "game/mplayer/mplayer.h"
#include "game/pak.h"
#include "game/options.h"
#include "game/propobj.h"
#include "bss.h"
#include "lib/joy.h"
#include "lib/vi.h"
#include "lib/main.h"
#include "lib/model.h"
#include "lib/snd.h"
#include "lib/memp.h"
#include "lib/rng.h"
#include "lib/mtx.h"
#include "lib/lib_317f0.h"
#include "data.h"
#include "types.h"

#if VERSION >= VERSION_PAL_FINAL
char g_CheatMarqueeString[300];
#elif VERSION >= VERSION_NTSC_1_0
char g_CheatMarqueeString[252];
#else
char g_StringPointer[104];
char g_StringPointer2[100];
#endif

u8 *g_BlurBuffer;
s32 var8009dfc0;
u32 var8009dfc4;
struct briefing g_Briefing;
u32 var8009dfe4;
struct missionconfig g_MissionConfig;
struct menu g_Menus[4];
struct menudata g_MenuData;
s32 g_MenuScissorX1;
s32 g_MenuScissorX2;
s32 g_MenuScissorY1;
s32 g_MenuScissorY2;
Vp var800a2048[4][2];

#if VERSION >= VERSION_NTSC_1_0
struct menudialogdef g_PakCannotReadGameBoyMenuDialog;
struct menudialogdef g_PakDamagedMenuDialog;
struct menudialogdef g_PakDataLostMenuDialog;
#else
struct menudialogdef g_PakDamagedMenuDialog;
#endif

struct menudialogdef g_PakFullMenuDialog;
struct menudialogdef g_PakRemovedMenuDialog;
struct menudialogdef g_PakRepairFailedMenuDialog;
struct menudialogdef g_PakRepairSuccessMenuDialog;

#if VERSION >= VERSION_JPN_FINAL
const struct menucolourpalette g_MenuColourPalettes[] = {
	{ 0x20202000, 0x20202000, 0x20202000, 0x4f4f4f00, 0x00000000, 0x00000000, 0x4f4f4f00, 0x4f4f4f00, 0x4f4f4f00, 0x4f4f4f00, 0x00000000, 0x00000000, 0x4f4f4f00, 0x00000000, 0x00000000 },
	{ 0x0060bf7f, 0x0000507f, 0x00f0ff7f, 0xffffffff, 0x00002f9f, 0x00006f7f, 0x00ffffff, 0x007f7fff, 0xffffffff, 0x8fffffff, 0x000044ff, 0x000030ff, 0x7f7fffff, 0xffffffff, 0x6644ff7f },
	{ 0xbf00007f, 0x5000007f, 0xff00007f, 0xffff00ff, 0x2f00009f, 0x6f00007f, 0xff9070ff, 0x7f0000ff, 0xffff00ff, 0xffa090ff, 0x440000ff, 0x003000ff, 0xffff00ff, 0xffffffff, 0xff44447f },
	{ 0x00bf007f, 0x0050007f, 0x00ff007f, 0xffff00ff, 0x002f009f, 0x00ff0028, 0x55ff55ff, 0x006f00af, 0xffffffff, 0x00000000, 0x004400ff, 0x003000ff, 0xffff00ff, 0xffffffff, 0x44ff447f },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff9f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
	{ 0xaaaaaaff, 0xaaaaaa7f, 0xaaaaaaff, 0xffffffff, 0xffffff9f, 0xffffffff, 0xffffffff, 0xffffffff, 0xff8888ff, 0xffffffff, 0x00000000, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
};
#else
const struct menucolourpalette g_MenuColourPalettes[] = {
	{ 0x20202000, 0x20202000, 0x20202000, 0x4f4f4f00, 0x00000000, 0x00000000, 0x4f4f4f00, 0x4f4f4f00, 0x4f4f4f00, 0x4f4f4f00, 0x00000000, 0x00000000, 0x4f4f4f00, 0x00000000, 0x00000000 },
	{ 0x0060bf7f, 0x0000507f, 0x00f0ff7f, 0xffffffff, 0x00002f7f, 0x00006f7f, 0x00ffffff, 0x007f7fff, 0xffffffff, 0x8fffffff, 0x000044ff, 0x000030ff, 0x7f7fffff, 0xffffffff, 0x6644ff7f },
	{ 0xbf00007f, 0x5000007f, 0xff00007f, 0xffff00ff, 0x2f00007f, 0x6f00007f, 0xff7050ff, 0x7f0000ff, 0xffff00ff, 0xff9070ff, 0x440000ff, 0x003000ff, 0xffff00ff, 0xffffffff, 0xff44447f },
	{ 0x00bf007f, 0x0050007f, 0x00ff007f, 0xffff00ff, 0x002f007f, 0x00ff0028, 0x55ff55ff, 0x006f00af, 0xffffffff, 0x00000000, 0x004400ff, 0x003000ff, 0xffff00ff, 0xffffffff, 0x44ff447f },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
	{ 0xaaaaaaff, 0xaaaaaa7f, 0xaaaaaaff, 0xffffffff, 0xffffff2f, 0xffffffff, 0xffffffff, 0xffffffff, 0xff8888ff, 0xffffffff, 0x00000000, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
};
#endif

const struct menucolourpalette g_MenuColourPalettes2[] = {
	{ 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x4f4f4f00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x00000000 },
	{ 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x006f6faf, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x00000000 },
	{ 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x006f6faf, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x00000000 },
	{ 0xffffff00, 0xffffff00, 0xffffff00, 0xff7f0000, 0xffffff00, 0xffffff00, 0x00ffff00, 0x006f6faf, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0x00000000 },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
};

const struct menucolourpalette g_MenuColourPalettes3[] = {
	{ 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x4f4f4f00, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x00000000 },
	{ 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x006f6faf, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x00000000 },
	{ 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x006f6faf, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x00000000 },
	{ 0x44444400, 0x44444400, 0x44444400, 0x00ff0000, 0x44444400, 0x44444400, 0xffff0000, 0x006f6faf, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x44444400, 0x00000000 },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
	{ 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffff7f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff5f, 0xffffffff, 0xffffff7f, 0xffffffff },
};

#if VERSION >= VERSION_NTSC_1_0
char *g_StringPointer = g_CheatMarqueeString;
char *g_StringPointer2 = &g_CheatMarqueeString[VERSION >= VERSION_PAL_FINAL ? 150 : 125];
#endif

s32 g_MpPlayerNum = 0;

void menuPlaySound(s32 menusound)
{
	s32 sound = -1;
	s32 flag1 = false;
	s32 flag2 = false;
	f32 speed = 1;

	switch (menusound) {
	case MENUSOUND_SWIPE:
		sound = SFX_MENU_SWIPE;
		break;
	case MENUSOUND_OPENDIALOG:
		sound = SFX_MENU_OPENDIALOG;
		break;
	case MENUSOUND_FOCUS:
		sound = SFX_MENU_FOCUS;
		break;
	case MENUSOUND_SELECT:
		sound = SFX_MENU_SELECT;
		break;
	case MENUSOUND_ERROR:
		speed = 0.4f;
		sound = SFX_MENU_ERROR;
		flag1 = true;
		break;
	case MENUSOUND_EXPLOSION:
		sound = SFX_EXPLOSION_8098;
		break;
	case MENUSOUND_TOGGLEON:
		sound = SFX_MENU_SELECT;
		break;
	case MENUSOUND_TOGGLEOFF:
		sound = SFX_MENU_SUBFOCUS;
		break;
	case MENUSOUND_SUBFOCUS:
		sound = SFX_MENU_SUBFOCUS;
		break;
	case MENUSOUND_KEYBOARDFOCUS:
		sound = SFX_PICKUP_AMMO;
		flag1 = true;
		flag2 = true;
		speed = 3.5f;
		break;
	case MENUSOUND_KEYBOARDCANCEL:
		sound = SFX_MENU_CANCEL;
		flag1 = true;
		speed = 0.41904801130295f;
		break;
	}

	if (sound != -1) {
		struct sndstate *handle;

#if VERSION >= VERSION_NTSC_1_0
		OSPri prevpri = osGetThreadPri(NULL);
		osSetThreadPri(0, osGetThreadPri(&g_AudioManager.thread) + 1);
#endif

		handle = sndStart(var80095200, sound, NULL, -1, -1, -1, -1, -1);

		if (handle && flag1) {
			audioPostEvent(handle, 16, *(s32 *)&speed);
		}

		if (handle && flag2) {
			audioPostEvent(handle, 8, 0x4000);
		}

#if VERSION >= VERSION_NTSC_1_0
		osSetThreadPri(0, prevpri);
#endif
	}
}

bool menuIsSoloMissionOrMp(void)
{
	switch (g_MenuData.root) {
	case MENUROOT_MAINMENU:
	case MENUROOT_TRAINING:
		if (g_Vars.stagenum == STAGE_CITRAINING) {
			return false;
		}

		return true;
	case MENUROOT_MPPAUSE:
		return true;
	}

	return false;
}

bool currentPlayerIsMenuOpenInSoloOrMp(void)
{
	s32 mpindex = g_Vars.currentplayerstats->mpindex;

	if (menuIsSoloMissionOrMp()) {
		if (mpindex >= 4) {
			mpindex -= 4;
		}

		if (g_Menus[mpindex].curdialog) {
			return true;
		}
	}

	return false;
}

bool func0f0f0c68(void)
{
	if (g_MenuData.bg || g_MenuData.nextbg != 255) {
		return false;
	}

	return true;
}

void menuSetBanner(s32 bannernum, bool allplayers)
{
	if (allplayers) {
		g_MenuData.bannernum = bannernum;
		return;
	}

	g_Menus[g_MpPlayerNum].bannernum = bannernum;
}

#if VERSION >= VERSION_JPN_FINAL
GLOBAL_ASM(
glabel menuRenderBanner
/*  f0f1a38:	27bdff70 */ 	addiu	$sp,$sp,-144
/*  f0f1a3c:	8fae00a4 */ 	lw	$t6,0xa4($sp)
/*  f0f1a40:	afb00038 */ 	sw	$s0,0x38($sp)
/*  f0f1a44:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1a48:	3c088008 */ 	lui	$t0,0x8008
/*  f0f1a4c:	00808025 */ 	move	$s0,$a0
/*  f0f1a50:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0f1a54:	afa50094 */ 	sw	$a1,0x94($sp)
/*  f0f1a58:	afa60098 */ 	sw	$a2,0x98($sp)
/*  f0f1a5c:	afa7009c */ 	sw	$a3,0x9c($sp)
/*  f0f1a60:	8c63014c */ 	lw	$v1,0x14c($v1)
/*  f0f1a64:	11c00008 */ 	beqz	$t6,.JF0f0f1a88
/*  f0f1a68:	8d080148 */ 	lw	$t0,0x148($t0)
/*  f0f1a6c:	3c028008 */ 	lui	$v0,0x8008
/*  f0f1a70:	8c420154 */ 	lw	$v0,0x154($v0)
/*  f0f1a74:	50400005 */ 	beqzl	$v0,.JF0f0f1a8c
/*  f0f1a78:	8faf0098 */ 	lw	$t7,0x98($sp)
/*  f0f1a7c:	3c088008 */ 	lui	$t0,0x8008
/*  f0f1a80:	00401825 */ 	move	$v1,$v0
/*  f0f1a84:	8d080150 */ 	lw	$t0,0x150($t0)
.JF0f0f1a88:
/*  f0f1a88:	8faf0098 */ 	lw	$t7,0x98($sp)
.JF0f0f1a8c:
/*  f0f1a8c:	8fb800a0 */ 	lw	$t8,0xa0($sp)
/*  f0f1a90:	8fa900a8 */ 	lw	$t1,0xa8($sp)
/*  f0f1a94:	3c0b8007 */ 	lui	$t3,0x8007
/*  f0f1a98:	01f83021 */ 	addu	$a2,$t7,$t8
/*  f0f1a9c:	256b199c */ 	addiu	$t3,$t3,0x199c
/*  f0f1aa0:	00095040 */ 	sll	$t2,$t1,0x1
/*  f0f1aa4:	04c10003 */ 	bgez	$a2,.JF0f0f1ab4
/*  f0f1aa8:	0006c843 */ 	sra	$t9,$a2,0x1
/*  f0f1aac:	24c10001 */ 	addiu	$at,$a2,0x1
/*  f0f1ab0:	0001c843 */ 	sra	$t9,$at,0x1
.JF0f0f1ab4:
/*  f0f1ab4:	014b1021 */ 	addu	$v0,$t2,$t3
/*  f0f1ab8:	94440000 */ 	lhu	$a0,0x0($v0)
/*  f0f1abc:	afa20040 */ 	sw	$v0,0x40($sp)
/*  f0f1ac0:	afb90084 */ 	sw	$t9,0x84($sp)
/*  f0f1ac4:	afa30064 */ 	sw	$v1,0x64($sp)
/*  f0f1ac8:	0fc5baa5 */ 	jal	langGet
/*  f0f1acc:	afa80060 */ 	sw	$t0,0x60($sp)
/*  f0f1ad0:	8fac0060 */ 	lw	$t4,0x60($sp)
/*  f0f1ad4:	27a4007c */ 	addiu	$a0,$sp,0x7c
/*  f0f1ad8:	27a50078 */ 	addiu	$a1,$sp,0x78
/*  f0f1adc:	00403025 */ 	move	$a2,$v0
/*  f0f1ae0:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f1ae4:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f1ae8:	0fc55d49 */ 	jal	textMeasure
/*  f0f1aec:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0f1af0:	0fc5baa5 */ 	jal	langGet
/*  f0f1af4:	240451ef */ 	li	$a0,0x51ef
/*  f0f1af8:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f1afc:	27a40074 */ 	addiu	$a0,$sp,0x74
/*  f0f1b00:	27a50070 */ 	addiu	$a1,$sp,0x70
/*  f0f1b04:	00403025 */ 	move	$a2,$v0
/*  f0f1b08:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f1b0c:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f1b10:	0fc55d49 */ 	jal	textMeasure
/*  f0f1b14:	afad0010 */ 	sw	$t5,0x10($sp)
/*  f0f1b18:	8fa2007c */ 	lw	$v0,0x7c($sp)
/*  f0f1b1c:	8fa30074 */ 	lw	$v1,0x74($sp)
/*  f0f1b20:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1b24:	02002025 */ 	move	$a0,$s0
/*  f0f1b28:	00437021 */ 	addu	$t6,$v0,$v1
/*  f0f1b2c:	05c10003 */ 	bgez	$t6,.JF0f0f1b3c
/*  f0f1b30:	000e7843 */ 	sra	$t7,$t6,0x1
/*  f0f1b34:	25c10001 */ 	addiu	$at,$t6,0x1
/*  f0f1b38:	00017843 */ 	sra	$t7,$at,0x1
.JF0f0f1b3c:
/*  f0f1b3c:	00cf3023 */ 	subu	$a2,$a2,$t7
/*  f0f1b40:	00c2c821 */ 	addu	$t9,$a2,$v0
/*  f0f1b44:	03234821 */ 	addu	$t1,$t9,$v1
/*  f0f1b48:	24d8fffc */ 	addiu	$t8,$a2,-4
/*  f0f1b4c:	252a0007 */ 	addiu	$t2,$t1,0x7
/*  f0f1b50:	afa60080 */ 	sw	$a2,0x80($sp)
/*  f0f1b54:	afb8006c */ 	sw	$t8,0x6c($sp)
/*  f0f1b58:	afaa0068 */ 	sw	$t2,0x68($sp)
/*  f0f1b5c:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1b60:	0fc54bce */ 	jal	textSetPrimColour
/*  f0f1b64:	2405007f */ 	li	$a1,0x7f
/*  f0f1b68:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1b6c:	246300f4 */ 	addiu	$v1,$v1,0xf4
/*  f0f1b70:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f1b74:	8fab009c */ 	lw	$t3,0x9c($sp)
/*  f0f1b78:	8fb900a0 */ 	lw	$t9,0xa0($sp)
/*  f0f1b7c:	3c01f600 */ 	lui	$at,0xf600
/*  f0f1b80:	016c0019 */ 	multu	$t3,$t4
/*  f0f1b84:	332903ff */ 	andi	$t1,$t9,0x3ff
/*  f0f1b88:	00095080 */ 	sll	$t2,$t1,0x2
/*  f0f1b8c:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f1b90:	00006812 */ 	mflo	$t5
/*  f0f1b94:	31ae03ff */ 	andi	$t6,$t5,0x3ff
/*  f0f1b98:	000e7b80 */ 	sll	$t7,$t6,0xe
/*  f0f1b9c:	01e1c025 */ 	or	$t8,$t7,$at
/*  f0f1ba0:	030a5825 */ 	or	$t3,$t8,$t2
/*  f0f1ba4:	ac4b0000 */ 	sw	$t3,0x0($v0)
/*  f0f1ba8:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f1bac:	8fac0094 */ 	lw	$t4,0x94($sp)
/*  f0f1bb0:	8fa90098 */ 	lw	$t1,0x98($sp)
/*  f0f1bb4:	018d0019 */ 	multu	$t4,$t5
/*  f0f1bb8:	313803ff */ 	andi	$t8,$t1,0x3ff
/*  f0f1bbc:	00185080 */ 	sll	$t2,$t8,0x2
/*  f0f1bc0:	00007012 */ 	mflo	$t6
/*  f0f1bc4:	31cf03ff */ 	andi	$t7,$t6,0x3ff
/*  f0f1bc8:	000fcb80 */ 	sll	$t9,$t7,0xe
/*  f0f1bcc:	032a5825 */ 	or	$t3,$t9,$t2
/*  f0f1bd0:	0fc54be5 */ 	jal	text0f153838
/*  f0f1bd4:	ac4b0004 */ 	sw	$t3,0x4($v0)
/*  f0f1bd8:	3c10f600 */ 	lui	$s0,0xf600
/*  f0f1bdc:	00402025 */ 	move	$a0,$v0
/*  f0f1be0:	0fc54bce */ 	jal	textSetPrimColour
/*  f0f1be4:	24057f7f */ 	li	$a1,0x7f7f
/*  f0f1be8:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1bec:	246300f4 */ 	addiu	$v1,$v1,0xf4
/*  f0f1bf0:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f1bf4:	8fac009c */ 	lw	$t4,0x9c($sp)
/*  f0f1bf8:	8fb90068 */ 	lw	$t9,0x68($sp)
/*  f0f1bfc:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f1c00:	018d0019 */ 	multu	$t4,$t5
/*  f0f1c04:	332a03ff */ 	andi	$t2,$t9,0x3ff
/*  f0f1c08:	000a5880 */ 	sll	$t3,$t2,0x2
/*  f0f1c0c:	00007012 */ 	mflo	$t6
/*  f0f1c10:	31cf03ff */ 	andi	$t7,$t6,0x3ff
/*  f0f1c14:	000f4b80 */ 	sll	$t1,$t7,0xe
/*  f0f1c18:	0130c025 */ 	or	$t8,$t1,$s0
/*  f0f1c1c:	030b6025 */ 	or	$t4,$t8,$t3
/*  f0f1c20:	ac4c0000 */ 	sw	$t4,0x0($v0)
/*  f0f1c24:	8c6e0000 */ 	lw	$t6,0x0($v1)
/*  f0f1c28:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1c2c:	8faa006c */ 	lw	$t2,0x6c($sp)
/*  f0f1c30:	01ae0019 */ 	multu	$t5,$t6
/*  f0f1c34:	315803ff */ 	andi	$t8,$t2,0x3ff
/*  f0f1c38:	00185880 */ 	sll	$t3,$t8,0x2
/*  f0f1c3c:	00007812 */ 	mflo	$t7
/*  f0f1c40:	31e903ff */ 	andi	$t1,$t7,0x3ff
/*  f0f1c44:	0009cb80 */ 	sll	$t9,$t1,0xe
/*  f0f1c48:	032b6025 */ 	or	$t4,$t9,$t3
/*  f0f1c4c:	0fc54be5 */ 	jal	text0f153838
/*  f0f1c50:	ac4c0004 */ 	sw	$t4,0x4($v0)
/*  f0f1c54:	3c057f7f */ 	lui	$a1,0x7f7f
/*  f0f1c58:	34a5ff7f */ 	ori	$a1,$a1,0xff7f
/*  f0f1c5c:	0fc54bce */ 	jal	textSetPrimColour
/*  f0f1c60:	00402025 */ 	move	$a0,$v0
/*  f0f1c64:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1c68:	246300f4 */ 	addiu	$v1,$v1,0xf4
/*  f0f1c6c:	8c6e0000 */ 	lw	$t6,0x0($v1)
/*  f0f1c70:	8fad009c */ 	lw	$t5,0x9c($sp)
/*  f0f1c74:	8fb90068 */ 	lw	$t9,0x68($sp)
/*  f0f1c78:	24440010 */ 	addiu	$a0,$v0,0x10
/*  f0f1c7c:	01ae0019 */ 	multu	$t5,$t6
/*  f0f1c80:	272b0004 */ 	addiu	$t3,$t9,0x4
/*  f0f1c84:	316c03ff */ 	andi	$t4,$t3,0x3ff
/*  f0f1c88:	000c6880 */ 	sll	$t5,$t4,0x2
/*  f0f1c8c:	00007812 */ 	mflo	$t7
/*  f0f1c90:	31e903ff */ 	andi	$t1,$t7,0x3ff
/*  f0f1c94:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0f1c98:	0150c025 */ 	or	$t8,$t2,$s0
/*  f0f1c9c:	030d7025 */ 	or	$t6,$t8,$t5
/*  f0f1ca0:	ac4e0000 */ 	sw	$t6,0x0($v0)
/*  f0f1ca4:	8c690000 */ 	lw	$t1,0x0($v1)
/*  f0f1ca8:	8faf0094 */ 	lw	$t7,0x94($sp)
/*  f0f1cac:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f1cb0:	01e90019 */ 	multu	$t7,$t1
/*  f0f1cb4:	25980002 */ 	addiu	$t8,$t4,0x2
/*  f0f1cb8:	330d03ff */ 	andi	$t5,$t8,0x3ff
/*  f0f1cbc:	000d7080 */ 	sll	$t6,$t5,0x2
/*  f0f1cc0:	00005012 */ 	mflo	$t2
/*  f0f1cc4:	315903ff */ 	andi	$t9,$t2,0x3ff
/*  f0f1cc8:	00195b80 */ 	sll	$t3,$t9,0xe
/*  f0f1ccc:	016e7825 */ 	or	$t7,$t3,$t6
/*  f0f1cd0:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f1cd4:	8c6a0000 */ 	lw	$t2,0x0($v1)
/*  f0f1cd8:	8fa9009c */ 	lw	$t1,0x9c($sp)
/*  f0f1cdc:	8fab006c */ 	lw	$t3,0x6c($sp)
/*  f0f1ce0:	012a0019 */ 	multu	$t1,$t2
/*  f0f1ce4:	256efffe */ 	addiu	$t6,$t3,-2
/*  f0f1ce8:	31cf03ff */ 	andi	$t7,$t6,0x3ff
/*  f0f1cec:	000f4880 */ 	sll	$t1,$t7,0x2
/*  f0f1cf0:	0000c812 */ 	mflo	$t9
/*  f0f1cf4:	332c03ff */ 	andi	$t4,$t9,0x3ff
/*  f0f1cf8:	000cc380 */ 	sll	$t8,$t4,0xe
/*  f0f1cfc:	03106825 */ 	or	$t5,$t8,$s0
/*  f0f1d00:	01a95025 */ 	or	$t2,$t5,$t1
/*  f0f1d04:	ac4a0008 */ 	sw	$t2,0x8($v0)
/*  f0f1d08:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f1d0c:	8fb90094 */ 	lw	$t9,0x94($sp)
/*  f0f1d10:	8faf006c */ 	lw	$t7,0x6c($sp)
/*  f0f1d14:	032c0019 */ 	multu	$t9,$t4
/*  f0f1d18:	25edfffc */ 	addiu	$t5,$t7,-4
/*  f0f1d1c:	31a903ff */ 	andi	$t1,$t5,0x3ff
/*  f0f1d20:	00095080 */ 	sll	$t2,$t1,0x2
/*  f0f1d24:	0000c012 */ 	mflo	$t8
/*  f0f1d28:	330b03ff */ 	andi	$t3,$t8,0x3ff
/*  f0f1d2c:	000b7380 */ 	sll	$t6,$t3,0xe
/*  f0f1d30:	01cac825 */ 	or	$t9,$t6,$t2
/*  f0f1d34:	0fc54be5 */ 	jal	text0f153838
/*  f0f1d38:	ac59000c */ 	sw	$t9,0xc($v0)
/*  f0f1d3c:	0fc54b55 */ 	jal	text0f153628
/*  f0f1d40:	00402025 */ 	move	$a0,$v0
/*  f0f1d44:	8fac0094 */ 	lw	$t4,0x94($sp)
/*  f0f1d48:	8fb800ac */ 	lw	$t8,0xac($sp)
/*  f0f1d4c:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0f1d50:	8fa900b0 */ 	lw	$t1,0xb0($sp)
/*  f0f1d54:	01985821 */ 	addu	$t3,$t4,$t8
/*  f0f1d58:	016f6821 */ 	addu	$t5,$t3,$t7
/*  f0f1d5c:	01a91823 */ 	subu	$v1,$t5,$t1
/*  f0f1d60:	8faa0078 */ 	lw	$t2,0x78($sp)
/*  f0f1d64:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1d68:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f1d6c:	00408025 */ 	move	$s0,$v0
/*  f0f1d70:	04610003 */ 	bgez	$v1,.JF0f0f1d80
/*  f0f1d74:	00037043 */ 	sra	$t6,$v1,0x1
/*  f0f1d78:	24610001 */ 	addiu	$at,$v1,0x1
/*  f0f1d7c:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0f1d80:
/*  f0f1d80:	01c01825 */ 	move	$v1,$t6
/*  f0f1d84:	05410003 */ 	bgez	$t2,.JF0f0f1d94
/*  f0f1d88:	000ac843 */ 	sra	$t9,$t2,0x1
/*  f0f1d8c:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f1d90:	0001c843 */ 	sra	$t9,$at,0x1
.JF0f0f1d94:
/*  f0f1d94:	00796023 */ 	subu	$t4,$v1,$t9
/*  f0f1d98:	25980002 */ 	addiu	$t8,$t4,0x2
/*  f0f1d9c:	afb80088 */ 	sw	$t8,0x88($sp)
/*  f0f1da0:	24c60002 */ 	addiu	$a2,$a2,0x2
/*  f0f1da4:	95640000 */ 	lhu	$a0,0x0($t3)
/*  f0f1da8:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1dac:	0fc5baa5 */ 	jal	langGet
/*  f0f1db0:	afa30044 */ 	sw	$v1,0x44($sp)
/*  f0f1db4:	0c002eeb */ 	jal	viGetWidth
/*  f0f1db8:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1dbc:	0c002eeb */ 	jal	viGetWidth
/*  f0f1dc0:	a7a20048 */ 	sh	$v0,0x48($sp)
/*  f0f1dc4:	8faf0064 */ 	lw	$t7,0x64($sp)
/*  f0f1dc8:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f1dcc:	87ae0048 */ 	lh	$t6,0x48($sp)
/*  f0f1dd0:	240900ff */ 	li	$t1,0xff
/*  f0f1dd4:	afa90018 */ 	sw	$t1,0x18($sp)
/*  f0f1dd8:	02002025 */ 	move	$a0,$s0
/*  f0f1ddc:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1de0:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1de4:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1de8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1dec:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1df0:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1df4:	afaf0010 */ 	sw	$t7,0x10($sp)
/*  f0f1df8:	afad0014 */ 	sw	$t5,0x14($sp)
/*  f0f1dfc:	0fc55886 */ 	jal	textRenderProjected
/*  f0f1e00:	afae001c */ 	sw	$t6,0x1c($sp)
/*  f0f1e04:	8fb90070 */ 	lw	$t9,0x70($sp)
/*  f0f1e08:	8faa0044 */ 	lw	$t2,0x44($sp)
/*  f0f1e0c:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1e10:	00408025 */ 	move	$s0,$v0
/*  f0f1e14:	07210003 */ 	bgez	$t9,.JF0f0f1e24
/*  f0f1e18:	00196043 */ 	sra	$t4,$t9,0x1
/*  f0f1e1c:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0f1e20:	00016043 */ 	sra	$t4,$at,0x1
.JF0f0f1e24:
/*  f0f1e24:	014cc023 */ 	subu	$t8,$t2,$t4
/*  f0f1e28:	270b0002 */ 	addiu	$t3,$t8,0x2
/*  f0f1e2c:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f1e30:	afab0088 */ 	sw	$t3,0x88($sp)
/*  f0f1e34:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1e38:	0fc5baa5 */ 	jal	langGet
/*  f0f1e3c:	240451ef */ 	li	$a0,0x51ef
/*  f0f1e40:	0c002eeb */ 	jal	viGetWidth
/*  f0f1e44:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1e48:	0c002eeb */ 	jal	viGetWidth
/*  f0f1e4c:	a7a20048 */ 	sh	$v0,0x48($sp)
/*  f0f1e50:	8faf0064 */ 	lw	$t7,0x64($sp)
/*  f0f1e54:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f1e58:	87ae0048 */ 	lh	$t6,0x48($sp)
/*  f0f1e5c:	240900ff */ 	li	$t1,0xff
/*  f0f1e60:	afa90018 */ 	sw	$t1,0x18($sp)
/*  f0f1e64:	02002025 */ 	move	$a0,$s0
/*  f0f1e68:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1e6c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1e70:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1e74:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1e78:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1e7c:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1e80:	afaf0010 */ 	sw	$t7,0x10($sp)
/*  f0f1e84:	afad0014 */ 	sw	$t5,0x14($sp)
/*  f0f1e88:	0fc55886 */ 	jal	textRenderProjected
/*  f0f1e8c:	afae001c */ 	sw	$t6,0x1c($sp)
/*  f0f1e90:	8faa0078 */ 	lw	$t2,0x78($sp)
/*  f0f1e94:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f1e98:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f1e9c:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f1ea0:	00408025 */ 	move	$s0,$v0
/*  f0f1ea4:	05410003 */ 	bgez	$t2,.JF0f0f1eb4
/*  f0f1ea8:	000a6043 */ 	sra	$t4,$t2,0x1
/*  f0f1eac:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f1eb0:	00016043 */ 	sra	$t4,$at,0x1
.JF0f0f1eb4:
/*  f0f1eb4:	032cc023 */ 	subu	$t8,$t9,$t4
/*  f0f1eb8:	afb80088 */ 	sw	$t8,0x88($sp)
/*  f0f1ebc:	95640000 */ 	lhu	$a0,0x0($t3)
/*  f0f1ec0:	0fc5baa5 */ 	jal	langGet
/*  f0f1ec4:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1ec8:	0c002eeb */ 	jal	viGetWidth
/*  f0f1ecc:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1ed0:	0c002eeb */ 	jal	viGetWidth
/*  f0f1ed4:	a7a20048 */ 	sh	$v0,0x48($sp)
/*  f0f1ed8:	8faf0064 */ 	lw	$t7,0x64($sp)
/*  f0f1edc:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f1ee0:	87ae0048 */ 	lh	$t6,0x48($sp)
/*  f0f1ee4:	3c09bfbf */ 	lui	$t1,0xbfbf
/*  f0f1ee8:	3529ffff */ 	ori	$t1,$t1,0xffff
/*  f0f1eec:	afa90018 */ 	sw	$t1,0x18($sp)
/*  f0f1ef0:	02002025 */ 	move	$a0,$s0
/*  f0f1ef4:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1ef8:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1efc:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1f00:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1f04:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1f08:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1f0c:	afaf0010 */ 	sw	$t7,0x10($sp)
/*  f0f1f10:	afad0014 */ 	sw	$t5,0x14($sp)
/*  f0f1f14:	0fc55886 */ 	jal	textRenderProjected
/*  f0f1f18:	afae001c */ 	sw	$t6,0x1c($sp)
/*  f0f1f1c:	8fb90070 */ 	lw	$t9,0x70($sp)
/*  f0f1f20:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1f24:	8faa0044 */ 	lw	$t2,0x44($sp)
/*  f0f1f28:	00408025 */ 	move	$s0,$v0
/*  f0f1f2c:	07210003 */ 	bgez	$t9,.JF0f0f1f3c
/*  f0f1f30:	00196043 */ 	sra	$t4,$t9,0x1
/*  f0f1f34:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0f1f38:	00016043 */ 	sra	$t4,$at,0x1
.JF0f0f1f3c:
/*  f0f1f3c:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f1f40:	014cc023 */ 	subu	$t8,$t2,$t4
/*  f0f1f44:	afb80088 */ 	sw	$t8,0x88($sp)
/*  f0f1f48:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1f4c:	0fc5baa5 */ 	jal	langGet
/*  f0f1f50:	240451ef */ 	li	$a0,0x51ef
/*  f0f1f54:	0c002eeb */ 	jal	viGetWidth
/*  f0f1f58:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1f5c:	0c002eeb */ 	jal	viGetWidth
/*  f0f1f60:	a7a20048 */ 	sh	$v0,0x48($sp)
/*  f0f1f64:	8fab0064 */ 	lw	$t3,0x64($sp)
/*  f0f1f68:	8faf0060 */ 	lw	$t7,0x60($sp)
/*  f0f1f6c:	87a90048 */ 	lh	$t1,0x48($sp)
/*  f0f1f70:	3c0dbfbf */ 	lui	$t5,0xbfbf
/*  f0f1f74:	35adffff */ 	ori	$t5,$t5,0xffff
/*  f0f1f78:	afad0018 */ 	sw	$t5,0x18($sp)
/*  f0f1f7c:	02002025 */ 	move	$a0,$s0
/*  f0f1f80:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1f84:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1f88:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1f8c:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1f90:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1f94:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1f98:	afab0010 */ 	sw	$t3,0x10($sp)
/*  f0f1f9c:	afaf0014 */ 	sw	$t7,0x14($sp)
/*  f0f1fa0:	0fc55886 */ 	jal	textRenderProjected
/*  f0f1fa4:	afa9001c */ 	sw	$t1,0x1c($sp)
/*  f0f1fa8:	0fc54bb7 */ 	jal	text0f153780
/*  f0f1fac:	00402025 */ 	move	$a0,$v0
/*  f0f1fb0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f1fb4:	8fb00038 */ 	lw	$s0,0x38($sp)
/*  f0f1fb8:	27bd0090 */ 	addiu	$sp,$sp,0x90
/*  f0f1fbc:	03e00008 */ 	jr	$ra
/*  f0f1fc0:	00000000 */ 	nop
);
#elif VERSION >= VERSION_PAL_FINAL
GLOBAL_ASM(
glabel menuRenderBanner
/*  f0f1408:	27bdff70 */ 	addiu	$sp,$sp,-144
/*  f0f140c:	8fae00a4 */ 	lw	$t6,0xa4($sp)
/*  f0f1410:	afb10038 */ 	sw	$s1,0x38($sp)
/*  f0f1414:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1418:	3c088008 */ 	lui	$t0,0x8008
/*  f0f141c:	00808825 */ 	move	$s1,$a0
/*  f0f1420:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0f1424:	afb00034 */ 	sw	$s0,0x34($sp)
/*  f0f1428:	afa50094 */ 	sw	$a1,0x94($sp)
/*  f0f142c:	afa60098 */ 	sw	$a2,0x98($sp)
/*  f0f1430:	afa7009c */ 	sw	$a3,0x9c($sp)
/*  f0f1434:	8c630070 */ 	lw	$v1,0x70($v1)
/*  f0f1438:	11c00008 */ 	beqz	$t6,.PF0f0f145c
/*  f0f143c:	8d08006c */ 	lw	$t0,0x6c($t0)
/*  f0f1440:	3c028008 */ 	lui	$v0,0x8008
/*  f0f1444:	8c420078 */ 	lw	$v0,0x78($v0)
/*  f0f1448:	50400005 */ 	beqzl	$v0,.PF0f0f1460
/*  f0f144c:	8faf0098 */ 	lw	$t7,0x98($sp)
/*  f0f1450:	3c088008 */ 	lui	$t0,0x8008
/*  f0f1454:	00401825 */ 	move	$v1,$v0
/*  f0f1458:	8d080074 */ 	lw	$t0,0x74($t0)
.PF0f0f145c:
/*  f0f145c:	8faf0098 */ 	lw	$t7,0x98($sp)
.PF0f0f1460:
/*  f0f1460:	8fb800a0 */ 	lw	$t8,0xa0($sp)
/*  f0f1464:	8fa900a8 */ 	lw	$t1,0xa8($sp)
/*  f0f1468:	3c0b8007 */ 	lui	$t3,0x8007
/*  f0f146c:	01f83021 */ 	addu	$a2,$t7,$t8
/*  f0f1470:	256b172c */ 	addiu	$t3,$t3,0x172c
/*  f0f1474:	00095040 */ 	sll	$t2,$t1,0x1
/*  f0f1478:	04c10003 */ 	bgez	$a2,.PF0f0f1488
/*  f0f147c:	0006c843 */ 	sra	$t9,$a2,0x1
/*  f0f1480:	24c10001 */ 	addiu	$at,$a2,0x1
/*  f0f1484:	0001c843 */ 	sra	$t9,$at,0x1
.PF0f0f1488:
/*  f0f1488:	014b1021 */ 	addu	$v0,$t2,$t3
/*  f0f148c:	94440000 */ 	lhu	$a0,0x0($v0)
/*  f0f1490:	afa20040 */ 	sw	$v0,0x40($sp)
/*  f0f1494:	afb90084 */ 	sw	$t9,0x84($sp)
/*  f0f1498:	afa30064 */ 	sw	$v1,0x64($sp)
/*  f0f149c:	0fc5bdaa */ 	jal	langGet
/*  f0f14a0:	afa80060 */ 	sw	$t0,0x60($sp)
/*  f0f14a4:	8fac0060 */ 	lw	$t4,0x60($sp)
/*  f0f14a8:	27a4007c */ 	addiu	$a0,$sp,0x7c
/*  f0f14ac:	27a50078 */ 	addiu	$a1,$sp,0x78
/*  f0f14b0:	00403025 */ 	move	$a2,$v0
/*  f0f14b4:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f14b8:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f14bc:	0fc5609a */ 	jal	textMeasure
/*  f0f14c0:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0f14c4:	0fc5bdaa */ 	jal	langGet
/*  f0f14c8:	240451ef */ 	li	$a0,0x51ef
/*  f0f14cc:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f14d0:	27a40074 */ 	addiu	$a0,$sp,0x74
/*  f0f14d4:	27a50070 */ 	addiu	$a1,$sp,0x70
/*  f0f14d8:	00403025 */ 	move	$a2,$v0
/*  f0f14dc:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f14e0:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f14e4:	0fc5609a */ 	jal	textMeasure
/*  f0f14e8:	afad0010 */ 	sw	$t5,0x10($sp)
/*  f0f14ec:	8fae0040 */ 	lw	$t6,0x40($sp)
/*  f0f14f0:	240151eb */ 	li	$at,0x51eb
/*  f0f14f4:	02202025 */ 	move	$a0,$s1
/*  f0f14f8:	95cf0000 */ 	lhu	$t7,0x0($t6)
/*  f0f14fc:	55e10005 */ 	bnel	$t7,$at,.PF0f0f1514
/*  f0f1500:	8fa2007c */ 	lw	$v0,0x7c($sp)
/*  f0f1504:	8fb80078 */ 	lw	$t8,0x78($sp)
/*  f0f1508:	27190007 */ 	addiu	$t9,$t8,0x7
/*  f0f150c:	afb90078 */ 	sw	$t9,0x78($sp)
/*  f0f1510:	8fa2007c */ 	lw	$v0,0x7c($sp)
.PF0f0f1514:
/*  f0f1514:	8fa30074 */ 	lw	$v1,0x74($sp)
/*  f0f1518:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f151c:	2405007f */ 	li	$a1,0x7f
/*  f0f1520:	00434821 */ 	addu	$t1,$v0,$v1
/*  f0f1524:	05210003 */ 	bgez	$t1,.PF0f0f1534
/*  f0f1528:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f152c:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f1530:	00015043 */ 	sra	$t2,$at,0x1
.PF0f0f1534:
/*  f0f1534:	00ca3023 */ 	subu	$a2,$a2,$t2
/*  f0f1538:	00c26021 */ 	addu	$t4,$a2,$v0
/*  f0f153c:	01836821 */ 	addu	$t5,$t4,$v1
/*  f0f1540:	24cbfffc */ 	addiu	$t3,$a2,-4
/*  f0f1544:	25ae0007 */ 	addiu	$t6,$t5,0x7
/*  f0f1548:	afa60080 */ 	sw	$a2,0x80($sp)
/*  f0f154c:	afab006c */ 	sw	$t3,0x6c($sp)
/*  f0f1550:	afae0068 */ 	sw	$t6,0x68($sp)
/*  f0f1554:	0fc5513c */ 	jal	textSetPrimColour
/*  f0f1558:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f155c:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1560:	24630020 */ 	addiu	$v1,$v1,0x20
/*  f0f1564:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0f1568:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0f156c:	8fac00a0 */ 	lw	$t4,0xa0($sp)
/*  f0f1570:	3c10f600 */ 	lui	$s0,0xf600
/*  f0f1574:	01f80019 */ 	multu	$t7,$t8
/*  f0f1578:	318d03ff */ 	andi	$t5,$t4,0x3ff
/*  f0f157c:	000d7080 */ 	sll	$t6,$t5,0x2
/*  f0f1580:	8fb1006c */ 	lw	$s1,0x6c($sp)
/*  f0f1584:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f1588:	0000c812 */ 	mflo	$t9
/*  f0f158c:	332903ff */ 	andi	$t1,$t9,0x3ff
/*  f0f1590:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0f1594:	01505825 */ 	or	$t3,$t2,$s0
/*  f0f1598:	016e7825 */ 	or	$t7,$t3,$t6
/*  f0f159c:	ac4f0000 */ 	sw	$t7,0x0($v0)
/*  f0f15a0:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f15a4:	8fb80094 */ 	lw	$t8,0x94($sp)
/*  f0f15a8:	8fad0098 */ 	lw	$t5,0x98($sp)
/*  f0f15ac:	03190019 */ 	multu	$t8,$t9
/*  f0f15b0:	31ab03ff */ 	andi	$t3,$t5,0x3ff
/*  f0f15b4:	000b7080 */ 	sll	$t6,$t3,0x2
/*  f0f15b8:	00004812 */ 	mflo	$t1
/*  f0f15bc:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f15c0:	000a6380 */ 	sll	$t4,$t2,0xe
/*  f0f15c4:	018e7825 */ 	or	$t7,$t4,$t6
/*  f0f15c8:	0fc55153 */ 	jal	text0f153838
/*  f0f15cc:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f15d0:	00402025 */ 	move	$a0,$v0
/*  f0f15d4:	0fc5513c */ 	jal	textSetPrimColour
/*  f0f15d8:	24057f7f */ 	li	$a1,0x7f7f
/*  f0f15dc:	3c038008 */ 	lui	$v1,0x8008
/*  f0f15e0:	24630020 */ 	addiu	$v1,$v1,0x20
/*  f0f15e4:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f15e8:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f15ec:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f15f0:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f15f4:	03190019 */ 	multu	$t8,$t9
/*  f0f15f8:	318e03ff */ 	andi	$t6,$t4,0x3ff
/*  f0f15fc:	000e7880 */ 	sll	$t7,$t6,0x2
/*  f0f1600:	322e03ff */ 	andi	$t6,$s1,0x3ff
/*  f0f1604:	00004812 */ 	mflo	$t1
/*  f0f1608:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f160c:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f1610:	01b05825 */ 	or	$t3,$t5,$s0
/*  f0f1614:	016fc025 */ 	or	$t8,$t3,$t7
/*  f0f1618:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0f161c:	8c690000 */ 	lw	$t1,0x0($v1)
/*  f0f1620:	8fb90094 */ 	lw	$t9,0x94($sp)
/*  f0f1624:	000e5880 */ 	sll	$t3,$t6,0x2
/*  f0f1628:	03290019 */ 	multu	$t9,$t1
/*  f0f162c:	00005012 */ 	mflo	$t2
/*  f0f1630:	314d03ff */ 	andi	$t5,$t2,0x3ff
/*  f0f1634:	000d6380 */ 	sll	$t4,$t5,0xe
/*  f0f1638:	018b7825 */ 	or	$t7,$t4,$t3
/*  f0f163c:	0fc55153 */ 	jal	text0f153838
/*  f0f1640:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f1644:	3c057f7f */ 	lui	$a1,0x7f7f
/*  f0f1648:	34a5ff7f */ 	ori	$a1,$a1,0xff7f
/*  f0f164c:	0fc5513c */ 	jal	textSetPrimColour
/*  f0f1650:	00402025 */ 	move	$a0,$v0
/*  f0f1654:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1658:	24630020 */ 	addiu	$v1,$v1,0x20
/*  f0f165c:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f1660:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f1664:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f1668:	24440010 */ 	addiu	$a0,$v0,0x10
/*  f0f166c:	03190019 */ 	multu	$t8,$t9
/*  f0f1670:	258b0004 */ 	addiu	$t3,$t4,0x4
/*  f0f1674:	316f03ff */ 	andi	$t7,$t3,0x3ff
/*  f0f1678:	000fc080 */ 	sll	$t8,$t7,0x2
/*  f0f167c:	00004812 */ 	mflo	$t1
/*  f0f1680:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f1684:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f1688:	01b07025 */ 	or	$t6,$t5,$s0
/*  f0f168c:	01d8c825 */ 	or	$t9,$t6,$t8
/*  f0f1690:	ac590000 */ 	sw	$t9,0x0($v0)
/*  f0f1694:	8c6a0000 */ 	lw	$t2,0x0($v1)
/*  f0f1698:	8fa90094 */ 	lw	$t1,0x94($sp)
/*  f0f169c:	8faf0068 */ 	lw	$t7,0x68($sp)
/*  f0f16a0:	012a0019 */ 	multu	$t1,$t2
/*  f0f16a4:	25ee0002 */ 	addiu	$t6,$t7,0x2
/*  f0f16a8:	31d803ff */ 	andi	$t8,$t6,0x3ff
/*  f0f16ac:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f16b0:	00006812 */ 	mflo	$t5
/*  f0f16b4:	31ac03ff */ 	andi	$t4,$t5,0x3ff
/*  f0f16b8:	000c5b80 */ 	sll	$t3,$t4,0xe
/*  f0f16bc:	01794825 */ 	or	$t1,$t3,$t9
/*  f0f16c0:	ac490004 */ 	sw	$t1,0x4($v0)
/*  f0f16c4:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f16c8:	8faa009c */ 	lw	$t2,0x9c($sp)
/*  f0f16cc:	262bfffe */ 	addiu	$t3,$s1,-2
/*  f0f16d0:	317903ff */ 	andi	$t9,$t3,0x3ff
/*  f0f16d4:	014d0019 */ 	multu	$t2,$t5
/*  f0f16d8:	00194880 */ 	sll	$t1,$t9,0x2
/*  f0f16dc:	2639fffc */ 	addiu	$t9,$s1,-4
/*  f0f16e0:	00006012 */ 	mflo	$t4
/*  f0f16e4:	318f03ff */ 	andi	$t7,$t4,0x3ff
/*  f0f16e8:	000f7380 */ 	sll	$t6,$t7,0xe
/*  f0f16ec:	01d0c025 */ 	or	$t8,$t6,$s0
/*  f0f16f0:	03095025 */ 	or	$t2,$t8,$t1
/*  f0f16f4:	ac4a0008 */ 	sw	$t2,0x8($v0)
/*  f0f16f8:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f16fc:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1700:	333803ff */ 	andi	$t8,$t9,0x3ff
/*  f0f1704:	00184880 */ 	sll	$t1,$t8,0x2
/*  f0f1708:	01ac0019 */ 	multu	$t5,$t4
/*  f0f170c:	00007812 */ 	mflo	$t7
/*  f0f1710:	31ee03ff */ 	andi	$t6,$t7,0x3ff
/*  f0f1714:	000e5b80 */ 	sll	$t3,$t6,0xe
/*  f0f1718:	01695025 */ 	or	$t2,$t3,$t1
/*  f0f171c:	0fc55153 */ 	jal	text0f153838
/*  f0f1720:	ac4a000c */ 	sw	$t2,0xc($v0)
/*  f0f1724:	0fc550cf */ 	jal	text0f153628
/*  f0f1728:	00402025 */ 	move	$a0,$v0
/*  f0f172c:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1730:	8fac00ac */ 	lw	$t4,0xac($sp)
/*  f0f1734:	8fae009c */ 	lw	$t6,0x9c($sp)
/*  f0f1738:	8fb800b0 */ 	lw	$t8,0xb0($sp)
/*  f0f173c:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f1740:	01eec821 */ 	addu	$t9,$t7,$t6
/*  f0f1744:	03381823 */ 	subu	$v1,$t9,$t8
/*  f0f1748:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f174c:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1750:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f1754:	00408825 */ 	move	$s1,$v0
/*  f0f1758:	04610003 */ 	bgez	$v1,.PF0f0f1768
/*  f0f175c:	00035843 */ 	sra	$t3,$v1,0x1
/*  f0f1760:	24610001 */ 	addiu	$at,$v1,0x1
/*  f0f1764:	00015843 */ 	sra	$t3,$at,0x1
.PF0f0f1768:
/*  f0f1768:	01601825 */ 	move	$v1,$t3
/*  f0f176c:	05210003 */ 	bgez	$t1,.PF0f0f177c
/*  f0f1770:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f1774:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f1778:	00015043 */ 	sra	$t2,$at,0x1
.PF0f0f177c:
/*  f0f177c:	006a6823 */ 	subu	$t5,$v1,$t2
/*  f0f1780:	25ac0002 */ 	addiu	$t4,$t5,0x2
/*  f0f1784:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f1788:	24c60002 */ 	addiu	$a2,$a2,0x2
/*  f0f178c:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f1790:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1794:	0fc5bdaa */ 	jal	langGet
/*  f0f1798:	afa30044 */ 	sw	$v1,0x44($sp)
/*  f0f179c:	0c002e53 */ 	jal	viGetWidth
/*  f0f17a0:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f17a4:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f17a8:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f17ac:	0c002e53 */ 	jal	viGetWidth
/*  f0f17b0:	01c08025 */ 	move	$s0,$t6
/*  f0f17b4:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f17b8:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f17bc:	240b00ff */ 	li	$t3,0xff
/*  f0f17c0:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f17c4:	02202025 */ 	move	$a0,$s1
/*  f0f17c8:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f17cc:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f17d0:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f17d4:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f17d8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f17dc:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f17e0:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f17e4:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f17e8:	0fc55c50 */ 	jal	textRenderProjected
/*  f0f17ec:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f17f0:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f17f4:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f17f8:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f17fc:	00408825 */ 	move	$s1,$v0
/*  f0f1800:	05410003 */ 	bgez	$t2,.PF0f0f1810
/*  f0f1804:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f1808:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f180c:	00016843 */ 	sra	$t5,$at,0x1
.PF0f0f1810:
/*  f0f1810:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f1814:	258f0002 */ 	addiu	$t7,$t4,0x2
/*  f0f1818:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f181c:	afaf0088 */ 	sw	$t7,0x88($sp)
/*  f0f1820:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1824:	0fc5bdaa */ 	jal	langGet
/*  f0f1828:	240451ef */ 	li	$a0,0x51ef
/*  f0f182c:	0c002e53 */ 	jal	viGetWidth
/*  f0f1830:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1834:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1838:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f183c:	0c002e53 */ 	jal	viGetWidth
/*  f0f1840:	01c08025 */ 	move	$s0,$t6
/*  f0f1844:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f1848:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f184c:	240b00ff */ 	li	$t3,0xff
/*  f0f1850:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1854:	02202025 */ 	move	$a0,$s1
/*  f0f1858:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f185c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1860:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1864:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1868:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f186c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1870:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1874:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f1878:	0fc55c50 */ 	jal	textRenderProjected
/*  f0f187c:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1880:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f1884:	8faa0044 */ 	lw	$t2,0x44($sp)
/*  f0f1888:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f188c:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f1890:	00408825 */ 	move	$s1,$v0
/*  f0f1894:	05210003 */ 	bgez	$t1,.PF0f0f18a4
/*  f0f1898:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f189c:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f18a0:	00016843 */ 	sra	$t5,$at,0x1
.PF0f0f18a4:
/*  f0f18a4:	014d6023 */ 	subu	$t4,$t2,$t5
/*  f0f18a8:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f18ac:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f18b0:	0fc5bdaa */ 	jal	langGet
/*  f0f18b4:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f18b8:	0c002e53 */ 	jal	viGetWidth
/*  f0f18bc:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f18c0:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f18c4:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f18c8:	0c002e53 */ 	jal	viGetWidth
/*  f0f18cc:	01c08025 */ 	move	$s0,$t6
/*  f0f18d0:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f18d4:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f18d8:	3c0bbfbf */ 	lui	$t3,0xbfbf
/*  f0f18dc:	356bffff */ 	ori	$t3,$t3,0xffff
/*  f0f18e0:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f18e4:	02202025 */ 	move	$a0,$s1
/*  f0f18e8:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f18ec:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f18f0:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f18f4:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f18f8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f18fc:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1900:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1904:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f1908:	0fc55c50 */ 	jal	textRenderProjected
/*  f0f190c:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1910:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f1914:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1918:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f191c:	00408825 */ 	move	$s1,$v0
/*  f0f1920:	05410003 */ 	bgez	$t2,.PF0f0f1930
/*  f0f1924:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f1928:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f192c:	00016843 */ 	sra	$t5,$at,0x1
.PF0f0f1930:
/*  f0f1930:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f1934:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f1938:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f193c:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1940:	0fc5bdaa */ 	jal	langGet
/*  f0f1944:	240451ef */ 	li	$a0,0x51ef
/*  f0f1948:	0c002e53 */ 	jal	viGetWidth
/*  f0f194c:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1950:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1954:	00107c03 */ 	sra	$t7,$s0,0x10
/*  f0f1958:	0c002e53 */ 	jal	viGetWidth
/*  f0f195c:	01e08025 */ 	move	$s0,$t7
/*  f0f1960:	8fae0064 */ 	lw	$t6,0x64($sp)
/*  f0f1964:	8fb90060 */ 	lw	$t9,0x60($sp)
/*  f0f1968:	3c18bfbf */ 	lui	$t8,0xbfbf
/*  f0f196c:	3718ffff */ 	ori	$t8,$t8,0xffff
/*  f0f1970:	afb80018 */ 	sw	$t8,0x18($sp)
/*  f0f1974:	02202025 */ 	move	$a0,$s1
/*  f0f1978:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f197c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1980:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1984:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1988:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f198c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1990:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1994:	afae0010 */ 	sw	$t6,0x10($sp)
/*  f0f1998:	0fc55c50 */ 	jal	textRenderProjected
/*  f0f199c:	afb90014 */ 	sw	$t9,0x14($sp)
/*  f0f19a0:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f19a4:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f19a8:	240151eb */ 	li	$at,0x51eb
/*  f0f19ac:	956a0000 */ 	lhu	$t2,0x0($t3)
/*  f0f19b0:	00408825 */ 	move	$s1,$v0
/*  f0f19b4:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f19b8:	15410023 */ 	bne	$t2,$at,.PF0f0f1a48
/*  f0f19bc:	24c6ffff */ 	addiu	$a2,$a2,-1
/*  f0f19c0:	8fac0044 */ 	lw	$t4,0x44($sp)
/*  f0f19c4:	05210003 */ 	bgez	$t1,.PF0f0f19d4
/*  f0f19c8:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f19cc:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f19d0:	00016843 */ 	sra	$t5,$at,0x1
.PF0f0f19d4:
/*  f0f19d4:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f19d8:	25eefff9 */ 	addiu	$t6,$t7,-7
/*  f0f19dc:	afae0088 */ 	sw	$t6,0x88($sp)
/*  f0f19e0:	0c002e53 */ 	jal	viGetWidth
/*  f0f19e4:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f19e8:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f19ec:	0010cc03 */ 	sra	$t9,$s0,0x10
/*  f0f19f0:	0c002e53 */ 	jal	viGetWidth
/*  f0f19f4:	03208025 */ 	move	$s0,$t9
/*  f0f19f8:	3c188008 */ 	lui	$t8,0x8008
/*  f0f19fc:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0f1a00:	8d6b0064 */ 	lw	$t3,0x64($t3)
/*  f0f1a04:	8f180068 */ 	lw	$t8,0x68($t8)
/*  f0f1a08:	3c0abfbf */ 	lui	$t2,0xbfbf
/*  f0f1a0c:	354affff */ 	ori	$t2,$t2,0xffff
/*  f0f1a10:	3c077f1b */ 	lui	$a3,0x7f1b
/*  f0f1a14:	24e736d8 */ 	addiu	$a3,$a3,0x36d8
/*  f0f1a18:	afaa0018 */ 	sw	$t2,0x18($sp)
/*  f0f1a1c:	02202025 */ 	move	$a0,$s1
/*  f0f1a20:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1a24:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1a28:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1a2c:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1a30:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1a34:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1a38:	afab0014 */ 	sw	$t3,0x14($sp)
/*  f0f1a3c:	0fc55c50 */ 	jal	textRenderProjected
/*  f0f1a40:	afb80010 */ 	sw	$t8,0x10($sp)
/*  f0f1a44:	00408825 */ 	move	$s1,$v0
.PF0f0f1a48:
/*  f0f1a48:	0fc55125 */ 	jal	text0f153780
/*  f0f1a4c:	02202025 */ 	move	$a0,$s1
/*  f0f1a50:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f1a54:	8fb00034 */ 	lw	$s0,0x34($sp)
/*  f0f1a58:	8fb10038 */ 	lw	$s1,0x38($sp)
/*  f0f1a5c:	03e00008 */ 	jr	$ra
/*  f0f1a60:	27bd0090 */ 	addiu	$sp,$sp,0x90
);
#elif VERSION >= VERSION_PAL_BETA
GLOBAL_ASM(
glabel menuRenderBanner
/*  f0f1248:	27bdff70 */ 	addiu	$sp,$sp,-144
/*  f0f124c:	8fae00a4 */ 	lw	$t6,0xa4($sp)
/*  f0f1250:	afb10038 */ 	sw	$s1,0x38($sp)
/*  f0f1254:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1258:	3c088008 */ 	lui	$t0,0x8008
/*  f0f125c:	00808825 */ 	move	$s1,$a0
/*  f0f1260:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0f1264:	afb00034 */ 	sw	$s0,0x34($sp)
/*  f0f1268:	afa50094 */ 	sw	$a1,0x94($sp)
/*  f0f126c:	afa60098 */ 	sw	$a2,0x98($sp)
/*  f0f1270:	afa7009c */ 	sw	$a3,0x9c($sp)
/*  f0f1274:	8c631fe0 */ 	lw	$v1,0x1fe0($v1)
/*  f0f1278:	11c00008 */ 	beqz	$t6,.PB0f0f129c
/*  f0f127c:	8d081fdc */ 	lw	$t0,0x1fdc($t0)
/*  f0f1280:	3c028008 */ 	lui	$v0,0x8008
/*  f0f1284:	8c421fe8 */ 	lw	$v0,0x1fe8($v0)
/*  f0f1288:	50400005 */ 	beqzl	$v0,.PB0f0f12a0
/*  f0f128c:	8faf0098 */ 	lw	$t7,0x98($sp)
/*  f0f1290:	3c088008 */ 	lui	$t0,0x8008
/*  f0f1294:	00401825 */ 	move	$v1,$v0
/*  f0f1298:	8d081fe4 */ 	lw	$t0,0x1fe4($t0)
.PB0f0f129c:
/*  f0f129c:	8faf0098 */ 	lw	$t7,0x98($sp)
.PB0f0f12a0:
/*  f0f12a0:	8fb800a0 */ 	lw	$t8,0xa0($sp)
/*  f0f12a4:	8fa900a8 */ 	lw	$t1,0xa8($sp)
/*  f0f12a8:	3c0b8007 */ 	lui	$t3,0x8007
/*  f0f12ac:	01f83021 */ 	addu	$a2,$t7,$t8
/*  f0f12b0:	256b311c */ 	addiu	$t3,$t3,0x311c
/*  f0f12b4:	00095040 */ 	sll	$t2,$t1,0x1
/*  f0f12b8:	04c10003 */ 	bgez	$a2,.PB0f0f12c8
/*  f0f12bc:	0006c843 */ 	sra	$t9,$a2,0x1
/*  f0f12c0:	24c10001 */ 	addiu	$at,$a2,0x1
/*  f0f12c4:	0001c843 */ 	sra	$t9,$at,0x1
.PB0f0f12c8:
/*  f0f12c8:	014b1021 */ 	addu	$v0,$t2,$t3
/*  f0f12cc:	94440000 */ 	lhu	$a0,0x0($v0)
/*  f0f12d0:	afa20040 */ 	sw	$v0,0x40($sp)
/*  f0f12d4:	afb90084 */ 	sw	$t9,0x84($sp)
/*  f0f12d8:	afa30064 */ 	sw	$v1,0x64($sp)
/*  f0f12dc:	0fc5c06a */ 	jal	langGet
/*  f0f12e0:	afa80060 */ 	sw	$t0,0x60($sp)
/*  f0f12e4:	8fac0060 */ 	lw	$t4,0x60($sp)
/*  f0f12e8:	27a4007c */ 	addiu	$a0,$sp,0x7c
/*  f0f12ec:	27a50078 */ 	addiu	$a1,$sp,0x78
/*  f0f12f0:	00403025 */ 	move	$a2,$v0
/*  f0f12f4:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f12f8:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f12fc:	0fc5625a */ 	jal	textMeasure
/*  f0f1300:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0f1304:	0fc5c06a */ 	jal	langGet
/*  f0f1308:	240451ef */ 	li	$a0,0x51ef
/*  f0f130c:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f1310:	27a40074 */ 	addiu	$a0,$sp,0x74
/*  f0f1314:	27a50070 */ 	addiu	$a1,$sp,0x70
/*  f0f1318:	00403025 */ 	move	$a2,$v0
/*  f0f131c:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f1320:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f1324:	0fc5625a */ 	jal	textMeasure
/*  f0f1328:	afad0010 */ 	sw	$t5,0x10($sp)
/*  f0f132c:	8fae0040 */ 	lw	$t6,0x40($sp)
/*  f0f1330:	240151eb */ 	li	$at,0x51eb
/*  f0f1334:	02202025 */ 	move	$a0,$s1
/*  f0f1338:	95cf0000 */ 	lhu	$t7,0x0($t6)
/*  f0f133c:	55e10005 */ 	bnel	$t7,$at,.PB0f0f1354
/*  f0f1340:	8fa2007c */ 	lw	$v0,0x7c($sp)
/*  f0f1344:	8fb80078 */ 	lw	$t8,0x78($sp)
/*  f0f1348:	27190007 */ 	addiu	$t9,$t8,0x7
/*  f0f134c:	afb90078 */ 	sw	$t9,0x78($sp)
/*  f0f1350:	8fa2007c */ 	lw	$v0,0x7c($sp)
.PB0f0f1354:
/*  f0f1354:	8fa30074 */ 	lw	$v1,0x74($sp)
/*  f0f1358:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f135c:	2405007f */ 	li	$a1,0x7f
/*  f0f1360:	00434821 */ 	addu	$t1,$v0,$v1
/*  f0f1364:	05210003 */ 	bgez	$t1,.PB0f0f1374
/*  f0f1368:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f136c:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f1370:	00015043 */ 	sra	$t2,$at,0x1
.PB0f0f1374:
/*  f0f1374:	00ca3023 */ 	subu	$a2,$a2,$t2
/*  f0f1378:	00c26021 */ 	addu	$t4,$a2,$v0
/*  f0f137c:	01836821 */ 	addu	$t5,$t4,$v1
/*  f0f1380:	24cbfffc */ 	addiu	$t3,$a2,-4
/*  f0f1384:	25ae0007 */ 	addiu	$t6,$t5,0x7
/*  f0f1388:	afa60080 */ 	sw	$a2,0x80($sp)
/*  f0f138c:	afab006c */ 	sw	$t3,0x6c($sp)
/*  f0f1390:	afae0068 */ 	sw	$t6,0x68($sp)
/*  f0f1394:	0fc552fc */ 	jal	textSetPrimColour
/*  f0f1398:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f139c:	3c038008 */ 	lui	$v1,0x8008
/*  f0f13a0:	24631f90 */ 	addiu	$v1,$v1,0x1f90
/*  f0f13a4:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0f13a8:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0f13ac:	8fac00a0 */ 	lw	$t4,0xa0($sp)
/*  f0f13b0:	3c10f600 */ 	lui	$s0,0xf600
/*  f0f13b4:	01f80019 */ 	multu	$t7,$t8
/*  f0f13b8:	318d03ff */ 	andi	$t5,$t4,0x3ff
/*  f0f13bc:	000d7080 */ 	sll	$t6,$t5,0x2
/*  f0f13c0:	8fb1006c */ 	lw	$s1,0x6c($sp)
/*  f0f13c4:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f13c8:	0000c812 */ 	mflo	$t9
/*  f0f13cc:	332903ff */ 	andi	$t1,$t9,0x3ff
/*  f0f13d0:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0f13d4:	01505825 */ 	or	$t3,$t2,$s0
/*  f0f13d8:	016e7825 */ 	or	$t7,$t3,$t6
/*  f0f13dc:	ac4f0000 */ 	sw	$t7,0x0($v0)
/*  f0f13e0:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f13e4:	8fb80094 */ 	lw	$t8,0x94($sp)
/*  f0f13e8:	8fad0098 */ 	lw	$t5,0x98($sp)
/*  f0f13ec:	03190019 */ 	multu	$t8,$t9
/*  f0f13f0:	31ab03ff */ 	andi	$t3,$t5,0x3ff
/*  f0f13f4:	000b7080 */ 	sll	$t6,$t3,0x2
/*  f0f13f8:	00004812 */ 	mflo	$t1
/*  f0f13fc:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f1400:	000a6380 */ 	sll	$t4,$t2,0xe
/*  f0f1404:	018e7825 */ 	or	$t7,$t4,$t6
/*  f0f1408:	0fc55313 */ 	jal	text0f153838
/*  f0f140c:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f1410:	00402025 */ 	move	$a0,$v0
/*  f0f1414:	0fc552fc */ 	jal	textSetPrimColour
/*  f0f1418:	24057f7f */ 	li	$a1,0x7f7f
/*  f0f141c:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1420:	24631f90 */ 	addiu	$v1,$v1,0x1f90
/*  f0f1424:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f1428:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f142c:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f1430:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f1434:	03190019 */ 	multu	$t8,$t9
/*  f0f1438:	318e03ff */ 	andi	$t6,$t4,0x3ff
/*  f0f143c:	000e7880 */ 	sll	$t7,$t6,0x2
/*  f0f1440:	322e03ff */ 	andi	$t6,$s1,0x3ff
/*  f0f1444:	00004812 */ 	mflo	$t1
/*  f0f1448:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f144c:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f1450:	01b05825 */ 	or	$t3,$t5,$s0
/*  f0f1454:	016fc025 */ 	or	$t8,$t3,$t7
/*  f0f1458:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0f145c:	8c690000 */ 	lw	$t1,0x0($v1)
/*  f0f1460:	8fb90094 */ 	lw	$t9,0x94($sp)
/*  f0f1464:	000e5880 */ 	sll	$t3,$t6,0x2
/*  f0f1468:	03290019 */ 	multu	$t9,$t1
/*  f0f146c:	00005012 */ 	mflo	$t2
/*  f0f1470:	314d03ff */ 	andi	$t5,$t2,0x3ff
/*  f0f1474:	000d6380 */ 	sll	$t4,$t5,0xe
/*  f0f1478:	018b7825 */ 	or	$t7,$t4,$t3
/*  f0f147c:	0fc55313 */ 	jal	text0f153838
/*  f0f1480:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f1484:	3c057f7f */ 	lui	$a1,0x7f7f
/*  f0f1488:	34a5ff7f */ 	ori	$a1,$a1,0xff7f
/*  f0f148c:	0fc552fc */ 	jal	textSetPrimColour
/*  f0f1490:	00402025 */ 	move	$a0,$v0
/*  f0f1494:	3c038008 */ 	lui	$v1,0x8008
/*  f0f1498:	24631f90 */ 	addiu	$v1,$v1,0x1f90
/*  f0f149c:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f14a0:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f14a4:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f14a8:	24440010 */ 	addiu	$a0,$v0,0x10
/*  f0f14ac:	03190019 */ 	multu	$t8,$t9
/*  f0f14b0:	258b0004 */ 	addiu	$t3,$t4,0x4
/*  f0f14b4:	316f03ff */ 	andi	$t7,$t3,0x3ff
/*  f0f14b8:	000fc080 */ 	sll	$t8,$t7,0x2
/*  f0f14bc:	00004812 */ 	mflo	$t1
/*  f0f14c0:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f14c4:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f14c8:	01b07025 */ 	or	$t6,$t5,$s0
/*  f0f14cc:	01d8c825 */ 	or	$t9,$t6,$t8
/*  f0f14d0:	ac590000 */ 	sw	$t9,0x0($v0)
/*  f0f14d4:	8c6a0000 */ 	lw	$t2,0x0($v1)
/*  f0f14d8:	8fa90094 */ 	lw	$t1,0x94($sp)
/*  f0f14dc:	8faf0068 */ 	lw	$t7,0x68($sp)
/*  f0f14e0:	012a0019 */ 	multu	$t1,$t2
/*  f0f14e4:	25ee0002 */ 	addiu	$t6,$t7,0x2
/*  f0f14e8:	31d803ff */ 	andi	$t8,$t6,0x3ff
/*  f0f14ec:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f14f0:	00006812 */ 	mflo	$t5
/*  f0f14f4:	31ac03ff */ 	andi	$t4,$t5,0x3ff
/*  f0f14f8:	000c5b80 */ 	sll	$t3,$t4,0xe
/*  f0f14fc:	01794825 */ 	or	$t1,$t3,$t9
/*  f0f1500:	ac490004 */ 	sw	$t1,0x4($v0)
/*  f0f1504:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f1508:	8faa009c */ 	lw	$t2,0x9c($sp)
/*  f0f150c:	262bfffe */ 	addiu	$t3,$s1,-2
/*  f0f1510:	317903ff */ 	andi	$t9,$t3,0x3ff
/*  f0f1514:	014d0019 */ 	multu	$t2,$t5
/*  f0f1518:	00194880 */ 	sll	$t1,$t9,0x2
/*  f0f151c:	2639fffc */ 	addiu	$t9,$s1,-4
/*  f0f1520:	00006012 */ 	mflo	$t4
/*  f0f1524:	318f03ff */ 	andi	$t7,$t4,0x3ff
/*  f0f1528:	000f7380 */ 	sll	$t6,$t7,0xe
/*  f0f152c:	01d0c025 */ 	or	$t8,$t6,$s0
/*  f0f1530:	03095025 */ 	or	$t2,$t8,$t1
/*  f0f1534:	ac4a0008 */ 	sw	$t2,0x8($v0)
/*  f0f1538:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f153c:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1540:	333803ff */ 	andi	$t8,$t9,0x3ff
/*  f0f1544:	00184880 */ 	sll	$t1,$t8,0x2
/*  f0f1548:	01ac0019 */ 	multu	$t5,$t4
/*  f0f154c:	00007812 */ 	mflo	$t7
/*  f0f1550:	31ee03ff */ 	andi	$t6,$t7,0x3ff
/*  f0f1554:	000e5b80 */ 	sll	$t3,$t6,0xe
/*  f0f1558:	01695025 */ 	or	$t2,$t3,$t1
/*  f0f155c:	0fc55313 */ 	jal	text0f153838
/*  f0f1560:	ac4a000c */ 	sw	$t2,0xc($v0)
/*  f0f1564:	0fc5528f */ 	jal	text0f153628
/*  f0f1568:	00402025 */ 	move	$a0,$v0
/*  f0f156c:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1570:	8fac00ac */ 	lw	$t4,0xac($sp)
/*  f0f1574:	8fae009c */ 	lw	$t6,0x9c($sp)
/*  f0f1578:	8fb800b0 */ 	lw	$t8,0xb0($sp)
/*  f0f157c:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f1580:	01eec821 */ 	addu	$t9,$t7,$t6
/*  f0f1584:	03381823 */ 	subu	$v1,$t9,$t8
/*  f0f1588:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f158c:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1590:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f1594:	00408825 */ 	move	$s1,$v0
/*  f0f1598:	04610003 */ 	bgez	$v1,.PB0f0f15a8
/*  f0f159c:	00035843 */ 	sra	$t3,$v1,0x1
/*  f0f15a0:	24610001 */ 	addiu	$at,$v1,0x1
/*  f0f15a4:	00015843 */ 	sra	$t3,$at,0x1
.PB0f0f15a8:
/*  f0f15a8:	01601825 */ 	move	$v1,$t3
/*  f0f15ac:	05210003 */ 	bgez	$t1,.PB0f0f15bc
/*  f0f15b0:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f15b4:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f15b8:	00015043 */ 	sra	$t2,$at,0x1
.PB0f0f15bc:
/*  f0f15bc:	006a6823 */ 	subu	$t5,$v1,$t2
/*  f0f15c0:	25ac0002 */ 	addiu	$t4,$t5,0x2
/*  f0f15c4:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f15c8:	24c60002 */ 	addiu	$a2,$a2,0x2
/*  f0f15cc:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f15d0:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f15d4:	0fc5c06a */ 	jal	langGet
/*  f0f15d8:	afa30044 */ 	sw	$v1,0x44($sp)
/*  f0f15dc:	0c002e4d */ 	jal	viGetWidth
/*  f0f15e0:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f15e4:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f15e8:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f15ec:	0c002e4d */ 	jal	viGetWidth
/*  f0f15f0:	01c08025 */ 	move	$s0,$t6
/*  f0f15f4:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f15f8:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f15fc:	240b00ff */ 	li	$t3,0xff
/*  f0f1600:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1604:	02202025 */ 	move	$a0,$s1
/*  f0f1608:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f160c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1610:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1614:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1618:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f161c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1620:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1624:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f1628:	0fc55e10 */ 	jal	textRenderProjected
/*  f0f162c:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1630:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f1634:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f1638:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f163c:	00408825 */ 	move	$s1,$v0
/*  f0f1640:	05410003 */ 	bgez	$t2,.PB0f0f1650
/*  f0f1644:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f1648:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f164c:	00016843 */ 	sra	$t5,$at,0x1
.PB0f0f1650:
/*  f0f1650:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f1654:	258f0002 */ 	addiu	$t7,$t4,0x2
/*  f0f1658:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f165c:	afaf0088 */ 	sw	$t7,0x88($sp)
/*  f0f1660:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1664:	0fc5c06a */ 	jal	langGet
/*  f0f1668:	240451ef */ 	li	$a0,0x51ef
/*  f0f166c:	0c002e4d */ 	jal	viGetWidth
/*  f0f1670:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1674:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1678:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f167c:	0c002e4d */ 	jal	viGetWidth
/*  f0f1680:	01c08025 */ 	move	$s0,$t6
/*  f0f1684:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f1688:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f168c:	240b00ff */ 	li	$t3,0xff
/*  f0f1690:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1694:	02202025 */ 	move	$a0,$s1
/*  f0f1698:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f169c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f16a0:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f16a4:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f16a8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f16ac:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f16b0:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f16b4:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f16b8:	0fc55e10 */ 	jal	textRenderProjected
/*  f0f16bc:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f16c0:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f16c4:	8faa0044 */ 	lw	$t2,0x44($sp)
/*  f0f16c8:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f16cc:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f16d0:	00408825 */ 	move	$s1,$v0
/*  f0f16d4:	05210003 */ 	bgez	$t1,.PB0f0f16e4
/*  f0f16d8:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f16dc:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f16e0:	00016843 */ 	sra	$t5,$at,0x1
.PB0f0f16e4:
/*  f0f16e4:	014d6023 */ 	subu	$t4,$t2,$t5
/*  f0f16e8:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f16ec:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f16f0:	0fc5c06a */ 	jal	langGet
/*  f0f16f4:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f16f8:	0c002e4d */ 	jal	viGetWidth
/*  f0f16fc:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1700:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1704:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f1708:	0c002e4d */ 	jal	viGetWidth
/*  f0f170c:	01c08025 */ 	move	$s0,$t6
/*  f0f1710:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f1714:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f1718:	3c0bbfbf */ 	lui	$t3,0xbfbf
/*  f0f171c:	356bffff */ 	ori	$t3,$t3,0xffff
/*  f0f1720:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1724:	02202025 */ 	move	$a0,$s1
/*  f0f1728:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f172c:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1730:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1734:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1738:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f173c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1740:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1744:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f1748:	0fc55e10 */ 	jal	textRenderProjected
/*  f0f174c:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1750:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f1754:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1758:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f175c:	00408825 */ 	move	$s1,$v0
/*  f0f1760:	05410003 */ 	bgez	$t2,.PB0f0f1770
/*  f0f1764:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f1768:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f176c:	00016843 */ 	sra	$t5,$at,0x1
.PB0f0f1770:
/*  f0f1770:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f1774:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f1778:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f177c:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1780:	0fc5c06a */ 	jal	langGet
/*  f0f1784:	240451ef */ 	li	$a0,0x51ef
/*  f0f1788:	0c002e4d */ 	jal	viGetWidth
/*  f0f178c:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1790:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1794:	00107c03 */ 	sra	$t7,$s0,0x10
/*  f0f1798:	0c002e4d */ 	jal	viGetWidth
/*  f0f179c:	01e08025 */ 	move	$s0,$t7
/*  f0f17a0:	8fae0064 */ 	lw	$t6,0x64($sp)
/*  f0f17a4:	8fb90060 */ 	lw	$t9,0x60($sp)
/*  f0f17a8:	3c18bfbf */ 	lui	$t8,0xbfbf
/*  f0f17ac:	3718ffff */ 	ori	$t8,$t8,0xffff
/*  f0f17b0:	afb80018 */ 	sw	$t8,0x18($sp)
/*  f0f17b4:	02202025 */ 	move	$a0,$s1
/*  f0f17b8:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f17bc:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f17c0:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f17c4:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f17c8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f17cc:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f17d0:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f17d4:	afae0010 */ 	sw	$t6,0x10($sp)
/*  f0f17d8:	0fc55e10 */ 	jal	textRenderProjected
/*  f0f17dc:	afb90014 */ 	sw	$t9,0x14($sp)
/*  f0f17e0:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f17e4:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f17e8:	240151eb */ 	li	$at,0x51eb
/*  f0f17ec:	956a0000 */ 	lhu	$t2,0x0($t3)
/*  f0f17f0:	00408825 */ 	move	$s1,$v0
/*  f0f17f4:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f17f8:	15410023 */ 	bne	$t2,$at,.PB0f0f1888
/*  f0f17fc:	24c6ffff */ 	addiu	$a2,$a2,-1
/*  f0f1800:	8fac0044 */ 	lw	$t4,0x44($sp)
/*  f0f1804:	05210003 */ 	bgez	$t1,.PB0f0f1814
/*  f0f1808:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f180c:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f1810:	00016843 */ 	sra	$t5,$at,0x1
.PB0f0f1814:
/*  f0f1814:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f1818:	25eefff9 */ 	addiu	$t6,$t7,-7
/*  f0f181c:	afae0088 */ 	sw	$t6,0x88($sp)
/*  f0f1820:	0c002e4d */ 	jal	viGetWidth
/*  f0f1824:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1828:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f182c:	0010cc03 */ 	sra	$t9,$s0,0x10
/*  f0f1830:	0c002e4d */ 	jal	viGetWidth
/*  f0f1834:	03208025 */ 	move	$s0,$t9
/*  f0f1838:	3c188008 */ 	lui	$t8,0x8008
/*  f0f183c:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0f1840:	8d6b1fd4 */ 	lw	$t3,0x1fd4($t3)
/*  f0f1844:	8f181fd8 */ 	lw	$t8,0x1fd8($t8)
/*  f0f1848:	3c0abfbf */ 	lui	$t2,0xbfbf
/*  f0f184c:	354affff */ 	ori	$t2,$t2,0xffff
/*  f0f1850:	3c077f1b */ 	lui	$a3,0x7f1b
/*  f0f1854:	24e74128 */ 	addiu	$a3,$a3,0x4128
/*  f0f1858:	afaa0018 */ 	sw	$t2,0x18($sp)
/*  f0f185c:	02202025 */ 	move	$a0,$s1
/*  f0f1860:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1864:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1868:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f186c:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1870:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1874:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1878:	afab0014 */ 	sw	$t3,0x14($sp)
/*  f0f187c:	0fc55e10 */ 	jal	textRenderProjected
/*  f0f1880:	afb80010 */ 	sw	$t8,0x10($sp)
/*  f0f1884:	00408825 */ 	move	$s1,$v0
.PB0f0f1888:
/*  f0f1888:	0fc552e5 */ 	jal	text0f153780
/*  f0f188c:	02202025 */ 	move	$a0,$s1
/*  f0f1890:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f1894:	8fb00034 */ 	lw	$s0,0x34($sp)
/*  f0f1898:	8fb10038 */ 	lw	$s1,0x38($sp)
/*  f0f189c:	03e00008 */ 	jr	$ra
/*  f0f18a0:	27bd0090 */ 	addiu	$sp,$sp,0x90
);
#elif VERSION >= VERSION_NTSC_1_0
GLOBAL_ASM(
glabel menuRenderBanner
/*  f0f0ce8:	27bdff70 */ 	addiu	$sp,$sp,-144
/*  f0f0cec:	8fae00a4 */ 	lw	$t6,0xa4($sp)
/*  f0f0cf0:	afb10038 */ 	sw	$s1,0x38($sp)
/*  f0f0cf4:	3c038008 */ 	lui	$v1,%hi(g_CharsHandelGothicSm)
/*  f0f0cf8:	3c088008 */ 	lui	$t0,%hi(g_FontHandelGothicSm)
/*  f0f0cfc:	00808825 */ 	or	$s1,$a0,$zero
/*  f0f0d00:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0f0d04:	afb00034 */ 	sw	$s0,0x34($sp)
/*  f0f0d08:	afa50094 */ 	sw	$a1,0x94($sp)
/*  f0f0d0c:	afa60098 */ 	sw	$a2,0x98($sp)
/*  f0f0d10:	afa7009c */ 	sw	$a3,0x9c($sp)
/*  f0f0d14:	8c63fb10 */ 	lw	$v1,%lo(g_CharsHandelGothicSm)($v1)
/*  f0f0d18:	11c00005 */ 	beqz	$t6,.L0f0f0d30
/*  f0f0d1c:	8d08fb0c */ 	lw	$t0,%lo(g_FontHandelGothicSm)($t0)
/*  f0f0d20:	3c038008 */ 	lui	$v1,%hi(g_CharsHandelGothicMd)
/*  f0f0d24:	3c088008 */ 	lui	$t0,%hi(g_FontHandelGothicMd)
/*  f0f0d28:	8c63fb18 */ 	lw	$v1,%lo(g_CharsHandelGothicMd)($v1)
/*  f0f0d2c:	8d08fb14 */ 	lw	$t0,%lo(g_FontHandelGothicMd)($t0)
.L0f0f0d30:
/*  f0f0d30:	8faf0098 */ 	lw	$t7,0x98($sp)
/*  f0f0d34:	8fb800a0 */ 	lw	$t8,0xa0($sp)
/*  f0f0d38:	8fa900a8 */ 	lw	$t1,0xa8($sp)
/*  f0f0d3c:	3c0b8007 */ 	lui	$t3,%hi(var8007144c)
/*  f0f0d40:	01f83021 */ 	addu	$a2,$t7,$t8
/*  f0f0d44:	256b144c */ 	addiu	$t3,$t3,%lo(var8007144c)
/*  f0f0d48:	00095040 */ 	sll	$t2,$t1,0x1
/*  f0f0d4c:	04c10003 */ 	bgez	$a2,.L0f0f0d5c
/*  f0f0d50:	0006c843 */ 	sra	$t9,$a2,0x1
/*  f0f0d54:	24c10001 */ 	addiu	$at,$a2,0x1
/*  f0f0d58:	0001c843 */ 	sra	$t9,$at,0x1
.L0f0f0d5c:
/*  f0f0d5c:	014b1021 */ 	addu	$v0,$t2,$t3
/*  f0f0d60:	94440000 */ 	lhu	$a0,0x0($v0)
/*  f0f0d64:	afa20040 */ 	sw	$v0,0x40($sp)
/*  f0f0d68:	afb90084 */ 	sw	$t9,0x84($sp)
/*  f0f0d6c:	afa30064 */ 	sw	$v1,0x64($sp)
/*  f0f0d70:	0fc5b9f1 */ 	jal	langGet
/*  f0f0d74:	afa80060 */ 	sw	$t0,0x60($sp)
/*  f0f0d78:	8fac0060 */ 	lw	$t4,0x60($sp)
/*  f0f0d7c:	27a4007c */ 	addiu	$a0,$sp,0x7c
/*  f0f0d80:	27a50078 */ 	addiu	$a1,$sp,0x78
/*  f0f0d84:	00403025 */ 	or	$a2,$v0,$zero
/*  f0f0d88:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f0d8c:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f0d90:	0fc55cbe */ 	jal	textMeasure
/*  f0f0d94:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0f0d98:	0fc5b9f1 */ 	jal	langGet
/*  f0f0d9c:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0f0da0:	8fad0060 */ 	lw	$t5,0x60($sp)
/*  f0f0da4:	27a40074 */ 	addiu	$a0,$sp,0x74
/*  f0f0da8:	27a50070 */ 	addiu	$a1,$sp,0x70
/*  f0f0dac:	00403025 */ 	or	$a2,$v0,$zero
/*  f0f0db0:	8fa70064 */ 	lw	$a3,0x64($sp)
/*  f0f0db4:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0f0db8:	0fc55cbe */ 	jal	textMeasure
/*  f0f0dbc:	afad0010 */ 	sw	$t5,0x10($sp)
/*  f0f0dc0:	8fae0040 */ 	lw	$t6,0x40($sp)
/*  f0f0dc4:	240151eb */ 	addiu	$at,$zero,0x51eb
/*  f0f0dc8:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f0dcc:	95cf0000 */ 	lhu	$t7,0x0($t6)
/*  f0f0dd0:	55e10005 */ 	bnel	$t7,$at,.L0f0f0de8
/*  f0f0dd4:	8fa2007c */ 	lw	$v0,0x7c($sp)
/*  f0f0dd8:	8fb80078 */ 	lw	$t8,0x78($sp)
/*  f0f0ddc:	27190007 */ 	addiu	$t9,$t8,0x7
/*  f0f0de0:	afb90078 */ 	sw	$t9,0x78($sp)
/*  f0f0de4:	8fa2007c */ 	lw	$v0,0x7c($sp)
.L0f0f0de8:
/*  f0f0de8:	8fa30074 */ 	lw	$v1,0x74($sp)
/*  f0f0dec:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f0df0:	2405007f */ 	addiu	$a1,$zero,0x7f
/*  f0f0df4:	00434821 */ 	addu	$t1,$v0,$v1
/*  f0f0df8:	05210003 */ 	bgez	$t1,.L0f0f0e08
/*  f0f0dfc:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f0e00:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f0e04:	00015043 */ 	sra	$t2,$at,0x1
.L0f0f0e08:
/*  f0f0e08:	00ca3023 */ 	subu	$a2,$a2,$t2
/*  f0f0e0c:	00c26021 */ 	addu	$t4,$a2,$v0
/*  f0f0e10:	01836821 */ 	addu	$t5,$t4,$v1
/*  f0f0e14:	24cbfffc */ 	addiu	$t3,$a2,-4
/*  f0f0e18:	25ae0007 */ 	addiu	$t6,$t5,0x7
/*  f0f0e1c:	afa60080 */ 	sw	$a2,0x80($sp)
/*  f0f0e20:	afab006c */ 	sw	$t3,0x6c($sp)
/*  f0f0e24:	afae0068 */ 	sw	$t6,0x68($sp)
/*  f0f0e28:	0fc54df7 */ 	jal	textSetPrimColour
/*  f0f0e2c:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f0e30:	3c038008 */ 	lui	$v1,%hi(g_ScaleX)
/*  f0f0e34:	2463fac0 */ 	addiu	$v1,$v1,%lo(g_ScaleX)
/*  f0f0e38:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0f0e3c:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0f0e40:	8fac00a0 */ 	lw	$t4,0xa0($sp)
/*  f0f0e44:	3c10f600 */ 	lui	$s0,0xf600
/*  f0f0e48:	01f80019 */ 	multu	$t7,$t8
/*  f0f0e4c:	318d03ff */ 	andi	$t5,$t4,0x3ff
/*  f0f0e50:	000d7080 */ 	sll	$t6,$t5,0x2
/*  f0f0e54:	8fb1006c */ 	lw	$s1,0x6c($sp)
/*  f0f0e58:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f0e5c:	0000c812 */ 	mflo	$t9
/*  f0f0e60:	332903ff */ 	andi	$t1,$t9,0x3ff
/*  f0f0e64:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0f0e68:	01505825 */ 	or	$t3,$t2,$s0
/*  f0f0e6c:	016e7825 */ 	or	$t7,$t3,$t6
/*  f0f0e70:	ac4f0000 */ 	sw	$t7,0x0($v0)
/*  f0f0e74:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f0e78:	8fb80094 */ 	lw	$t8,0x94($sp)
/*  f0f0e7c:	8fad0098 */ 	lw	$t5,0x98($sp)
/*  f0f0e80:	03190019 */ 	multu	$t8,$t9
/*  f0f0e84:	31ab03ff */ 	andi	$t3,$t5,0x3ff
/*  f0f0e88:	000b7080 */ 	sll	$t6,$t3,0x2
/*  f0f0e8c:	00004812 */ 	mflo	$t1
/*  f0f0e90:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f0e94:	000a6380 */ 	sll	$t4,$t2,0xe
/*  f0f0e98:	018e7825 */ 	or	$t7,$t4,$t6
/*  f0f0e9c:	0fc54e0e */ 	jal	text0f153838
/*  f0f0ea0:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f0ea4:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f0ea8:	0fc54df7 */ 	jal	textSetPrimColour
/*  f0f0eac:	24057f7f */ 	addiu	$a1,$zero,0x7f7f
/*  f0f0eb0:	3c038008 */ 	lui	$v1,%hi(g_ScaleX)
/*  f0f0eb4:	2463fac0 */ 	addiu	$v1,$v1,%lo(g_ScaleX)
/*  f0f0eb8:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f0ebc:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f0ec0:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f0ec4:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0f0ec8:	03190019 */ 	multu	$t8,$t9
/*  f0f0ecc:	318e03ff */ 	andi	$t6,$t4,0x3ff
/*  f0f0ed0:	000e7880 */ 	sll	$t7,$t6,0x2
/*  f0f0ed4:	322e03ff */ 	andi	$t6,$s1,0x3ff
/*  f0f0ed8:	00004812 */ 	mflo	$t1
/*  f0f0edc:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f0ee0:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f0ee4:	01b05825 */ 	or	$t3,$t5,$s0
/*  f0f0ee8:	016fc025 */ 	or	$t8,$t3,$t7
/*  f0f0eec:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0f0ef0:	8c690000 */ 	lw	$t1,0x0($v1)
/*  f0f0ef4:	8fb90094 */ 	lw	$t9,0x94($sp)
/*  f0f0ef8:	000e5880 */ 	sll	$t3,$t6,0x2
/*  f0f0efc:	03290019 */ 	multu	$t9,$t1
/*  f0f0f00:	00005012 */ 	mflo	$t2
/*  f0f0f04:	314d03ff */ 	andi	$t5,$t2,0x3ff
/*  f0f0f08:	000d6380 */ 	sll	$t4,$t5,0xe
/*  f0f0f0c:	018b7825 */ 	or	$t7,$t4,$t3
/*  f0f0f10:	0fc54e0e */ 	jal	text0f153838
/*  f0f0f14:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0f0f18:	3c057f7f */ 	lui	$a1,0x7f7f
/*  f0f0f1c:	34a5ff7f */ 	ori	$a1,$a1,0xff7f
/*  f0f0f20:	0fc54df7 */ 	jal	textSetPrimColour
/*  f0f0f24:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f0f28:	3c038008 */ 	lui	$v1,%hi(g_ScaleX)
/*  f0f0f2c:	2463fac0 */ 	addiu	$v1,$v1,%lo(g_ScaleX)
/*  f0f0f30:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f0f34:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0f0f38:	8fac0068 */ 	lw	$t4,0x68($sp)
/*  f0f0f3c:	24440010 */ 	addiu	$a0,$v0,0x10
/*  f0f0f40:	03190019 */ 	multu	$t8,$t9
/*  f0f0f44:	258b0004 */ 	addiu	$t3,$t4,0x4
/*  f0f0f48:	316f03ff */ 	andi	$t7,$t3,0x3ff
/*  f0f0f4c:	000fc080 */ 	sll	$t8,$t7,0x2
/*  f0f0f50:	00004812 */ 	mflo	$t1
/*  f0f0f54:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0f0f58:	000a6b80 */ 	sll	$t5,$t2,0xe
/*  f0f0f5c:	01b07025 */ 	or	$t6,$t5,$s0
/*  f0f0f60:	01d8c825 */ 	or	$t9,$t6,$t8
/*  f0f0f64:	ac590000 */ 	sw	$t9,0x0($v0)
/*  f0f0f68:	8c6a0000 */ 	lw	$t2,0x0($v1)
/*  f0f0f6c:	8fa90094 */ 	lw	$t1,0x94($sp)
/*  f0f0f70:	8faf0068 */ 	lw	$t7,0x68($sp)
/*  f0f0f74:	012a0019 */ 	multu	$t1,$t2
/*  f0f0f78:	25ee0002 */ 	addiu	$t6,$t7,0x2
/*  f0f0f7c:	31d803ff */ 	andi	$t8,$t6,0x3ff
/*  f0f0f80:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f0f84:	00006812 */ 	mflo	$t5
/*  f0f0f88:	31ac03ff */ 	andi	$t4,$t5,0x3ff
/*  f0f0f8c:	000c5b80 */ 	sll	$t3,$t4,0xe
/*  f0f0f90:	01794825 */ 	or	$t1,$t3,$t9
/*  f0f0f94:	ac490004 */ 	sw	$t1,0x4($v0)
/*  f0f0f98:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f0f9c:	8faa009c */ 	lw	$t2,0x9c($sp)
/*  f0f0fa0:	262bfffe */ 	addiu	$t3,$s1,-2
/*  f0f0fa4:	317903ff */ 	andi	$t9,$t3,0x3ff
/*  f0f0fa8:	014d0019 */ 	multu	$t2,$t5
/*  f0f0fac:	00194880 */ 	sll	$t1,$t9,0x2
/*  f0f0fb0:	2639fffc */ 	addiu	$t9,$s1,-4
/*  f0f0fb4:	00006012 */ 	mflo	$t4
/*  f0f0fb8:	318f03ff */ 	andi	$t7,$t4,0x3ff
/*  f0f0fbc:	000f7380 */ 	sll	$t6,$t7,0xe
/*  f0f0fc0:	01d0c025 */ 	or	$t8,$t6,$s0
/*  f0f0fc4:	03095025 */ 	or	$t2,$t8,$t1
/*  f0f0fc8:	ac4a0008 */ 	sw	$t2,0x8($v0)
/*  f0f0fcc:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f0fd0:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f0fd4:	333803ff */ 	andi	$t8,$t9,0x3ff
/*  f0f0fd8:	00184880 */ 	sll	$t1,$t8,0x2
/*  f0f0fdc:	01ac0019 */ 	multu	$t5,$t4
/*  f0f0fe0:	00007812 */ 	mflo	$t7
/*  f0f0fe4:	31ee03ff */ 	andi	$t6,$t7,0x3ff
/*  f0f0fe8:	000e5b80 */ 	sll	$t3,$t6,0xe
/*  f0f0fec:	01695025 */ 	or	$t2,$t3,$t1
/*  f0f0ff0:	0fc54e0e */ 	jal	text0f153838
/*  f0f0ff4:	ac4a000c */ 	sw	$t2,0xc($v0)
/*  f0f0ff8:	0fc54d8a */ 	jal	text0f153628
/*  f0f0ffc:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f1000:	8fad0094 */ 	lw	$t5,0x94($sp)
/*  f0f1004:	8fac00ac */ 	lw	$t4,0xac($sp)
/*  f0f1008:	8fae009c */ 	lw	$t6,0x9c($sp)
/*  f0f100c:	8fb800b0 */ 	lw	$t8,0xb0($sp)
/*  f0f1010:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f1014:	01eec821 */ 	addu	$t9,$t7,$t6
/*  f0f1018:	03381823 */ 	subu	$v1,$t9,$t8
/*  f0f101c:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f1020:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f1024:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f1028:	00408825 */ 	or	$s1,$v0,$zero
/*  f0f102c:	04610003 */ 	bgez	$v1,.L0f0f103c
/*  f0f1030:	00035843 */ 	sra	$t3,$v1,0x1
/*  f0f1034:	24610001 */ 	addiu	$at,$v1,0x1
/*  f0f1038:	00015843 */ 	sra	$t3,$at,0x1
.L0f0f103c:
/*  f0f103c:	01601825 */ 	or	$v1,$t3,$zero
/*  f0f1040:	05210003 */ 	bgez	$t1,.L0f0f1050
/*  f0f1044:	00095043 */ 	sra	$t2,$t1,0x1
/*  f0f1048:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f104c:	00015043 */ 	sra	$t2,$at,0x1
.L0f0f1050:
/*  f0f1050:	006a6823 */ 	subu	$t5,$v1,$t2
/*  f0f1054:	25ac0002 */ 	addiu	$t4,$t5,0x2
/*  f0f1058:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f105c:	24c60002 */ 	addiu	$a2,$a2,0x2
/*  f0f1060:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f1064:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1068:	0fc5b9f1 */ 	jal	langGet
/*  f0f106c:	afa30044 */ 	sw	$v1,0x44($sp)
/*  f0f1070:	0c002f02 */ 	jal	viGetWidth
/*  f0f1074:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1078:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f107c:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f1080:	0c002f02 */ 	jal	viGetWidth
/*  f0f1084:	01c08025 */ 	or	$s0,$t6,$zero
/*  f0f1088:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f108c:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f1090:	240b00ff */ 	addiu	$t3,$zero,0xff
/*  f0f1094:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1098:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f109c:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f10a0:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f10a4:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f10a8:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f10ac:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f10b0:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f10b4:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f10b8:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f10bc:	0fc5580f */ 	jal	textRenderProjected
/*  f0f10c0:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f10c4:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f10c8:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f10cc:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f10d0:	00408825 */ 	or	$s1,$v0,$zero
/*  f0f10d4:	05410003 */ 	bgez	$t2,.L0f0f10e4
/*  f0f10d8:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f10dc:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f10e0:	00016843 */ 	sra	$t5,$at,0x1
.L0f0f10e4:
/*  f0f10e4:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f10e8:	258f0002 */ 	addiu	$t7,$t4,0x2
/*  f0f10ec:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f10f0:	afaf0088 */ 	sw	$t7,0x88($sp)
/*  f0f10f4:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f10f8:	0fc5b9f1 */ 	jal	langGet
/*  f0f10fc:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0f1100:	0c002f02 */ 	jal	viGetWidth
/*  f0f1104:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1108:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f110c:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f1110:	0c002f02 */ 	jal	viGetWidth
/*  f0f1114:	01c08025 */ 	or	$s0,$t6,$zero
/*  f0f1118:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f111c:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f1120:	240b00ff */ 	addiu	$t3,$zero,0xff
/*  f0f1124:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f1128:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f112c:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1130:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1134:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1138:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f113c:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1140:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1144:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1148:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f114c:	0fc5580f */ 	jal	textRenderProjected
/*  f0f1150:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1154:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f1158:	8faa0044 */ 	lw	$t2,0x44($sp)
/*  f0f115c:	8faf0040 */ 	lw	$t7,0x40($sp)
/*  f0f1160:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f1164:	00408825 */ 	or	$s1,$v0,$zero
/*  f0f1168:	05210003 */ 	bgez	$t1,.L0f0f1178
/*  f0f116c:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f1170:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f1174:	00016843 */ 	sra	$t5,$at,0x1
.L0f0f1178:
/*  f0f1178:	014d6023 */ 	subu	$t4,$t2,$t5
/*  f0f117c:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f1180:	95e40000 */ 	lhu	$a0,0x0($t7)
/*  f0f1184:	0fc5b9f1 */ 	jal	langGet
/*  f0f1188:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f118c:	0c002f02 */ 	jal	viGetWidth
/*  f0f1190:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1194:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1198:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f119c:	0c002f02 */ 	jal	viGetWidth
/*  f0f11a0:	01c08025 */ 	or	$s0,$t6,$zero
/*  f0f11a4:	8fb90064 */ 	lw	$t9,0x64($sp)
/*  f0f11a8:	8fb80060 */ 	lw	$t8,0x60($sp)
/*  f0f11ac:	3c0bbfbf */ 	lui	$t3,0xbfbf
/*  f0f11b0:	356bffff */ 	ori	$t3,$t3,0xffff
/*  f0f11b4:	afab0018 */ 	sw	$t3,0x18($sp)
/*  f0f11b8:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f11bc:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f11c0:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f11c4:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f11c8:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f11cc:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f11d0:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f11d4:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f11d8:	afb90010 */ 	sw	$t9,0x10($sp)
/*  f0f11dc:	0fc5580f */ 	jal	textRenderProjected
/*  f0f11e0:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f11e4:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0f11e8:	8fa60084 */ 	lw	$a2,0x84($sp)
/*  f0f11ec:	8fa90044 */ 	lw	$t1,0x44($sp)
/*  f0f11f0:	00408825 */ 	or	$s1,$v0,$zero
/*  f0f11f4:	05410003 */ 	bgez	$t2,.L0f0f1204
/*  f0f11f8:	000a6843 */ 	sra	$t5,$t2,0x1
/*  f0f11fc:	25410001 */ 	addiu	$at,$t2,0x1
/*  f0f1200:	00016843 */ 	sra	$t5,$at,0x1
.L0f0f1204:
/*  f0f1204:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0f1208:	012d6023 */ 	subu	$t4,$t1,$t5
/*  f0f120c:	afac0088 */ 	sw	$t4,0x88($sp)
/*  f0f1210:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f1214:	0fc5b9f1 */ 	jal	langGet
/*  f0f1218:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0f121c:	0c002f02 */ 	jal	viGetWidth
/*  f0f1220:	afa2004c */ 	sw	$v0,0x4c($sp)
/*  f0f1224:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1228:	00107c03 */ 	sra	$t7,$s0,0x10
/*  f0f122c:	0c002f02 */ 	jal	viGetWidth
/*  f0f1230:	01e08025 */ 	or	$s0,$t7,$zero
/*  f0f1234:	8fae0064 */ 	lw	$t6,0x64($sp)
/*  f0f1238:	8fb90060 */ 	lw	$t9,0x60($sp)
/*  f0f123c:	3c18bfbf */ 	lui	$t8,0xbfbf
/*  f0f1240:	3718ffff */ 	ori	$t8,$t8,0xffff
/*  f0f1244:	afb80018 */ 	sw	$t8,0x18($sp)
/*  f0f1248:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f124c:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f1250:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f1254:	8fa7004c */ 	lw	$a3,0x4c($sp)
/*  f0f1258:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f125c:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1260:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1264:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f1268:	afae0010 */ 	sw	$t6,0x10($sp)
/*  f0f126c:	0fc5580f */ 	jal	textRenderProjected
/*  f0f1270:	afb90014 */ 	sw	$t9,0x14($sp)
/*  f0f1274:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f1278:	8fa60080 */ 	lw	$a2,0x80($sp)
/*  f0f127c:	240151eb */ 	addiu	$at,$zero,0x51eb
/*  f0f1280:	956a0000 */ 	lhu	$t2,0x0($t3)
/*  f0f1284:	00408825 */ 	or	$s1,$v0,$zero
/*  f0f1288:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0f128c:	15410023 */ 	bne	$t2,$at,.L0f0f131c
/*  f0f1290:	24c6ffff */ 	addiu	$a2,$a2,-1
/*  f0f1294:	8fac0044 */ 	lw	$t4,0x44($sp)
/*  f0f1298:	05210003 */ 	bgez	$t1,.L0f0f12a8
/*  f0f129c:	00096843 */ 	sra	$t5,$t1,0x1
/*  f0f12a0:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0f12a4:	00016843 */ 	sra	$t5,$at,0x1
.L0f0f12a8:
/*  f0f12a8:	01ac7821 */ 	addu	$t7,$t5,$t4
/*  f0f12ac:	25eefff9 */ 	addiu	$t6,$t7,-7
/*  f0f12b0:	afae0088 */ 	sw	$t6,0x88($sp)
/*  f0f12b4:	0c002f02 */ 	jal	viGetWidth
/*  f0f12b8:	afa60084 */ 	sw	$a2,0x84($sp)
/*  f0f12bc:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f12c0:	0010cc03 */ 	sra	$t9,$s0,0x10
/*  f0f12c4:	0c002f02 */ 	jal	viGetWidth
/*  f0f12c8:	03208025 */ 	or	$s0,$t9,$zero
/*  f0f12cc:	3c188008 */ 	lui	$t8,%hi(g_CharsHandelGothicXs)
/*  f0f12d0:	3c0b8008 */ 	lui	$t3,%hi(g_FontHandelGothicXs)
/*  f0f12d4:	8d6bfb04 */ 	lw	$t3,%lo(g_FontHandelGothicXs)($t3)
/*  f0f12d8:	8f18fb08 */ 	lw	$t8,%lo(g_CharsHandelGothicXs)($t8)
/*  f0f12dc:	3c0abfbf */ 	lui	$t2,0xbfbf
/*  f0f12e0:	354affff */ 	ori	$t2,$t2,0xffff
/*  f0f12e4:	3c077f1b */ 	lui	$a3,%hi(var7f1b23e8)
/*  f0f12e8:	24e723e8 */ 	addiu	$a3,$a3,%lo(var7f1b23e8)
/*  f0f12ec:	afaa0018 */ 	sw	$t2,0x18($sp)
/*  f0f12f0:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f12f4:	27a50088 */ 	addiu	$a1,$sp,0x88
/*  f0f12f8:	27a60084 */ 	addiu	$a2,$sp,0x84
/*  f0f12fc:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0f1300:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0f1304:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f1308:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0f130c:	afab0014 */ 	sw	$t3,0x14($sp)
/*  f0f1310:	0fc5580f */ 	jal	textRenderProjected
/*  f0f1314:	afb80010 */ 	sw	$t8,0x10($sp)
/*  f0f1318:	00408825 */ 	or	$s1,$v0,$zero
.L0f0f131c:
/*  f0f131c:	0fc54de0 */ 	jal	text0f153780
/*  f0f1320:	02202025 */ 	or	$a0,$s1,$zero
/*  f0f1324:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f1328:	8fb00034 */ 	lw	$s0,0x34($sp)
/*  f0f132c:	8fb10038 */ 	lw	$s1,0x38($sp)
/*  f0f1330:	03e00008 */ 	jr	$ra
/*  f0f1334:	27bd0090 */ 	addiu	$sp,$sp,0x90
);
#else
GLOBAL_ASM(
glabel menuRenderBanner
/*  f0ed9ec:	27bdff68 */ 	addiu	$sp,$sp,-152
/*  f0ed9f0:	8fae00ac */ 	lw	$t6,0xac($sp)
/*  f0ed9f4:	afb30040 */ 	sw	$s3,0x40($sp)
/*  f0ed9f8:	afb2003c */ 	sw	$s2,0x3c($sp)
/*  f0ed9fc:	afb10038 */ 	sw	$s1,0x38($sp)
/*  f0eda00:	3c128008 */ 	lui	$s2,0x8008
/*  f0eda04:	3c138008 */ 	lui	$s3,0x8008
/*  f0eda08:	00808825 */ 	or	$s1,$a0,$zero
/*  f0eda0c:	afbf0044 */ 	sw	$ra,0x44($sp)
/*  f0eda10:	afb00034 */ 	sw	$s0,0x34($sp)
/*  f0eda14:	afa5009c */ 	sw	$a1,0x9c($sp)
/*  f0eda18:	afa600a0 */ 	sw	$a2,0xa0($sp)
/*  f0eda1c:	afa700a4 */ 	sw	$a3,0xa4($sp)
/*  f0eda20:	8e522370 */ 	lw	$s2,0x2370($s2)
/*  f0eda24:	11c00005 */ 	beqz	$t6,.NB0f0eda3c
/*  f0eda28:	8e73236c */ 	lw	$s3,0x236c($s3)
/*  f0eda2c:	3c128008 */ 	lui	$s2,0x8008
/*  f0eda30:	3c138008 */ 	lui	$s3,0x8008
/*  f0eda34:	8e522378 */ 	lw	$s2,0x2378($s2)
/*  f0eda38:	8e732374 */ 	lw	$s3,0x2374($s3)
.NB0f0eda3c:
/*  f0eda3c:	8faf00a0 */ 	lw	$t7,0xa0($sp)
/*  f0eda40:	8fb800a8 */ 	lw	$t8,0xa8($sp)
/*  f0eda44:	8fa800b0 */ 	lw	$t0,0xb0($sp)
/*  f0eda48:	3c0a8007 */ 	lui	$t2,0x8007
/*  f0eda4c:	01f83021 */ 	addu	$a2,$t7,$t8
/*  f0eda50:	254a3af4 */ 	addiu	$t2,$t2,0x3af4
/*  f0eda54:	00084840 */ 	sll	$t1,$t0,0x1
/*  f0eda58:	04c10003 */ 	bgez	$a2,.NB0f0eda68
/*  f0eda5c:	0006c843 */ 	sra	$t9,$a2,0x1
/*  f0eda60:	24c10001 */ 	addiu	$at,$a2,0x1
/*  f0eda64:	0001c843 */ 	sra	$t9,$at,0x1
.NB0f0eda68:
/*  f0eda68:	012a1021 */ 	addu	$v0,$t1,$t2
/*  f0eda6c:	94440000 */ 	lhu	$a0,0x0($v0)
/*  f0eda70:	afa20048 */ 	sw	$v0,0x48($sp)
/*  f0eda74:	0fc5a4dd */ 	jal	langGet
/*  f0eda78:	afb9008c */ 	sw	$t9,0x8c($sp)
/*  f0eda7c:	27a40084 */ 	addiu	$a0,$sp,0x84
/*  f0eda80:	27a50080 */ 	addiu	$a1,$sp,0x80
/*  f0eda84:	00403025 */ 	or	$a2,$v0,$zero
/*  f0eda88:	02403825 */ 	or	$a3,$s2,$zero
/*  f0eda8c:	afb30010 */ 	sw	$s3,0x10($sp)
/*  f0eda90:	0fc54655 */ 	jal	textMeasure
/*  f0eda94:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0eda98:	0fc5a4dd */ 	jal	langGet
/*  f0eda9c:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0edaa0:	27a4007c */ 	addiu	$a0,$sp,0x7c
/*  f0edaa4:	27a50078 */ 	addiu	$a1,$sp,0x78
/*  f0edaa8:	00403025 */ 	or	$a2,$v0,$zero
/*  f0edaac:	02403825 */ 	or	$a3,$s2,$zero
/*  f0edab0:	afb30010 */ 	sw	$s3,0x10($sp)
/*  f0edab4:	0fc54655 */ 	jal	textMeasure
/*  f0edab8:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0edabc:	8fa20084 */ 	lw	$v0,0x84($sp)
/*  f0edac0:	8fa3007c */ 	lw	$v1,0x7c($sp)
/*  f0edac4:	8fa6008c */ 	lw	$a2,0x8c($sp)
/*  f0edac8:	02202025 */ 	or	$a0,$s1,$zero
/*  f0edacc:	00435821 */ 	addu	$t3,$v0,$v1
/*  f0edad0:	05610003 */ 	bgez	$t3,.NB0f0edae0
/*  f0edad4:	000b6043 */ 	sra	$t4,$t3,0x1
/*  f0edad8:	25610001 */ 	addiu	$at,$t3,0x1
/*  f0edadc:	00016043 */ 	sra	$t4,$at,0x1
.NB0f0edae0:
/*  f0edae0:	00cc3023 */ 	subu	$a2,$a2,$t4
/*  f0edae4:	00c27021 */ 	addu	$t6,$a2,$v0
/*  f0edae8:	01c37821 */ 	addu	$t7,$t6,$v1
/*  f0edaec:	24cdfffc */ 	addiu	$t5,$a2,-4
/*  f0edaf0:	25f80007 */ 	addiu	$t8,$t7,0x7
/*  f0edaf4:	afa60088 */ 	sw	$a2,0x88($sp)
/*  f0edaf8:	afad0074 */ 	sw	$t5,0x74($sp)
/*  f0edafc:	afb80070 */ 	sw	$t8,0x70($sp)
/*  f0edb00:	afa6008c */ 	sw	$a2,0x8c($sp)
/*  f0edb04:	0fc537b7 */ 	jal	textSetPrimColour
/*  f0edb08:	2405007f */ 	addiu	$a1,$zero,0x7f
/*  f0edb0c:	3c038008 */ 	lui	$v1,0x8008
/*  f0edb10:	24632320 */ 	addiu	$v1,$v1,0x2320
/*  f0edb14:	8c680000 */ 	lw	$t0,0x0($v1)
/*  f0edb18:	8fb900a4 */ 	lw	$t9,0xa4($sp)
/*  f0edb1c:	8fad00a8 */ 	lw	$t5,0xa8($sp)
/*  f0edb20:	3c10f600 */ 	lui	$s0,0xf600
/*  f0edb24:	03280019 */ 	multu	$t9,$t0
/*  f0edb28:	31ae03ff */ 	andi	$t6,$t5,0x3ff
/*  f0edb2c:	000e7880 */ 	sll	$t7,$t6,0x2
/*  f0edb30:	8fb10074 */ 	lw	$s1,0x74($sp)
/*  f0edb34:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0edb38:	00004812 */ 	mflo	$t1
/*  f0edb3c:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0edb40:	000a5b80 */ 	sll	$t3,$t2,0xe
/*  f0edb44:	01706025 */ 	or	$t4,$t3,$s0
/*  f0edb48:	018fc025 */ 	or	$t8,$t4,$t7
/*  f0edb4c:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0edb50:	8c680000 */ 	lw	$t0,0x0($v1)
/*  f0edb54:	8fb9009c */ 	lw	$t9,0x9c($sp)
/*  f0edb58:	8fad00a0 */ 	lw	$t5,0xa0($sp)
/*  f0edb5c:	03280019 */ 	multu	$t9,$t0
/*  f0edb60:	31ae03ff */ 	andi	$t6,$t5,0x3ff
/*  f0edb64:	000e6080 */ 	sll	$t4,$t6,0x2
/*  f0edb68:	00004812 */ 	mflo	$t1
/*  f0edb6c:	312a03ff */ 	andi	$t2,$t1,0x3ff
/*  f0edb70:	000a5b80 */ 	sll	$t3,$t2,0xe
/*  f0edb74:	016c7825 */ 	or	$t7,$t3,$t4
/*  f0edb78:	0fc537ce */ 	jal	text0f153838
/*  f0edb7c:	ac4f0004 */ 	sw	$t7,0x4($v0)
/*  f0edb80:	00402025 */ 	or	$a0,$v0,$zero
/*  f0edb84:	0fc537b7 */ 	jal	textSetPrimColour
/*  f0edb88:	24057f7f */ 	addiu	$a1,$zero,0x7f7f
/*  f0edb8c:	3c038008 */ 	lui	$v1,0x8008
/*  f0edb90:	24632320 */ 	addiu	$v1,$v1,0x2320
/*  f0edb94:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0edb98:	8fb800a4 */ 	lw	$t8,0xa4($sp)
/*  f0edb9c:	8fae0070 */ 	lw	$t6,0x70($sp)
/*  f0edba0:	24440008 */ 	addiu	$a0,$v0,0x8
/*  f0edba4:	03190019 */ 	multu	$t8,$t9
/*  f0edba8:	31cb03ff */ 	andi	$t3,$t6,0x3ff
/*  f0edbac:	000b6080 */ 	sll	$t4,$t3,0x2
/*  f0edbb0:	322e03ff */ 	andi	$t6,$s1,0x3ff
/*  f0edbb4:	000e5880 */ 	sll	$t3,$t6,0x2
/*  f0edbb8:	00004012 */ 	mflo	$t0
/*  f0edbbc:	310903ff */ 	andi	$t1,$t0,0x3ff
/*  f0edbc0:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0edbc4:	01506825 */ 	or	$t5,$t2,$s0
/*  f0edbc8:	01ac7825 */ 	or	$t7,$t5,$t4
/*  f0edbcc:	ac4f0000 */ 	sw	$t7,0x0($v0)
/*  f0edbd0:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0edbd4:	8fb8009c */ 	lw	$t8,0x9c($sp)
/*  f0edbd8:	03190019 */ 	multu	$t8,$t9
/*  f0edbdc:	00004012 */ 	mflo	$t0
/*  f0edbe0:	310903ff */ 	andi	$t1,$t0,0x3ff
/*  f0edbe4:	00095380 */ 	sll	$t2,$t1,0xe
/*  f0edbe8:	014b6825 */ 	or	$t5,$t2,$t3
/*  f0edbec:	0fc537ce */ 	jal	text0f153838
/*  f0edbf0:	ac4d0004 */ 	sw	$t5,0x4($v0)
/*  f0edbf4:	3c057f7f */ 	lui	$a1,0x7f7f
/*  f0edbf8:	34a5ff7f */ 	ori	$a1,$a1,0xff7f
/*  f0edbfc:	0fc537b7 */ 	jal	textSetPrimColour
/*  f0edc00:	00402025 */ 	or	$a0,$v0,$zero
/*  f0edc04:	3c038008 */ 	lui	$v1,0x8008
/*  f0edc08:	24632320 */ 	addiu	$v1,$v1,0x2320
/*  f0edc0c:	8c6f0000 */ 	lw	$t7,0x0($v1)
/*  f0edc10:	8fac00a4 */ 	lw	$t4,0xa4($sp)
/*  f0edc14:	8fae0070 */ 	lw	$t6,0x70($sp)
/*  f0edc18:	24440010 */ 	addiu	$a0,$v0,0x10
/*  f0edc1c:	018f0019 */ 	multu	$t4,$t7
/*  f0edc20:	25ca0004 */ 	addiu	$t2,$t6,0x4
/*  f0edc24:	314b03ff */ 	andi	$t3,$t2,0x3ff
/*  f0edc28:	000b6880 */ 	sll	$t5,$t3,0x2
/*  f0edc2c:	0000c012 */ 	mflo	$t8
/*  f0edc30:	331903ff */ 	andi	$t9,$t8,0x3ff
/*  f0edc34:	00194380 */ 	sll	$t0,$t9,0xe
/*  f0edc38:	01104825 */ 	or	$t1,$t0,$s0
/*  f0edc3c:	012d6025 */ 	or	$t4,$t1,$t5
/*  f0edc40:	ac4c0000 */ 	sw	$t4,0x0($v0)
/*  f0edc44:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0edc48:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0edc4c:	8faa0070 */ 	lw	$t2,0x70($sp)
/*  f0edc50:	01f80019 */ 	multu	$t7,$t8
/*  f0edc54:	254b0002 */ 	addiu	$t3,$t2,0x2
/*  f0edc58:	316903ff */ 	andi	$t1,$t3,0x3ff
/*  f0edc5c:	00096880 */ 	sll	$t5,$t1,0x2
/*  f0edc60:	2629fffe */ 	addiu	$t1,$s1,-2
/*  f0edc64:	0000c812 */ 	mflo	$t9
/*  f0edc68:	332803ff */ 	andi	$t0,$t9,0x3ff
/*  f0edc6c:	00087380 */ 	sll	$t6,$t0,0xe
/*  f0edc70:	01cd6025 */ 	or	$t4,$t6,$t5
/*  f0edc74:	ac4c0004 */ 	sw	$t4,0x4($v0)
/*  f0edc78:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0edc7c:	8faf00a4 */ 	lw	$t7,0xa4($sp)
/*  f0edc80:	312e03ff */ 	andi	$t6,$t1,0x3ff
/*  f0edc84:	000e6880 */ 	sll	$t5,$t6,0x2
/*  f0edc88:	01f80019 */ 	multu	$t7,$t8
/*  f0edc8c:	2629fffc */ 	addiu	$t1,$s1,-4
/*  f0edc90:	312e03ff */ 	andi	$t6,$t1,0x3ff
/*  f0edc94:	0000c812 */ 	mflo	$t9
/*  f0edc98:	332803ff */ 	andi	$t0,$t9,0x3ff
/*  f0edc9c:	00085380 */ 	sll	$t2,$t0,0xe
/*  f0edca0:	01505825 */ 	or	$t3,$t2,$s0
/*  f0edca4:	016d6025 */ 	or	$t4,$t3,$t5
/*  f0edca8:	ac4c0008 */ 	sw	$t4,0x8($v0)
/*  f0edcac:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0edcb0:	8faf009c */ 	lw	$t7,0x9c($sp)
/*  f0edcb4:	000e5880 */ 	sll	$t3,$t6,0x2
/*  f0edcb8:	01f80019 */ 	multu	$t7,$t8
/*  f0edcbc:	0000c812 */ 	mflo	$t9
/*  f0edcc0:	332803ff */ 	andi	$t0,$t9,0x3ff
/*  f0edcc4:	00085380 */ 	sll	$t2,$t0,0xe
/*  f0edcc8:	014b6825 */ 	or	$t5,$t2,$t3
/*  f0edccc:	0fc537ce */ 	jal	text0f153838
/*  f0edcd0:	ac4d000c */ 	sw	$t5,0xc($v0)
/*  f0edcd4:	0fc5374a */ 	jal	text0f153628
/*  f0edcd8:	00402025 */ 	or	$a0,$v0,$zero
/*  f0edcdc:	8fac009c */ 	lw	$t4,0x9c($sp)
/*  f0edce0:	8faf00a4 */ 	lw	$t7,0xa4($sp)
/*  f0edce4:	8fb90080 */ 	lw	$t9,0x80($sp)
/*  f0edce8:	8fa6008c */ 	lw	$a2,0x8c($sp)
/*  f0edcec:	018f1821 */ 	addu	$v1,$t4,$t7
/*  f0edcf0:	8faa0048 */ 	lw	$t2,0x48($sp)
/*  f0edcf4:	24c60002 */ 	addiu	$a2,$a2,0x2
/*  f0edcf8:	00408825 */ 	or	$s1,$v0,$zero
/*  f0edcfc:	04610003 */ 	bgez	$v1,.NB0f0edd0c
/*  f0edd00:	0003c043 */ 	sra	$t8,$v1,0x1
/*  f0edd04:	24610001 */ 	addiu	$at,$v1,0x1
/*  f0edd08:	0001c043 */ 	sra	$t8,$at,0x1
.NB0f0edd0c:
/*  f0edd0c:	03001825 */ 	or	$v1,$t8,$zero
/*  f0edd10:	07210003 */ 	bgez	$t9,.NB0f0edd20
/*  f0edd14:	00194043 */ 	sra	$t0,$t9,0x1
/*  f0edd18:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0edd1c:	00014043 */ 	sra	$t0,$at,0x1
.NB0f0edd20:
/*  f0edd20:	00684823 */ 	subu	$t1,$v1,$t0
/*  f0edd24:	252e0002 */ 	addiu	$t6,$t1,0x2
/*  f0edd28:	afae0090 */ 	sw	$t6,0x90($sp)
/*  f0edd2c:	95440000 */ 	lhu	$a0,0x0($t2)
/*  f0edd30:	afa6008c */ 	sw	$a2,0x8c($sp)
/*  f0edd34:	0fc5a4dd */ 	jal	langGet
/*  f0edd38:	afa3004c */ 	sw	$v1,0x4c($sp)
/*  f0edd3c:	0c002f77 */ 	jal	viGetWidth
/*  f0edd40:	afa20054 */ 	sw	$v0,0x54($sp)
/*  f0edd44:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0edd48:	00105c03 */ 	sra	$t3,$s0,0x10
/*  f0edd4c:	0c002f77 */ 	jal	viGetWidth
/*  f0edd50:	01608025 */ 	or	$s0,$t3,$zero
/*  f0edd54:	240d00ff */ 	addiu	$t5,$zero,0xff
/*  f0edd58:	afad0018 */ 	sw	$t5,0x18($sp)
/*  f0edd5c:	02202025 */ 	or	$a0,$s1,$zero
/*  f0edd60:	27a50090 */ 	addiu	$a1,$sp,0x90
/*  f0edd64:	27a6008c */ 	addiu	$a2,$sp,0x8c
/*  f0edd68:	8fa70054 */ 	lw	$a3,0x54($sp)
/*  f0edd6c:	afb20010 */ 	sw	$s2,0x10($sp)
/*  f0edd70:	afb30014 */ 	sw	$s3,0x14($sp)
/*  f0edd74:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0edd78:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0edd7c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0edd80:	0fc541a6 */ 	jal	textRenderProjected
/*  f0edd84:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0edd88:	8faf0078 */ 	lw	$t7,0x78($sp)
/*  f0edd8c:	8fac004c */ 	lw	$t4,0x4c($sp)
/*  f0edd90:	8fa6008c */ 	lw	$a2,0x8c($sp)
/*  f0edd94:	00408825 */ 	or	$s1,$v0,$zero
/*  f0edd98:	05e10003 */ 	bgez	$t7,.NB0f0edda8
/*  f0edd9c:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0edda0:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0edda4:	0001c043 */ 	sra	$t8,$at,0x1
.NB0f0edda8:
/*  f0edda8:	0198c823 */ 	subu	$t9,$t4,$t8
/*  f0eddac:	27280002 */ 	addiu	$t0,$t9,0x2
/*  f0eddb0:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0eddb4:	afa80090 */ 	sw	$t0,0x90($sp)
/*  f0eddb8:	afa6008c */ 	sw	$a2,0x8c($sp)
/*  f0eddbc:	0fc5a4dd */ 	jal	langGet
/*  f0eddc0:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0eddc4:	0c002f77 */ 	jal	viGetWidth
/*  f0eddc8:	afa20054 */ 	sw	$v0,0x54($sp)
/*  f0eddcc:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0eddd0:	00104c03 */ 	sra	$t1,$s0,0x10
/*  f0eddd4:	0c002f77 */ 	jal	viGetWidth
/*  f0eddd8:	01208025 */ 	or	$s0,$t1,$zero
/*  f0edddc:	240e00ff */ 	addiu	$t6,$zero,0xff
/*  f0edde0:	afae0018 */ 	sw	$t6,0x18($sp)
/*  f0edde4:	02202025 */ 	or	$a0,$s1,$zero
/*  f0edde8:	27a50090 */ 	addiu	$a1,$sp,0x90
/*  f0eddec:	27a6008c */ 	addiu	$a2,$sp,0x8c
/*  f0eddf0:	8fa70054 */ 	lw	$a3,0x54($sp)
/*  f0eddf4:	afb20010 */ 	sw	$s2,0x10($sp)
/*  f0eddf8:	afb30014 */ 	sw	$s3,0x14($sp)
/*  f0eddfc:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0ede00:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0ede04:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0ede08:	0fc541a6 */ 	jal	textRenderProjected
/*  f0ede0c:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0ede10:	8fab0080 */ 	lw	$t3,0x80($sp)
/*  f0ede14:	8faa004c */ 	lw	$t2,0x4c($sp)
/*  f0ede18:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0ede1c:	8fa60088 */ 	lw	$a2,0x88($sp)
/*  f0ede20:	00408825 */ 	or	$s1,$v0,$zero
/*  f0ede24:	05610003 */ 	bgez	$t3,.NB0f0ede34
/*  f0ede28:	000b6843 */ 	sra	$t5,$t3,0x1
/*  f0ede2c:	25610001 */ 	addiu	$at,$t3,0x1
/*  f0ede30:	00016843 */ 	sra	$t5,$at,0x1
.NB0f0ede34:
/*  f0ede34:	014d7823 */ 	subu	$t7,$t2,$t5
/*  f0ede38:	afaf0090 */ 	sw	$t7,0x90($sp)
/*  f0ede3c:	95840000 */ 	lhu	$a0,0x0($t4)
/*  f0ede40:	0fc5a4dd */ 	jal	langGet
/*  f0ede44:	afa6008c */ 	sw	$a2,0x8c($sp)
/*  f0ede48:	0c002f77 */ 	jal	viGetWidth
/*  f0ede4c:	afa20054 */ 	sw	$v0,0x54($sp)
/*  f0ede50:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0ede54:	0010c403 */ 	sra	$t8,$s0,0x10
/*  f0ede58:	0c002f77 */ 	jal	viGetWidth
/*  f0ede5c:	03008025 */ 	or	$s0,$t8,$zero
/*  f0ede60:	3c19bfbf */ 	lui	$t9,0xbfbf
/*  f0ede64:	3739ffff */ 	ori	$t9,$t9,0xffff
/*  f0ede68:	afb90018 */ 	sw	$t9,0x18($sp)
/*  f0ede6c:	02202025 */ 	or	$a0,$s1,$zero
/*  f0ede70:	27a50090 */ 	addiu	$a1,$sp,0x90
/*  f0ede74:	27a6008c */ 	addiu	$a2,$sp,0x8c
/*  f0ede78:	8fa70054 */ 	lw	$a3,0x54($sp)
/*  f0ede7c:	afb20010 */ 	sw	$s2,0x10($sp)
/*  f0ede80:	afb30014 */ 	sw	$s3,0x14($sp)
/*  f0ede84:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0ede88:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0ede8c:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0ede90:	0fc541a6 */ 	jal	textRenderProjected
/*  f0ede94:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0ede98:	8fa90078 */ 	lw	$t1,0x78($sp)
/*  f0ede9c:	8fa6008c */ 	lw	$a2,0x8c($sp)
/*  f0edea0:	8fa8004c */ 	lw	$t0,0x4c($sp)
/*  f0edea4:	00408825 */ 	or	$s1,$v0,$zero
/*  f0edea8:	05210003 */ 	bgez	$t1,.NB0f0edeb8
/*  f0edeac:	00097043 */ 	sra	$t6,$t1,0x1
/*  f0edeb0:	25210001 */ 	addiu	$at,$t1,0x1
/*  f0edeb4:	00017043 */ 	sra	$t6,$at,0x1
.NB0f0edeb8:
/*  f0edeb8:	24c60003 */ 	addiu	$a2,$a2,0x3
/*  f0edebc:	010e5823 */ 	subu	$t3,$t0,$t6
/*  f0edec0:	afab0090 */ 	sw	$t3,0x90($sp)
/*  f0edec4:	afa6008c */ 	sw	$a2,0x8c($sp)
/*  f0edec8:	0fc5a4dd */ 	jal	langGet
/*  f0edecc:	240451ef */ 	addiu	$a0,$zero,0x51ef
/*  f0eded0:	0c002f77 */ 	jal	viGetWidth
/*  f0eded4:	afa20054 */ 	sw	$v0,0x54($sp)
/*  f0eded8:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0ededc:	00105403 */ 	sra	$t2,$s0,0x10
/*  f0edee0:	0c002f77 */ 	jal	viGetWidth
/*  f0edee4:	01408025 */ 	or	$s0,$t2,$zero
/*  f0edee8:	3c0dbfbf */ 	lui	$t5,0xbfbf
/*  f0edeec:	35adffff */ 	ori	$t5,$t5,0xffff
/*  f0edef0:	afad0018 */ 	sw	$t5,0x18($sp)
/*  f0edef4:	02202025 */ 	or	$a0,$s1,$zero
/*  f0edef8:	27a50090 */ 	addiu	$a1,$sp,0x90
/*  f0edefc:	27a6008c */ 	addiu	$a2,$sp,0x8c
/*  f0edf00:	8fa70054 */ 	lw	$a3,0x54($sp)
/*  f0edf04:	afb20010 */ 	sw	$s2,0x10($sp)
/*  f0edf08:	afb30014 */ 	sw	$s3,0x14($sp)
/*  f0edf0c:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0edf10:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0edf14:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0edf18:	0fc541a6 */ 	jal	textRenderProjected
/*  f0edf1c:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0edf20:	0fc537a0 */ 	jal	text0f153780
/*  f0edf24:	00402025 */ 	or	$a0,$v0,$zero
/*  f0edf28:	8fbf0044 */ 	lw	$ra,0x44($sp)
/*  f0edf2c:	8fb00034 */ 	lw	$s0,0x34($sp)
/*  f0edf30:	8fb10038 */ 	lw	$s1,0x38($sp)
/*  f0edf34:	8fb2003c */ 	lw	$s2,0x3c($sp)
/*  f0edf38:	8fb30040 */ 	lw	$s3,0x40($sp)
/*  f0edf3c:	03e00008 */ 	jr	$ra
/*  f0edf40:	27bd0098 */ 	addiu	$sp,$sp,0x98
);
#endif

u32 var8007144c = 0x51e451e5;
u32 var80071450 = 0x51e651e7;
u32 var80071454 = 0x51e851e9;
u32 var80071458 = 0x51ea51eb;
u32 var8007145c = 0x51ec51ed;
u32 var80071460 = 0x51ee0000;

#if VERSION >= VERSION_NTSC_1_0 && VERSION != VERSION_JPN_FINAL
const char var7f1b23e8[] = "TM";
#endif

// Mismatch: Regalloc at first y calculation. Note that the calculation of tmp
// is postponed until later (tmp is eventually stored in a compiler-managed
// stack slot).
//#if VERSION >= VERSION_NTSC_1_0
//Gfx *menuRenderBanner(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, bool big, s32 msgnum, s32 arg7, s32 arg8)
//#else
//Gfx *menuRenderBanner(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, bool big, s32 msgnum)
//#endif
//{
//	s32 tmp;
//	s32 x; // 88
//	s32 y; // 84
//	s32 texttop; // 80
//	s32 textheight; // 7c
//	s32 textwidth; // 78
//	s32 waitheight; // 74
//	s32 waitwidth; // 70
//	s32 bannertop; // 6c
//	s32 bannerbottom; // 68
//	struct fontchar *chars; // 64
//	struct font *font; // 60
//
//	static u16 msgs[] = {
//		L_MPMENU_484, // "Searching for Camera!"
//		L_MPMENU_485, // "Calibrating Camera"
//		L_MPMENU_486, // "Downloading Image"
//		L_MPMENU_487, // "Loading Image"
//		L_MPMENU_488, // "Saving Image"
//		L_MPMENU_489, // "Transferring Image"
//		L_MPMENU_490, // "Uploading Segment"
//		L_MPMENU_491, // "Checking Controller Pak"
//		L_MPMENU_492, // "Getting PerfectHead"
//		L_MPMENU_493, // "Saving PerfectHead"
//		L_MPMENU_494, // "Auto Camera Adjustment"
//	};
//
//	chars = g_CharsHandelGothicSm;
//	font = g_FontHandelGothicSm;
//
//	if (big) {
//#if VERSION >= VERSION_PAL_FINAL
//		if (g_CharsHandelGothicMd) {
//			chars = g_CharsHandelGothicMd;
//			font = g_FontHandelGothicMd;
//		}
//#else
//		chars = g_CharsHandelGothicMd;
//		font = g_FontHandelGothicMd;
//#endif
//	}
//
//#if VERSION >= VERSION_NTSC_1_0
//	tmp = x1 + arg7 + x2 - arg8;
//	tmp /= 2;
//#else
//	tmp = x1 + x2;
//	tmp /= 2;
//#endif
//
//	y = y1 + y2;
//	y /= 2;
//
//	textMeasure(&textheight, &textwidth, langGet(msgs[msgnum]), chars, font, 0);
//
//	// "Please Wait..."
//	textMeasure(&waitheight, &waitwidth, langGet(L_MPMENU_495), chars, font, 0);
//
//#if VERSION >= VERSION_NTSC_1_0
//	if (msgs[msgnum] == L_MPMENU_491) { // "Checking Controller Pak"
//		// Add space for "TM"
//		textwidth += 7;
//	}
//#endif
//
//	texttop = y - (textheight + waitheight) / 2;
//	bannertop = texttop - 4;
//	bannerbottom = texttop + textheight + waitheight + 7;
//
//	y = texttop;
//
//	// Black fill
//	gdl = textSetPrimColour(gdl, 0x0000007f);
//	gDPFillRectangleScaled(gdl++, x1, y1, x2, y2);
//	gdl = text0f153838(gdl);
//
//	// Dark blue fill
//	gdl = textSetPrimColour(gdl, 0x00007f7f);
//	gDPFillRectangleScaled(gdl++, x1, bannertop, x2, bannerbottom);
//	gdl = text0f153838(gdl);
//
//	// Top and bottom borders (light blue)
//	gdl = textSetPrimColour(gdl, 0x7f7fff7f);
//	gDPFillRectangleScaled(gdl++, x1, bannerbottom + 2, x2, bannerbottom + 4);
//	gDPFillRectangleScaled(gdl++, x1, bannertop - 4, x2, bannertop - 2);
//	gdl = text0f153838(gdl);
//
//	gdl = text0f153628(gdl);
//
//	// Render the selected message's shadow
//	x = tmp - textwidth / 2 + 2;
//	y += 2;
//	gdl = textRenderProjected(gdl, &x, &y, langGet(msgs[msgnum]),
//			chars, font, 0x000000ff, viGetWidth(), viGetWidth(), 0, 0);
//
//	// Render "Please Wait..." shadow
//	x = tmp - waitwidth / 2 + 2;
//	y += 3;
//	gdl = textRenderProjected(gdl, &x, &y, langGet(L_MPMENU_495),
//			chars, font, 0x000000ff, viGetWidth(), viGetWidth(), 0, 0);
//
//	// Render the selected message proper
//	x = tmp - textwidth / 2;
//	y = texttop;
//	gdl = textRenderProjected(gdl, &x, &y, langGet(msgs[msgnum]),
//			chars, font, 0xbfbfffff, viGetWidth(), viGetWidth(), 0, 0);
//
//	// Render "Please Wait..." proper
//	x = tmp - waitwidth / 2;
//	y += 3;
//	gdl = textRenderProjected(gdl, &x, &y, langGet(L_MPMENU_495),
//			chars, font, 0xbfbfffff, viGetWidth(), viGetWidth(), 0, 0);
//
//#if VERSION >= VERSION_NTSC_1_0
//	if (msgs[msgnum] == L_MPMENU_491) { // "Checking Controller Pak"
//		// Render "TM"
//		y = texttop - 1;
//		x = textwidth / 2 + tmp - 7;
//		gdl = textRenderProjected(gdl, &x, &y, "TM",
//				g_CharsHandelGothicXs, g_FontHandelGothicXs, 0xbfbfffff, viGetWidth(), viGetWidth(), 0, 0);
//	}
//#endif
//
//	gdl = text0f153780(gdl);
//
//	return gdl;
//}

u32 var80071464 = 0;

struct menudfc *func0f0f1338(struct menuitem *item)
{
	s32 i;

	for (i = 0; i < 4; i++) {
		if (g_Menus[g_MpPlayerNum].unkdfc[i].item == item) {
			return &g_Menus[g_MpPlayerNum].unkdfc[i];
		}
	}

	return NULL;
}

void func0f0f139c(struct menuitem *item, f32 arg1)
{
	struct menudfc *thing = func0f0f1338(item);

	if (thing) {
		thing->unk04 = arg1;
		return;
	}

	thing = func0f0f1338(NULL);

	if (thing) {
		thing->item = item;
		thing->unk04 = arg1;
	}
}

void func0f0f13ec(struct menuitem *item)
{
	struct menudfc *thing = func0f0f1338(item);

	if (thing) {
		thing->item = NULL;
	}
}

void func0f0f1418(void)
{
	s32 i;

	for (i = 0; i < 4; i++) {
		if (g_Menus[g_MpPlayerNum].unkdfc[i].item) {
#if VERSION >= VERSION_PAL_BETA
			g_Menus[g_MpPlayerNum].unkdfc[i].unk04 += g_Vars.diffframe60freal / 60.0f;
#else
			g_Menus[g_MpPlayerNum].unkdfc[i].unk04 += g_Vars.diffframe60f / 60.0f;
#endif
		}
	}
}

void func0f0f1494(void)
{
	s32 i;

	for (i = 0; i < 4; i++) {
		g_Menus[g_MpPlayerNum].unkdfc[i].item = NULL;
	}
}

char *menuResolveText(u32 thing, void *dialogoritem)
{
	char *(*handler)(void *dialogoritem) = (void *)thing;

	// Null/zero
	if (thing == 0) {
		return NULL;
	}

	// Text ID
	if (thing < 0x5a00) {
		return langGet((u32)thing);
	}

	if (thing > (u32)func0f1a78b0) {
#if VERSION < VERSION_NTSC_1_0
		CRASH();
#endif
		return NULL;
	}

	// Function pointer
	if (handler) {
		return handler(dialogoritem);
	}

	return "";
}

char *menuResolveParam2Text(struct menuitem *item)
{
	return menuResolveText(item->param2, item);
}

char *menuResolveDialogTitle(struct menudialogdef *dialogdef)
{
	return menuResolveText(dialogdef->title, dialogdef);
}

void func0f0f15a4(struct menuitem *item, s32 *arg1)
{
	switch (item->type) {
	case MENUITEMTYPE_SLIDER:
	case MENUITEMTYPE_CHECKBOX:
	case MENUITEMTYPE_RANKING:
	case MENUITEMTYPE_14:
	case MENUITEMTYPE_18:
		*arg1 = 1;
		break;
#if VERSION < VERSION_PAL_BETA
	case MENUITEMTYPE_SCROLLABLE:
#endif
	case MENUITEMTYPE_MARQUEE:
	case MENUITEMTYPE_CONTROLLER:
		*arg1 = 2;
		break;
	case MENUITEMTYPE_LIST:
#if VERSION >= VERSION_PAL_BETA
	case MENUITEMTYPE_SCROLLABLE:
#endif
		*arg1 = 3;
		break;
	case MENUITEMTYPE_DROPDOWN:
		*arg1 = 4;
		break;
	case MENUITEMTYPE_PLAYERSTATS:
		*arg1 = 5;
		break;
	case MENUITEMTYPE_KEYBOARD:
	case MENUITEMTYPE_10:
	case MENUITEMTYPE_16:
		*arg1 = 3;
		break;
	}
}

void menuCalculateItemSize(struct menuitem *item, s16 *width, s16 *height, struct menudialog *dialog)
{
	char *text;
	s32 textwidth;
	s32 textheight;
	struct fontchar *chars = g_CharsHandelGothicSm;
	struct font *font = g_FontHandelGothicSm;
	s32 i;
	union handlerdata handlerdata;
	union handlerdata handlerdata2;
	char *text2;
	s32 numobjectives;

	// Check if item's handler handles MENUOP_CHECKHIDDEN
	if (item->handler && (item->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0) {
		if (item->handler(MENUOP_CHECKHIDDEN, item, &handlerdata)) {
			*width = 0;
			*height = 0;
			return;
		}
	}

	switch (item->type) {
	case MENUITEMTYPE_CONTROLLER:
#if VERSION == VERSION_JPN_FINAL
		*height = 190;
		*width = 240;
#elif PAL
		*height = 156;
		*width = 230;
#else
		*height = 150;
		*width = 230;
#endif
		break;
	case MENUITEMTYPE_18:
		*height = item->param2 == 1 ? 170 : 126;
		*width = 210;
		break;
	case MENUITEMTYPE_14:
		*width = 90;
		*height = 54;
		break;
	case MENUITEMTYPE_METER:
		*width = 24;
		*height = 6;
		break;
	case MENUITEMTYPE_KEYBOARD:
		*width = 130;
		*height = 73;
		break;
	case MENUITEMTYPE_LIST:
		if (item->param2 > 0) {
			*width = item->param2;
		} else {
			*width = 80;

#if VERSION >= VERSION_NTSC_1_0
			if ((item->flags & MENUITEMFLAG_LIST_WIDE) != 0) {
				*width = 180;
			}
#else
			if ((item->flags && MENUITEMFLAG_LIST_WIDE) != 0) {
				*width = 180;
			}
#endif
		}

#if VERSION >= VERSION_JPN_FINAL
		*height = item->param3 > 0 ? item->param3 : 112;
#else
		*height = item->param3 > 0 ? item->param3 : 121;
#endif
		break;
	case MENUITEMTYPE_DROPDOWN:
		text = menuResolveParam2Text(item);

		if (text && strcmp(text, "") == 0) {
			*width = 0;
			*height = 0;
		} else {
			textwidth = 0;
			textheight = 0;

			if (text != NULL) {
				textMeasure(&textheight, &textwidth, text, g_CharsHandelGothicSm, g_FontHandelGothicSm, 0);
			}

			*width = textwidth + 20;
			*height = VERSION == VERSION_JPN_FINAL ? 14 : 12;

			if (item->handler) {
				handlerdata2.dropdown.value = 0;
				item->handler(MENUOP_GETSELECTEDINDEX, item, &handlerdata2);
				handlerdata2.dropdown.unk04 = 0;
				text2 = (char *)item->handler(MENUOP_GETOPTIONTEXT, item, &handlerdata2);
				textMeasure(&textheight, &textwidth, text2, g_CharsHandelGothicSm, g_FontHandelGothicSm, 0);

#if VERSION >= VERSION_PAL_FINAL
				if ((item->flags & MENUITEMFLAG_ADJUSTWIDTH) == 0) {
					*width += textwidth + 10;
				} else {
					*width += textwidth + 3;
				}
#else
				*width += textwidth + 10;
#endif
			}
		}
		break;
	case MENUITEMTYPE_13:
		*width = 70;
		*height = 50;
		break;
	case MENUITEMTYPE_SLIDER:
		if (dialog && dialog->unk6e) {
			*width = 120;
			*height = 22;
			return;
		}

		*width = 150;
		*height = VERSION == VERSION_JPN_FINAL ? 14 : 12;

		if (item->flags & MENUITEMFLAG_SLIDER_ALTSIZE) {
			*height = 22;
			*width = 120;
		}
		break;
	case MENUITEMTYPE_CHECKBOX:
		if (item->flags & MENUITEMFLAG_SMALLFONT) {
			chars = g_CharsHandelGothicXs;
			font = g_FontHandelGothicXs;
		}

		text = menuResolveParam2Text(item);

		if (text == NULL) {
			*width = 120;
		} else if (strcmp(text, "") == 0) {
			*width = 0;
			*height = 0;
		} else {
			textMeasure(&textheight, &textwidth, text, chars, font, 0);
			*width = (s16)textwidth + 34;
		}
		*height = VERSION == VERSION_JPN_FINAL ? 14 : 12;
		break;
	case MENUITEMTYPE_MODEL:
		*width = item->param2;
		*height = item->param3;
		break;
	case MENUITEMTYPE_SEPARATOR:
		*width = 1;
		if (item->param2) {
			*width = item->param2;
		}
		*height = VERSION == VERSION_JPN_FINAL ? 2 : 5;
		break;
	case MENUITEMTYPE_MARQUEE:
		*width = 1;
#if VERSION == VERSION_JPN_FINAL
		*height = LINEHEIGHT;
#else
		if (item->flags & MENUITEMFLAG_SMALLFONT) {
			*height = LINEHEIGHT;
		} else {
			*height = LINEHEIGHT + 2;
		}
#endif
		break;
	case MENUITEMTYPE_LABEL:
	case MENUITEMTYPE_SELECTABLE:
		text = menuResolveParam2Text(item);

		if (text == NULL) {
			*width = 0;
			*height = 0;
			return;
		}

		if (item->flags & MENUITEMFLAG_SMALLFONT) {
			chars = g_CharsHandelGothicXs;
			font = g_FontHandelGothicXs;
		}

		if (item->flags & MENUITEMFLAG_BIGFONT) {
			chars = g_CharsHandelGothicMd;
			font = g_FontHandelGothicMd;
		}

		if (strcmp(text, "") == 0) {
			*height = 0;
			*width = *height;
		} else {
			textMeasure(&textheight, &textwidth, text, chars, font, 0);
			*width = (s16)textwidth + 8;

			if ((item->flags & (MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_ADJUSTWIDTH)) == 0) {
				*width += 20;
			}

#if VERSION == VERSION_JPN_FINAL
			*height = textheight;
#else
			*height = textheight + 3;

			if (item->flags & MENUITEMFLAG_SMALLFONT) {
				*height -= 2;
			}
#endif

			if ((item->flags & (MENUITEMFLAG_LABEL_HASRIGHTTEXT | MENUITEMFLAG_BIGFONT)) == 0) {
				text = menuResolveText(item->param3, item);

				// @bug: This is not how you check for an empty string
				if (text != NULL && text != "") {
					textMeasure(&textheight, &textwidth, text, chars, font, 0);
					*width += textwidth + 5;

					if (item->flags & MENUITEMFLAG_ADJUSTWIDTH) {
						*width -= 6;
					}
				}
			}
		}

		if (item->flags & MENUITEMFLAG_BIGFONT) {
			*height = 28;
			*width += 36;
		}

		if (item->flags & MENUITEMFLAG_LESSHEIGHT) {
			*height -= 1;
		}
		break;
	case MENUITEMTYPE_SCROLLABLE:
		*width = item->param2 > 0 ? item->param2 : 240;
		*height = item->param3 > 0 ? item->param3 : 150;
		break;
	case MENUITEMTYPE_OBJECTIVES:
		numobjectives = 0;
		*width = 240;

		for (i = 0; i < 6; i++) {
			if (g_Briefing.objectivenames[i] && (g_Briefing.objectivedifficulties[i] & (1 << lvGetDifficulty()))) {
				numobjectives++;
			}
		}

#if VERSION >= VERSION_JPN_FINAL
		if (item->param == 0) {
			*height = 9 + numobjectives * 24;
		} else if (item->param == 1) {
			*height = 9 + numobjectives * 16;
		} else if (item->param == 2) {
			*height = 9 + numobjectives * 36;
			*height -= 5;
			*width = 120;
		}
#elif VERSION >= VERSION_PAL_FINAL
		if (item->param == 0) {
			*height = 9 + numobjectives * 18;
		} else if (item->param == 1) {
			*height = 9 + numobjectives * 14;
		} else if (item->param == 2) {
			*height = 9 + numobjectives * 34;
			*width = 120;
		}
#else
		if (item->param == 0) {
			*height = 9 + numobjectives * 18;
		} else if (item->param == 1) {
			*height = 9 + numobjectives * 14;
		} else if (item->param == 2) {
			*height = 9 + numobjectives * 30;
			*width = 120;
		}
#endif
		break;
	case MENUITEMTYPE_07:
		*width = 120;
		*height = 120;
		break;
	case MENUITEMTYPE_PLAYERSTATS:
		*width = 125;
		*height = 68;
		break;
	case MENUITEMTYPE_RANKING:
		*width = 125;
		*height = 58;
		break;
	case MENUITEMTYPE_10:
		*width = item->param2 ? item->param2 + 2 : 66;
		*height = *width;
		break;
	case MENUITEMTYPE_16:
		*width = 66;
		*height = *width;
		break;
	case MENUITEMTYPE_CAROUSEL:
		*width = 130;
		*height = item->param3;
		break;
	default:
		*width = 80;
		*height = 12;
		break;
	}
}

const char var7f1b2400[] = "(BNC:Menu) findItem Warning: Item not found\n";
const char var7f1b2430[] = "(BNC:Menu) menuFirstFocus - Menu %s does not have any focusable item!\n";
const char var7f1b2478[] = "(BNC:Menu) menuLastFocus - Menu %s does not have any focusable item!\n";
const char var7f1b24c0[] = "Init menu %x\n";
const char var7f1b24d0[] = "[]-[] Added Main menu to ctl slot %d\n";
const char var7f1b24f8[] = "[]-[] Added Slide menu to ctl slot %d\n";
const char var7f1b2520[] = "[]-[] StackStart Stats: slides %d, ctlPtr %d slidePtr %d\n";
const char var7f1b255c[] = "$-$-$avePtr: %d\n";
const char var7f1b2570[] = "[]-[] Menu End: now %d slides\n";
const char var7f1b2590[] = "Freeing challenge mem\n";
const char var7f1b25a8[] = "IG:) style %d gbHead:%d\n";
const char var7f1b25c4[] = "GRABBED GUN MEM!\n";
const char var7f1b25d8[] = "Freeing challenge mem\n";

void func0f0f1d6c(struct menudialogdef *dialogdef, struct menudialog *dialog, struct menu *menu)
{
	s32 colindex = menu->colend - 1;
	s32 rowindex = menu->rowend;
	s32 itemindex = 0;
	s32 numblocksthisitem;
	struct menuitem *item = dialogdef->items;
	s16 blockindex = menu->blockend;

	dialog->numcols = 0;
	dialog->colstart = (u16) colindex + 1;
	dialog->blockstart = blockindex;

	if (item) {
		bool newcolumn = true;

		while (item->type != MENUITEMTYPE_END) {
			if (item->flags & MENUITEMFLAG_NEWCOLUMN) {
				newcolumn = true;
			}

			if (newcolumn) {
				dialog->numcols++;
				colindex++;

				menu->cols[colindex].width = 0;
				menu->cols[colindex].height = 0;
				menu->cols[colindex].unk04 = 0;
				menu->cols[colindex].numrows = 0;
				menu->cols[colindex].rowstart = rowindex;

				newcolumn = false;
			}

			numblocksthisitem = -1;
			func0f0f15a4(item, &numblocksthisitem);

			if (numblocksthisitem != -1) {
				menu->rows[rowindex].blockindex = blockindex;
				blockindex += (s16)numblocksthisitem;
			} else {
				menu->rows[rowindex].blockindex = -1;
			}

			menu->rows[rowindex].itemindex = itemindex;
			menu->cols[colindex].numrows++;
			rowindex++;
			item++;
			itemindex++;
		}
	}

	menu->colend = colindex + 1;
	menu->rowend = rowindex;
	menu->blockend = blockindex;
}

void dialog0f0f1ef4(struct menudialog *dialog)
{
	s32 bodyheight = dialog->height - LINEHEIGHT - 1;
	s32 itemheight;
	s32 i;
	s32 j;

	if ((dialog->definition->flags & MENUDIALOGFLAG_SMOOTHSCROLLABLE) == 0
			&& g_MenuData.root != MENUROOT_TRAINING
			&& bodyheight < dialog->contentheight) {
		for (i = 0; i < dialog->numcols; i++) {
			s32 colindex = dialog->colstart + i;
			s32 remaining = g_Menus[g_MpPlayerNum].cols[colindex].height - bodyheight;

			if (remaining > 0) {
				for (j = 0; j < g_Menus[g_MpPlayerNum].cols[colindex].numrows; j++) {
					if (remaining > 0) {
						s32 itemheight = 0;
						s32 rowindex = g_Menus[g_MpPlayerNum].cols[colindex].rowstart + j;
						struct menuitem *item = &dialog->definition->items[g_Menus[g_MpPlayerNum].rows[rowindex].itemindex];

						switch (item->type) {
						case MENUITEMTYPE_LIST:
							if (item->flags & MENUITEMFLAG_LIST_CUSTOMRENDER) {
								itemheight = remaining;

								if (g_Menus[g_MpPlayerNum].rows[rowindex].height - itemheight < 30) {
									itemheight = g_Menus[g_MpPlayerNum].rows[rowindex].height - 30;
								}
							} else {
								itemheight = (remaining + 10) / 11 * 11;
							}
							break;
						case MENUITEMTYPE_SCROLLABLE:
						case MENUITEMTYPE_MODEL:
							itemheight = remaining;
							if (g_Menus[g_MpPlayerNum].rows[rowindex].height - remaining < 50) {
								itemheight = g_Menus[g_MpPlayerNum].rows[rowindex].height - 50;
							}
							break;
						}

						if (itemheight > 0) {
							g_Menus[g_MpPlayerNum].rows[rowindex].height -= itemheight;
							remaining -= itemheight;
						}
					}
				}
			}
		}
	}
}

void dialogCalculateContentSize(struct menudialogdef *dialogdef, struct menudialog *dialog, struct menu *menu)
{
	s32 contentheight;
	s32 rowindex;
	s32 contentwidth;
	s32 titleextra;
	s32 i;
	s32 textwidth;
	s32 textheight;
	u32 stack;
	struct menuitem *item = dialogdef->items;
	s32 colindex = dialog->colstart - 1;

	// Iterate items and calculate their dimensions
	if (item != NULL) {
		bool newcolumn = true;
		s16 width;
		s16 height;

		while (item->type != MENUITEMTYPE_END) {
			if (item->flags & MENUITEMFLAG_NEWCOLUMN) {
				newcolumn = true;
			}

			if (newcolumn) {
				colindex++;
				menu->cols[colindex].width = 0;
				menu->cols[colindex].height = 0;

				newcolumn = false;
				rowindex = menu->cols[colindex].rowstart;
			}

			menuCalculateItemSize(item, &width, &height, dialog);

			if (width > menu->cols[colindex].width) {
				menu->cols[colindex].width = width;
			}

			menu->rows[rowindex].height = height;
			menu->cols[colindex].height += height;
			rowindex++;
			item++;
		}
	}

	contentheight = 0;
	contentwidth = 0;

	for (i = 0; i < dialog->numcols; i++) {
		s32 columnheight = menu->cols[dialog->colstart + i].height;

		contentwidth += menu->cols[dialog->colstart + i].width;

		if (columnheight > contentheight) {
			contentheight = columnheight;
		}
	}

#if VERSION == VERSION_JPN_FINAL
	contentheight += 15;

	if ((dialog->definition->flags & MENUDIALOGFLAG_1000) == 0) {
		contentheight += 2;
	}
#else
	contentheight += 12;
#endif

	// Calculate and consider the title width.
	// Some of the multiplayer dialogs have a player number
	// in the top right, so extra space is considered for those.
	textMeasure(&textheight, &textwidth, menuResolveDialogTitle(dialog->definition), g_CharsHandelGothicSm, g_FontHandelGothicSm, 0);

	titleextra = 8;

	switch (g_MenuData.root) {
	case MENUROOT_MPSETUP:
	case MENUROOT_MPPAUSE:
	case MENUROOT_MPENDSCREEN:
	case MENUROOT_4MBMAINMENU:
		titleextra = 17;
		break;
	}

	if (textwidth + titleextra > contentwidth) {
		contentwidth = textwidth + titleextra;
	}

	dialog->contentwidth = contentwidth;
	dialog->contentheight = contentheight;
}

/**
 * Find the given item in the given dialog, and write its column and row indices
 * to the given pointers. Return the y value of the item relative to the dialog.
 */
s32 dialogFindItem(struct menudialog *dialog, struct menuitem *item, s32 *rowindex, s32 *colindex)
{
	for (*colindex = dialog->colstart; *colindex < dialog->colstart + dialog->numcols; *colindex += 1) {
		s32 y = 0;
		*rowindex = g_Menus[g_MpPlayerNum].cols[*colindex].rowstart;

		while (*rowindex < g_Menus[g_MpPlayerNum].cols[*colindex].rowstart + g_Menus[g_MpPlayerNum].cols[*colindex].numrows) {
			struct menuitem *thisitem = &dialog->definition->items[g_Menus[g_MpPlayerNum].rows[*rowindex].itemindex];

			if (thisitem == item) {
				return y;
			}

			y = y + g_Menus[g_MpPlayerNum].rows[*rowindex].height;
			*rowindex += 1;
		}
	}

	*colindex = dialog->colstart;
	*rowindex = g_Menus[g_MpPlayerNum].cols[*colindex].rowstart;

	return 0;
}

/**
 * If this returns true, the scrollable is rendered with less padding and
 * scrolling is disabled.
 */
bool menuIsScrollableUnscrollable(struct menuitem *item)
{
	if (item->type == MENUITEMTYPE_SCROLLABLE) {
		if (item->param == DESCRIPTION_MPCONFIG
				|| item->param == DESCRIPTION_MPCHALLENGE
				|| item->param == DESCRIPTION_DEVICETRAINING
				|| item->param == DESCRIPTION_FRWEAPON
				|| item->param == DESCRIPTION_HOLOTRAINING) {
			return true;
		}
	}

	return false;
}

bool menuIsItemDisabled(struct menuitem *item, struct menudialog *dialog)
{
	union handlerdata sp30;
	s16 width;
	s16 height;
	u32 stack[2];

	if (item->flags & MENUITEMFLAG_ALWAYSDISABLED) {
		return true;
	}

	if (mpIsPlayerLockedOut(g_MpPlayerNum) && item->flags & MENUITEMFLAG_LOCKABLEMAJOR) {
		return true;
	}

	if (menuIsScrollableUnscrollable(item)) {
		return true;
	}

	if (item->handler
			&& (item->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0
			&& item->handler(MENUOP_CHECKDISABLED, item, &sp30)) {
		return true;
	}

	menuCalculateItemSize(item, &width, &height, dialog);

	if (height == 0) {
		return true;
	}

	return false;
}

bool menuIsItemFocusable(struct menuitem *item, struct menudialog *dialog, s32 arg2)
{
	s32 rowindex;
	s32 colindex;

	switch (item->type) {
	case MENUITEMTYPE_LABEL:
	case MENUITEMTYPE_OBJECTIVES:
	case MENUITEMTYPE_07:
	case MENUITEMTYPE_SEPARATOR:
	case MENUITEMTYPE_MODEL:
	case MENUITEMTYPE_13:
	case MENUITEMTYPE_METER:
	case MENUITEMTYPE_MARQUEE:
	case MENUITEMTYPE_CONTROLLER:
		return false;
	case MENUITEMTYPE_10:
	case MENUITEMTYPE_14:
	case MENUITEMTYPE_16:
	case MENUITEMTYPE_18:
		dialogFindItem(dialog, item, &rowindex, &colindex);
	}

	if (menuIsItemDisabled(item, dialog)) {
		return false;
	}

	return true;
}

struct menuitem *dialogFindItemAtColY(s32 targety, s32 colindex, struct menudialogdef *dialogdef, s32 *rowindexptr, struct menudialog *dialog)
{
	struct menuitem *result = NULL;
	bool done = false;
	s32 rowindex = g_Menus[g_MpPlayerNum].cols[colindex].rowstart;
	s32 y;
	s32 i;

	for (i = 0, y = 0; !done && i < g_Menus[g_MpPlayerNum].cols[colindex].numrows; rowindex++, i++) {
		struct menuitem *item = &dialogdef->items[g_Menus[g_MpPlayerNum].rows[rowindex].itemindex];

		if (menuIsItemFocusable(item, dialog, 1)) {
			result = item;

			if (y >= targety) {
				done = true;
			}

			*rowindexptr = rowindex;
		}

		y = y + g_Menus[g_MpPlayerNum].rows[rowindex].height;
	}

	return result;
}

struct menuitem *dialogFindFirstItem(struct menudialog *dialog)
{
	s32 i;
	s32 colindex = dialog->colstart;
	s32 rowindex;

	for (i = 0; i < dialog->numcols; i++) {
		struct menuitem *item = dialogFindItemAtColY(0, colindex, dialog->definition, &rowindex, dialog);

		if (item != NULL) {
			return item;
		}

		colindex++;
	}

	menuResolveDialogTitle(dialog->definition);

	return dialog->definition->items;
}

struct menuitem *dialogFindFirstItemRight(struct menudialog *dialog)
{
	s32 i;
	s32 colindex = dialog->colstart + dialog->numcols - 1;
	s32 rowindex;

	for (i = 0; i < dialog->numcols; i++) {
		struct menuitem *item = dialogFindItemAtColY(0, colindex, dialog->definition, &rowindex, dialog);

		if (item != NULL) {
			return item;
		}

		colindex--;
	}

	menuResolveDialogTitle(dialog->definition);

	return dialog->definition->items;
}

void dialogChangeItemFocusVertically(struct menudialog *dialog, s32 updown)
{
	s32 rowindex;
	s32 colindex;
	bool done = false;
	s32 startrowindex;
	struct menuitem *item;
	s32 start;
	s32 end;

	dialogFindItem(dialog, dialog->focuseditem, &rowindex, &colindex);

	startrowindex = rowindex;

	while (!done) {
		rowindex += updown;
		start = g_Menus[g_MpPlayerNum].cols[colindex].rowstart;
		end = g_Menus[g_MpPlayerNum].cols[colindex].numrows + start;

		if (rowindex >= end) {
			rowindex = start;
		}

		if (rowindex < start) {
			rowindex = end - 1;
		}

		item = &dialog->definition->items[g_Menus[g_MpPlayerNum].rows[rowindex].itemindex];

		if (menuIsItemFocusable(item, dialog, updown)) {
			done = true;
		}

		if (rowindex == startrowindex) {
			done = true;
		}
	}

	dialog->focuseditem = item;
}

s32 dialogChangeItemFocusHorizontally(struct menudialog *dialog, s32 leftright)
{
	s32 rowindex;
	s32 colindex;
	bool done = false;
	s32 swipedir = 0;
	s32 y = dialogFindItem(dialog, dialog->focuseditem, &rowindex, &colindex);
	s32 startcolindex = colindex;
	struct menuitem *item;

	while (!done) {
		colindex += leftright;

		if (colindex >= dialog->colstart + dialog->numcols) {
			swipedir = 1;
			colindex = dialog->colstart;
		}

		if (colindex < dialog->colstart) {
			swipedir = -1;
			colindex = dialog->colstart + dialog->numcols - 1;
		}

		item = dialogFindItemAtColY(y, colindex, dialog->definition, &rowindex, dialog);

		if (item) {
			done = true;
		}

		if (colindex == startcolindex) {
			done = true;
		}
	}

	if (item) {
		dialog->focuseditem = item;
	}

	return swipedir;
}

s32 dialogChangeItemFocus(struct menudialog *dialog, s32 leftright, s32 updown)
{
	s32 swipedir = 0;

	if (leftright == 0 && updown == 0) {
		return 0;
	}

	if (updown != 0) {
		dialogChangeItemFocusVertically(dialog, updown);
	}

	if (leftright != 0) {
		swipedir = dialogChangeItemFocusHorizontally(dialog, leftright);
	}

	if (dialog->focuseditem != 0) {
		if (dialog->focuseditem->handler != NULL) {
			if ((dialog->focuseditem->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0) {
				union handlerdata data;
				dialog->focuseditem->handler(MENUOP_FOCUS, dialog->focuseditem, &data);
			}
		}
	}

	return swipedir;
}

void menuOpenDialog(struct menudialogdef *dialogdef, struct menudialog *dialog, struct menu *menu)
{
	union handlerdata data3;
	struct menuitem *item;
	union handlerdata data1;
	union handlerdata data2;

	dialog->definition = dialogdef;

	switch (g_MenuData.root) {
	case MENUROOT_MPSETUP:
	case MENUROOT_4MBMAINMENU:
		dialog->unk6e = 1;
		break;
	case MENUROOT_MAINMENU:
	case MENUROOT_TRAINING:
	default:
		dialog->unk6e = 0;
		break;
	}

	func0f0f1d6c(dialogdef, dialog, menu);
	dialogInitItems(dialog);

	dialog->type = dialogdef->type;
	dialog->transitionfrac = -1;
	dialog->redrawtimer = 0;
	dialog->unk4c = RANDOMFRAC() * M_TAU;

	g_Menus[g_MpPlayerNum].curdialog->state = MENUDIALOGSTATE_PREOPEN;
	g_Menus[g_MpPlayerNum].curdialog->statefrac = 0;

	dialog->unk54 = 0;
	dialog->unk58 = 0;
	dialog->unk5c = 0;

	dialog->focuseditem = dialogFindFirstItem(dialog);

	// Check if any items should be focused automatically
	item = dialog->definition->items;

	while (item->type != MENUITEMTYPE_END) {
		if (item->handler
				&& (item->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0
				&& item->handler(MENUOP_CHECKPREFOCUSED, item, &data1)) {
			dialog->focuseditem = item;
		}

		item++;
	}

	// Run focus handler
	if (dialog->focuseditem
			&& dialog->focuseditem->handler
			&& (dialog->focuseditem->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0) {
		dialog->focuseditem->handler(MENUOP_FOCUS, dialog->focuseditem, &data2);
	}

	dialog->dimmed = false;
	dialog->scroll = 0;
	dialog->dstscroll = 0;

	if (dialogdef->handler) {
		dialogdef->handler(MENUOP_OPEN, dialogdef, &data3);
	}

	dialogCalculateContentSize(dialogdef, dialog, menu);
	dialogCalculatePosition(dialog);

	dialog->x = dialog->dstx;
	dialog->y = dialog->dsty;
	dialog->width = dialog->dstwidth;
	dialog->height = dialog->dstheight;
}

void menuPushDialog(struct menudialogdef *dialogdef)
{
	if (dialogdef) {
		func0f0f37a4(&g_Menus[g_MpPlayerNum].unk840);

		if (g_Menus[g_MpPlayerNum].depth < 6 && g_Menus[g_MpPlayerNum].numdialogs < ARRAYCOUNT(g_Menus[0].dialogs)) {
			struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth];
			struct menudialogdef *sibling;
			struct menudialog *dialog;

			g_Menus[g_MpPlayerNum].depth++;

			layer->numsiblings = 1;
			layer->cursibling = 0;

			dialog = &g_Menus[g_MpPlayerNum].dialogs[g_Menus[g_MpPlayerNum].numdialogs];
			g_Menus[g_MpPlayerNum].numdialogs++;
			layer->siblings[0] = dialog;
			g_Menus[g_MpPlayerNum].curdialog = dialog;
			dialog->swipedir = 0;

			menuOpenDialog(dialogdef, dialog, &g_Menus[g_MpPlayerNum]);

			dialog->dstx = (viGetWidth() - dialog->width) / 2;
			dialog->dsty = (viGetHeight() - dialog->height) / 2;

			g_Menus[g_MpPlayerNum].fm.unke40_00 = true;
			sibling = dialogdef->nextsibling;

			while (sibling && layer->numsiblings < 5) {
				// If this limit were to be reached, the game would soft lock
				// because sibling is incremented inside the if-statement block.
				if (g_Menus[g_MpPlayerNum].numdialogs < ARRAYCOUNT(g_Menus[0].dialogs)) {
					dialog = &g_Menus[g_MpPlayerNum].dialogs[g_Menus[g_MpPlayerNum].numdialogs];
					g_Menus[g_MpPlayerNum].numdialogs++;

					layer->siblings[layer->numsiblings] = dialog;
					layer->numsiblings++;

					dialog->swipedir = -1;

					menuOpenDialog(sibling, dialog, &g_Menus[g_MpPlayerNum]);

					dialog->dstx = dialog->x = -320;
					dialog->dsty = dialog->y = (viGetHeight() - dialog->height) / 2;
					dialog->type = 0;

					sibling = sibling->nextsibling;
				}
			}

			if (sibling);

			menuPlaySound(MENUSOUND_OPENDIALOG);

			if (dialogdef->type == MENUDIALOGTYPE_DANGER) {
				menuPlaySound(MENUSOUND_ERROR);
			}

			if (dialogdef->type == MENUDIALOGTYPE_SUCCESS) {
				menuPlaySound(MENUSOUND_SUCCESS);
			}
		}
	}
}

#if VERSION >= VERSION_NTSC_1_0
bool func0f0f3220(s32 arg0)
{
	bool save = true;
	s32 i;

	if (g_MenuData.unk669[arg0] == 4) {
		s32 prevplayernum = g_MpPlayerNum;

		for (i = 3; i >= 0; i--) {
			if (g_Menus[i].curdialog) {
				g_MpPlayerNum = i;
			}
		}

		if (g_Menus[g_MpPlayerNum].depth >= 2) {
			save = false;
		}

		if (g_Menus[g_MpPlayerNum].curdialog
				&& g_Menus[g_MpPlayerNum].curdialog->definition->type == MENUDIALOGTYPE_DANGER) {
			save = false;

			if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_MpEndscreenChallengeCheatedMenuDialog
					|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_MpEndscreenChallengeFailedMenuDialog) {
				save = true;
			}
		}

		if (save) {
			filemgrSaveOrLoad(&g_GameFileGuid, FILEOP_SAVE_GAME_000, 0);
		}

		g_MpPlayerNum = prevplayernum;
	} else if (g_MenuData.unk669[arg0] < 4) {
		s32 prevplayernum = g_MpPlayerNum;
		g_MpPlayerNum = g_MenuData.unk669[arg0];
		filemgrSaveOrLoad(&g_PlayerConfigsArray[g_MpPlayerNum].fileguid, FILEOP_SAVE_MPPLAYER, g_MpPlayerNum);
		save = true;
		g_MpPlayerNum = prevplayernum;
	}

	if (save) {
		g_MenuData.unk66e--;
	}

	return save;
}
#else
void func0f0f3220(s32 arg0)
{
	s32 i;

	if (g_MenuData.unk669[arg0] == 4) {
		s32 prevplayernum = g_MpPlayerNum;

		for (i = 3; i >= 0; i--) {
			if (g_Menus[i].curdialog) {
				g_MpPlayerNum = i;
			}
		}

		filemgrSaveOrLoad(&g_GameFileGuid, FILEOP_SAVE_GAME_000, 0);

		g_MpPlayerNum = prevplayernum;
	} else if (g_MenuData.unk669[arg0] < 4) {
		s32 prevplayernum = g_MpPlayerNum;
		g_MpPlayerNum = g_MenuData.unk669[arg0];
		filemgrSaveOrLoad(&g_PlayerConfigsArray[g_MpPlayerNum].fileguid, FILEOP_SAVE_MPPLAYER, g_MpPlayerNum);
		g_MpPlayerNum = prevplayernum;
	}

	g_MenuData.unk66e--;
}
#endif

void menuCloseDialog(void)
{
	if (g_Menus[g_MpPlayerNum].depth > 0) {
		union handlerdata data;
		struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth - 1];
		u32 value_prevent = 1; // constant value, but required for match
		s32 i;

		for (i = 0; i < layer->numsiblings; i++) {
			data.dialog1.preventclose = false;

			if (layer->siblings[i]->definition->handler) {
				layer->siblings[i]->definition->handler(MENUOP_CLOSE, layer->siblings[i]->definition, &data);
			}

			if (value_prevent == data.dialog1.preventclose) {
				return;
			}
		}

		for (i = 0; i < layer->numsiblings; i++) {
			g_Menus[g_MpPlayerNum].numdialogs--;
		}

		g_Menus[g_MpPlayerNum].rowend = g_Menus[g_MpPlayerNum].cols[layer->siblings[0]->colstart].rowstart;
		g_Menus[g_MpPlayerNum].colend = layer->siblings[0]->colstart;
		g_Menus[g_MpPlayerNum].blockend = layer->siblings[0]->blockstart;
		g_Menus[g_MpPlayerNum].depth--;

		menuPlaySound(MENUSOUND_0B);
	}

#if VERSION >= VERSION_NTSC_1_0
	if (g_MenuData.unk66e > 0 && g_Menus[g_MpPlayerNum].depth == 0)
#else
	if (g_MenuData.unk66e > 0)
#endif
	{
		s32 value = g_MenuData.unk66e;

		while (value >= 0) {
			func0f0f3220(value);
			value--;
		}
	}

	if (g_Menus[g_MpPlayerNum].depth == 0) {
		g_Menus[g_MpPlayerNum].curdialog = NULL;
	} else {
		struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth - 1];
		g_Menus[g_MpPlayerNum].curdialog = layer->siblings[layer->cursibling];
	}
}

void menuUpdateCurFrame(void)
{
	s32 depth = g_Menus[g_MpPlayerNum].depth;

	if (depth == 0) {
		// No more parent menus - return control to the player
		g_Vars.currentplayer->joybutinhibit = 0xffffffff;
		menuClose();
		g_Menus[g_MpPlayerNum].curdialog = NULL;
	} else {
		// Set up parent menu
		struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[depth - 1];
		g_Menus[g_MpPlayerNum].curdialog = layer->siblings[layer->cursibling];
	}
}

void menuPopDialog(void)
{
	menuCloseDialog();
	menuUpdateCurFrame();
}

void func0f0f3704(struct menudialogdef *dialogdef)
{
	menuCloseDialog();
	menuPushDialog(dialogdef);
}

void func0f0f372c(struct menu840 *arg0, f32 x, f32 y, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, u8 flags)
{
	arg0->unk5b1_05 = true;

	if (flags & 2) {
		arg0->unk538 = x;
		arg0->unk53c = y;
		arg0->unk540 = arg3;
	}

	if (flags & 4) {
		arg0->unk548 = arg4;
		arg0->unk54c = arg5;
		arg0->unk550 = arg6;
	}

	if (flags & 1) {
		arg0->unk544 = arg7;
	}

	arg0->unk568 = flags;
	arg0->unk564 = 0.0f;
}

void func0f0f37a4(struct menu840 *arg0)
{
	if (arg0->unk010 == 0x4fac5ace) {
		challengeUnsetCurrent();
	}

	arg0->unk000 = 0;
	arg0->unk00c = 0;
	arg0->unk010 = 0;
	arg0->bodymodeldef = 0;
	arg0->headmodeldef = 0;
	arg0->unk05c = 0;
	arg0->unk05e = 0;
	arg0->unk5b0 = 0;
	arg0->unk5b1_01 = false;
	arg0->unk5b1_02 = false;
	arg0->unk5b1_03 = false;
	arg0->unk5b1_04 = false;
	arg0->unk5b1_05 = false;
	arg0->unk5b1_06 = false;
	arg0->unk5b1_07 = false;
	arg0->partvisibility = NULL;
	arg0->unk560 = -1;
	arg0->headnum = -1;
	arg0->bodynum = -1;
	arg0->unk550 = 0.0f;
	arg0->unk54c = 0.0f;
	arg0->unk548 = 0.0f;
	arg0->unk540 = 0.0f;
	arg0->unk53c = 0.0f;
	arg0->unk538 = 0.0f;
	arg0->unk534 = 0.0f;
	arg0->unk530 = 0.0f;
	arg0->unk52c = 0.0f;
	arg0->unk528 = 0.0f;
	arg0->unk524 = 0.0f;
	arg0->unk520 = 0.0f;
	arg0->unk518 = 0.0f;
	arg0->unk514 = 0.0f;
	arg0->unk510 = 0.0f;
	arg0->unk558 = 0.0f;
	arg0->unk55c = 1.0f;
	arg0->unk51c = 1.0f;
	arg0->unk544 = 1.0f;
	arg0->unk554 = -1.0f;
}

u32 var80071468 = 0x96969600;
u32 var8007146c = 0x96969600;

u32 var80071470 = 0xffffff00;
u32 var80071474 = 0xffffff00;
u32 var80071478 = 0xb24d2e00;
u32 var8007147c = 0x00000000;

#if VERSION >= VERSION_NTSC_1_0
/**
 * Render the hudpiece as well as any models within dialogs.
 */
Gfx *menuRenderModels(Gfx *gdl, struct menu840 *thing, s32 arg2)
{
	f32 a;
	f32 b;
	f32 c;
	f32 sp430;
	f32 sp42c;
	f32 sp428;
	f32 sp424;
	s32 totalfilelen;
	struct texturething sp410;
	s32 bodyfilelen2;
	u16 bodyfilenum;
	u16 headfilenum;
	s32 bodynum;
	s32 headnum;

	if (g_Vars.stagenum != STAGE_CITRAINING && g_Vars.stagenum != STAGE_CREDITS) {
		if (g_MenuData.unk5d5_01 && arg2 != 1 && arg2 < 3) {
			return gdl;
		}

		if (thing->unk004 == NULL) {
			if (bgun0f09e004(1)) {
				thing->unk004 = bgunGetGunMem();
				thing->unk008 = bgunCalculateGunMemCapacity();
			} else {
				return gdl;
			}
		}
	}

	if (thing->unk004 == NULL) {
		return gdl;
	}

	if (thing->unk00c != 0) {
		if (thing->unk00c == thing->unk010) {
			thing->unk00c = 0;
			thing->unk000 = 0;
		} else {
			if (thing->unk010 == 0x4fac5ace) {
				challengeUnsetCurrent();
			}

			if (thing->unk000 == 0) {
				thing->unk000 = 1;
				return gdl;
			}

			thing->unk000--;

			if (thing->unk000 == 0) {
				if ((thing->unk00c & 0xffff) == 0xffff || (thing->unk00c & 0x80000000)) {
					if (thing->unk00c & 0x80000000) {
						headnum = thing->unk00c & 0x3ff;
						bodynum = (thing->unk00c & 0xffc00) >> 10;
					} else {
						s32 mpheadnum = (thing->unk00c >> 16) & 0xff;
						s32 mpbodynum = (thing->unk00c >> 24) & 0xff;
						bodynum = mpGetBodyId(mpbodynum);

						if (mpheadnum < mpGetNumHeads2()) {
							headnum = mpGetHeadId(mpheadnum);
						} else {
							headnum = func0f14a9f8(mpheadnum - mpGetNumHeads2());
							headnum = mpGetBeauHeadId(headnum);
							thing->unk5b0 = (mpheadnum - mpGetNumHeads2()) & 0xff;
						}
					}

					bodyfilenum = g_HeadsAndBodies[bodynum].filenum;

					totalfilelen = fileGetInflatedSize(bodyfilenum);
					totalfilelen = ALIGN64(totalfilelen);

					if (g_HeadsAndBodies[bodynum].unk00_01) {
						headnum = -1;
						headfilenum = 0xffff;
					} else {
						headfilenum = g_HeadsAndBodies[headnum].filenum;
						totalfilelen += ALIGN64(fileGetInflatedSize(headfilenum));
					}

					totalfilelen += 0x4000;

					tex0f172e70(&sp410, thing->unk004 + totalfilelen, thing->unk008 - totalfilelen);

					thing->headnum = headnum;
					thing->bodynum = bodynum;
					thing->bodymodeldef = modeldefLoad(bodyfilenum, thing->unk004, totalfilelen, &sp410);
					bodyfilelen2 = ALIGN64(fileGetLoadedSize(bodyfilenum));
					modelCalculateRwDataLen(thing->bodymodeldef);

					if (headnum < 0) {
						thing->headmodeldef = NULL;
					} else {
						thing->headmodeldef = modeldefLoad(headfilenum, thing->unk004 + bodyfilelen2, totalfilelen - bodyfilelen2, &sp410);
						fileGetLoadedSize(headfilenum);
						bodyCalculateHeadOffset(thing->headmodeldef, headnum, bodynum);
						modelCalculateRwDataLen(thing->headmodeldef);
					}

					modelInit(&thing->bodymodel, thing->bodymodeldef, &thing->unk110, true);
					animInit(&thing->bodyanim);

					thing->bodymodel.unk02 = 256;
					thing->bodymodel.anim = &thing->bodyanim;

					body0f02ce8c(bodynum, headnum, thing->bodymodeldef, thing->headmodeldef, totalfilelen * 0, &thing->bodymodel, false, 1);
				} else {
					totalfilelen = ALIGN64(fileGetInflatedSize(thing->unk00c)) + 0x4000;
					if (1);

					tex0f172e70(&sp410, &thing->unk004[(u32)totalfilelen], thing->unk008 - totalfilelen);

					thing->headnum = -1;
					thing->bodynum = -1;
					thing->bodymodeldef = modeldefLoad(thing->unk00c, thing->unk004, totalfilelen, &sp410);

					fileGetLoadedSize(thing->unk00c);
					modelCalculateRwDataLen(thing->bodymodeldef);
					modelInit(&thing->bodymodel, thing->bodymodeldef, &thing->unk110, true);
					animInit(&thing->bodyanim);

					thing->bodymodel.unk02 = 256;
					thing->bodymodel.anim = &thing->bodyanim;
				}

				thing->unk010 = thing->unk00c;
				thing->unk05e = 0;
				thing->unk00c = 0;
			} else {
				return gdl;
			}
		}
	}

	if (thing->bodymodeldef != NULL) {
		struct modelrenderdata renderdata = {NULL, true, 3};
		Mtxf *sp3b4;
		s32 i;
		u32 stack[3];
		struct coord sp398;
		f32 sp390[2];
		Mtxf sp350;
		Mtxf sp310;
		f32 sp30c[1];
		bool sp308;
		struct coord sp2fc;
		f32 sp2f8;
		bool sp2f4;

		if (arg2 < 3 && g_MenuData.unk5d5_03) {
			gdl = vi0000b280(gdl);
			gdl = vi0000b1d0(gdl);

			g_MenuData.unk5d5_03 = false;

			if (arg2 != 2) {
				gdl = menuApplyScissor(gdl);
			}

			gSPSetGeometryMode(gdl++, G_ZBUFFER);
		}

		gSPDisplayList(gdl++, var80061380);
		gSPDisplayList(gdl++, var800613a0);

		sp308 = false;

		if (thing->unk554 > 0.0f) {
			sp2f4 = true;

			if (thing->bodymodeldef->skel == &g_SkelChr) {
				struct modelnode *node = modelGetPart(thing->bodymodeldef, MODELPART_CHR_0006);

				if (node) {
					struct modelrodata_position *rodata = &node->rodata->position;
					f32 f0 = func0f006bd0(thing->unk574 / TICKS(480.0f));

					sp2fc.f[0] = 0.0f;
					sp2fc.f[1] = 0.0f - (rodata->pos.f[1] / 7.6f * (1.0f - f0 * f0));
					sp2fc.f[2] = 0.0f;

					sp308 = true;

					thing->unk554 = 100.0f + (1.0f - f0) * 270.0f;
					sp2f8 = thing->unk554 / (rodata->pos.f[1] / 2.0f);

					sp2f4 = false;

					modelFindBboxRodata(&thing->bodymodel);
				}

				if (1);
			}

			if (sp2f4) {
				struct modelrodata_bbox *bbox = modelFindBboxRodata(&thing->bodymodel);

				if (bbox) {
					sp2fc.x = -(bbox->xmax - ((bbox->xmax - bbox->xmin) * 0.5f));
					sp2fc.y = -(bbox->ymax - ((bbox->ymax - bbox->ymin) * 0.5f));
					sp2fc.z = -(bbox->zmax - ((bbox->zmax - bbox->zmin) * 0.5f));
					sp308 = true;
					sp2f8 = thing->unk554 / ((bbox->ymax - bbox->ymin) * 0.5f);
				}
			}
		}

		mtx4LoadIdentity(&sp350);

		if (arg2 == 1) {
			if (IS8MB()) {
				s32 i;

				if (thing->unk510 != thing->unk538) {
					for (i = 0; i < g_Vars.diffframe60; i++) {
						thing->unk510 = (thing->unk538 * PALUPF(0.002f)) + ((1.0f - PALUPF(0.002f)) * thing->unk510);
					}
				}

				if (thing->unk514 != thing->unk53c) {
					for (i = 0; i < g_Vars.diffframe60; i++) {
						thing->unk514 = (thing->unk53c * PALUPF(0.002f)) + ((1.0f - PALUPF(0.002f)) * thing->unk514);
					}
				}

				if (thing->unk518 != thing->unk540) {
					for (i = 0; i < g_Vars.diffframe60; i++) {
						thing->unk518 = (thing->unk540 * PALUPF(0.002f)) + ((1.0f - PALUPF(0.002f)) * thing->unk518);
					}
				}

				if (thing->unk51c != thing->unk544) {
					for (i = 0; i < g_Vars.diffframe60; i++) {
						thing->unk51c = (thing->unk544 * PALUPF(0.002f)) + ((1.0f - PALUPF(0.002f)) * thing->unk51c);
					}
				}

				sp430 = thing->unk510;

#if !PAL
				if (g_ViRes == VIRES_HI) {
					sp430 *= 2.0f;
				}
#endif

				sp42c = thing->unk514;
				sp428 = thing->unk518;
				sp424 = thing->unk51c;

				a = thing->unk548;
				b = thing->unk54c;
				c = thing->unk550;

				thing->unk520 = a;
				thing->unk524 = b;
				thing->unk528 = c;

				sp398.x = a;
				sp398.y = b;
				sp398.z = c;

				mtx4LoadRotation(&sp398, &sp350);
			}
		} else {
			if (thing->unk5b1_05) {
#if VERSION >= VERSION_PAL_BETA
				thing->unk564 += g_Vars.diffframe60freal / 40.0f;
#else
				thing->unk564 += g_Vars.diffframe60f / 40.0f;
#endif

				if (thing->unk564 > 1.0f) {
					thing->unk5b1_05 = false;
					thing->unk510 = thing->unk538;
					thing->unk514 = thing->unk53c;
					thing->unk518 = thing->unk540;
					thing->unk51c = thing->unk544;
				} else {
					f32 sp2d0 = (-cosf(thing->unk564 * M_PI) * 0.5f) + 0.5f;
					f32 sp2cc = 1.0f - sp2d0;

					if (thing->unk568 & 2) {
						sp430 = (thing->unk510 * sp2cc) + (sp2d0 * thing->unk538);
						sp42c = (thing->unk514 * sp2cc) + (sp2d0 * thing->unk53c);
						sp428 = (thing->unk518 * sp2cc) + (sp2d0 * thing->unk540);
					} else {
						sp430 = thing->unk510 = thing->unk538;
						sp42c = thing->unk514 = thing->unk53c;
						sp428 = thing->unk518 = thing->unk540;
					}

					if (thing->unk568 & 1) {
						sp424 = (thing->unk51c * sp2cc) + (sp2d0 * thing->unk544);
					} else {
						sp424 = thing->unk51c = thing->unk544;
					}

					if (thing->unk568 & 4) {
						f32 sp2bc[4];
						f32 sp2ac[4];
						f32 sp29c[4];
						struct coord sp290;

						sp290.x = thing->unk520;
						sp290.y = thing->unk524;
						sp290.z = thing->unk528;

						quaternion0f096ca0(&sp290, sp2bc);

						sp290.x = thing->unk548;
						sp290.y = thing->unk54c;
						sp290.z = thing->unk550;

						quaternion0f096ca0(&sp290, sp2ac);
						quaternionSlerp(sp2bc, sp2ac, sp2d0, sp29c);
						quaternionToMtx(sp29c, &sp350);
					} else {
						a = thing->unk548;
						b = thing->unk54c;
						c = thing->unk550;

						thing->unk520 = a;
						thing->unk524 = b;
						thing->unk528 = c;

						sp398.x = a;
						sp398.y = b;
						sp398.z = c;

						mtx4LoadRotation(&sp398, &sp350);
					}
				}
			}

			if (!thing->unk5b1_05) {
				sp430 = thing->unk510 = thing->unk538;
				sp42c = thing->unk514 = thing->unk53c;
				sp428 = thing->unk518 = thing->unk540;
				sp424 = thing->unk51c = thing->unk544;

				if (1);
				a = thing->unk548;
				b = thing->unk54c;
				c = thing->unk550;

				thing->unk520 = a;
				thing->unk524 = b;
				thing->unk528 = c;

				sp398.x = a;
				sp398.y = b;
				sp398.z = c;

				mtx4LoadRotation(&sp398, &sp350);
			}
		}

		sp30c[0] = -100.0f + sp428;

		if (arg2 == 1) {
			if (IS8MB()) {
				sp390[0] = thing->unk510 * g_ScaleX;
				sp390[1] = thing->unk514;
			}
		} else {
			sp390[0] = sp430 * g_ScaleX + viGetViewLeft() + viGetViewWidth() * 0.5f;
			sp390[1] = sp42c + viGetViewTop() + viGetViewHeight() * 0.5f;
		}

		cam0f0b4c3c(sp390, &sp398, 1.0f);
		mtx4LoadIdentity(&sp310);

		if (thing->partvisibility != NULL) {
			struct modelpartvisibility *ptr = thing->partvisibility;

			while (ptr->part != 255) {
				struct modelnode *node = modelGetPart(thing->bodymodeldef, ptr->part);

				if (node) {
					union modelrwdata *rwdata = modelGetNodeRwData(&thing->bodymodel, node);

					if (rwdata) {
						if (ptr->visible) {
							rwdata->toggle.visible = true;
						} else {
							rwdata->toggle.visible = false;
						}
					}
				}

				ptr++;
			}
		}

		if (arg2 == 3) {
			sp398.x = thing->unk510;
			sp398.y = thing->unk514;
			sp398.z = thing->unk518;
		} else {
			c = sp30c[0] / sp398.z;
			sp398.x = c * sp398.x;
			sp398.y = c * sp398.y;
			sp398.z = c * sp398.z;
		}

#if VERSION < VERSION_NTSC_1_0
		if (thing->unk010 < 0) {
			struct coord newpos = {0, 0, 0};
			struct coord oldpos;

			model0001b3bc(&thing->bodymodel);

			modelGetRootPosition(&thing->bodymodel, &oldpos);

			if (joyGetButtons(0, L_TRIG)) {
				modelSetRootPosition(&thing->bodymodel, &newpos);
			}
		}
#endif

		mtx4LoadTranslation(&sp398, &sp310);

		if (sp308) {
			mtx00015f04(sp424 * sp2f8, &sp310);
		} else {
			mtx00015f04(sp424, &sp310);
		}

		{
			Mtxf sp244;
			Mtxf sp204;
			Mtxf sp1c4;
			Mtxf sp184;

			if (sp308) {
				mtx4LoadTranslation(&sp2fc, &sp204);
			} else {
				sp398.x = thing->unk52c;
				sp398.y = thing->unk530;
				sp398.z = thing->unk534;
				mtx4LoadTranslation(&sp398, &sp204);
			}

			mtx4MultMtx4(&sp310, &sp350, &sp244);

			if (arg2 == 3) {
				credits0f13ae04(&sp1c4);
				mtx4MultMtx4(&sp1c4, &sp244, &sp184);
				mtx4MultMtx4(&sp184, &sp204, &thing->unk014);
			} else {
				mtx4MultMtx4(&sp244, &sp204, &thing->unk014);
			}
		}

		gdl = menugfx0f0e2348(gdl);

		if (arg2 < 3) {
			if (arg2 != 0) {
				gdl = func0f0d49c8(gdl);
				gSPMatrix(gdl++, osVirtualToPhysical(camGetPerspectiveMtxL()), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
			} else {
				f32 aspect = (f32) (g_MenuScissorX2 - g_MenuScissorX1) / (f32) (g_MenuScissorY2 - g_MenuScissorY1);
				static u32 znear = 10;
				static u32 zfar = 300;

				mainOverrideVariable("mzn", &znear);
				mainOverrideVariable("mzf", &zfar);

				gdl = func0f0d49c8(gdl);

				viSetViewPosition(g_MenuScissorX1 * g_ScaleX, g_MenuScissorY1);
				viSetFovAspectAndSize(g_Vars.currentplayer->fovy, aspect, (g_MenuScissorX2 - g_MenuScissorX1) * g_ScaleX, g_MenuScissorY2 - g_MenuScissorY1);

				gdl = vi0000af00(gdl, var800a2048[g_MpPlayerNum]);
				gdl = vi0000aca4(gdl, znear, zfar);
			}
		}

		sp3b4 = gfxAllocate(thing->bodymodeldef->nummatrices * sizeof(Mtxf));

		for (i = 0; i < thing->bodymodeldef->nummatrices; i++) {
			mtx4LoadIdentity(&sp3b4[i]);
		}

		thing->bodymodel.matrices = sp3b4;

		if (thing->unk05c && thing->unk05e != thing->unk05c) {
			if (thing->unk5b1_04) {
				modelSetAnimation(&thing->bodymodel, thing->unk05c, false, 0, PALUPF(-0.5f), 0.0f);
				model0001e018(&thing->bodymodel, modelGetNumAnimFrames(&thing->bodymodel));
			} else {
				modelSetAnimation(&thing->bodymodel, thing->unk05c, false, 0, PALUPF(0.5f), 0.0f);
			}

			thing->unk05e = thing->unk05c;
		}

		thing->unk05c = 0;

		if (thing->unk05e != 0) {
			f32 sp178;
			u32 stack;

			model0001ee18(&thing->bodymodel, g_Vars.diffframe240, true);

			if (thing->unk5b1_04) {
				sp178 = modelGetNumAnimFrames(&thing->bodymodel) - modelGetCurAnimFrame(&thing->bodymodel);
			} else {
				sp178 = modelGetCurAnimFrame(&thing->bodymodel);
			}

			if (sp178 >= modelGetNumAnimFrames(&thing->bodymodel) - 1) {
				thing->unk05e = 0;
			}
		}

		mtx4Copy(&thing->unk014, sp3b4);

		renderdata.unk00 = &thing->unk014;
		renderdata.unk10 = thing->bodymodel.matrices;

		model0001cebc(&renderdata, &thing->bodymodel);

		if (thing->bodymodeldef->skel == &g_SkelHudPiece) {
			struct modelnode *node = modelGetPart(thing->bodymodeldef, MODELPART_HUDPIECE_0000);

			if (node) {
				struct modelrodata_gundl *rodata = &node->rodata->gundl;
				s32 i;
				s32 j;

				for (i = 0; i < rodata->numvertices; i++) {
#if VERSION >= VERSION_PAL_BETA
					rodata->vertices[i].s -= (s32) (100.0f * g_Vars.diffframe60freal);
#else
					rodata->vertices[i].s -= 100 * g_Vars.diffframe60;
#endif

					if (rodata->vertices[i].s < -0x6000) {
						for (j = 0; j < rodata->numvertices; j++) {
							rodata->vertices[j].s += 0x2000;
						}
					}
				}
			}

			node = modelGetPart(thing->bodymodeldef, MODELPART_HUDPIECE_0002);

			if (node) {
				s32 sp160;
				Mtxf sp120;
				Mtxf spe0;
				sp160 = model0001a524(node, 0);
				mtx4LoadIdentity(&sp120);
				mtx4LoadXRotation(func0f006b54(4), &sp120);
				mtx4MultMtx4((Mtxf *)((u32)sp3b4 + sp160 * sizeof(Mtxf)), &sp120, &spe0);
				mtx4Copy(&spe0, (Mtxf *)((u32)sp3b4 + sp160 * sizeof(Mtxf)));
			}

			node = modelGetPart(thing->bodymodeldef, MODELPART_HUDPIECE_0001);

			if (node) {
				if (g_MenuData.root == MENUROOT_MAINMENU
						|| g_MenuData.root == MENUROOT_FILEMGR
						|| g_MenuData.root == MENUROOT_MPSETUP
						|| g_MenuData.root == MENUROOT_TRAINING) {
					s32 index = model0001a524(node, 0);
					struct coord spd0;
					f32 spc8[2];

					spd0.x = sp3b4[index].m[3][0];
					spd0.y = sp3b4[index].m[3][1];
					spd0.z = sp3b4[index].m[3][2];

					cam0f0b4d04(&spd0, spc8);
					var8009de98 = ((s32)spc8[0] - viGetWidth() / 2) / g_ScaleX;
					var8009de9c = (s32)spc8[1] - viGetHeight() / 2;
				}
			}
		}

		gSPNumLights(gdl++, 1);
		gSPLight(gdl++, &var80071470, 1);
		gSPLight(gdl++, &var80071468, 2);
		gSPLookAtX(gdl++, &camGetLookAt()->l[0]);
		gSPLookAtY(gdl++, &camGetLookAt()->l[1]);

		renderdata.unk30 = 1;
		renderdata.envcolour = 0xffffffff;
		renderdata.fogcolour = 0xffffffff;

		gSPSetGeometryMode(gdl++, G_ZBUFFER);

		renderdata.gdl = gdl;
		renderdata.zbufferenabled = true;
		modelRender(&renderdata, &thing->bodymodel);
		gdl = renderdata.gdl;

		mtx00016760();

		for (i = 0; i < thing->bodymodeldef->nummatrices; i++) {
			Mtxf sp70;
			mtx4Copy((Mtxf *)((u32)thing->bodymodel.matrices + i * sizeof(Mtxf)), &sp70);
			mtx00016054(&sp70, &thing->bodymodel.matrices[i]);
		}

		mtx00016784();

		if (arg2 < 3) {
			gdl = func0f0d479c(gdl);
		}

		gDPPipeSync(gdl++);
		gDPSetCycleType(gdl++, G_CYC_1CYCLE);
		gDPSetAlphaCompare(gdl++, G_AC_NONE);
		gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
		gSPClearGeometryMode(gdl++, G_CULL_BOTH);
		gDPSetTextureFilter(gdl++, G_TF_BILERP);

		texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

		gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

		texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

		gSPDisplayList(gdl++, var800613a0);
	}

	return gdl;
}
#else
u32 var80071480 = 0x00000000;
u32 var80071484 = 0x00000001;
u32 var80071488 = 0x00000003;
u32 var8007148c = 0x00000000;
u32 var80071490 = 0x00000000;
u32 var80071494 = 0x00000000;
u32 var80071498 = 0x00000000;
u32 var8007149c = 0x00000000;
u32 var800714a0 = 0x00000000;
u32 var800714a4 = 0x00000000;
u32 var800714a8 = 0x00000000;
u32 var800714ac = 0x00000000;
u32 var800714b0 = 0x00000000;
u32 var800714b4 = 0x00000000;
u32 var800714b8 = 0x00000000;
u32 var800714bc = 0x00000000;

u32 var80073b6cnb[3] = {0};

u32 var800714c0 = 0x0000000a;
u32 var800714c4 = 0x0000012c;

const char var7f1b25f0[] = "mzn";
const char var7f1b25f4[] = "mzf";

GLOBAL_ASM(
glabel menuRenderModels
.late_rodata
glabel var7f1b292c
.word 0x40f33333
glabel var7f1b2930
.word 0x3b03126f
glabel var7f1b2934
.word 0x3f7f7cee
glabel var7f1b2938
.word 0x3f7f7cee
glabel var7f1b293c
.word 0x3b03126f
glabel var7f1b2940
.word 0x3b03126f
glabel var7f1b2944
.word 0x3b03126f
glabel var7f1b2948
.word 0x40490fdb
.text
/*  f0f03f4:	27bdfb98 */ 	addiu	$sp,$sp,-1128
/*  f0f03f8:	3c02800a */ 	lui	$v0,0x800a
/*  f0f03fc:	8c42eb74 */ 	lw	$v0,-0x148c($v0)
/*  f0f0400:	afb10028 */ 	sw	$s1,0x28($sp)
/*  f0f0404:	24010026 */ 	addiu	$at,$zero,0x26
/*  f0f0408:	00a08825 */ 	or	$s1,$a1,$zero
/*  f0f040c:	afbf002c */ 	sw	$ra,0x2c($sp)
/*  f0f0410:	afb00024 */ 	sw	$s0,0x24($sp)
/*  f0f0414:	afa40468 */ 	sw	$a0,0x468($sp)
/*  f0f0418:	1041001d */ 	beq	$v0,$at,.NB0f0f0490
/*  f0f041c:	afa60470 */ 	sw	$a2,0x470($sp)
/*  f0f0420:	2401005c */ 	addiu	$at,$zero,0x5c
/*  f0f0424:	1041001a */ 	beq	$v0,$at,.NB0f0f0490
/*  f0f0428:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f042c:	81cf6485 */ 	lb	$t7,0x6485($t6)
/*  f0f0430:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0434:	05e30008 */ 	bgezl	$t7,.NB0f0f0458
/*  f0f0438:	8e390004 */ 	lw	$t9,0x4($s1)
/*  f0f043c:	10c10005 */ 	beq	$a2,$at,.NB0f0f0454
/*  f0f0440:	28c10003 */ 	slti	$at,$a2,0x3
/*  f0f0444:	50200004 */ 	beqzl	$at,.NB0f0f0458
/*  f0f0448:	8e390004 */ 	lw	$t9,0x4($s1)
/*  f0f044c:	100005d1 */ 	beqz	$zero,.NB0f0f1b94
/*  f0f0450:	00801025 */ 	or	$v0,$a0,$zero
.NB0f0f0454:
/*  f0f0454:	8e390004 */ 	lw	$t9,0x4($s1)
.NB0f0f0458:
/*  f0f0458:	5720000e */ 	bnezl	$t9,.NB0f0f0494
/*  f0f045c:	8e2a0004 */ 	lw	$t2,0x4($s1)
/*  f0f0460:	0fc26fc8 */ 	jal	bgun0f09e004
/*  f0f0464:	24040001 */ 	addiu	$a0,$zero,0x1
/*  f0f0468:	10400007 */ 	beqz	$v0,.NB0f0f0488
/*  f0f046c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0470:	0fc26f42 */ 	jal	bgunGetGunMem
/*  f0f0474:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0478:	0fc26f46 */ 	jal	bgunCalculateGunMemCapacity
/*  f0f047c:	ae220004 */ 	sw	$v0,0x4($s1)
/*  f0f0480:	10000003 */ 	beqz	$zero,.NB0f0f0490
/*  f0f0484:	ae220008 */ 	sw	$v0,0x8($s1)
.NB0f0f0488:
/*  f0f0488:	100005c2 */ 	beqz	$zero,.NB0f0f1b94
/*  f0f048c:	8fa20468 */ 	lw	$v0,0x468($sp)
.NB0f0f0490:
/*  f0f0490:	8e2a0004 */ 	lw	$t2,0x4($s1)
.NB0f0f0494:
/*  f0f0494:	55400004 */ 	bnezl	$t2,.NB0f0f04a8
/*  f0f0498:	8e22000c */ 	lw	$v0,0xc($s1)
/*  f0f049c:	100005bd */ 	beqz	$zero,.NB0f0f1b94
/*  f0f04a0:	8fa20468 */ 	lw	$v0,0x468($sp)
/*  f0f04a4:	8e22000c */ 	lw	$v0,0xc($s1)
.NB0f0f04a8:
/*  f0f04a8:	504000dc */ 	beqzl	$v0,.NB0f0f081c
/*  f0f04ac:	8e2a0054 */ 	lw	$t2,0x54($s1)
/*  f0f04b0:	8e230010 */ 	lw	$v1,0x10($s1)
/*  f0f04b4:	3c014fac */ 	lui	$at,0x4fac
/*  f0f04b8:	34215ace */ 	ori	$at,$at,0x5ace
/*  f0f04bc:	14620004 */ 	bne	$v1,$v0,.NB0f0f04d0
/*  f0f04c0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f04c4:	ae20000c */ 	sw	$zero,0xc($s1)
/*  f0f04c8:	100000d3 */ 	beqz	$zero,.NB0f0f0818
/*  f0f04cc:	a2200000 */ 	sb	$zero,0x0($s1)
.NB0f0f04d0:
/*  f0f04d0:	54610004 */ 	bnel	$v1,$at,.NB0f0f04e4
/*  f0f04d4:	92220000 */ 	lbu	$v0,0x0($s1)
/*  f0f04d8:	0fc658b5 */ 	jal	challengeUnsetCurrent
/*  f0f04dc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f04e0:	92220000 */ 	lbu	$v0,0x0($s1)
.NB0f0f04e4:
/*  f0f04e4:	240b0001 */ 	addiu	$t3,$zero,0x1
/*  f0f04e8:	14400004 */ 	bnez	$v0,.NB0f0f04fc
/*  f0f04ec:	244cffff */ 	addiu	$t4,$v0,-1
/*  f0f04f0:	a22b0000 */ 	sb	$t3,0x0($s1)
/*  f0f04f4:	100005a7 */ 	beqz	$zero,.NB0f0f1b94
/*  f0f04f8:	8fa20468 */ 	lw	$v0,0x468($sp)
.NB0f0f04fc:
/*  f0f04fc:	318d00ff */ 	andi	$t5,$t4,0xff
/*  f0f0500:	15a000c3 */ 	bnez	$t5,.NB0f0f0810
/*  f0f0504:	a22c0000 */ 	sb	$t4,0x0($s1)
/*  f0f0508:	8e22000c */ 	lw	$v0,0xc($s1)
/*  f0f050c:	3401ffff */ 	dli	$at,0xffff
/*  f0f0510:	304effff */ 	andi	$t6,$v0,0xffff
/*  f0f0514:	11c10002 */ 	beq	$t6,$at,.NB0f0f0520
/*  f0f0518:	00027800 */ 	sll	$t7,$v0,0x0
/*  f0f051c:	05e10092 */ 	bgez	$t7,.NB0f0f0768
.NB0f0f0520:
/*  f0f0520:	0002c000 */ 	sll	$t8,$v0,0x0
/*  f0f0524:	07010008 */ 	bgez	$t8,.NB0f0f0548
/*  f0f0528:	3c01000f */ 	lui	$at,0xf
/*  f0f052c:	3421fc00 */ 	ori	$at,$at,0xfc00
/*  f0f0530:	00415024 */ 	and	$t2,$v0,$at
/*  f0f0534:	305903ff */ 	andi	$t9,$v0,0x3ff
/*  f0f0538:	000a5a82 */ 	srl	$t3,$t2,0xa
/*  f0f053c:	afb90428 */ 	sw	$t9,0x428($sp)
/*  f0f0540:	1000001a */ 	beqz	$zero,.NB0f0f05ac
/*  f0f0544:	afab042c */ 	sw	$t3,0x42c($sp)
.NB0f0f0548:
/*  f0f0548:	00028402 */ 	srl	$s0,$v0,0x10
/*  f0f054c:	320c00ff */ 	andi	$t4,$s0,0xff
/*  f0f0550:	00026e02 */ 	srl	$t5,$v0,0x18
/*  f0f0554:	01808025 */ 	or	$s0,$t4,$zero
/*  f0f0558:	0fc6177a */ 	jal	mpGetBodyId
/*  f0f055c:	31a400ff */ 	andi	$a0,$t5,0xff
/*  f0f0560:	0fc6175d */ 	jal	mpGetNumHeads2
/*  f0f0564:	afa2042c */ 	sw	$v0,0x42c($sp)
/*  f0f0568:	0202082a */ 	slt	$at,$s0,$v0
/*  f0f056c:	10200005 */ 	beqz	$at,.NB0f0f0584
/*  f0f0570:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0574:	0fc61761 */ 	jal	mpGetHeadId
/*  f0f0578:	320400ff */ 	andi	$a0,$s0,0xff
/*  f0f057c:	1000000b */ 	beqz	$zero,.NB0f0f05ac
/*  f0f0580:	afa20428 */ 	sw	$v0,0x428($sp)
.NB0f0f0584:
/*  f0f0584:	0fc6175d */ 	jal	mpGetNumHeads2
/*  f0f0588:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f058c:	0fc51472 */ 	jal	func0f14a9f8
/*  f0f0590:	02022023 */ 	subu	$a0,$s0,$v0
/*  f0f0594:	0fc6176f */ 	jal	mpGetBeauHeadId
/*  f0f0598:	304400ff */ 	andi	$a0,$v0,0xff
/*  f0f059c:	0fc6175d */ 	jal	mpGetNumHeads2
/*  f0f05a0:	afa20428 */ 	sw	$v0,0x428($sp)
/*  f0f05a4:	02027823 */ 	subu	$t7,$s0,$v0
/*  f0f05a8:	a22f05b0 */ 	sb	$t7,0x5b0($s1)
.NB0f0f05ac:
/*  f0f05ac:	8fb8042c */ 	lw	$t8,0x42c($sp)
/*  f0f05b0:	3c0a8008 */ 	lui	$t2,0x8008
/*  f0f05b4:	254af764 */ 	addiu	$t2,$t2,-2204
/*  f0f05b8:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f05bc:	0338c821 */ 	addu	$t9,$t9,$t8
/*  f0f05c0:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0f05c4:	032a8021 */ 	addu	$s0,$t9,$t2
/*  f0f05c8:	96020002 */ 	lhu	$v0,0x2($s0)
/*  f0f05cc:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f05d0:	a7a20432 */ 	sh	$v0,0x432($sp)
/*  f0f05d4:	0fc58635 */ 	jal	fileGetInflatedSize
/*  f0f05d8:	afa20040 */ 	sw	$v0,0x40($sp)
/*  f0f05dc:	8e0d0000 */ 	lw	$t5,0x0($s0)
/*  f0f05e0:	2443003f */ 	addiu	$v1,$v0,0x3f
/*  f0f05e4:	346b003f */ 	ori	$t3,$v1,0x3f
/*  f0f05e8:	000d7840 */ 	sll	$t7,$t5,0x1
/*  f0f05ec:	05e10005 */ 	bgez	$t7,.NB0f0f0604
/*  f0f05f0:	3963003f */ 	xori	$v1,$t3,0x3f
/*  f0f05f4:	2418ffff */ 	addiu	$t8,$zero,-1
/*  f0f05f8:	afb80428 */ 	sw	$t8,0x428($sp)
/*  f0f05fc:	10000011 */ 	beqz	$zero,.NB0f0f0644
/*  f0f0600:	3410ffff */ 	dli	$s0,0xffff
.NB0f0f0604:
/*  f0f0604:	8fb90428 */ 	lw	$t9,0x428($sp)
/*  f0f0608:	3c108008 */ 	lui	$s0,0x8008
/*  f0f060c:	afa30448 */ 	sw	$v1,0x448($sp)
/*  f0f0610:	00195080 */ 	sll	$t2,$t9,0x2
/*  f0f0614:	01595021 */ 	addu	$t2,$t2,$t9
/*  f0f0618:	000a5080 */ 	sll	$t2,$t2,0x2
/*  f0f061c:	020a8021 */ 	addu	$s0,$s0,$t2
/*  f0f0620:	9610f766 */ 	lhu	$s0,-0x89a($s0)
/*  f0f0624:	0fc58635 */ 	jal	fileGetInflatedSize
/*  f0f0628:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f062c:	8fab0448 */ 	lw	$t3,0x448($sp)
/*  f0f0630:	244c003f */ 	addiu	$t4,$v0,0x3f
/*  f0f0634:	358d003f */ 	ori	$t5,$t4,0x3f
/*  f0f0638:	39ae003f */ 	xori	$t6,$t5,0x3f
/*  f0f063c:	016e1821 */ 	addu	$v1,$t3,$t6
/*  f0f0640:	afa30448 */ 	sw	$v1,0x448($sp)
.NB0f0f0644:
/*  f0f0644:	8e380004 */ 	lw	$t8,0x4($s1)
/*  f0f0648:	8e390008 */ 	lw	$t9,0x8($s1)
/*  f0f064c:	24634000 */ 	addiu	$v1,$v1,0x4000
/*  f0f0650:	afa30448 */ 	sw	$v1,0x448($sp)
/*  f0f0654:	27a40438 */ 	addiu	$a0,$sp,0x438
/*  f0f0658:	03032821 */ 	addu	$a1,$t8,$v1
/*  f0f065c:	0fc5b6c2 */ 	jal	tex0f172e70
/*  f0f0660:	03233023 */ 	subu	$a2,$t9,$v1
/*  f0f0664:	8faa0428 */ 	lw	$t2,0x428($sp)
/*  f0f0668:	8e250004 */ 	lw	$a1,0x4($s1)
/*  f0f066c:	27a70438 */ 	addiu	$a3,$sp,0x438
/*  f0f0670:	a62a0002 */ 	sh	$t2,0x2($s1)
/*  f0f0674:	8fac042c */ 	lw	$t4,0x42c($sp)
/*  f0f0678:	a62c056a */ 	sh	$t4,0x56a($s1)
/*  f0f067c:	8fa60448 */ 	lw	$a2,0x448($sp)
/*  f0f0680:	0c0091a1 */ 	jal	modeldefLoad
/*  f0f0684:	97a40432 */ 	lhu	$a0,0x432($sp)
/*  f0f0688:	ae220054 */ 	sw	$v0,0x54($s1)
/*  f0f068c:	0fc586ed */ 	jal	fileGetLoadedSize
/*  f0f0690:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f0694:	2443003f */ 	addiu	$v1,$v0,0x3f
/*  f0f0698:	346d003f */ 	ori	$t5,$v1,0x3f
/*  f0f069c:	39ab003f */ 	xori	$t3,$t5,0x3f
/*  f0f06a0:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f06a4:	0c008f6f */ 	jal	modelCalculateRwDataLen
/*  f0f06a8:	afab0434 */ 	sw	$t3,0x434($sp)
/*  f0f06ac:	8fae0428 */ 	lw	$t6,0x428($sp)
/*  f0f06b0:	8fa30434 */ 	lw	$v1,0x434($sp)
/*  f0f06b4:	3204ffff */ 	andi	$a0,$s0,0xffff
/*  f0f06b8:	05c10003 */ 	bgez	$t6,.NB0f0f06c8
/*  f0f06bc:	8fb80448 */ 	lw	$t8,0x448($sp)
/*  f0f06c0:	10000010 */ 	beqz	$zero,.NB0f0f0704
/*  f0f06c4:	ae200058 */ 	sw	$zero,0x58($s1)
.NB0f0f06c8:
/*  f0f06c8:	8e2f0004 */ 	lw	$t7,0x4($s1)
/*  f0f06cc:	afb00044 */ 	sw	$s0,0x44($sp)
/*  f0f06d0:	03033023 */ 	subu	$a2,$t8,$v1
/*  f0f06d4:	27a70438 */ 	addiu	$a3,$sp,0x438
/*  f0f06d8:	0c0091a1 */ 	jal	modeldefLoad
/*  f0f06dc:	01e32821 */ 	addu	$a1,$t7,$v1
/*  f0f06e0:	ae220058 */ 	sw	$v0,0x58($s1)
/*  f0f06e4:	0fc586ed */ 	jal	fileGetLoadedSize
/*  f0f06e8:	8fa40044 */ 	lw	$a0,0x44($sp)
/*  f0f06ec:	8e240058 */ 	lw	$a0,0x58($s1)
/*  f0f06f0:	8fa50428 */ 	lw	$a1,0x428($sp)
/*  f0f06f4:	0fc0b5be */ 	jal	bodyCalculateHeadOffset
/*  f0f06f8:	8fa6042c */ 	lw	$a2,0x42c($sp)
/*  f0f06fc:	0c008f6f */ 	jal	modelCalculateRwDataLen
/*  f0f0700:	8e240058 */ 	lw	$a0,0x58($s1)
.NB0f0f0704:
/*  f0f0704:	26240060 */ 	addiu	$a0,$s1,0x60
/*  f0f0708:	8e250054 */ 	lw	$a1,0x54($s1)
/*  f0f070c:	afa40040 */ 	sw	$a0,0x40($sp)
/*  f0f0710:	26260110 */ 	addiu	$a2,$s1,0x110
/*  f0f0714:	0c00900f */ 	jal	modelInit
/*  f0f0718:	24070001 */ 	addiu	$a3,$zero,0x1
/*  f0f071c:	26300084 */ 	addiu	$s0,$s1,0x84
/*  f0f0720:	0c00904e */ 	jal	animInit
/*  f0f0724:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f0728:	24190100 */ 	addiu	$t9,$zero,0x100
/*  f0f072c:	a6390062 */ 	sh	$t9,0x62($s1)
/*  f0f0730:	ae300080 */ 	sw	$s0,0x80($s1)
/*  f0f0734:	8e260054 */ 	lw	$a2,0x54($s1)
/*  f0f0738:	8e270058 */ 	lw	$a3,0x58($s1)
/*  f0f073c:	8faa0040 */ 	lw	$t2,0x40($sp)
/*  f0f0740:	240c0001 */ 	addiu	$t4,$zero,0x1
/*  f0f0744:	afac001c */ 	sw	$t4,0x1c($sp)
/*  f0f0748:	afa00018 */ 	sw	$zero,0x18($sp)
/*  f0f074c:	afa00010 */ 	sw	$zero,0x10($sp)
/*  f0f0750:	8fa50428 */ 	lw	$a1,0x428($sp)
/*  f0f0754:	8fa4042c */ 	lw	$a0,0x42c($sp)
/*  f0f0758:	0fc0b20b */ 	jal	body0f02ce8c
/*  f0f075c:	afaa0014 */ 	sw	$t2,0x14($sp)
/*  f0f0760:	10000027 */ 	beqz	$zero,.NB0f0f0800
/*  f0f0764:	8e39000c */ 	lw	$t9,0xc($s1)
.NB0f0f0768:
/*  f0f0768:	0fc58635 */ 	jal	fileGetInflatedSize
/*  f0f076c:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f0770:	2443003f */ 	addiu	$v1,$v0,0x3f
/*  f0f0774:	346d003f */ 	ori	$t5,$v1,0x3f
/*  f0f0778:	39ab003f */ 	xori	$t3,$t5,0x3f
/*  f0f077c:	8e2e0004 */ 	lw	$t6,0x4($s1)
/*  f0f0780:	8e2f0008 */ 	lw	$t7,0x8($s1)
/*  f0f0784:	25634000 */ 	addiu	$v1,$t3,0x4000
/*  f0f0788:	afa30448 */ 	sw	$v1,0x448($sp)
/*  f0f078c:	27a40438 */ 	addiu	$a0,$sp,0x438
/*  f0f0790:	01c32821 */ 	addu	$a1,$t6,$v1
/*  f0f0794:	0fc5b6c2 */ 	jal	tex0f172e70
/*  f0f0798:	01e33023 */ 	subu	$a2,$t7,$v1
/*  f0f079c:	2402ffff */ 	addiu	$v0,$zero,-1
/*  f0f07a0:	a6220002 */ 	sh	$v0,0x2($s1)
/*  f0f07a4:	a622056a */ 	sh	$v0,0x56a($s1)
/*  f0f07a8:	8fa60448 */ 	lw	$a2,0x448($sp)
/*  f0f07ac:	9624000e */ 	lhu	$a0,0xe($s1)
/*  f0f07b0:	8e250004 */ 	lw	$a1,0x4($s1)
/*  f0f07b4:	0c0091a1 */ 	jal	modeldefLoad
/*  f0f07b8:	27a70438 */ 	addiu	$a3,$sp,0x438
/*  f0f07bc:	ae220054 */ 	sw	$v0,0x54($s1)
/*  f0f07c0:	0fc586ed */ 	jal	fileGetLoadedSize
/*  f0f07c4:	8e24000c */ 	lw	$a0,0xc($s1)
/*  f0f07c8:	0c008f6f */ 	jal	modelCalculateRwDataLen
/*  f0f07cc:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f07d0:	26240060 */ 	addiu	$a0,$s1,0x60
/*  f0f07d4:	8e250054 */ 	lw	$a1,0x54($s1)
/*  f0f07d8:	26260110 */ 	addiu	$a2,$s1,0x110
/*  f0f07dc:	0c00900f */ 	jal	modelInit
/*  f0f07e0:	24070001 */ 	addiu	$a3,$zero,0x1
/*  f0f07e4:	26300084 */ 	addiu	$s0,$s1,0x84
/*  f0f07e8:	0c00904e */ 	jal	animInit
/*  f0f07ec:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f07f0:	24180100 */ 	addiu	$t8,$zero,0x100
/*  f0f07f4:	a6380062 */ 	sh	$t8,0x62($s1)
/*  f0f07f8:	ae300080 */ 	sw	$s0,0x80($s1)
/*  f0f07fc:	8e39000c */ 	lw	$t9,0xc($s1)
.NB0f0f0800:
/*  f0f0800:	a620005e */ 	sh	$zero,0x5e($s1)
/*  f0f0804:	ae20000c */ 	sw	$zero,0xc($s1)
/*  f0f0808:	10000003 */ 	beqz	$zero,.NB0f0f0818
/*  f0f080c:	ae390010 */ 	sw	$t9,0x10($s1)
.NB0f0f0810:
/*  f0f0810:	100004e0 */ 	beqz	$zero,.NB0f0f1b94
/*  f0f0814:	8fa20468 */ 	lw	$v0,0x468($sp)
.NB0f0f0818:
/*  f0f0818:	8e2a0054 */ 	lw	$t2,0x54($s1)
.NB0f0f081c:
/*  f0f081c:	3c0d8007 */ 	lui	$t5,0x8007
/*  f0f0820:	25ad3b28 */ 	addiu	$t5,$t5,0x3b28
/*  f0f0824:	114004da */ 	beqz	$t2,.NB0f0f1b90
/*  f0f0828:	27ac03e0 */ 	addiu	$t4,$sp,0x3e0
/*  f0f082c:	25ae003c */ 	addiu	$t6,$t5,0x3c
.NB0f0f0830:
/*  f0f0830:	8da10000 */ 	lw	$at,0x0($t5)
/*  f0f0834:	25ad000c */ 	addiu	$t5,$t5,0xc
/*  f0f0838:	258c000c */ 	addiu	$t4,$t4,0xc
/*  f0f083c:	ad81fff4 */ 	sw	$at,-0xc($t4)
/*  f0f0840:	8da1fff8 */ 	lw	$at,-0x8($t5)
/*  f0f0844:	ad81fff8 */ 	sw	$at,-0x8($t4)
/*  f0f0848:	8da1fffc */ 	lw	$at,-0x4($t5)
/*  f0f084c:	15aefff8 */ 	bne	$t5,$t6,.NB0f0f0830
/*  f0f0850:	ad81fffc */ 	sw	$at,-0x4($t4)
/*  f0f0854:	8da10000 */ 	lw	$at,0x0($t5)
/*  f0f0858:	262f0060 */ 	addiu	$t7,$s1,0x60
/*  f0f085c:	ad810000 */ 	sw	$at,0x0($t4)
/*  f0f0860:	8fb80470 */ 	lw	$t8,0x470($sp)
/*  f0f0864:	afaf0040 */ 	sw	$t7,0x40($sp)
/*  f0f0868:	2b010003 */ 	slti	$at,$t8,0x3
/*  f0f086c:	1020001b */ 	beqz	$at,.NB0f0f08dc
/*  f0f0870:	3c19800a */ 	lui	$t9,0x800a
/*  f0f0874:	8f396484 */ 	lw	$t9,0x6484($t9)
/*  f0f0878:	00195a80 */ 	sll	$t3,$t9,0xa
/*  f0f087c:	05630018 */ 	bgezl	$t3,.NB0f0f08e0
/*  f0f0880:	8faa0468 */ 	lw	$t2,0x468($sp)
/*  f0f0884:	0c002d00 */ 	jal	vi0000b280
/*  f0f0888:	8fa40468 */ 	lw	$a0,0x468($sp)
/*  f0f088c:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f0890:	0c002cd4 */ 	jal	vi0000b1d0
/*  f0f0894:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f0898:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f089c:	91ce6485 */ 	lbu	$t6,0x6485($t6)
/*  f0f08a0:	8fac0470 */ 	lw	$t4,0x470($sp)
/*  f0f08a4:	3c01800a */ 	lui	$at,0x800a
/*  f0f08a8:	31cdffdf */ 	andi	$t5,$t6,0xffdf
/*  f0f08ac:	a02d6485 */ 	sb	$t5,0x6485($at)
/*  f0f08b0:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0f08b4:	11810003 */ 	beq	$t4,$at,.NB0f0f08c4
/*  f0f08b8:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f08bc:	0fc3c728 */ 	jal	menuApplyScissor
/*  f0f08c0:	00402025 */ 	or	$a0,$v0,$zero
.NB0f0f08c4:
/*  f0f08c4:	244f0008 */ 	addiu	$t7,$v0,0x8
/*  f0f08c8:	afaf0468 */ 	sw	$t7,0x468($sp)
/*  f0f08cc:	3c18b700 */ 	lui	$t8,0xb700
/*  f0f08d0:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f08d4:	ac590004 */ 	sw	$t9,0x4($v0)
/*  f0f08d8:	ac580000 */ 	sw	$t8,0x0($v0)
.NB0f0f08dc:
/*  f0f08dc:	8faa0468 */ 	lw	$t2,0x468($sp)
.NB0f0f08e0:
/*  f0f08e0:	3c0d8006 */ 	lui	$t5,0x8006
/*  f0f08e4:	25ad3890 */ 	addiu	$t5,$t5,0x3890
/*  f0f08e8:	254b0008 */ 	addiu	$t3,$t2,0x8
/*  f0f08ec:	afab0468 */ 	sw	$t3,0x468($sp)
/*  f0f08f0:	3c0e0600 */ 	lui	$t6,0x600
/*  f0f08f4:	ad4e0000 */ 	sw	$t6,0x0($t2)
/*  f0f08f8:	ad4d0004 */ 	sw	$t5,0x4($t2)
/*  f0f08fc:	8fac0468 */ 	lw	$t4,0x468($sp)
/*  f0f0900:	3c180600 */ 	lui	$t8,0x600
/*  f0f0904:	3c198006 */ 	lui	$t9,0x8006
/*  f0f0908:	258f0008 */ 	addiu	$t7,$t4,0x8
/*  f0f090c:	afaf0468 */ 	sw	$t7,0x468($sp)
/*  f0f0910:	273938b0 */ 	addiu	$t9,$t9,0x38b0
/*  f0f0914:	ad980000 */ 	sw	$t8,0x0($t4)
/*  f0f0918:	afb9003c */ 	sw	$t9,0x3c($sp)
/*  f0f091c:	ad990004 */ 	sw	$t9,0x4($t4)
/*  f0f0920:	afa00330 */ 	sw	$zero,0x330($sp)
/*  f0f0924:	c6260554 */ 	lwc1	$f6,0x554($s1)
/*  f0f0928:	44802000 */ 	mtc1	$zero,$f4
/*  f0f092c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0930:	4606203c */ 	c.lt.s	$f4,$f6
/*  f0f0934:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0938:	4500005c */ 	bc1f	.NB0f0f0aac
/*  f0f093c:	240a0001 */ 	addiu	$t2,$zero,0x1
/*  f0f0940:	afaa031c */ 	sw	$t2,0x31c($sp)
/*  f0f0944:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f0948:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0f094c:	256bf6a0 */ 	addiu	$t3,$t3,-2400
/*  f0f0950:	8c8e0004 */ 	lw	$t6,0x4($a0)
/*  f0f0954:	556e002f */ 	bnel	$t3,$t6,.NB0f0f0a14
/*  f0f0958:	8faf031c */ 	lw	$t7,0x31c($sp)
/*  f0f095c:	0c006ea3 */ 	jal	modelGetPart
/*  f0f0960:	24050006 */ 	addiu	$a1,$zero,0x6
/*  f0f0964:	5040002b */ 	beqzl	$v0,.NB0f0f0a14
/*  f0f0968:	8faf031c */ 	lw	$t7,0x31c($sp)
/*  f0f096c:	8e2d0574 */ 	lw	$t5,0x574($s1)
/*  f0f0970:	3c0143f0 */ 	lui	$at,0x43f0
/*  f0f0974:	44812000 */ 	mtc1	$at,$f4
/*  f0f0978:	448d4000 */ 	mtc1	$t5,$f8
/*  f0f097c:	8c500004 */ 	lw	$s0,0x4($v0)
/*  f0f0980:	468042a0 */ 	cvt.s.w	$f10,$f8
/*  f0f0984:	0fc01a1d */ 	jal	func0f006bd0
/*  f0f0988:	46045303 */ 	div.s	$f12,$f10,$f4
/*  f0f098c:	44806000 */ 	mtc1	$zero,$f12
/*  f0f0990:	3c013f80 */ 	lui	$at,0x3f80
/*  f0f0994:	44817000 */ 	mtc1	$at,$f14
/*  f0f0998:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f099c:	e7ac0324 */ 	swc1	$f12,0x324($sp)
/*  f0f09a0:	c6060004 */ 	lwc1	$f6,0x4($s0)
/*  f0f09a4:	c428cc58 */ 	lwc1	$f8,-0x33a8($at)
/*  f0f09a8:	46000102 */ 	mul.s	$f4,$f0,$f0
/*  f0f09ac:	3c014387 */ 	lui	$at,0x4387
/*  f0f09b0:	240c0001 */ 	addiu	$t4,$zero,0x1
/*  f0f09b4:	afac0330 */ 	sw	$t4,0x330($sp)
/*  f0f09b8:	e7ac032c */ 	swc1	$f12,0x32c($sp)
/*  f0f09bc:	46083283 */ 	div.s	$f10,$f6,$f8
/*  f0f09c0:	46047181 */ 	sub.s	$f6,$f14,$f4
/*  f0f09c4:	46065202 */ 	mul.s	$f8,$f10,$f6
/*  f0f09c8:	46007281 */ 	sub.s	$f10,$f14,$f0
/*  f0f09cc:	44813000 */ 	mtc1	$at,$f6
/*  f0f09d0:	3c0142c8 */ 	lui	$at,0x42c8
/*  f0f09d4:	46086101 */ 	sub.s	$f4,$f12,$f8
/*  f0f09d8:	46065202 */ 	mul.s	$f8,$f10,$f6
/*  f0f09dc:	e7a40328 */ 	swc1	$f4,0x328($sp)
/*  f0f09e0:	44812000 */ 	mtc1	$at,$f4
/*  f0f09e4:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f09e8:	44813000 */ 	mtc1	$at,$f6
/*  f0f09ec:	46044080 */ 	add.s	$f2,$f8,$f4
/*  f0f09f0:	e6220554 */ 	swc1	$f2,0x554($s1)
/*  f0f09f4:	c60a0004 */ 	lwc1	$f10,0x4($s0)
/*  f0f09f8:	afa0031c */ 	sw	$zero,0x31c($sp)
/*  f0f09fc:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f0a00:	46065202 */ 	mul.s	$f8,$f10,$f6
/*  f0f0a04:	46081103 */ 	div.s	$f4,$f2,$f8
/*  f0f0a08:	0fc19f4a */ 	jal	modelFindBboxRodata
/*  f0f0a0c:	e7a40320 */ 	swc1	$f4,0x320($sp)
/*  f0f0a10:	8faf031c */ 	lw	$t7,0x31c($sp)
.NB0f0f0a14:
/*  f0f0a14:	11e00025 */ 	beqz	$t7,.NB0f0f0aac
/*  f0f0a18:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0a1c:	0fc19f4a */ 	jal	modelFindBboxRodata
/*  f0f0a20:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f0a24:	10400021 */ 	beqz	$v0,.NB0f0f0aac
/*  f0f0a28:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0a2c:	c4400008 */ 	lwc1	$f0,0x8($v0)
/*  f0f0a30:	c44a0004 */ 	lwc1	$f10,0x4($v0)
/*  f0f0a34:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f0a38:	44817000 */ 	mtc1	$at,$f14
/*  f0f0a3c:	460a0181 */ 	sub.s	$f6,$f0,$f10
/*  f0f0a40:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f0a44:	460e3202 */ 	mul.s	$f8,$f6,$f14
/*  f0f0a48:	46080101 */ 	sub.s	$f4,$f0,$f8
/*  f0f0a4c:	46002287 */ 	neg.s	$f10,$f4
/*  f0f0a50:	e7aa0324 */ 	swc1	$f10,0x324($sp)
/*  f0f0a54:	c446000c */ 	lwc1	$f6,0xc($v0)
/*  f0f0a58:	c4420010 */ 	lwc1	$f2,0x10($v0)
/*  f0f0a5c:	46061201 */ 	sub.s	$f8,$f2,$f6
/*  f0f0a60:	460e4102 */ 	mul.s	$f4,$f8,$f14
/*  f0f0a64:	46041281 */ 	sub.s	$f10,$f2,$f4
/*  f0f0a68:	46005187 */ 	neg.s	$f6,$f10
/*  f0f0a6c:	e7a60328 */ 	swc1	$f6,0x328($sp)
/*  f0f0a70:	c4480014 */ 	lwc1	$f8,0x14($v0)
/*  f0f0a74:	c44c0018 */ 	lwc1	$f12,0x18($v0)
/*  f0f0a78:	afb80330 */ 	sw	$t8,0x330($sp)
/*  f0f0a7c:	46086101 */ 	sub.s	$f4,$f12,$f8
/*  f0f0a80:	460e2282 */ 	mul.s	$f10,$f4,$f14
/*  f0f0a84:	460a6181 */ 	sub.s	$f6,$f12,$f10
/*  f0f0a88:	46003207 */ 	neg.s	$f8,$f6
/*  f0f0a8c:	e7a8032c */ 	swc1	$f8,0x32c($sp)
/*  f0f0a90:	c44a000c */ 	lwc1	$f10,0xc($v0)
/*  f0f0a94:	c4440010 */ 	lwc1	$f4,0x10($v0)
/*  f0f0a98:	460a2181 */ 	sub.s	$f6,$f4,$f10
/*  f0f0a9c:	c6240554 */ 	lwc1	$f4,0x554($s1)
/*  f0f0aa0:	460e3202 */ 	mul.s	$f8,$f6,$f14
/*  f0f0aa4:	46082283 */ 	div.s	$f10,$f4,$f8
/*  f0f0aa8:	e7aa0320 */ 	swc1	$f10,0x320($sp)
.NB0f0f0aac:
/*  f0f0aac:	0c005a28 */ 	jal	mtx4LoadIdentity
/*  f0f0ab0:	27a40378 */ 	addiu	$a0,$sp,0x378
/*  f0f0ab4:	8fb90470 */ 	lw	$t9,0x470($sp)
/*  f0f0ab8:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0abc:	17210083 */ 	bne	$t9,$at,.NB0f0f0ccc
/*  f0f0ac0:	3c0a8009 */ 	lui	$t2,0x8009
/*  f0f0ac4:	914a30e0 */ 	lbu	$t2,0x30e0($t2)
/*  f0f0ac8:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0acc:	27a403c0 */ 	addiu	$a0,$sp,0x3c0
/*  f0f0ad0:	11410130 */ 	beq	$t2,$at,.NB0f0f0f94
/*  f0f0ad4:	27a50378 */ 	addiu	$a1,$sp,0x378
/*  f0f0ad8:	c6200538 */ 	lwc1	$f0,0x538($s1)
/*  f0f0adc:	c6260510 */ 	lwc1	$f6,0x510($s1)
/*  f0f0ae0:	3c03800a */ 	lui	$v1,0x800a
/*  f0f0ae4:	2463e6c0 */ 	addiu	$v1,$v1,-6464
/*  f0f0ae8:	46060032 */ 	c.eq.s	$f0,$f6
/*  f0f0aec:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0af0:	45030014 */ 	bc1tl	.NB0f0f0b44
/*  f0f0af4:	c62c053c */ 	lwc1	$f12,0x53c($s1)
/*  f0f0af8:	8c6b0000 */ 	lw	$t3,0x0($v1)
/*  f0f0afc:	00001025 */ 	or	$v0,$zero,$zero
/*  f0f0b00:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0b04:	5960000f */ 	blezl	$t3,.NB0f0f0b44
/*  f0f0b08:	c62c053c */ 	lwc1	$f12,0x53c($s1)
/*  f0f0b0c:	c424cc5c */ 	lwc1	$f4,-0x33a4($at)
/*  f0f0b10:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0b14:	46040082 */ 	mul.s	$f2,$f0,$f4
/*  f0f0b18:	c420cc60 */ 	lwc1	$f0,-0x33a0($at)
/*  f0f0b1c:	c6280510 */ 	lwc1	$f8,0x510($s1)
.NB0f0f0b20:
/*  f0f0b20:	24420001 */ 	addiu	$v0,$v0,0x1
/*  f0f0b24:	46080282 */ 	mul.s	$f10,$f0,$f8
/*  f0f0b28:	460a1180 */ 	add.s	$f6,$f2,$f10
/*  f0f0b2c:	e6260510 */ 	swc1	$f6,0x510($s1)
/*  f0f0b30:	8c6e0000 */ 	lw	$t6,0x0($v1)
/*  f0f0b34:	004e082a */ 	slt	$at,$v0,$t6
/*  f0f0b38:	5420fff9 */ 	bnezl	$at,.NB0f0f0b20
/*  f0f0b3c:	c6280510 */ 	lwc1	$f8,0x510($s1)
/*  f0f0b40:	c62c053c */ 	lwc1	$f12,0x53c($s1)
.NB0f0f0b44:
/*  f0f0b44:	c6240514 */ 	lwc1	$f4,0x514($s1)
/*  f0f0b48:	3c03800a */ 	lui	$v1,0x800a
/*  f0f0b4c:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0b50:	46046032 */ 	c.eq.s	$f12,$f4
/*  f0f0b54:	c420cc64 */ 	lwc1	$f0,-0x339c($at)
/*  f0f0b58:	2463e6c0 */ 	addiu	$v1,$v1,-6464
/*  f0f0b5c:	3c0b8007 */ 	lui	$t3,0x8007
/*  f0f0b60:	45030013 */ 	bc1tl	.NB0f0f0bb0
/*  f0f0b64:	c62c0540 */ 	lwc1	$f12,0x540($s1)
/*  f0f0b68:	8c6d0000 */ 	lw	$t5,0x0($v1)
/*  f0f0b6c:	00001025 */ 	or	$v0,$zero,$zero
/*  f0f0b70:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0b74:	59a0000e */ 	blezl	$t5,.NB0f0f0bb0
/*  f0f0b78:	c62c0540 */ 	lwc1	$f12,0x540($s1)
/*  f0f0b7c:	c428cc68 */ 	lwc1	$f8,-0x3398($at)
/*  f0f0b80:	46086082 */ 	mul.s	$f2,$f12,$f8
/*  f0f0b84:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0b88:	c62a0514 */ 	lwc1	$f10,0x514($s1)
.NB0f0f0b8c:
/*  f0f0b8c:	24420001 */ 	addiu	$v0,$v0,0x1
/*  f0f0b90:	460a0182 */ 	mul.s	$f6,$f0,$f10
/*  f0f0b94:	46061100 */ 	add.s	$f4,$f2,$f6
/*  f0f0b98:	e6240514 */ 	swc1	$f4,0x514($s1)
/*  f0f0b9c:	8c6c0000 */ 	lw	$t4,0x0($v1)
/*  f0f0ba0:	004c082a */ 	slt	$at,$v0,$t4
/*  f0f0ba4:	5420fff9 */ 	bnezl	$at,.NB0f0f0b8c
/*  f0f0ba8:	c62a0514 */ 	lwc1	$f10,0x514($s1)
/*  f0f0bac:	c62c0540 */ 	lwc1	$f12,0x540($s1)
.NB0f0f0bb0:
/*  f0f0bb0:	c6280518 */ 	lwc1	$f8,0x518($s1)
/*  f0f0bb4:	46086032 */ 	c.eq.s	$f12,$f8
/*  f0f0bb8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0bbc:	45030013 */ 	bc1tl	.NB0f0f0c0c
/*  f0f0bc0:	c62c0544 */ 	lwc1	$f12,0x544($s1)
/*  f0f0bc4:	8c6f0000 */ 	lw	$t7,0x0($v1)
/*  f0f0bc8:	00001025 */ 	or	$v0,$zero,$zero
/*  f0f0bcc:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0bd0:	59e0000e */ 	blezl	$t7,.NB0f0f0c0c
/*  f0f0bd4:	c62c0544 */ 	lwc1	$f12,0x544($s1)
/*  f0f0bd8:	c42acc6c */ 	lwc1	$f10,-0x3394($at)
/*  f0f0bdc:	460a6082 */ 	mul.s	$f2,$f12,$f10
/*  f0f0be0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0be4:	c6260518 */ 	lwc1	$f6,0x518($s1)
.NB0f0f0be8:
/*  f0f0be8:	24420001 */ 	addiu	$v0,$v0,0x1
/*  f0f0bec:	46060102 */ 	mul.s	$f4,$f0,$f6
/*  f0f0bf0:	46041200 */ 	add.s	$f8,$f2,$f4
/*  f0f0bf4:	e6280518 */ 	swc1	$f8,0x518($s1)
/*  f0f0bf8:	8c780000 */ 	lw	$t8,0x0($v1)
/*  f0f0bfc:	0058082a */ 	slt	$at,$v0,$t8
/*  f0f0c00:	5420fff9 */ 	bnezl	$at,.NB0f0f0be8
/*  f0f0c04:	c6260518 */ 	lwc1	$f6,0x518($s1)
/*  f0f0c08:	c62c0544 */ 	lwc1	$f12,0x544($s1)
.NB0f0f0c0c:
/*  f0f0c0c:	c62e051c */ 	lwc1	$f14,0x51c($s1)
/*  f0f0c10:	460e6032 */ 	c.eq.s	$f12,$f14
/*  f0f0c14:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0c18:	45030014 */ 	bc1tl	.NB0f0f0c6c
/*  f0f0c1c:	c62a0510 */ 	lwc1	$f10,0x510($s1)
/*  f0f0c20:	8c790000 */ 	lw	$t9,0x0($v1)
/*  f0f0c24:	00001025 */ 	or	$v0,$zero,$zero
/*  f0f0c28:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0c2c:	5b20000f */ 	blezl	$t9,.NB0f0f0c6c
/*  f0f0c30:	c62a0510 */ 	lwc1	$f10,0x510($s1)
/*  f0f0c34:	c42acc70 */ 	lwc1	$f10,-0x3390($at)
/*  f0f0c38:	460a6082 */ 	mul.s	$f2,$f12,$f10
/*  f0f0c3c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0c40:	c626051c */ 	lwc1	$f6,0x51c($s1)
.NB0f0f0c44:
/*  f0f0c44:	24420001 */ 	addiu	$v0,$v0,0x1
/*  f0f0c48:	46060102 */ 	mul.s	$f4,$f0,$f6
/*  f0f0c4c:	46041200 */ 	add.s	$f8,$f2,$f4
/*  f0f0c50:	e628051c */ 	swc1	$f8,0x51c($s1)
/*  f0f0c54:	8c6a0000 */ 	lw	$t2,0x0($v1)
/*  f0f0c58:	004a082a */ 	slt	$at,$v0,$t2
/*  f0f0c5c:	5420fff9 */ 	bnezl	$at,.NB0f0f0c44
/*  f0f0c60:	c626051c */ 	lwc1	$f6,0x51c($s1)
/*  f0f0c64:	c62e051c */ 	lwc1	$f14,0x51c($s1)
/*  f0f0c68:	c62a0510 */ 	lwc1	$f10,0x510($s1)
.NB0f0f0c6c:
/*  f0f0c6c:	8d6b2d88 */ 	lw	$t3,0x2d88($t3)
/*  f0f0c70:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0c74:	e7aa0458 */ 	swc1	$f10,0x458($sp)
/*  f0f0c78:	c6320518 */ 	lwc1	$f18,0x518($s1)
/*  f0f0c7c:	15610004 */ 	bne	$t3,$at,.NB0f0f0c90
/*  f0f0c80:	c6300514 */ 	lwc1	$f16,0x514($s1)
/*  f0f0c84:	c7a60458 */ 	lwc1	$f6,0x458($sp)
/*  f0f0c88:	46063100 */ 	add.s	$f4,$f6,$f6
/*  f0f0c8c:	e7a40458 */ 	swc1	$f4,0x458($sp)
.NB0f0f0c90:
/*  f0f0c90:	e7b00454 */ 	swc1	$f16,0x454($sp)
/*  f0f0c94:	e7b20450 */ 	swc1	$f18,0x450($sp)
/*  f0f0c98:	e7ae044c */ 	swc1	$f14,0x44c($sp)
/*  f0f0c9c:	c6220548 */ 	lwc1	$f2,0x548($s1)
/*  f0f0ca0:	c62c054c */ 	lwc1	$f12,0x54c($s1)
/*  f0f0ca4:	c6200550 */ 	lwc1	$f0,0x550($s1)
/*  f0f0ca8:	e6220520 */ 	swc1	$f2,0x520($s1)
/*  f0f0cac:	e62c0524 */ 	swc1	$f12,0x524($s1)
/*  f0f0cb0:	e6200528 */ 	swc1	$f0,0x528($s1)
/*  f0f0cb4:	e7a203c0 */ 	swc1	$f2,0x3c0($sp)
/*  f0f0cb8:	e7ac03c4 */ 	swc1	$f12,0x3c4($sp)
/*  f0f0cbc:	0c005d07 */ 	jal	mtx4LoadRotation
/*  f0f0cc0:	e7a003c8 */ 	swc1	$f0,0x3c8($sp)
/*  f0f0cc4:	100000b4 */ 	beqz	$zero,.NB0f0f0f98
/*  f0f0cc8:	3c01c2c8 */ 	lui	$at,0xc2c8
.NB0f0f0ccc:
/*  f0f0ccc:	8e2205b0 */ 	lw	$v0,0x5b0($s1)
/*  f0f0cd0:	3c01800a */ 	lui	$at,0x800a
/*  f0f0cd4:	00027300 */ 	sll	$t6,$v0,0xc
/*  f0f0cd8:	000e17c2 */ 	srl	$v0,$t6,0x1f
/*  f0f0cdc:	10400093 */ 	beqz	$v0,.NB0f0f0f2c
/*  f0f0ce0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0ce4:	c428e6c4 */ 	lwc1	$f8,-0x193c($at)
/*  f0f0ce8:	3c014220 */ 	lui	$at,0x4220
/*  f0f0cec:	44815000 */ 	mtc1	$at,$f10
/*  f0f0cf0:	c6240564 */ 	lwc1	$f4,0x564($s1)
/*  f0f0cf4:	3c013f80 */ 	lui	$at,0x3f80
/*  f0f0cf8:	460a4183 */ 	div.s	$f6,$f8,$f10
/*  f0f0cfc:	44815000 */ 	mtc1	$at,$f10
/*  f0f0d00:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f0d04:	46062200 */ 	add.s	$f8,$f4,$f6
/*  f0f0d08:	e6280564 */ 	swc1	$f8,0x564($s1)
/*  f0f0d0c:	c6200564 */ 	lwc1	$f0,0x564($s1)
/*  f0f0d10:	4600503c */ 	c.lt.s	$f10,$f0
/*  f0f0d14:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0d18:	45000010 */ 	bc1f	.NB0f0f0d5c
/*  f0f0d1c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0d20:	922c05b1 */ 	lbu	$t4,0x5b1($s1)
/*  f0f0d24:	c6240538 */ 	lwc1	$f4,0x538($s1)
/*  f0f0d28:	c626053c */ 	lwc1	$f6,0x53c($s1)
/*  f0f0d2c:	318ffff7 */ 	andi	$t7,$t4,0xfff7
/*  f0f0d30:	a22f05b1 */ 	sb	$t7,0x5b1($s1)
/*  f0f0d34:	8e2205b0 */ 	lw	$v0,0x5b0($s1)
/*  f0f0d38:	c6280540 */ 	lwc1	$f8,0x540($s1)
/*  f0f0d3c:	c62a0544 */ 	lwc1	$f10,0x544($s1)
/*  f0f0d40:	0002c300 */ 	sll	$t8,$v0,0xc
/*  f0f0d44:	001817c2 */ 	srl	$v0,$t8,0x1f
/*  f0f0d48:	e6240510 */ 	swc1	$f4,0x510($s1)
/*  f0f0d4c:	e6260514 */ 	swc1	$f6,0x514($s1)
/*  f0f0d50:	e6280518 */ 	swc1	$f8,0x518($s1)
/*  f0f0d54:	10000075 */ 	beqz	$zero,.NB0f0f0f2c
/*  f0f0d58:	e62a051c */ 	swc1	$f10,0x51c($s1)
.NB0f0f0d5c:
/*  f0f0d5c:	c424cc74 */ 	lwc1	$f4,-0x338c($at)
/*  f0f0d60:	46040302 */ 	mul.s	$f12,$f0,$f4
/*  f0f0d64:	0c006d52 */ 	jal	cosf
/*  f0f0d68:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0d6c:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f0d70:	44814000 */ 	mtc1	$at,$f8
/*  f0f0d74:	46000187 */ 	neg.s	$f6,$f0
/*  f0f0d78:	44812000 */ 	mtc1	$at,$f4
/*  f0f0d7c:	46083282 */ 	mul.s	$f10,$f6,$f8
/*  f0f0d80:	92220568 */ 	lbu	$v0,0x568($s1)
/*  f0f0d84:	3c013f80 */ 	lui	$at,0x3f80
/*  f0f0d88:	44813000 */ 	mtc1	$at,$f6
/*  f0f0d8c:	304a0002 */ 	andi	$t2,$v0,0x2
/*  f0f0d90:	27a403c0 */ 	addiu	$a0,$sp,0x3c0
/*  f0f0d94:	27a50378 */ 	addiu	$a1,$sp,0x378
/*  f0f0d98:	46045080 */ 	add.s	$f2,$f10,$f4
/*  f0f0d9c:	46001386 */ 	mov.s	$f14,$f2
/*  f0f0da0:	11400017 */ 	beqz	$t2,.NB0f0f0e00
/*  f0f0da4:	46023301 */ 	sub.s	$f12,$f6,$f2
/*  f0f0da8:	c6280510 */ 	lwc1	$f8,0x510($s1)
/*  f0f0dac:	c6240538 */ 	lwc1	$f4,0x538($s1)
/*  f0f0db0:	460c4282 */ 	mul.s	$f10,$f8,$f12
/*  f0f0db4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0db8:	46041182 */ 	mul.s	$f6,$f2,$f4
/*  f0f0dbc:	46065200 */ 	add.s	$f8,$f10,$f6
/*  f0f0dc0:	e7a80458 */ 	swc1	$f8,0x458($sp)
/*  f0f0dc4:	c6240514 */ 	lwc1	$f4,0x514($s1)
/*  f0f0dc8:	c626053c */ 	lwc1	$f6,0x53c($s1)
/*  f0f0dcc:	460c2282 */ 	mul.s	$f10,$f4,$f12
/*  f0f0dd0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0dd4:	46061202 */ 	mul.s	$f8,$f2,$f6
/*  f0f0dd8:	46085100 */ 	add.s	$f4,$f10,$f8
/*  f0f0ddc:	e7a40454 */ 	swc1	$f4,0x454($sp)
/*  f0f0de0:	c6260518 */ 	lwc1	$f6,0x518($s1)
/*  f0f0de4:	c6280540 */ 	lwc1	$f8,0x540($s1)
/*  f0f0de8:	460c3282 */ 	mul.s	$f10,$f6,$f12
/*  f0f0dec:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0df0:	46081102 */ 	mul.s	$f4,$f2,$f8
/*  f0f0df4:	46045180 */ 	add.s	$f6,$f10,$f4
/*  f0f0df8:	1000000b */ 	beqz	$zero,.NB0f0f0e28
/*  f0f0dfc:	e7a60450 */ 	swc1	$f6,0x450($sp)
.NB0f0f0e00:
/*  f0f0e00:	c6200538 */ 	lwc1	$f0,0x538($s1)
/*  f0f0e04:	e6200510 */ 	swc1	$f0,0x510($s1)
/*  f0f0e08:	e7a00458 */ 	swc1	$f0,0x458($sp)
/*  f0f0e0c:	c620053c */ 	lwc1	$f0,0x53c($s1)
/*  f0f0e10:	e6200514 */ 	swc1	$f0,0x514($s1)
/*  f0f0e14:	e7a00454 */ 	swc1	$f0,0x454($sp)
/*  f0f0e18:	c6200540 */ 	lwc1	$f0,0x540($s1)
/*  f0f0e1c:	e6200518 */ 	swc1	$f0,0x518($s1)
/*  f0f0e20:	e7a00450 */ 	swc1	$f0,0x450($sp)
/*  f0f0e24:	92220568 */ 	lbu	$v0,0x568($s1)
.NB0f0f0e28:
/*  f0f0e28:	304b0001 */ 	andi	$t3,$v0,0x1
/*  f0f0e2c:	5160000a */ 	beqzl	$t3,.NB0f0f0e58
/*  f0f0e30:	c6200544 */ 	lwc1	$f0,0x544($s1)
/*  f0f0e34:	c628051c */ 	lwc1	$f8,0x51c($s1)
/*  f0f0e38:	c6240544 */ 	lwc1	$f4,0x544($s1)
/*  f0f0e3c:	460c4282 */ 	mul.s	$f10,$f8,$f12
/*  f0f0e40:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0e44:	46047182 */ 	mul.s	$f6,$f14,$f4
/*  f0f0e48:	46065200 */ 	add.s	$f8,$f10,$f6
/*  f0f0e4c:	10000005 */ 	beqz	$zero,.NB0f0f0e64
/*  f0f0e50:	e7a8044c */ 	swc1	$f8,0x44c($sp)
/*  f0f0e54:	c6200544 */ 	lwc1	$f0,0x544($s1)
.NB0f0f0e58:
/*  f0f0e58:	e620051c */ 	swc1	$f0,0x51c($s1)
/*  f0f0e5c:	e7a0044c */ 	swc1	$f0,0x44c($sp)
/*  f0f0e60:	92220568 */ 	lbu	$v0,0x568($s1)
.NB0f0f0e64:
/*  f0f0e64:	304e0004 */ 	andi	$t6,$v0,0x4
/*  f0f0e68:	51c00024 */ 	beqzl	$t6,.NB0f0f0efc
/*  f0f0e6c:	c6220548 */ 	lwc1	$f2,0x548($s1)
/*  f0f0e70:	c6240520 */ 	lwc1	$f4,0x520($s1)
/*  f0f0e74:	27b002b8 */ 	addiu	$s0,$sp,0x2b8
/*  f0f0e78:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f0e7c:	e7a402b8 */ 	swc1	$f4,0x2b8($sp)
/*  f0f0e80:	c62a0524 */ 	lwc1	$f10,0x524($s1)
/*  f0f0e84:	27a502e4 */ 	addiu	$a1,$sp,0x2e4
/*  f0f0e88:	e7aa02bc */ 	swc1	$f10,0x2bc($sp)
/*  f0f0e8c:	c6260528 */ 	lwc1	$f6,0x528($s1)
/*  f0f0e90:	e7ae02f8 */ 	swc1	$f14,0x2f8($sp)
/*  f0f0e94:	0fc2530c */ 	jal	quaternion0f096ca0
/*  f0f0e98:	e7a602c0 */ 	swc1	$f6,0x2c0($sp)
/*  f0f0e9c:	c6280548 */ 	lwc1	$f8,0x548($s1)
/*  f0f0ea0:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f0ea4:	27a502d4 */ 	addiu	$a1,$sp,0x2d4
/*  f0f0ea8:	e7a802b8 */ 	swc1	$f8,0x2b8($sp)
/*  f0f0eac:	c624054c */ 	lwc1	$f4,0x54c($s1)
/*  f0f0eb0:	e7a402bc */ 	swc1	$f4,0x2bc($sp)
/*  f0f0eb4:	c62a0550 */ 	lwc1	$f10,0x550($s1)
/*  f0f0eb8:	0fc2530c */ 	jal	quaternion0f096ca0
/*  f0f0ebc:	e7aa02c0 */ 	swc1	$f10,0x2c0($sp)
/*  f0f0ec0:	c7ae02f8 */ 	lwc1	$f14,0x2f8($sp)
/*  f0f0ec4:	27b002c4 */ 	addiu	$s0,$sp,0x2c4
/*  f0f0ec8:	02003825 */ 	or	$a3,$s0,$zero
/*  f0f0ecc:	44067000 */ 	mfc1	$a2,$f14
/*  f0f0ed0:	27a402e4 */ 	addiu	$a0,$sp,0x2e4
/*  f0f0ed4:	0fc25492 */ 	jal	quaternionSlerp
/*  f0f0ed8:	27a502d4 */ 	addiu	$a1,$sp,0x2d4
/*  f0f0edc:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f0ee0:	0fc25399 */ 	jal	quaternionToMtx
/*  f0f0ee4:	27a50378 */ 	addiu	$a1,$sp,0x378
/*  f0f0ee8:	8e2205b0 */ 	lw	$v0,0x5b0($s1)
/*  f0f0eec:	00026b00 */ 	sll	$t5,$v0,0xc
/*  f0f0ef0:	1000000e */ 	beqz	$zero,.NB0f0f0f2c
/*  f0f0ef4:	000d17c2 */ 	srl	$v0,$t5,0x1f
/*  f0f0ef8:	c6220548 */ 	lwc1	$f2,0x548($s1)
.NB0f0f0efc:
/*  f0f0efc:	c62c054c */ 	lwc1	$f12,0x54c($s1)
/*  f0f0f00:	c6200550 */ 	lwc1	$f0,0x550($s1)
/*  f0f0f04:	e6220520 */ 	swc1	$f2,0x520($s1)
/*  f0f0f08:	e62c0524 */ 	swc1	$f12,0x524($s1)
/*  f0f0f0c:	e6200528 */ 	swc1	$f0,0x528($s1)
/*  f0f0f10:	e7a203c0 */ 	swc1	$f2,0x3c0($sp)
/*  f0f0f14:	e7ac03c4 */ 	swc1	$f12,0x3c4($sp)
/*  f0f0f18:	0c005d07 */ 	jal	mtx4LoadRotation
/*  f0f0f1c:	e7a003c8 */ 	swc1	$f0,0x3c8($sp)
/*  f0f0f20:	8e2205b0 */ 	lw	$v0,0x5b0($s1)
/*  f0f0f24:	00027b00 */ 	sll	$t7,$v0,0xc
/*  f0f0f28:	000f17c2 */ 	srl	$v0,$t7,0x1f
.NB0f0f0f2c:
/*  f0f0f2c:	5440001a */ 	bnezl	$v0,.NB0f0f0f98
/*  f0f0f30:	3c01c2c8 */ 	lui	$at,0xc2c8
/*  f0f0f34:	c6200538 */ 	lwc1	$f0,0x538($s1)
/*  f0f0f38:	27a403c0 */ 	addiu	$a0,$sp,0x3c0
/*  f0f0f3c:	27a50378 */ 	addiu	$a1,$sp,0x378
/*  f0f0f40:	e6200510 */ 	swc1	$f0,0x510($s1)
/*  f0f0f44:	e7a00458 */ 	swc1	$f0,0x458($sp)
/*  f0f0f48:	c620053c */ 	lwc1	$f0,0x53c($s1)
/*  f0f0f4c:	e6200514 */ 	swc1	$f0,0x514($s1)
/*  f0f0f50:	e7a00454 */ 	swc1	$f0,0x454($sp)
/*  f0f0f54:	c6200540 */ 	lwc1	$f0,0x540($s1)
/*  f0f0f58:	e6200518 */ 	swc1	$f0,0x518($s1)
/*  f0f0f5c:	e7a00450 */ 	swc1	$f0,0x450($sp)
/*  f0f0f60:	c6200544 */ 	lwc1	$f0,0x544($s1)
/*  f0f0f64:	e620051c */ 	swc1	$f0,0x51c($s1)
/*  f0f0f68:	e7a0044c */ 	swc1	$f0,0x44c($sp)
/*  f0f0f6c:	c6220548 */ 	lwc1	$f2,0x548($s1)
/*  f0f0f70:	c62c054c */ 	lwc1	$f12,0x54c($s1)
/*  f0f0f74:	c6200550 */ 	lwc1	$f0,0x550($s1)
/*  f0f0f78:	e6220520 */ 	swc1	$f2,0x520($s1)
/*  f0f0f7c:	e62c0524 */ 	swc1	$f12,0x524($s1)
/*  f0f0f80:	e6200528 */ 	swc1	$f0,0x528($s1)
/*  f0f0f84:	e7a203c0 */ 	swc1	$f2,0x3c0($sp)
/*  f0f0f88:	e7ac03c4 */ 	swc1	$f12,0x3c4($sp)
/*  f0f0f8c:	0c005d07 */ 	jal	mtx4LoadRotation
/*  f0f0f90:	e7a003c8 */ 	swc1	$f0,0x3c8($sp)
.NB0f0f0f94:
/*  f0f0f94:	3c01c2c8 */ 	lui	$at,0xc2c8
.NB0f0f0f98:
/*  f0f0f98:	44813000 */ 	mtc1	$at,$f6
/*  f0f0f9c:	c7a80450 */ 	lwc1	$f8,0x450($sp)
/*  f0f0fa0:	8fb90470 */ 	lw	$t9,0x470($sp)
/*  f0f0fa4:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0fa8:	46083100 */ 	add.s	$f4,$f6,$f8
/*  f0f0fac:	17210011 */ 	bne	$t9,$at,.NB0f0f0ff4
/*  f0f0fb0:	e7a40334 */ 	swc1	$f4,0x334($sp)
/*  f0f0fb4:	3c0a8009 */ 	lui	$t2,0x8009
/*  f0f0fb8:	914a30e0 */ 	lbu	$t2,0x30e0($t2)
/*  f0f0fbc:	24010001 */ 	addiu	$at,$zero,0x1
/*  f0f0fc0:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0f0fc4:	51410035 */ 	beql	$t2,$at,.NB0f0f109c
/*  f0f0fc8:	27a403b8 */ 	addiu	$a0,$sp,0x3b8
/*  f0f0fcc:	8d6b2320 */ 	lw	$t3,0x2320($t3)
/*  f0f0fd0:	c62a0510 */ 	lwc1	$f10,0x510($s1)
/*  f0f0fd4:	448b3000 */ 	mtc1	$t3,$f6
/*  f0f0fd8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0fdc:	46803220 */ 	cvt.s.w	$f8,$f6
/*  f0f0fe0:	46085102 */ 	mul.s	$f4,$f10,$f8
/*  f0f0fe4:	e7a403b8 */ 	swc1	$f4,0x3b8($sp)
/*  f0f0fe8:	c6260514 */ 	lwc1	$f6,0x514($s1)
/*  f0f0fec:	1000002a */ 	beqz	$zero,.NB0f0f1098
/*  f0f0ff0:	e7a603bc */ 	swc1	$f6,0x3bc($sp)
.NB0f0f0ff4:
/*  f0f0ff4:	0c002fb5 */ 	jal	viGetViewLeft
/*  f0f0ff8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f0ffc:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f1000:	00107403 */ 	sra	$t6,$s0,0x10
/*  f0f1004:	0c002f97 */ 	jal	viGetViewWidth
/*  f0f1008:	01c08025 */ 	or	$s0,$t6,$zero
/*  f0f100c:	44825000 */ 	mtc1	$v0,$f10
/*  f0f1010:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f1014:	44812000 */ 	mtc1	$at,$f4
/*  f0f1018:	46805220 */ 	cvt.s.w	$f8,$f10
/*  f0f101c:	3c0d8008 */ 	lui	$t5,0x8008
/*  f0f1020:	8dad2320 */ 	lw	$t5,0x2320($t5)
/*  f0f1024:	c7aa0458 */ 	lwc1	$f10,0x458($sp)
/*  f0f1028:	46044182 */ 	mul.s	$f6,$f8,$f4
/*  f0f102c:	448d4000 */ 	mtc1	$t5,$f8
/*  f0f1030:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1034:	46804120 */ 	cvt.s.w	$f4,$f8
/*  f0f1038:	46045202 */ 	mul.s	$f8,$f10,$f4
/*  f0f103c:	44905000 */ 	mtc1	$s0,$f10
/*  f0f1040:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1044:	46805120 */ 	cvt.s.w	$f4,$f10
/*  f0f1048:	46044280 */ 	add.s	$f10,$f8,$f4
/*  f0f104c:	460a3200 */ 	add.s	$f8,$f6,$f10
/*  f0f1050:	0c002fb9 */ 	jal	viGetViewTop
/*  f0f1054:	e7a803b8 */ 	swc1	$f8,0x3b8($sp)
/*  f0f1058:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0f105c:	00106403 */ 	sra	$t4,$s0,0x10
/*  f0f1060:	0c002f9b */ 	jal	viGetViewHeight
/*  f0f1064:	01808025 */ 	or	$s0,$t4,$zero
/*  f0f1068:	44822000 */ 	mtc1	$v0,$f4
/*  f0f106c:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f1070:	44815000 */ 	mtc1	$at,$f10
/*  f0f1074:	468021a0 */ 	cvt.s.w	$f6,$f4
/*  f0f1078:	c7a40454 */ 	lwc1	$f4,0x454($sp)
/*  f0f107c:	460a3202 */ 	mul.s	$f8,$f6,$f10
/*  f0f1080:	44903000 */ 	mtc1	$s0,$f6
/*  f0f1084:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1088:	468032a0 */ 	cvt.s.w	$f10,$f6
/*  f0f108c:	460a2180 */ 	add.s	$f6,$f4,$f10
/*  f0f1090:	46064100 */ 	add.s	$f4,$f8,$f6
/*  f0f1094:	e7a403bc */ 	swc1	$f4,0x3bc($sp)
.NB0f0f1098:
/*  f0f1098:	27a403b8 */ 	addiu	$a0,$sp,0x3b8
.NB0f0f109c:
/*  f0f109c:	27a503c0 */ 	addiu	$a1,$sp,0x3c0
/*  f0f10a0:	0fc2ca67 */ 	jal	cam0f0b4c3c
/*  f0f10a4:	3c063f80 */ 	lui	$a2,0x3f80
/*  f0f10a8:	0c005a28 */ 	jal	mtx4LoadIdentity
/*  f0f10ac:	27a40338 */ 	addiu	$a0,$sp,0x338
/*  f0f10b0:	8e2205b4 */ 	lw	$v0,0x5b4($s1)
/*  f0f10b4:	5040001d */ 	beqzl	$v0,.NB0f0f112c
/*  f0f10b8:	8fb90470 */ 	lw	$t9,0x470($sp)
/*  f0f10bc:	90450000 */ 	lbu	$a1,0x0($v0)
/*  f0f10c0:	241000ff */ 	addiu	$s0,$zero,0xff
/*  f0f10c4:	00401825 */ 	or	$v1,$v0,$zero
/*  f0f10c8:	52050018 */ 	beql	$s0,$a1,.NB0f0f112c
/*  f0f10cc:	8fb90470 */ 	lw	$t9,0x470($sp)
/*  f0f10d0:	8e240054 */ 	lw	$a0,0x54($s1)
.NB0f0f10d4:
/*  f0f10d4:	0c006ea3 */ 	jal	modelGetPart
/*  f0f10d8:	afa302b4 */ 	sw	$v1,0x2b4($sp)
/*  f0f10dc:	8fa302b4 */ 	lw	$v1,0x2b4($sp)
/*  f0f10e0:	1040000d */ 	beqz	$v0,.NB0f0f1118
/*  f0f10e4:	00402825 */ 	or	$a1,$v0,$zero
/*  f0f10e8:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f10ec:	0c006bab */ 	jal	modelGetNodeRwData
/*  f0f10f0:	afa302b4 */ 	sw	$v1,0x2b4($sp)
/*  f0f10f4:	10400008 */ 	beqz	$v0,.NB0f0f1118
/*  f0f10f8:	8fa302b4 */ 	lw	$v1,0x2b4($sp)
/*  f0f10fc:	906f0001 */ 	lbu	$t7,0x1($v1)
/*  f0f1100:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f1104:	51e00004 */ 	beqzl	$t7,.NB0f0f1118
/*  f0f1108:	ac400000 */ 	sw	$zero,0x0($v0)
/*  f0f110c:	10000002 */ 	beqz	$zero,.NB0f0f1118
/*  f0f1110:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0f1114:	ac400000 */ 	sw	$zero,0x0($v0)
.NB0f0f1118:
/*  f0f1118:	90650002 */ 	lbu	$a1,0x2($v1)
/*  f0f111c:	24630002 */ 	addiu	$v1,$v1,0x2
/*  f0f1120:	5605ffec */ 	bnel	$s0,$a1,.NB0f0f10d4
/*  f0f1124:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f1128:	8fb90470 */ 	lw	$t9,0x470($sp)
.NB0f0f112c:
/*  f0f112c:	24010003 */ 	addiu	$at,$zero,0x3
/*  f0f1130:	57210009 */ 	bnel	$t9,$at,.NB0f0f1158
/*  f0f1134:	c7a203c8 */ 	lwc1	$f2,0x3c8($sp)
/*  f0f1138:	c62a0510 */ 	lwc1	$f10,0x510($s1)
/*  f0f113c:	e7aa03c0 */ 	swc1	$f10,0x3c0($sp)
/*  f0f1140:	c6280514 */ 	lwc1	$f8,0x514($s1)
/*  f0f1144:	e7a803c4 */ 	swc1	$f8,0x3c4($sp)
/*  f0f1148:	c6260518 */ 	lwc1	$f6,0x518($s1)
/*  f0f114c:	1000000e */ 	beqz	$zero,.NB0f0f1188
/*  f0f1150:	e7a603c8 */ 	swc1	$f6,0x3c8($sp)
/*  f0f1154:	c7a203c8 */ 	lwc1	$f2,0x3c8($sp)
.NB0f0f1158:
/*  f0f1158:	c7a40334 */ 	lwc1	$f4,0x334($sp)
/*  f0f115c:	c7aa03c0 */ 	lwc1	$f10,0x3c0($sp)
/*  f0f1160:	c7a603c4 */ 	lwc1	$f6,0x3c4($sp)
/*  f0f1164:	46022003 */ 	div.s	$f0,$f4,$f2
/*  f0f1168:	460a0202 */ 	mul.s	$f8,$f0,$f10
/*  f0f116c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1170:	46060102 */ 	mul.s	$f4,$f0,$f6
/*  f0f1174:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1178:	46020082 */ 	mul.s	$f2,$f0,$f2
/*  f0f117c:	e7a803c0 */ 	swc1	$f8,0x3c0($sp)
/*  f0f1180:	e7a403c4 */ 	swc1	$f4,0x3c4($sp)
/*  f0f1184:	e7a203c8 */ 	swc1	$f2,0x3c8($sp)
.NB0f0f1188:
/*  f0f1188:	8e2a0010 */ 	lw	$t2,0x10($s1)
/*  f0f118c:	000a5800 */ 	sll	$t3,$t2,0x0
/*  f0f1190:	05610015 */ 	bgez	$t3,.NB0f0f11e8
/*  f0f1194:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0f1198:	25ce3b68 */ 	addiu	$t6,$t6,0x3b68
/*  f0f119c:	8dc10000 */ 	lw	$at,0x0($t6)
/*  f0f11a0:	27b00294 */ 	addiu	$s0,$sp,0x294
/*  f0f11a4:	ae010000 */ 	sw	$at,0x0($s0)
/*  f0f11a8:	8dcc0004 */ 	lw	$t4,0x4($t6)
/*  f0f11ac:	ae0c0004 */ 	sw	$t4,0x4($s0)
/*  f0f11b0:	8dc10008 */ 	lw	$at,0x8($t6)
/*  f0f11b4:	ae010008 */ 	sw	$at,0x8($s0)
/*  f0f11b8:	0c00710f */ 	jal	model0001b3bc
/*  f0f11bc:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f11c0:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f11c4:	0c006f63 */ 	jal	modelGetRootPosition
/*  f0f11c8:	27a502a0 */ 	addiu	$a1,$sp,0x2a0
/*  f0f11cc:	00002025 */ 	or	$a0,$zero,$zero
/*  f0f11d0:	0c005790 */ 	jal	joyGetButtons
/*  f0f11d4:	24050020 */ 	addiu	$a1,$zero,0x20
/*  f0f11d8:	10400003 */ 	beqz	$v0,.NB0f0f11e8
/*  f0f11dc:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f11e0:	0c006f6d */ 	jal	modelSetRootPosition
/*  f0f11e4:	02002825 */ 	or	$a1,$s0,$zero
.NB0f0f11e8:
/*  f0f11e8:	27a403c0 */ 	addiu	$a0,$sp,0x3c0
/*  f0f11ec:	0c005d9b */ 	jal	mtx4LoadTranslation
/*  f0f11f0:	27a50338 */ 	addiu	$a1,$sp,0x338
/*  f0f11f4:	8faf0330 */ 	lw	$t7,0x330($sp)
/*  f0f11f8:	51e0000a */ 	beqzl	$t7,.NB0f0f1224
/*  f0f11fc:	c7ac044c */ 	lwc1	$f12,0x44c($sp)
/*  f0f1200:	c7aa044c */ 	lwc1	$f10,0x44c($sp)
/*  f0f1204:	c7a80320 */ 	lwc1	$f8,0x320($sp)
/*  f0f1208:	27a50338 */ 	addiu	$a1,$sp,0x338
/*  f0f120c:	46085302 */ 	mul.s	$f12,$f10,$f8
/*  f0f1210:	0c005b7d */ 	jal	mtx00015f04
/*  f0f1214:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1218:	10000005 */ 	beqz	$zero,.NB0f0f1230
/*  f0f121c:	8fb80330 */ 	lw	$t8,0x330($sp)
/*  f0f1220:	c7ac044c */ 	lwc1	$f12,0x44c($sp)
.NB0f0f1224:
/*  f0f1224:	0c005b7d */ 	jal	mtx00015f04
/*  f0f1228:	27a50338 */ 	addiu	$a1,$sp,0x338
/*  f0f122c:	8fb80330 */ 	lw	$t8,0x330($sp)
.NB0f0f1230:
/*  f0f1230:	13000005 */ 	beqz	$t8,.NB0f0f1248
/*  f0f1234:	27a40324 */ 	addiu	$a0,$sp,0x324
/*  f0f1238:	0c005d9b */ 	jal	mtx4LoadTranslation
/*  f0f123c:	27a50208 */ 	addiu	$a1,$sp,0x208
/*  f0f1240:	1000000b */ 	beqz	$zero,.NB0f0f1270
/*  f0f1244:	27b00248 */ 	addiu	$s0,$sp,0x248
.NB0f0f1248:
/*  f0f1248:	c626052c */ 	lwc1	$f6,0x52c($s1)
/*  f0f124c:	27a403c0 */ 	addiu	$a0,$sp,0x3c0
/*  f0f1250:	27a50208 */ 	addiu	$a1,$sp,0x208
/*  f0f1254:	e7a603c0 */ 	swc1	$f6,0x3c0($sp)
/*  f0f1258:	c6240530 */ 	lwc1	$f4,0x530($s1)
/*  f0f125c:	e7a403c4 */ 	swc1	$f4,0x3c4($sp)
/*  f0f1260:	c62a0534 */ 	lwc1	$f10,0x534($s1)
/*  f0f1264:	0c005d9b */ 	jal	mtx4LoadTranslation
/*  f0f1268:	e7aa03c8 */ 	swc1	$f10,0x3c8($sp)
/*  f0f126c:	27b00248 */ 	addiu	$s0,$sp,0x248
.NB0f0f1270:
/*  f0f1270:	02003025 */ 	or	$a2,$s0,$zero
/*  f0f1274:	27a40338 */ 	addiu	$a0,$sp,0x338
/*  f0f1278:	0c005a3c */ 	jal	mtx4MultMtx4
/*  f0f127c:	27a50378 */ 	addiu	$a1,$sp,0x378
/*  f0f1280:	8fb90470 */ 	lw	$t9,0x470($sp)
/*  f0f1284:	24010003 */ 	addiu	$at,$zero,0x3
/*  f0f1288:	5721000f */ 	bnel	$t9,$at,.NB0f0f12c8
/*  f0f128c:	26260014 */ 	addiu	$a2,$s1,0x14
/*  f0f1290:	0fc4d643 */ 	jal	credits0f13ae04
/*  f0f1294:	27a401c8 */ 	addiu	$a0,$sp,0x1c8
/*  f0f1298:	27a401c8 */ 	addiu	$a0,$sp,0x1c8
/*  f0f129c:	02002825 */ 	or	$a1,$s0,$zero
/*  f0f12a0:	0c005a3c */ 	jal	mtx4MultMtx4
/*  f0f12a4:	27a60188 */ 	addiu	$a2,$sp,0x188
/*  f0f12a8:	26260014 */ 	addiu	$a2,$s1,0x14
/*  f0f12ac:	afa60044 */ 	sw	$a2,0x44($sp)
/*  f0f12b0:	27a40188 */ 	addiu	$a0,$sp,0x188
/*  f0f12b4:	0c005a3c */ 	jal	mtx4MultMtx4
/*  f0f12b8:	27a50208 */ 	addiu	$a1,$sp,0x208
/*  f0f12bc:	10000006 */ 	beqz	$zero,.NB0f0f12d8
/*  f0f12c0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f12c4:	26260014 */ 	addiu	$a2,$s1,0x14
.NB0f0f12c8:
/*  f0f12c8:	afa60044 */ 	sw	$a2,0x44($sp)
/*  f0f12cc:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f12d0:	0c005a3c */ 	jal	mtx4MultMtx4
/*  f0f12d4:	27a50208 */ 	addiu	$a1,$sp,0x208
.NB0f0f12d8:
/*  f0f12d8:	0fc37df7 */ 	jal	menugfx0f0e2348
/*  f0f12dc:	8fa40468 */ 	lw	$a0,0x468($sp)
/*  f0f12e0:	8faa0470 */ 	lw	$t2,0x470($sp)
/*  f0f12e4:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f12e8:	29410003 */ 	slti	$at,$t2,0x3
/*  f0f12ec:	5020006f */ 	beqzl	$at,.NB0f0f14ac
/*  f0f12f0:	8e2f0054 */ 	lw	$t7,0x54($s1)
/*  f0f12f4:	1140000e */ 	beqz	$t2,.NB0f0f1330
/*  f0f12f8:	3c10800a */ 	lui	$s0,0x800a
/*  f0f12fc:	0fc34852 */ 	jal	func0f0d49c8
/*  f0f1300:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f1304:	244b0008 */ 	addiu	$t3,$v0,0x8
/*  f0f1308:	3c0d0103 */ 	lui	$t5,0x103
/*  f0f130c:	35ad0040 */ 	ori	$t5,$t5,0x40
/*  f0f1310:	afab0468 */ 	sw	$t3,0x468($sp)
/*  f0f1314:	ac4d0000 */ 	sw	$t5,0x0($v0)
/*  f0f1318:	0fc2cb52 */ 	jal	camGetPerspectiveMtxL
/*  f0f131c:	00408025 */ 	or	$s0,$v0,$zero
/*  f0f1320:	0c013100 */ 	jal	osVirtualToPhysical
/*  f0f1324:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f1328:	1000005f */ 	beqz	$zero,.NB0f0f14a8
/*  f0f132c:	ae020004 */ 	sw	$v0,0x4($s0)
.NB0f0f1330:
/*  f0f1330:	26106528 */ 	addiu	$s0,$s0,0x6528
/*  f0f1334:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f1338:	3c18800a */ 	lui	$t8,0x800a
/*  f0f133c:	3c19800a */ 	lui	$t9,0x800a
/*  f0f1340:	8f396530 */ 	lw	$t9,0x6530($t9)
/*  f0f1344:	8f186534 */ 	lw	$t8,0x6534($t8)
/*  f0f1348:	8dce652c */ 	lw	$t6,0x652c($t6)
/*  f0f134c:	8e0c0000 */ 	lw	$t4,0x0($s0)
/*  f0f1350:	03195023 */ 	subu	$t2,$t8,$t9
/*  f0f1354:	448a2000 */ 	mtc1	$t2,$f4
/*  f0f1358:	01cc7823 */ 	subu	$t7,$t6,$t4
/*  f0f135c:	448f4000 */ 	mtc1	$t7,$f8
/*  f0f1360:	468022a0 */ 	cvt.s.w	$f10,$f4
/*  f0f1364:	3c047f1b */ 	lui	$a0,0x7f1b
/*  f0f1368:	3c058007 */ 	lui	$a1,0x8007
/*  f0f136c:	24a53b74 */ 	addiu	$a1,$a1,0x3b74
/*  f0f1370:	2484c91c */ 	addiu	$a0,$a0,-14052
/*  f0f1374:	468041a0 */ 	cvt.s.w	$f6,$f8
/*  f0f1378:	460a3203 */ 	div.s	$f8,$f6,$f10
/*  f0f137c:	0c00381c */ 	jal	mainOverrideVariable
/*  f0f1380:	e7a80180 */ 	swc1	$f8,0x180($sp)
/*  f0f1384:	3c047f1b */ 	lui	$a0,0x7f1b
/*  f0f1388:	3c058007 */ 	lui	$a1,0x8007
/*  f0f138c:	24a53b78 */ 	addiu	$a1,$a1,0x3b78
/*  f0f1390:	0c00381c */ 	jal	mainOverrideVariable
/*  f0f1394:	2484c920 */ 	addiu	$a0,$a0,-14048
/*  f0f1398:	0fc34852 */ 	jal	func0f0d49c8
/*  f0f139c:	8fa40468 */ 	lw	$a0,0x468($sp)
/*  f0f13a0:	3c0d8008 */ 	lui	$t5,0x8008
/*  f0f13a4:	8dad2320 */ 	lw	$t5,0x2320($t5)
/*  f0f13a8:	8e0b0000 */ 	lw	$t3,0x0($s0)
/*  f0f13ac:	3c05800a */ 	lui	$a1,0x800a
/*  f0f13b0:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f13b4:	016d0019 */ 	multu	$t3,$t5
/*  f0f13b8:	84a56532 */ 	lh	$a1,0x6532($a1)
/*  f0f13bc:	00002012 */ 	mflo	$a0
/*  f0f13c0:	00047400 */ 	sll	$t6,$a0,0x10
/*  f0f13c4:	0c002f9f */ 	jal	viSetViewPosition
/*  f0f13c8:	000e2403 */ 	sra	$a0,$t6,0x10
/*  f0f13cc:	3c18800a */ 	lui	$t8,0x800a
/*  f0f13d0:	8f18652c */ 	lw	$t8,0x652c($t8)
/*  f0f13d4:	8e190000 */ 	lw	$t9,0x0($s0)
/*  f0f13d8:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0f13dc:	8d6b2320 */ 	lw	$t3,0x2320($t3)
/*  f0f13e0:	03195023 */ 	subu	$t2,$t8,$t9
/*  f0f13e4:	3c0f800a */ 	lui	$t7,0x800a
/*  f0f13e8:	014b0019 */ 	multu	$t2,$t3
/*  f0f13ec:	8defe944 */ 	lw	$t7,-0x16bc($t7)
/*  f0f13f0:	3c0c800a */ 	lui	$t4,0x800a
/*  f0f13f4:	8d8c6534 */ 	lw	$t4,0x6534($t4)
/*  f0f13f8:	c5ec1854 */ 	lwc1	$f12,0x1854($t7)
/*  f0f13fc:	3c0f800a */ 	lui	$t7,0x800a
/*  f0f1400:	8def6530 */ 	lw	$t7,0x6530($t7)
/*  f0f1404:	c7ae0180 */ 	lwc1	$f14,0x180($sp)
/*  f0f1408:	018f3823 */ 	subu	$a3,$t4,$t7
/*  f0f140c:	00003012 */ 	mflo	$a2
/*  f0f1410:	00066c00 */ 	sll	$t5,$a2,0x10
/*  f0f1414:	0007c400 */ 	sll	$t8,$a3,0x10
/*  f0f1418:	00183c03 */ 	sra	$a3,$t8,0x10
/*  f0f141c:	0c002feb */ 	jal	viSetFovAspectAndSize
/*  f0f1420:	000d3403 */ 	sra	$a2,$t5,0x10
/*  f0f1424:	3c0a8007 */ 	lui	$t2,0x8007
/*  f0f1428:	8d4a3af0 */ 	lw	$t2,0x3af0($t2)
/*  f0f142c:	3c0d800a */ 	lui	$t5,0x800a
/*  f0f1430:	25ad6538 */ 	addiu	$t5,$t5,0x6538
/*  f0f1434:	000a5940 */ 	sll	$t3,$t2,0x5
/*  f0f1438:	016d2821 */ 	addu	$a1,$t3,$t5
/*  f0f143c:	0c002c20 */ 	jal	vi0000af00
/*  f0f1440:	8fa40468 */ 	lw	$a0,0x468($sp)
/*  f0f1444:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0f1448:	8dce3b74 */ 	lw	$t6,0x3b74($t6)
/*  f0f144c:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f1450:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f1454:	448e2000 */ 	mtc1	$t6,$f4
/*  f0f1458:	3c0c8007 */ 	lui	$t4,0x8007
/*  f0f145c:	05c10005 */ 	bgez	$t6,.NB0f0f1474
/*  f0f1460:	46802120 */ 	cvt.s.w	$f4,$f4
/*  f0f1464:	3c014f80 */ 	lui	$at,0x4f80
/*  f0f1468:	44813000 */ 	mtc1	$at,$f6
/*  f0f146c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1470:	46062100 */ 	add.s	$f4,$f4,$f6
.NB0f0f1474:
/*  f0f1474:	8d8c3b78 */ 	lw	$t4,0x3b78($t4)
/*  f0f1478:	44052000 */ 	mfc1	$a1,$f4
/*  f0f147c:	3c014f80 */ 	lui	$at,0x4f80
/*  f0f1480:	448c5000 */ 	mtc1	$t4,$f10
/*  f0f1484:	05810004 */ 	bgez	$t4,.NB0f0f1498
/*  f0f1488:	468052a0 */ 	cvt.s.w	$f10,$f10
/*  f0f148c:	44814000 */ 	mtc1	$at,$f8
/*  f0f1490:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1494:	46085280 */ 	add.s	$f10,$f10,$f8
.NB0f0f1498:
/*  f0f1498:	44065000 */ 	mfc1	$a2,$f10
/*  f0f149c:	0c002b89 */ 	jal	vi0000aca4
/*  f0f14a0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f14a4:	afa20468 */ 	sw	$v0,0x468($sp)
.NB0f0f14a8:
/*  f0f14a8:	8e2f0054 */ 	lw	$t7,0x54($s1)
.NB0f0f14ac:
/*  f0f14ac:	85e4000e */ 	lh	$a0,0xe($t7)
/*  f0f14b0:	0004c180 */ 	sll	$t8,$a0,0x6
/*  f0f14b4:	0fc588cd */ 	jal	gfxAllocate
/*  f0f14b8:	03002025 */ 	or	$a0,$t8,$zero
/*  f0f14bc:	afa203dc */ 	sw	$v0,0x3dc($sp)
/*  f0f14c0:	8e390054 */ 	lw	$t9,0x54($s1)
/*  f0f14c4:	00008025 */ 	or	$s0,$zero,$zero
/*  f0f14c8:	872a000e */ 	lh	$t2,0xe($t9)
/*  f0f14cc:	5940000d */ 	blezl	$t2,.NB0f0f1504
/*  f0f14d0:	8625005c */ 	lh	$a1,0x5c($s1)
/*  f0f14d4:	8fad03dc */ 	lw	$t5,0x3dc($sp)
.NB0f0f14d8:
/*  f0f14d8:	00105980 */ 	sll	$t3,$s0,0x6
/*  f0f14dc:	0c005a28 */ 	jal	mtx4LoadIdentity
/*  f0f14e0:	016d2021 */ 	addu	$a0,$t3,$t5
/*  f0f14e4:	8e2e0054 */ 	lw	$t6,0x54($s1)
/*  f0f14e8:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0f14ec:	85cc000e */ 	lh	$t4,0xe($t6)
/*  f0f14f0:	020c082a */ 	slt	$at,$s0,$t4
/*  f0f14f4:	5420fff8 */ 	bnezl	$at,.NB0f0f14d8
/*  f0f14f8:	8fad03dc */ 	lw	$t5,0x3dc($sp)
/*  f0f14fc:	00008025 */ 	or	$s0,$zero,$zero
/*  f0f1500:	8625005c */ 	lh	$a1,0x5c($s1)
.NB0f0f1504:
/*  f0f1504:	8faf03dc */ 	lw	$t7,0x3dc($sp)
/*  f0f1508:	10a00025 */ 	beqz	$a1,.NB0f0f15a0
/*  f0f150c:	ae2f006c */ 	sw	$t7,0x6c($s1)
/*  f0f1510:	8638005e */ 	lh	$t8,0x5e($s1)
/*  f0f1514:	50b80023 */ 	beql	$a1,$t8,.NB0f0f15a4
/*  f0f1518:	862e005e */ 	lh	$t6,0x5e($s1)
/*  f0f151c:	8e3905b0 */ 	lw	$t9,0x5b0($s1)
/*  f0f1520:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f1524:	00003025 */ 	or	$a2,$zero,$zero
/*  f0f1528:	00195ac0 */ 	sll	$t3,$t9,0xb
/*  f0f152c:	05610014 */ 	bgez	$t3,.NB0f0f1580
/*  f0f1530:	3c013f00 */ 	lui	$at,0x3f00
/*  f0f1534:	3c01bf00 */ 	lui	$at,0xbf00
/*  f0f1538:	44812000 */ 	mtc1	$at,$f4
/*  f0f153c:	44803000 */ 	mtc1	$zero,$f6
/*  f0f1540:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f1544:	00003025 */ 	or	$a2,$zero,$zero
/*  f0f1548:	24070000 */ 	addiu	$a3,$zero,0x0
/*  f0f154c:	e7a40010 */ 	swc1	$f4,0x10($sp)
/*  f0f1550:	0c007b53 */ 	jal	modelSetAnimation
/*  f0f1554:	e7a60014 */ 	swc1	$f6,0x14($sp)
/*  f0f1558:	0c0078a6 */ 	jal	modelGetNumAnimFrames
/*  f0f155c:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f1560:	44825000 */ 	mtc1	$v0,$f10
/*  f0f1564:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f1568:	468052a0 */ 	cvt.s.w	$f10,$f10
/*  f0f156c:	44055000 */ 	mfc1	$a1,$f10
/*  f0f1570:	0c007c2c */ 	jal	model0001e018
/*  f0f1574:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1578:	10000008 */ 	beqz	$zero,.NB0f0f159c
/*  f0f157c:	862d005c */ 	lh	$t5,0x5c($s1)
.NB0f0f1580:
/*  f0f1580:	44814000 */ 	mtc1	$at,$f8
/*  f0f1584:	44802000 */ 	mtc1	$zero,$f4
/*  f0f1588:	24070000 */ 	addiu	$a3,$zero,0x0
/*  f0f158c:	e7a80010 */ 	swc1	$f8,0x10($sp)
/*  f0f1590:	0c007b53 */ 	jal	modelSetAnimation
/*  f0f1594:	e7a40014 */ 	swc1	$f4,0x14($sp)
/*  f0f1598:	862d005c */ 	lh	$t5,0x5c($s1)
.NB0f0f159c:
/*  f0f159c:	a62d005e */ 	sh	$t5,0x5e($s1)
.NB0f0f15a0:
/*  f0f15a0:	862e005e */ 	lh	$t6,0x5e($s1)
.NB0f0f15a4:
/*  f0f15a4:	a620005c */ 	sh	$zero,0x5c($s1)
/*  f0f15a8:	11c00023 */ 	beqz	$t6,.NB0f0f1638
/*  f0f15ac:	3c05800a */ 	lui	$a1,0x800a
/*  f0f15b0:	8ca5e700 */ 	lw	$a1,-0x1900($a1)
/*  f0f15b4:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f15b8:	0c007fac */ 	jal	model0001ee18
/*  f0f15bc:	24060001 */ 	addiu	$a2,$zero,0x1
/*  f0f15c0:	8e2c05b0 */ 	lw	$t4,0x5b0($s1)
/*  f0f15c4:	000cc2c0 */ 	sll	$t8,$t4,0xb
/*  f0f15c8:	0701000c */ 	bgez	$t8,.NB0f0f15fc
/*  f0f15cc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f15d0:	0c0078a6 */ 	jal	modelGetNumAnimFrames
/*  f0f15d4:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f15d8:	afa20050 */ 	sw	$v0,0x50($sp)
/*  f0f15dc:	0c00787f */ 	jal	modelGetCurAnimFrame
/*  f0f15e0:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f15e4:	8fb90050 */ 	lw	$t9,0x50($sp)
/*  f0f15e8:	44993000 */ 	mtc1	$t9,$f6
/*  f0f15ec:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f15f0:	468032a0 */ 	cvt.s.w	$f10,$f6
/*  f0f15f4:	10000004 */ 	beqz	$zero,.NB0f0f1608
/*  f0f15f8:	46005081 */ 	sub.s	$f2,$f10,$f0
.NB0f0f15fc:
/*  f0f15fc:	0c00787f */ 	jal	modelGetCurAnimFrame
/*  f0f1600:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f1604:	46000086 */ 	mov.s	$f2,$f0
.NB0f0f1608:
/*  f0f1608:	8fa40040 */ 	lw	$a0,0x40($sp)
/*  f0f160c:	0c0078a6 */ 	jal	modelGetNumAnimFrames
/*  f0f1610:	e7a2017c */ 	swc1	$f2,0x17c($sp)
/*  f0f1614:	244affff */ 	addiu	$t2,$v0,-1
/*  f0f1618:	448a4000 */ 	mtc1	$t2,$f8
/*  f0f161c:	c7a2017c */ 	lwc1	$f2,0x17c($sp)
/*  f0f1620:	46804120 */ 	cvt.s.w	$f4,$f8
/*  f0f1624:	4602203e */ 	c.le.s	$f4,$f2
/*  f0f1628:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f162c:	45020003 */ 	bc1fl	.NB0f0f163c
/*  f0f1630:	8fa40044 */ 	lw	$a0,0x44($sp)
/*  f0f1634:	a620005e */ 	sh	$zero,0x5e($s1)
.NB0f0f1638:
/*  f0f1638:	8fa40044 */ 	lw	$a0,0x44($sp)
.NB0f0f163c:
/*  f0f163c:	0c005b02 */ 	jal	mtx4Copy
/*  f0f1640:	8fa503dc */ 	lw	$a1,0x3dc($sp)
/*  f0f1644:	8fab0044 */ 	lw	$t3,0x44($sp)
/*  f0f1648:	27a403e0 */ 	addiu	$a0,$sp,0x3e0
/*  f0f164c:	8fa50040 */ 	lw	$a1,0x40($sp)
/*  f0f1650:	afab03e0 */ 	sw	$t3,0x3e0($sp)
/*  f0f1654:	8e2d006c */ 	lw	$t5,0x6c($s1)
/*  f0f1658:	0c0077cf */ 	jal	model0001cebc
/*  f0f165c:	afad03f0 */ 	sw	$t5,0x3f0($sp)
/*  f0f1660:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f1664:	3c0e8008 */ 	lui	$t6,0x8008
/*  f0f1668:	25cef494 */ 	addiu	$t6,$t6,-2924
/*  f0f166c:	8c8c0004 */ 	lw	$t4,0x4($a0)
/*  f0f1670:	55cc0092 */ 	bnel	$t6,$t4,.NB0f0f18bc
/*  f0f1674:	8fae0468 */ 	lw	$t6,0x468($sp)
/*  f0f1678:	0c006ea3 */ 	jal	modelGetPart
/*  f0f167c:	00002825 */ 	or	$a1,$zero,$zero
/*  f0f1680:	5040002e */ 	beqzl	$v0,.NB0f0f173c
/*  f0f1684:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f1688:	8c450004 */ 	lw	$a1,0x4($v0)
/*  f0f168c:	3c09800a */ 	lui	$t1,0x800a
/*  f0f1690:	2529e6c0 */ 	addiu	$t1,$t1,-6464
/*  f0f1694:	84af0010 */ 	lh	$t7,0x10($a1)
/*  f0f1698:	00003825 */ 	or	$a3,$zero,$zero
/*  f0f169c:	00003025 */ 	or	$a2,$zero,$zero
/*  f0f16a0:	19e00025 */ 	blez	$t7,.NB0f0f1738
/*  f0f16a4:	24080064 */ 	addiu	$t0,$zero,0x64
/*  f0f16a8:	8d2a0000 */ 	lw	$t2,0x0($t1)
.NB0f0f16ac:
/*  f0f16ac:	8cb8000c */ 	lw	$t8,0xc($a1)
/*  f0f16b0:	01480019 */ 	multu	$t2,$t0
/*  f0f16b4:	03061021 */ 	addu	$v0,$t8,$a2
/*  f0f16b8:	84590008 */ 	lh	$t9,0x8($v0)
/*  f0f16bc:	00005812 */ 	mflo	$t3
/*  f0f16c0:	032b6823 */ 	subu	$t5,$t9,$t3
/*  f0f16c4:	a44d0008 */ 	sh	$t5,0x8($v0)
/*  f0f16c8:	8cae000c */ 	lw	$t6,0xc($a1)
/*  f0f16cc:	01c66021 */ 	addu	$t4,$t6,$a2
/*  f0f16d0:	858f0008 */ 	lh	$t7,0x8($t4)
/*  f0f16d4:	29e1a000 */ 	slti	$at,$t7,-24576
/*  f0f16d8:	50200012 */ 	beqzl	$at,.NB0f0f1724
/*  f0f16dc:	84ae0010 */ 	lh	$t6,0x10($a1)
/*  f0f16e0:	84b80010 */ 	lh	$t8,0x10($a1)
/*  f0f16e4:	00001825 */ 	or	$v1,$zero,$zero
/*  f0f16e8:	00002025 */ 	or	$a0,$zero,$zero
/*  f0f16ec:	5b00000d */ 	blezl	$t8,.NB0f0f1724
/*  f0f16f0:	84ae0010 */ 	lh	$t6,0x10($a1)
/*  f0f16f4:	8caa000c */ 	lw	$t2,0xc($a1)
.NB0f0f16f8:
/*  f0f16f8:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0f16fc:	01441021 */ 	addu	$v0,$t2,$a0
/*  f0f1700:	84590008 */ 	lh	$t9,0x8($v0)
/*  f0f1704:	2484000c */ 	addiu	$a0,$a0,0xc
/*  f0f1708:	272b2000 */ 	addiu	$t3,$t9,0x2000
/*  f0f170c:	a44b0008 */ 	sh	$t3,0x8($v0)
/*  f0f1710:	84ad0010 */ 	lh	$t5,0x10($a1)
/*  f0f1714:	006d082a */ 	slt	$at,$v1,$t5
/*  f0f1718:	5420fff7 */ 	bnezl	$at,.NB0f0f16f8
/*  f0f171c:	8caa000c */ 	lw	$t2,0xc($a1)
/*  f0f1720:	84ae0010 */ 	lh	$t6,0x10($a1)
.NB0f0f1724:
/*  f0f1724:	24e70001 */ 	addiu	$a3,$a3,0x1
/*  f0f1728:	24c6000c */ 	addiu	$a2,$a2,0xc
/*  f0f172c:	00ee082a */ 	slt	$at,$a3,$t6
/*  f0f1730:	5420ffde */ 	bnezl	$at,.NB0f0f16ac
/*  f0f1734:	8d2a0000 */ 	lw	$t2,0x0($t1)
.NB0f0f1738:
/*  f0f1738:	8e240054 */ 	lw	$a0,0x54($s1)
.NB0f0f173c:
/*  f0f173c:	0c006ea3 */ 	jal	modelGetPart
/*  f0f1740:	24050002 */ 	addiu	$a1,$zero,0x2
/*  f0f1744:	10400019 */ 	beqz	$v0,.NB0f0f17ac
/*  f0f1748:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f174c:	0c006da5 */ 	jal	model0001a524
/*  f0f1750:	00002825 */ 	or	$a1,$zero,$zero
/*  f0f1754:	afa20164 */ 	sw	$v0,0x164($sp)
/*  f0f1758:	0c005a28 */ 	jal	mtx4LoadIdentity
/*  f0f175c:	27a40124 */ 	addiu	$a0,$sp,0x124
/*  f0f1760:	3c014080 */ 	lui	$at,0x4080
/*  f0f1764:	44816000 */ 	mtc1	$at,$f12
/*  f0f1768:	0fc019fe */ 	jal	func0f006b54
/*  f0f176c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1770:	46000306 */ 	mov.s	$f12,$f0
/*  f0f1774:	0c005c9e */ 	jal	mtx4LoadXRotation
/*  f0f1778:	27a50124 */ 	addiu	$a1,$sp,0x124
/*  f0f177c:	8faf0164 */ 	lw	$t7,0x164($sp)
/*  f0f1780:	8fac03dc */ 	lw	$t4,0x3dc($sp)
/*  f0f1784:	27a50124 */ 	addiu	$a1,$sp,0x124
/*  f0f1788:	000fc180 */ 	sll	$t8,$t7,0x6
/*  f0f178c:	01983821 */ 	addu	$a3,$t4,$t8
/*  f0f1790:	00e02025 */ 	or	$a0,$a3,$zero
/*  f0f1794:	afa70044 */ 	sw	$a3,0x44($sp)
/*  f0f1798:	0c005a3c */ 	jal	mtx4MultMtx4
/*  f0f179c:	27a600e4 */ 	addiu	$a2,$sp,0xe4
/*  f0f17a0:	8fa50044 */ 	lw	$a1,0x44($sp)
/*  f0f17a4:	0c005b02 */ 	jal	mtx4Copy
/*  f0f17a8:	27a400e4 */ 	addiu	$a0,$sp,0xe4
.NB0f0f17ac:
/*  f0f17ac:	8e240054 */ 	lw	$a0,0x54($s1)
/*  f0f17b0:	0c006ea3 */ 	jal	modelGetPart
/*  f0f17b4:	24050001 */ 	addiu	$a1,$zero,0x1
/*  f0f17b8:	1040003f */ 	beqz	$v0,.NB0f0f18b8
/*  f0f17bc:	00402025 */ 	or	$a0,$v0,$zero
/*  f0f17c0:	3c02800a */ 	lui	$v0,0x800a
/*  f0f17c4:	8c425eb4 */ 	lw	$v0,0x5eb4($v0)
/*  f0f17c8:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0f17cc:	10410007 */ 	beq	$v0,$at,.NB0f0f17ec
/*  f0f17d0:	24010006 */ 	addiu	$at,$zero,0x6
/*  f0f17d4:	10410005 */ 	beq	$v0,$at,.NB0f0f17ec
/*  f0f17d8:	24010003 */ 	addiu	$at,$zero,0x3
/*  f0f17dc:	10410003 */ 	beq	$v0,$at,.NB0f0f17ec
/*  f0f17e0:	2401000d */ 	addiu	$at,$zero,0xd
/*  f0f17e4:	54410035 */ 	bnel	$v0,$at,.NB0f0f18bc
/*  f0f17e8:	8fae0468 */ 	lw	$t6,0x468($sp)
.NB0f0f17ec:
/*  f0f17ec:	0c006da5 */ 	jal	model0001a524
/*  f0f17f0:	00002825 */ 	or	$a1,$zero,$zero
/*  f0f17f4:	8faa03dc */ 	lw	$t2,0x3dc($sp)
/*  f0f17f8:	0002c980 */ 	sll	$t9,$v0,0x6
/*  f0f17fc:	27a400d4 */ 	addiu	$a0,$sp,0xd4
/*  f0f1800:	01591821 */ 	addu	$v1,$t2,$t9
/*  f0f1804:	c4660030 */ 	lwc1	$f6,0x30($v1)
/*  f0f1808:	27a500cc */ 	addiu	$a1,$sp,0xcc
/*  f0f180c:	e7a600d4 */ 	swc1	$f6,0xd4($sp)
/*  f0f1810:	c46a0034 */ 	lwc1	$f10,0x34($v1)
/*  f0f1814:	e7aa00d8 */ 	swc1	$f10,0xd8($sp)
/*  f0f1818:	c4680038 */ 	lwc1	$f8,0x38($v1)
/*  f0f181c:	0fc2ca99 */ 	jal	cam0f0b4d04
/*  f0f1820:	e7a800dc */ 	swc1	$f8,0xdc($sp)
/*  f0f1824:	0c002f77 */ 	jal	viGetWidth
/*  f0f1828:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f182c:	c7a400cc */ 	lwc1	$f4,0xcc($sp)
/*  f0f1830:	3c0c8008 */ 	lui	$t4,0x8008
/*  f0f1834:	8d8c2320 */ 	lw	$t4,0x2320($t4)
/*  f0f1838:	4600218d */ 	trunc.w.s	$f6,$f4
/*  f0f183c:	440d3000 */ 	mfc1	$t5,$f6
/*  f0f1840:	04410003 */ 	bgez	$v0,.NB0f0f1850
/*  f0f1844:	00027043 */ 	sra	$t6,$v0,0x1
/*  f0f1848:	24410001 */ 	addiu	$at,$v0,0x1
/*  f0f184c:	00017043 */ 	sra	$t6,$at,0x1
.NB0f0f1850:
/*  f0f1850:	01ae7823 */ 	subu	$t7,$t5,$t6
/*  f0f1854:	01ec001a */ 	div	$zero,$t7,$t4
/*  f0f1858:	15800002 */ 	bnez	$t4,.NB0f0f1864
/*  f0f185c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1860:	0007000d */ 	break	0x7
.NB0f0f1864:
/*  f0f1864:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f1868:	15810004 */ 	bne	$t4,$at,.NB0f0f187c
/*  f0f186c:	3c018000 */ 	lui	$at,0x8000
/*  f0f1870:	15e10002 */ 	bne	$t7,$at,.NB0f0f187c
/*  f0f1874:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1878:	0006000d */ 	break	0x6
.NB0f0f187c:
/*  f0f187c:	3c01800a */ 	lui	$at,0x800a
/*  f0f1880:	0000c012 */ 	mflo	$t8
/*  f0f1884:	ac382688 */ 	sw	$t8,0x2688($at)
/*  f0f1888:	0c002f7b */ 	jal	viGetHeight
/*  f0f188c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1890:	c7aa00d0 */ 	lwc1	$f10,0xd0($sp)
/*  f0f1894:	4600520d */ 	trunc.w.s	$f8,$f10
/*  f0f1898:	44194000 */ 	mfc1	$t9,$f8
/*  f0f189c:	04410003 */ 	bgez	$v0,.NB0f0f18ac
/*  f0f18a0:	00025843 */ 	sra	$t3,$v0,0x1
/*  f0f18a4:	24410001 */ 	addiu	$at,$v0,0x1
/*  f0f18a8:	00015843 */ 	sra	$t3,$at,0x1
.NB0f0f18ac:
/*  f0f18ac:	3c01800a */ 	lui	$at,0x800a
/*  f0f18b0:	032b6823 */ 	subu	$t5,$t9,$t3
/*  f0f18b4:	ac2d268c */ 	sw	$t5,0x268c($at)
.NB0f0f18b8:
/*  f0f18b8:	8fae0468 */ 	lw	$t6,0x468($sp)
.NB0f0f18bc:
/*  f0f18bc:	3c0cbc00 */ 	lui	$t4,0xbc00
/*  f0f18c0:	3c188000 */ 	lui	$t8,0x8000
/*  f0f18c4:	25cf0008 */ 	addiu	$t7,$t6,0x8
/*  f0f18c8:	afaf0468 */ 	sw	$t7,0x468($sp)
/*  f0f18cc:	37180040 */ 	ori	$t8,$t8,0x40
/*  f0f18d0:	358c0002 */ 	ori	$t4,$t4,0x2
/*  f0f18d4:	adcc0000 */ 	sw	$t4,0x0($t6)
/*  f0f18d8:	add80004 */ 	sw	$t8,0x4($t6)
/*  f0f18dc:	8faa0468 */ 	lw	$t2,0x468($sp)
/*  f0f18e0:	3c0b0386 */ 	lui	$t3,0x386
/*  f0f18e4:	3c0d8007 */ 	lui	$t5,0x8007
/*  f0f18e8:	25590008 */ 	addiu	$t9,$t2,0x8
/*  f0f18ec:	afb90468 */ 	sw	$t9,0x468($sp)
/*  f0f18f0:	25ad3b18 */ 	addiu	$t5,$t5,0x3b18
/*  f0f18f4:	356b0010 */ 	ori	$t3,$t3,0x10
/*  f0f18f8:	ad4b0000 */ 	sw	$t3,0x0($t2)
/*  f0f18fc:	ad4d0004 */ 	sw	$t5,0x4($t2)
/*  f0f1900:	8fae0468 */ 	lw	$t6,0x468($sp)
/*  f0f1904:	3c188007 */ 	lui	$t8,0x8007
/*  f0f1908:	3c0c0388 */ 	lui	$t4,0x388
/*  f0f190c:	25cf0008 */ 	addiu	$t7,$t6,0x8
/*  f0f1910:	afaf0468 */ 	sw	$t7,0x468($sp)
/*  f0f1914:	358c0010 */ 	ori	$t4,$t4,0x10
/*  f0f1918:	27183b10 */ 	addiu	$t8,$t8,0x3b10
/*  f0f191c:	add80004 */ 	sw	$t8,0x4($t6)
/*  f0f1920:	adcc0000 */ 	sw	$t4,0x0($t6)
/*  f0f1924:	8faa0468 */ 	lw	$t2,0x468($sp)
/*  f0f1928:	3c0b0384 */ 	lui	$t3,0x384
/*  f0f192c:	356b0010 */ 	ori	$t3,$t3,0x10
/*  f0f1930:	25590008 */ 	addiu	$t9,$t2,0x8
/*  f0f1934:	afb90468 */ 	sw	$t9,0x468($sp)
/*  f0f1938:	ad4b0000 */ 	sw	$t3,0x0($t2)
/*  f0f193c:	0fc2cd42 */ 	jal	camGetLookAt
/*  f0f1940:	afaa00bc */ 	sw	$t2,0xbc($sp)
/*  f0f1944:	8fa500bc */ 	lw	$a1,0xbc($sp)
/*  f0f1948:	3c0f0382 */ 	lui	$t7,0x382
/*  f0f194c:	35ef0010 */ 	ori	$t7,$t7,0x10
/*  f0f1950:	aca20004 */ 	sw	$v0,0x4($a1)
/*  f0f1954:	8fad0468 */ 	lw	$t5,0x468($sp)
/*  f0f1958:	25ae0008 */ 	addiu	$t6,$t5,0x8
/*  f0f195c:	afae0468 */ 	sw	$t6,0x468($sp)
/*  f0f1960:	adaf0000 */ 	sw	$t7,0x0($t5)
/*  f0f1964:	0fc2cd42 */ 	jal	camGetLookAt
/*  f0f1968:	afad00b8 */ 	sw	$t5,0xb8($sp)
/*  f0f196c:	8fa600b8 */ 	lw	$a2,0xb8($sp)
/*  f0f1970:	244c0010 */ 	addiu	$t4,$v0,0x10
/*  f0f1974:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f1978:	accc0004 */ 	sw	$t4,0x4($a2)
/*  f0f197c:	8fab0468 */ 	lw	$t3,0x468($sp)
/*  f0f1980:	240affff */ 	addiu	$t2,$zero,-1
/*  f0f1984:	2419ffff */ 	addiu	$t9,$zero,-1
/*  f0f1988:	256d0008 */ 	addiu	$t5,$t3,0x8
/*  f0f198c:	afb80410 */ 	sw	$t8,0x410($sp)
/*  f0f1990:	afaa0414 */ 	sw	$t2,0x414($sp)
/*  f0f1994:	afb90418 */ 	sw	$t9,0x418($sp)
/*  f0f1998:	afad0468 */ 	sw	$t5,0x468($sp)
/*  f0f199c:	3c0eb700 */ 	lui	$t6,0xb700
/*  f0f19a0:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f19a4:	ad6f0004 */ 	sw	$t7,0x4($t3)
/*  f0f19a8:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f19ac:	8fac0468 */ 	lw	$t4,0x468($sp)
/*  f0f19b0:	afb803e4 */ 	sw	$t8,0x3e4($sp)
/*  f0f19b4:	8fa50040 */ 	lw	$a1,0x40($sp)
/*  f0f19b8:	27a403e0 */ 	addiu	$a0,$sp,0x3e0
/*  f0f19bc:	0c008be3 */ 	jal	modelRender
/*  f0f19c0:	afac03ec */ 	sw	$t4,0x3ec($sp)
/*  f0f19c4:	8faa03ec */ 	lw	$t2,0x3ec($sp)
/*  f0f19c8:	0c005dbc */ 	jal	mtx00016760
/*  f0f19cc:	afaa0468 */ 	sw	$t2,0x468($sp)
/*  f0f19d0:	8e390054 */ 	lw	$t9,0x54($s1)
/*  f0f19d4:	00001025 */ 	or	$v0,$zero,$zero
/*  f0f19d8:	872b000e */ 	lh	$t3,0xe($t9)
/*  f0f19dc:	19600013 */ 	blez	$t3,.NB0f0f1a2c
/*  f0f19e0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f19e4:	8e2d006c */ 	lw	$t5,0x6c($s1)
.NB0f0f19e8:
/*  f0f19e8:	afa20044 */ 	sw	$v0,0x44($sp)
/*  f0f19ec:	27a50074 */ 	addiu	$a1,$sp,0x74
/*  f0f19f0:	0c005b02 */ 	jal	mtx4Copy
/*  f0f19f4:	01a22021 */ 	addu	$a0,$t5,$v0
/*  f0f19f8:	8e2e006c */ 	lw	$t6,0x6c($s1)
/*  f0f19fc:	00107980 */ 	sll	$t7,$s0,0x6
/*  f0f1a00:	27a40074 */ 	addiu	$a0,$sp,0x74
/*  f0f1a04:	0c005bd1 */ 	jal	mtx00016054
/*  f0f1a08:	01cf2821 */ 	addu	$a1,$t6,$t7
/*  f0f1a0c:	8e2c0054 */ 	lw	$t4,0x54($s1)
/*  f0f1a10:	8fa20044 */ 	lw	$v0,0x44($sp)
/*  f0f1a14:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0f1a18:	8598000e */ 	lh	$t8,0xe($t4)
/*  f0f1a1c:	24420040 */ 	addiu	$v0,$v0,0x40
/*  f0f1a20:	0218082a */ 	slt	$at,$s0,$t8
/*  f0f1a24:	5420fff0 */ 	bnezl	$at,.NB0f0f19e8
/*  f0f1a28:	8e2d006c */ 	lw	$t5,0x6c($s1)
.NB0f0f1a2c:
/*  f0f1a2c:	0c005dc5 */ 	jal	mtx00016784
/*  f0f1a30:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f1a34:	8faa0470 */ 	lw	$t2,0x470($sp)
/*  f0f1a38:	29410003 */ 	slti	$at,$t2,0x3
/*  f0f1a3c:	50200005 */ 	beqzl	$at,.NB0f0f1a54
/*  f0f1a40:	8fb90468 */ 	lw	$t9,0x468($sp)
/*  f0f1a44:	0fc347cf */ 	jal	func0f0d479c
/*  f0f1a48:	8fa40468 */ 	lw	$a0,0x468($sp)
/*  f0f1a4c:	afa20468 */ 	sw	$v0,0x468($sp)
/*  f0f1a50:	8fb90468 */ 	lw	$t9,0x468($sp)
.NB0f0f1a54:
/*  f0f1a54:	3c0de700 */ 	lui	$t5,0xe700
/*  f0f1a58:	3c0cba00 */ 	lui	$t4,0xba00
/*  f0f1a5c:	272b0008 */ 	addiu	$t3,$t9,0x8
/*  f0f1a60:	afab0468 */ 	sw	$t3,0x468($sp)
/*  f0f1a64:	af200004 */ 	sw	$zero,0x4($t9)
/*  f0f1a68:	af2d0000 */ 	sw	$t5,0x0($t9)
/*  f0f1a6c:	8fae0468 */ 	lw	$t6,0x468($sp)
/*  f0f1a70:	358c1402 */ 	ori	$t4,$t4,0x1402
/*  f0f1a74:	3c19b900 */ 	lui	$t9,0xb900
/*  f0f1a78:	25cf0008 */ 	addiu	$t7,$t6,0x8
/*  f0f1a7c:	afaf0468 */ 	sw	$t7,0x468($sp)
/*  f0f1a80:	adc00004 */ 	sw	$zero,0x4($t6)
/*  f0f1a84:	adcc0000 */ 	sw	$t4,0x0($t6)
/*  f0f1a88:	8fb80468 */ 	lw	$t8,0x468($sp)
/*  f0f1a8c:	37390002 */ 	ori	$t9,$t9,0x2
/*  f0f1a90:	3c0efc12 */ 	lui	$t6,0xfc12
/*  f0f1a94:	270a0008 */ 	addiu	$t2,$t8,0x8
/*  f0f1a98:	afaa0468 */ 	sw	$t2,0x468($sp)
/*  f0f1a9c:	af000004 */ 	sw	$zero,0x4($t8)
/*  f0f1aa0:	af190000 */ 	sw	$t9,0x0($t8)
/*  f0f1aa4:	8fab0468 */ 	lw	$t3,0x468($sp)
/*  f0f1aa8:	35ce7e24 */ 	ori	$t6,$t6,0x7e24
/*  f0f1aac:	240ff9fc */ 	addiu	$t7,$zero,-1540
/*  f0f1ab0:	256d0008 */ 	addiu	$t5,$t3,0x8
/*  f0f1ab4:	afad0468 */ 	sw	$t5,0x468($sp)
/*  f0f1ab8:	ad6f0004 */ 	sw	$t7,0x4($t3)
/*  f0f1abc:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f1ac0:	8fac0468 */ 	lw	$t4,0x468($sp)
/*  f0f1ac4:	24193000 */ 	addiu	$t9,$zero,0x3000
/*  f0f1ac8:	3c0ab600 */ 	lui	$t2,0xb600
/*  f0f1acc:	25980008 */ 	addiu	$t8,$t4,0x8
/*  f0f1ad0:	afb80468 */ 	sw	$t8,0x468($sp)
/*  f0f1ad4:	ad990004 */ 	sw	$t9,0x4($t4)
/*  f0f1ad8:	ad8a0000 */ 	sw	$t2,0x0($t4)
/*  f0f1adc:	8fab0468 */ 	lw	$t3,0x468($sp)
/*  f0f1ae0:	3c0eba00 */ 	lui	$t6,0xba00
/*  f0f1ae4:	35ce0c02 */ 	ori	$t6,$t6,0xc02
/*  f0f1ae8:	256d0008 */ 	addiu	$t5,$t3,0x8
/*  f0f1aec:	afad0468 */ 	sw	$t5,0x468($sp)
/*  f0f1af0:	240f2000 */ 	addiu	$t7,$zero,0x2000
/*  f0f1af4:	ad6f0004 */ 	sw	$t7,0x4($t3)
/*  f0f1af8:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f1afc:	27b00468 */ 	addiu	$s0,$sp,0x468
/*  f0f1b00:	240c0002 */ 	addiu	$t4,$zero,0x2
/*  f0f1b04:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f1b08:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0f1b0c:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0f1b10:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f1b14:	afa00018 */ 	sw	$zero,0x18($sp)
/*  f0f1b18:	00002825 */ 	or	$a1,$zero,$zero
/*  f0f1b1c:	24060002 */ 	addiu	$a2,$zero,0x2
/*  f0f1b20:	0fc2c5c8 */ 	jal	texSelect
/*  f0f1b24:	00003825 */ 	or	$a3,$zero,$zero
/*  f0f1b28:	8faa0468 */ 	lw	$t2,0x468($sp)
/*  f0f1b2c:	3c0bb900 */ 	lui	$t3,0xb900
/*  f0f1b30:	3c0d0050 */ 	lui	$t5,0x50
/*  f0f1b34:	25590008 */ 	addiu	$t9,$t2,0x8
/*  f0f1b38:	afb90468 */ 	sw	$t9,0x468($sp)
/*  f0f1b3c:	35ad4240 */ 	ori	$t5,$t5,0x4240
/*  f0f1b40:	356b031d */ 	ori	$t3,$t3,0x31d
/*  f0f1b44:	ad4b0000 */ 	sw	$t3,0x0($t2)
/*  f0f1b48:	ad4d0004 */ 	sw	$t5,0x4($t2)
/*  f0f1b4c:	240e0002 */ 	addiu	$t6,$zero,0x2
/*  f0f1b50:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f1b54:	afaf0014 */ 	sw	$t7,0x14($sp)
/*  f0f1b58:	afae0010 */ 	sw	$t6,0x10($sp)
/*  f0f1b5c:	afa00018 */ 	sw	$zero,0x18($sp)
/*  f0f1b60:	02002025 */ 	or	$a0,$s0,$zero
/*  f0f1b64:	00002825 */ 	or	$a1,$zero,$zero
/*  f0f1b68:	24060002 */ 	addiu	$a2,$zero,0x2
/*  f0f1b6c:	0fc2c5c8 */ 	jal	texSelect
/*  f0f1b70:	00003825 */ 	or	$a3,$zero,$zero
/*  f0f1b74:	8fac0468 */ 	lw	$t4,0x468($sp)
/*  f0f1b78:	3c0a0600 */ 	lui	$t2,0x600
/*  f0f1b7c:	25980008 */ 	addiu	$t8,$t4,0x8
/*  f0f1b80:	afb80468 */ 	sw	$t8,0x468($sp)
/*  f0f1b84:	ad8a0000 */ 	sw	$t2,0x0($t4)
/*  f0f1b88:	8fb9003c */ 	lw	$t9,0x3c($sp)
/*  f0f1b8c:	ad990004 */ 	sw	$t9,0x4($t4)
.NB0f0f1b90:
/*  f0f1b90:	8fa20468 */ 	lw	$v0,0x468($sp)
.NB0f0f1b94:
/*  f0f1b94:	8fbf002c */ 	lw	$ra,0x2c($sp)
/*  f0f1b98:	8fb00024 */ 	lw	$s0,0x24($sp)
/*  f0f1b9c:	8fb10028 */ 	lw	$s1,0x28($sp)
/*  f0f1ba0:	03e00008 */ 	jr	$ra
/*  f0f1ba4:	27bd0468 */ 	addiu	$sp,$sp,0x468
);
#endif

void menuGetTeamTitlebarColours(u32 *top, u32 *middle, u32 *bottom)
{
	const u32 colours[][3] = {
		// top, middle, bottom
		{ 0xbf000000, 0x50000000, 0xff000000 },
		{ 0xbfbf0000, 0x50500000, 0xffff0000 },
		{ 0x0000bf00, 0x00005000, 0x0000ff00 },
		{ 0xbf00bf00, 0x50005000, 0xff00ff00 },
		{ 0x00bfbf00, 0x00505000, 0x00ffff00 },
		{ 0xff885500, 0x7f482000, 0xff885500 },
		{ 0xff888800, 0x7f484800, 0xff888800 },
		{ 0x88445500, 0x48242000, 0x88445500 },
	};

	*top = colours[g_PlayerConfigsArray[g_MpPlayerNum].base.team][0] | (*top & 0xff);
	*middle = colours[g_PlayerConfigsArray[g_MpPlayerNum].base.team][1] | (*middle & 0xff);
	*bottom = colours[g_PlayerConfigsArray[g_MpPlayerNum].base.team][2] | (*bottom & 0xff);
}

Gfx *menuApplyScissor(Gfx *gdl)
{
	gDPPipeSync(gdl++);

#if VERSION >= VERSION_NTSC_1_0
	g_ScissorX1 = g_MenuScissorX1 * g_ScaleX;
	g_ScissorX2 = g_MenuScissorX2 * g_ScaleX;
	g_ScissorY1 = g_MenuScissorY1;
	g_ScissorY2 = g_MenuScissorY2;

	if (g_ScissorX1 < 0) {
		g_ScissorX1 = 0;
	}

	if (g_ScissorX2 < 0) {
		g_ScissorX2 = 0;
	}

	if (g_ScissorY1 < 0) {
		g_ScissorY1 = 0;
	}

	if (g_ScissorY2 < 0) {
		g_ScissorY2 = 0;
	}

	if (g_ScissorX1 > viGetBufWidth()) {
		g_ScissorX1 = viGetBufWidth();
	}

	if (g_ScissorX2 > viGetBufWidth()) {
		g_ScissorX2 = viGetBufWidth();
	}

	if (g_ScissorY1 > viGetBufHeight()) {
		g_ScissorY1 = viGetBufHeight();
	}

	if (g_ScissorY2 > viGetBufHeight()) {
		g_ScissorY2 = viGetBufHeight();
	}

	if (g_ScissorX2 < g_ScissorX1) {
		g_ScissorX2 = g_ScissorX1;
	}

	if (g_ScissorY2 < g_ScissorY1) {
		g_ScissorY2 = g_ScissorY1;
	}

	gDPSetScissor(gdl++, G_SC_NON_INTERLACE, g_ScissorX1, g_ScissorY1, g_ScissorX2, g_ScissorY2);
#else
	gDPSetScissor(gdl++, G_SC_NON_INTERLACE,
			g_MenuScissorX1 * g_ScaleX, g_MenuScissorY1,
			g_MenuScissorX2 * g_ScaleX, g_MenuScissorY2);
#endif

	return gdl;
}

/**
 * Render a single menu dialog.
 *
 * The lightweight argument is always false. If set to true, a lighter-weight
 * variant of the dialog is rendered which has no borders, less background,
 * no overlays and no models such as inventory weapons.
 */
Gfx *dialogRender(Gfx *gdl, struct menudialog *dialog, struct menu *menu, bool lightweight)
{
	s32 i;
	s32 dialogleft;
	s32 dialogtop;
	s32 dialogright;
	s32 dialogbottom;
	s32 x;
	s32 y;
	s16 dialogwidth;
	union menuitemdata *itemdata;
	s32 j;
	u32 colour1;
	u32 colour2;
	u32 colour3;
	struct menurendercontext context;
	s32 curx;
	s32 bgx1;
	s32 bgy1;
	s32 bgx2;
	s32 bgy2;
	s16 dialogheight;
	char *title;
	u32 colour4;
	u32 colour5;
	f32 sp170;

	bgx1 = dialog->x;
	bgy1 = dialog->y;
	bgx2 = dialog->x + dialog->width;
	bgy2 = dialog->y + dialog->height;

#if VERSION >= VERSION_NTSC_1_0
	if ((g_Vars.coopplayernum >= 0 || g_Vars.antiplayernum >= 0)
			&& menuGetRoot() == MENUROOT_MPENDSCREEN
			&& !var8009dfc0) {
		return gdl;
	}
#endif

	colour1 = MIXCOLOUR(dialog, unk28);

	text0f156030(colour1);

	var8007fb9c = false;

	if (g_Menus[g_MpPlayerNum].curdialog == dialog
			&& (dialog->definition->flags & MENUDIALOGFLAG_0002)
			&& !lightweight
			&& g_Menus[g_MpPlayerNum].unk840.unk5b1_07 == 1) {
		gSPSetGeometryMode(gdl++, G_ZBUFFER);
		gdl = menuRenderModels(gdl, &g_Menus[g_MpPlayerNum].unk840, 2);
		gSPClearGeometryMode(gdl++, G_ZBUFFER);
	}

	dialogwidth = dialog->width;
	dialogheight = dialog->height;

	if (dialog->state == MENUDIALOGSTATE_PREOPEN) {
#if VERSION >= VERSION_NTSC_1_0
		if (dialog->definition == &g_MpReadyMenuDialog) {
			return gdl;
		}
#endif

		sp170 = 1.0f - g_MenuData.unk010;

#if VERSION >= VERSION_NTSC_1_0
		if ((g_Vars.coopplayernum >= 0 || g_Vars.antiplayernum >= 0) && menuGetRoot() == MENUROOT_MPENDSCREEN) {
			sp170 = 1.0f - dialog->statefrac;
		}
#endif

		sp170 = 1.0f - sp170 * sp170;
		dialogheight *= sp170;
		bgy2 = dialog->y + dialogheight;
	}

	dialogleft = dialog->x;
	dialogtop = dialog->y;
	dialogright = dialogleft + dialogwidth;
	dialogbottom = dialogtop + dialogheight;

	title = menuResolveDialogTitle(dialog->definition);

	colour1 = MIXCOLOUR(dialog, unk00);
	colour2 = MIXCOLOUR(dialog, unk04);
	colour3 = MIXCOLOUR(dialog, unk08);

	gSPClearGeometryMode(gdl++, G_ZBUFFER);

	colour4 = colour1;
	colour5 = colour3;

	if ((colour4 & 0xff) > 0x3f) {
		colour4 = (colour4 & 0xffffff00) | 0x3f;
	}

	if ((colour5 & 0xff) > 0x3f) {
		colour5 = (colour5 & 0xffffff00) | 0x3f;
	}

	var8009de90 = -1000;
	var8009de94 = 1000;

	if (dialog->definition->flags & MENUDIALOGFLAG_DISABLETITLEBAR) {
		bgy1 += LINEHEIGHT;
	}

	// Render the walls/floor/ceiling coming from the projection source.
	// Each surface is rendered a second time with the colours swapped.
	// The order is top, right, bottom, left.
	if (g_MenuData.root != MENUROOT_MPSETUP && (g_MenuData.root != MENUROOT_MPPAUSE || g_Vars.normmplayerisrunning)) {
		var800a4634 = func0f0d4d0c(var800a4634, bgx1, bgy1, bgx2, bgy1, colour4, colour5, 0);
		var800a4634 = func0f0d4d0c(var800a4634, bgx2, bgy1, bgx2, bgy2, colour5, colour4, 0);
		var800a4634 = func0f0d4d0c(var800a4634, bgx2, bgy2, bgx1, bgy2, colour4, colour5, 0);
		var800a4634 = func0f0d4d0c(var800a4634, bgx1, bgy2, bgx1, bgy1, colour5, colour4, 0);
		var800a4634 = func0f0d4d0c(var800a4634, bgx1, bgy1, bgx2, bgy1, colour5, colour4, 1);
		var800a4634 = func0f0d4d0c(var800a4634, bgx2, bgy1, bgx2, bgy2, colour4, colour5, 1);
		var800a4634 = func0f0d4d0c(var800a4634, bgx2, bgy2, bgx1, bgy2, colour5, colour4, 1);
		var800a4634 = func0f0d4d0c(var800a4634, bgx1, bgy2, bgx1, bgy1, colour4, colour5, 1);
	}

	// Render the title bar
	if ((dialog->definition->flags & MENUDIALOGFLAG_DISABLETITLEBAR) == 0) {
		if (((g_MenuData.root == MENUROOT_MPSETUP) || (g_MenuData.root == MENUROOT_4MBMAINMENU))
				&& (g_MpSetup.options & MPOPTION_TEAMSENABLED)
				&& g_Vars.mpsetupmenu != MPSETUPMENU_GENERAL) {
			menuGetTeamTitlebarColours(&colour1, &colour2, &colour3);
		}

		gdl = menugfxRenderGradient(gdl, dialogleft - 2, dialogtop, dialogright + 2, dialogtop + LINEHEIGHT, colour1, colour2, colour3);
		gdl = menugfxDrawShimmer(gdl, dialogleft - 2, dialogtop, dialogright + 2, dialogtop + 1, (colour1 & 0xff) >> 1, 1, 40, 0);
		gdl = menugfxDrawShimmer(gdl, dialogleft - 2, dialogtop + 10, dialogright + 2, dialogtop + LINEHEIGHT, (colour1 & 0xff) >> 1, 0, 40, 1);

		x = dialogleft + 2;
		y = dialogtop + 2;

		gdl = text0f153628(gdl);

		context.unk18 = false;

		if (lightweight) {
			context.unk18 = true;
		}

		{
			char *sp154[] = {
				"1\n",
				"2\n",
				"3\n",
				"4\n",
			};

			colour1 = MIXCOLOUR(dialog, unk0c);

			textSetWaveColours(g_MenuColourPalettes3[dialog->type].unk0c, g_MenuColourPalettes2[dialog->type].unk0c);

			// Title shadow
			x = dialogleft + 3;
			y = dialogtop + 3;

			gdl = textRenderProjected(gdl, &x, &y, title, g_CharsHandelGothicSm, g_FontHandelGothicSm, colour1 & 0xff, dialogwidth, viGetHeight(), 0, 0);

			// Title proper
			x = dialogleft + 2;
			y = dialogtop + 2;

			gdl = textRenderProjected(gdl, &x, &y, title, g_CharsHandelGothicSm, g_FontHandelGothicSm, colour1, dialogwidth, viGetHeight(), 0, 0);

			// In MP dialogs, render the player number in the top right
			if (g_MenuData.root == MENUROOT_MPSETUP
					|| g_MenuData.root == MENUROOT_MPPAUSE
					|| g_MenuData.root == MENUROOT_MPENDSCREEN
					|| g_MenuData.root == MENUROOT_4MBMAINMENU) {
				x = dialogright - 9;
				y = dialogtop + 2;

				gdl = textRenderProjected(gdl, &x, &y, sp154[g_MpPlayerNum], g_CharsHandelGothicSm, g_FontHandelGothicSm, colour1, dialogwidth, viGetHeight(), 0, 0);
			}
		}

		gdl = text0f153780(gdl);
	}

	// Configure things for the redraw effect
	if (!(dialog->redrawtimer < 0.0f)) {
		if (g_MenuData.root != MENUROOT_MPPAUSE) {
			if (dialog->state >= MENUDIALOGSTATE_POPULATED) {
				textSetDiagonalBlend(dialog->x, dialog->y, dialog->redrawtimer, DIAGMODE_REDRAW);
			} else {
				textSetDiagonalBlend(dialog->x, dialog->y, dialog->redrawtimer, DIAGMODE_FADEIN);
			}

			var8007fb9c = true;
		}
	} else if (dialog->state == MENUDIALOGSTATE_POPULATED) {
		textSetMenuBlend(dialog->statefrac);
	}

	if (dialogbottom < dialogtop + LINEHEIGHT) {
		dialogbottom = dialogtop + LINEHEIGHT;
	}

	colour1 = MIXCOLOUR(dialog, unk10);

	if (dialog->dimmed) {
		colour1 = (colourBlend(colour1, 0x00000000, 44) & 0xffffff00) | (colour1 & 0xff);
	}

	colour2 = MIXCOLOUR(dialog, unk14);

	// Draw the dialog's background and outer borders
	if (!lightweight) {
		if (dialog->state == MENUDIALOGSTATE_OPENING) {
			gdl = menugfxRenderDialogBackground(gdl, dialogleft + 1, dialogtop + LINEHEIGHT, dialogright - 1, dialogbottom, dialog, colour1, colour2, 1.0f);
		} else if (dialog->state == MENUDIALOGSTATE_POPULATING) {
			gdl = menugfxRenderDialogBackground(gdl, dialogleft + 1, dialogtop + LINEHEIGHT, dialogright - 1, dialogbottom, dialog, colour1, colour2, dialog->statefrac);
		} else {
			gdl = menugfxRenderDialogBackground(gdl, dialogleft + 1, dialogtop + LINEHEIGHT, dialogright - 1, dialogbottom, dialog, colour1, colour2, -1.0f);
		}

		// No dialog has this flag, so this branch is unused
		if (dialog->definition->flags & MENUDIALOGFLAG_DISABLETITLEBAR) {
			gdl = menugfxDrawDialogBorderLine(gdl, dialogleft + 1, dialogtop + LINEHEIGHT, dialogright - 1, dialogtop + LINEHEIGHT + 1, MIXCOLOUR(dialog, unk00), MIXCOLOUR(dialog, unk08));
		}
	}

	if (dialog->state == MENUDIALOGSTATE_PREOPEN) {
		return gdl;
	}

	{
		struct menulayer *layer;
		s32 viewleft = viGetViewLeft() / g_ScaleX;
		s32 viewtop = viGetViewTop();
		s32 viewright = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX;
		s32 viewbottom = viGetViewTop() + viGetViewHeight();

		g_MenuScissorX1 = dialogleft + 2;
		g_MenuScissorX2 = dialogright - 2;
		g_MenuScissorY1 = dialogtop + LINEHEIGHT;
		g_MenuScissorY2 = dialogbottom - 1;

		if (g_MenuScissorX2 < viewleft) {
			return gdl;
		}

		if (g_MenuScissorY2 < viewtop) {
			return gdl;
		}

		if (g_MenuScissorX1 > viewright) {
			return gdl;
		}

		if (g_MenuScissorY1 > viewbottom) {
			return gdl;
		}

		if (g_MenuScissorX2 > viewright) {
			g_MenuScissorX2 = viewright;
		}

		if (g_MenuScissorY2 > viewbottom) {
			g_MenuScissorY2 = viewbottom;
		}

		if (g_MenuScissorX1 < viewleft) {
			g_MenuScissorX1 = viewleft;
		}

		// @bug: This should be g_MenuScissorY1
		// but the condition can't pass anyway.
		if (g_MenuScissorX2 < viewtop) {
			g_MenuScissorX2 = viewtop;
		}

		var8009de90 = g_MenuScissorY1;
		var8009de94 = g_MenuScissorY2;

		gdl = menuApplyScissor(gdl);

		// Render models (inventory, chr/vehicle bios)
		if (g_Menus[g_MpPlayerNum].curdialog == dialog
				&& (dialog->definition->flags & MENUDIALOGFLAG_0002)
				&& !lightweight
				&& !g_Menus[g_MpPlayerNum].unk840.unk5b1_07) {
			gSPSetGeometryMode(gdl++, G_ZBUFFER);

			gdl = menuRenderModels(gdl, &g_Menus[g_MpPlayerNum].unk840, 0);

			gSPClearGeometryMode(gdl++, G_ZBUFFER);

			viSetViewPosition(g_Vars.currentplayer->viewleft, g_Vars.currentplayer->viewtop);
			viSetFovAspectAndSize(g_Vars.currentplayer->fovy, g_Vars.currentplayer->aspect,
					g_Vars.currentplayer->viewwidth, g_Vars.currentplayer->viewheight);
		}

		// Render menu items
		if (dialog->type != 0 || dialog->transitionfrac >= 0.0f) {
			u32 stack;
			s32 sumwidth;
			s32 cury;
			bool prevwaslist;
			s32 colwidth;
			u32 sp120;
			bool offscreen;
			struct menuitem *item;
			s32 focused;
			s32 colindex;
			s32 rowindex;

			sumwidth = 0;
			curx = dialogleft;

			for (i = 0; i < dialog->numcols; i++) {
				cury = dialogtop + LINEHEIGHT + 1 + dialog->scroll;
				prevwaslist = false;

				sp120 = MIXCOLOUR(dialog, unfocused);
				sp120 = (sp120 & 0xffffff00) | 0x3f;

				colindex = dialog->colstart + i;

				if (i != 0 && (dialog->definition->flags & MENUDIALOGFLAG_0400) == 0) {
					gdl = menugfxDrawFilledRect(gdl, curx - 1, dialogtop + LINEHEIGHT + 1, curx, dialogbottom, sp120, sp120);
				}

				colwidth = menu->cols[colindex].width;
				sumwidth += colwidth;

				if (i == dialog->numcols - 1) {
					s32 v0 = (dialogright - dialogleft) - 2;

					if (sumwidth < v0) {
						colwidth = (colwidth + v0) - sumwidth;
					}
				}

				for (j = 0; j < menu->cols[colindex].numrows; j++) {
					focused = 0;
					rowindex = menu->cols[colindex].rowstart + j;
					item = &dialog->definition->items[menu->rows[rowindex].itemindex];
					itemdata = NULL;
					offscreen = false;

					if (item == dialog->focuseditem) {
						focused = 1;

						if (dialog->dimmed) {
							focused = 3;
						}
					}

					if (menu->rows[rowindex].blockindex != -1) {
						itemdata = (union menuitemdata *)&menu->blocks[menu->rows[rowindex].blockindex];
					}

					context.x = curx;
					context.y = cury;
					context.width = colwidth;
					context.height = menu->rows[rowindex].height;

					if (context.y + context.height < dialogtop + LINEHEIGHT + 1) {
						offscreen = true;
					}

					if (context.y > dialogbottom) {
						offscreen = true;
					}

					if (context.height == 0) {
						offscreen = true;
					}

					if (!offscreen) {
						context.item = item;
						context.data = itemdata;
						context.focused = focused;
						context.dialog = dialog;

						if (prevwaslist) {
							gdl = menugfxDrawFilledRect(gdl, context.x, context.y - 1, context.x + context.width, context.y, sp120, sp120);
							prevwaslist = false;
						}

						if ((item->flags & MENUITEMFLAG_DARKERBG) && !lightweight) {
							// Render a darker background behind the item
							s32 x1 = context.x;
							s32 y1 = context.y;
							s32 x2 = x1 + context.width;
							s32 y2 = y1 + context.height;
							u32 colour;
							u32 colour2;

							colour2 = MIXCOLOUR(dialog, unk28);
							colour = colourBlend(colour2, colour2 & 0xffffff00, 127);

							gdl = textSetPrimColour(gdl, colour);
							gDPFillRectangleScaled(gdl++, x1, y1, x2, y2);
							gdl = text0f153838(gdl);
						}

						if (focused) {
							if (item->type == MENUITEMTYPE_03
									|| item->type == MENUITEMTYPE_SELECTABLE
									|| item->type == MENUITEMTYPE_CHECKBOX
									|| item->type == MENUITEMTYPE_0A
									|| item->type == MENUITEMTYPE_SLIDER
									|| item->type == MENUITEMTYPE_DROPDOWN) {
#if VERSION >= VERSION_NTSC_1_0
								if (!(dialog->transitionfrac >= 0.0f && dialog->type2 == 0)
										&& !(dialog->transitionfrac < 0.0f && dialog->type == 0)) {
									text0f156024(1);
								}
#else
								text0f156024(1);
#endif
							}

							// Render the horizontal line behind the focused item
							if (item->type == MENUITEMTYPE_SELECTABLE
									|| item->type == MENUITEMTYPE_CHECKBOX
									|| item->type == MENUITEMTYPE_0A
									|| item->type == MENUITEMTYPE_DROPDOWN) {
								s32 liney = context.y + context.height / 2 - 1;
								s32 x1 = context.x;
								s32 x3 = context.x + 8;
								s32 x4 = context.x + context.width / 3;
								u32 colour = (sp120 & 0xffffff00) | 0x2f;

								// Left side
								gdl = menugfx0f0e2498(gdl);
								gdl = menugfxDrawTri2(gdl, x1, liney - 1, x3 - 3, liney, sp120, sp120, 0);
								gdl = menugfxDrawTri2(gdl, x3 - 3, liney - 1, x3, liney, sp120, 0xffffffff, 0);
								gdl = menugfxDrawTri2(gdl, x1, liney + 1, x3 - 3, liney + 2, sp120, sp120, 0);
								gdl = menugfxDrawTri2(gdl, x3 - 3, liney + 1, x3, liney + 2, sp120, 0xffffffff, 0);
								gdl = menugfxDrawTri2(gdl, x3 - 2, liney, x4, liney + 1, colour, sp120 & 0xffffff00, 0);

								if (item->flags & MENUITEMFLAG_SELECTABLE_CENTRE) {
									// Right side
									x1 = context.x + context.width;
									x3 = context.x + context.width - 8;
									x4 = context.x + context.width - context.width / 3;

									gdl = menugfxDrawTri2(gdl, x1 - 5, liney - 1, x1, liney, sp120, sp120, 0);
									gdl = menugfxDrawTri2(gdl, x3, liney - 1, x3 + 3, liney, -1, sp120, 0);
									gdl = menugfxDrawTri2(gdl, x3 + 3, liney + 1, x1, liney + 2, sp120, sp120, 0);
									gdl = menugfxDrawTri2(gdl, x3, liney + 1, x3 + 3, liney + 2, -1, sp120, 0);
									gdl = menugfxDrawTri2(gdl, x4, liney, x3 + 2, liney + 1, sp120 & 0xffffff00, colour, 0);
								}
							}
						}

						gdl = menuitemRender(gdl, &context);

						if (item->type == MENUITEMTYPE_LIST) {
							prevwaslist = true;
						}

						if (focused) {
							text0f156024(0);
						}
					}

					cury += menu->rows[rowindex].height;
				}

				curx += menu->cols[colindex].width;
			}

			// Render overlays, such as dropdown menus
			if (!lightweight) {
				gdl = textSetPrimColour(gdl, 0x00000000);

				curx = dialogleft;

				for (i = 0; i < dialog->numcols; i++) {
#if VERSION >= VERSION_NTSC_1_0
					s32 cury = dialogtop + LINEHEIGHT + 1 + dialog->scroll;
#else
					s32 cury = dialogtop + LINEHEIGHT + 1;
#endif
					colindex = dialog->colstart + i;

					for (j = 0; j < menu->cols[colindex].numrows; j++) {
						union menuitemdata *itemdata;
						struct menuitem *item;

						rowindex = menu->cols[colindex].rowstart + j;
						itemdata = NULL;
						item = &dialog->definition->items[menu->rows[rowindex].itemindex];

						if (menu->rows[rowindex].blockindex != -1) {
							itemdata = (union menuitemdata *)&menu->blocks[menu->rows[rowindex].blockindex];
						}

						gdl = menuitemOverlay(gdl, curx, cury, menu->cols[colindex].width, menu->rows[rowindex].height, item, dialog, itemdata);

						cury += menu->rows[rowindex].height;
					}

					curx += menu->cols[colindex].width;
				}

				gdl = text0f153838(gdl);
			}

			gDPSetScissor(gdl++, G_SC_NON_INTERLACE, viGetViewLeft(), viGetViewTop(),
					viGetViewLeft() + viGetViewWidth(), viGetViewTop() + viGetViewHeight());
		} else {
			gDPSetScissor(gdl++, G_SC_NON_INTERLACE, viGetViewLeft(), viGetViewTop(),
					viGetViewLeft() + viGetViewWidth(), viGetViewTop() + viGetViewHeight());
		}

		// Render left/right chevrons and sibling dialog titles
		layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth - 1];

		if ((dialog->type != 0 || dialog->transitionfrac >= 0.0f)
				&& layer->siblings[layer->cursibling] == dialog
				&& layer->numsiblings >= 2) {
			// Draw chevrons
			u32 colour1;
			u32 colour;
			u32 weight = func0f006b08(10) * 255.0f;

			colour1 = MIXCOLOUR(dialog, unk00);
			colour = colourBlend(0xffffffff, colour1, weight);

			gdl = menugfxDrawDialogChevron(gdl, dialogleft - 5, (dialogtop + dialogbottom) / 2, 9, 1, colour, colour, func0f006b08(20));
			gdl = menugfxDrawDialogChevron(gdl, dialogright + 5, (dialogtop + dialogbottom) / 2, 9, 3, colour, colour, func0f006b08(20));

			if (g_MenuData.root == MENUROOT_MAINMENU
					|| g_MenuData.root == MENUROOT_4MBFILEMGR
					|| g_MenuData.root == MENUROOT_TRAINING
					|| g_MenuData.root == MENUROOT_FILEMGR) {
				char *title;
				s32 textheight;
				s32 textwidth;
				s32 previndex;
				s32 nextindex;

				textResetBlends();
				textSetRotation90(true);

				gdl = text0f153628(gdl);

				// Left/previous title
				previndex = layer->cursibling - 1;

				if (previndex < 0) {
					previndex = layer->numsiblings - 1;
				}

				title = menuResolveDialogTitle(layer->siblings[previndex]->definition);

				textMeasure(&textheight, &textwidth, title, g_CharsHandelGothicXs, g_FontHandelGothicXs, 0);

				x = dialogleft - 1;
				y = (dialogtop + dialogbottom) / 2 - textwidth - 3;

				if (y < dialogtop) {
					y = (dialogtop + dialogbottom - textwidth) / 2;
					x -= 3;
				}

				gdl = textRenderProjected(gdl, &y, &x, title, g_CharsHandelGothicXs, g_FontHandelGothicXs, 0xffffffff, dialogwidth, viGetHeight(), 0, 0);

				// Right/next title
				nextindex = layer->cursibling + 1;

				if (nextindex >= layer->numsiblings) {
					nextindex = 0;
				}

				title = menuResolveDialogTitle(layer->siblings[nextindex]->definition);

				textMeasure(&textheight, &textwidth, title, g_CharsHandelGothicXs, g_FontHandelGothicXs, 0);

#if VERSION == VERSION_JPN_FINAL
				x = dialogright + 13;
#else
				x = dialogright + 7;
#endif
				y = (dialogtop + dialogbottom) / 2 + 3;

				if (y + textwidth > dialogbottom) {
					y = (dialogtop + dialogbottom - textwidth) / 2;
					x += 3;
				}

				gdl = textRenderProjected(gdl, &y, &x, title, g_CharsHandelGothicXs, g_FontHandelGothicXs, -1, dialogwidth, viGetHeight(), 0, 0);
				gdl = text0f153780(gdl);

				textSetRotation90(false);
			}
		}
	}

	return gdl;
}

const char var7f1b2668[] = "[]-[] Terminate Complete\n";
const char var7f1b2684[] = "Enabling control %d\n";
const char var7f1b269c[] = "NOT IN MODE MULTIINGAME!\n";
const char var7f1b26b8[] = "Numactive now:%d\n";
const char var7f1b26cc[] = "[]-[] SwitchMenuMode called, context %d\n";

void menuGetContPads(s8 *contpadnum1, s8 *contpadnum2)
{
	switch (g_MenuData.root) {
	case MENUROOT_MPSETUP:
	case MENUROOT_FILEMGR:
	case MENUROOT_BOOTPAKMGR:
	case MENUROOT_4MBFILEMGR:
	case MENUROOT_4MBMAINMENU:
		*contpadnum1 = g_MpPlayerNum;
		*contpadnum2 = -1;
		break;
	default:
		*contpadnum1 = optionsGetContpadNum1(g_Vars.currentplayerstats->mpindex);
		*contpadnum2 = -1;

		if (!g_Vars.normmplayerisrunning) {
			s32 mode = optionsGetControlMode(g_Vars.currentplayerstats->mpindex);

			if (mode == CONTROLMODE_23 || mode == CONTROLMODE_24 || mode == CONTROLMODE_22 || mode == CONTROLMODE_21) {
				*contpadnum2 = optionsGetContpadNum2(g_Vars.currentplayerstats->mpindex);
			}
		}
		break;
	}
}

u32 g_MpNumJoined = 1;

/**
 * Choose which direction a new dialog should swipe from in the combat simulator
 * menus.
 */
void func0f0f7594(s32 arg0, s32 *vdir, s32 *hdir)
{
	if (g_MenuData.root == MENUROOT_MPSETUP) {
		s32 playernum = g_Menus[g_MpPlayerNum].playernum;

		*vdir = 0;
		*hdir = 0;

		switch (g_MpNumJoined) {
		case 1:
			*hdir = arg0;
			break;
		case 2:
			if (playernum == 0) {
				if (arg0 < 0) {
					*hdir = -1;
				} else {
					*vdir = -1;
				}
			} else {
				if (arg0 > 0) {
					*hdir = 1;
				} else {
					*vdir = 1;
				}
			}
			break;
		case 3:
			if (playernum == 2) {
				*hdir = arg0;
			} else if (playernum == 0) {
				if (arg0 < 0) {
					*hdir = -1;
				} else {
					*vdir = -1;
				}
			} else {
				if (arg0 > 0) {
					*hdir = 1;
				} else {
					*vdir = -1;
				}
			}
			break;
		case 4:
			if (playernum == 0 || playernum == 2) {
				if (arg0 < 0) {
					*hdir = -1;
				} else {
					*vdir = playernum == 0 ? -1 : 1;
				}
			} else {
				if (arg0 > 0) {
					*hdir = 1;
				} else {
					*vdir = playernum == 1 ? -1 : 1;
				}
			}
			break;
		}
	} else {
		*vdir = 0;
		*hdir = arg0;
	}
}

#if VERSION >= VERSION_JPN_FINAL
GLOBAL_ASM(
glabel menuFindAvailableSize
.late_rodata
glabel var7f1b369cjf
.word 0x7f0f859c
.word 0x7f0f87f4
.word 0x7f0f87f4
.word 0x7f0f8b64
.word 0x7f0f8b64
.word 0x7f0f87f4
.word 0x7f0f8b64
.word 0x7f0f87f4
.word 0x7f0f859c
.text
/*  f0f83cc:	27bdffc0 */ 	addiu	$sp,$sp,-64
/*  f0f83d0:	afbf0014 */ 	sw	$ra,0x14($sp)
/*  f0f83d4:	afa40040 */ 	sw	$a0,0x40($sp)
/*  f0f83d8:	afa50044 */ 	sw	$a1,0x44($sp)
/*  f0f83dc:	afa60048 */ 	sw	$a2,0x48($sp)
/*  f0f83e0:	0c002f29 */ 	jal	viGetViewLeft
/*  f0f83e4:	afa7004c */ 	sw	$a3,0x4c($sp)
/*  f0f83e8:	3c0e8008 */ 	lui	$t6,0x8008
/*  f0f83ec:	8dce00f4 */ 	lw	$t6,0xf4($t6)
/*  f0f83f0:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f83f4:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0f83f8:	004e001a */ 	div	$zero,$v0,$t6
/*  f0f83fc:	00003012 */ 	mflo	$a2
/*  f0f8400:	24c60014 */ 	addiu	$a2,$a2,0x14
/*  f0f8404:	15c00002 */ 	bnez	$t6,.JF0f0f8410
/*  f0f8408:	00000000 */ 	nop
/*  f0f840c:	0007000d */ 	break	0x7
.JF0f0f8410:
/*  f0f8410:	2401ffff */ 	li	$at,-1
/*  f0f8414:	15c10004 */ 	bne	$t6,$at,.JF0f0f8428
/*  f0f8418:	3c018000 */ 	lui	$at,0x8000
/*  f0f841c:	14410002 */ 	bne	$v0,$at,.JF0f0f8428
/*  f0f8420:	00000000 */ 	nop
/*  f0f8424:	0006000d */ 	break	0x6
.JF0f0f8428:
/*  f0f8428:	afa6003c */ 	sw	$a2,0x3c($sp)
/*  f0f842c:	0c002f2d */ 	jal	viGetViewTop
/*  f0f8430:	00000000 */ 	nop
/*  f0f8434:	24440004 */ 	addiu	$a0,$v0,0x4
/*  f0f8438:	0c002f29 */ 	jal	viGetViewLeft
/*  f0f843c:	afa40038 */ 	sw	$a0,0x38($sp)
/*  f0f8440:	0c002f0b */ 	jal	viGetViewWidth
/*  f0f8444:	a7a2001e */ 	sh	$v0,0x1e($sp)
/*  f0f8448:	87af001e */ 	lh	$t7,0x1e($sp)
/*  f0f844c:	3c198008 */ 	lui	$t9,0x8008
/*  f0f8450:	8f3900f4 */ 	lw	$t9,0xf4($t9)
/*  f0f8454:	004fc021 */ 	addu	$t8,$v0,$t7
/*  f0f8458:	8fa40038 */ 	lw	$a0,0x38($sp)
/*  f0f845c:	0319001a */ 	div	$zero,$t8,$t9
/*  f0f8460:	00003812 */ 	mflo	$a3
/*  f0f8464:	24e7ffec */ 	addiu	$a3,$a3,-20
/*  f0f8468:	8fa6003c */ 	lw	$a2,0x3c($sp)
/*  f0f846c:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f8470:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0f8474:	17200002 */ 	bnez	$t9,.JF0f0f8480
/*  f0f8478:	00000000 */ 	nop
/*  f0f847c:	0007000d */ 	break	0x7
.JF0f0f8480:
/*  f0f8480:	2401ffff */ 	li	$at,-1
/*  f0f8484:	17210004 */ 	bne	$t9,$at,.JF0f0f8498
/*  f0f8488:	3c018000 */ 	lui	$at,0x8000
/*  f0f848c:	17010002 */ 	bne	$t8,$at,.JF0f0f8498
/*  f0f8490:	00000000 */ 	nop
/*  f0f8494:	0006000d */ 	break	0x6
.JF0f0f8498:
/*  f0f8498:	afa70034 */ 	sw	$a3,0x34($sp)
/*  f0f849c:	0c002f2d */ 	jal	viGetViewTop
/*  f0f84a0:	00000000 */ 	nop
/*  f0f84a4:	0c002f0f */ 	jal	viGetViewHeight
/*  f0f84a8:	a7a2001e */ 	sh	$v0,0x1e($sp)
/*  f0f84ac:	87ae001e */ 	lh	$t6,0x1e($sp)
/*  f0f84b0:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0f84b4:	004e2821 */ 	addu	$a1,$v0,$t6
/*  f0f84b8:	24a5fffc */ 	addiu	$a1,$a1,-4
/*  f0f84bc:	0fc3c64f */ 	jal	currentPlayerIsMenuOpenInSoloOrMp
/*  f0f84c0:	afa50030 */ 	sw	$a1,0x30($sp)
/*  f0f84c4:	8fa30024 */ 	lw	$v1,0x24($sp)
/*  f0f84c8:	8fa40038 */ 	lw	$a0,0x38($sp)
/*  f0f84cc:	8fa50030 */ 	lw	$a1,0x30($sp)
/*  f0f84d0:	8fa6003c */ 	lw	$a2,0x3c($sp)
/*  f0f84d4:	8fa70034 */ 	lw	$a3,0x34($sp)
/*  f0f84d8:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f84dc:	10400002 */ 	beqz	$v0,.JF0f0f84e8
/*  f0f84e0:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0f84e4:	24030001 */ 	li	$v1,0x1
.JF0f0f84e8:
/*  f0f84e8:	8fa80050 */ 	lw	$t0,0x50($sp)
/*  f0f84ec:	1100000d */ 	beqz	$t0,.JF0f0f8524
/*  f0f84f0:	00000000 */ 	nop
/*  f0f84f4:	8d020000 */ 	lw	$v0,0x0($t0)
/*  f0f84f8:	3c0f8007 */ 	lui	$t7,0x8007
/*  f0f84fc:	25ef26a8 */ 	addiu	$t7,$t7,0x26a8
/*  f0f8500:	11e20007 */ 	beq	$t7,$v0,.JF0f0f8520
/*  f0f8504:	3c188007 */ 	lui	$t8,0x8007
/*  f0f8508:	27182654 */ 	addiu	$t8,$t8,0x2654
/*  f0f850c:	13020004 */ 	beq	$t8,$v0,.JF0f0f8520
/*  f0f8510:	3c198007 */ 	lui	$t9,0x8007
/*  f0f8514:	27392600 */ 	addiu	$t9,$t9,0x2600
/*  f0f8518:	17220002 */ 	bne	$t9,$v0,.JF0f0f8524
/*  f0f851c:	00000000 */ 	nop
.JF0f0f8520:
/*  f0f8520:	00001825 */ 	move	$v1,$zero
.JF0f0f8524:
/*  f0f8524:	10600011 */ 	beqz	$v1,.JF0f0f856c
/*  f0f8528:	00000000 */ 	nop
/*  f0f852c:	24840016 */ 	addiu	$a0,$a0,0x16
/*  f0f8530:	afa40038 */ 	sw	$a0,0x38($sp)
/*  f0f8534:	afa50030 */ 	sw	$a1,0x30($sp)
/*  f0f8538:	afa6003c */ 	sw	$a2,0x3c($sp)
/*  f0f853c:	0fc548e8 */ 	jal	optionsGetEffectiveScreenSize
/*  f0f8540:	afa70034 */ 	sw	$a3,0x34($sp)
/*  f0f8544:	24010002 */ 	li	$at,0x2
/*  f0f8548:	8fa40038 */ 	lw	$a0,0x38($sp)
/*  f0f854c:	8fa50030 */ 	lw	$a1,0x30($sp)
/*  f0f8550:	8fa6003c */ 	lw	$a2,0x3c($sp)
/*  f0f8554:	8fa70034 */ 	lw	$a3,0x34($sp)
/*  f0f8558:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f855c:	14410003 */ 	bne	$v0,$at,.JF0f0f856c
/*  f0f8560:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0f8564:	2484fff8 */ 	addiu	$a0,$a0,-8
/*  f0f8568:	24a50004 */ 	addiu	$a1,$a1,0x4
.JF0f0f856c:
/*  f0f856c:	3c02800a */ 	lui	$v0,0x800a
/*  f0f8570:	244220c0 */ 	addiu	$v0,$v0,0x20c0
/*  f0f8574:	8c4e0004 */ 	lw	$t6,0x4($v0)
/*  f0f8578:	25cffffd */ 	addiu	$t7,$t6,-3
/*  f0f857c:	2de10009 */ 	sltiu	$at,$t7,0x9
/*  f0f8580:	10200178 */ 	beqz	$at,.JF0f0f8b64
/*  f0f8584:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0f8588:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f858c:	002f0821 */ 	addu	$at,$at,$t7
/*  f0f8590:	8c2f369c */ 	lw	$t7,0x369c($at)
/*  f0f8594:	01e00008 */ 	jr	$t7
/*  f0f8598:	00000000 */ 	nop
/*  f0f859c:	3c1f8007 */ 	lui	$ra,0x8007
/*  f0f85a0:	27ff1998 */ 	addiu	$ra,$ra,0x1998
/*  f0f85a4:	8ff80000 */ 	lw	$t8,0x0($ra)
/*  f0f85a8:	904f0017 */ 	lbu	$t7,0x17($v0)
/*  f0f85ac:	3c0d800a */ 	lui	$t5,0x800a
/*  f0f85b0:	0018c8c0 */ 	sll	$t9,$t8,0x3
/*  f0f85b4:	0338c823 */ 	subu	$t9,$t9,$t8
/*  f0f85b8:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0f85bc:	0338c821 */ 	addu	$t9,$t9,$t8
/*  f0f85c0:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0f85c4:	0338c823 */ 	subu	$t9,$t9,$t8
/*  f0f85c8:	25ade700 */ 	addiu	$t5,$t5,-6400
/*  f0f85cc:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0f85d0:	01b97021 */ 	addu	$t6,$t5,$t9
/*  f0f85d4:	1de00004 */ 	bgtz	$t7,.JF0f0f85e8
/*  f0f85d8:	91c3083b */ 	lbu	$v1,0x83b($t6)
/*  f0f85dc:	90580018 */ 	lbu	$t8,0x18($v0)
/*  f0f85e0:	5b000003 */ 	blezl	$t8,.JF0f0f85f0
/*  f0f85e4:	90590019 */ 	lbu	$t9,0x19($v0)
.JF0f0f85e8:
/*  f0f85e8:	2484000a */ 	addiu	$a0,$a0,0xa
/*  f0f85ec:	90590019 */ 	lbu	$t9,0x19($v0)
.JF0f0f85f0:
/*  f0f85f0:	5f200005 */ 	bgtzl	$t9,.JF0f0f8608
/*  f0f85f4:	24a5fff6 */ 	addiu	$a1,$a1,-10
/*  f0f85f8:	904e001a */ 	lbu	$t6,0x1a($v0)
/*  f0f85fc:	19c00002 */ 	blez	$t6,.JF0f0f8608
/*  f0f8600:	00000000 */ 	nop
/*  f0f8604:	24a5fff6 */ 	addiu	$a1,$a1,-10
.JF0f0f8608:
/*  f0f8608:	3c028007 */ 	lui	$v0,0x8007
/*  f0f860c:	8c421a28 */ 	lw	$v0,0x1a28($v0)
/*  f0f8610:	24010001 */ 	li	$at,0x1
/*  f0f8614:	1041000a */ 	beq	$v0,$at,.JF0f0f8640
/*  f0f8618:	24010002 */ 	li	$at,0x2
/*  f0f861c:	1041000f */ 	beq	$v0,$at,.JF0f0f865c
/*  f0f8620:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f8624:	24010003 */ 	li	$at,0x3
/*  f0f8628:	10410020 */ 	beq	$v0,$at,.JF0f0f86ac
/*  f0f862c:	24010004 */ 	li	$at,0x4
/*  f0f8630:	10410047 */ 	beq	$v0,$at,.JF0f0f8750
/*  f0f8634:	00000000 */ 	nop
/*  f0f8638:	10000151 */ 	b	.JF0f0f8b80
/*  f0f863c:	8fbf0014 */ 	lw	$ra,0x14($sp)
.JF0f0f8640:
/*  f0f8640:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f8644:	ad870000 */ 	sw	$a3,0x0($t4)
/*  f0f8648:	8faf0044 */ 	lw	$t7,0x44($sp)
/*  f0f864c:	ade40000 */ 	sw	$a0,0x0($t7)
/*  f0f8650:	8fb8004c */ 	lw	$t8,0x4c($sp)
/*  f0f8654:	10000149 */ 	b	.JF0f0f8b7c
/*  f0f8658:	af050000 */ 	sw	$a1,0x0($t8)
.JF0f0f865c:
/*  f0f865c:	af240000 */ 	sw	$a0,0x0($t9)
/*  f0f8660:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f8664:	00c7c821 */ 	addu	$t9,$a2,$a3
/*  f0f8668:	14600009 */ 	bnez	$v1,.JF0f0f8690
/*  f0f866c:	adc50000 */ 	sw	$a1,0x0($t6)
/*  f0f8670:	00c77821 */ 	addu	$t7,$a2,$a3
/*  f0f8674:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f8678:	05e10003 */ 	bgez	$t7,.JF0f0f8688
/*  f0f867c:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0f8680:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0f8684:	0001c043 */ 	sra	$t8,$at,0x1
.JF0f0f8688:
/*  f0f8688:	1000013c */ 	b	.JF0f0f8b7c
/*  f0f868c:	ad980000 */ 	sw	$t8,0x0($t4)
.JF0f0f8690:
/*  f0f8690:	07210003 */ 	bgez	$t9,.JF0f0f86a0
/*  f0f8694:	00197043 */ 	sra	$t6,$t9,0x1
/*  f0f8698:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0f869c:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0f86a0:
/*  f0f86a0:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f86a4:	10000135 */ 	b	.JF0f0f8b7c
/*  f0f86a8:	ad870000 */ 	sw	$a3,0x0($t4)
.JF0f0f86ac:
/*  f0f86ac:	10600003 */ 	beqz	$v1,.JF0f0f86bc
/*  f0f86b0:	24080001 */ 	li	$t0,0x1
/*  f0f86b4:	1468001b */ 	bne	$v1,$t0,.JF0f0f8724
/*  f0f86b8:	00857821 */ 	addu	$t7,$a0,$a1
.JF0f0f86bc:
/*  f0f86bc:	8faf0044 */ 	lw	$t7,0x44($sp)
/*  f0f86c0:	0085c021 */ 	addu	$t8,$a0,$a1
/*  f0f86c4:	ade40000 */ 	sw	$a0,0x0($t7)
/*  f0f86c8:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f86cc:	07010003 */ 	bgez	$t8,.JF0f0f86dc
/*  f0f86d0:	0018c843 */ 	sra	$t9,$t8,0x1
/*  f0f86d4:	27010001 */ 	addiu	$at,$t8,0x1
/*  f0f86d8:	0001c843 */ 	sra	$t9,$at,0x1
.JF0f0f86dc:
/*  f0f86dc:	14600009 */ 	bnez	$v1,.JF0f0f8704
/*  f0f86e0:	add90000 */ 	sw	$t9,0x0($t6)
/*  f0f86e4:	00c77821 */ 	addu	$t7,$a2,$a3
/*  f0f86e8:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f86ec:	05e10003 */ 	bgez	$t7,.JF0f0f86fc
/*  f0f86f0:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0f86f4:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0f86f8:	0001c043 */ 	sra	$t8,$at,0x1
.JF0f0f86fc:
/*  f0f86fc:	1000011f */ 	b	.JF0f0f8b7c
/*  f0f8700:	ad980000 */ 	sw	$t8,0x0($t4)
.JF0f0f8704:
/*  f0f8704:	00c7c821 */ 	addu	$t9,$a2,$a3
/*  f0f8708:	07210003 */ 	bgez	$t9,.JF0f0f8718
/*  f0f870c:	00197043 */ 	sra	$t6,$t9,0x1
/*  f0f8710:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0f8714:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0f8718:
/*  f0f8718:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f871c:	10000117 */ 	b	.JF0f0f8b7c
/*  f0f8720:	ad870000 */ 	sw	$a3,0x0($t4)
.JF0f0f8724:
/*  f0f8724:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f8728:	05e10003 */ 	bgez	$t7,.JF0f0f8738
/*  f0f872c:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0f8730:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0f8734:	0001c043 */ 	sra	$t8,$at,0x1
.JF0f0f8738:
/*  f0f8738:	af380000 */ 	sw	$t8,0x0($t9)
/*  f0f873c:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f8740:	adc50000 */ 	sw	$a1,0x0($t6)
/*  f0f8744:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f8748:	1000010c */ 	b	.JF0f0f8b7c
/*  f0f874c:	ad870000 */ 	sw	$a3,0x0($t4)
.JF0f0f8750:
/*  f0f8750:	10600003 */ 	beqz	$v1,.JF0f0f8760
/*  f0f8754:	24010002 */ 	li	$at,0x2
/*  f0f8758:	14610009 */ 	bne	$v1,$at,.JF0f0f8780
/*  f0f875c:	00c7c821 */ 	addu	$t9,$a2,$a3
.JF0f0f8760:
/*  f0f8760:	00c77821 */ 	addu	$t7,$a2,$a3
/*  f0f8764:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f8768:	05e10003 */ 	bgez	$t7,.JF0f0f8778
/*  f0f876c:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0f8770:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0f8774:	0001c043 */ 	sra	$t8,$at,0x1
.JF0f0f8778:
/*  f0f8778:	10000007 */ 	b	.JF0f0f8798
/*  f0f877c:	ad980000 */ 	sw	$t8,0x0($t4)
.JF0f0f8780:
/*  f0f8780:	07210003 */ 	bgez	$t9,.JF0f0f8790
/*  f0f8784:	00197043 */ 	sra	$t6,$t9,0x1
/*  f0f8788:	27210001 */ 	addiu	$at,$t9,0x1
/*  f0f878c:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0f8790:
/*  f0f8790:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f8794:	ad870000 */ 	sw	$a3,0x0($t4)
.JF0f0f8798:
/*  f0f8798:	10600003 */ 	beqz	$v1,.JF0f0f87a8
/*  f0f879c:	24080001 */ 	li	$t0,0x1
/*  f0f87a0:	1468000b */ 	bne	$v1,$t0,.JF0f0f87d0
/*  f0f87a4:	00857821 */ 	addu	$t7,$a0,$a1
.JF0f0f87a8:
/*  f0f87a8:	8faf0044 */ 	lw	$t7,0x44($sp)
/*  f0f87ac:	0085c021 */ 	addu	$t8,$a0,$a1
/*  f0f87b0:	ade40000 */ 	sw	$a0,0x0($t7)
/*  f0f87b4:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f87b8:	07010003 */ 	bgez	$t8,.JF0f0f87c8
/*  f0f87bc:	0018c843 */ 	sra	$t9,$t8,0x1
/*  f0f87c0:	27010001 */ 	addiu	$at,$t8,0x1
/*  f0f87c4:	0001c843 */ 	sra	$t9,$at,0x1
.JF0f0f87c8:
/*  f0f87c8:	100000ec */ 	b	.JF0f0f8b7c
/*  f0f87cc:	add90000 */ 	sw	$t9,0x0($t6)
.JF0f0f87d0:
/*  f0f87d0:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f87d4:	05e10003 */ 	bgez	$t7,.JF0f0f87e4
/*  f0f87d8:	000fc043 */ 	sra	$t8,$t7,0x1
/*  f0f87dc:	25e10001 */ 	addiu	$at,$t7,0x1
/*  f0f87e0:	0001c043 */ 	sra	$t8,$at,0x1
.JF0f0f87e4:
/*  f0f87e4:	af380000 */ 	sw	$t8,0x0($t9)
/*  f0f87e8:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f87ec:	100000e3 */ 	b	.JF0f0f8b7c
/*  f0f87f0:	adc50000 */ 	sw	$a1,0x0($t6)
/*  f0f87f4:	3c1f8007 */ 	lui	$ra,0x8007
/*  f0f87f8:	27ff1998 */ 	addiu	$ra,$ra,0x1998
/*  f0f87fc:	8fef0000 */ 	lw	$t7,0x0($ra)
/*  f0f8800:	3c0d800a */ 	lui	$t5,0x800a
/*  f0f8804:	25ade700 */ 	addiu	$t5,$t5,-6400
/*  f0f8808:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0f880c:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0f8810:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0f8814:	030fc021 */ 	addu	$t8,$t8,$t7
/*  f0f8818:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0f881c:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0f8820:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0f8824:	01b8c821 */ 	addu	$t9,$t5,$t8
/*  f0f8828:	932e083b */ 	lbu	$t6,0x83b($t9)
/*  f0f882c:	3c03800a */ 	lui	$v1,0x800a
/*  f0f8830:	2463a630 */ 	addiu	$v1,$v1,-22992
/*  f0f8834:	000e7880 */ 	sll	$t7,$t6,0x2
/*  f0f8838:	006fc021 */ 	addu	$t8,$v1,$t7
/*  f0f883c:	8f190064 */ 	lw	$t9,0x64($t8)
/*  f0f8840:	3c0f8008 */ 	lui	$t7,0x8008
/*  f0f8844:	8def00f4 */ 	lw	$t7,0xf4($t7)
/*  f0f8848:	872e0634 */ 	lh	$t6,0x634($t9)
/*  f0f884c:	00005025 */ 	move	$t2,$zero
/*  f0f8850:	01cf001a */ 	div	$zero,$t6,$t7
/*  f0f8854:	0000c012 */ 	mflo	$t8
/*  f0f8858:	ad780000 */ 	sw	$t8,0x0($t3)
/*  f0f885c:	8ff90000 */ 	lw	$t9,0x0($ra)
/*  f0f8860:	15e00002 */ 	bnez	$t7,.JF0f0f886c
/*  f0f8864:	00000000 */ 	nop
/*  f0f8868:	0007000d */ 	break	0x7
.JF0f0f886c:
/*  f0f886c:	2401ffff */ 	li	$at,-1
/*  f0f8870:	15e10004 */ 	bne	$t7,$at,.JF0f0f8884
/*  f0f8874:	3c018000 */ 	lui	$at,0x8000
/*  f0f8878:	15c10002 */ 	bne	$t6,$at,.JF0f0f8884
/*  f0f887c:	00000000 */ 	nop
/*  f0f8880:	0006000d */ 	break	0x6
.JF0f0f8884:
/*  f0f8884:	001970c0 */ 	sll	$t6,$t9,0x3
/*  f0f8888:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f888c:	000e7080 */ 	sll	$t6,$t6,0x2
/*  f0f8890:	01d97021 */ 	addu	$t6,$t6,$t9
/*  f0f8894:	000e70c0 */ 	sll	$t6,$t6,0x3
/*  f0f8898:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f889c:	000e7100 */ 	sll	$t6,$t6,0x4
/*  f0f88a0:	01ae7821 */ 	addu	$t7,$t5,$t6
/*  f0f88a4:	91f8083b */ 	lbu	$t8,0x83b($t7)
/*  f0f88a8:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f88ac:	00797021 */ 	addu	$t6,$v1,$t9
/*  f0f88b0:	8dcf0064 */ 	lw	$t7,0x64($t6)
/*  f0f88b4:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f88b8:	85f80636 */ 	lh	$t8,0x636($t7)
/*  f0f88bc:	af380000 */ 	sw	$t8,0x0($t9)
/*  f0f88c0:	8fee0000 */ 	lw	$t6,0x0($ra)
/*  f0f88c4:	000e78c0 */ 	sll	$t7,$t6,0x3
/*  f0f88c8:	01ee7823 */ 	subu	$t7,$t7,$t6
/*  f0f88cc:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0f88d0:	01ee7821 */ 	addu	$t7,$t7,$t6
/*  f0f88d4:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0f88d8:	01ee7823 */ 	subu	$t7,$t7,$t6
/*  f0f88dc:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0f88e0:	01afc021 */ 	addu	$t8,$t5,$t7
/*  f0f88e4:	9319083b */ 	lbu	$t9,0x83b($t8)
/*  f0f88e8:	00197080 */ 	sll	$t6,$t9,0x2
/*  f0f88ec:	006e7821 */ 	addu	$t7,$v1,$t6
/*  f0f88f0:	8de20064 */ 	lw	$v0,0x64($t7)
/*  f0f88f4:	3c0f8008 */ 	lui	$t7,0x8008
/*  f0f88f8:	8def00f4 */ 	lw	$t7,0xf4($t7)
/*  f0f88fc:	84580630 */ 	lh	$t8,0x630($v0)
/*  f0f8900:	84590634 */ 	lh	$t9,0x634($v0)
/*  f0f8904:	03197021 */ 	addu	$t6,$t8,$t9
/*  f0f8908:	01cf001a */ 	div	$zero,$t6,$t7
/*  f0f890c:	0000c012 */ 	mflo	$t8
/*  f0f8910:	ad980000 */ 	sw	$t8,0x0($t4)
/*  f0f8914:	8ff90000 */ 	lw	$t9,0x0($ra)
/*  f0f8918:	15e00002 */ 	bnez	$t7,.JF0f0f8924
/*  f0f891c:	00000000 */ 	nop
/*  f0f8920:	0007000d */ 	break	0x7
.JF0f0f8924:
/*  f0f8924:	2401ffff */ 	li	$at,-1
/*  f0f8928:	15e10004 */ 	bne	$t7,$at,.JF0f0f893c
/*  f0f892c:	3c018000 */ 	lui	$at,0x8000
/*  f0f8930:	15c10002 */ 	bne	$t6,$at,.JF0f0f893c
/*  f0f8934:	00000000 */ 	nop
/*  f0f8938:	0006000d */ 	break	0x6
.JF0f0f893c:
/*  f0f893c:	001970c0 */ 	sll	$t6,$t9,0x3
/*  f0f8940:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f8944:	000e7080 */ 	sll	$t6,$t6,0x2
/*  f0f8948:	01d97021 */ 	addu	$t6,$t6,$t9
/*  f0f894c:	000e70c0 */ 	sll	$t6,$t6,0x3
/*  f0f8950:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f8954:	000e7100 */ 	sll	$t6,$t6,0x4
/*  f0f8958:	01ae7821 */ 	addu	$t7,$t5,$t6
/*  f0f895c:	91f8083b */ 	lbu	$t8,0x83b($t7)
/*  f0f8960:	0018c880 */ 	sll	$t9,$t8,0x2
/*  f0f8964:	00797021 */ 	addu	$t6,$v1,$t9
/*  f0f8968:	8dc20064 */ 	lw	$v0,0x64($t6)
/*  f0f896c:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f8970:	844f0632 */ 	lh	$t7,0x632($v0)
/*  f0f8974:	84580636 */ 	lh	$t8,0x636($v0)
/*  f0f8978:	01f8c821 */ 	addu	$t9,$t7,$t8
/*  f0f897c:	add90000 */ 	sw	$t9,0x0($t6)
/*  f0f8980:	8c65006c */ 	lw	$a1,0x6c($v1)
/*  f0f8984:	10a00003 */ 	beqz	$a1,.JF0f0f8994
/*  f0f8988:	00000000 */ 	nop
/*  f0f898c:	10000001 */ 	b	.JF0f0f8994
/*  f0f8990:	240a0001 */ 	li	$t2,0x1
.JF0f0f8994:
/*  f0f8994:	8c660068 */ 	lw	$a2,0x68($v1)
/*  f0f8998:	00003825 */ 	move	$a3,$zero
/*  f0f899c:	00002025 */ 	move	$a0,$zero
/*  f0f89a0:	10c00003 */ 	beqz	$a2,.JF0f0f89b0
/*  f0f89a4:	00000000 */ 	nop
/*  f0f89a8:	10000001 */ 	b	.JF0f0f89b0
/*  f0f89ac:	24070001 */ 	li	$a3,0x1
.JF0f0f89b0:
/*  f0f89b0:	8c680064 */ 	lw	$t0,0x64($v1)
/*  f0f89b4:	00001025 */ 	move	$v0,$zero
/*  f0f89b8:	11000003 */ 	beqz	$t0,.JF0f0f89c8
/*  f0f89bc:	00000000 */ 	nop
/*  f0f89c0:	10000001 */ 	b	.JF0f0f89c8
/*  f0f89c4:	24040001 */ 	li	$a0,0x1
.JF0f0f89c8:
/*  f0f89c8:	8c690070 */ 	lw	$t1,0x70($v1)
/*  f0f89cc:	11200003 */ 	beqz	$t1,.JF0f0f89dc
/*  f0f89d0:	00000000 */ 	nop
/*  f0f89d4:	10000001 */ 	b	.JF0f0f89dc
/*  f0f89d8:	24020001 */ 	li	$v0,0x1
.JF0f0f89dc:
/*  f0f89dc:	00447821 */ 	addu	$t7,$v0,$a0
/*  f0f89e0:	01e7c021 */ 	addu	$t8,$t7,$a3
/*  f0f89e4:	030ac821 */ 	addu	$t9,$t8,$t2
/*  f0f89e8:	2b210003 */ 	slti	$at,$t9,0x3
/*  f0f89ec:	14200026 */ 	bnez	$at,.JF0f0f8a88
/*  f0f89f0:	00005025 */ 	move	$t2,$zero
/*  f0f89f4:	8fee0000 */ 	lw	$t6,0x0($ra)
/*  f0f89f8:	24010002 */ 	li	$at,0x2
/*  f0f89fc:	3c05800a */ 	lui	$a1,0x800a
/*  f0f8a00:	000e78c0 */ 	sll	$t7,$t6,0x3
/*  f0f8a04:	01ee7823 */ 	subu	$t7,$t7,$t6
/*  f0f8a08:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0f8a0c:	01ee7821 */ 	addu	$t7,$t7,$t6
/*  f0f8a10:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0f8a14:	01ee7823 */ 	subu	$t7,$t7,$t6
/*  f0f8a18:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0f8a1c:	01afc021 */ 	addu	$t8,$t5,$t7
/*  f0f8a20:	9302083b */ 	lbu	$v0,0x83b($t8)
/*  f0f8a24:	3c06800a */ 	lui	$a2,0x800a
/*  f0f8a28:	50400004 */ 	beqzl	$v0,.JF0f0f8a3c
/*  f0f8a2c:	8d790000 */ 	lw	$t9,0x0($t3)
/*  f0f8a30:	1441000b */ 	bne	$v0,$at,.JF0f0f8a60
/*  f0f8a34:	3c08800a */ 	lui	$t0,0x800a
/*  f0f8a38:	8d790000 */ 	lw	$t9,0x0($t3)
.JF0f0f8a3c:
/*  f0f8a3c:	3c08800a */ 	lui	$t0,0x800a
/*  f0f8a40:	3c09800a */ 	lui	$t1,0x800a
/*  f0f8a44:	272e0016 */ 	addiu	$t6,$t9,0x16
/*  f0f8a48:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f8a4c:	8d29a6a0 */ 	lw	$t1,-0x5960($t1)
/*  f0f8a50:	8d08a694 */ 	lw	$t0,-0x596c($t0)
/*  f0f8a54:	8cc6a698 */ 	lw	$a2,-0x5968($a2)
/*  f0f8a58:	1000000b */ 	b	.JF0f0f8a88
/*  f0f8a5c:	8ca5a69c */ 	lw	$a1,-0x5964($a1)
.JF0f0f8a60:
/*  f0f8a60:	8d8f0000 */ 	lw	$t7,0x0($t4)
/*  f0f8a64:	3c05800a */ 	lui	$a1,0x800a
/*  f0f8a68:	3c06800a */ 	lui	$a2,0x800a
/*  f0f8a6c:	25f8ffea */ 	addiu	$t8,$t7,-22
/*  f0f8a70:	ad980000 */ 	sw	$t8,0x0($t4)
/*  f0f8a74:	3c09800a */ 	lui	$t1,0x800a
/*  f0f8a78:	8d29a6a0 */ 	lw	$t1,-0x5960($t1)
/*  f0f8a7c:	8d08a694 */ 	lw	$t0,-0x596c($t0)
/*  f0f8a80:	8cc6a698 */ 	lw	$a2,-0x5968($a2)
/*  f0f8a84:	8ca5a69c */ 	lw	$a1,-0x5964($a1)
.JF0f0f8a88:
/*  f0f8a88:	10a00003 */ 	beqz	$a1,.JF0f0f8a98
/*  f0f8a8c:	24010002 */ 	li	$at,0x2
/*  f0f8a90:	10000001 */ 	b	.JF0f0f8a98
/*  f0f8a94:	240a0001 */ 	li	$t2,0x1
.JF0f0f8a98:
/*  f0f8a98:	10c00003 */ 	beqz	$a2,.JF0f0f8aa8
/*  f0f8a9c:	00003825 */ 	move	$a3,$zero
/*  f0f8aa0:	10000001 */ 	b	.JF0f0f8aa8
/*  f0f8aa4:	24070001 */ 	li	$a3,0x1
.JF0f0f8aa8:
/*  f0f8aa8:	11000003 */ 	beqz	$t0,.JF0f0f8ab8
/*  f0f8aac:	00002025 */ 	move	$a0,$zero
/*  f0f8ab0:	10000001 */ 	b	.JF0f0f8ab8
/*  f0f8ab4:	24040001 */ 	li	$a0,0x1
.JF0f0f8ab8:
/*  f0f8ab8:	11200003 */ 	beqz	$t1,.JF0f0f8ac8
/*  f0f8abc:	00001025 */ 	move	$v0,$zero
/*  f0f8ac0:	10000001 */ 	b	.JF0f0f8ac8
/*  f0f8ac4:	24020001 */ 	li	$v0,0x1
.JF0f0f8ac8:
/*  f0f8ac8:	0044c821 */ 	addu	$t9,$v0,$a0
/*  f0f8acc:	03277021 */ 	addu	$t6,$t9,$a3
/*  f0f8ad0:	01ca7821 */ 	addu	$t7,$t6,$t2
/*  f0f8ad4:	55e1002a */ 	bnel	$t7,$at,.JF0f0f8b80
/*  f0f8ad8:	8fbf0014 */ 	lw	$ra,0x14($sp)
/*  f0f8adc:	0fc54978 */ 	jal	optionsGetScreenSplit
/*  f0f8ae0:	00000000 */ 	nop
/*  f0f8ae4:	24080001 */ 	li	$t0,0x1
/*  f0f8ae8:	3c0d800a */ 	lui	$t5,0x800a
/*  f0f8aec:	3c1f8007 */ 	lui	$ra,0x8007
/*  f0f8af0:	27ff1998 */ 	addiu	$ra,$ra,0x1998
/*  f0f8af4:	25ade700 */ 	addiu	$t5,$t5,-6400
/*  f0f8af8:	8fab0040 */ 	lw	$t3,0x40($sp)
/*  f0f8afc:	11020005 */ 	beq	$t0,$v0,.JF0f0f8b14
/*  f0f8b00:	8fac0048 */ 	lw	$t4,0x48($sp)
/*  f0f8b04:	3c188009 */ 	lui	$t8,0x8009
/*  f0f8b08:	93181160 */ 	lbu	$t8,0x1160($t8)
/*  f0f8b0c:	5518001c */ 	bnel	$t0,$t8,.JF0f0f8b80
/*  f0f8b10:	8fbf0014 */ 	lw	$ra,0x14($sp)
.JF0f0f8b14:
/*  f0f8b14:	8ff90000 */ 	lw	$t9,0x0($ra)
/*  f0f8b18:	001970c0 */ 	sll	$t6,$t9,0x3
/*  f0f8b1c:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f8b20:	000e7080 */ 	sll	$t6,$t6,0x2
/*  f0f8b24:	01d97021 */ 	addu	$t6,$t6,$t9
/*  f0f8b28:	000e70c0 */ 	sll	$t6,$t6,0x3
/*  f0f8b2c:	01d97023 */ 	subu	$t6,$t6,$t9
/*  f0f8b30:	000e7100 */ 	sll	$t6,$t6,0x4
/*  f0f8b34:	01ae7821 */ 	addu	$t7,$t5,$t6
/*  f0f8b38:	91f8083b */ 	lbu	$t8,0x83b($t7)
/*  f0f8b3c:	57000006 */ 	bnezl	$t8,.JF0f0f8b58
/*  f0f8b40:	8d8f0000 */ 	lw	$t7,0x0($t4)
/*  f0f8b44:	8d790000 */ 	lw	$t9,0x0($t3)
/*  f0f8b48:	272e0016 */ 	addiu	$t6,$t9,0x16
/*  f0f8b4c:	1000000b */ 	b	.JF0f0f8b7c
/*  f0f8b50:	ad6e0000 */ 	sw	$t6,0x0($t3)
/*  f0f8b54:	8d8f0000 */ 	lw	$t7,0x0($t4)
.JF0f0f8b58:
/*  f0f8b58:	25f8ffea */ 	addiu	$t8,$t7,-22
/*  f0f8b5c:	10000007 */ 	b	.JF0f0f8b7c
/*  f0f8b60:	ad980000 */ 	sw	$t8,0x0($t4)
.JF0f0f8b64:
/*  f0f8b64:	ad660000 */ 	sw	$a2,0x0($t3)
/*  f0f8b68:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0f8b6c:	af240000 */ 	sw	$a0,0x0($t9)
/*  f0f8b70:	ad870000 */ 	sw	$a3,0x0($t4)
/*  f0f8b74:	8fae004c */ 	lw	$t6,0x4c($sp)
/*  f0f8b78:	adc50000 */ 	sw	$a1,0x0($t6)
.JF0f0f8b7c:
/*  f0f8b7c:	8fbf0014 */ 	lw	$ra,0x14($sp)
.JF0f0f8b80:
/*  f0f8b80:	27bd0040 */ 	addiu	$sp,$sp,0x40
/*  f0f8b84:	03e00008 */ 	jr	$ra
/*  f0f8b88:	00000000 */ 	nop
);
#else
/**
 * Find the maximum possible dimensions for a dialog.
 *
 * This is pretty much the player's viewport with some adjustments made for
 * padding. There are some trickier calculations for MP setup where the players
 * are sharing a viewport.
 */
void menuFindAvailableSize(s32 *leftptr, s32 *topptr, s32 *rightptr, s32 *bottomptr)
{
	s32 left = viGetViewLeft() / g_ScaleX + 20;
	s32 top = viGetViewTop() + 4;
	s32 right = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX - 20;
	s32 bottom = viGetViewTop() + viGetViewHeight() - 4;
	s32 playernum;
	u32 stack[2];

	if (currentPlayerIsMenuOpenInSoloOrMp()) {
		// Make room for health bar
		top += 22;

		if (optionsGetEffectiveScreenSize() == SCREENSIZE_CINEMA) {
			top -= 8;
			bottom += 4;
		}
	}

	switch (g_MenuData.root) {
	case MENUROOT_MPSETUP:
	case MENUROOT_4MBMAINMENU:
		playernum = g_Menus[g_MpPlayerNum].playernum;

		// Make room for the "Press START" labels
		if (g_MenuData.playerjoinalpha[0] > 0 || g_MenuData.playerjoinalpha[1] > 0) {
			top += 10;
		}

		if (g_MenuData.playerjoinalpha[2] > 0 || g_MenuData.playerjoinalpha[3] > 0) {
			bottom -= 10;
		}

		switch (g_MpNumJoined) {
		case 1:
			// 1 player - the full screen is available
			*leftptr = left;
			*rightptr = right;
			*topptr = top;
			*bottomptr = bottom;
			return;
		case 2:
			// 2 players - left/right halves each
			*topptr = top;
			*bottomptr = bottom;

			if (playernum == 0) {
				*leftptr = left;
				*rightptr = (left + right) / 2;
			} else {
				*leftptr = (left + right) / 2;
				*rightptr = right;
			}
			return;
		case 3:
			// 3 players - top two quarters and the third takes the full bottom
			if (playernum == 0 || playernum == 1) {
				*topptr = top;
				*bottomptr = (top + bottom) / 2;

				if (playernum == 0) {
					*leftptr = left;
					*rightptr = (left + right) / 2;
				} else {
					*leftptr = (left + right) / 2;
					*rightptr = right;
				}
			} else {
				*topptr = (top + bottom) / 2;
				*bottomptr = bottom;
				*leftptr = left;
				*rightptr = right;
			}
			break;
		case 4:
			// 4 players - quarters
			if (playernum == 0 || playernum == 2) {
				*leftptr = left;
				*rightptr = (left + right) / 2;
			} else {
				*leftptr = (left + right) / 2;
				*rightptr = right;
			}
			if (playernum == 0 || playernum == 1) {
				*topptr = top;
				*bottomptr = (top + bottom) / 2;
			} else {
				*topptr = (top + bottom) / 2;
				*bottomptr = bottom;
			}
			break;
		}
		break;
	case MENUROOT_MPPAUSE:
	case MENUROOT_MPENDSCREEN:
	case MENUROOT_PICKTARGET:
	case MENUROOT_4MBFILEMGR:
		*leftptr = g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewleft / g_ScaleX;
		*topptr = g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewtop;
		*rightptr = (g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewleft + g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewwidth) / g_ScaleX;
		*bottomptr = g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewtop + g_Vars.players[g_Menus[g_MpPlayerNum].playernum]->viewheight;

		if (PLAYERCOUNT() > 2) {
			if (g_Menus[g_MpPlayerNum].playernum == 0 || g_Menus[g_MpPlayerNum].playernum == 2) {
				*leftptr += 22;
			} else {
				*rightptr -= 22;
			}
		}

#if VERSION >= VERSION_NTSC_1_0
		if (PLAYERCOUNT() == 2 && (optionsGetScreenSplit() == SCREENSPLIT_VERTICAL || IS4MB()))
#else
		if (PLAYERCOUNT() == 2 && optionsGetScreenSplit() == SCREENSPLIT_VERTICAL)
#endif
		{
			if (g_Menus[g_MpPlayerNum].playernum == 0) {
				*leftptr += 22;
			} else {
				*rightptr -= 22;
			}
		}
		break;
	default:
		*leftptr = left;
		*topptr = top;
		*rightptr = right;
		*bottomptr = bottom;
		break;
	}
}
#endif

void dialogCalculatePosition(struct menudialog *dialog)
{
	s32 xmin;
	s32 xmax;
	s32 ymin;
	s32 ymax;
	s32 width;
	s32 height;
	s32 vdir;
	s32 hdir;

#if VERSION >= VERSION_JPN_FINAL
	menuFindAvailableSize(&xmin, &ymin, &xmax, &ymax, dialog);
#else
	menuFindAvailableSize(&xmin, &ymin, &xmax, &ymax);
#endif

	height = ymax - ymin - 6;
	width = xmax - xmin - 6;

	if (width > dialog->contentwidth) {
		width = dialog->contentwidth;
	}

	if (height > dialog->contentheight) {
		height = dialog->contentheight;
	}

	dialog->dstx = (xmax + xmin - width) / 2;
	dialog->dsty = (ymin + ymax - height) / 2;
	dialog->dstwidth = width;
	dialog->dstheight = height;

	if (dialog->swipedir != 0) {
		func0f0f7594(dialog->swipedir, &vdir, &hdir);

		if (hdir < 0) {
			dialog->dstx = -4 - dialog->dstwidth;
		}

		if (hdir > 0) {
			dialog->dstx = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX + 4;
		}

		if (vdir < 0) {
			dialog->dsty = -4 - dialog->dstheight;
		}

		if (vdir > 0) {
			dialog->dsty = viGetViewTop() + viGetViewHeight() + 4;
		}
	}
}

void menuClose(void)
{
	g_Menus[g_MpPlayerNum].depth = 0;
	g_Menus[g_MpPlayerNum].numdialogs = 0;
	g_Menus[g_MpPlayerNum].rowend = 0;
	g_Menus[g_MpPlayerNum].colend = 0;
	g_Menus[g_MpPlayerNum].blockend = 0;
	g_Menus[g_MpPlayerNum].curdialog = NULL;
	g_Menus[g_MpPlayerNum].openinhibit = 10;

	if (g_MenuData.root == MENUROOT_MPPAUSE) {
		g_PlayersWithControl[g_Menus[g_MpPlayerNum].playernum] = true;
	}

	g_MenuData.count--;

	if (g_MenuData.root == MENUROOT_MPPAUSE && g_Vars.currentplayer->activemenumode == AMMODE_EDIT) {
		g_Vars.currentplayer->activemenumode = AMMODE_VIEW;
	}

	if (g_MenuData.root == MENUROOT_BOOTPAKMGR) {
		mainChangeToStage(STAGE_TITLE);
	}
}

void func0f0f8120(void)
{
	struct menudialog *prev = g_Menus[g_MpPlayerNum].curdialog;
	s32 i;

	if (g_MenuData.unk66e > 0) {
		for (i = g_MenuData.unk66e; i >= 0; i--) {
			func0f0f3220(i);
		}
	}

	if (g_Menus[g_MpPlayerNum].curdialog == prev) {
		while (g_Menus[g_MpPlayerNum].depth > 0) {
			menuPopDialog();
		}
	}
}

void func0f0f820c(struct menudialogdef *dialogdef, s32 root)
{
	s32 i;
	s32 prevplayernum = g_MpPlayerNum;

	for (i = 0; i < 4; i++) {
		if (g_Menus[i].curdialog) {
			g_MpPlayerNum = i;
			func0f0f8120();
		}
	}

	g_MpPlayerNum = prevplayernum;

	g_MenuData.unk008 = root;
	g_MenuData.unk00c = dialogdef;
}

bool menuSetBackground(s32 bg)
{
	// Can only screenshot if there is no background already,
	// because we want a clean screenshot
	bool screenshot = g_MenuData.bg == 0;

	// If there's already a screenshottable background queued,
	// there's no need to create another
	if (g_MenuData.nextbg == MENUBG_BLUR || g_MenuData.nextbg == MENUBG_CONEALPHA) {
		screenshot = false;
	}

	if (g_MenuData.bg != bg) {
		g_MenuData.nextbg = bg;
	}

	if (screenshot && g_MenuData.bg == 0) {
		g_MenuData.screenshottimer = 1;
	}
}

void func0f0f8300(void)
{
	s32 i;

	if (g_MenuData.count == 0) {
		for (i = 0; i < 4; i++) {
			g_MenuData.playerjoinalpha[i] = 0;
		}
	}
}

void menuPushRootDialog(struct menudialogdef *dialogdef, s32 root)
{
	g_Menus[g_MpPlayerNum].numdialogs = 0;
	g_Menus[g_MpPlayerNum].depth = 0;

	g_MenuData.unk5d5_01 = false;
	g_MenuData.unk5d5_04 = false;

	g_PlayersWithControl[g_Menus[g_MpPlayerNum].playernum] = false;

	func0f0f1494();

	g_MenuData.count++;

	if (root == MENUROOT_PICKTARGET) {
		g_Menus[g_MpPlayerNum].mppause.unke40_00 = true;
		root = MENUROOT_MPPAUSE;
	}

	switch (root) {
	case MENUROOT_ENDSCREEN:
	case MENUROOT_MAINMENU:
	case MENUROOT_FILEMGR:
	case MENUROOT_BOOTPAKMGR:
	case MENUROOT_PICKTARGET:
	case MENUROOT_COOPCONTINUE:
	case MENUROOT_4MBFILEMGR:
	case MENUROOT_TRAINING:
		g_MenuData.count = 1;
		break;
	}

	g_Menus[g_MpPlayerNum].unk820 = 1;

	g_MenuData.root = root;
	g_MenuData.unk008 = -1;
	g_MenuData.unk5d5_02 = false;

	if (root == MENUROOT_MAINMENU
			|| root == MENUROOT_MPSETUP
			|| root == MENUROOT_TRAINING
			|| root == MENUROOT_FILEMGR) {
		if (IS8MB() && (g_MenuData.unk5d4 == 0 || g_MenuData.unk01c.unk5b1_04)) {
			if (!g_MenuData.unk5d5_04) {
				g_MenuData.unk5d5_05 = true;
			}
		}
	}

	menuPushDialog(dialogdef);

	switch (root) {
	case MENUROOT_MPSETUP:
		menuSetBackground(MENUBG_CONEALPHA);
		break;
	case MENUROOT_4MBFILEMGR:
		musicStartMenu();
		g_MenuData.bg = MENUBG_CONEOPAQUE;
		break;
	case MENUROOT_4MBMAINMENU:
		g_MenuData.bg = MENUBG_CONEOPAQUE;
		break;
	case MENUROOT_ENDSCREEN:
		if (dialogdef->type == MENUDIALOGTYPE_DANGER) {
			g_MenuData.unk5d4 = 0;
			g_MenuData.nextbg = MENUBG_FAILURE;
			break;
		}
		// fall-through
	case MENUROOT_MAINMENU:
	case MENUROOT_MPENDSCREEN:
	case MENUROOT_FILEMGR:
	case MENUROOT_COOPCONTINUE:
	case MENUROOT_TRAINING:
		menuSetBackground(MENUBG_BLUR);
		break;
	case MENUROOT_BOOTPAKMGR:
		musicStartMenu();
		g_MenuData.bg = MENUBG_GRADIENT;
		break;
	}

	if (menuIsSoloMissionOrMp()) {
		playerDisplayHealth();
	}
}

void func0f0f85e0(struct menudialogdef *dialogdef, s32 root)
{
	if (dialogdef == &g_CiMenuViaPcMenuDialog) {
		musicStartMenu();
	}

	menuPushRootDialog(dialogdef, root);
	lvSetPaused(true);
	g_Vars.currentplayer->pausemode = PAUSEMODE_PAUSED;
}

u32 g_MenuCThresh = 120;

Gfx *menuRenderDialog(Gfx *gdl, struct menudialog *dialog, struct menu *menu, bool lightweight)
{
	mainOverrideVariable("cthresh", &g_MenuCThresh);

	textSetWaveBlend(dialog->unk54, dialog->unk58, g_MenuCThresh);

	gdl = dialogRender(gdl, dialog, menu, lightweight);

	textResetBlends();

	return gdl;
}

const char var7f1b2700[] = "[]-[] slide from %d";
const char var7f1b2714[] = " to %d\n";
const char var7f1b271c[] = "UNPAUSE: enabling control 0\n";
const char var7f1b273c[] = "file: type %d guid %x-%x data %x err %d\n";
const char var7f1b2768[] = "StartSelects\n";

/**
 * Render all dialogs for the current player.
 *
 * There's usually only one on screen at a time, but there can be multiple when
 * transitioning between dialogs. This happens when swiping left or right beteen
 * dialogs on the same layer, or when opening or closing dialogs.
 */
Gfx *menuRenderDialogs(Gfx *gdl)
{
	if (g_Menus[g_MpPlayerNum].curdialog) {
		if (g_MenuData.root == MENUROOT_MPPAUSE
				|| g_MenuData.root == MENUROOT_PICKTARGET
				|| g_MenuData.root == MENUROOT_MPENDSCREEN) {
			var8009de98 = g_Menus[g_MpPlayerNum].curdialog->x + g_Menus[g_MpPlayerNum].curdialog->width / 2 - viGetWidth() / (g_ScaleX * 2);
			var8009de9c = g_Menus[g_MpPlayerNum].curdialog->y + g_Menus[g_MpPlayerNum].curdialog->height / 2 - viGetHeight() / 2;

			gdl = menuRenderDialog(gdl, g_Menus[g_MpPlayerNum].curdialog, &g_Menus[g_MpPlayerNum], 0);
		} else {
			s32 i;
			s32 j;

#if VERSION >= VERSION_NTSC_1_0
			// NTSC 1.0 and newer renders one "other" dialog plus the current
			// dialog. The other dialog is only rendered if on-screen, and it
			// ensures the current dialog is drawn last.
			struct menudialog *dialogs[] = { NULL, NULL };

			for (i = 0; i < g_Menus[g_MpPlayerNum].depth; i++) {
				struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[i];

				for (j = 0; j < layer->numsiblings; j++) {
					if (layer->siblings[j] != g_Menus[g_MpPlayerNum].curdialog) {
						if (layer->siblings[j]->type != 0 || layer->siblings[j]->transitionfrac >= 0) {
							dialogs[0] = layer->siblings[j];
						}
					}
				}
			}

			// Render the other dialog if any
			if (dialogs[0]) {
				gdl = menuRenderDialog(gdl, dialogs[0], &g_Menus[g_MpPlayerNum], 0);
			}

			// Render the current dialog
			if (g_Menus[g_MpPlayerNum].curdialog) {
				gdl = menuRenderDialog(gdl, g_Menus[g_MpPlayerNum].curdialog, &g_Menus[g_MpPlayerNum], 0);
			}
#else
			// NTSC beta renders all dialogs all the time, and in their natural order
			for (i = 0; i < g_Menus[g_MpPlayerNum].depth; i++) {
				struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[i];

				for (j = 0; j < layer->numsiblings; j++) {
					gdl = menuRenderDialog(gdl, layer->siblings[j], &g_Menus[g_MpPlayerNum], 0);
				}
			}
#endif
		}

		// Render banner messages
		if (g_Menus[g_MpPlayerNum].bannernum != -1
				&& (g_Menus[g_MpPlayerNum].curdialog->definition->flags & MENUDIALOGFLAG_DISABLEBANNER) == 0) {
			if (g_MenuData.count >= 2) {
				s32 xmin;
				s32 ymin;
				s32 xmax;
				s32 ymax;

#if VERSION >= VERSION_JPN_FINAL
				menuFindAvailableSize(&xmin, &ymin, &xmax, &ymax, NULL);
#else
				menuFindAvailableSize(&xmin, &ymin, &xmax, &ymax);
#endif

#if VERSION >= VERSION_NTSC_1_0
				gdl = menuRenderBanner(gdl, xmin, ymin, xmax, ymax, false, g_Menus[g_MpPlayerNum].bannernum, 0, 0);
#else
				gdl = menuRenderBanner(gdl, xmin, ymin, xmax, ymax, false, g_Menus[g_MpPlayerNum].bannernum);
#endif
			} else {
				s32 xmin = viGetViewLeft() / g_ScaleX;
				s32 ymin = viGetViewTop();
				s32 xmax = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX;
				s32 ymax = viGetViewTop() + viGetViewHeight();

#if VERSION >= VERSION_NTSC_1_0
				gdl = menuRenderBanner(gdl, xmin, ymin, xmax, ymax, true, g_Menus[g_MpPlayerNum].bannernum, 0, 0);
#else
				gdl = menuRenderBanner(gdl, xmin, ymin, xmax, ymax, true, g_Menus[g_MpPlayerNum].bannernum);
#endif
			}
		}
	}

	return gdl;
}

u32 var800714e8 = 0;

void func0f0f8bb4(struct menu840 *arg0, u32 arg1, u32 arg2)
{
	arg0->unk008 = arg1;
	arg0->unk004 = arg2 ? mempAlloc(arg1, MEMPOOL_STAGE) : NULL;
	arg0->unk54c = 0.0f;
	arg0->unk000 = 0;
	arg0->unk00c = 0xffff;
	arg0->bodymodeldef = NULL;
	arg0->unk010 = 0;
	arg0->unk56c = 0;
	arg0->unk570 = 0;
	arg0->partvisibility = NULL;
	arg0->unk5b1_01 = false;
	arg0->unk5b1_02 = false;
	arg0->unk5b1_04 = false;
	arg0->unk5b1_06 = false;
	arg0->headnum = -1;
	arg0->bodynum = -1;
	arg0->unk538 = 0.0f;
	arg0->unk510 = 0.0f;
	arg0->unk53c = 0.0f;
	arg0->unk514 = 0.0f;
	arg0->unk540 = 0.0f;
	arg0->unk518 = 0.0f;
	arg0->unk544 = 0.0f;
	arg0->unk51c = 0.0f;
	arg0->unk548 = 0.0f;
	arg0->unk520 = 0.0f;
	arg0->unk550 = 0.0f;
	arg0->unk528 = 0.0f;
	arg0->unk534 = 0.0f;
	arg0->unk530 = 0.0f;
	arg0->unk52c = 0.0f;
	arg0->unk524 = arg0->unk54c;
}

void menuReset(void)
{
	s32 i;

	func0f110bf0();

	var8009dfc0 = 0;

	if (IS8MB()) {
		g_BlurBuffer = mempAlloc(0x4b00, MEMPOOL_STAGE);
	}

	g_MenuData.unk5d5_01 = false;

	texLoadFromConfig(&g_TexGeneralConfigs[1]);
	texLoadFromConfig(&g_TexGeneralConfigs[6]);
	texLoadFromConfig(&g_TexGeneralConfigs[51]);
	texLoadFromConfig(&g_TexGeneralConfigs[52]);
	texLoadFromConfig(&g_TexGeneralConfigs[53]);
	texLoadFromConfig(&g_TexGeneralConfigs[54]);
	texLoadFromConfig(&g_TexGeneralConfigs[55]);

	if (g_Vars.stagenum == STAGE_CITRAINING) {
		for (i = 12; i < 34; i++) {
			texLoadFromConfig(&g_TexGeneralConfigs[i]);
		}

		texLoadFromConfig(&g_TexGeneralConfigs[34]);
		texLoadFromConfig(&g_TexGeneralConfigs[36]);
		texLoadFromConfig(&g_TexGeneralConfigs[35]);
	}

	if (g_Vars.mplayerisrunning) {
		texLoadFromConfig(&g_TexGeneralConfigs[35]);
	}

	for (i = 0; i < 4; i++) {
		g_Menus[i].unk840.unk004 = NULL;
	}

	g_MenuData.unk01c.unk004 = NULL;

	if (g_Vars.stagenum == STAGE_CITRAINING) {
		g_MissionConfig.iscoop = false;
		g_MissionConfig.isanti = false;
		g_MissionConfig.pdmode = false;
	}

	if (!g_Vars.mplayerisrunning) {
		s32 max = 0;

		if (g_Vars.stagenum == STAGE_CITRAINING) {
			max = 4;
		}

		if (g_Vars.stagenum == STAGE_4MBMENU) {
			max = 4;
		}

		for (i = 0; i < max; i++) {
			func0f0f8bb4(&g_Menus[i].unk840, IS4MB() ? 0xb400 : 0x25800, 1);
		}

		if (IS8MB()) {
			func0f0f8bb4(&g_MenuData.unk01c, 0xc800, 1);
		}

		g_MenuData.unk01c.unk00c = 0x259;
		g_MenuData.unk01c.unk524 = g_MenuData.unk01c.unk54c = -M_PI;
		g_MenuData.unk01c.unk520 = g_MenuData.unk01c.unk548 = 0;
		g_MenuData.unk01c.unk528 = g_MenuData.unk01c.unk550 = 0;
		g_MenuData.unk01c.unk510 = g_MenuData.unk01c.unk538 = -205.5f;
		g_MenuData.unk01c.unk514 = g_MenuData.unk01c.unk53c = 244.7f;
		g_MenuData.unk01c.unk518 = g_MenuData.unk01c.unk540 = 68.3f;
		g_MenuData.unk01c.unk51c = g_MenuData.unk01c.unk544 = 0.12209f;
		g_MenuData.unk01c.unk5b1_01 = false;
		g_MenuData.unk01c.unk574 = 0;
		g_MenuData.unk01c.unk580 = 0;
	}

	g_MenuData.unk5d4 = 0;
	g_MenuData.unk5d5_05 = false;

	for (i = 0; i < 4; i++) {
		g_Menus[i].curdialog = NULL;
		g_Menus[i].depth = 0;
		g_Menus[i].numdialogs = 0;
		g_Menus[i].unk820 = 0;
		g_Menus[i].rowend = 0;
		g_Menus[i].blockend = 0;
		g_Menus[i].colend = 0;
		g_Menus[i].bannernum = -1;
		g_Menus[i].fm.unke41 = 0;
		g_Menus[i].fm.unke64 = 0;
		g_Menus[i].fm.headtextures = NULL;
	}

	g_MenuData.unk668 = -1;
	g_MenuData.unk00c = 0;
	g_MenuData.unk008 = -1;
	g_MenuData.count = 0;
	g_MenuData.root = 0;
	g_MenuData.unk010 = 0;
	g_MenuData.bg = 0;
	g_MenuData.unk5d5_06 = false;
	g_MenuData.nextbg = 255;
	g_MenuData.bannernum = -1;

	for (i = 0; i < 5; i++) {
		g_MenuData.unk669[i] = 0xff;
	}

	g_MenuData.unk66e = 0;
	g_MenuData.unk66f = 0;
}

void menuSwipe(s32 direction)
{
	struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth - 1];
	struct menuitem *item;
	union handlerdata sp50;
	union handlerdata sp40;

	if (layer->numsiblings >= 2) {
		g_Menus[g_MpPlayerNum].curdialog->swipedir = -direction;

		layer->cursibling += (s8)direction;

		if (layer->cursibling < 0) {
			layer->cursibling = layer->numsiblings - 1;
		}

		if (layer->cursibling >= layer->numsiblings) {
			layer->cursibling = 0;
		}

		g_Menus[g_MpPlayerNum].curdialog = layer->siblings[layer->cursibling];

		if (direction == 1) {
			g_Menus[g_MpPlayerNum].curdialog->focuseditem = dialogFindFirstItem(g_Menus[g_MpPlayerNum].curdialog);
		} else {
			g_Menus[g_MpPlayerNum].curdialog->focuseditem = dialogFindFirstItemRight(g_Menus[g_MpPlayerNum].curdialog);
		}

		item = g_Menus[g_MpPlayerNum].curdialog->definition->items;

		while (item->type != MENUITEMTYPE_END) {
			if (item->handler
					&& (item->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0
					&& item->handler(MENUOP_CHECKPREFOCUSED, item, &sp50)) {
				g_Menus[g_MpPlayerNum].curdialog->focuseditem = item;
			}

			item++;
		}

		if (g_Menus[g_MpPlayerNum].curdialog->focuseditem != 0
				&& g_Menus[g_MpPlayerNum].curdialog->focuseditem->handler
				&& ((g_Menus[g_MpPlayerNum].curdialog->focuseditem->flags & MENUITEMFLAG_SELECTABLE_OPENSDIALOG) == 0)) {
			g_Menus[g_MpPlayerNum].curdialog->focuseditem->handler(MENUOP_FOCUS, g_Menus[g_MpPlayerNum].curdialog->focuseditem, &sp40);
		}

		g_Menus[g_MpPlayerNum].curdialog->swipedir = direction;

		dialogCalculatePosition(g_Menus[g_MpPlayerNum].curdialog);

		g_Menus[g_MpPlayerNum].curdialog->x = g_Menus[g_MpPlayerNum].curdialog->dstx;
		g_Menus[g_MpPlayerNum].curdialog->y = g_Menus[g_MpPlayerNum].curdialog->dsty;
		g_Menus[g_MpPlayerNum].curdialog->swipedir = 0;
		g_Menus[g_MpPlayerNum].curdialog->state = MENUDIALOGSTATE_PREOPEN;
		g_Menus[g_MpPlayerNum].curdialog->statefrac = 0.0f;

		func0f0f37a4(&g_Menus[g_MpPlayerNum].unk840);

		menuPlaySound(MENUSOUND_SWIPE);
	}
}

extern struct menudialogdef g_MpDropOut4MbMenuDialog;

void dialogTick(struct menudialog *dialog, struct menuinputs *inputs, u32 tickflags)
{
	bool usedefaultbehaviour;
	struct menudialogdef *definition;
	struct menu *menu;
	u8 transitiontotype;
	s32 oldfracint;
	s32 oldx;
	s32 col;
	f32 newx;
	union handlerdata data;
	struct menuinputs spd8;
	s32 oldy;
	f32 newy;
	s32 oldwidth;
	f32 newwidth;
	s32 oldheight;
	f32 newheight;
	s32 i;
	u32 stack;

	usedefaultbehaviour = false;
	definition = dialog->definition;
	menu = &g_Menus[g_MpPlayerNum];

	if (g_Menus[g_MpPlayerNum].fm.unke40_00 || g_MainIsDebugMenuOpen) {
		inputs->leftright = inputs->updown = inputs->select = inputs->back = inputs->xaxis = inputs->yaxis = inputs->shoulder = inputs->back2 = inputs->unk14 = 0;
		g_Menus[g_MpPlayerNum].fm.unke40_00 = false;
	}

	g_Menus[g_MpPlayerNum].fm.unke40_00 = false;

	spd8.select = 0;
	spd8.back = inputs->back;
	spd8.leftright = inputs->leftright;
	spd8.updown = inputs->updown;
	spd8.xaxis = inputs->xaxis;
	spd8.yaxis = inputs->yaxis;
	spd8.leftrightheld = inputs->leftrightheld;
	spd8.updownheld = inputs->updownheld;
	spd8.start = 0;
	spd8.unk0c = inputs->unk0c;
	spd8.unk10 = inputs->unk10;

	dialog->unk54++;
	dialog->unk5c += g_Vars.diffframe60;
	dialog->unk54 += dialog->unk5c / 9;
	dialog->unk5c %= 9;
	dialog->unk54 %= g_MenuCThresh;

	// For endscreens, handle transitioning of background and dialog type
	if (dialog->transitionfrac < 0.0f) {
		// Transition not yet started
		if (dialog == g_Menus[g_MpPlayerNum].curdialog) {
			transitiontotype = definition->type;

			if (mpIsPlayerLockedOut(g_MpPlayerNum) && (dialog->definition->flags & MENUDIALOGFLAG_MPLOCKABLE)) {
				transitiontotype = MENUDIALOGTYPE_DANGER;
			}

			if (g_MenuData.root == MENUROOT_ENDSCREEN) {
				if (g_MenuData.bg == MENUBG_8 || g_MenuData.bg == MENUBG_SUCCESS) {
					transitiontotype = MENUDIALOGTYPE_DEFAULT;
				}

				if (g_StageIndex == STAGEINDEX_DEFENSE
						&& g_MenuData.bg != MENUBG_FAILURE
						&& g_MenuData.nextbg != MENUBG_FAILURE) {
					transitiontotype = MENUDIALOGTYPE_WHITE;
					dialog->type = MENUDIALOGTYPE_WHITE;
				}
			}

			if (dialog->type != transitiontotype) {
				dialog->type2 = transitiontotype;
				dialog->colourweight = 0;
				dialog->transitionfrac = 0;
			}
		} else if (dialog->type != 0) {
			dialog->type2 = 0;
			dialog->colourweight = 0;
			dialog->transitionfrac = 0;
		}
	} else {
		// Mid-transition
		if (g_MenuData.root == MENUROOT_ENDSCREEN
				&& g_StageIndex == STAGEINDEX_DEFENSE
				&& g_MenuData.bg != MENUBG_FAILURE
				&& g_MenuData.nextbg != MENUBG_FAILURE
				&& dialog->type2 != 0) {
			dialog->type2 = MENUDIALOGTYPE_WHITE;
		}

		// Success endscreens transition to the background slower than failures
		if (g_MenuData.root == MENUROOT_ENDSCREEN
				&& dialog->type == MENUDIALOGTYPE_SUCCESS
				&& (g_MenuData.bg == MENUBG_8 || g_MenuData.bg == MENUBG_SUCCESS)) {
#if VERSION >= VERSION_PAL_BETA
			dialog->transitionfrac += g_Vars.diffframe60freal * 0.01f;
#else
			dialog->transitionfrac += g_Vars.diffframe60f * 0.01f;
#endif
		} else {
#if VERSION >= VERSION_PAL_BETA
			dialog->transitionfrac += g_Vars.diffframe60freal * 0.042f;
#else
			dialog->transitionfrac += g_Vars.diffframe60f * 0.042f;
#endif
		}

		if (dialog->transitionfrac > 1.0f) {
			dialog->transitionfrac = -1.0f;
			dialog->type = dialog->type2;
		}

		dialog->colourweight = dialog->transitionfrac * 255.0f;
	}

	// When fully open and ready, the dialog shows the text fully opaque for
	// 2 seconds (redrawtimer < 0.0f) then starts the redraw timer and waits for
	// it to finish before repeating the process. While the redraw timer is
	// active, the text begins to fade before being redrawn.
	if (dialog->state == MENUDIALOGSTATE_POPULATED) {
		if (g_MenuData.nextbg != MENUBG_CONEALPHA) {
			if (dialog->redrawtimer < 0.0f) {
#if VERSION >= VERSION_PAL_BETA
				dialog->statefrac += g_Vars.diffframe60freal / 120.0f;
#else
				dialog->statefrac += g_Vars.diffframe60f / 120.0f;
#endif

				if (dialog->statefrac > 1.0f) {
					dialog->redrawtimer = 0.0f;
				}
			} else {
				dialog->statefrac = 0.0f;
			}
		}
	}

	// When populating the text for the first time, wait for both the redraw
	// timer to finish and a minimum duration.
	if (dialog->state == MENUDIALOGSTATE_POPULATING) {
#if VERSION >= VERSION_PAL_BETA
		dialog->statefrac -= 0.05f * g_Vars.diffframe60freal;
#else
		dialog->statefrac -= 0.05f * g_Vars.diffframe60f;
#endif

		if (dialog->statefrac < 0.0f) {
			dialog->statefrac = 0.0f;

			if (dialog->redrawtimer < 0.0f) {
				dialog->state = MENUDIALOGSTATE_POPULATED;
			}
		}
	}

	// When opening the dialog, the height increases gradually over several
	// frames. This part doesn't actually change the height - it updates the
	// statefrac based on the current height.
	if (dialog->state == MENUDIALOGSTATE_OPENING) {
		oldfracint = dialog->statefrac;

		if (dialog->statefrac != dialog->height) {
			for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
				dialog->statefrac = dialog->height * 0.235f + 0.765f * dialog->statefrac;
#else
				dialog->statefrac = dialog->height * 0.2f + 0.8f * dialog->statefrac;
#endif
			}
		}

		if ((s32)dialog->statefrac == oldfracint) {
			dialog->statefrac = oldfracint + 1.0f;
		}

		if (dialog->statefrac > dialog->height - 1.0f && dialog->statefrac < dialog->height + 1.0f) {
			dialog->state = MENUDIALOGSTATE_POPULATING;
			dialog->statefrac = 1.0f;
		}
	}

#if VERSION >= VERSION_NTSC_1_0
	if (dialog->state == MENUDIALOGSTATE_PREOPEN) {
		if (dialog->definition == &g_MpReadyMenuDialog) {
			if (dialog->statefrac < 0.1f) {
				dialog->statefrac += 0.04f;
			} else {
				dialog->state = MENUDIALOGSTATE_OPENING;
				dialog->redrawtimer = 0.0f;
				dialog->statefrac = 0.5f;
			}
		} else if ((g_Vars.coopplayernum >= 0 || g_Vars.antiplayernum >= 0) && menuGetRoot() == MENUROOT_MPENDSCREEN) {
			if (var8009dfc0) {
#if VERSION >= VERSION_PAL_BETA
				dialog->statefrac += g_Vars.diffframe240freal / 60.0f;
#else
				dialog->statefrac += g_Vars.diffframe240 / 60.0f;
#endif

				if (dialog->statefrac > 1.0f) {
					dialog->state = MENUDIALOGSTATE_OPENING;
					dialog->redrawtimer = 0.0f;
					dialog->statefrac = 0.5f;
				}
			}
		} else {
			if (g_MenuData.nextbg == 255 || g_MenuData.bg != 0) {
				dialog->state = MENUDIALOGSTATE_OPENING;
				dialog->redrawtimer = 0.0f;
				dialog->statefrac = 0.5f;
			}
		}
	}
#else
	if (dialog->state == MENUDIALOGSTATE_PREOPEN) {
		if (g_MenuData.nextbg == 255 || g_MenuData.bg != 0) {
			dialog->state = MENUDIALOGSTATE_OPENING;
			dialog->redrawtimer = 0.0f;
			dialog->statefrac = 0.5f;
		}
	}
#endif

	// Increment the redraw timer, which makes the contents fade and then get
	// redrawn. The initial draw is done faster than subsequent draws.
	if (dialog->redrawtimer < 0.0f) {
		// Redraw not active
	} else {
#if VERSION >= VERSION_PAL_BETA
		if (dialog->state == MENUDIALOGSTATE_POPULATED) {
			dialog->redrawtimer += g_Vars.diffframe60freal + g_Vars.diffframe60freal;
		} else {
			dialog->redrawtimer += 5 * g_Vars.diffframe60freal;
		}
#else
		if (dialog->state == MENUDIALOGSTATE_POPULATED) {
			dialog->redrawtimer += 2 * g_Vars.diffframe60;
		} else {
			dialog->redrawtimer += 5 * g_Vars.diffframe60;
		}
#endif

		if (dialog->redrawtimer > 600.0f) {
			dialog->redrawtimer = -1.0f;
		}
	}

	if ((dialog->definition->flags & MENUDIALOGFLAG_DISABLERESIZE) == 0) {
		dialogCalculateContentSize(dialog->definition, dialog, menu);
	}

	dialogCalculatePosition(dialog);
	dialog0f0f1ef4(dialog);

	// Update slide
	if (g_MenuData.root == MENUROOT_MPPAUSE
			|| g_MenuData.root == MENUROOT_PICKTARGET
			|| g_MenuData.root == MENUROOT_MPENDSCREEN) {
		// Don't slide
		dialog->x = dialog->dstx;
		dialog->y = dialog->dsty;
	} else {
		// Slide X
		if (dialog->x != dialog->dstx) {
			oldx = dialog->x;
			newx = dialog->x;

			for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
				newx = dialog->dstx * 0.348f + 0.652f * newx;
#else
				newx = dialog->dstx * 0.3f + 0.7f * newx;
#endif
			}

			dialog->x = newx;

			if (dialog->x != dialog->dstx && dialog->x == oldx) {
				if (dialog->x < dialog->dstx) {
					dialog->x++;
				} else {
					dialog->x--;
				}
			}
		}

		// Slide Y
		if (dialog->y != dialog->dsty) {
			oldy = dialog->y;
			newy = dialog->y;

			for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
				newy = dialog->dsty * 0.348f + 0.652f * newy;
#else
				newy = dialog->dsty * 0.3f + 0.7f * newy;
#endif
			}

			dialog->y = newy;

			if (dialog->y != dialog->dsty && dialog->y == oldy) {
				if (dialog->y < dialog->dsty) {
					dialog->y++;
				} else {
					dialog->y--;
				}
			}
		}
	}

	// Tween the width
	if (dialog->width != dialog->dstwidth) {
		oldwidth = dialog->width;
		newwidth = dialog->width;

		for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
			newwidth = dialog->dstwidth * 0.348f + 0.652f * newwidth;
#else
			newwidth = dialog->dstwidth * 0.3f + 0.7f * newwidth;
#endif
		}

		dialog->width = newwidth;

		if (dialog->width != dialog->dstwidth && dialog->width == oldwidth) {
			if (dialog->width < dialog->dstwidth) {
				dialog->width++;
			} else {
				dialog->width--;
			}
		}
	}

	// Tween the height
	if (dialog->height != dialog->dstheight) {
		s32 i;
		oldheight = dialog->height;
		newheight = dialog->height;

		for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
			newheight = dialog->dstheight * 0.348f + 0.652f * newheight;
#else
			newheight = dialog->dstheight * 0.3f + 0.7f * newheight;
#endif
		}

		dialog->height = newheight;

		if (dialog->height != dialog->dstheight && dialog->height == oldheight) {
			if (dialog->height < dialog->dstheight) {
				dialog->height++;
			} else {
				dialog->height--;
			}
		}
	}

	// Call the dialog's tick handler, if any
	data.dialog2.inputs = inputs;

	if (definition->handler != NULL) {
		definition->handler(MENUOP_TICK, definition, &data);
	}

	if (dialog->dimmed) {
		dialog->unk10 += g_Vars.diffframe60;
	} else {
		dialog->unk10 = 0;
	}

	// Tick each item in the dialog
	{
		s32 j;

		for (col = 0; col < dialog->numcols; col++) {
			s32 colindex = dialog->colstart + col;

			for (j = 0; j < menu->cols[colindex].numrows; j++) {
				s32 rowindex = menu->cols[colindex].rowstart + j;
				struct menuitem *item = &dialog->definition->items[menu->rows[rowindex].itemindex];
				union menuitemdata *handlerdata = NULL;
				struct menuinputs *inputsptr = inputs;

				if (mpIsPlayerLockedOut(g_MpPlayerNum) && (item->flags & MENUITEMFLAG_LOCKABLEMINOR)) {
					inputsptr = &spd8;
				} else if ((item->flags & MENUITEMFLAG_MPWEAPONSLOT) && mpGetWeaponSet() != func0f189088()) {
					inputsptr = &spd8;
				} else if (g_MenuData.root == MENUROOT_12) {
					inputsptr = &spd8;
				}

				if (inputsptr);

				if (menu->rows[rowindex].blockindex != -1) {
					handlerdata = (union menuitemdata *)&menu->blocks[menu->rows[rowindex].blockindex];
				}

				if (g_Menus[g_MpPlayerNum].fm.unke40_00 == 0 && !g_MainIsDebugMenuOpen) {
					if ((tickflags & MENUTICKFLAG_DIALOGISCURRENT) && item == dialog->focuseditem) {
						u32 itemtickflags = tickflags | MENUTICKFLAG_ITEMISFOCUSED;

						if (dialog->dimmed) {
							usedefaultbehaviour = menuitemTick(item, dialog, inputsptr, itemtickflags | MENUTICKFLAG_DIALOGISDIMMED, handlerdata);
						} else {
							usedefaultbehaviour = menuitemTick(item, dialog, inputsptr, itemtickflags, handlerdata);
						}
					} else {
						menuitemTick(item, dialog, inputsptr, tickflags, handlerdata);
					}
				}
			}
		}
	}

	// If the focused item is disabled somehow, automatically jump to the next
	if (dialog->focuseditem
			&& menuIsItemDisabled(dialog->focuseditem, dialog)
			&& (tickflags & MENUTICKFLAG_DIALOGISCURRENT)) {
		usedefaultbehaviour = true;
		inputs->updown = 1;
		dialog->dimmed = false;
	}

	// Apply default navigational behaviour if requested
	if (usedefaultbehaviour && (tickflags & MENUTICKFLAG_DIALOGISCURRENT) && !dialog->dimmed) {
		struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[g_Menus[g_MpPlayerNum].depth - 1];

		if (layer->numsiblings <= 1) {
			struct menuitem *prevfocuseditem = dialog->focuseditem;

			dialogChangeItemFocus(dialog, inputs->leftright, inputs->updown);

			if (dialog->focuseditem != prevfocuseditem) {
				menuPlaySound(MENUSOUND_FOCUS);
			}
		} else {
			struct menuitem *prevfocuseditem = dialog->focuseditem;
			s32 swipedir = dialogChangeItemFocus(dialog, inputs->leftright, inputs->updown);

			if (swipedir != 0) {
				menuSwipe(swipedir);
			} else if (prevfocuseditem != dialog->focuseditem) {
				menuPlaySound(MENUSOUND_FOCUS);
			}
		}

		if (inputs->back) {
			if ((dialog->definition->flags & MENUDIALOGFLAG_DROPOUTONCLOSE) && g_Vars.unk000498) {
				if (IS4MB()) {
					menuPushDialog(&g_MpDropOut4MbMenuDialog);
				} else {
					menuPushDialog(&g_MpDropOutMenuDialog);
				}
			} else if ((dialog->definition->flags & MENUDIALOGFLAG_IGNOREBACK) == 0) {
				menuPopDialog();
			}
		} else if (dialog->definition->flags & MENUDIALOGFLAG_CLOSEONSELECT) {
			if (dialog->state > MENUDIALOGSTATE_PREOPEN) {
				if ((inputs->select & 1) == 1 || (inputs->back & 1) == 1) {
					menuPopDialog();
				}
			}
		}
	}

	// Scrolling related (when the dialog is too big vertically)
	if (dialog->focuseditem && (dialog->definition->flags & MENUDIALOGFLAG_DISABLEITEMSCROLL) == 0) {
#if VERSION >= VERSION_NTSC_1_0
		s32 dstscroll;
		s32 itemy;
		s32 rowindex;
		s32 colindex;
#else
		s32 dstscroll;
		s32 rowindex;
		s32 colindex;
		s32 itemy;
#endif

		s32 y = dialogFindItem(dialog, dialog->focuseditem, &rowindex, &colindex);

		if ((dialog->focuseditem->flags & MENUITEMFLAG_00010000) == 0) {
			itemy = y + menu->rows[rowindex].height / 2;
			dstscroll = (dialog->height - LINEHEIGHT - 1) / 2 - itemy;

			if (dstscroll > 0) {
				dstscroll = 0;
			}

			if (dstscroll < dialog->height - dialog->contentheight) {
				dstscroll = dialog->height - dialog->contentheight;
			}

			dialog->dstscroll = dstscroll;
		} else {
			dialog->dstscroll = 0;
		}
	} else if (dialog->definition->flags & MENUDIALOGFLAG_SMOOTHSCROLLABLE) {
#if VERSION >= VERSION_PAL_BETA
		s32 adjustment = (s32)(inputs->yaxis * g_Vars.diffframe60freal / 20) - (s32)(inputs->updownheld * g_Vars.diffframe60freal);
#else
		s32 adjustment = inputs->yaxis * g_Vars.diffframe60 / 20 - inputs->updownheld * g_Vars.diffframe60;
#endif
		dialog->dstscroll += adjustment;

		if (dialog->dstscroll > 0) {
			dialog->dstscroll = 0;
		}

		if (dialog->dstscroll < dialog->height - dialog->contentheight) {
			dialog->dstscroll = dialog->height - dialog->contentheight;
		}

		dialog->scroll = dialog->dstscroll;
	}

	{
		if (dialog->scroll != dialog->dstscroll) {
			s32 oldscroll = dialog->scroll;
			f32 newscroll = dialog->scroll;
			s32 i;

			for (i = 0; i < g_Vars.diffframe60; i++) {
#if PAL
				newscroll = (dialog->dstscroll * 0.235f) + (0.765f * newscroll);
#else
				newscroll = (dialog->dstscroll * 0.2f) + (0.8f * newscroll);
#endif
			}

			dialog->scroll = newscroll;

			if (dialog->scroll != dialog->dstscroll && dialog->scroll == oldscroll) {
				if (dialog->scroll < dialog->dstscroll) {
					dialog->scroll++;
				} else {
					dialog->scroll--;
				}
			}
		}
	}

	if (g_Menus[g_MpPlayerNum].fm.unke40_00 || g_MainIsDebugMenuOpen) {
		inputs->leftright = inputs->updown = inputs->select = inputs->back = inputs->xaxis = inputs->yaxis = inputs->shoulder = inputs->back2 = inputs->unk14 = 0;
		g_Menus[g_MpPlayerNum].fm.unke40_00 = false;
	}
}

void dialogInitItems(struct menudialog *dialog)
{
	struct menu *menu = &g_Menus[g_MpPlayerNum];
	s32 i;
	s32 j;

	for (i = 0; i < dialog->numcols; i++) {
		s32 colindex = dialog->colstart + i;

		for (j = 0; j < menu->cols[colindex].numrows; j++) {
			s32 rowindex = menu->cols[colindex].rowstart + j;
			struct menuitem *item = &dialog->definition->items[menu->rows[rowindex].itemindex];
			union menuitemdata *data = NULL;

			if (menu->rows[rowindex].blockindex != -1) {
				data = (union menuitemdata *)&menu->blocks[menu->rows[rowindex].blockindex];
			}

			menuitemInit(item, data);
		}
	}
}

void func0f0fa6ac(void)
{
	switch (g_MenuData.root) {
	case MENUROOT_MAINMENU:
	case MENUROOT_MPSETUP:
	case MENUROOT_FILEMGR:
	case MENUROOT_4MBMAINMENU:
	case MENUROOT_TRAINING:
		playerUnpause();
		g_PlayersWithControl[0] = true;
	}
}

#if VERSION >= VERSION_JPN_FINAL
GLOBAL_ASM(
glabel menuProcessInput
.late_rodata
glabel var7f1b2a98
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2a9c
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2aa0
.word menuProcessInput+0xc38 # f0fb33c
glabel var7f1b2aa4
.word menuProcessInput+0xcc0 # f0fb3c4
glabel var7f1b2aa8
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2aac
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab0
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab4
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab8
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2abc
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ac0
.word menuProcessInput+0xc38 # f0fb33c
glabel var7f1b2ac4
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ac8
.word menuProcessInput+0xd10 # f0fb414
.text
/*  f0fb3f8:	27bdfed0 */ 	addiu	$sp,$sp,-304
/*  f0fb3fc:	afb70034 */ 	sw	$s7,0x34($sp)
/*  f0fb400:	3c178007 */ 	lui	$s7,0x8007
/*  f0fb404:	26f71998 */ 	addiu	$s7,$s7,0x1998
/*  f0fb408:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fb40c:	afbe0038 */ 	sw	$s8,0x38($sp)
/*  f0fb410:	241e0e70 */ 	li	$s8,0xe70
/*  f0fb414:	01de0019 */ 	multu	$t6,$s8
/*  f0fb418:	afb60030 */ 	sw	$s6,0x30($sp)
/*  f0fb41c:	3c16800a */ 	lui	$s6,0x800a
/*  f0fb420:	26d6e700 */ 	addiu	$s6,$s6,-6400
/*  f0fb424:	3c19800a */ 	lui	$t9,0x800a
/*  f0fb428:	8f3920c4 */ 	lw	$t9,0x20c4($t9)
/*  f0fb42c:	afb5002c */ 	sw	$s5,0x2c($sp)
/*  f0fb430:	24010004 */ 	li	$at,0x4
/*  f0fb434:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0fb438:	afb40028 */ 	sw	$s4,0x28($sp)
/*  f0fb43c:	00007812 */ 	mflo	$t7
/*  f0fb440:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fb444:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fb448:	afb30024 */ 	sw	$s3,0x24($sp)
/*  f0fb44c:	afb20020 */ 	sw	$s2,0x20($sp)
/*  f0fb450:	afb1001c */ 	sw	$s1,0x1c($sp)
/*  f0fb454:	afb00018 */ 	sw	$s0,0x18($sp)
/*  f0fb458:	00006025 */ 	move	$t4,$zero
/*  f0fb45c:	00004825 */ 	move	$t1,$zero
/*  f0fb460:	00004025 */ 	move	$t0,$zero
/*  f0fb464:	0000a825 */ 	move	$s5,$zero
/*  f0fb468:	afa0011c */ 	sw	$zero,0x11c($sp)
/*  f0fb46c:	afa700fc */ 	sw	$a3,0xfc($sp)
/*  f0fb470:	17210005 */ 	bne	$t9,$at,.JF0f0fb488
/*  f0fb474:	afb80100 */ 	sw	$t8,0x100($sp)
/*  f0fb478:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fb47c:	8dcea8bc */ 	lw	$t6,-0x5744($t6)
/*  f0fb480:	3c01800a */ 	lui	$at,0x800a
/*  f0fb484:	ac2e28b8 */ 	sw	$t6,0x28b8($at)
.JF0f0fb488:
/*  f0fb488:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb48c:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb490:	0fc3c829 */ 	jal	func0f0f1418
/*  f0fb494:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb498:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fb49c:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb4a0:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb4a4:	01fe0019 */ 	multu	$t7,$s8
/*  f0fb4a8:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fb4ac:	a3a00106 */ 	sb	$zero,0x106($sp)
/*  f0fb4b0:	a3a00107 */ 	sb	$zero,0x107($sp)
/*  f0fb4b4:	a3a0010a */ 	sb	$zero,0x10a($sp)
/*  f0fb4b8:	a3a0010b */ 	sb	$zero,0x10b($sp)
/*  f0fb4bc:	00005025 */ 	move	$t2,$zero
/*  f0fb4c0:	00005825 */ 	move	$t3,$zero
/*  f0fb4c4:	3c0f800a */ 	lui	$t7,0x800a
/*  f0fb4c8:	0000c012 */ 	mflo	$t8
/*  f0fb4cc:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fb4d0:	8f2e04f8 */ 	lw	$t6,0x4f8($t9)
/*  f0fb4d4:	51c0031e */ 	beqzl	$t6,.JF0f0fc150
/*  f0fb4d8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb4dc:	8def20c4 */ 	lw	$t7,0x20c4($t7)
/*  f0fb4e0:	24010007 */ 	li	$at,0x7
/*  f0fb4e4:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*  f0fb4e8:	15e1000b */ 	bne	$t7,$at,.JF0f0fb518
/*  f0fb4ec:	afa000ec */ 	sw	$zero,0xec($sp)
/*  f0fb4f0:	24180001 */ 	li	$t8,0x1
/*  f0fb4f4:	24190002 */ 	li	$t9,0x2
/*  f0fb4f8:	240e0003 */ 	li	$t6,0x3
/*  f0fb4fc:	240f0004 */ 	li	$t7,0x4
/*  f0fb500:	afa000d8 */ 	sw	$zero,0xd8($sp)
/*  f0fb504:	afb800dc */ 	sw	$t8,0xdc($sp)
/*  f0fb508:	afb900e0 */ 	sw	$t9,0xe0($sp)
/*  f0fb50c:	afae00e4 */ 	sw	$t6,0xe4($sp)
/*  f0fb510:	1000001b */ 	b	.JF0f0fb580
/*  f0fb514:	afaf00ec */ 	sw	$t7,0xec($sp)
.JF0f0fb518:
/*  f0fb518:	27a400d7 */ 	addiu	$a0,$sp,0xd7
/*  f0fb51c:	27a500d6 */ 	addiu	$a1,$sp,0xd6
/*  f0fb520:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb524:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb528:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fb52c:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fb530:	0fc3e053 */ 	jal	menuGetContPads
/*  f0fb534:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb538:	83a200d7 */ 	lb	$v0,0xd7($sp)
/*  f0fb53c:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb540:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb544:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fb548:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fb54c:	04400004 */ 	bltz	$v0,.JF0f0fb560
/*  f0fb550:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fb554:	24030001 */ 	li	$v1,0x1
/*  f0fb558:	afa200d8 */ 	sw	$v0,0xd8($sp)
/*  f0fb55c:	afa300ec */ 	sw	$v1,0xec($sp)
.JF0f0fb560:
/*  f0fb560:	83a200d6 */ 	lb	$v0,0xd6($sp)
/*  f0fb564:	8fa300ec */ 	lw	$v1,0xec($sp)
/*  f0fb568:	04400005 */ 	bltz	$v0,.JF0f0fb580
/*  f0fb56c:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fb570:	03b8c821 */ 	addu	$t9,$sp,$t8
/*  f0fb574:	af2200d8 */ 	sw	$v0,0xd8($t9)
/*  f0fb578:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0fb57c:	afa300ec */ 	sw	$v1,0xec($sp)
.JF0f0fb580:
/*  f0fb580:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fb584:	01de0019 */ 	multu	$t6,$s8
/*  f0fb588:	00007812 */ 	mflo	$t7
/*  f0fb58c:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fb590:	90e20e41 */ 	lbu	$v0,0xe41($a3)
/*  f0fb594:	1840001a */ 	blez	$v0,.JF0f0fb600
/*  f0fb598:	2458ffff */ 	addiu	$t8,$v0,-1
/*  f0fb59c:	331900ff */ 	andi	$t9,$t8,0xff
/*  f0fb5a0:	17200014 */ 	bnez	$t9,.JF0f0fb5f4
/*  f0fb5a4:	a0f80e41 */ 	sb	$t8,0xe41($a3)
/*  f0fb5a8:	8cee0e48 */ 	lw	$t6,0xe48($a3)
/*  f0fb5ac:	8cef0e4c */ 	lw	$t7,0xe4c($a3)
/*  f0fb5b0:	27a400cc */ 	addiu	$a0,$sp,0xcc
/*  f0fb5b4:	2405ffff */ 	li	$a1,-1
/*  f0fb5b8:	00003025 */ 	move	$a2,$zero
/*  f0fb5bc:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb5c0:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb5c4:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fb5c8:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fb5cc:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb5d0:	afae00cc */ 	sw	$t6,0xcc($sp)
/*  f0fb5d4:	0fc42599 */ 	jal	filemgrSaveOrLoad
/*  f0fb5d8:	a7af00d0 */ 	sh	$t7,0xd0($sp)
/*  f0fb5dc:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb5e0:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb5e4:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fb5e8:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fb5ec:	10000004 */ 	b	.JF0f0fb600
/*  f0fb5f0:	8fac012c */ 	lw	$t4,0x12c($sp)
.JF0f0fb5f4:
/*  f0fb5f4:	90f90e40 */ 	lbu	$t9,0xe40($a3)
/*  f0fb5f8:	372e0080 */ 	ori	$t6,$t9,0x80
/*  f0fb5fc:	a0ee0e40 */ 	sb	$t6,0xe40($a3)
.JF0f0fb600:
/*  f0fb600:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0fb604:	0000a025 */ 	move	$s4,$zero
/*  f0fb608:	19e00083 */ 	blez	$t7,.JF0f0fb818
/*  f0fb60c:	27b300d8 */ 	addiu	$s3,$sp,0xd8
.JF0f0fb610:
/*  f0fb610:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb614:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb618:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fb61c:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fb620:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb624:	0c00536a */ 	jal	joyGetStickX
/*  f0fb628:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb62c:	00028600 */ 	sll	$s0,$v0,0x18
/*  f0fb630:	0010c603 */ 	sra	$t8,$s0,0x18
/*  f0fb634:	03008025 */ 	move	$s0,$t8
/*  f0fb638:	0c005394 */ 	jal	joyGetStickY
/*  f0fb63c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb640:	00028e00 */ 	sll	$s1,$v0,0x18
/*  f0fb644:	0011ce03 */ 	sra	$t9,$s1,0x18
/*  f0fb648:	03208825 */ 	move	$s1,$t9
/*  f0fb64c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb650:	0c0053be */ 	jal	joyGetButtons
/*  f0fb654:	3405ffff */ 	li	$a1,0xffff
/*  f0fb658:	3052ffff */ 	andi	$s2,$v0,0xffff
/*  f0fb65c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb660:	0c0053ee */ 	jal	joyGetButtonsPressedThisFrame
/*  f0fb664:	3405ffff */ 	li	$a1,0xffff
/*  f0fb668:	304e8000 */ 	andi	$t6,$v0,0x8000
/*  f0fb66c:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb670:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb674:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fb678:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fb67c:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fb680:	11c00003 */ 	beqz	$t6,.JF0f0fb690
/*  f0fb684:	00403025 */ 	move	$a2,$v0
/*  f0fb688:	240f0001 */ 	li	$t7,0x1
/*  f0fb68c:	a3af0106 */ 	sb	$t7,0x106($sp)
.JF0f0fb690:
/*  f0fb690:	30d84000 */ 	andi	$t8,$a2,0x4000
/*  f0fb694:	13000003 */ 	beqz	$t8,.JF0f0fb6a4
/*  f0fb698:	30ce2000 */ 	andi	$t6,$a2,0x2000
/*  f0fb69c:	24190001 */ 	li	$t9,0x1
/*  f0fb6a0:	a3b90107 */ 	sb	$t9,0x107($sp)
.JF0f0fb6a4:
/*  f0fb6a4:	11c00003 */ 	beqz	$t6,.JF0f0fb6b4
/*  f0fb6a8:	30d81000 */ 	andi	$t8,$a2,0x1000
/*  f0fb6ac:	240f0001 */ 	li	$t7,0x1
/*  f0fb6b0:	a3af0106 */ 	sb	$t7,0x106($sp)
.JF0f0fb6b4:
/*  f0fb6b4:	13000003 */ 	beqz	$t8,.JF0f0fb6c4
/*  f0fb6b8:	324e0010 */ 	andi	$t6,$s2,0x10
/*  f0fb6bc:	24190001 */ 	li	$t9,0x1
/*  f0fb6c0:	afb900f8 */ 	sw	$t9,0xf8($sp)
.JF0f0fb6c4:
/*  f0fb6c4:	11c00003 */ 	beqz	$t6,.JF0f0fb6d4
/*  f0fb6c8:	02402825 */ 	move	$a1,$s2
/*  f0fb6cc:	240f0001 */ 	li	$t7,0x1
/*  f0fb6d0:	a3af010a */ 	sb	$t7,0x10a($sp)
.JF0f0fb6d4:
/*  f0fb6d4:	30b80020 */ 	andi	$t8,$a1,0x20
/*  f0fb6d8:	13000003 */ 	beqz	$t8,.JF0f0fb6e8
/*  f0fb6dc:	30ae0008 */ 	andi	$t6,$a1,0x8
/*  f0fb6e0:	24190001 */ 	li	$t9,0x1
/*  f0fb6e4:	a3b9010a */ 	sb	$t9,0x10a($sp)
.JF0f0fb6e8:
/*  f0fb6e8:	05410003 */ 	bgez	$t2,.JF0f0fb6f8
/*  f0fb6ec:	30cf0008 */ 	andi	$t7,$a2,0x8
/*  f0fb6f0:	10000002 */ 	b	.JF0f0fb6fc
/*  f0fb6f4:	000a2023 */ 	negu	$a0,$t2
.JF0f0fb6f8:
/*  f0fb6f8:	01402025 */ 	move	$a0,$t2
.JF0f0fb6fc:
/*  f0fb6fc:	06010003 */ 	bgez	$s0,.JF0f0fb70c
/*  f0fb700:	30b80004 */ 	andi	$t8,$a1,0x4
/*  f0fb704:	10000002 */ 	b	.JF0f0fb710
/*  f0fb708:	00101823 */ 	negu	$v1,$s0
.JF0f0fb70c:
/*  f0fb70c:	02001825 */ 	move	$v1,$s0
.JF0f0fb710:
/*  f0fb710:	0083082a */ 	slt	$at,$a0,$v1
/*  f0fb714:	10200002 */ 	beqz	$at,.JF0f0fb720
/*  f0fb718:	30d90004 */ 	andi	$t9,$a2,0x4
/*  f0fb71c:	02005025 */ 	move	$t2,$s0
.JF0f0fb720:
/*  f0fb720:	05610003 */ 	bgez	$t3,.JF0f0fb730
/*  f0fb724:	01602025 */ 	move	$a0,$t3
/*  f0fb728:	10000001 */ 	b	.JF0f0fb730
/*  f0fb72c:	000b2023 */ 	negu	$a0,$t3
.JF0f0fb730:
/*  f0fb730:	06210003 */ 	bgez	$s1,.JF0f0fb740
/*  f0fb734:	02201825 */ 	move	$v1,$s1
/*  f0fb738:	10000001 */ 	b	.JF0f0fb740
/*  f0fb73c:	00111823 */ 	negu	$v1,$s1
.JF0f0fb740:
/*  f0fb740:	0083082a */ 	slt	$at,$a0,$v1
/*  f0fb744:	10200002 */ 	beqz	$at,.JF0f0fb750
/*  f0fb748:	00000000 */ 	nop
/*  f0fb74c:	02205825 */ 	move	$t3,$s1
.JF0f0fb750:
/*  f0fb750:	11c00002 */ 	beqz	$t6,.JF0f0fb75c
/*  f0fb754:	00000000 */ 	nop
/*  f0fb758:	240cffff */ 	li	$t4,-1
.JF0f0fb75c:
/*  f0fb75c:	11e00002 */ 	beqz	$t7,.JF0f0fb768
/*  f0fb760:	30ae0002 */ 	andi	$t6,$a1,0x2
/*  f0fb764:	2408ffff */ 	li	$t0,-1
.JF0f0fb768:
/*  f0fb768:	13000002 */ 	beqz	$t8,.JF0f0fb774
/*  f0fb76c:	30cf0002 */ 	andi	$t7,$a2,0x2
/*  f0fb770:	240c0001 */ 	li	$t4,0x1
.JF0f0fb774:
/*  f0fb774:	13200002 */ 	beqz	$t9,.JF0f0fb780
/*  f0fb778:	30b80001 */ 	andi	$t8,$a1,0x1
/*  f0fb77c:	24080001 */ 	li	$t0,0x1
.JF0f0fb780:
/*  f0fb780:	11c00002 */ 	beqz	$t6,.JF0f0fb78c
/*  f0fb784:	30d90001 */ 	andi	$t9,$a2,0x1
/*  f0fb788:	2409ffff */ 	li	$t1,-1
.JF0f0fb78c:
/*  f0fb78c:	11e00002 */ 	beqz	$t7,.JF0f0fb798
/*  f0fb790:	30ae0800 */ 	andi	$t6,$a1,0x800
/*  f0fb794:	2415ffff */ 	li	$s5,-1
.JF0f0fb798:
/*  f0fb798:	13000002 */ 	beqz	$t8,.JF0f0fb7a4
/*  f0fb79c:	30cf0800 */ 	andi	$t7,$a2,0x800
/*  f0fb7a0:	24090001 */ 	li	$t1,0x1
.JF0f0fb7a4:
/*  f0fb7a4:	13200002 */ 	beqz	$t9,.JF0f0fb7b0
/*  f0fb7a8:	30b80400 */ 	andi	$t8,$a1,0x400
/*  f0fb7ac:	24150001 */ 	li	$s5,0x1
.JF0f0fb7b0:
/*  f0fb7b0:	11c00002 */ 	beqz	$t6,.JF0f0fb7bc
/*  f0fb7b4:	30d90400 */ 	andi	$t9,$a2,0x400
/*  f0fb7b8:	240cffff */ 	li	$t4,-1
.JF0f0fb7bc:
/*  f0fb7bc:	11e00002 */ 	beqz	$t7,.JF0f0fb7c8
/*  f0fb7c0:	30ae0200 */ 	andi	$t6,$a1,0x200
/*  f0fb7c4:	2408ffff */ 	li	$t0,-1
.JF0f0fb7c8:
/*  f0fb7c8:	13000002 */ 	beqz	$t8,.JF0f0fb7d4
/*  f0fb7cc:	30cf0200 */ 	andi	$t7,$a2,0x200
/*  f0fb7d0:	240c0001 */ 	li	$t4,0x1
.JF0f0fb7d4:
/*  f0fb7d4:	13200002 */ 	beqz	$t9,.JF0f0fb7e0
/*  f0fb7d8:	30b80100 */ 	andi	$t8,$a1,0x100
/*  f0fb7dc:	24080001 */ 	li	$t0,0x1
.JF0f0fb7e0:
/*  f0fb7e0:	11c00002 */ 	beqz	$t6,.JF0f0fb7ec
/*  f0fb7e4:	30d90100 */ 	andi	$t9,$a2,0x100
/*  f0fb7e8:	2409ffff */ 	li	$t1,-1
.JF0f0fb7ec:
/*  f0fb7ec:	11e00002 */ 	beqz	$t7,.JF0f0fb7f8
/*  f0fb7f0:	8fae00ec */ 	lw	$t6,0xec($sp)
/*  f0fb7f4:	2415ffff */ 	li	$s5,-1
.JF0f0fb7f8:
/*  f0fb7f8:	13000002 */ 	beqz	$t8,.JF0f0fb804
/*  f0fb7fc:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fb800:	24090001 */ 	li	$t1,0x1
.JF0f0fb804:
/*  f0fb804:	13200002 */ 	beqz	$t9,.JF0f0fb810
/*  f0fb808:	00000000 */ 	nop
/*  f0fb80c:	24150001 */ 	li	$s5,0x1
.JF0f0fb810:
/*  f0fb810:	168eff7f */ 	bne	$s4,$t6,.JF0f0fb610
/*  f0fb814:	26730004 */ 	addiu	$s3,$s3,0x4
.JF0f0fb818:
/*  f0fb818:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fb81c:	93ae0106 */ 	lbu	$t6,0x106($sp)
/*  f0fb820:	2410000a */ 	li	$s0,0xa
/*  f0fb824:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0fb828:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb82c:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0fb830:	030fc021 */ 	addu	$t8,$t8,$t7
/*  f0fb834:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb838:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb83c:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0fb840:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fb844:	11c00002 */ 	beqz	$t6,.JF0f0fb850
/*  f0fb848:	8f2404f8 */ 	lw	$a0,0x4f8($t9)
/*  f0fb84c:	a3a00107 */ 	sb	$zero,0x107($sp)
.JF0f0fb850:
/*  f0fb850:	11000002 */ 	beqz	$t0,.JF0f0fb85c
/*  f0fb854:	2406001e */ 	li	$a2,0x1e
/*  f0fb858:	01006025 */ 	move	$t4,$t0
.JF0f0fb85c:
/*  f0fb85c:	12a00002 */ 	beqz	$s5,.JF0f0fb868
/*  f0fb860:	2413003c */ 	li	$s3,0x3c
/*  f0fb864:	02a04825 */ 	move	$t1,$s5
.JF0f0fb868:
/*  f0fb868:	24110021 */ 	li	$s1,0x21
/*  f0fb86c:	2414002c */ 	li	$s4,0x2c
/*  f0fb870:	10800019 */ 	beqz	$a0,.JF0f0fb8d8
/*  f0fb874:	00009025 */ 	move	$s2,$zero
/*  f0fb878:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb87c:	10400016 */ 	beqz	$v0,.JF0f0fb8d8
/*  f0fb880:	00000000 */ 	nop
/*  f0fb884:	90430000 */ 	lbu	$v1,0x0($v0)
/*  f0fb888:	24010008 */ 	li	$at,0x8
/*  f0fb88c:	10610003 */ 	beq	$v1,$at,.JF0f0fb89c
/*  f0fb890:	24010010 */ 	li	$at,0x10
/*  f0fb894:	54610009 */ 	bnel	$v1,$at,.JF0f0fb8bc
/*  f0fb898:	2401000d */ 	li	$at,0xd
.JF0f0fb89c:
/*  f0fb89c:	8c8f000c */ 	lw	$t7,0xc($a0)
/*  f0fb8a0:	51e00006 */ 	beqzl	$t7,.JF0f0fb8bc
/*  f0fb8a4:	2401000d */ 	li	$at,0xd
/*  f0fb8a8:	24100005 */ 	li	$s0,0x5
/*  f0fb8ac:	24060014 */ 	li	$a2,0x14
/*  f0fb8b0:	2413001e */ 	li	$s3,0x1e
/*  f0fb8b4:	2411000a */ 	li	$s1,0xa
/*  f0fb8b8:	2401000d */ 	li	$at,0xd
.JF0f0fb8bc:
/*  f0fb8bc:	14610006 */ 	bne	$v1,$at,.JF0f0fb8d8
/*  f0fb8c0:	00000000 */ 	nop
/*  f0fb8c4:	24120001 */ 	li	$s2,0x1
/*  f0fb8c8:	24100005 */ 	li	$s0,0x5
/*  f0fb8cc:	24060014 */ 	li	$a2,0x14
/*  f0fb8d0:	2411000a */ 	li	$s1,0xa
/*  f0fb8d4:	2414000a */ 	li	$s4,0xa
.JF0f0fb8d8:
/*  f0fb8d8:	15200004 */ 	bnez	$t1,.JF0f0fb8ec
/*  f0fb8dc:	0000f825 */ 	move	$ra,$zero
/*  f0fb8e0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb8e4:	2418ffff */ 	li	$t8,-1
/*  f0fb8e8:	a478082c */ 	sh	$t8,0x82c($v1)
.JF0f0fb8ec:
/*  f0fb8ec:	12a00006 */ 	beqz	$s5,.JF0f0fb908
/*  f0fb8f0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb8f4:	a460082c */ 	sh	$zero,0x82c($v1)
/*  f0fb8f8:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fb8fc:	a475082a */ 	sh	$s5,0x82a($v1)
/*  f0fb900:	10000004 */ 	b	.JF0f0fb914
/*  f0fb904:	241f0001 */ 	li	$ra,0x1
.JF0f0fb908:
/*  f0fb908:	51200003 */ 	beqzl	$t1,.JF0f0fb918
/*  f0fb90c:	8c620824 */ 	lw	$v0,0x824($v1)
/*  f0fb910:	8469082a */ 	lh	$t1,0x82a($v1)
.JF0f0fb914:
/*  f0fb914:	8c620824 */ 	lw	$v0,0x824($v1)
.JF0f0fb918:
/*  f0fb918:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fb91c:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0fb920:	14200004 */ 	bnez	$at,.JF0f0fb934
/*  f0fb924:	00000000 */ 	nop
/*  f0fb928:	24190001 */ 	li	$t9,0x1
/*  f0fb92c:	a479082c */ 	sh	$t9,0x82c($v1)
/*  f0fb930:	8c620824 */ 	lw	$v0,0x824($v1)
.JF0f0fb934:
/*  f0fb934:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fb938:	8dcea630 */ 	lw	$t6,-0x59d0($t6)
/*  f0fb93c:	00003812 */ 	mflo	$a3
/*  f0fb940:	8478082c */ 	lh	$t8,0x82c($v1)
/*  f0fb944:	004e7821 */ 	addu	$t7,$v0,$t6
/*  f0fb948:	01f0001a */ 	div	$zero,$t7,$s0
/*  f0fb94c:	16000002 */ 	bnez	$s0,.JF0f0fb958
/*  f0fb950:	00000000 */ 	nop
/*  f0fb954:	0007000d */ 	break	0x7
.JF0f0fb958:
/*  f0fb958:	2401ffff */ 	li	$at,-1
/*  f0fb95c:	16010004 */ 	bne	$s0,$at,.JF0f0fb970
/*  f0fb960:	3c018000 */ 	lui	$at,0x8000
/*  f0fb964:	14410002 */ 	bne	$v0,$at,.JF0f0fb970
/*  f0fb968:	00000000 */ 	nop
/*  f0fb96c:	0006000d */ 	break	0x6
.JF0f0fb970:
/*  f0fb970:	00006812 */ 	mflo	$t5
/*  f0fb974:	16000002 */ 	bnez	$s0,.JF0f0fb980
/*  f0fb978:	00000000 */ 	nop
/*  f0fb97c:	0007000d */ 	break	0x7
.JF0f0fb980:
/*  f0fb980:	2401ffff */ 	li	$at,-1
/*  f0fb984:	16010004 */ 	bne	$s0,$at,.JF0f0fb998
/*  f0fb988:	3c018000 */ 	lui	$at,0x8000
/*  f0fb98c:	15e10002 */ 	bne	$t7,$at,.JF0f0fb998
/*  f0fb990:	00000000 */ 	nop
/*  f0fb994:	0006000d */ 	break	0x6
.JF0f0fb998:
/*  f0fb998:	1700000b */ 	bnez	$t8,.JF0f0fb9c8
/*  f0fb99c:	00000000 */ 	nop
/*  f0fb9a0:	04e10003 */ 	bgez	$a3,.JF0f0fb9b0
/*  f0fb9a4:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0fb9a8:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fb9ac:	0001c843 */ 	sra	$t9,$at,0x1
.JF0f0fb9b0:
/*  f0fb9b0:	03203825 */ 	move	$a3,$t9
/*  f0fb9b4:	05a10003 */ 	bgez	$t5,.JF0f0fb9c4
/*  f0fb9b8:	000d7043 */ 	sra	$t6,$t5,0x1
/*  f0fb9bc:	25a10001 */ 	addiu	$at,$t5,0x1
/*  f0fb9c0:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0fb9c4:
/*  f0fb9c4:	01c06825 */ 	move	$t5,$t6
.JF0f0fb9c8:
/*  f0fb9c8:	05410003 */ 	bgez	$t2,.JF0f0fb9d8
/*  f0fb9cc:	a3a9010c */ 	sb	$t1,0x10c($sp)
/*  f0fb9d0:	10000002 */ 	b	.JF0f0fb9dc
/*  f0fb9d4:	000a1023 */ 	negu	$v0,$t2
.JF0f0fb9d8:
/*  f0fb9d8:	01401025 */ 	move	$v0,$t2
.JF0f0fb9dc:
/*  f0fb9dc:	05610003 */ 	bgez	$t3,.JF0f0fb9ec
/*  f0fb9e0:	0046082a */ 	slt	$at,$v0,$a2
/*  f0fb9e4:	10000002 */ 	b	.JF0f0fb9f0
/*  f0fb9e8:	000b1823 */ 	negu	$v1,$t3
.JF0f0fb9ec:
/*  f0fb9ec:	01601825 */ 	move	$v1,$t3
.JF0f0fb9f0:
/*  f0fb9f0:	14200065 */ 	bnez	$at,.JF0f0fbb88
/*  f0fb9f4:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fb9f8:	14200003 */ 	bnez	$at,.JF0f0fba08
/*  f0fb9fc:	00000000 */ 	nop
/*  f0fba00:	52400062 */ 	beqzl	$s2,.JF0f0fbb8c
/*  f0fba04:	8fb900fc */ 	lw	$t9,0xfc($sp)
.JF0f0fba08:
/*  f0fba08:	05410006 */ 	bgez	$t2,.JF0f0fba24
/*  f0fba0c:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fba10:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fba14:	846f0828 */ 	lh	$t7,0x828($v1)
/*  f0fba18:	59e00003 */ 	blezl	$t7,.JF0f0fba28
/*  f0fba1c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fba20:	a4600828 */ 	sh	$zero,0x828($v1)
.JF0f0fba24:
/*  f0fba24:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fba28:
/*  f0fba28:	19400005 */ 	blez	$t2,.JF0f0fba40
/*  f0fba2c:	84650828 */ 	lh	$a1,0x828($v1)
/*  f0fba30:	04a10003 */ 	bgez	$a1,.JF0f0fba40
/*  f0fba34:	00000000 */ 	nop
/*  f0fba38:	a4600828 */ 	sh	$zero,0x828($v1)
/*  f0fba3c:	84650828 */ 	lh	$a1,0x828($v1)
.JF0f0fba40:
/*  f0fba40:	14a00003 */ 	bnez	$a1,.JF0f0fba50
/*  f0fba44:	00000000 */ 	nop
/*  f0fba48:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fba4c:	84650828 */ 	lh	$a1,0x828($v1)
.JF0f0fba50:
/*  f0fba50:	54200003 */ 	bnezl	$at,.JF0f0fba60
/*  f0fba54:	00461023 */ 	subu	$v0,$v0,$a2
/*  f0fba58:	24020046 */ 	li	$v0,0x46
/*  f0fba5c:	00461023 */ 	subu	$v0,$v0,$a2
.JF0f0fba60:
/*  f0fba60:	02220019 */ 	multu	$s1,$v0
/*  f0fba64:	24190046 */ 	li	$t9,0x46
/*  f0fba68:	03267023 */ 	subu	$t6,$t9,$a2
/*  f0fba6c:	0000c012 */ 	mflo	$t8
/*  f0fba70:	00000000 */ 	nop
/*  f0fba74:	00000000 */ 	nop
/*  f0fba78:	030e001a */ 	div	$zero,$t8,$t6
/*  f0fba7c:	15c00002 */ 	bnez	$t6,.JF0f0fba88
/*  f0fba80:	00000000 */ 	nop
/*  f0fba84:	0007000d */ 	break	0x7
.JF0f0fba88:
/*  f0fba88:	2401ffff */ 	li	$at,-1
/*  f0fba8c:	15c10004 */ 	bne	$t6,$at,.JF0f0fbaa0
/*  f0fba90:	3c018000 */ 	lui	$at,0x8000
/*  f0fba94:	17010002 */ 	bne	$t8,$at,.JF0f0fbaa0
/*  f0fba98:	00000000 */ 	nop
/*  f0fba9c:	0006000d */ 	break	0x6
.JF0f0fbaa0:
/*  f0fbaa0:	00007812 */ 	mflo	$t7
/*  f0fbaa4:	026f2023 */ 	subu	$a0,$s3,$t7
/*  f0fbaa8:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fbaac:	10200004 */ 	beqz	$at,.JF0f0fbac0
/*  f0fbab0:	00801825 */ 	move	$v1,$a0
/*  f0fbab4:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fbab8:	10200005 */ 	beqz	$at,.JF0f0fbad0
/*  f0fbabc:	00000000 */ 	nop
.JF0f0fbac0:
/*  f0fbac0:	04810003 */ 	bgez	$a0,.JF0f0fbad0
/*  f0fbac4:	00041843 */ 	sra	$v1,$a0,0x1
/*  f0fbac8:	24810001 */ 	addiu	$at,$a0,0x1
/*  f0fbacc:	00011843 */ 	sra	$v1,$at,0x1
.JF0f0fbad0:
/*  f0fbad0:	1860002f */ 	blez	$v1,.JF0f0fbb90
/*  f0fbad4:	00000000 */ 	nop
/*  f0fbad8:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0fbadc:	3c06800a */ 	lui	$a2,0x800a
/*  f0fbae0:	8cc6a630 */ 	lw	$a2,-0x59d0($a2)
/*  f0fbae4:	8f220824 */ 	lw	$v0,0x824($t9)
/*  f0fbae8:	24090001 */ 	li	$t1,0x1
/*  f0fbaec:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fbaf0:	0046c021 */ 	addu	$t8,$v0,$a2
/*  f0fbaf4:	00003812 */ 	mflo	$a3
/*  f0fbaf8:	14600002 */ 	bnez	$v1,.JF0f0fbb04
/*  f0fbafc:	00000000 */ 	nop
/*  f0fbb00:	0007000d */ 	break	0x7
.JF0f0fbb04:
/*  f0fbb04:	2401ffff */ 	li	$at,-1
/*  f0fbb08:	14610004 */ 	bne	$v1,$at,.JF0f0fbb1c
/*  f0fbb0c:	3c018000 */ 	lui	$at,0x8000
/*  f0fbb10:	14410002 */ 	bne	$v0,$at,.JF0f0fbb1c
/*  f0fbb14:	00000000 */ 	nop
/*  f0fbb18:	0006000d */ 	break	0x6
.JF0f0fbb1c:
/*  f0fbb1c:	0303001a */ 	div	$zero,$t8,$v1
/*  f0fbb20:	00006812 */ 	mflo	$t5
/*  f0fbb24:	14600002 */ 	bnez	$v1,.JF0f0fbb30
/*  f0fbb28:	00000000 */ 	nop
/*  f0fbb2c:	0007000d */ 	break	0x7
.JF0f0fbb30:
/*  f0fbb30:	2401ffff */ 	li	$at,-1
/*  f0fbb34:	14610004 */ 	bne	$v1,$at,.JF0f0fbb48
/*  f0fbb38:	3c018000 */ 	lui	$at,0x8000
/*  f0fbb3c:	17010002 */ 	bne	$t8,$at,.JF0f0fbb48
/*  f0fbb40:	00000000 */ 	nop
/*  f0fbb44:	0006000d */ 	break	0x6
.JF0f0fbb48:
/*  f0fbb48:	05410003 */ 	bgez	$t2,.JF0f0fbb58
/*  f0fbb4c:	00000000 */ 	nop
/*  f0fbb50:	10000001 */ 	b	.JF0f0fbb58
/*  f0fbb54:	2409ffff */ 	li	$t1,-1
.JF0f0fbb58:
/*  f0fbb58:	10ed0002 */ 	beq	$a3,$t5,.JF0f0fbb64
/*  f0fbb5c:	00000000 */ 	nop
/*  f0fbb60:	241f0001 */ 	li	$ra,0x1
.JF0f0fbb64:
/*  f0fbb64:	14a00002 */ 	bnez	$a1,.JF0f0fbb70
/*  f0fbb68:	00000000 */ 	nop
/*  f0fbb6c:	241f0001 */ 	li	$ra,0x1
.JF0f0fbb70:
/*  f0fbb70:	13e00007 */ 	beqz	$ra,.JF0f0fbb90
/*  f0fbb74:	00000000 */ 	nop
/*  f0fbb78:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fbb7c:	00a97021 */ 	addu	$t6,$a1,$t1
/*  f0fbb80:	10000003 */ 	b	.JF0f0fbb90
/*  f0fbb84:	a5ee0828 */ 	sh	$t6,0x828($t7)
.JF0f0fbb88:
/*  f0fbb88:	8fb900fc */ 	lw	$t9,0xfc($sp)
.JF0f0fbb8c:
/*  f0fbb8c:	a7200828 */ 	sh	$zero,0x828($t9)
.JF0f0fbb90:
/*  f0fbb90:	10ed0002 */ 	beq	$a3,$t5,.JF0f0fbb9c
/*  f0fbb94:	3c0f800a */ 	lui	$t7,0x800a
/*  f0fbb98:	241f0001 */ 	li	$ra,0x1
.JF0f0fbb9c:
/*  f0fbb9c:	17e00002 */ 	bnez	$ra,.JF0f0fbba8
/*  f0fbba0:	00000000 */ 	nop
/*  f0fbba4:	00004825 */ 	move	$t1,$zero
.JF0f0fbba8:
/*  f0fbba8:	11000007 */ 	beqz	$t0,.JF0f0fbbc8
/*  f0fbbac:	00006825 */ 	move	$t5,$zero
/*  f0fbbb0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbbb4:	240d0001 */ 	li	$t5,0x1
/*  f0fbbb8:	a4600838 */ 	sh	$zero,0x838($v1)
/*  f0fbbbc:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fbbc0:	10000005 */ 	b	.JF0f0fbbd8
/*  f0fbbc4:	a4680836 */ 	sh	$t0,0x836($v1)
.JF0f0fbbc8:
/*  f0fbbc8:	51800004 */ 	beqzl	$t4,.JF0f0fbbdc
/*  f0fbbcc:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbbd0:	8fb800fc */ 	lw	$t8,0xfc($sp)
/*  f0fbbd4:	870c0836 */ 	lh	$t4,0x836($t8)
.JF0f0fbbd8:
/*  f0fbbd8:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fbbdc:
/*  f0fbbdc:	8c620830 */ 	lw	$v0,0x830($v1)
/*  f0fbbe0:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0fbbe4:	14200004 */ 	bnez	$at,.JF0f0fbbf8
/*  f0fbbe8:	00000000 */ 	nop
/*  f0fbbec:	240e0001 */ 	li	$t6,0x1
/*  f0fbbf0:	a46e0838 */ 	sh	$t6,0x838($v1)
/*  f0fbbf4:	8c620830 */ 	lw	$v0,0x830($v1)
.JF0f0fbbf8:
/*  f0fbbf8:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fbbfc:	8defa630 */ 	lw	$t7,-0x59d0($t7)
/*  f0fbc00:	00003812 */ 	mflo	$a3
/*  f0fbc04:	84780838 */ 	lh	$t8,0x838($v1)
/*  f0fbc08:	004fc821 */ 	addu	$t9,$v0,$t7
/*  f0fbc0c:	0330001a */ 	div	$zero,$t9,$s0
/*  f0fbc10:	16000002 */ 	bnez	$s0,.JF0f0fbc1c
/*  f0fbc14:	00000000 */ 	nop
/*  f0fbc18:	0007000d */ 	break	0x7
.JF0f0fbc1c:
/*  f0fbc1c:	2401ffff */ 	li	$at,-1
/*  f0fbc20:	16010004 */ 	bne	$s0,$at,.JF0f0fbc34
/*  f0fbc24:	3c018000 */ 	lui	$at,0x8000
/*  f0fbc28:	14410002 */ 	bne	$v0,$at,.JF0f0fbc34
/*  f0fbc2c:	00000000 */ 	nop
/*  f0fbc30:	0006000d */ 	break	0x6
.JF0f0fbc34:
/*  f0fbc34:	00004012 */ 	mflo	$t0
/*  f0fbc38:	01601025 */ 	move	$v0,$t3
/*  f0fbc3c:	16000002 */ 	bnez	$s0,.JF0f0fbc48
/*  f0fbc40:	00000000 */ 	nop
/*  f0fbc44:	0007000d */ 	break	0x7
.JF0f0fbc48:
/*  f0fbc48:	2401ffff */ 	li	$at,-1
/*  f0fbc4c:	16010004 */ 	bne	$s0,$at,.JF0f0fbc60
/*  f0fbc50:	3c018000 */ 	lui	$at,0x8000
/*  f0fbc54:	17210002 */ 	bne	$t9,$at,.JF0f0fbc60
/*  f0fbc58:	00000000 */ 	nop
/*  f0fbc5c:	0006000d */ 	break	0x6
.JF0f0fbc60:
/*  f0fbc60:	1700000b */ 	bnez	$t8,.JF0f0fbc90
/*  f0fbc64:	00000000 */ 	nop
/*  f0fbc68:	04e10003 */ 	bgez	$a3,.JF0f0fbc78
/*  f0fbc6c:	00077043 */ 	sra	$t6,$a3,0x1
/*  f0fbc70:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fbc74:	00017043 */ 	sra	$t6,$at,0x1
.JF0f0fbc78:
/*  f0fbc78:	01c03825 */ 	move	$a3,$t6
/*  f0fbc7c:	05010003 */ 	bgez	$t0,.JF0f0fbc8c
/*  f0fbc80:	00087843 */ 	sra	$t7,$t0,0x1
/*  f0fbc84:	25010001 */ 	addiu	$at,$t0,0x1
/*  f0fbc88:	00017843 */ 	sra	$t7,$at,0x1
.JF0f0fbc8c:
/*  f0fbc8c:	01e04025 */ 	move	$t0,$t7
.JF0f0fbc90:
/*  f0fbc90:	05610003 */ 	bgez	$t3,.JF0f0fbca0
/*  f0fbc94:	a3ac010d */ 	sb	$t4,0x10d($sp)
/*  f0fbc98:	10000001 */ 	b	.JF0f0fbca0
/*  f0fbc9c:	000b1023 */ 	negu	$v0,$t3
.JF0f0fbca0:
/*  f0fbca0:	05410003 */ 	bgez	$t2,.JF0f0fbcb0
/*  f0fbca4:	28410014 */ 	slti	$at,$v0,0x14
/*  f0fbca8:	10000002 */ 	b	.JF0f0fbcb4
/*  f0fbcac:	000a1823 */ 	negu	$v1,$t2
.JF0f0fbcb0:
/*  f0fbcb0:	01401825 */ 	move	$v1,$t2
.JF0f0fbcb4:
/*  f0fbcb4:	1420005d */ 	bnez	$at,.JF0f0fbe2c
/*  f0fbcb8:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fbcbc:	14200003 */ 	bnez	$at,.JF0f0fbccc
/*  f0fbcc0:	3c06800a */ 	lui	$a2,0x800a
/*  f0fbcc4:	5240005a */ 	beqzl	$s2,.JF0f0fbe30
/*  f0fbcc8:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fbccc:
/*  f0fbccc:	05610006 */ 	bgez	$t3,.JF0f0fbce8
/*  f0fbcd0:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fbcd4:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbcd8:	84790834 */ 	lh	$t9,0x834($v1)
/*  f0fbcdc:	07230003 */ 	bgezl	$t9,.JF0f0fbcec
/*  f0fbce0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbce4:	a4600834 */ 	sh	$zero,0x834($v1)
.JF0f0fbce8:
/*  f0fbce8:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fbcec:
/*  f0fbcec:	19600005 */ 	blez	$t3,.JF0f0fbd04
/*  f0fbcf0:	84650834 */ 	lh	$a1,0x834($v1)
/*  f0fbcf4:	18a00003 */ 	blez	$a1,.JF0f0fbd04
/*  f0fbcf8:	00000000 */ 	nop
/*  f0fbcfc:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fbd00:	84650834 */ 	lh	$a1,0x834($v1)
.JF0f0fbd04:
/*  f0fbd04:	14a00003 */ 	bnez	$a1,.JF0f0fbd14
/*  f0fbd08:	00000000 */ 	nop
/*  f0fbd0c:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fbd10:	84650834 */ 	lh	$a1,0x834($v1)
.JF0f0fbd14:
/*  f0fbd14:	14200002 */ 	bnez	$at,.JF0f0fbd20
/*  f0fbd18:	8cc6a630 */ 	lw	$a2,-0x59d0($a2)
/*  f0fbd1c:	24020046 */ 	li	$v0,0x46
.JF0f0fbd20:
/*  f0fbd20:	2442ffec */ 	addiu	$v0,$v0,-20
/*  f0fbd24:	02820019 */ 	multu	$s4,$v0
/*  f0fbd28:	24010032 */ 	li	$at,0x32
/*  f0fbd2c:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fbd30:	0000c012 */ 	mflo	$t8
/*  f0fbd34:	00000000 */ 	nop
/*  f0fbd38:	00000000 */ 	nop
/*  f0fbd3c:	0301001a */ 	div	$zero,$t8,$at
/*  f0fbd40:	00007012 */ 	mflo	$t6
/*  f0fbd44:	026e2023 */ 	subu	$a0,$s3,$t6
/*  f0fbd48:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fbd4c:	10200004 */ 	beqz	$at,.JF0f0fbd60
/*  f0fbd50:	00801825 */ 	move	$v1,$a0
/*  f0fbd54:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fbd58:	10200006 */ 	beqz	$at,.JF0f0fbd74
/*  f0fbd5c:	00000000 */ 	nop
.JF0f0fbd60:
/*  f0fbd60:	24010003 */ 	li	$at,0x3
/*  f0fbd64:	0081001a */ 	div	$zero,$a0,$at
/*  f0fbd68:	00001812 */ 	mflo	$v1
/*  f0fbd6c:	00000000 */ 	nop
/*  f0fbd70:	00000000 */ 	nop
.JF0f0fbd74:
/*  f0fbd74:	5860002b */ 	blezl	$v1,.JF0f0fbe24
/*  f0fbd78:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbd7c:	8de20830 */ 	lw	$v0,0x830($t7)
/*  f0fbd80:	240c0001 */ 	li	$t4,0x1
/*  f0fbd84:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fbd88:	0046c821 */ 	addu	$t9,$v0,$a2
/*  f0fbd8c:	00003812 */ 	mflo	$a3
/*  f0fbd90:	14600002 */ 	bnez	$v1,.JF0f0fbd9c
/*  f0fbd94:	00000000 */ 	nop
/*  f0fbd98:	0007000d */ 	break	0x7
.JF0f0fbd9c:
/*  f0fbd9c:	2401ffff */ 	li	$at,-1
/*  f0fbda0:	14610004 */ 	bne	$v1,$at,.JF0f0fbdb4
/*  f0fbda4:	3c018000 */ 	lui	$at,0x8000
/*  f0fbda8:	14410002 */ 	bne	$v0,$at,.JF0f0fbdb4
/*  f0fbdac:	00000000 */ 	nop
/*  f0fbdb0:	0006000d */ 	break	0x6
.JF0f0fbdb4:
/*  f0fbdb4:	0323001a */ 	div	$zero,$t9,$v1
/*  f0fbdb8:	00004012 */ 	mflo	$t0
/*  f0fbdbc:	14600002 */ 	bnez	$v1,.JF0f0fbdc8
/*  f0fbdc0:	00000000 */ 	nop
/*  f0fbdc4:	0007000d */ 	break	0x7
.JF0f0fbdc8:
/*  f0fbdc8:	2401ffff */ 	li	$at,-1
/*  f0fbdcc:	14610004 */ 	bne	$v1,$at,.JF0f0fbde0
/*  f0fbdd0:	3c018000 */ 	lui	$at,0x8000
/*  f0fbdd4:	17210002 */ 	bne	$t9,$at,.JF0f0fbde0
/*  f0fbdd8:	00000000 */ 	nop
/*  f0fbddc:	0006000d */ 	break	0x6
.JF0f0fbde0:
/*  f0fbde0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbde4:	19600003 */ 	blez	$t3,.JF0f0fbdf4
/*  f0fbde8:	00000000 */ 	nop
/*  f0fbdec:	10000001 */ 	b	.JF0f0fbdf4
/*  f0fbdf0:	240cffff */ 	li	$t4,-1
.JF0f0fbdf4:
/*  f0fbdf4:	10e80002 */ 	beq	$a3,$t0,.JF0f0fbe00
/*  f0fbdf8:	00acc021 */ 	addu	$t8,$a1,$t4
/*  f0fbdfc:	240d0001 */ 	li	$t5,0x1
.JF0f0fbe00:
/*  f0fbe00:	14a00002 */ 	bnez	$a1,.JF0f0fbe0c
/*  f0fbe04:	00000000 */ 	nop
/*  f0fbe08:	240d0001 */ 	li	$t5,0x1
.JF0f0fbe0c:
/*  f0fbe0c:	51a00005 */ 	beqzl	$t5,.JF0f0fbe24
/*  f0fbe10:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fbe14:	a4780834 */ 	sh	$t8,0x834($v1)
/*  f0fbe18:	3c06800a */ 	lui	$a2,0x800a
/*  f0fbe1c:	8cc6a630 */ 	lw	$a2,-0x59d0($a2)
/*  f0fbe20:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fbe24:
/*  f0fbe24:	10000006 */ 	b	.JF0f0fbe40
/*  f0fbe28:	8c620830 */ 	lw	$v0,0x830($v1)
.JF0f0fbe2c:
/*  f0fbe2c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.JF0f0fbe30:
/*  f0fbe30:	3c06800a */ 	lui	$a2,0x800a
/*  f0fbe34:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fbe38:	8cc6a630 */ 	lw	$a2,-0x59d0($a2)
/*  f0fbe3c:	8c620830 */ 	lw	$v0,0x830($v1)
.JF0f0fbe40:
/*  f0fbe40:	10e80002 */ 	beq	$a3,$t0,.JF0f0fbe4c
/*  f0fbe44:	27b50104 */ 	addiu	$s5,$sp,0x104
/*  f0fbe48:	240d0001 */ 	li	$t5,0x1
.JF0f0fbe4c:
/*  f0fbe4c:	55a00003 */ 	bnezl	$t5,.JF0f0fbe5c
/*  f0fbe50:	8c6e0824 */ 	lw	$t6,0x824($v1)
/*  f0fbe54:	00006025 */ 	move	$t4,$zero
/*  f0fbe58:	8c6e0824 */ 	lw	$t6,0x824($v1)
.JF0f0fbe5c:
/*  f0fbe5c:	8fa500f8 */ 	lw	$a1,0xf8($sp)
/*  f0fbe60:	3c19800a */ 	lui	$t9,0x800a
/*  f0fbe64:	01c67821 */ 	addu	$t7,$t6,$a2
/*  f0fbe68:	ac6f0824 */ 	sw	$t7,0x824($v1)
/*  f0fbe6c:	8f39a630 */ 	lw	$t9,-0x59d0($t9)
/*  f0fbe70:	240e0001 */ 	li	$t6,0x1
/*  f0fbe74:	0059c021 */ 	addu	$t8,$v0,$t9
/*  f0fbe78:	ac780830 */ 	sw	$t8,0x830($v1)
/*  f0fbe7c:	a3a90104 */ 	sb	$t1,0x104($sp)
/*  f0fbe80:	a3ac0105 */ 	sb	$t4,0x105($sp)
/*  f0fbe84:	a3aa0108 */ 	sb	$t2,0x108($sp)
/*  f0fbe88:	a3ab0109 */ 	sb	$t3,0x109($sp)
/*  f0fbe8c:	10a00003 */ 	beqz	$a1,.JF0f0fbe9c
/*  f0fbe90:	a3a00118 */ 	sb	$zero,0x118($sp)
/*  f0fbe94:	10000002 */ 	b	.JF0f0fbea0
/*  f0fbe98:	a3ae010e */ 	sb	$t6,0x10e($sp)
.JF0f0fbe9c:
/*  f0fbe9c:	a3a0010e */ 	sb	$zero,0x10e($sp)
.JF0f0fbea0:
/*  f0fbea0:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fbea4:	007e0019 */ 	multu	$v1,$s8
/*  f0fbea8:	00007812 */ 	mflo	$t7
/*  f0fbeac:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fbeb0:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fbeb4:	50800016 */ 	beqzl	$a0,.JF0f0fbf10
/*  f0fbeb8:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fbebc:	50a00014 */ 	beqzl	$a1,.JF0f0fbf10
/*  f0fbec0:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fbec4:	8c990000 */ 	lw	$t9,0x0($a0)
/*  f0fbec8:	240f0001 */ 	li	$t7,0x1
/*  f0fbecc:	8f380010 */ 	lw	$t8,0x10($t9)
/*  f0fbed0:	24190001 */ 	li	$t9,0x1
/*  f0fbed4:	330e0004 */ 	andi	$t6,$t8,0x4
/*  f0fbed8:	51c00004 */ 	beqzl	$t6,.JF0f0fbeec
/*  f0fbedc:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fbee0:	a3af0106 */ 	sb	$t7,0x106($sp)
/*  f0fbee4:	afb9011c */ 	sw	$t9,0x11c($sp)
/*  f0fbee8:	8c820008 */ 	lw	$v0,0x8($a0)
.JF0f0fbeec:
/*  f0fbeec:	50400008 */ 	beqzl	$v0,.JF0f0fbf10
/*  f0fbef0:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fbef4:	90580000 */ 	lbu	$t8,0x0($v0)
/*  f0fbef8:	24010002 */ 	li	$at,0x2
/*  f0fbefc:	240e0001 */ 	li	$t6,0x1
/*  f0fbf00:	57010003 */ 	bnel	$t8,$at,.JF0f0fbf10
/*  f0fbf04:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fbf08:	a3ae0106 */ 	sb	$t6,0x106($sp)
/*  f0fbf0c:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.JF0f0fbf10:
/*  f0fbf10:	00009825 */ 	move	$s3,$zero
/*  f0fbf14:	0000a025 */ 	move	$s4,$zero
/*  f0fbf18:	1840003a */ 	blez	$v0,.JF0f0fc004
/*  f0fbf1c:	00000000 */ 	nop
/*  f0fbf20:	afa00044 */ 	sw	$zero,0x44($sp)
.JF0f0fbf24:
/*  f0fbf24:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fbf28:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fbf2c:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fbf30:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fbf34:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fbf38:	8fb80044 */ 	lw	$t8,0x44($sp)
/*  f0fbf3c:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fbf40:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fbf44:	02cfc821 */ 	addu	$t9,$s6,$t7
/*  f0fbf48:	03389021 */ 	addu	$s2,$t9,$t8
/*  f0fbf4c:	824e0478 */ 	lb	$t6,0x478($s2)
/*  f0fbf50:	26520464 */ 	addiu	$s2,$s2,0x464
/*  f0fbf54:	02408825 */ 	move	$s1,$s2
/*  f0fbf58:	19c00024 */ 	blez	$t6,.JF0f0fbfec
/*  f0fbf5c:	00008025 */ 	move	$s0,$zero
/*  f0fbf60:	8ef90000 */ 	lw	$t9,0x0($s7)
.JF0f0fbf64:
/*  f0fbf64:	268f0001 */ 	addiu	$t7,$s4,0x1
/*  f0fbf68:	00003025 */ 	move	$a2,$zero
/*  f0fbf6c:	033e0019 */ 	multu	$t9,$s8
/*  f0fbf70:	02a02825 */ 	move	$a1,$s5
/*  f0fbf74:	0000c012 */ 	mflo	$t8
/*  f0fbf78:	02d87021 */ 	addu	$t6,$s6,$t8
/*  f0fbf7c:	85d904f4 */ 	lh	$t9,0x4f4($t6)
/*  f0fbf80:	15f90008 */ 	bne	$t7,$t9,.JF0f0fbfa4
/*  f0fbf84:	00000000 */ 	nop
/*  f0fbf88:	82580015 */ 	lb	$t8,0x15($s2)
/*  f0fbf8c:	16180005 */ 	bne	$s0,$t8,.JF0f0fbfa4
/*  f0fbf90:	00000000 */ 	nop
/*  f0fbf94:	16600003 */ 	bnez	$s3,.JF0f0fbfa4
/*  f0fbf98:	00000000 */ 	nop
/*  f0fbf9c:	24060001 */ 	li	$a2,0x1
/*  f0fbfa0:	24130001 */ 	li	$s3,0x1
.JF0f0fbfa4:
/*  f0fbfa4:	0fc3e816 */ 	jal	dialogTick
/*  f0fbfa8:	8e240000 */ 	lw	$a0,0x0($s1)
/*  f0fbfac:	824e0014 */ 	lb	$t6,0x14($s2)
/*  f0fbfb0:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0fbfb4:	26310004 */ 	addiu	$s1,$s1,0x4
/*  f0fbfb8:	020e082a */ 	slt	$at,$s0,$t6
/*  f0fbfbc:	5420ffe9 */ 	bnezl	$at,.JF0f0fbf64
/*  f0fbfc0:	8ef90000 */ 	lw	$t9,0x0($s7)
/*  f0fbfc4:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fbfc8:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fbfcc:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fbfd0:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fbfd4:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fbfd8:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fbfdc:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fbfe0:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fbfe4:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fbfe8:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.JF0f0fbfec:
/*  f0fbfec:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0fbff0:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fbff4:	0282082a */ 	slt	$at,$s4,$v0
/*  f0fbff8:	27380018 */ 	addiu	$t8,$t9,0x18
/*  f0fbffc:	1420ffc9 */ 	bnez	$at,.JF0f0fbf24
/*  f0fc000:	afb80044 */ 	sw	$t8,0x44($sp)
.JF0f0fc004:
/*  f0fc004:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fc008:	8dce20c4 */ 	lw	$t6,0x20c4($t6)
/*  f0fc00c:	25cfffff */ 	addiu	$t7,$t6,-1
/*  f0fc010:	2de1000d */ 	sltiu	$at,$t7,0xd
/*  f0fc014:	1020004d */ 	beqz	$at,.JF0f0fc14c
/*  f0fc018:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fc01c:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fc020:	002f0821 */ 	addu	$at,$at,$t7
/*  f0fc024:	8c2f37c4 */ 	lw	$t7,0x37c4($at)
/*  f0fc028:	01e00008 */ 	jr	$t7
/*  f0fc02c:	00000000 */ 	nop
/*  f0fc030:	83b9010e */ 	lb	$t9,0x10e($sp)
/*  f0fc034:	8fb8011c */ 	lw	$t8,0x11c($sp)
/*  f0fc038:	53200045 */ 	beqzl	$t9,.JF0f0fc150
/*  f0fc03c:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc040:	57000043 */ 	bnezl	$t8,.JF0f0fc150
/*  f0fc044:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc048:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fc04c:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fc050:	5080003f */ 	beqzl	$a0,.JF0f0fc150
/*  f0fc054:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc058:	8dcf000c */ 	lw	$t7,0xc($t6)
/*  f0fc05c:	3c19800a */ 	lui	$t9,0x800a
/*  f0fc060:	55e0003b */ 	bnezl	$t7,.JF0f0fc150
/*  f0fc064:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc068:	8f39aac0 */ 	lw	$t9,-0x5540($t9)
/*  f0fc06c:	8c820000 */ 	lw	$v0,0x0($a0)
/*  f0fc070:	24010002 */ 	li	$at,0x2
/*  f0fc074:	13210008 */ 	beq	$t9,$at,.JF0f0fc098
/*  f0fc078:	3c048008 */ 	lui	$a0,0x8008
/*  f0fc07c:	24845e34 */ 	addiu	$a0,$a0,0x5e34
/*  f0fc080:	10820005 */ 	beq	$a0,$v0,.JF0f0fc098
/*  f0fc084:	00000000 */ 	nop
/*  f0fc088:	0fc3cef8 */ 	jal	menuPushDialog
/*  f0fc08c:	00000000 */ 	nop
/*  f0fc090:	1000002f */ 	b	.JF0f0fc150
/*  f0fc094:	8fbf003c */ 	lw	$ra,0x3c($sp)
.JF0f0fc098:
/*  f0fc098:	3c188008 */ 	lui	$t8,0x8008
/*  f0fc09c:	27186cfc */ 	addiu	$t8,$t8,0x6cfc
/*  f0fc0a0:	5702002b */ 	bnel	$t8,$v0,.JF0f0fc150
/*  f0fc0a4:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc0a8:	0fc5fe86 */ 	jal	func0f17f428
/*  f0fc0ac:	00000000 */ 	nop
/*  f0fc0b0:	10000027 */ 	b	.JF0f0fc150
/*  f0fc0b4:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc0b8:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0fc0bc:	8dce0cb4 */ 	lw	$t6,0xcb4($t6)
/*  f0fc0c0:	51c00010 */ 	beqzl	$t6,.JF0f0fc104
/*  f0fc0c4:	240e000a */ 	li	$t6,0xa
/*  f0fc0c8:	0fc3e386 */ 	jal	func0f0f8120
/*  f0fc0cc:	00000000 */ 	nop
/*  f0fc0d0:	3c0f8007 */ 	lui	$t7,0x8007
/*  f0fc0d4:	8def1998 */ 	lw	$t7,0x1998($t7)
/*  f0fc0d8:	3c18800a */ 	lui	$t8,0x800a
/*  f0fc0dc:	2718e700 */ 	addiu	$t8,$t8,-6400
/*  f0fc0e0:	000fc8c0 */ 	sll	$t9,$t7,0x3
/*  f0fc0e4:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fc0e8:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fc0ec:	032fc821 */ 	addu	$t9,$t9,$t7
/*  f0fc0f0:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fc0f4:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fc0f8:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fc0fc:	03383821 */ 	addu	$a3,$t9,$t8
/*  f0fc100:	240e000a */ 	li	$t6,0xa
.JF0f0fc104:
/*  f0fc104:	a0ee083c */ 	sb	$t6,0x83c($a3)
/*  f0fc108:	83af010e */ 	lb	$t7,0x10e($sp)
/*  f0fc10c:	8fb9011c */ 	lw	$t9,0x11c($sp)
/*  f0fc110:	51e0000f */ 	beqzl	$t7,.JF0f0fc150
/*  f0fc114:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc118:	5720000d */ 	bnezl	$t9,.JF0f0fc150
/*  f0fc11c:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc120:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fc124:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fc128:	53000009 */ 	beqzl	$t8,.JF0f0fc150
/*  f0fc12c:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc130:	8dcf0000 */ 	lw	$t7,0x0($t6)
/*  f0fc134:	8df90010 */ 	lw	$t9,0x10($t7)
/*  f0fc138:	33380020 */ 	andi	$t8,$t9,0x20
/*  f0fc13c:	57000004 */ 	bnezl	$t8,.JF0f0fc150
/*  f0fc140:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fc144:	0fc3e386 */ 	jal	func0f0f8120
/*  f0fc148:	00000000 */ 	nop
.JF0f0fc14c:
/*  f0fc14c:	8fbf003c */ 	lw	$ra,0x3c($sp)
.JF0f0fc150:
/*  f0fc150:	8fb00018 */ 	lw	$s0,0x18($sp)
/*  f0fc154:	8fb1001c */ 	lw	$s1,0x1c($sp)
/*  f0fc158:	8fb20020 */ 	lw	$s2,0x20($sp)
/*  f0fc15c:	8fb30024 */ 	lw	$s3,0x24($sp)
/*  f0fc160:	8fb40028 */ 	lw	$s4,0x28($sp)
/*  f0fc164:	8fb5002c */ 	lw	$s5,0x2c($sp)
/*  f0fc168:	8fb60030 */ 	lw	$s6,0x30($sp)
/*  f0fc16c:	8fb70034 */ 	lw	$s7,0x34($sp)
/*  f0fc170:	8fbe0038 */ 	lw	$s8,0x38($sp)
/*  f0fc174:	03e00008 */ 	jr	$ra
/*  f0fc178:	27bd0130 */ 	addiu	$sp,$sp,0x130
);
#elif VERSION >= VERSION_PAL_FINAL
GLOBAL_ASM(
glabel menuProcessInput
.late_rodata
glabel var7f1b2a98
.word menuProcessInput+0xd64
glabel var7f1b2a9c
.word menuProcessInput+0xd64
glabel var7f1b2aa0
.word menuProcessInput+0xc8c
glabel var7f1b2aa4
.word menuProcessInput+0xd14
glabel var7f1b2aa8
.word menuProcessInput+0xd64
glabel var7f1b2aac
.word menuProcessInput+0xda8
glabel var7f1b2ab0
.word menuProcessInput+0xda8
glabel var7f1b2ab4
.word menuProcessInput+0xda8
glabel var7f1b2ab8
.word menuProcessInput+0xda8
glabel var7f1b2abc
.word menuProcessInput+0xda8
glabel var7f1b2ac0
.word menuProcessInput+0xc8c
glabel var7f1b2ac4
.word menuProcessInput+0xda8
glabel var7f1b2ac8
.word menuProcessInput+0xd64
.text
/*  f0fae40:	27bdfed0 */ 	addiu	$sp,$sp,-304
/*  f0fae44:	afb70034 */ 	sw	$s7,0x34($sp)
/*  f0fae48:	3c178007 */ 	lui	$s7,0x8007
/*  f0fae4c:	26f71728 */ 	addiu	$s7,$s7,0x1728
/*  f0fae50:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fae54:	afbe0038 */ 	sw	$s8,0x38($sp)
/*  f0fae58:	241e0e70 */ 	li	$s8,0xe70
/*  f0fae5c:	01de0019 */ 	multu	$t6,$s8
/*  f0fae60:	afb60030 */ 	sw	$s6,0x30($sp)
/*  f0fae64:	3c16800a */ 	lui	$s6,0x800a
/*  f0fae68:	26d6e5a0 */ 	addiu	$s6,$s6,-6752
/*  f0fae6c:	3c19800a */ 	lui	$t9,0x800a
/*  f0fae70:	8f391f64 */ 	lw	$t9,0x1f64($t9)
/*  f0fae74:	afb5002c */ 	sw	$s5,0x2c($sp)
/*  f0fae78:	24010004 */ 	li	$at,0x4
/*  f0fae7c:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0fae80:	afb40028 */ 	sw	$s4,0x28($sp)
/*  f0fae84:	00007812 */ 	mflo	$t7
/*  f0fae88:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fae8c:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fae90:	afb30024 */ 	sw	$s3,0x24($sp)
/*  f0fae94:	afb20020 */ 	sw	$s2,0x20($sp)
/*  f0fae98:	afb1001c */ 	sw	$s1,0x1c($sp)
/*  f0fae9c:	afb00018 */ 	sw	$s0,0x18($sp)
/*  f0faea0:	00006025 */ 	move	$t4,$zero
/*  f0faea4:	00004825 */ 	move	$t1,$zero
/*  f0faea8:	00004025 */ 	move	$t0,$zero
/*  f0faeac:	0000a825 */ 	move	$s5,$zero
/*  f0faeb0:	afa0011c */ 	sw	$zero,0x11c($sp)
/*  f0faeb4:	afa700fc */ 	sw	$a3,0xfc($sp)
/*  f0faeb8:	17210005 */ 	bne	$t9,$at,.PF0f0faed0
/*  f0faebc:	afb80100 */ 	sw	$t8,0x100($sp)
/*  f0faec0:	3c0e800a */ 	lui	$t6,0x800a
/*  f0faec4:	8dcea79c */ 	lw	$t6,-0x5864($t6)
/*  f0faec8:	3c01800a */ 	lui	$at,0x800a
/*  f0faecc:	ac2e2758 */ 	sw	$t6,0x2758($at)
.PF0f0faed0:
/*  f0faed0:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0faed4:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0faed8:	0fc3c6d1 */ 	jal	func0f0f1418
/*  f0faedc:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0faee0:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0faee4:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0faee8:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0faeec:	01fe0019 */ 	multu	$t7,$s8
/*  f0faef0:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0faef4:	a3a00106 */ 	sb	$zero,0x106($sp)
/*  f0faef8:	a3a00107 */ 	sb	$zero,0x107($sp)
/*  f0faefc:	a3a0010a */ 	sb	$zero,0x10a($sp)
/*  f0faf00:	a3a0010b */ 	sb	$zero,0x10b($sp)
/*  f0faf04:	00005025 */ 	move	$t2,$zero
/*  f0faf08:	00005825 */ 	move	$t3,$zero
/*  f0faf0c:	3c0f800a */ 	lui	$t7,0x800a
/*  f0faf10:	0000c012 */ 	mflo	$t8
/*  f0faf14:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0faf18:	8f2e04f8 */ 	lw	$t6,0x4f8($t9)
/*  f0faf1c:	51c00333 */ 	beqzl	$t6,.PF0f0fbbec
/*  f0faf20:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0faf24:	8def1f64 */ 	lw	$t7,0x1f64($t7)
/*  f0faf28:	24010007 */ 	li	$at,0x7
/*  f0faf2c:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*  f0faf30:	15e1000b */ 	bne	$t7,$at,.PF0f0faf60
/*  f0faf34:	afa000ec */ 	sw	$zero,0xec($sp)
/*  f0faf38:	24180001 */ 	li	$t8,0x1
/*  f0faf3c:	24190002 */ 	li	$t9,0x2
/*  f0faf40:	240e0003 */ 	li	$t6,0x3
/*  f0faf44:	240f0004 */ 	li	$t7,0x4
/*  f0faf48:	afa000d8 */ 	sw	$zero,0xd8($sp)
/*  f0faf4c:	afb800dc */ 	sw	$t8,0xdc($sp)
/*  f0faf50:	afb900e0 */ 	sw	$t9,0xe0($sp)
/*  f0faf54:	afae00e4 */ 	sw	$t6,0xe4($sp)
/*  f0faf58:	1000001b */ 	b	.PF0f0fafc8
/*  f0faf5c:	afaf00ec */ 	sw	$t7,0xec($sp)
.PF0f0faf60:
/*  f0faf60:	27a400d7 */ 	addiu	$a0,$sp,0xd7
/*  f0faf64:	27a500d6 */ 	addiu	$a1,$sp,0xd6
/*  f0faf68:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0faf6c:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0faf70:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0faf74:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0faf78:	0fc3defb */ 	jal	menuGetContPads
/*  f0faf7c:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0faf80:	83a200d7 */ 	lb	$v0,0xd7($sp)
/*  f0faf84:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0faf88:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0faf8c:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0faf90:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0faf94:	04400004 */ 	bltz	$v0,.PF0f0fafa8
/*  f0faf98:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0faf9c:	24030001 */ 	li	$v1,0x1
/*  f0fafa0:	afa200d8 */ 	sw	$v0,0xd8($sp)
/*  f0fafa4:	afa300ec */ 	sw	$v1,0xec($sp)
.PF0f0fafa8:
/*  f0fafa8:	83a200d6 */ 	lb	$v0,0xd6($sp)
/*  f0fafac:	8fa300ec */ 	lw	$v1,0xec($sp)
/*  f0fafb0:	04400005 */ 	bltz	$v0,.PF0f0fafc8
/*  f0fafb4:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fafb8:	03b8c821 */ 	addu	$t9,$sp,$t8
/*  f0fafbc:	af2200d8 */ 	sw	$v0,0xd8($t9)
/*  f0fafc0:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0fafc4:	afa300ec */ 	sw	$v1,0xec($sp)
.PF0f0fafc8:
/*  f0fafc8:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fafcc:	01de0019 */ 	multu	$t6,$s8
/*  f0fafd0:	00007812 */ 	mflo	$t7
/*  f0fafd4:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fafd8:	90e20e41 */ 	lbu	$v0,0xe41($a3)
/*  f0fafdc:	1840001a */ 	blez	$v0,.PF0f0fb048
/*  f0fafe0:	2458ffff */ 	addiu	$t8,$v0,-1
/*  f0fafe4:	331900ff */ 	andi	$t9,$t8,0xff
/*  f0fafe8:	17200014 */ 	bnez	$t9,.PF0f0fb03c
/*  f0fafec:	a0f80e41 */ 	sb	$t8,0xe41($a3)
/*  f0faff0:	8cee0e48 */ 	lw	$t6,0xe48($a3)
/*  f0faff4:	8cef0e4c */ 	lw	$t7,0xe4c($a3)
/*  f0faff8:	27a400cc */ 	addiu	$a0,$sp,0xcc
/*  f0faffc:	2405ffff */ 	li	$a1,-1
/*  f0fb000:	00003025 */ 	move	$a2,$zero
/*  f0fb004:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb008:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb00c:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fb010:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fb014:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb018:	afae00cc */ 	sw	$t6,0xcc($sp)
/*  f0fb01c:	0fc42793 */ 	jal	filemgrSaveOrLoad
/*  f0fb020:	a7af00d0 */ 	sh	$t7,0xd0($sp)
/*  f0fb024:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb028:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb02c:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fb030:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fb034:	10000004 */ 	b	.PF0f0fb048
/*  f0fb038:	8fac012c */ 	lw	$t4,0x12c($sp)
.PF0f0fb03c:
/*  f0fb03c:	90f90e40 */ 	lbu	$t9,0xe40($a3)
/*  f0fb040:	372e0080 */ 	ori	$t6,$t9,0x80
/*  f0fb044:	a0ee0e40 */ 	sb	$t6,0xe40($a3)
.PF0f0fb048:
/*  f0fb048:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0fb04c:	0000a025 */ 	move	$s4,$zero
/*  f0fb050:	19e00083 */ 	blez	$t7,.PF0f0fb260
/*  f0fb054:	27b300d8 */ 	addiu	$s3,$sp,0xd8
.PF0f0fb058:
/*  f0fb058:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb05c:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fb060:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fb064:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fb068:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fb06c:	0c0052e6 */ 	jal	joyGetStickX
/*  f0fb070:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fb074:	00028600 */ 	sll	$s0,$v0,0x18
/*  f0fb078:	0010c603 */ 	sra	$t8,$s0,0x18
/*  f0fb07c:	03008025 */ 	move	$s0,$t8
/*  f0fb080:	0c005310 */ 	jal	joyGetStickY
/*  f0fb084:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb088:	00028e00 */ 	sll	$s1,$v0,0x18
/*  f0fb08c:	0011ce03 */ 	sra	$t9,$s1,0x18
/*  f0fb090:	03208825 */ 	move	$s1,$t9
/*  f0fb094:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb098:	0c00533a */ 	jal	joyGetButtons
/*  f0fb09c:	3405ffff */ 	li	$a1,0xffff
/*  f0fb0a0:	3052ffff */ 	andi	$s2,$v0,0xffff
/*  f0fb0a4:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fb0a8:	0c00536a */ 	jal	joyGetButtonsPressedThisFrame
/*  f0fb0ac:	3405ffff */ 	li	$a1,0xffff
/*  f0fb0b0:	304e8000 */ 	andi	$t6,$v0,0x8000
/*  f0fb0b4:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fb0b8:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fb0bc:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fb0c0:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fb0c4:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fb0c8:	11c00003 */ 	beqz	$t6,.PF0f0fb0d8
/*  f0fb0cc:	00403025 */ 	move	$a2,$v0
/*  f0fb0d0:	240f0001 */ 	li	$t7,0x1
/*  f0fb0d4:	a3af0106 */ 	sb	$t7,0x106($sp)
.PF0f0fb0d8:
/*  f0fb0d8:	30d84000 */ 	andi	$t8,$a2,0x4000
/*  f0fb0dc:	13000003 */ 	beqz	$t8,.PF0f0fb0ec
/*  f0fb0e0:	30ce2000 */ 	andi	$t6,$a2,0x2000
/*  f0fb0e4:	24190001 */ 	li	$t9,0x1
/*  f0fb0e8:	a3b90107 */ 	sb	$t9,0x107($sp)
.PF0f0fb0ec:
/*  f0fb0ec:	11c00003 */ 	beqz	$t6,.PF0f0fb0fc
/*  f0fb0f0:	30d81000 */ 	andi	$t8,$a2,0x1000
/*  f0fb0f4:	240f0001 */ 	li	$t7,0x1
/*  f0fb0f8:	a3af0106 */ 	sb	$t7,0x106($sp)
.PF0f0fb0fc:
/*  f0fb0fc:	13000003 */ 	beqz	$t8,.PF0f0fb10c
/*  f0fb100:	324e0010 */ 	andi	$t6,$s2,0x10
/*  f0fb104:	24190001 */ 	li	$t9,0x1
/*  f0fb108:	afb900f8 */ 	sw	$t9,0xf8($sp)
.PF0f0fb10c:
/*  f0fb10c:	11c00003 */ 	beqz	$t6,.PF0f0fb11c
/*  f0fb110:	02402825 */ 	move	$a1,$s2
/*  f0fb114:	240f0001 */ 	li	$t7,0x1
/*  f0fb118:	a3af010a */ 	sb	$t7,0x10a($sp)
.PF0f0fb11c:
/*  f0fb11c:	30b80020 */ 	andi	$t8,$a1,0x20
/*  f0fb120:	13000003 */ 	beqz	$t8,.PF0f0fb130
/*  f0fb124:	30ae0008 */ 	andi	$t6,$a1,0x8
/*  f0fb128:	24190001 */ 	li	$t9,0x1
/*  f0fb12c:	a3b9010a */ 	sb	$t9,0x10a($sp)
.PF0f0fb130:
/*  f0fb130:	05410003 */ 	bgez	$t2,.PF0f0fb140
/*  f0fb134:	30cf0008 */ 	andi	$t7,$a2,0x8
/*  f0fb138:	10000002 */ 	b	.PF0f0fb144
/*  f0fb13c:	000a2023 */ 	negu	$a0,$t2
.PF0f0fb140:
/*  f0fb140:	01402025 */ 	move	$a0,$t2
.PF0f0fb144:
/*  f0fb144:	06010003 */ 	bgez	$s0,.PF0f0fb154
/*  f0fb148:	30b80004 */ 	andi	$t8,$a1,0x4
/*  f0fb14c:	10000002 */ 	b	.PF0f0fb158
/*  f0fb150:	00101823 */ 	negu	$v1,$s0
.PF0f0fb154:
/*  f0fb154:	02001825 */ 	move	$v1,$s0
.PF0f0fb158:
/*  f0fb158:	0083082a */ 	slt	$at,$a0,$v1
/*  f0fb15c:	10200002 */ 	beqz	$at,.PF0f0fb168
/*  f0fb160:	30d90004 */ 	andi	$t9,$a2,0x4
/*  f0fb164:	02005025 */ 	move	$t2,$s0
.PF0f0fb168:
/*  f0fb168:	05610003 */ 	bgez	$t3,.PF0f0fb178
/*  f0fb16c:	01602025 */ 	move	$a0,$t3
/*  f0fb170:	10000001 */ 	b	.PF0f0fb178
/*  f0fb174:	000b2023 */ 	negu	$a0,$t3
.PF0f0fb178:
/*  f0fb178:	06210003 */ 	bgez	$s1,.PF0f0fb188
/*  f0fb17c:	02201825 */ 	move	$v1,$s1
/*  f0fb180:	10000001 */ 	b	.PF0f0fb188
/*  f0fb184:	00111823 */ 	negu	$v1,$s1
.PF0f0fb188:
/*  f0fb188:	0083082a */ 	slt	$at,$a0,$v1
/*  f0fb18c:	10200002 */ 	beqz	$at,.PF0f0fb198
/*  f0fb190:	00000000 */ 	nop
/*  f0fb194:	02205825 */ 	move	$t3,$s1
.PF0f0fb198:
/*  f0fb198:	11c00002 */ 	beqz	$t6,.PF0f0fb1a4
/*  f0fb19c:	00000000 */ 	nop
/*  f0fb1a0:	240cffff */ 	li	$t4,-1
.PF0f0fb1a4:
/*  f0fb1a4:	11e00002 */ 	beqz	$t7,.PF0f0fb1b0
/*  f0fb1a8:	30ae0002 */ 	andi	$t6,$a1,0x2
/*  f0fb1ac:	2408ffff */ 	li	$t0,-1
.PF0f0fb1b0:
/*  f0fb1b0:	13000002 */ 	beqz	$t8,.PF0f0fb1bc
/*  f0fb1b4:	30cf0002 */ 	andi	$t7,$a2,0x2
/*  f0fb1b8:	240c0001 */ 	li	$t4,0x1
.PF0f0fb1bc:
/*  f0fb1bc:	13200002 */ 	beqz	$t9,.PF0f0fb1c8
/*  f0fb1c0:	30b80001 */ 	andi	$t8,$a1,0x1
/*  f0fb1c4:	24080001 */ 	li	$t0,0x1
.PF0f0fb1c8:
/*  f0fb1c8:	11c00002 */ 	beqz	$t6,.PF0f0fb1d4
/*  f0fb1cc:	30d90001 */ 	andi	$t9,$a2,0x1
/*  f0fb1d0:	2409ffff */ 	li	$t1,-1
.PF0f0fb1d4:
/*  f0fb1d4:	11e00002 */ 	beqz	$t7,.PF0f0fb1e0
/*  f0fb1d8:	30ae0800 */ 	andi	$t6,$a1,0x800
/*  f0fb1dc:	2415ffff */ 	li	$s5,-1
.PF0f0fb1e0:
/*  f0fb1e0:	13000002 */ 	beqz	$t8,.PF0f0fb1ec
/*  f0fb1e4:	30cf0800 */ 	andi	$t7,$a2,0x800
/*  f0fb1e8:	24090001 */ 	li	$t1,0x1
.PF0f0fb1ec:
/*  f0fb1ec:	13200002 */ 	beqz	$t9,.PF0f0fb1f8
/*  f0fb1f0:	30b80400 */ 	andi	$t8,$a1,0x400
/*  f0fb1f4:	24150001 */ 	li	$s5,0x1
.PF0f0fb1f8:
/*  f0fb1f8:	11c00002 */ 	beqz	$t6,.PF0f0fb204
/*  f0fb1fc:	30d90400 */ 	andi	$t9,$a2,0x400
/*  f0fb200:	240cffff */ 	li	$t4,-1
.PF0f0fb204:
/*  f0fb204:	11e00002 */ 	beqz	$t7,.PF0f0fb210
/*  f0fb208:	30ae0200 */ 	andi	$t6,$a1,0x200
/*  f0fb20c:	2408ffff */ 	li	$t0,-1
.PF0f0fb210:
/*  f0fb210:	13000002 */ 	beqz	$t8,.PF0f0fb21c
/*  f0fb214:	30cf0200 */ 	andi	$t7,$a2,0x200
/*  f0fb218:	240c0001 */ 	li	$t4,0x1
.PF0f0fb21c:
/*  f0fb21c:	13200002 */ 	beqz	$t9,.PF0f0fb228
/*  f0fb220:	30b80100 */ 	andi	$t8,$a1,0x100
/*  f0fb224:	24080001 */ 	li	$t0,0x1
.PF0f0fb228:
/*  f0fb228:	11c00002 */ 	beqz	$t6,.PF0f0fb234
/*  f0fb22c:	30d90100 */ 	andi	$t9,$a2,0x100
/*  f0fb230:	2409ffff */ 	li	$t1,-1
.PF0f0fb234:
/*  f0fb234:	11e00002 */ 	beqz	$t7,.PF0f0fb240
/*  f0fb238:	8fae00ec */ 	lw	$t6,0xec($sp)
/*  f0fb23c:	2415ffff */ 	li	$s5,-1
.PF0f0fb240:
/*  f0fb240:	13000002 */ 	beqz	$t8,.PF0f0fb24c
/*  f0fb244:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fb248:	24090001 */ 	li	$t1,0x1
.PF0f0fb24c:
/*  f0fb24c:	13200002 */ 	beqz	$t9,.PF0f0fb258
/*  f0fb250:	00000000 */ 	nop
/*  f0fb254:	24150001 */ 	li	$s5,0x1
.PF0f0fb258:
/*  f0fb258:	168eff7f */ 	bne	$s4,$t6,.PF0f0fb058
/*  f0fb25c:	26730004 */ 	addiu	$s3,$s3,0x4
.PF0f0fb260:
/*  f0fb260:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fb264:	93ae0106 */ 	lbu	$t6,0x106($sp)
/*  f0fb268:	24100008 */ 	li	$s0,0x8
/*  f0fb26c:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0fb270:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb274:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0fb278:	030fc021 */ 	addu	$t8,$t8,$t7
/*  f0fb27c:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb280:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb284:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0fb288:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fb28c:	11c00002 */ 	beqz	$t6,.PF0f0fb298
/*  f0fb290:	8f2404f8 */ 	lw	$a0,0x4f8($t9)
/*  f0fb294:	a3a00107 */ 	sb	$zero,0x107($sp)
.PF0f0fb298:
/*  f0fb298:	11000002 */ 	beqz	$t0,.PF0f0fb2a4
/*  f0fb29c:	2406001e */ 	li	$a2,0x1e
/*  f0fb2a0:	01006025 */ 	move	$t4,$t0
.PF0f0fb2a4:
/*  f0fb2a4:	12a00002 */ 	beqz	$s5,.PF0f0fb2b0
/*  f0fb2a8:	2413003c */ 	li	$s3,0x3c
/*  f0fb2ac:	02a04825 */ 	move	$t1,$s5
.PF0f0fb2b0:
/*  f0fb2b0:	24110021 */ 	li	$s1,0x21
/*  f0fb2b4:	2414002c */ 	li	$s4,0x2c
/*  f0fb2b8:	10800019 */ 	beqz	$a0,.PF0f0fb320
/*  f0fb2bc:	00009025 */ 	move	$s2,$zero
/*  f0fb2c0:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb2c4:	10400016 */ 	beqz	$v0,.PF0f0fb320
/*  f0fb2c8:	00000000 */ 	nop
/*  f0fb2cc:	90430000 */ 	lbu	$v1,0x0($v0)
/*  f0fb2d0:	24010008 */ 	li	$at,0x8
/*  f0fb2d4:	10610003 */ 	beq	$v1,$at,.PF0f0fb2e4
/*  f0fb2d8:	24010010 */ 	li	$at,0x10
/*  f0fb2dc:	54610009 */ 	bnel	$v1,$at,.PF0f0fb304
/*  f0fb2e0:	2401000d */ 	li	$at,0xd
.PF0f0fb2e4:
/*  f0fb2e4:	8c8f000c */ 	lw	$t7,0xc($a0)
/*  f0fb2e8:	51e00006 */ 	beqzl	$t7,.PF0f0fb304
/*  f0fb2ec:	2401000d */ 	li	$at,0xd
/*  f0fb2f0:	24100004 */ 	li	$s0,0x4
/*  f0fb2f4:	24060014 */ 	li	$a2,0x14
/*  f0fb2f8:	2413001e */ 	li	$s3,0x1e
/*  f0fb2fc:	2411000a */ 	li	$s1,0xa
/*  f0fb300:	2401000d */ 	li	$at,0xd
.PF0f0fb304:
/*  f0fb304:	14610006 */ 	bne	$v1,$at,.PF0f0fb320
/*  f0fb308:	00000000 */ 	nop
/*  f0fb30c:	24120001 */ 	li	$s2,0x1
/*  f0fb310:	24100004 */ 	li	$s0,0x4
/*  f0fb314:	24060014 */ 	li	$a2,0x14
/*  f0fb318:	2411000a */ 	li	$s1,0xa
/*  f0fb31c:	2414000a */ 	li	$s4,0xa
.PF0f0fb320:
/*  f0fb320:	15200004 */ 	bnez	$t1,.PF0f0fb334
/*  f0fb324:	0000f825 */ 	move	$ra,$zero
/*  f0fb328:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb32c:	2418ffff */ 	li	$t8,-1
/*  f0fb330:	a478082c */ 	sh	$t8,0x82c($v1)
.PF0f0fb334:
/*  f0fb334:	12a00006 */ 	beqz	$s5,.PF0f0fb350
/*  f0fb338:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb33c:	a460082c */ 	sh	$zero,0x82c($v1)
/*  f0fb340:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fb344:	a475082a */ 	sh	$s5,0x82a($v1)
/*  f0fb348:	10000004 */ 	b	.PF0f0fb35c
/*  f0fb34c:	241f0001 */ 	li	$ra,0x1
.PF0f0fb350:
/*  f0fb350:	51200003 */ 	beqzl	$t1,.PF0f0fb360
/*  f0fb354:	8c620824 */ 	lw	$v0,0x824($v1)
/*  f0fb358:	8469082a */ 	lh	$t1,0x82a($v1)
.PF0f0fb35c:
/*  f0fb35c:	8c620824 */ 	lw	$v0,0x824($v1)
.PF0f0fb360:
/*  f0fb360:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fb364:	28410033 */ 	slti	$at,$v0,0x33
/*  f0fb368:	14200004 */ 	bnez	$at,.PF0f0fb37c
/*  f0fb36c:	00000000 */ 	nop
/*  f0fb370:	24190001 */ 	li	$t9,0x1
/*  f0fb374:	a479082c */ 	sh	$t9,0x82c($v1)
/*  f0fb378:	8c620824 */ 	lw	$v0,0x824($v1)
.PF0f0fb37c:
/*  f0fb37c:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fb380:	8dcea510 */ 	lw	$t6,-0x5af0($t6)
/*  f0fb384:	00003812 */ 	mflo	$a3
/*  f0fb388:	8478082c */ 	lh	$t8,0x82c($v1)
/*  f0fb38c:	004e7821 */ 	addu	$t7,$v0,$t6
/*  f0fb390:	01f0001a */ 	div	$zero,$t7,$s0
/*  f0fb394:	16000002 */ 	bnez	$s0,.PF0f0fb3a0
/*  f0fb398:	00000000 */ 	nop
/*  f0fb39c:	0007000d */ 	break	0x7
.PF0f0fb3a0:
/*  f0fb3a0:	2401ffff */ 	li	$at,-1
/*  f0fb3a4:	16010004 */ 	bne	$s0,$at,.PF0f0fb3b8
/*  f0fb3a8:	3c018000 */ 	lui	$at,0x8000
/*  f0fb3ac:	14410002 */ 	bne	$v0,$at,.PF0f0fb3b8
/*  f0fb3b0:	00000000 */ 	nop
/*  f0fb3b4:	0006000d */ 	break	0x6
.PF0f0fb3b8:
/*  f0fb3b8:	00006812 */ 	mflo	$t5
/*  f0fb3bc:	16000002 */ 	bnez	$s0,.PF0f0fb3c8
/*  f0fb3c0:	00000000 */ 	nop
/*  f0fb3c4:	0007000d */ 	break	0x7
.PF0f0fb3c8:
/*  f0fb3c8:	2401ffff */ 	li	$at,-1
/*  f0fb3cc:	16010004 */ 	bne	$s0,$at,.PF0f0fb3e0
/*  f0fb3d0:	3c018000 */ 	lui	$at,0x8000
/*  f0fb3d4:	15e10002 */ 	bne	$t7,$at,.PF0f0fb3e0
/*  f0fb3d8:	00000000 */ 	nop
/*  f0fb3dc:	0006000d */ 	break	0x6
.PF0f0fb3e0:
/*  f0fb3e0:	1700000b */ 	bnez	$t8,.PF0f0fb410
/*  f0fb3e4:	00000000 */ 	nop
/*  f0fb3e8:	04e10003 */ 	bgez	$a3,.PF0f0fb3f8
/*  f0fb3ec:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0fb3f0:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fb3f4:	0001c843 */ 	sra	$t9,$at,0x1
.PF0f0fb3f8:
/*  f0fb3f8:	03203825 */ 	move	$a3,$t9
/*  f0fb3fc:	05a10003 */ 	bgez	$t5,.PF0f0fb40c
/*  f0fb400:	000d7043 */ 	sra	$t6,$t5,0x1
/*  f0fb404:	25a10001 */ 	addiu	$at,$t5,0x1
/*  f0fb408:	00017043 */ 	sra	$t6,$at,0x1
.PF0f0fb40c:
/*  f0fb40c:	01c06825 */ 	move	$t5,$t6
.PF0f0fb410:
/*  f0fb410:	05410003 */ 	bgez	$t2,.PF0f0fb420
/*  f0fb414:	a3a9010c */ 	sb	$t1,0x10c($sp)
/*  f0fb418:	10000002 */ 	b	.PF0f0fb424
/*  f0fb41c:	000a1023 */ 	negu	$v0,$t2
.PF0f0fb420:
/*  f0fb420:	01401025 */ 	move	$v0,$t2
.PF0f0fb424:
/*  f0fb424:	05610003 */ 	bgez	$t3,.PF0f0fb434
/*  f0fb428:	0046082a */ 	slt	$at,$v0,$a2
/*  f0fb42c:	10000002 */ 	b	.PF0f0fb438
/*  f0fb430:	000b1823 */ 	negu	$v1,$t3
.PF0f0fb434:
/*  f0fb434:	01601825 */ 	move	$v1,$t3
.PF0f0fb438:
/*  f0fb438:	14200070 */ 	bnez	$at,.PF0f0fb5fc
/*  f0fb43c:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fb440:	14200003 */ 	bnez	$at,.PF0f0fb450
/*  f0fb444:	00000000 */ 	nop
/*  f0fb448:	5240006d */ 	beqzl	$s2,.PF0f0fb600
/*  f0fb44c:	8fae00fc */ 	lw	$t6,0xfc($sp)
.PF0f0fb450:
/*  f0fb450:	05410006 */ 	bgez	$t2,.PF0f0fb46c
/*  f0fb454:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fb458:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb45c:	846f0828 */ 	lh	$t7,0x828($v1)
/*  f0fb460:	59e00003 */ 	blezl	$t7,.PF0f0fb470
/*  f0fb464:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb468:	a4600828 */ 	sh	$zero,0x828($v1)
.PF0f0fb46c:
/*  f0fb46c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb470:
/*  f0fb470:	19400005 */ 	blez	$t2,.PF0f0fb488
/*  f0fb474:	84650828 */ 	lh	$a1,0x828($v1)
/*  f0fb478:	04a10003 */ 	bgez	$a1,.PF0f0fb488
/*  f0fb47c:	00000000 */ 	nop
/*  f0fb480:	a4600828 */ 	sh	$zero,0x828($v1)
/*  f0fb484:	84650828 */ 	lh	$a1,0x828($v1)
.PF0f0fb488:
/*  f0fb488:	14a00003 */ 	bnez	$a1,.PF0f0fb498
/*  f0fb48c:	00000000 */ 	nop
/*  f0fb490:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fb494:	84650828 */ 	lh	$a1,0x828($v1)
.PF0f0fb498:
/*  f0fb498:	54200003 */ 	bnezl	$at,.PF0f0fb4a8
/*  f0fb49c:	00461023 */ 	subu	$v0,$v0,$a2
/*  f0fb4a0:	24020046 */ 	li	$v0,0x46
/*  f0fb4a4:	00461023 */ 	subu	$v0,$v0,$a2
.PF0f0fb4a8:
/*  f0fb4a8:	02220019 */ 	multu	$s1,$v0
/*  f0fb4ac:	24190046 */ 	li	$t9,0x46
/*  f0fb4b0:	03267023 */ 	subu	$t6,$t9,$a2
/*  f0fb4b4:	0000c012 */ 	mflo	$t8
/*  f0fb4b8:	00000000 */ 	nop
/*  f0fb4bc:	00000000 */ 	nop
/*  f0fb4c0:	030e001a */ 	div	$zero,$t8,$t6
/*  f0fb4c4:	15c00002 */ 	bnez	$t6,.PF0f0fb4d0
/*  f0fb4c8:	00000000 */ 	nop
/*  f0fb4cc:	0007000d */ 	break	0x7
.PF0f0fb4d0:
/*  f0fb4d0:	2401ffff */ 	li	$at,-1
/*  f0fb4d4:	15c10004 */ 	bne	$t6,$at,.PF0f0fb4e8
/*  f0fb4d8:	3c018000 */ 	lui	$at,0x8000
/*  f0fb4dc:	17010002 */ 	bne	$t8,$at,.PF0f0fb4e8
/*  f0fb4e0:	00000000 */ 	nop
/*  f0fb4e4:	0006000d */ 	break	0x6
.PF0f0fb4e8:
/*  f0fb4e8:	00007812 */ 	mflo	$t7
/*  f0fb4ec:	026f2023 */ 	subu	$a0,$s3,$t7
/*  f0fb4f0:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fb4f4:	10200004 */ 	beqz	$at,.PF0f0fb508
/*  f0fb4f8:	00801825 */ 	move	$v1,$a0
/*  f0fb4fc:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fb500:	10200005 */ 	beqz	$at,.PF0f0fb518
/*  f0fb504:	00000000 */ 	nop
.PF0f0fb508:
/*  f0fb508:	04810003 */ 	bgez	$a0,.PF0f0fb518
/*  f0fb50c:	00041843 */ 	sra	$v1,$a0,0x1
/*  f0fb510:	24810001 */ 	addiu	$at,$a0,0x1
/*  f0fb514:	00011843 */ 	sra	$v1,$at,0x1
.PF0f0fb518:
/*  f0fb518:	1860003a */ 	blez	$v1,.PF0f0fb604
/*  f0fb51c:	28610004 */ 	slti	$at,$v1,0x4
/*  f0fb520:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb524:	1420000b */ 	bnez	$at,.PF0f0fb554
/*  f0fb528:	8cc6a510 */ 	lw	$a2,-0x5af0($a2)
/*  f0fb52c:	0003c880 */ 	sll	$t9,$v1,0x2
/*  f0fb530:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb534:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fb538:	0323c821 */ 	addu	$t9,$t9,$v1
/*  f0fb53c:	0019c840 */ 	sll	$t9,$t9,0x1
/*  f0fb540:	2401003c */ 	li	$at,0x3c
/*  f0fb544:	0321001a */ 	div	$zero,$t9,$at
/*  f0fb548:	0000c012 */ 	mflo	$t8
/*  f0fb54c:	03001825 */ 	move	$v1,$t8
/*  f0fb550:	00000000 */ 	nop
.PF0f0fb554:
/*  f0fb554:	8fae00fc */ 	lw	$t6,0xfc($sp)
/*  f0fb558:	24090001 */ 	li	$t1,0x1
/*  f0fb55c:	8dc20824 */ 	lw	$v0,0x824($t6)
/*  f0fb560:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fb564:	00467821 */ 	addu	$t7,$v0,$a2
/*  f0fb568:	00003812 */ 	mflo	$a3
/*  f0fb56c:	14600002 */ 	bnez	$v1,.PF0f0fb578
/*  f0fb570:	00000000 */ 	nop
/*  f0fb574:	0007000d */ 	break	0x7
.PF0f0fb578:
/*  f0fb578:	2401ffff */ 	li	$at,-1
/*  f0fb57c:	14610004 */ 	bne	$v1,$at,.PF0f0fb590
/*  f0fb580:	3c018000 */ 	lui	$at,0x8000
/*  f0fb584:	14410002 */ 	bne	$v0,$at,.PF0f0fb590
/*  f0fb588:	00000000 */ 	nop
/*  f0fb58c:	0006000d */ 	break	0x6
.PF0f0fb590:
/*  f0fb590:	01e3001a */ 	div	$zero,$t7,$v1
/*  f0fb594:	00006812 */ 	mflo	$t5
/*  f0fb598:	14600002 */ 	bnez	$v1,.PF0f0fb5a4
/*  f0fb59c:	00000000 */ 	nop
/*  f0fb5a0:	0007000d */ 	break	0x7
.PF0f0fb5a4:
/*  f0fb5a4:	2401ffff */ 	li	$at,-1
/*  f0fb5a8:	14610004 */ 	bne	$v1,$at,.PF0f0fb5bc
/*  f0fb5ac:	3c018000 */ 	lui	$at,0x8000
/*  f0fb5b0:	15e10002 */ 	bne	$t7,$at,.PF0f0fb5bc
/*  f0fb5b4:	00000000 */ 	nop
/*  f0fb5b8:	0006000d */ 	break	0x6
.PF0f0fb5bc:
/*  f0fb5bc:	05410003 */ 	bgez	$t2,.PF0f0fb5cc
/*  f0fb5c0:	00000000 */ 	nop
/*  f0fb5c4:	10000001 */ 	b	.PF0f0fb5cc
/*  f0fb5c8:	2409ffff */ 	li	$t1,-1
.PF0f0fb5cc:
/*  f0fb5cc:	10ed0002 */ 	beq	$a3,$t5,.PF0f0fb5d8
/*  f0fb5d0:	00000000 */ 	nop
/*  f0fb5d4:	241f0001 */ 	li	$ra,0x1
.PF0f0fb5d8:
/*  f0fb5d8:	14a00002 */ 	bnez	$a1,.PF0f0fb5e4
/*  f0fb5dc:	00000000 */ 	nop
/*  f0fb5e0:	241f0001 */ 	li	$ra,0x1
.PF0f0fb5e4:
/*  f0fb5e4:	13e00007 */ 	beqz	$ra,.PF0f0fb604
/*  f0fb5e8:	00000000 */ 	nop
/*  f0fb5ec:	8fb800fc */ 	lw	$t8,0xfc($sp)
/*  f0fb5f0:	00a9c821 */ 	addu	$t9,$a1,$t1
/*  f0fb5f4:	10000003 */ 	b	.PF0f0fb604
/*  f0fb5f8:	a7190828 */ 	sh	$t9,0x828($t8)
.PF0f0fb5fc:
/*  f0fb5fc:	8fae00fc */ 	lw	$t6,0xfc($sp)
.PF0f0fb600:
/*  f0fb600:	a5c00828 */ 	sh	$zero,0x828($t6)
.PF0f0fb604:
/*  f0fb604:	10ed0002 */ 	beq	$a3,$t5,.PF0f0fb610
/*  f0fb608:	3c18800a */ 	lui	$t8,0x800a
/*  f0fb60c:	241f0001 */ 	li	$ra,0x1
.PF0f0fb610:
/*  f0fb610:	17e00002 */ 	bnez	$ra,.PF0f0fb61c
/*  f0fb614:	00000000 */ 	nop
/*  f0fb618:	00004825 */ 	move	$t1,$zero
.PF0f0fb61c:
/*  f0fb61c:	11000007 */ 	beqz	$t0,.PF0f0fb63c
/*  f0fb620:	00006825 */ 	move	$t5,$zero
/*  f0fb624:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb628:	240d0001 */ 	li	$t5,0x1
/*  f0fb62c:	a4600838 */ 	sh	$zero,0x838($v1)
/*  f0fb630:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fb634:	10000005 */ 	b	.PF0f0fb64c
/*  f0fb638:	a4680836 */ 	sh	$t0,0x836($v1)
.PF0f0fb63c:
/*  f0fb63c:	51800004 */ 	beqzl	$t4,.PF0f0fb650
/*  f0fb640:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb644:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fb648:	85ec0836 */ 	lh	$t4,0x836($t7)
.PF0f0fb64c:
/*  f0fb64c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb650:
/*  f0fb650:	8c620830 */ 	lw	$v0,0x830($v1)
/*  f0fb654:	28410033 */ 	slti	$at,$v0,0x33
/*  f0fb658:	14200004 */ 	bnez	$at,.PF0f0fb66c
/*  f0fb65c:	00000000 */ 	nop
/*  f0fb660:	24190001 */ 	li	$t9,0x1
/*  f0fb664:	a4790838 */ 	sh	$t9,0x838($v1)
/*  f0fb668:	8c620830 */ 	lw	$v0,0x830($v1)
.PF0f0fb66c:
/*  f0fb66c:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fb670:	8f18a510 */ 	lw	$t8,-0x5af0($t8)
/*  f0fb674:	00003812 */ 	mflo	$a3
/*  f0fb678:	846f0838 */ 	lh	$t7,0x838($v1)
/*  f0fb67c:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0fb680:	01d0001a */ 	div	$zero,$t6,$s0
/*  f0fb684:	16000002 */ 	bnez	$s0,.PF0f0fb690
/*  f0fb688:	00000000 */ 	nop
/*  f0fb68c:	0007000d */ 	break	0x7
.PF0f0fb690:
/*  f0fb690:	2401ffff */ 	li	$at,-1
/*  f0fb694:	16010004 */ 	bne	$s0,$at,.PF0f0fb6a8
/*  f0fb698:	3c018000 */ 	lui	$at,0x8000
/*  f0fb69c:	14410002 */ 	bne	$v0,$at,.PF0f0fb6a8
/*  f0fb6a0:	00000000 */ 	nop
/*  f0fb6a4:	0006000d */ 	break	0x6
.PF0f0fb6a8:
/*  f0fb6a8:	00004012 */ 	mflo	$t0
/*  f0fb6ac:	01601025 */ 	move	$v0,$t3
/*  f0fb6b0:	16000002 */ 	bnez	$s0,.PF0f0fb6bc
/*  f0fb6b4:	00000000 */ 	nop
/*  f0fb6b8:	0007000d */ 	break	0x7
.PF0f0fb6bc:
/*  f0fb6bc:	2401ffff */ 	li	$at,-1
/*  f0fb6c0:	16010004 */ 	bne	$s0,$at,.PF0f0fb6d4
/*  f0fb6c4:	3c018000 */ 	lui	$at,0x8000
/*  f0fb6c8:	15c10002 */ 	bne	$t6,$at,.PF0f0fb6d4
/*  f0fb6cc:	00000000 */ 	nop
/*  f0fb6d0:	0006000d */ 	break	0x6
.PF0f0fb6d4:
/*  f0fb6d4:	15e0000b */ 	bnez	$t7,.PF0f0fb704
/*  f0fb6d8:	00000000 */ 	nop
/*  f0fb6dc:	04e10003 */ 	bgez	$a3,.PF0f0fb6ec
/*  f0fb6e0:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0fb6e4:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fb6e8:	0001c843 */ 	sra	$t9,$at,0x1
.PF0f0fb6ec:
/*  f0fb6ec:	03203825 */ 	move	$a3,$t9
/*  f0fb6f0:	05010003 */ 	bgez	$t0,.PF0f0fb700
/*  f0fb6f4:	0008c043 */ 	sra	$t8,$t0,0x1
/*  f0fb6f8:	25010001 */ 	addiu	$at,$t0,0x1
/*  f0fb6fc:	0001c043 */ 	sra	$t8,$at,0x1
.PF0f0fb700:
/*  f0fb700:	03004025 */ 	move	$t0,$t8
.PF0f0fb704:
/*  f0fb704:	05610003 */ 	bgez	$t3,.PF0f0fb714
/*  f0fb708:	a3ac010d */ 	sb	$t4,0x10d($sp)
/*  f0fb70c:	10000001 */ 	b	.PF0f0fb714
/*  f0fb710:	000b1023 */ 	negu	$v0,$t3
.PF0f0fb714:
/*  f0fb714:	05410003 */ 	bgez	$t2,.PF0f0fb724
/*  f0fb718:	28410014 */ 	slti	$at,$v0,0x14
/*  f0fb71c:	10000002 */ 	b	.PF0f0fb728
/*  f0fb720:	000a1823 */ 	negu	$v1,$t2
.PF0f0fb724:
/*  f0fb724:	01401825 */ 	move	$v1,$t2
.PF0f0fb728:
/*  f0fb728:	14200067 */ 	bnez	$at,.PF0f0fb8c8
/*  f0fb72c:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fb730:	14200003 */ 	bnez	$at,.PF0f0fb740
/*  f0fb734:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb738:	52400064 */ 	beqzl	$s2,.PF0f0fb8cc
/*  f0fb73c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb740:
/*  f0fb740:	05610006 */ 	bgez	$t3,.PF0f0fb75c
/*  f0fb744:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fb748:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb74c:	846e0834 */ 	lh	$t6,0x834($v1)
/*  f0fb750:	05c30003 */ 	bgezl	$t6,.PF0f0fb760
/*  f0fb754:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb758:	a4600834 */ 	sh	$zero,0x834($v1)
.PF0f0fb75c:
/*  f0fb75c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb760:
/*  f0fb760:	19600005 */ 	blez	$t3,.PF0f0fb778
/*  f0fb764:	84650834 */ 	lh	$a1,0x834($v1)
/*  f0fb768:	18a00003 */ 	blez	$a1,.PF0f0fb778
/*  f0fb76c:	00000000 */ 	nop
/*  f0fb770:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb774:	84650834 */ 	lh	$a1,0x834($v1)
.PF0f0fb778:
/*  f0fb778:	14a00003 */ 	bnez	$a1,.PF0f0fb788
/*  f0fb77c:	00000000 */ 	nop
/*  f0fb780:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fb784:	84650834 */ 	lh	$a1,0x834($v1)
.PF0f0fb788:
/*  f0fb788:	14200002 */ 	bnez	$at,.PF0f0fb794
/*  f0fb78c:	8cc6a510 */ 	lw	$a2,-0x5af0($a2)
/*  f0fb790:	24020046 */ 	li	$v0,0x46
.PF0f0fb794:
/*  f0fb794:	2442ffec */ 	addiu	$v0,$v0,-20
/*  f0fb798:	02820019 */ 	multu	$s4,$v0
/*  f0fb79c:	24010032 */ 	li	$at,0x32
/*  f0fb7a0:	00007812 */ 	mflo	$t7
/*  f0fb7a4:	00000000 */ 	nop
/*  f0fb7a8:	00000000 */ 	nop
/*  f0fb7ac:	01e1001a */ 	div	$zero,$t7,$at
/*  f0fb7b0:	0000c812 */ 	mflo	$t9
/*  f0fb7b4:	02792023 */ 	subu	$a0,$s3,$t9
/*  f0fb7b8:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fb7bc:	10200003 */ 	beqz	$at,.PF0f0fb7cc
/*  f0fb7c0:	00801825 */ 	move	$v1,$a0
/*  f0fb7c4:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fb7c8:	10200005 */ 	beqz	$at,.PF0f0fb7e0
.PF0f0fb7cc:
/*  f0fb7cc:	24010003 */ 	li	$at,0x3
/*  f0fb7d0:	0081001a */ 	div	$zero,$a0,$at
/*  f0fb7d4:	00001812 */ 	mflo	$v1
/*  f0fb7d8:	00000000 */ 	nop
/*  f0fb7dc:	00000000 */ 	nop
.PF0f0fb7e0:
/*  f0fb7e0:	18600036 */ 	blez	$v1,.PF0f0fb8bc
/*  f0fb7e4:	28610004 */ 	slti	$at,$v1,0x4
/*  f0fb7e8:	1420000b */ 	bnez	$at,.PF0f0fb818
/*  f0fb7ec:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fb7f0:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fb7f4:	0303c023 */ 	subu	$t8,$t8,$v1
/*  f0fb7f8:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb7fc:	0303c021 */ 	addu	$t8,$t8,$v1
/*  f0fb800:	0018c040 */ 	sll	$t8,$t8,0x1
/*  f0fb804:	2401003c */ 	li	$at,0x3c
/*  f0fb808:	0301001a */ 	div	$zero,$t8,$at
/*  f0fb80c:	00007012 */ 	mflo	$t6
/*  f0fb810:	01c01825 */ 	move	$v1,$t6
/*  f0fb814:	00000000 */ 	nop
.PF0f0fb818:
/*  f0fb818:	8de20830 */ 	lw	$v0,0x830($t7)
/*  f0fb81c:	240c0001 */ 	li	$t4,0x1
/*  f0fb820:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fb824:	0046c821 */ 	addu	$t9,$v0,$a2
/*  f0fb828:	00003812 */ 	mflo	$a3
/*  f0fb82c:	14600002 */ 	bnez	$v1,.PF0f0fb838
/*  f0fb830:	00000000 */ 	nop
/*  f0fb834:	0007000d */ 	break	0x7
.PF0f0fb838:
/*  f0fb838:	2401ffff */ 	li	$at,-1
/*  f0fb83c:	14610004 */ 	bne	$v1,$at,.PF0f0fb850
/*  f0fb840:	3c018000 */ 	lui	$at,0x8000
/*  f0fb844:	14410002 */ 	bne	$v0,$at,.PF0f0fb850
/*  f0fb848:	00000000 */ 	nop
/*  f0fb84c:	0006000d */ 	break	0x6
.PF0f0fb850:
/*  f0fb850:	0323001a */ 	div	$zero,$t9,$v1
/*  f0fb854:	00004012 */ 	mflo	$t0
/*  f0fb858:	14600002 */ 	bnez	$v1,.PF0f0fb864
/*  f0fb85c:	00000000 */ 	nop
/*  f0fb860:	0007000d */ 	break	0x7
.PF0f0fb864:
/*  f0fb864:	2401ffff */ 	li	$at,-1
/*  f0fb868:	14610004 */ 	bne	$v1,$at,.PF0f0fb87c
/*  f0fb86c:	3c018000 */ 	lui	$at,0x8000
/*  f0fb870:	17210002 */ 	bne	$t9,$at,.PF0f0fb87c
/*  f0fb874:	00000000 */ 	nop
/*  f0fb878:	0006000d */ 	break	0x6
.PF0f0fb87c:
/*  f0fb87c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb880:	19600003 */ 	blez	$t3,.PF0f0fb890
/*  f0fb884:	00000000 */ 	nop
/*  f0fb888:	10000001 */ 	b	.PF0f0fb890
/*  f0fb88c:	240cffff */ 	li	$t4,-1
.PF0f0fb890:
/*  f0fb890:	10e80002 */ 	beq	$a3,$t0,.PF0f0fb89c
/*  f0fb894:	00acc021 */ 	addu	$t8,$a1,$t4
/*  f0fb898:	240d0001 */ 	li	$t5,0x1
.PF0f0fb89c:
/*  f0fb89c:	14a00002 */ 	bnez	$a1,.PF0f0fb8a8
/*  f0fb8a0:	00000000 */ 	nop
/*  f0fb8a4:	240d0001 */ 	li	$t5,0x1
.PF0f0fb8a8:
/*  f0fb8a8:	51a00005 */ 	beqzl	$t5,.PF0f0fb8c0
/*  f0fb8ac:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb8b0:	a4780834 */ 	sh	$t8,0x834($v1)
/*  f0fb8b4:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb8b8:	8cc6a510 */ 	lw	$a2,-0x5af0($a2)
.PF0f0fb8bc:
/*  f0fb8bc:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb8c0:
/*  f0fb8c0:	10000006 */ 	b	.PF0f0fb8dc
/*  f0fb8c4:	8c620830 */ 	lw	$v0,0x830($v1)
.PF0f0fb8c8:
/*  f0fb8c8:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PF0f0fb8cc:
/*  f0fb8cc:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb8d0:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb8d4:	8cc6a510 */ 	lw	$a2,-0x5af0($a2)
/*  f0fb8d8:	8c620830 */ 	lw	$v0,0x830($v1)
.PF0f0fb8dc:
/*  f0fb8dc:	10e80002 */ 	beq	$a3,$t0,.PF0f0fb8e8
/*  f0fb8e0:	27b50104 */ 	addiu	$s5,$sp,0x104
/*  f0fb8e4:	240d0001 */ 	li	$t5,0x1
.PF0f0fb8e8:
/*  f0fb8e8:	55a00003 */ 	bnezl	$t5,.PF0f0fb8f8
/*  f0fb8ec:	8c6e0824 */ 	lw	$t6,0x824($v1)
/*  f0fb8f0:	00006025 */ 	move	$t4,$zero
/*  f0fb8f4:	8c6e0824 */ 	lw	$t6,0x824($v1)
.PF0f0fb8f8:
/*  f0fb8f8:	8fa500f8 */ 	lw	$a1,0xf8($sp)
/*  f0fb8fc:	3c19800a */ 	lui	$t9,0x800a
/*  f0fb900:	01c67821 */ 	addu	$t7,$t6,$a2
/*  f0fb904:	ac6f0824 */ 	sw	$t7,0x824($v1)
/*  f0fb908:	8f39a510 */ 	lw	$t9,-0x5af0($t9)
/*  f0fb90c:	240e0001 */ 	li	$t6,0x1
/*  f0fb910:	0059c021 */ 	addu	$t8,$v0,$t9
/*  f0fb914:	ac780830 */ 	sw	$t8,0x830($v1)
/*  f0fb918:	a3a90104 */ 	sb	$t1,0x104($sp)
/*  f0fb91c:	a3ac0105 */ 	sb	$t4,0x105($sp)
/*  f0fb920:	a3aa0108 */ 	sb	$t2,0x108($sp)
/*  f0fb924:	a3ab0109 */ 	sb	$t3,0x109($sp)
/*  f0fb928:	10a00003 */ 	beqz	$a1,.PF0f0fb938
/*  f0fb92c:	a3a00118 */ 	sb	$zero,0x118($sp)
/*  f0fb930:	10000002 */ 	b	.PF0f0fb93c
/*  f0fb934:	a3ae010e */ 	sb	$t6,0x10e($sp)
.PF0f0fb938:
/*  f0fb938:	a3a0010e */ 	sb	$zero,0x10e($sp)
.PF0f0fb93c:
/*  f0fb93c:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fb940:	007e0019 */ 	multu	$v1,$s8
/*  f0fb944:	00007812 */ 	mflo	$t7
/*  f0fb948:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fb94c:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fb950:	50800016 */ 	beqzl	$a0,.PF0f0fb9ac
/*  f0fb954:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb958:	50a00014 */ 	beqzl	$a1,.PF0f0fb9ac
/*  f0fb95c:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb960:	8c990000 */ 	lw	$t9,0x0($a0)
/*  f0fb964:	240f0001 */ 	li	$t7,0x1
/*  f0fb968:	8f380010 */ 	lw	$t8,0x10($t9)
/*  f0fb96c:	24190001 */ 	li	$t9,0x1
/*  f0fb970:	330e0004 */ 	andi	$t6,$t8,0x4
/*  f0fb974:	51c00004 */ 	beqzl	$t6,.PF0f0fb988
/*  f0fb978:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb97c:	a3af0106 */ 	sb	$t7,0x106($sp)
/*  f0fb980:	afb9011c */ 	sw	$t9,0x11c($sp)
/*  f0fb984:	8c820008 */ 	lw	$v0,0x8($a0)
.PF0f0fb988:
/*  f0fb988:	50400008 */ 	beqzl	$v0,.PF0f0fb9ac
/*  f0fb98c:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb990:	90580000 */ 	lbu	$t8,0x0($v0)
/*  f0fb994:	24010002 */ 	li	$at,0x2
/*  f0fb998:	240e0001 */ 	li	$t6,0x1
/*  f0fb99c:	57010003 */ 	bnel	$t8,$at,.PF0f0fb9ac
/*  f0fb9a0:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb9a4:	a3ae0106 */ 	sb	$t6,0x106($sp)
/*  f0fb9a8:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.PF0f0fb9ac:
/*  f0fb9ac:	00009825 */ 	move	$s3,$zero
/*  f0fb9b0:	0000a025 */ 	move	$s4,$zero
/*  f0fb9b4:	1840003a */ 	blez	$v0,.PF0f0fbaa0
/*  f0fb9b8:	00000000 */ 	nop
/*  f0fb9bc:	afa00044 */ 	sw	$zero,0x44($sp)
.PF0f0fb9c0:
/*  f0fb9c0:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fb9c4:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb9c8:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fb9cc:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fb9d0:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fb9d4:	8fb80044 */ 	lw	$t8,0x44($sp)
/*  f0fb9d8:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb9dc:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fb9e0:	02cfc821 */ 	addu	$t9,$s6,$t7
/*  f0fb9e4:	03389021 */ 	addu	$s2,$t9,$t8
/*  f0fb9e8:	824e0478 */ 	lb	$t6,0x478($s2)
/*  f0fb9ec:	26520464 */ 	addiu	$s2,$s2,0x464
/*  f0fb9f0:	02408825 */ 	move	$s1,$s2
/*  f0fb9f4:	19c00024 */ 	blez	$t6,.PF0f0fba88
/*  f0fb9f8:	00008025 */ 	move	$s0,$zero
/*  f0fb9fc:	8ef90000 */ 	lw	$t9,0x0($s7)
.PF0f0fba00:
/*  f0fba00:	268f0001 */ 	addiu	$t7,$s4,0x1
/*  f0fba04:	00003025 */ 	move	$a2,$zero
/*  f0fba08:	033e0019 */ 	multu	$t9,$s8
/*  f0fba0c:	02a02825 */ 	move	$a1,$s5
/*  f0fba10:	0000c012 */ 	mflo	$t8
/*  f0fba14:	02d87021 */ 	addu	$t6,$s6,$t8
/*  f0fba18:	85d904f4 */ 	lh	$t9,0x4f4($t6)
/*  f0fba1c:	15f90008 */ 	bne	$t7,$t9,.PF0f0fba40
/*  f0fba20:	00000000 */ 	nop
/*  f0fba24:	82580015 */ 	lb	$t8,0x15($s2)
/*  f0fba28:	16180005 */ 	bne	$s0,$t8,.PF0f0fba40
/*  f0fba2c:	00000000 */ 	nop
/*  f0fba30:	16600003 */ 	bnez	$s3,.PF0f0fba40
/*  f0fba34:	00000000 */ 	nop
/*  f0fba38:	24060001 */ 	li	$a2,0x1
/*  f0fba3c:	24130001 */ 	li	$s3,0x1
.PF0f0fba40:
/*  f0fba40:	0fc3e6a8 */ 	jal	dialogTick
/*  f0fba44:	8e240000 */ 	lw	$a0,0x0($s1)
/*  f0fba48:	824e0014 */ 	lb	$t6,0x14($s2)
/*  f0fba4c:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0fba50:	26310004 */ 	addiu	$s1,$s1,0x4
/*  f0fba54:	020e082a */ 	slt	$at,$s0,$t6
/*  f0fba58:	5420ffe9 */ 	bnezl	$at,.PF0f0fba00
/*  f0fba5c:	8ef90000 */ 	lw	$t9,0x0($s7)
/*  f0fba60:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fba64:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fba68:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fba6c:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fba70:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fba74:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fba78:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fba7c:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fba80:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fba84:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.PF0f0fba88:
/*  f0fba88:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0fba8c:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fba90:	0282082a */ 	slt	$at,$s4,$v0
/*  f0fba94:	27380018 */ 	addiu	$t8,$t9,0x18
/*  f0fba98:	1420ffc9 */ 	bnez	$at,.PF0f0fb9c0
/*  f0fba9c:	afb80044 */ 	sw	$t8,0x44($sp)
.PF0f0fbaa0:
/*  f0fbaa0:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fbaa4:	8dce1f64 */ 	lw	$t6,0x1f64($t6)
/*  f0fbaa8:	25cfffff */ 	addiu	$t7,$t6,-1
/*  f0fbaac:	2de1000d */ 	sltiu	$at,$t7,0xd
/*  f0fbab0:	1020004d */ 	beqz	$at,.PF0f0fbbe8
/*  f0fbab4:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fbab8:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fbabc:	002f0821 */ 	addu	$at,$at,$t7
/*  f0fbac0:	8c2f3d90 */ 	lw	$t7,0x3d90($at)
/*  f0fbac4:	01e00008 */ 	jr	$t7
/*  f0fbac8:	00000000 */ 	nop
/*  f0fbacc:	83b9010e */ 	lb	$t9,0x10e($sp)
/*  f0fbad0:	8fb8011c */ 	lw	$t8,0x11c($sp)
/*  f0fbad4:	53200045 */ 	beqzl	$t9,.PF0f0fbbec
/*  f0fbad8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbadc:	57000043 */ 	bnezl	$t8,.PF0f0fbbec
/*  f0fbae0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbae4:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fbae8:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fbaec:	5080003f */ 	beqzl	$a0,.PF0f0fbbec
/*  f0fbaf0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbaf4:	8dcf000c */ 	lw	$t7,0xc($t6)
/*  f0fbaf8:	3c19800a */ 	lui	$t9,0x800a
/*  f0fbafc:	55e0003b */ 	bnezl	$t7,.PF0f0fbbec
/*  f0fbb00:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbb04:	8f39a9a0 */ 	lw	$t9,-0x5660($t9)
/*  f0fbb08:	8c820000 */ 	lw	$v0,0x0($a0)
/*  f0fbb0c:	24010002 */ 	li	$at,0x2
/*  f0fbb10:	13210008 */ 	beq	$t9,$at,.PF0f0fbb34
/*  f0fbb14:	3c048008 */ 	lui	$a0,0x8008
/*  f0fbb18:	24845d08 */ 	addiu	$a0,$a0,0x5d08
/*  f0fbb1c:	10820005 */ 	beq	$a0,$v0,.PF0f0fbb34
/*  f0fbb20:	00000000 */ 	nop
/*  f0fbb24:	0fc3cda8 */ 	jal	menuPushDialog
/*  f0fbb28:	00000000 */ 	nop
/*  f0fbb2c:	1000002f */ 	b	.PF0f0fbbec
/*  f0fbb30:	8fbf003c */ 	lw	$ra,0x3c($sp)
.PF0f0fbb34:
/*  f0fbb34:	3c188008 */ 	lui	$t8,0x8008
/*  f0fbb38:	27186bd0 */ 	addiu	$t8,$t8,0x6bd0
/*  f0fbb3c:	5702002b */ 	bnel	$t8,$v0,.PF0f0fbbec
/*  f0fbb40:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbb44:	0fc6011a */ 	jal	func0f17f428
/*  f0fbb48:	00000000 */ 	nop
/*  f0fbb4c:	10000027 */ 	b	.PF0f0fbbec
/*  f0fbb50:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbb54:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0fbb58:	8dce0a44 */ 	lw	$t6,0xa44($t6)
/*  f0fbb5c:	51c00010 */ 	beqzl	$t6,.PF0f0fbba0
/*  f0fbb60:	240e000a */ 	li	$t6,0xa
/*  f0fbb64:	0fc3e219 */ 	jal	func0f0f8120
/*  f0fbb68:	00000000 */ 	nop
/*  f0fbb6c:	3c0f8007 */ 	lui	$t7,0x8007
/*  f0fbb70:	8def1728 */ 	lw	$t7,0x1728($t7)
/*  f0fbb74:	3c18800a */ 	lui	$t8,0x800a
/*  f0fbb78:	2718e5a0 */ 	addiu	$t8,$t8,-6752
/*  f0fbb7c:	000fc8c0 */ 	sll	$t9,$t7,0x3
/*  f0fbb80:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fbb84:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fbb88:	032fc821 */ 	addu	$t9,$t9,$t7
/*  f0fbb8c:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fbb90:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fbb94:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fbb98:	03383821 */ 	addu	$a3,$t9,$t8
/*  f0fbb9c:	240e000a */ 	li	$t6,0xa
.PF0f0fbba0:
/*  f0fbba0:	a0ee083c */ 	sb	$t6,0x83c($a3)
/*  f0fbba4:	83af010e */ 	lb	$t7,0x10e($sp)
/*  f0fbba8:	8fb9011c */ 	lw	$t9,0x11c($sp)
/*  f0fbbac:	51e0000f */ 	beqzl	$t7,.PF0f0fbbec
/*  f0fbbb0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbbb4:	5720000d */ 	bnezl	$t9,.PF0f0fbbec
/*  f0fbbb8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbbbc:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fbbc0:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fbbc4:	53000009 */ 	beqzl	$t8,.PF0f0fbbec
/*  f0fbbc8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbbcc:	8dcf0000 */ 	lw	$t7,0x0($t6)
/*  f0fbbd0:	8df90010 */ 	lw	$t9,0x10($t7)
/*  f0fbbd4:	33380020 */ 	andi	$t8,$t9,0x20
/*  f0fbbd8:	57000004 */ 	bnezl	$t8,.PF0f0fbbec
/*  f0fbbdc:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fbbe0:	0fc3e219 */ 	jal	func0f0f8120
/*  f0fbbe4:	00000000 */ 	nop
.PF0f0fbbe8:
/*  f0fbbe8:	8fbf003c */ 	lw	$ra,0x3c($sp)
.PF0f0fbbec:
/*  f0fbbec:	8fb00018 */ 	lw	$s0,0x18($sp)
/*  f0fbbf0:	8fb1001c */ 	lw	$s1,0x1c($sp)
/*  f0fbbf4:	8fb20020 */ 	lw	$s2,0x20($sp)
/*  f0fbbf8:	8fb30024 */ 	lw	$s3,0x24($sp)
/*  f0fbbfc:	8fb40028 */ 	lw	$s4,0x28($sp)
/*  f0fbc00:	8fb5002c */ 	lw	$s5,0x2c($sp)
/*  f0fbc04:	8fb60030 */ 	lw	$s6,0x30($sp)
/*  f0fbc08:	8fb70034 */ 	lw	$s7,0x34($sp)
/*  f0fbc0c:	8fbe0038 */ 	lw	$s8,0x38($sp)
/*  f0fbc10:	03e00008 */ 	jr	$ra
/*  f0fbc14:	27bd0130 */ 	addiu	$sp,$sp,0x130
);
#elif VERSION >= VERSION_PAL_BETA
GLOBAL_ASM(
glabel menuProcessInput
.late_rodata
glabel var7f1b2a98
.word menuProcessInput+0xd64
glabel var7f1b2a9c
.word menuProcessInput+0xd64
glabel var7f1b2aa0
.word menuProcessInput+0xc8c
glabel var7f1b2aa4
.word menuProcessInput+0xd14
glabel var7f1b2aa8
.word menuProcessInput+0xd64
glabel var7f1b2aac
.word menuProcessInput+0xda8
glabel var7f1b2ab0
.word menuProcessInput+0xda8
glabel var7f1b2ab4
.word menuProcessInput+0xda8
glabel var7f1b2ab8
.word menuProcessInput+0xda8
glabel var7f1b2abc
.word menuProcessInput+0xda8
glabel var7f1b2ac0
.word menuProcessInput+0xc8c
glabel var7f1b2ac4
.word menuProcessInput+0xda8
glabel var7f1b2ac8
.word menuProcessInput+0xd64
.text
/*  f0fac58:	27bdfed0 */ 	addiu	$sp,$sp,-304
/*  f0fac5c:	afb70034 */ 	sw	$s7,0x34($sp)
/*  f0fac60:	3c178007 */ 	lui	$s7,0x8007
/*  f0fac64:	26f73118 */ 	addiu	$s7,$s7,0x3118
/*  f0fac68:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fac6c:	afbe0038 */ 	sw	$s8,0x38($sp)
/*  f0fac70:	241e0e70 */ 	li	$s8,0xe70
/*  f0fac74:	01de0019 */ 	multu	$t6,$s8
/*  f0fac78:	afb60030 */ 	sw	$s6,0x30($sp)
/*  f0fac7c:	3c16800a */ 	lui	$s6,0x800a
/*  f0fac80:	26d62530 */ 	addiu	$s6,$s6,0x2530
/*  f0fac84:	3c19800a */ 	lui	$t9,0x800a
/*  f0fac88:	8f395ef4 */ 	lw	$t9,0x5ef4($t9)
/*  f0fac8c:	afb5002c */ 	sw	$s5,0x2c($sp)
/*  f0fac90:	24010004 */ 	li	$at,0x4
/*  f0fac94:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0fac98:	afb40028 */ 	sw	$s4,0x28($sp)
/*  f0fac9c:	00007812 */ 	mflo	$t7
/*  f0faca0:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0faca4:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0faca8:	afb30024 */ 	sw	$s3,0x24($sp)
/*  f0facac:	afb20020 */ 	sw	$s2,0x20($sp)
/*  f0facb0:	afb1001c */ 	sw	$s1,0x1c($sp)
/*  f0facb4:	afb00018 */ 	sw	$s0,0x18($sp)
/*  f0facb8:	00006025 */ 	move	$t4,$zero
/*  f0facbc:	00004825 */ 	move	$t1,$zero
/*  f0facc0:	00004025 */ 	move	$t0,$zero
/*  f0facc4:	0000a825 */ 	move	$s5,$zero
/*  f0facc8:	afa0011c */ 	sw	$zero,0x11c($sp)
/*  f0faccc:	afa700fc */ 	sw	$a3,0xfc($sp)
/*  f0facd0:	17210005 */ 	bne	$t9,$at,.PB0f0face8
/*  f0facd4:	afb80100 */ 	sw	$t8,0x100($sp)
/*  f0facd8:	3c0e800a */ 	lui	$t6,0x800a
/*  f0facdc:	8dcee75c */ 	lw	$t6,-0x18a4($t6)
/*  f0face0:	3c01800a */ 	lui	$at,0x800a
/*  f0face4:	ac2e66e8 */ 	sw	$t6,0x66e8($at)
.PB0f0face8:
/*  f0face8:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0facec:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0facf0:	0fc3c661 */ 	jal	func0f0f1418
/*  f0facf4:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0facf8:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0facfc:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fad00:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fad04:	01fe0019 */ 	multu	$t7,$s8
/*  f0fad08:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fad0c:	a3a00106 */ 	sb	$zero,0x106($sp)
/*  f0fad10:	a3a00107 */ 	sb	$zero,0x107($sp)
/*  f0fad14:	a3a0010a */ 	sb	$zero,0x10a($sp)
/*  f0fad18:	a3a0010b */ 	sb	$zero,0x10b($sp)
/*  f0fad1c:	00005025 */ 	move	$t2,$zero
/*  f0fad20:	00005825 */ 	move	$t3,$zero
/*  f0fad24:	3c0f800a */ 	lui	$t7,0x800a
/*  f0fad28:	0000c012 */ 	mflo	$t8
/*  f0fad2c:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fad30:	8f2e04f8 */ 	lw	$t6,0x4f8($t9)
/*  f0fad34:	51c00333 */ 	beqzl	$t6,.PB0f0fba04
/*  f0fad38:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fad3c:	8def5ef4 */ 	lw	$t7,0x5ef4($t7)
/*  f0fad40:	24010007 */ 	li	$at,0x7
/*  f0fad44:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*  f0fad48:	15e1000b */ 	bne	$t7,$at,.PB0f0fad78
/*  f0fad4c:	afa000ec */ 	sw	$zero,0xec($sp)
/*  f0fad50:	24180001 */ 	li	$t8,0x1
/*  f0fad54:	24190002 */ 	li	$t9,0x2
/*  f0fad58:	240e0003 */ 	li	$t6,0x3
/*  f0fad5c:	240f0004 */ 	li	$t7,0x4
/*  f0fad60:	afa000d8 */ 	sw	$zero,0xd8($sp)
/*  f0fad64:	afb800dc */ 	sw	$t8,0xdc($sp)
/*  f0fad68:	afb900e0 */ 	sw	$t9,0xe0($sp)
/*  f0fad6c:	afae00e4 */ 	sw	$t6,0xe4($sp)
/*  f0fad70:	1000001b */ 	b	.PB0f0fade0
/*  f0fad74:	afaf00ec */ 	sw	$t7,0xec($sp)
.PB0f0fad78:
/*  f0fad78:	27a400d7 */ 	addiu	$a0,$sp,0xd7
/*  f0fad7c:	27a500d6 */ 	addiu	$a1,$sp,0xd6
/*  f0fad80:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fad84:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fad88:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fad8c:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fad90:	0fc3de81 */ 	jal	menuGetContPads
/*  f0fad94:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fad98:	83a200d7 */ 	lb	$v0,0xd7($sp)
/*  f0fad9c:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fada0:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fada4:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fada8:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fadac:	04400004 */ 	bltz	$v0,.PB0f0fadc0
/*  f0fadb0:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fadb4:	24030001 */ 	li	$v1,0x1
/*  f0fadb8:	afa200d8 */ 	sw	$v0,0xd8($sp)
/*  f0fadbc:	afa300ec */ 	sw	$v1,0xec($sp)
.PB0f0fadc0:
/*  f0fadc0:	83a200d6 */ 	lb	$v0,0xd6($sp)
/*  f0fadc4:	8fa300ec */ 	lw	$v1,0xec($sp)
/*  f0fadc8:	04400005 */ 	bltz	$v0,.PB0f0fade0
/*  f0fadcc:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fadd0:	03b8c821 */ 	addu	$t9,$sp,$t8
/*  f0fadd4:	af2200d8 */ 	sw	$v0,0xd8($t9)
/*  f0fadd8:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0faddc:	afa300ec */ 	sw	$v1,0xec($sp)
.PB0f0fade0:
/*  f0fade0:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fade4:	01de0019 */ 	multu	$t6,$s8
/*  f0fade8:	00007812 */ 	mflo	$t7
/*  f0fadec:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fadf0:	90e20e41 */ 	lbu	$v0,0xe41($a3)
/*  f0fadf4:	1840001a */ 	blez	$v0,.PB0f0fae60
/*  f0fadf8:	2458ffff */ 	addiu	$t8,$v0,-1
/*  f0fadfc:	331900ff */ 	andi	$t9,$t8,0xff
/*  f0fae00:	17200014 */ 	bnez	$t9,.PB0f0fae54
/*  f0fae04:	a0f80e41 */ 	sb	$t8,0xe41($a3)
/*  f0fae08:	8cee0e48 */ 	lw	$t6,0xe48($a3)
/*  f0fae0c:	8cef0e4c */ 	lw	$t7,0xe4c($a3)
/*  f0fae10:	27a400cc */ 	addiu	$a0,$sp,0xcc
/*  f0fae14:	2405ffff */ 	li	$a1,-1
/*  f0fae18:	00003025 */ 	move	$a2,$zero
/*  f0fae1c:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fae20:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fae24:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fae28:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fae2c:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fae30:	afae00cc */ 	sw	$t6,0xcc($sp)
/*  f0fae34:	0fc426eb */ 	jal	filemgrSaveOrLoad
/*  f0fae38:	a7af00d0 */ 	sh	$t7,0xd0($sp)
/*  f0fae3c:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fae40:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fae44:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fae48:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fae4c:	10000004 */ 	b	.PB0f0fae60
/*  f0fae50:	8fac012c */ 	lw	$t4,0x12c($sp)
.PB0f0fae54:
/*  f0fae54:	90f90e40 */ 	lbu	$t9,0xe40($a3)
/*  f0fae58:	372e0080 */ 	ori	$t6,$t9,0x80
/*  f0fae5c:	a0ee0e40 */ 	sb	$t6,0xe40($a3)
.PB0f0fae60:
/*  f0fae60:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0fae64:	0000a025 */ 	move	$s4,$zero
/*  f0fae68:	19e00083 */ 	blez	$t7,.PB0f0fb078
/*  f0fae6c:	27b300d8 */ 	addiu	$s3,$sp,0xd8
.PB0f0fae70:
/*  f0fae70:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fae74:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fae78:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fae7c:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fae80:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fae84:	0c005616 */ 	jal	joyGetStickX
/*  f0fae88:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fae8c:	00028600 */ 	sll	$s0,$v0,0x18
/*  f0fae90:	0010c603 */ 	sra	$t8,$s0,0x18
/*  f0fae94:	03008025 */ 	move	$s0,$t8
/*  f0fae98:	0c005640 */ 	jal	joyGetStickY
/*  f0fae9c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0faea0:	00028e00 */ 	sll	$s1,$v0,0x18
/*  f0faea4:	0011ce03 */ 	sra	$t9,$s1,0x18
/*  f0faea8:	03208825 */ 	move	$s1,$t9
/*  f0faeac:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0faeb0:	0c00566a */ 	jal	joyGetButtons
/*  f0faeb4:	3405ffff */ 	li	$a1,0xffff
/*  f0faeb8:	3052ffff */ 	andi	$s2,$v0,0xffff
/*  f0faebc:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0faec0:	0c00569a */ 	jal	joyGetButtonsPressedThisFrame
/*  f0faec4:	3405ffff */ 	li	$a1,0xffff
/*  f0faec8:	304e8000 */ 	andi	$t6,$v0,0x8000
/*  f0faecc:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0faed0:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0faed4:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0faed8:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0faedc:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0faee0:	11c00003 */ 	beqz	$t6,.PB0f0faef0
/*  f0faee4:	00403025 */ 	move	$a2,$v0
/*  f0faee8:	240f0001 */ 	li	$t7,0x1
/*  f0faeec:	a3af0106 */ 	sb	$t7,0x106($sp)
.PB0f0faef0:
/*  f0faef0:	30d84000 */ 	andi	$t8,$a2,0x4000
/*  f0faef4:	13000003 */ 	beqz	$t8,.PB0f0faf04
/*  f0faef8:	30ce2000 */ 	andi	$t6,$a2,0x2000
/*  f0faefc:	24190001 */ 	li	$t9,0x1
/*  f0faf00:	a3b90107 */ 	sb	$t9,0x107($sp)
.PB0f0faf04:
/*  f0faf04:	11c00003 */ 	beqz	$t6,.PB0f0faf14
/*  f0faf08:	30d81000 */ 	andi	$t8,$a2,0x1000
/*  f0faf0c:	240f0001 */ 	li	$t7,0x1
/*  f0faf10:	a3af0106 */ 	sb	$t7,0x106($sp)
.PB0f0faf14:
/*  f0faf14:	13000003 */ 	beqz	$t8,.PB0f0faf24
/*  f0faf18:	324e0010 */ 	andi	$t6,$s2,0x10
/*  f0faf1c:	24190001 */ 	li	$t9,0x1
/*  f0faf20:	afb900f8 */ 	sw	$t9,0xf8($sp)
.PB0f0faf24:
/*  f0faf24:	11c00003 */ 	beqz	$t6,.PB0f0faf34
/*  f0faf28:	02402825 */ 	move	$a1,$s2
/*  f0faf2c:	240f0001 */ 	li	$t7,0x1
/*  f0faf30:	a3af010a */ 	sb	$t7,0x10a($sp)
.PB0f0faf34:
/*  f0faf34:	30b80020 */ 	andi	$t8,$a1,0x20
/*  f0faf38:	13000003 */ 	beqz	$t8,.PB0f0faf48
/*  f0faf3c:	30ae0008 */ 	andi	$t6,$a1,0x8
/*  f0faf40:	24190001 */ 	li	$t9,0x1
/*  f0faf44:	a3b9010a */ 	sb	$t9,0x10a($sp)
.PB0f0faf48:
/*  f0faf48:	05410003 */ 	bgez	$t2,.PB0f0faf58
/*  f0faf4c:	30cf0008 */ 	andi	$t7,$a2,0x8
/*  f0faf50:	10000002 */ 	b	.PB0f0faf5c
/*  f0faf54:	000a2023 */ 	negu	$a0,$t2
.PB0f0faf58:
/*  f0faf58:	01402025 */ 	move	$a0,$t2
.PB0f0faf5c:
/*  f0faf5c:	06010003 */ 	bgez	$s0,.PB0f0faf6c
/*  f0faf60:	30b80004 */ 	andi	$t8,$a1,0x4
/*  f0faf64:	10000002 */ 	b	.PB0f0faf70
/*  f0faf68:	00101823 */ 	negu	$v1,$s0
.PB0f0faf6c:
/*  f0faf6c:	02001825 */ 	move	$v1,$s0
.PB0f0faf70:
/*  f0faf70:	0083082a */ 	slt	$at,$a0,$v1
/*  f0faf74:	10200002 */ 	beqz	$at,.PB0f0faf80
/*  f0faf78:	30d90004 */ 	andi	$t9,$a2,0x4
/*  f0faf7c:	02005025 */ 	move	$t2,$s0
.PB0f0faf80:
/*  f0faf80:	05610003 */ 	bgez	$t3,.PB0f0faf90
/*  f0faf84:	01602025 */ 	move	$a0,$t3
/*  f0faf88:	10000001 */ 	b	.PB0f0faf90
/*  f0faf8c:	000b2023 */ 	negu	$a0,$t3
.PB0f0faf90:
/*  f0faf90:	06210003 */ 	bgez	$s1,.PB0f0fafa0
/*  f0faf94:	02201825 */ 	move	$v1,$s1
/*  f0faf98:	10000001 */ 	b	.PB0f0fafa0
/*  f0faf9c:	00111823 */ 	negu	$v1,$s1
.PB0f0fafa0:
/*  f0fafa0:	0083082a */ 	slt	$at,$a0,$v1
/*  f0fafa4:	10200002 */ 	beqz	$at,.PB0f0fafb0
/*  f0fafa8:	00000000 */ 	nop
/*  f0fafac:	02205825 */ 	move	$t3,$s1
.PB0f0fafb0:
/*  f0fafb0:	11c00002 */ 	beqz	$t6,.PB0f0fafbc
/*  f0fafb4:	00000000 */ 	nop
/*  f0fafb8:	240cffff */ 	li	$t4,-1
.PB0f0fafbc:
/*  f0fafbc:	11e00002 */ 	beqz	$t7,.PB0f0fafc8
/*  f0fafc0:	30ae0002 */ 	andi	$t6,$a1,0x2
/*  f0fafc4:	2408ffff */ 	li	$t0,-1
.PB0f0fafc8:
/*  f0fafc8:	13000002 */ 	beqz	$t8,.PB0f0fafd4
/*  f0fafcc:	30cf0002 */ 	andi	$t7,$a2,0x2
/*  f0fafd0:	240c0001 */ 	li	$t4,0x1
.PB0f0fafd4:
/*  f0fafd4:	13200002 */ 	beqz	$t9,.PB0f0fafe0
/*  f0fafd8:	30b80001 */ 	andi	$t8,$a1,0x1
/*  f0fafdc:	24080001 */ 	li	$t0,0x1
.PB0f0fafe0:
/*  f0fafe0:	11c00002 */ 	beqz	$t6,.PB0f0fafec
/*  f0fafe4:	30d90001 */ 	andi	$t9,$a2,0x1
/*  f0fafe8:	2409ffff */ 	li	$t1,-1
.PB0f0fafec:
/*  f0fafec:	11e00002 */ 	beqz	$t7,.PB0f0faff8
/*  f0faff0:	30ae0800 */ 	andi	$t6,$a1,0x800
/*  f0faff4:	2415ffff */ 	li	$s5,-1
.PB0f0faff8:
/*  f0faff8:	13000002 */ 	beqz	$t8,.PB0f0fb004
/*  f0faffc:	30cf0800 */ 	andi	$t7,$a2,0x800
/*  f0fb000:	24090001 */ 	li	$t1,0x1
.PB0f0fb004:
/*  f0fb004:	13200002 */ 	beqz	$t9,.PB0f0fb010
/*  f0fb008:	30b80400 */ 	andi	$t8,$a1,0x400
/*  f0fb00c:	24150001 */ 	li	$s5,0x1
.PB0f0fb010:
/*  f0fb010:	11c00002 */ 	beqz	$t6,.PB0f0fb01c
/*  f0fb014:	30d90400 */ 	andi	$t9,$a2,0x400
/*  f0fb018:	240cffff */ 	li	$t4,-1
.PB0f0fb01c:
/*  f0fb01c:	11e00002 */ 	beqz	$t7,.PB0f0fb028
/*  f0fb020:	30ae0200 */ 	andi	$t6,$a1,0x200
/*  f0fb024:	2408ffff */ 	li	$t0,-1
.PB0f0fb028:
/*  f0fb028:	13000002 */ 	beqz	$t8,.PB0f0fb034
/*  f0fb02c:	30cf0200 */ 	andi	$t7,$a2,0x200
/*  f0fb030:	240c0001 */ 	li	$t4,0x1
.PB0f0fb034:
/*  f0fb034:	13200002 */ 	beqz	$t9,.PB0f0fb040
/*  f0fb038:	30b80100 */ 	andi	$t8,$a1,0x100
/*  f0fb03c:	24080001 */ 	li	$t0,0x1
.PB0f0fb040:
/*  f0fb040:	11c00002 */ 	beqz	$t6,.PB0f0fb04c
/*  f0fb044:	30d90100 */ 	andi	$t9,$a2,0x100
/*  f0fb048:	2409ffff */ 	li	$t1,-1
.PB0f0fb04c:
/*  f0fb04c:	11e00002 */ 	beqz	$t7,.PB0f0fb058
/*  f0fb050:	8fae00ec */ 	lw	$t6,0xec($sp)
/*  f0fb054:	2415ffff */ 	li	$s5,-1
.PB0f0fb058:
/*  f0fb058:	13000002 */ 	beqz	$t8,.PB0f0fb064
/*  f0fb05c:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fb060:	24090001 */ 	li	$t1,0x1
.PB0f0fb064:
/*  f0fb064:	13200002 */ 	beqz	$t9,.PB0f0fb070
/*  f0fb068:	00000000 */ 	nop
/*  f0fb06c:	24150001 */ 	li	$s5,0x1
.PB0f0fb070:
/*  f0fb070:	168eff7f */ 	bne	$s4,$t6,.PB0f0fae70
/*  f0fb074:	26730004 */ 	addiu	$s3,$s3,0x4
.PB0f0fb078:
/*  f0fb078:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fb07c:	93ae0106 */ 	lbu	$t6,0x106($sp)
/*  f0fb080:	24100008 */ 	li	$s0,0x8
/*  f0fb084:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0fb088:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb08c:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0fb090:	030fc021 */ 	addu	$t8,$t8,$t7
/*  f0fb094:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb098:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fb09c:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0fb0a0:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fb0a4:	11c00002 */ 	beqz	$t6,.PB0f0fb0b0
/*  f0fb0a8:	8f2404f8 */ 	lw	$a0,0x4f8($t9)
/*  f0fb0ac:	a3a00107 */ 	sb	$zero,0x107($sp)
.PB0f0fb0b0:
/*  f0fb0b0:	11000002 */ 	beqz	$t0,.PB0f0fb0bc
/*  f0fb0b4:	2406001e */ 	li	$a2,0x1e
/*  f0fb0b8:	01006025 */ 	move	$t4,$t0
.PB0f0fb0bc:
/*  f0fb0bc:	12a00002 */ 	beqz	$s5,.PB0f0fb0c8
/*  f0fb0c0:	2413003c */ 	li	$s3,0x3c
/*  f0fb0c4:	02a04825 */ 	move	$t1,$s5
.PB0f0fb0c8:
/*  f0fb0c8:	24110021 */ 	li	$s1,0x21
/*  f0fb0cc:	2414002c */ 	li	$s4,0x2c
/*  f0fb0d0:	10800019 */ 	beqz	$a0,.PB0f0fb138
/*  f0fb0d4:	00009025 */ 	move	$s2,$zero
/*  f0fb0d8:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb0dc:	10400016 */ 	beqz	$v0,.PB0f0fb138
/*  f0fb0e0:	00000000 */ 	nop
/*  f0fb0e4:	90430000 */ 	lbu	$v1,0x0($v0)
/*  f0fb0e8:	24010008 */ 	li	$at,0x8
/*  f0fb0ec:	10610003 */ 	beq	$v1,$at,.PB0f0fb0fc
/*  f0fb0f0:	24010010 */ 	li	$at,0x10
/*  f0fb0f4:	54610009 */ 	bnel	$v1,$at,.PB0f0fb11c
/*  f0fb0f8:	2401000d */ 	li	$at,0xd
.PB0f0fb0fc:
/*  f0fb0fc:	8c8f000c */ 	lw	$t7,0xc($a0)
/*  f0fb100:	51e00006 */ 	beqzl	$t7,.PB0f0fb11c
/*  f0fb104:	2401000d */ 	li	$at,0xd
/*  f0fb108:	24100004 */ 	li	$s0,0x4
/*  f0fb10c:	24060014 */ 	li	$a2,0x14
/*  f0fb110:	2413001e */ 	li	$s3,0x1e
/*  f0fb114:	2411000a */ 	li	$s1,0xa
/*  f0fb118:	2401000d */ 	li	$at,0xd
.PB0f0fb11c:
/*  f0fb11c:	14610006 */ 	bne	$v1,$at,.PB0f0fb138
/*  f0fb120:	00000000 */ 	nop
/*  f0fb124:	24120001 */ 	li	$s2,0x1
/*  f0fb128:	24100004 */ 	li	$s0,0x4
/*  f0fb12c:	24060014 */ 	li	$a2,0x14
/*  f0fb130:	2411000a */ 	li	$s1,0xa
/*  f0fb134:	2414000a */ 	li	$s4,0xa
.PB0f0fb138:
/*  f0fb138:	15200004 */ 	bnez	$t1,.PB0f0fb14c
/*  f0fb13c:	0000f825 */ 	move	$ra,$zero
/*  f0fb140:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb144:	2418ffff */ 	li	$t8,-1
/*  f0fb148:	a478082c */ 	sh	$t8,0x82c($v1)
.PB0f0fb14c:
/*  f0fb14c:	12a00006 */ 	beqz	$s5,.PB0f0fb168
/*  f0fb150:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb154:	a460082c */ 	sh	$zero,0x82c($v1)
/*  f0fb158:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fb15c:	a475082a */ 	sh	$s5,0x82a($v1)
/*  f0fb160:	10000004 */ 	b	.PB0f0fb174
/*  f0fb164:	241f0001 */ 	li	$ra,0x1
.PB0f0fb168:
/*  f0fb168:	51200003 */ 	beqzl	$t1,.PB0f0fb178
/*  f0fb16c:	8c620824 */ 	lw	$v0,0x824($v1)
/*  f0fb170:	8469082a */ 	lh	$t1,0x82a($v1)
.PB0f0fb174:
/*  f0fb174:	8c620824 */ 	lw	$v0,0x824($v1)
.PB0f0fb178:
/*  f0fb178:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fb17c:	28410033 */ 	slti	$at,$v0,0x33
/*  f0fb180:	14200004 */ 	bnez	$at,.PB0f0fb194
/*  f0fb184:	00000000 */ 	nop
/*  f0fb188:	24190001 */ 	li	$t9,0x1
/*  f0fb18c:	a479082c */ 	sh	$t9,0x82c($v1)
/*  f0fb190:	8c620824 */ 	lw	$v0,0x824($v1)
.PB0f0fb194:
/*  f0fb194:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fb198:	8dcee4d0 */ 	lw	$t6,-0x1b30($t6)
/*  f0fb19c:	00003812 */ 	mflo	$a3
/*  f0fb1a0:	8478082c */ 	lh	$t8,0x82c($v1)
/*  f0fb1a4:	004e7821 */ 	addu	$t7,$v0,$t6
/*  f0fb1a8:	01f0001a */ 	div	$zero,$t7,$s0
/*  f0fb1ac:	16000002 */ 	bnez	$s0,.PB0f0fb1b8
/*  f0fb1b0:	00000000 */ 	nop
/*  f0fb1b4:	0007000d */ 	break	0x7
.PB0f0fb1b8:
/*  f0fb1b8:	2401ffff */ 	li	$at,-1
/*  f0fb1bc:	16010004 */ 	bne	$s0,$at,.PB0f0fb1d0
/*  f0fb1c0:	3c018000 */ 	lui	$at,0x8000
/*  f0fb1c4:	14410002 */ 	bne	$v0,$at,.PB0f0fb1d0
/*  f0fb1c8:	00000000 */ 	nop
/*  f0fb1cc:	0006000d */ 	break	0x6
.PB0f0fb1d0:
/*  f0fb1d0:	00006812 */ 	mflo	$t5
/*  f0fb1d4:	16000002 */ 	bnez	$s0,.PB0f0fb1e0
/*  f0fb1d8:	00000000 */ 	nop
/*  f0fb1dc:	0007000d */ 	break	0x7
.PB0f0fb1e0:
/*  f0fb1e0:	2401ffff */ 	li	$at,-1
/*  f0fb1e4:	16010004 */ 	bne	$s0,$at,.PB0f0fb1f8
/*  f0fb1e8:	3c018000 */ 	lui	$at,0x8000
/*  f0fb1ec:	15e10002 */ 	bne	$t7,$at,.PB0f0fb1f8
/*  f0fb1f0:	00000000 */ 	nop
/*  f0fb1f4:	0006000d */ 	break	0x6
.PB0f0fb1f8:
/*  f0fb1f8:	1700000b */ 	bnez	$t8,.PB0f0fb228
/*  f0fb1fc:	00000000 */ 	nop
/*  f0fb200:	04e10003 */ 	bgez	$a3,.PB0f0fb210
/*  f0fb204:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0fb208:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fb20c:	0001c843 */ 	sra	$t9,$at,0x1
.PB0f0fb210:
/*  f0fb210:	03203825 */ 	move	$a3,$t9
/*  f0fb214:	05a10003 */ 	bgez	$t5,.PB0f0fb224
/*  f0fb218:	000d7043 */ 	sra	$t6,$t5,0x1
/*  f0fb21c:	25a10001 */ 	addiu	$at,$t5,0x1
/*  f0fb220:	00017043 */ 	sra	$t6,$at,0x1
.PB0f0fb224:
/*  f0fb224:	01c06825 */ 	move	$t5,$t6
.PB0f0fb228:
/*  f0fb228:	05410003 */ 	bgez	$t2,.PB0f0fb238
/*  f0fb22c:	a3a9010c */ 	sb	$t1,0x10c($sp)
/*  f0fb230:	10000002 */ 	b	.PB0f0fb23c
/*  f0fb234:	000a1023 */ 	negu	$v0,$t2
.PB0f0fb238:
/*  f0fb238:	01401025 */ 	move	$v0,$t2
.PB0f0fb23c:
/*  f0fb23c:	05610003 */ 	bgez	$t3,.PB0f0fb24c
/*  f0fb240:	0046082a */ 	slt	$at,$v0,$a2
/*  f0fb244:	10000002 */ 	b	.PB0f0fb250
/*  f0fb248:	000b1823 */ 	negu	$v1,$t3
.PB0f0fb24c:
/*  f0fb24c:	01601825 */ 	move	$v1,$t3
.PB0f0fb250:
/*  f0fb250:	14200070 */ 	bnez	$at,.PB0f0fb414
/*  f0fb254:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fb258:	14200003 */ 	bnez	$at,.PB0f0fb268
/*  f0fb25c:	00000000 */ 	nop
/*  f0fb260:	5240006d */ 	beqzl	$s2,.PB0f0fb418
/*  f0fb264:	8fae00fc */ 	lw	$t6,0xfc($sp)
.PB0f0fb268:
/*  f0fb268:	05410006 */ 	bgez	$t2,.PB0f0fb284
/*  f0fb26c:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fb270:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb274:	846f0828 */ 	lh	$t7,0x828($v1)
/*  f0fb278:	59e00003 */ 	blezl	$t7,.PB0f0fb288
/*  f0fb27c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb280:	a4600828 */ 	sh	$zero,0x828($v1)
.PB0f0fb284:
/*  f0fb284:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb288:
/*  f0fb288:	19400005 */ 	blez	$t2,.PB0f0fb2a0
/*  f0fb28c:	84650828 */ 	lh	$a1,0x828($v1)
/*  f0fb290:	04a10003 */ 	bgez	$a1,.PB0f0fb2a0
/*  f0fb294:	00000000 */ 	nop
/*  f0fb298:	a4600828 */ 	sh	$zero,0x828($v1)
/*  f0fb29c:	84650828 */ 	lh	$a1,0x828($v1)
.PB0f0fb2a0:
/*  f0fb2a0:	14a00003 */ 	bnez	$a1,.PB0f0fb2b0
/*  f0fb2a4:	00000000 */ 	nop
/*  f0fb2a8:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fb2ac:	84650828 */ 	lh	$a1,0x828($v1)
.PB0f0fb2b0:
/*  f0fb2b0:	54200003 */ 	bnezl	$at,.PB0f0fb2c0
/*  f0fb2b4:	00461023 */ 	subu	$v0,$v0,$a2
/*  f0fb2b8:	24020046 */ 	li	$v0,0x46
/*  f0fb2bc:	00461023 */ 	subu	$v0,$v0,$a2
.PB0f0fb2c0:
/*  f0fb2c0:	02220019 */ 	multu	$s1,$v0
/*  f0fb2c4:	24190046 */ 	li	$t9,0x46
/*  f0fb2c8:	03267023 */ 	subu	$t6,$t9,$a2
/*  f0fb2cc:	0000c012 */ 	mflo	$t8
/*  f0fb2d0:	00000000 */ 	nop
/*  f0fb2d4:	00000000 */ 	nop
/*  f0fb2d8:	030e001a */ 	div	$zero,$t8,$t6
/*  f0fb2dc:	15c00002 */ 	bnez	$t6,.PB0f0fb2e8
/*  f0fb2e0:	00000000 */ 	nop
/*  f0fb2e4:	0007000d */ 	break	0x7
.PB0f0fb2e8:
/*  f0fb2e8:	2401ffff */ 	li	$at,-1
/*  f0fb2ec:	15c10004 */ 	bne	$t6,$at,.PB0f0fb300
/*  f0fb2f0:	3c018000 */ 	lui	$at,0x8000
/*  f0fb2f4:	17010002 */ 	bne	$t8,$at,.PB0f0fb300
/*  f0fb2f8:	00000000 */ 	nop
/*  f0fb2fc:	0006000d */ 	break	0x6
.PB0f0fb300:
/*  f0fb300:	00007812 */ 	mflo	$t7
/*  f0fb304:	026f2023 */ 	subu	$a0,$s3,$t7
/*  f0fb308:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fb30c:	10200004 */ 	beqz	$at,.PB0f0fb320
/*  f0fb310:	00801825 */ 	move	$v1,$a0
/*  f0fb314:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fb318:	10200005 */ 	beqz	$at,.PB0f0fb330
/*  f0fb31c:	00000000 */ 	nop
.PB0f0fb320:
/*  f0fb320:	04810003 */ 	bgez	$a0,.PB0f0fb330
/*  f0fb324:	00041843 */ 	sra	$v1,$a0,0x1
/*  f0fb328:	24810001 */ 	addiu	$at,$a0,0x1
/*  f0fb32c:	00011843 */ 	sra	$v1,$at,0x1
.PB0f0fb330:
/*  f0fb330:	1860003a */ 	blez	$v1,.PB0f0fb41c
/*  f0fb334:	28610004 */ 	slti	$at,$v1,0x4
/*  f0fb338:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb33c:	1420000b */ 	bnez	$at,.PB0f0fb36c
/*  f0fb340:	8cc6e4d0 */ 	lw	$a2,-0x1b30($a2)
/*  f0fb344:	0003c880 */ 	sll	$t9,$v1,0x2
/*  f0fb348:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb34c:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fb350:	0323c821 */ 	addu	$t9,$t9,$v1
/*  f0fb354:	0019c840 */ 	sll	$t9,$t9,0x1
/*  f0fb358:	2401003c */ 	li	$at,0x3c
/*  f0fb35c:	0321001a */ 	div	$zero,$t9,$at
/*  f0fb360:	0000c012 */ 	mflo	$t8
/*  f0fb364:	03001825 */ 	move	$v1,$t8
/*  f0fb368:	00000000 */ 	nop
.PB0f0fb36c:
/*  f0fb36c:	8fae00fc */ 	lw	$t6,0xfc($sp)
/*  f0fb370:	24090001 */ 	li	$t1,0x1
/*  f0fb374:	8dc20824 */ 	lw	$v0,0x824($t6)
/*  f0fb378:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fb37c:	00467821 */ 	addu	$t7,$v0,$a2
/*  f0fb380:	00003812 */ 	mflo	$a3
/*  f0fb384:	14600002 */ 	bnez	$v1,.PB0f0fb390
/*  f0fb388:	00000000 */ 	nop
/*  f0fb38c:	0007000d */ 	break	0x7
.PB0f0fb390:
/*  f0fb390:	2401ffff */ 	li	$at,-1
/*  f0fb394:	14610004 */ 	bne	$v1,$at,.PB0f0fb3a8
/*  f0fb398:	3c018000 */ 	lui	$at,0x8000
/*  f0fb39c:	14410002 */ 	bne	$v0,$at,.PB0f0fb3a8
/*  f0fb3a0:	00000000 */ 	nop
/*  f0fb3a4:	0006000d */ 	break	0x6
.PB0f0fb3a8:
/*  f0fb3a8:	01e3001a */ 	div	$zero,$t7,$v1
/*  f0fb3ac:	00006812 */ 	mflo	$t5
/*  f0fb3b0:	14600002 */ 	bnez	$v1,.PB0f0fb3bc
/*  f0fb3b4:	00000000 */ 	nop
/*  f0fb3b8:	0007000d */ 	break	0x7
.PB0f0fb3bc:
/*  f0fb3bc:	2401ffff */ 	li	$at,-1
/*  f0fb3c0:	14610004 */ 	bne	$v1,$at,.PB0f0fb3d4
/*  f0fb3c4:	3c018000 */ 	lui	$at,0x8000
/*  f0fb3c8:	15e10002 */ 	bne	$t7,$at,.PB0f0fb3d4
/*  f0fb3cc:	00000000 */ 	nop
/*  f0fb3d0:	0006000d */ 	break	0x6
.PB0f0fb3d4:
/*  f0fb3d4:	05410003 */ 	bgez	$t2,.PB0f0fb3e4
/*  f0fb3d8:	00000000 */ 	nop
/*  f0fb3dc:	10000001 */ 	b	.PB0f0fb3e4
/*  f0fb3e0:	2409ffff */ 	li	$t1,-1
.PB0f0fb3e4:
/*  f0fb3e4:	10ed0002 */ 	beq	$a3,$t5,.PB0f0fb3f0
/*  f0fb3e8:	00000000 */ 	nop
/*  f0fb3ec:	241f0001 */ 	li	$ra,0x1
.PB0f0fb3f0:
/*  f0fb3f0:	14a00002 */ 	bnez	$a1,.PB0f0fb3fc
/*  f0fb3f4:	00000000 */ 	nop
/*  f0fb3f8:	241f0001 */ 	li	$ra,0x1
.PB0f0fb3fc:
/*  f0fb3fc:	13e00007 */ 	beqz	$ra,.PB0f0fb41c
/*  f0fb400:	00000000 */ 	nop
/*  f0fb404:	8fb800fc */ 	lw	$t8,0xfc($sp)
/*  f0fb408:	00a9c821 */ 	addu	$t9,$a1,$t1
/*  f0fb40c:	10000003 */ 	b	.PB0f0fb41c
/*  f0fb410:	a7190828 */ 	sh	$t9,0x828($t8)
.PB0f0fb414:
/*  f0fb414:	8fae00fc */ 	lw	$t6,0xfc($sp)
.PB0f0fb418:
/*  f0fb418:	a5c00828 */ 	sh	$zero,0x828($t6)
.PB0f0fb41c:
/*  f0fb41c:	10ed0002 */ 	beq	$a3,$t5,.PB0f0fb428
/*  f0fb420:	3c18800a */ 	lui	$t8,0x800a
/*  f0fb424:	241f0001 */ 	li	$ra,0x1
.PB0f0fb428:
/*  f0fb428:	17e00002 */ 	bnez	$ra,.PB0f0fb434
/*  f0fb42c:	00000000 */ 	nop
/*  f0fb430:	00004825 */ 	move	$t1,$zero
.PB0f0fb434:
/*  f0fb434:	11000007 */ 	beqz	$t0,.PB0f0fb454
/*  f0fb438:	00006825 */ 	move	$t5,$zero
/*  f0fb43c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb440:	240d0001 */ 	li	$t5,0x1
/*  f0fb444:	a4600838 */ 	sh	$zero,0x838($v1)
/*  f0fb448:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fb44c:	10000005 */ 	b	.PB0f0fb464
/*  f0fb450:	a4680836 */ 	sh	$t0,0x836($v1)
.PB0f0fb454:
/*  f0fb454:	51800004 */ 	beqzl	$t4,.PB0f0fb468
/*  f0fb458:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb45c:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fb460:	85ec0836 */ 	lh	$t4,0x836($t7)
.PB0f0fb464:
/*  f0fb464:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb468:
/*  f0fb468:	8c620830 */ 	lw	$v0,0x830($v1)
/*  f0fb46c:	28410033 */ 	slti	$at,$v0,0x33
/*  f0fb470:	14200004 */ 	bnez	$at,.PB0f0fb484
/*  f0fb474:	00000000 */ 	nop
/*  f0fb478:	24190001 */ 	li	$t9,0x1
/*  f0fb47c:	a4790838 */ 	sh	$t9,0x838($v1)
/*  f0fb480:	8c620830 */ 	lw	$v0,0x830($v1)
.PB0f0fb484:
/*  f0fb484:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fb488:	8f18e4d0 */ 	lw	$t8,-0x1b30($t8)
/*  f0fb48c:	00003812 */ 	mflo	$a3
/*  f0fb490:	846f0838 */ 	lh	$t7,0x838($v1)
/*  f0fb494:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0fb498:	01d0001a */ 	div	$zero,$t6,$s0
/*  f0fb49c:	16000002 */ 	bnez	$s0,.PB0f0fb4a8
/*  f0fb4a0:	00000000 */ 	nop
/*  f0fb4a4:	0007000d */ 	break	0x7
.PB0f0fb4a8:
/*  f0fb4a8:	2401ffff */ 	li	$at,-1
/*  f0fb4ac:	16010004 */ 	bne	$s0,$at,.PB0f0fb4c0
/*  f0fb4b0:	3c018000 */ 	lui	$at,0x8000
/*  f0fb4b4:	14410002 */ 	bne	$v0,$at,.PB0f0fb4c0
/*  f0fb4b8:	00000000 */ 	nop
/*  f0fb4bc:	0006000d */ 	break	0x6
.PB0f0fb4c0:
/*  f0fb4c0:	00004012 */ 	mflo	$t0
/*  f0fb4c4:	01601025 */ 	move	$v0,$t3
/*  f0fb4c8:	16000002 */ 	bnez	$s0,.PB0f0fb4d4
/*  f0fb4cc:	00000000 */ 	nop
/*  f0fb4d0:	0007000d */ 	break	0x7
.PB0f0fb4d4:
/*  f0fb4d4:	2401ffff */ 	li	$at,-1
/*  f0fb4d8:	16010004 */ 	bne	$s0,$at,.PB0f0fb4ec
/*  f0fb4dc:	3c018000 */ 	lui	$at,0x8000
/*  f0fb4e0:	15c10002 */ 	bne	$t6,$at,.PB0f0fb4ec
/*  f0fb4e4:	00000000 */ 	nop
/*  f0fb4e8:	0006000d */ 	break	0x6
.PB0f0fb4ec:
/*  f0fb4ec:	15e0000b */ 	bnez	$t7,.PB0f0fb51c
/*  f0fb4f0:	00000000 */ 	nop
/*  f0fb4f4:	04e10003 */ 	bgez	$a3,.PB0f0fb504
/*  f0fb4f8:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0fb4fc:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0fb500:	0001c843 */ 	sra	$t9,$at,0x1
.PB0f0fb504:
/*  f0fb504:	03203825 */ 	move	$a3,$t9
/*  f0fb508:	05010003 */ 	bgez	$t0,.PB0f0fb518
/*  f0fb50c:	0008c043 */ 	sra	$t8,$t0,0x1
/*  f0fb510:	25010001 */ 	addiu	$at,$t0,0x1
/*  f0fb514:	0001c043 */ 	sra	$t8,$at,0x1
.PB0f0fb518:
/*  f0fb518:	03004025 */ 	move	$t0,$t8
.PB0f0fb51c:
/*  f0fb51c:	05610003 */ 	bgez	$t3,.PB0f0fb52c
/*  f0fb520:	a3ac010d */ 	sb	$t4,0x10d($sp)
/*  f0fb524:	10000001 */ 	b	.PB0f0fb52c
/*  f0fb528:	000b1023 */ 	negu	$v0,$t3
.PB0f0fb52c:
/*  f0fb52c:	05410003 */ 	bgez	$t2,.PB0f0fb53c
/*  f0fb530:	28410014 */ 	slti	$at,$v0,0x14
/*  f0fb534:	10000002 */ 	b	.PB0f0fb540
/*  f0fb538:	000a1823 */ 	negu	$v1,$t2
.PB0f0fb53c:
/*  f0fb53c:	01401825 */ 	move	$v1,$t2
.PB0f0fb540:
/*  f0fb540:	14200067 */ 	bnez	$at,.PB0f0fb6e0
/*  f0fb544:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fb548:	14200003 */ 	bnez	$at,.PB0f0fb558
/*  f0fb54c:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb550:	52400064 */ 	beqzl	$s2,.PB0f0fb6e4
/*  f0fb554:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb558:
/*  f0fb558:	05610006 */ 	bgez	$t3,.PB0f0fb574
/*  f0fb55c:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fb560:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb564:	846e0834 */ 	lh	$t6,0x834($v1)
/*  f0fb568:	05c30003 */ 	bgezl	$t6,.PB0f0fb578
/*  f0fb56c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb570:	a4600834 */ 	sh	$zero,0x834($v1)
.PB0f0fb574:
/*  f0fb574:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb578:
/*  f0fb578:	19600005 */ 	blez	$t3,.PB0f0fb590
/*  f0fb57c:	84650834 */ 	lh	$a1,0x834($v1)
/*  f0fb580:	18a00003 */ 	blez	$a1,.PB0f0fb590
/*  f0fb584:	00000000 */ 	nop
/*  f0fb588:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb58c:	84650834 */ 	lh	$a1,0x834($v1)
.PB0f0fb590:
/*  f0fb590:	14a00003 */ 	bnez	$a1,.PB0f0fb5a0
/*  f0fb594:	00000000 */ 	nop
/*  f0fb598:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fb59c:	84650834 */ 	lh	$a1,0x834($v1)
.PB0f0fb5a0:
/*  f0fb5a0:	14200002 */ 	bnez	$at,.PB0f0fb5ac
/*  f0fb5a4:	8cc6e4d0 */ 	lw	$a2,-0x1b30($a2)
/*  f0fb5a8:	24020046 */ 	li	$v0,0x46
.PB0f0fb5ac:
/*  f0fb5ac:	2442ffec */ 	addiu	$v0,$v0,-20
/*  f0fb5b0:	02820019 */ 	multu	$s4,$v0
/*  f0fb5b4:	24010032 */ 	li	$at,0x32
/*  f0fb5b8:	00007812 */ 	mflo	$t7
/*  f0fb5bc:	00000000 */ 	nop
/*  f0fb5c0:	00000000 */ 	nop
/*  f0fb5c4:	01e1001a */ 	div	$zero,$t7,$at
/*  f0fb5c8:	0000c812 */ 	mflo	$t9
/*  f0fb5cc:	02792023 */ 	subu	$a0,$s3,$t9
/*  f0fb5d0:	28a10003 */ 	slti	$at,$a1,0x3
/*  f0fb5d4:	10200003 */ 	beqz	$at,.PB0f0fb5e4
/*  f0fb5d8:	00801825 */ 	move	$v1,$a0
/*  f0fb5dc:	28a1fffe */ 	slti	$at,$a1,-2
/*  f0fb5e0:	10200005 */ 	beqz	$at,.PB0f0fb5f8
.PB0f0fb5e4:
/*  f0fb5e4:	24010003 */ 	li	$at,0x3
/*  f0fb5e8:	0081001a */ 	div	$zero,$a0,$at
/*  f0fb5ec:	00001812 */ 	mflo	$v1
/*  f0fb5f0:	00000000 */ 	nop
/*  f0fb5f4:	00000000 */ 	nop
.PB0f0fb5f8:
/*  f0fb5f8:	18600036 */ 	blez	$v1,.PB0f0fb6d4
/*  f0fb5fc:	28610004 */ 	slti	$at,$v1,0x4
/*  f0fb600:	1420000b */ 	bnez	$at,.PB0f0fb630
/*  f0fb604:	8faf00fc */ 	lw	$t7,0xfc($sp)
/*  f0fb608:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fb60c:	0303c023 */ 	subu	$t8,$t8,$v1
/*  f0fb610:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb614:	0303c021 */ 	addu	$t8,$t8,$v1
/*  f0fb618:	0018c040 */ 	sll	$t8,$t8,0x1
/*  f0fb61c:	2401003c */ 	li	$at,0x3c
/*  f0fb620:	0301001a */ 	div	$zero,$t8,$at
/*  f0fb624:	00007012 */ 	mflo	$t6
/*  f0fb628:	01c01825 */ 	move	$v1,$t6
/*  f0fb62c:	00000000 */ 	nop
.PB0f0fb630:
/*  f0fb630:	8de20830 */ 	lw	$v0,0x830($t7)
/*  f0fb634:	240c0001 */ 	li	$t4,0x1
/*  f0fb638:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fb63c:	0046c821 */ 	addu	$t9,$v0,$a2
/*  f0fb640:	00003812 */ 	mflo	$a3
/*  f0fb644:	14600002 */ 	bnez	$v1,.PB0f0fb650
/*  f0fb648:	00000000 */ 	nop
/*  f0fb64c:	0007000d */ 	break	0x7
.PB0f0fb650:
/*  f0fb650:	2401ffff */ 	li	$at,-1
/*  f0fb654:	14610004 */ 	bne	$v1,$at,.PB0f0fb668
/*  f0fb658:	3c018000 */ 	lui	$at,0x8000
/*  f0fb65c:	14410002 */ 	bne	$v0,$at,.PB0f0fb668
/*  f0fb660:	00000000 */ 	nop
/*  f0fb664:	0006000d */ 	break	0x6
.PB0f0fb668:
/*  f0fb668:	0323001a */ 	div	$zero,$t9,$v1
/*  f0fb66c:	00004012 */ 	mflo	$t0
/*  f0fb670:	14600002 */ 	bnez	$v1,.PB0f0fb67c
/*  f0fb674:	00000000 */ 	nop
/*  f0fb678:	0007000d */ 	break	0x7
.PB0f0fb67c:
/*  f0fb67c:	2401ffff */ 	li	$at,-1
/*  f0fb680:	14610004 */ 	bne	$v1,$at,.PB0f0fb694
/*  f0fb684:	3c018000 */ 	lui	$at,0x8000
/*  f0fb688:	17210002 */ 	bne	$t9,$at,.PB0f0fb694
/*  f0fb68c:	00000000 */ 	nop
/*  f0fb690:	0006000d */ 	break	0x6
.PB0f0fb694:
/*  f0fb694:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb698:	19600003 */ 	blez	$t3,.PB0f0fb6a8
/*  f0fb69c:	00000000 */ 	nop
/*  f0fb6a0:	10000001 */ 	b	.PB0f0fb6a8
/*  f0fb6a4:	240cffff */ 	li	$t4,-1
.PB0f0fb6a8:
/*  f0fb6a8:	10e80002 */ 	beq	$a3,$t0,.PB0f0fb6b4
/*  f0fb6ac:	00acc021 */ 	addu	$t8,$a1,$t4
/*  f0fb6b0:	240d0001 */ 	li	$t5,0x1
.PB0f0fb6b4:
/*  f0fb6b4:	14a00002 */ 	bnez	$a1,.PB0f0fb6c0
/*  f0fb6b8:	00000000 */ 	nop
/*  f0fb6bc:	240d0001 */ 	li	$t5,0x1
.PB0f0fb6c0:
/*  f0fb6c0:	51a00005 */ 	beqzl	$t5,.PB0f0fb6d8
/*  f0fb6c4:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb6c8:	a4780834 */ 	sh	$t8,0x834($v1)
/*  f0fb6cc:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb6d0:	8cc6e4d0 */ 	lw	$a2,-0x1b30($a2)
.PB0f0fb6d4:
/*  f0fb6d4:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb6d8:
/*  f0fb6d8:	10000006 */ 	b	.PB0f0fb6f4
/*  f0fb6dc:	8c620830 */ 	lw	$v0,0x830($v1)
.PB0f0fb6e0:
/*  f0fb6e0:	8fa300fc */ 	lw	$v1,0xfc($sp)
.PB0f0fb6e4:
/*  f0fb6e4:	3c06800a */ 	lui	$a2,0x800a
/*  f0fb6e8:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb6ec:	8cc6e4d0 */ 	lw	$a2,-0x1b30($a2)
/*  f0fb6f0:	8c620830 */ 	lw	$v0,0x830($v1)
.PB0f0fb6f4:
/*  f0fb6f4:	10e80002 */ 	beq	$a3,$t0,.PB0f0fb700
/*  f0fb6f8:	27b50104 */ 	addiu	$s5,$sp,0x104
/*  f0fb6fc:	240d0001 */ 	li	$t5,0x1
.PB0f0fb700:
/*  f0fb700:	55a00003 */ 	bnezl	$t5,.PB0f0fb710
/*  f0fb704:	8c6e0824 */ 	lw	$t6,0x824($v1)
/*  f0fb708:	00006025 */ 	move	$t4,$zero
/*  f0fb70c:	8c6e0824 */ 	lw	$t6,0x824($v1)
.PB0f0fb710:
/*  f0fb710:	8fa500f8 */ 	lw	$a1,0xf8($sp)
/*  f0fb714:	3c19800a */ 	lui	$t9,0x800a
/*  f0fb718:	01c67821 */ 	addu	$t7,$t6,$a2
/*  f0fb71c:	ac6f0824 */ 	sw	$t7,0x824($v1)
/*  f0fb720:	8f39e4d0 */ 	lw	$t9,-0x1b30($t9)
/*  f0fb724:	240e0001 */ 	li	$t6,0x1
/*  f0fb728:	0059c021 */ 	addu	$t8,$v0,$t9
/*  f0fb72c:	ac780830 */ 	sw	$t8,0x830($v1)
/*  f0fb730:	a3a90104 */ 	sb	$t1,0x104($sp)
/*  f0fb734:	a3ac0105 */ 	sb	$t4,0x105($sp)
/*  f0fb738:	a3aa0108 */ 	sb	$t2,0x108($sp)
/*  f0fb73c:	a3ab0109 */ 	sb	$t3,0x109($sp)
/*  f0fb740:	10a00003 */ 	beqz	$a1,.PB0f0fb750
/*  f0fb744:	a3a00118 */ 	sb	$zero,0x118($sp)
/*  f0fb748:	10000002 */ 	b	.PB0f0fb754
/*  f0fb74c:	a3ae010e */ 	sb	$t6,0x10e($sp)
.PB0f0fb750:
/*  f0fb750:	a3a0010e */ 	sb	$zero,0x10e($sp)
.PB0f0fb754:
/*  f0fb754:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fb758:	007e0019 */ 	multu	$v1,$s8
/*  f0fb75c:	00007812 */ 	mflo	$t7
/*  f0fb760:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fb764:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fb768:	50800016 */ 	beqzl	$a0,.PB0f0fb7c4
/*  f0fb76c:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb770:	50a00014 */ 	beqzl	$a1,.PB0f0fb7c4
/*  f0fb774:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb778:	8c990000 */ 	lw	$t9,0x0($a0)
/*  f0fb77c:	240f0001 */ 	li	$t7,0x1
/*  f0fb780:	8f380010 */ 	lw	$t8,0x10($t9)
/*  f0fb784:	24190001 */ 	li	$t9,0x1
/*  f0fb788:	330e0004 */ 	andi	$t6,$t8,0x4
/*  f0fb78c:	51c00004 */ 	beqzl	$t6,.PB0f0fb7a0
/*  f0fb790:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb794:	a3af0106 */ 	sb	$t7,0x106($sp)
/*  f0fb798:	afb9011c */ 	sw	$t9,0x11c($sp)
/*  f0fb79c:	8c820008 */ 	lw	$v0,0x8($a0)
.PB0f0fb7a0:
/*  f0fb7a0:	50400008 */ 	beqzl	$v0,.PB0f0fb7c4
/*  f0fb7a4:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb7a8:	90580000 */ 	lbu	$t8,0x0($v0)
/*  f0fb7ac:	24010002 */ 	li	$at,0x2
/*  f0fb7b0:	240e0001 */ 	li	$t6,0x1
/*  f0fb7b4:	57010003 */ 	bnel	$t8,$at,.PB0f0fb7c4
/*  f0fb7b8:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb7bc:	a3ae0106 */ 	sb	$t6,0x106($sp)
/*  f0fb7c0:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.PB0f0fb7c4:
/*  f0fb7c4:	00009825 */ 	move	$s3,$zero
/*  f0fb7c8:	0000a025 */ 	move	$s4,$zero
/*  f0fb7cc:	1840003a */ 	blez	$v0,.PB0f0fb8b8
/*  f0fb7d0:	00000000 */ 	nop
/*  f0fb7d4:	afa00044 */ 	sw	$zero,0x44($sp)
.PB0f0fb7d8:
/*  f0fb7d8:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fb7dc:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb7e0:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fb7e4:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fb7e8:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fb7ec:	8fb80044 */ 	lw	$t8,0x44($sp)
/*  f0fb7f0:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb7f4:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fb7f8:	02cfc821 */ 	addu	$t9,$s6,$t7
/*  f0fb7fc:	03389021 */ 	addu	$s2,$t9,$t8
/*  f0fb800:	824e0478 */ 	lb	$t6,0x478($s2)
/*  f0fb804:	26520464 */ 	addiu	$s2,$s2,0x464
/*  f0fb808:	02408825 */ 	move	$s1,$s2
/*  f0fb80c:	19c00024 */ 	blez	$t6,.PB0f0fb8a0
/*  f0fb810:	00008025 */ 	move	$s0,$zero
/*  f0fb814:	8ef90000 */ 	lw	$t9,0x0($s7)
.PB0f0fb818:
/*  f0fb818:	268f0001 */ 	addiu	$t7,$s4,0x1
/*  f0fb81c:	00003025 */ 	move	$a2,$zero
/*  f0fb820:	033e0019 */ 	multu	$t9,$s8
/*  f0fb824:	02a02825 */ 	move	$a1,$s5
/*  f0fb828:	0000c012 */ 	mflo	$t8
/*  f0fb82c:	02d87021 */ 	addu	$t6,$s6,$t8
/*  f0fb830:	85d904f4 */ 	lh	$t9,0x4f4($t6)
/*  f0fb834:	15f90008 */ 	bne	$t7,$t9,.PB0f0fb858
/*  f0fb838:	00000000 */ 	nop
/*  f0fb83c:	82580015 */ 	lb	$t8,0x15($s2)
/*  f0fb840:	16180005 */ 	bne	$s0,$t8,.PB0f0fb858
/*  f0fb844:	00000000 */ 	nop
/*  f0fb848:	16600003 */ 	bnez	$s3,.PB0f0fb858
/*  f0fb84c:	00000000 */ 	nop
/*  f0fb850:	24060001 */ 	li	$a2,0x1
/*  f0fb854:	24130001 */ 	li	$s3,0x1
.PB0f0fb858:
/*  f0fb858:	0fc3e62e */ 	jal	dialogTick
/*  f0fb85c:	8e240000 */ 	lw	$a0,0x0($s1)
/*  f0fb860:	824e0014 */ 	lb	$t6,0x14($s2)
/*  f0fb864:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0fb868:	26310004 */ 	addiu	$s1,$s1,0x4
/*  f0fb86c:	020e082a */ 	slt	$at,$s0,$t6
/*  f0fb870:	5420ffe9 */ 	bnezl	$at,.PB0f0fb818
/*  f0fb874:	8ef90000 */ 	lw	$t9,0x0($s7)
/*  f0fb878:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fb87c:	000378c0 */ 	sll	$t7,$v1,0x3
/*  f0fb880:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb884:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fb888:	01e37821 */ 	addu	$t7,$t7,$v1
/*  f0fb88c:	000f78c0 */ 	sll	$t7,$t7,0x3
/*  f0fb890:	01e37823 */ 	subu	$t7,$t7,$v1
/*  f0fb894:	000f7900 */ 	sll	$t7,$t7,0x4
/*  f0fb898:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fb89c:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.PB0f0fb8a0:
/*  f0fb8a0:	8fb90044 */ 	lw	$t9,0x44($sp)
/*  f0fb8a4:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fb8a8:	0282082a */ 	slt	$at,$s4,$v0
/*  f0fb8ac:	27380018 */ 	addiu	$t8,$t9,0x18
/*  f0fb8b0:	1420ffc9 */ 	bnez	$at,.PB0f0fb7d8
/*  f0fb8b4:	afb80044 */ 	sw	$t8,0x44($sp)
.PB0f0fb8b8:
/*  f0fb8b8:	3c0e800a */ 	lui	$t6,0x800a
/*  f0fb8bc:	8dce5ef4 */ 	lw	$t6,0x5ef4($t6)
/*  f0fb8c0:	25cfffff */ 	addiu	$t7,$t6,-1
/*  f0fb8c4:	2de1000d */ 	sltiu	$at,$t7,0xd
/*  f0fb8c8:	1020004d */ 	beqz	$at,.PB0f0fba00
/*  f0fb8cc:	000f7880 */ 	sll	$t7,$t7,0x2
/*  f0fb8d0:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fb8d4:	002f0821 */ 	addu	$at,$at,$t7
/*  f0fb8d8:	8c2f47e0 */ 	lw	$t7,0x47e0($at)
/*  f0fb8dc:	01e00008 */ 	jr	$t7
/*  f0fb8e0:	00000000 */ 	nop
/*  f0fb8e4:	83b9010e */ 	lb	$t9,0x10e($sp)
/*  f0fb8e8:	8fb8011c */ 	lw	$t8,0x11c($sp)
/*  f0fb8ec:	53200045 */ 	beqzl	$t9,.PB0f0fba04
/*  f0fb8f0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb8f4:	57000043 */ 	bnezl	$t8,.PB0f0fba04
/*  f0fb8f8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb8fc:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fb900:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fb904:	5080003f */ 	beqzl	$a0,.PB0f0fba04
/*  f0fb908:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb90c:	8dcf000c */ 	lw	$t7,0xc($t6)
/*  f0fb910:	3c19800a */ 	lui	$t9,0x800a
/*  f0fb914:	55e0003b */ 	bnezl	$t7,.PB0f0fba04
/*  f0fb918:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb91c:	8f39e960 */ 	lw	$t9,-0x16a0($t9)
/*  f0fb920:	8c820000 */ 	lw	$v0,0x0($a0)
/*  f0fb924:	24010002 */ 	li	$at,0x2
/*  f0fb928:	13210008 */ 	beq	$t9,$at,.PB0f0fb94c
/*  f0fb92c:	3c048008 */ 	lui	$a0,0x8008
/*  f0fb930:	24847c98 */ 	addiu	$a0,$a0,0x7c98
/*  f0fb934:	10820005 */ 	beq	$a0,$v0,.PB0f0fb94c
/*  f0fb938:	00000000 */ 	nop
/*  f0fb93c:	0fc3cd2e */ 	jal	menuPushDialog
/*  f0fb940:	00000000 */ 	nop
/*  f0fb944:	1000002f */ 	b	.PB0f0fba04
/*  f0fb948:	8fbf003c */ 	lw	$ra,0x3c($sp)
.PB0f0fb94c:
/*  f0fb94c:	3c188009 */ 	lui	$t8,0x8009
/*  f0fb950:	27188b60 */ 	addiu	$t8,$t8,-29856
/*  f0fb954:	5702002b */ 	bnel	$t8,$v0,.PB0f0fba04
/*  f0fb958:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb95c:	0fc603da */ 	jal	func0f17f428
/*  f0fb960:	00000000 */ 	nop
/*  f0fb964:	10000027 */ 	b	.PB0f0fba04
/*  f0fb968:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb96c:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0fb970:	8dce2434 */ 	lw	$t6,0x2434($t6)
/*  f0fb974:	51c00010 */ 	beqzl	$t6,.PB0f0fb9b8
/*  f0fb978:	240e000a */ 	li	$t6,0xa
/*  f0fb97c:	0fc3e19f */ 	jal	func0f0f8120
/*  f0fb980:	00000000 */ 	nop
/*  f0fb984:	3c0f8007 */ 	lui	$t7,0x8007
/*  f0fb988:	8def3118 */ 	lw	$t7,0x3118($t7)
/*  f0fb98c:	3c18800a */ 	lui	$t8,0x800a
/*  f0fb990:	27182530 */ 	addiu	$t8,$t8,0x2530
/*  f0fb994:	000fc8c0 */ 	sll	$t9,$t7,0x3
/*  f0fb998:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fb99c:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fb9a0:	032fc821 */ 	addu	$t9,$t9,$t7
/*  f0fb9a4:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fb9a8:	032fc823 */ 	subu	$t9,$t9,$t7
/*  f0fb9ac:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fb9b0:	03383821 */ 	addu	$a3,$t9,$t8
/*  f0fb9b4:	240e000a */ 	li	$t6,0xa
.PB0f0fb9b8:
/*  f0fb9b8:	a0ee083c */ 	sb	$t6,0x83c($a3)
/*  f0fb9bc:	83af010e */ 	lb	$t7,0x10e($sp)
/*  f0fb9c0:	8fb9011c */ 	lw	$t9,0x11c($sp)
/*  f0fb9c4:	51e0000f */ 	beqzl	$t7,.PB0f0fba04
/*  f0fb9c8:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb9cc:	5720000d */ 	bnezl	$t9,.PB0f0fba04
/*  f0fb9d0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb9d4:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fb9d8:	8fae0100 */ 	lw	$t6,0x100($sp)
/*  f0fb9dc:	53000009 */ 	beqzl	$t8,.PB0f0fba04
/*  f0fb9e0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb9e4:	8dcf0000 */ 	lw	$t7,0x0($t6)
/*  f0fb9e8:	8df90010 */ 	lw	$t9,0x10($t7)
/*  f0fb9ec:	33380020 */ 	andi	$t8,$t9,0x20
/*  f0fb9f0:	57000004 */ 	bnezl	$t8,.PB0f0fba04
/*  f0fb9f4:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb9f8:	0fc3e19f */ 	jal	func0f0f8120
/*  f0fb9fc:	00000000 */ 	nop
.PB0f0fba00:
/*  f0fba00:	8fbf003c */ 	lw	$ra,0x3c($sp)
.PB0f0fba04:
/*  f0fba04:	8fb00018 */ 	lw	$s0,0x18($sp)
/*  f0fba08:	8fb1001c */ 	lw	$s1,0x1c($sp)
/*  f0fba0c:	8fb20020 */ 	lw	$s2,0x20($sp)
/*  f0fba10:	8fb30024 */ 	lw	$s3,0x24($sp)
/*  f0fba14:	8fb40028 */ 	lw	$s4,0x28($sp)
/*  f0fba18:	8fb5002c */ 	lw	$s5,0x2c($sp)
/*  f0fba1c:	8fb60030 */ 	lw	$s6,0x30($sp)
/*  f0fba20:	8fb70034 */ 	lw	$s7,0x34($sp)
/*  f0fba24:	8fbe0038 */ 	lw	$s8,0x38($sp)
/*  f0fba28:	03e00008 */ 	jr	$ra
/*  f0fba2c:	27bd0130 */ 	addiu	$sp,$sp,0x130
);
#elif VERSION >= VERSION_NTSC_1_0
GLOBAL_ASM(
glabel menuProcessInput
.late_rodata
glabel var7f1b2a98
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2a9c
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2aa0
.word menuProcessInput+0xc38 # f0fb33c
glabel var7f1b2aa4
.word menuProcessInput+0xcc0 # f0fb3c4
glabel var7f1b2aa8
.word menuProcessInput+0xd10 # f0fb414
glabel var7f1b2aac
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab0
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab4
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ab8
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2abc
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ac0
.word menuProcessInput+0xc38 # f0fb33c
glabel var7f1b2ac4
.word menuProcessInput+0xd54 # f0fb458
glabel var7f1b2ac8
.word menuProcessInput+0xd10 # f0fb414
.text
/*  f0fa704:	27bdfed0 */ 	addiu	$sp,$sp,-304
/*  f0fa708:	afb70034 */ 	sw	$s7,0x34($sp)
/*  f0fa70c:	3c178007 */ 	lui	$s7,%hi(g_MpPlayerNum)
/*  f0fa710:	26f71448 */ 	addiu	$s7,$s7,%lo(g_MpPlayerNum)
/*  f0fa714:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fa718:	afbe0038 */ 	sw	$s8,0x38($sp)
/*  f0fa71c:	241e0e70 */ 	addiu	$s8,$zero,0xe70
/*  f0fa720:	01de0019 */ 	multu	$t6,$s8
/*  f0fa724:	afb60030 */ 	sw	$s6,0x30($sp)
/*  f0fa728:	3c16800a */ 	lui	$s6,%hi(g_Menus)
/*  f0fa72c:	26d6e000 */ 	addiu	$s6,$s6,%lo(g_Menus)
/*  f0fa730:	3c19800a */ 	lui	$t9,%hi(g_MenuData+0x4)
/*  f0fa734:	8f3919c4 */ 	lw	$t9,%lo(g_MenuData+0x4)($t9)
/*  f0fa738:	afb5002c */ 	sw	$s5,0x2c($sp)
/*  f0fa73c:	24010004 */ 	addiu	$at,$zero,0x4
/*  f0fa740:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0fa744:	afb40028 */ 	sw	$s4,0x28($sp)
/*  f0fa748:	00007812 */ 	mflo	$t7
/*  f0fa74c:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fa750:	8cf804f8 */ 	lw	$t8,0x4f8($a3)
/*  f0fa754:	afb30024 */ 	sw	$s3,0x24($sp)
/*  f0fa758:	afb20020 */ 	sw	$s2,0x20($sp)
/*  f0fa75c:	afb1001c */ 	sw	$s1,0x1c($sp)
/*  f0fa760:	afb00018 */ 	sw	$s0,0x18($sp)
/*  f0fa764:	00006025 */ 	or	$t4,$zero,$zero
/*  f0fa768:	00004825 */ 	or	$t1,$zero,$zero
/*  f0fa76c:	00004025 */ 	or	$t0,$zero,$zero
/*  f0fa770:	0000a825 */ 	or	$s5,$zero,$zero
/*  f0fa774:	afa0011c */ 	sw	$zero,0x11c($sp)
/*  f0fa778:	afa700fc */ 	sw	$a3,0xfc($sp)
/*  f0fa77c:	17210005 */ 	bne	$t9,$at,.L0f0fa794
/*  f0fa780:	afb80100 */ 	sw	$t8,0x100($sp)
/*  f0fa784:	3c0e800a */ 	lui	$t6,%hi(g_Vars+0x28c)
/*  f0fa788:	8dcea24c */ 	lw	$t6,%lo(g_Vars+0x28c)($t6)
/*  f0fa78c:	3c01800a */ 	lui	$at,%hi(g_AmIndex)
/*  f0fa790:	ac2e21b8 */ 	sw	$t6,%lo(g_AmIndex)($at)
.L0f0fa794:
/*  f0fa794:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fa798:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fa79c:	0fc3c506 */ 	jal	func0f0f1418
/*  f0fa7a0:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fa7a4:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fa7a8:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fa7ac:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fa7b0:	01fe0019 */ 	multu	$t7,$s8
/*  f0fa7b4:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fa7b8:	a3a00106 */ 	sb	$zero,0x106($sp)
/*  f0fa7bc:	a3a00107 */ 	sb	$zero,0x107($sp)
/*  f0fa7c0:	a3a0010a */ 	sb	$zero,0x10a($sp)
/*  f0fa7c4:	a3a0010b */ 	sb	$zero,0x10b($sp)
/*  f0fa7c8:	00005025 */ 	or	$t2,$zero,$zero
/*  f0fa7cc:	00005825 */ 	or	$t3,$zero,$zero
/*  f0fa7d0:	3c0f800a */ 	lui	$t7,%hi(g_MenuData+0x4)
/*  f0fa7d4:	0000c012 */ 	mflo	$t8
/*  f0fa7d8:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fa7dc:	8f2e04f8 */ 	lw	$t6,0x4f8($t9)
/*  f0fa7e0:	51c0031e */ 	beqzl	$t6,.L0f0fb45c
/*  f0fa7e4:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fa7e8:	8def19c4 */ 	lw	$t7,%lo(g_MenuData+0x4)($t7)
/*  f0fa7ec:	24010007 */ 	addiu	$at,$zero,0x7
/*  f0fa7f0:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*  f0fa7f4:	15e1000b */ 	bne	$t7,$at,.L0f0fa824
/*  f0fa7f8:	afa000ec */ 	sw	$zero,0xec($sp)
/*  f0fa7fc:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0fa800:	24190002 */ 	addiu	$t9,$zero,0x2
/*  f0fa804:	240e0003 */ 	addiu	$t6,$zero,0x3
/*  f0fa808:	240f0004 */ 	addiu	$t7,$zero,0x4
/*  f0fa80c:	afa000d8 */ 	sw	$zero,0xd8($sp)
/*  f0fa810:	afb800dc */ 	sw	$t8,0xdc($sp)
/*  f0fa814:	afb900e0 */ 	sw	$t9,0xe0($sp)
/*  f0fa818:	afae00e4 */ 	sw	$t6,0xe4($sp)
/*  f0fa81c:	1000001b */ 	b	.L0f0fa88c
/*  f0fa820:	afaf00ec */ 	sw	$t7,0xec($sp)
.L0f0fa824:
/*  f0fa824:	27a400d7 */ 	addiu	$a0,$sp,0xd7
/*  f0fa828:	27a500d6 */ 	addiu	$a1,$sp,0xd6
/*  f0fa82c:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fa830:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fa834:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fa838:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fa83c:	0fc3dd2a */ 	jal	menuGetContPads
/*  f0fa840:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fa844:	83a200d7 */ 	lb	$v0,0xd7($sp)
/*  f0fa848:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fa84c:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fa850:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fa854:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fa858:	04400004 */ 	bltz	$v0,.L0f0fa86c
/*  f0fa85c:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fa860:	24030001 */ 	addiu	$v1,$zero,0x1
/*  f0fa864:	afa200d8 */ 	sw	$v0,0xd8($sp)
/*  f0fa868:	afa300ec */ 	sw	$v1,0xec($sp)
.L0f0fa86c:
/*  f0fa86c:	83a200d6 */ 	lb	$v0,0xd6($sp)
/*  f0fa870:	8fa300ec */ 	lw	$v1,0xec($sp)
/*  f0fa874:	04400005 */ 	bltz	$v0,.L0f0fa88c
/*  f0fa878:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0fa87c:	03b8c821 */ 	addu	$t9,$sp,$t8
/*  f0fa880:	af2200d8 */ 	sw	$v0,0xd8($t9)
/*  f0fa884:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0fa888:	afa300ec */ 	sw	$v1,0xec($sp)
.L0f0fa88c:
/*  f0fa88c:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0fa890:	01de0019 */ 	multu	$t6,$s8
/*  f0fa894:	00007812 */ 	mflo	$t7
/*  f0fa898:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0fa89c:	90e20e41 */ 	lbu	$v0,0xe41($a3)
/*  f0fa8a0:	1840001a */ 	blez	$v0,.L0f0fa90c
/*  f0fa8a4:	2458ffff */ 	addiu	$t8,$v0,-1
/*  f0fa8a8:	331900ff */ 	andi	$t9,$t8,0xff
/*  f0fa8ac:	17200014 */ 	bnez	$t9,.L0f0fa900
/*  f0fa8b0:	a0f80e41 */ 	sb	$t8,0xe41($a3)
/*  f0fa8b4:	8cee0e48 */ 	lw	$t6,0xe48($a3)
/*  f0fa8b8:	8cef0e4c */ 	lw	$t7,0xe4c($a3)
/*  f0fa8bc:	27a400cc */ 	addiu	$a0,$sp,0xcc
/*  f0fa8c0:	2405ffff */ 	addiu	$a1,$zero,-1
/*  f0fa8c4:	00003025 */ 	or	$a2,$zero,$zero
/*  f0fa8c8:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fa8cc:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fa8d0:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fa8d4:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fa8d8:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fa8dc:	afae00cc */ 	sw	$t6,0xcc($sp)
/*  f0fa8e0:	0fc42539 */ 	jal	filemgrSaveOrLoad
/*  f0fa8e4:	a7af00d0 */ 	sh	$t7,0xd0($sp)
/*  f0fa8e8:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fa8ec:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fa8f0:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fa8f4:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fa8f8:	10000004 */ 	b	.L0f0fa90c
/*  f0fa8fc:	8fac012c */ 	lw	$t4,0x12c($sp)
.L0f0fa900:
/*  f0fa900:	90f90e40 */ 	lbu	$t9,0xe40($a3)
/*  f0fa904:	372e0080 */ 	ori	$t6,$t9,0x80
/*  f0fa908:	a0ee0e40 */ 	sb	$t6,0xe40($a3)
.L0f0fa90c:
/*  f0fa90c:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0fa910:	0000a025 */ 	or	$s4,$zero,$zero
/*  f0fa914:	19e00083 */ 	blez	$t7,.L0f0fab24
/*  f0fa918:	27b300d8 */ 	addiu	$s3,$sp,0xd8
.L0f0fa91c:
/*  f0fa91c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fa920:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0fa924:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0fa928:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0fa92c:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0fa930:	0c005384 */ 	jal	joyGetStickX
/*  f0fa934:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0fa938:	00028600 */ 	sll	$s0,$v0,0x18
/*  f0fa93c:	0010c603 */ 	sra	$t8,$s0,0x18
/*  f0fa940:	03008025 */ 	or	$s0,$t8,$zero
/*  f0fa944:	0c0053ae */ 	jal	joyGetStickY
/*  f0fa948:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fa94c:	00028e00 */ 	sll	$s1,$v0,0x18
/*  f0fa950:	0011ce03 */ 	sra	$t9,$s1,0x18
/*  f0fa954:	03208825 */ 	or	$s1,$t9,$zero
/*  f0fa958:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fa95c:	0c0053d8 */ 	jal	joyGetButtons
/*  f0fa960:	3405ffff */ 	dli	$a1,0xffff
/*  f0fa964:	3052ffff */ 	andi	$s2,$v0,0xffff
/*  f0fa968:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0fa96c:	0c005408 */ 	jal	joyGetButtonsPressedThisFrame
/*  f0fa970:	3405ffff */ 	dli	$a1,0xffff
/*  f0fa974:	304e8000 */ 	andi	$t6,$v0,0x8000
/*  f0fa978:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0fa97c:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0fa980:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0fa984:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0fa988:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0fa98c:	11c00003 */ 	beqz	$t6,.L0f0fa99c
/*  f0fa990:	00403025 */ 	or	$a2,$v0,$zero
/*  f0fa994:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0fa998:	a3af0106 */ 	sb	$t7,0x106($sp)
.L0f0fa99c:
/*  f0fa99c:	30d84000 */ 	andi	$t8,$a2,0x4000
/*  f0fa9a0:	13000003 */ 	beqz	$t8,.L0f0fa9b0
/*  f0fa9a4:	30ce2000 */ 	andi	$t6,$a2,0x2000
/*  f0fa9a8:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0fa9ac:	a3b90107 */ 	sb	$t9,0x107($sp)
.L0f0fa9b0:
/*  f0fa9b0:	11c00003 */ 	beqz	$t6,.L0f0fa9c0
/*  f0fa9b4:	30d81000 */ 	andi	$t8,$a2,0x1000
/*  f0fa9b8:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0fa9bc:	a3af0106 */ 	sb	$t7,0x106($sp)
.L0f0fa9c0:
/*  f0fa9c0:	13000003 */ 	beqz	$t8,.L0f0fa9d0
/*  f0fa9c4:	324e0010 */ 	andi	$t6,$s2,0x10
/*  f0fa9c8:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0fa9cc:	afb900f8 */ 	sw	$t9,0xf8($sp)
.L0f0fa9d0:
/*  f0fa9d0:	11c00003 */ 	beqz	$t6,.L0f0fa9e0
/*  f0fa9d4:	02402825 */ 	or	$a1,$s2,$zero
/*  f0fa9d8:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0fa9dc:	a3af010a */ 	sb	$t7,0x10a($sp)
.L0f0fa9e0:
/*  f0fa9e0:	30b80020 */ 	andi	$t8,$a1,0x20
/*  f0fa9e4:	13000003 */ 	beqz	$t8,.L0f0fa9f4
/*  f0fa9e8:	30ae0008 */ 	andi	$t6,$a1,0x8
/*  f0fa9ec:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0fa9f0:	a3b9010a */ 	sb	$t9,0x10a($sp)
.L0f0fa9f4:
/*  f0fa9f4:	05410003 */ 	bgez	$t2,.L0f0faa04
/*  f0fa9f8:	30cf0008 */ 	andi	$t7,$a2,0x8
/*  f0fa9fc:	10000002 */ 	b	.L0f0faa08
/*  f0faa00:	000a2023 */ 	negu	$a0,$t2
.L0f0faa04:
/*  f0faa04:	01402025 */ 	or	$a0,$t2,$zero
.L0f0faa08:
/*  f0faa08:	06010003 */ 	bgez	$s0,.L0f0faa18
/*  f0faa0c:	30b80004 */ 	andi	$t8,$a1,0x4
/*  f0faa10:	10000002 */ 	b	.L0f0faa1c
/*  f0faa14:	00101823 */ 	negu	$v1,$s0
.L0f0faa18:
/*  f0faa18:	02001825 */ 	or	$v1,$s0,$zero
.L0f0faa1c:
/*  f0faa1c:	0083082a */ 	slt	$at,$a0,$v1
/*  f0faa20:	10200002 */ 	beqz	$at,.L0f0faa2c
/*  f0faa24:	30d90004 */ 	andi	$t9,$a2,0x4
/*  f0faa28:	02005025 */ 	or	$t2,$s0,$zero
.L0f0faa2c:
/*  f0faa2c:	05610003 */ 	bgez	$t3,.L0f0faa3c
/*  f0faa30:	01602025 */ 	or	$a0,$t3,$zero
/*  f0faa34:	10000001 */ 	b	.L0f0faa3c
/*  f0faa38:	000b2023 */ 	negu	$a0,$t3
.L0f0faa3c:
/*  f0faa3c:	06210003 */ 	bgez	$s1,.L0f0faa4c
/*  f0faa40:	02201825 */ 	or	$v1,$s1,$zero
/*  f0faa44:	10000001 */ 	b	.L0f0faa4c
/*  f0faa48:	00111823 */ 	negu	$v1,$s1
.L0f0faa4c:
/*  f0faa4c:	0083082a */ 	slt	$at,$a0,$v1
/*  f0faa50:	10200002 */ 	beqz	$at,.L0f0faa5c
/*  f0faa54:	00000000 */ 	nop
/*  f0faa58:	02205825 */ 	or	$t3,$s1,$zero
.L0f0faa5c:
/*  f0faa5c:	11c00002 */ 	beqz	$t6,.L0f0faa68
/*  f0faa60:	00000000 */ 	nop
/*  f0faa64:	240cffff */ 	addiu	$t4,$zero,-1
.L0f0faa68:
/*  f0faa68:	11e00002 */ 	beqz	$t7,.L0f0faa74
/*  f0faa6c:	30ae0002 */ 	andi	$t6,$a1,0x2
/*  f0faa70:	2408ffff */ 	addiu	$t0,$zero,-1
.L0f0faa74:
/*  f0faa74:	13000002 */ 	beqz	$t8,.L0f0faa80
/*  f0faa78:	30cf0002 */ 	andi	$t7,$a2,0x2
/*  f0faa7c:	240c0001 */ 	addiu	$t4,$zero,0x1
.L0f0faa80:
/*  f0faa80:	13200002 */ 	beqz	$t9,.L0f0faa8c
/*  f0faa84:	30b80001 */ 	andi	$t8,$a1,0x1
/*  f0faa88:	24080001 */ 	addiu	$t0,$zero,0x1
.L0f0faa8c:
/*  f0faa8c:	11c00002 */ 	beqz	$t6,.L0f0faa98
/*  f0faa90:	30d90001 */ 	andi	$t9,$a2,0x1
/*  f0faa94:	2409ffff */ 	addiu	$t1,$zero,-1
.L0f0faa98:
/*  f0faa98:	11e00002 */ 	beqz	$t7,.L0f0faaa4
/*  f0faa9c:	30ae0800 */ 	andi	$t6,$a1,0x800
/*  f0faaa0:	2415ffff */ 	addiu	$s5,$zero,-1
.L0f0faaa4:
/*  f0faaa4:	13000002 */ 	beqz	$t8,.L0f0faab0
/*  f0faaa8:	30cf0800 */ 	andi	$t7,$a2,0x800
/*  f0faaac:	24090001 */ 	addiu	$t1,$zero,0x1
.L0f0faab0:
/*  f0faab0:	13200002 */ 	beqz	$t9,.L0f0faabc
/*  f0faab4:	30b80400 */ 	andi	$t8,$a1,0x400
/*  f0faab8:	24150001 */ 	addiu	$s5,$zero,0x1
.L0f0faabc:
/*  f0faabc:	11c00002 */ 	beqz	$t6,.L0f0faac8
/*  f0faac0:	30d90400 */ 	andi	$t9,$a2,0x400
/*  f0faac4:	240cffff */ 	addiu	$t4,$zero,-1
.L0f0faac8:
/*  f0faac8:	11e00002 */ 	beqz	$t7,.L0f0faad4
/*  f0faacc:	30ae0200 */ 	andi	$t6,$a1,0x200
/*  f0faad0:	2408ffff */ 	addiu	$t0,$zero,-1
.L0f0faad4:
/*  f0faad4:	13000002 */ 	beqz	$t8,.L0f0faae0
/*  f0faad8:	30cf0200 */ 	andi	$t7,$a2,0x200
/*  f0faadc:	240c0001 */ 	addiu	$t4,$zero,0x1
.L0f0faae0:
/*  f0faae0:	13200002 */ 	beqz	$t9,.L0f0faaec
/*  f0faae4:	30b80100 */ 	andi	$t8,$a1,0x100
/*  f0faae8:	24080001 */ 	addiu	$t0,$zero,0x1
.L0f0faaec:
/*  f0faaec:	11c00002 */ 	beqz	$t6,.L0f0faaf8
/*  f0faaf0:	30d90100 */ 	andi	$t9,$a2,0x100
/*  f0faaf4:	2409ffff */ 	addiu	$t1,$zero,-1
.L0f0faaf8:
/*  f0faaf8:	11e00002 */ 	beqz	$t7,.L0f0fab04
/*  f0faafc:	8fae00ec */ 	lw	$t6,0xec($sp)
/*  f0fab00:	2415ffff */ 	addiu	$s5,$zero,-1
.L0f0fab04:
/*  f0fab04:	13000002 */ 	beqz	$t8,.L0f0fab10
/*  f0fab08:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fab0c:	24090001 */ 	addiu	$t1,$zero,0x1
.L0f0fab10:
/*  f0fab10:	13200002 */ 	beqz	$t9,.L0f0fab1c
/*  f0fab14:	00000000 */ 	nop
/*  f0fab18:	24150001 */ 	addiu	$s5,$zero,0x1
.L0f0fab1c:
/*  f0fab1c:	168eff7f */ 	bne	$s4,$t6,.L0f0fa91c
/*  f0fab20:	26730004 */ 	addiu	$s3,$s3,0x4
.L0f0fab24:
/*  f0fab24:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fab28:	93ae0106 */ 	lbu	$t6,0x106($sp)
/*  f0fab2c:	2410000a */ 	addiu	$s0,$zero,0xa
/*  f0fab30:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0fab34:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fab38:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0fab3c:	030fc021 */ 	addu	$t8,$t8,$t7
/*  f0fab40:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fab44:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0fab48:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0fab4c:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0fab50:	11c00002 */ 	beqz	$t6,.L0f0fab5c
/*  f0fab54:	8f2404f8 */ 	lw	$a0,0x4f8($t9)
/*  f0fab58:	a3a00107 */ 	sb	$zero,0x107($sp)
.L0f0fab5c:
/*  f0fab5c:	11000002 */ 	beqz	$t0,.L0f0fab68
/*  f0fab60:	2406001e */ 	addiu	$a2,$zero,0x1e
/*  f0fab64:	01006025 */ 	or	$t4,$t0,$zero
.L0f0fab68:
/*  f0fab68:	12a00002 */ 	beqz	$s5,.L0f0fab74
/*  f0fab6c:	2413003c */ 	addiu	$s3,$zero,0x3c
/*  f0fab70:	02a04825 */ 	or	$t1,$s5,$zero
.L0f0fab74:
/*  f0fab74:	24110021 */ 	addiu	$s1,$zero,0x21
/*  f0fab78:	2414002c */ 	addiu	$s4,$zero,0x2c
/*  f0fab7c:	10800019 */ 	beqz	$a0,.L0f0fabe4
/*  f0fab80:	00009025 */ 	or	$s2,$zero,$zero
/*  f0fab84:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fab88:	10400016 */ 	beqz	$v0,.L0f0fabe4
/*  f0fab8c:	00000000 */ 	nop
/*  f0fab90:	90430000 */ 	lbu	$v1,0x0($v0)
/*  f0fab94:	24010008 */ 	addiu	$at,$zero,0x8
/*  f0fab98:	10610003 */ 	beq	$v1,$at,.L0f0faba8
/*  f0fab9c:	24010010 */ 	addiu	$at,$zero,0x10
/*  f0faba0:	54610009 */ 	bnel	$v1,$at,.L0f0fabc8
/*  f0faba4:	2401000d */ 	addiu	$at,$zero,0xd
.L0f0faba8:
/*  f0faba8:	8c8f000c */ 	lw	$t7,0xc($a0)
/*  f0fabac:	51e00006 */ 	beqzl	$t7,.L0f0fabc8
/*  f0fabb0:	2401000d */ 	addiu	$at,$zero,0xd
/*  f0fabb4:	24100005 */ 	addiu	$s0,$zero,0x5
/*  f0fabb8:	24060014 */ 	addiu	$a2,$zero,0x14
/*  f0fabbc:	2413001e */ 	addiu	$s3,$zero,0x1e
/*  f0fabc0:	2411000a */ 	addiu	$s1,$zero,0xa
/*  f0fabc4:	2401000d */ 	addiu	$at,$zero,0xd
.L0f0fabc8:
/*  f0fabc8:	14610006 */ 	bne	$v1,$at,.L0f0fabe4
/*  f0fabcc:	00000000 */ 	nop
/*  f0fabd0:	24120001 */ 	addiu	$s2,$zero,0x1
/*  f0fabd4:	24100005 */ 	addiu	$s0,$zero,0x5
/*  f0fabd8:	24060014 */ 	addiu	$a2,$zero,0x14
/*  f0fabdc:	2411000a */ 	addiu	$s1,$zero,0xa
/*  f0fabe0:	2414000a */ 	addiu	$s4,$zero,0xa
.L0f0fabe4:
/*  f0fabe4:	15200004 */ 	bnez	$t1,.L0f0fabf8
/*  f0fabe8:	0000f825 */ 	or	$ra,$zero,$zero
/*  f0fabec:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fabf0:	2418ffff */ 	addiu	$t8,$zero,-1
/*  f0fabf4:	a478082c */ 	sh	$t8,0x82c($v1)
.L0f0fabf8:
/*  f0fabf8:	12a00006 */ 	beqz	$s5,.L0f0fac14
/*  f0fabfc:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fac00:	a460082c */ 	sh	$zero,0x82c($v1)
/*  f0fac04:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fac08:	a475082a */ 	sh	$s5,0x82a($v1)
/*  f0fac0c:	10000004 */ 	b	.L0f0fac20
/*  f0fac10:	241f0001 */ 	addiu	$ra,$zero,0x1
.L0f0fac14:
/*  f0fac14:	51200003 */ 	beqzl	$t1,.L0f0fac24
/*  f0fac18:	8c620824 */ 	lw	$v0,0x824($v1)
/*  f0fac1c:	8469082a */ 	lh	$t1,0x82a($v1)
.L0f0fac20:
/*  f0fac20:	8c620824 */ 	lw	$v0,0x824($v1)
.L0f0fac24:
/*  f0fac24:	3c0e800a */ 	lui	$t6,%hi(g_Vars)
/*  f0fac28:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0fac2c:	14200004 */ 	bnez	$at,.L0f0fac40
/*  f0fac30:	00000000 */ 	nop
/*  f0fac34:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0fac38:	a479082c */ 	sh	$t9,0x82c($v1)
/*  f0fac3c:	8c620824 */ 	lw	$v0,0x824($v1)
.L0f0fac40:
/*  f0fac40:	0050001a */ 	div	$zero,$v0,$s0
/*  f0fac44:	8dce9fc0 */ 	lw	$t6,%lo(g_Vars)($t6)
/*  f0fac48:	00003812 */ 	mflo	$a3
/*  f0fac4c:	8478082c */ 	lh	$t8,0x82c($v1)
/*  f0fac50:	004e7821 */ 	addu	$t7,$v0,$t6
/*  f0fac54:	01f0001a */ 	div	$zero,$t7,$s0
/*  f0fac58:	16000002 */ 	bnez	$s0,.L0f0fac64
/*  f0fac5c:	00000000 */ 	nop
/*  f0fac60:	0007000d */ 	break	0x7
.L0f0fac64:
/*  f0fac64:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fac68:	16010004 */ 	bne	$s0,$at,.L0f0fac7c
/*  f0fac6c:	3c018000 */ 	lui	$at,0x8000
/*  f0fac70:	14410002 */ 	bne	$v0,$at,.L0f0fac7c
/*  f0fac74:	00000000 */ 	nop
/*  f0fac78:	0006000d */ 	break	0x6
.L0f0fac7c:
/*  f0fac7c:	00006812 */ 	mflo	$t5
/*  f0fac80:	16000002 */ 	bnez	$s0,.L0f0fac8c
/*  f0fac84:	00000000 */ 	nop
/*  f0fac88:	0007000d */ 	break	0x7
.L0f0fac8c:
/*  f0fac8c:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fac90:	16010004 */ 	bne	$s0,$at,.L0f0faca4
/*  f0fac94:	3c018000 */ 	lui	$at,0x8000
/*  f0fac98:	15e10002 */ 	bne	$t7,$at,.L0f0faca4
/*  f0fac9c:	00000000 */ 	nop
/*  f0faca0:	0006000d */ 	break	0x6
.L0f0faca4:
/*  f0faca4:	1700000b */ 	bnez	$t8,.L0f0facd4
/*  f0faca8:	00000000 */ 	nop
/*  f0facac:	04e10003 */ 	bgez	$a3,.L0f0facbc
/*  f0facb0:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0facb4:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0facb8:	0001c843 */ 	sra	$t9,$at,0x1
.L0f0facbc:
/*  f0facbc:	03203825 */ 	or	$a3,$t9,$zero
/*  f0facc0:	05a10003 */ 	bgez	$t5,.L0f0facd0
/*  f0facc4:	000d7043 */ 	sra	$t6,$t5,0x1
/*  f0facc8:	25a10001 */ 	addiu	$at,$t5,0x1
/*  f0faccc:	00017043 */ 	sra	$t6,$at,0x1
.L0f0facd0:
/*  f0facd0:	01c06825 */ 	or	$t5,$t6,$zero
.L0f0facd4:
/*  f0facd4:	05410003 */ 	bgez	$t2,.L0f0face4
/*  f0facd8:	a3a9010c */ 	sb	$t1,0x10c($sp)
/*  f0facdc:	10000002 */ 	b	.L0f0face8
/*  f0face0:	000a1023 */ 	negu	$v0,$t2
.L0f0face4:
/*  f0face4:	01401025 */ 	or	$v0,$t2,$zero
.L0f0face8:
/*  f0face8:	05610003 */ 	bgez	$t3,.L0f0facf8
/*  f0facec:	0046082a */ 	slt	$at,$v0,$a2
/*  f0facf0:	10000002 */ 	b	.L0f0facfc
/*  f0facf4:	000b1823 */ 	negu	$v1,$t3
.L0f0facf8:
/*  f0facf8:	01601825 */ 	or	$v1,$t3,$zero
.L0f0facfc:
/*  f0facfc:	14200065 */ 	bnez	$at,.L0f0fae94
/*  f0fad00:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fad04:	14200003 */ 	bnez	$at,.L0f0fad14
/*  f0fad08:	00000000 */ 	nop
/*  f0fad0c:	52400062 */ 	beqzl	$s2,.L0f0fae98
/*  f0fad10:	8fb800fc */ 	lw	$t8,0xfc($sp)
.L0f0fad14:
/*  f0fad14:	05410006 */ 	bgez	$t2,.L0f0fad30
/*  f0fad18:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fad1c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fad20:	846f0828 */ 	lh	$t7,0x828($v1)
/*  f0fad24:	59e00003 */ 	blezl	$t7,.L0f0fad34
/*  f0fad28:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fad2c:	a4600828 */ 	sh	$zero,0x828($v1)
.L0f0fad30:
/*  f0fad30:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0fad34:
/*  f0fad34:	19400005 */ 	blez	$t2,.L0f0fad4c
/*  f0fad38:	84640828 */ 	lh	$a0,0x828($v1)
/*  f0fad3c:	04810003 */ 	bgez	$a0,.L0f0fad4c
/*  f0fad40:	00000000 */ 	nop
/*  f0fad44:	a4600828 */ 	sh	$zero,0x828($v1)
/*  f0fad48:	84640828 */ 	lh	$a0,0x828($v1)
.L0f0fad4c:
/*  f0fad4c:	14800003 */ 	bnez	$a0,.L0f0fad5c
/*  f0fad50:	00000000 */ 	nop
/*  f0fad54:	ac600824 */ 	sw	$zero,0x824($v1)
/*  f0fad58:	84640828 */ 	lh	$a0,0x828($v1)
.L0f0fad5c:
/*  f0fad5c:	54200003 */ 	bnezl	$at,.L0f0fad6c
/*  f0fad60:	00461023 */ 	subu	$v0,$v0,$a2
/*  f0fad64:	24020046 */ 	addiu	$v0,$zero,0x46
/*  f0fad68:	00461023 */ 	subu	$v0,$v0,$a2
.L0f0fad6c:
/*  f0fad6c:	02220019 */ 	multu	$s1,$v0
/*  f0fad70:	24190046 */ 	addiu	$t9,$zero,0x46
/*  f0fad74:	03267023 */ 	subu	$t6,$t9,$a2
/*  f0fad78:	0000c012 */ 	mflo	$t8
/*  f0fad7c:	00000000 */ 	nop
/*  f0fad80:	00000000 */ 	nop
/*  f0fad84:	030e001a */ 	div	$zero,$t8,$t6
/*  f0fad88:	15c00002 */ 	bnez	$t6,.L0f0fad94
/*  f0fad8c:	00000000 */ 	nop
/*  f0fad90:	0007000d */ 	break	0x7
.L0f0fad94:
/*  f0fad94:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fad98:	15c10004 */ 	bne	$t6,$at,.L0f0fadac
/*  f0fad9c:	3c018000 */ 	lui	$at,0x8000
/*  f0fada0:	17010002 */ 	bne	$t8,$at,.L0f0fadac
/*  f0fada4:	00000000 */ 	nop
/*  f0fada8:	0006000d */ 	break	0x6
.L0f0fadac:
/*  f0fadac:	00007812 */ 	mflo	$t7
/*  f0fadb0:	026f2823 */ 	subu	$a1,$s3,$t7
/*  f0fadb4:	28810003 */ 	slti	$at,$a0,0x3
/*  f0fadb8:	10200004 */ 	beqz	$at,.L0f0fadcc
/*  f0fadbc:	00a01825 */ 	or	$v1,$a1,$zero
/*  f0fadc0:	2881fffe */ 	slti	$at,$a0,-2
/*  f0fadc4:	10200005 */ 	beqz	$at,.L0f0faddc
/*  f0fadc8:	00000000 */ 	nop
.L0f0fadcc:
/*  f0fadcc:	04a10003 */ 	bgez	$a1,.L0f0faddc
/*  f0fadd0:	00051843 */ 	sra	$v1,$a1,0x1
/*  f0fadd4:	24a10001 */ 	addiu	$at,$a1,0x1
/*  f0fadd8:	00011843 */ 	sra	$v1,$at,0x1
.L0f0faddc:
/*  f0faddc:	1860002f */ 	blez	$v1,.L0f0fae9c
/*  f0fade0:	00000000 */ 	nop
/*  f0fade4:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0fade8:	3c18800a */ 	lui	$t8,%hi(g_Vars)
/*  f0fadec:	8f189fc0 */ 	lw	$t8,%lo(g_Vars)($t8)
/*  f0fadf0:	8f220824 */ 	lw	$v0,0x824($t9)
/*  f0fadf4:	24090001 */ 	addiu	$t1,$zero,0x1
/*  f0fadf8:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fadfc:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0fae00:	00003812 */ 	mflo	$a3
/*  f0fae04:	14600002 */ 	bnez	$v1,.L0f0fae10
/*  f0fae08:	00000000 */ 	nop
/*  f0fae0c:	0007000d */ 	break	0x7
.L0f0fae10:
/*  f0fae10:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fae14:	14610004 */ 	bne	$v1,$at,.L0f0fae28
/*  f0fae18:	3c018000 */ 	lui	$at,0x8000
/*  f0fae1c:	14410002 */ 	bne	$v0,$at,.L0f0fae28
/*  f0fae20:	00000000 */ 	nop
/*  f0fae24:	0006000d */ 	break	0x6
.L0f0fae28:
/*  f0fae28:	01c3001a */ 	div	$zero,$t6,$v1
/*  f0fae2c:	00006812 */ 	mflo	$t5
/*  f0fae30:	14600002 */ 	bnez	$v1,.L0f0fae3c
/*  f0fae34:	00000000 */ 	nop
/*  f0fae38:	0007000d */ 	break	0x7
.L0f0fae3c:
/*  f0fae3c:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fae40:	14610004 */ 	bne	$v1,$at,.L0f0fae54
/*  f0fae44:	3c018000 */ 	lui	$at,0x8000
/*  f0fae48:	15c10002 */ 	bne	$t6,$at,.L0f0fae54
/*  f0fae4c:	00000000 */ 	nop
/*  f0fae50:	0006000d */ 	break	0x6
.L0f0fae54:
/*  f0fae54:	05410003 */ 	bgez	$t2,.L0f0fae64
/*  f0fae58:	00000000 */ 	nop
/*  f0fae5c:	10000001 */ 	b	.L0f0fae64
/*  f0fae60:	2409ffff */ 	addiu	$t1,$zero,-1
.L0f0fae64:
/*  f0fae64:	10ed0002 */ 	beq	$a3,$t5,.L0f0fae70
/*  f0fae68:	00000000 */ 	nop
/*  f0fae6c:	241f0001 */ 	addiu	$ra,$zero,0x1
.L0f0fae70:
/*  f0fae70:	14800002 */ 	bnez	$a0,.L0f0fae7c
/*  f0fae74:	00000000 */ 	nop
/*  f0fae78:	241f0001 */ 	addiu	$ra,$zero,0x1
.L0f0fae7c:
/*  f0fae7c:	13e00007 */ 	beqz	$ra,.L0f0fae9c
/*  f0fae80:	00000000 */ 	nop
/*  f0fae84:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0fae88:	00897821 */ 	addu	$t7,$a0,$t1
/*  f0fae8c:	10000003 */ 	b	.L0f0fae9c
/*  f0fae90:	a72f0828 */ 	sh	$t7,0x828($t9)
.L0f0fae94:
/*  f0fae94:	8fb800fc */ 	lw	$t8,0xfc($sp)
.L0f0fae98:
/*  f0fae98:	a7000828 */ 	sh	$zero,0x828($t8)
.L0f0fae9c:
/*  f0fae9c:	10ed0002 */ 	beq	$a3,$t5,.L0f0faea8
/*  f0faea0:	3c19800a */ 	lui	$t9,%hi(g_Vars)
/*  f0faea4:	241f0001 */ 	addiu	$ra,$zero,0x1
.L0f0faea8:
/*  f0faea8:	17e00002 */ 	bnez	$ra,.L0f0faeb4
/*  f0faeac:	00000000 */ 	nop
/*  f0faeb0:	00004825 */ 	or	$t1,$zero,$zero
.L0f0faeb4:
/*  f0faeb4:	11000007 */ 	beqz	$t0,.L0f0faed4
/*  f0faeb8:	00006825 */ 	or	$t5,$zero,$zero
/*  f0faebc:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0faec0:	240d0001 */ 	addiu	$t5,$zero,0x1
/*  f0faec4:	a4600838 */ 	sh	$zero,0x838($v1)
/*  f0faec8:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0faecc:	10000005 */ 	b	.L0f0faee4
/*  f0faed0:	a4680836 */ 	sh	$t0,0x836($v1)
.L0f0faed4:
/*  f0faed4:	51800004 */ 	beqzl	$t4,.L0f0faee8
/*  f0faed8:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0faedc:	8fae00fc */ 	lw	$t6,0xfc($sp)
/*  f0faee0:	85cc0836 */ 	lh	$t4,0x836($t6)
.L0f0faee4:
/*  f0faee4:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0faee8:
/*  f0faee8:	8c620830 */ 	lw	$v0,0x830($v1)
/*  f0faeec:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0faef0:	14200004 */ 	bnez	$at,.L0f0faf04
/*  f0faef4:	00000000 */ 	nop
/*  f0faef8:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0faefc:	a46f0838 */ 	sh	$t7,0x838($v1)
/*  f0faf00:	8c620830 */ 	lw	$v0,0x830($v1)
.L0f0faf04:
/*  f0faf04:	0050001a */ 	div	$zero,$v0,$s0
/*  f0faf08:	8f399fc0 */ 	lw	$t9,%lo(g_Vars)($t9)
/*  f0faf0c:	00003812 */ 	mflo	$a3
/*  f0faf10:	846e0838 */ 	lh	$t6,0x838($v1)
/*  f0faf14:	0059c021 */ 	addu	$t8,$v0,$t9
/*  f0faf18:	0310001a */ 	div	$zero,$t8,$s0
/*  f0faf1c:	16000002 */ 	bnez	$s0,.L0f0faf28
/*  f0faf20:	00000000 */ 	nop
/*  f0faf24:	0007000d */ 	break	0x7
.L0f0faf28:
/*  f0faf28:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0faf2c:	16010004 */ 	bne	$s0,$at,.L0f0faf40
/*  f0faf30:	3c018000 */ 	lui	$at,0x8000
/*  f0faf34:	14410002 */ 	bne	$v0,$at,.L0f0faf40
/*  f0faf38:	00000000 */ 	nop
/*  f0faf3c:	0006000d */ 	break	0x6
.L0f0faf40:
/*  f0faf40:	00004012 */ 	mflo	$t0
/*  f0faf44:	01601025 */ 	or	$v0,$t3,$zero
/*  f0faf48:	16000002 */ 	bnez	$s0,.L0f0faf54
/*  f0faf4c:	00000000 */ 	nop
/*  f0faf50:	0007000d */ 	break	0x7
.L0f0faf54:
/*  f0faf54:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0faf58:	16010004 */ 	bne	$s0,$at,.L0f0faf6c
/*  f0faf5c:	3c018000 */ 	lui	$at,0x8000
/*  f0faf60:	17010002 */ 	bne	$t8,$at,.L0f0faf6c
/*  f0faf64:	00000000 */ 	nop
/*  f0faf68:	0006000d */ 	break	0x6
.L0f0faf6c:
/*  f0faf6c:	15c0000b */ 	bnez	$t6,.L0f0faf9c
/*  f0faf70:	00000000 */ 	nop
/*  f0faf74:	04e10003 */ 	bgez	$a3,.L0f0faf84
/*  f0faf78:	00077843 */ 	sra	$t7,$a3,0x1
/*  f0faf7c:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0faf80:	00017843 */ 	sra	$t7,$at,0x1
.L0f0faf84:
/*  f0faf84:	01e03825 */ 	or	$a3,$t7,$zero
/*  f0faf88:	05010003 */ 	bgez	$t0,.L0f0faf98
/*  f0faf8c:	0008c843 */ 	sra	$t9,$t0,0x1
/*  f0faf90:	25010001 */ 	addiu	$at,$t0,0x1
/*  f0faf94:	0001c843 */ 	sra	$t9,$at,0x1
.L0f0faf98:
/*  f0faf98:	03204025 */ 	or	$t0,$t9,$zero
.L0f0faf9c:
/*  f0faf9c:	05610003 */ 	bgez	$t3,.L0f0fafac
/*  f0fafa0:	a3ac010d */ 	sb	$t4,0x10d($sp)
/*  f0fafa4:	10000001 */ 	b	.L0f0fafac
/*  f0fafa8:	000b1023 */ 	negu	$v0,$t3
.L0f0fafac:
/*  f0fafac:	05410003 */ 	bgez	$t2,.L0f0fafbc
/*  f0fafb0:	28410014 */ 	slti	$at,$v0,0x14
/*  f0fafb4:	10000002 */ 	b	.L0f0fafc0
/*  f0fafb8:	000a1823 */ 	negu	$v1,$t2
.L0f0fafbc:
/*  f0fafbc:	01401825 */ 	or	$v1,$t2,$zero
.L0f0fafc0:
/*  f0fafc0:	1420005d */ 	bnez	$at,.L0f0fb138
/*  f0fafc4:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fafc8:	14200003 */ 	bnez	$at,.L0f0fafd8
/*  f0fafcc:	3c06800a */ 	lui	$a2,%hi(g_Vars)
/*  f0fafd0:	5240005a */ 	beqzl	$s2,.L0f0fb13c
/*  f0fafd4:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0fafd8:
/*  f0fafd8:	05610006 */ 	bgez	$t3,.L0f0faff4
/*  f0fafdc:	28410047 */ 	slti	$at,$v0,0x47
/*  f0fafe0:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fafe4:	84780834 */ 	lh	$t8,0x834($v1)
/*  f0fafe8:	07030003 */ 	bgezl	$t8,.L0f0faff8
/*  f0fafec:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0faff0:	a4600834 */ 	sh	$zero,0x834($v1)
.L0f0faff4:
/*  f0faff4:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0faff8:
/*  f0faff8:	19600005 */ 	blez	$t3,.L0f0fb010
/*  f0faffc:	84640834 */ 	lh	$a0,0x834($v1)
/*  f0fb000:	18800003 */ 	blez	$a0,.L0f0fb010
/*  f0fb004:	00000000 */ 	nop
/*  f0fb008:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb00c:	84640834 */ 	lh	$a0,0x834($v1)
.L0f0fb010:
/*  f0fb010:	14800003 */ 	bnez	$a0,.L0f0fb020
/*  f0fb014:	00000000 */ 	nop
/*  f0fb018:	ac600830 */ 	sw	$zero,0x830($v1)
/*  f0fb01c:	84640834 */ 	lh	$a0,0x834($v1)
.L0f0fb020:
/*  f0fb020:	14200002 */ 	bnez	$at,.L0f0fb02c
/*  f0fb024:	8cc69fc0 */ 	lw	$a2,%lo(g_Vars)($a2)
/*  f0fb028:	24020046 */ 	addiu	$v0,$zero,0x46
.L0f0fb02c:
/*  f0fb02c:	2442ffec */ 	addiu	$v0,$v0,-20
/*  f0fb030:	02820019 */ 	multu	$s4,$v0
/*  f0fb034:	24010032 */ 	addiu	$at,$zero,0x32
/*  f0fb038:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0fb03c:	00007012 */ 	mflo	$t6
/*  f0fb040:	00000000 */ 	nop
/*  f0fb044:	00000000 */ 	nop
/*  f0fb048:	01c1001a */ 	div	$zero,$t6,$at
/*  f0fb04c:	00007812 */ 	mflo	$t7
/*  f0fb050:	026f2823 */ 	subu	$a1,$s3,$t7
/*  f0fb054:	28810003 */ 	slti	$at,$a0,0x3
/*  f0fb058:	10200004 */ 	beqz	$at,.L0f0fb06c
/*  f0fb05c:	00a01825 */ 	or	$v1,$a1,$zero
/*  f0fb060:	2881fffe */ 	slti	$at,$a0,-2
/*  f0fb064:	10200006 */ 	beqz	$at,.L0f0fb080
/*  f0fb068:	00000000 */ 	nop
.L0f0fb06c:
/*  f0fb06c:	24010003 */ 	addiu	$at,$zero,0x3
/*  f0fb070:	00a1001a */ 	div	$zero,$a1,$at
/*  f0fb074:	00001812 */ 	mflo	$v1
/*  f0fb078:	00000000 */ 	nop
/*  f0fb07c:	00000000 */ 	nop
.L0f0fb080:
/*  f0fb080:	5860002b */ 	blezl	$v1,.L0f0fb130
/*  f0fb084:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb088:	8f220830 */ 	lw	$v0,0x830($t9)
/*  f0fb08c:	240c0001 */ 	addiu	$t4,$zero,0x1
/*  f0fb090:	0043001a */ 	div	$zero,$v0,$v1
/*  f0fb094:	0046c021 */ 	addu	$t8,$v0,$a2
/*  f0fb098:	00003812 */ 	mflo	$a3
/*  f0fb09c:	14600002 */ 	bnez	$v1,.L0f0fb0a8
/*  f0fb0a0:	00000000 */ 	nop
/*  f0fb0a4:	0007000d */ 	break	0x7
.L0f0fb0a8:
/*  f0fb0a8:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fb0ac:	14610004 */ 	bne	$v1,$at,.L0f0fb0c0
/*  f0fb0b0:	3c018000 */ 	lui	$at,0x8000
/*  f0fb0b4:	14410002 */ 	bne	$v0,$at,.L0f0fb0c0
/*  f0fb0b8:	00000000 */ 	nop
/*  f0fb0bc:	0006000d */ 	break	0x6
.L0f0fb0c0:
/*  f0fb0c0:	0303001a */ 	div	$zero,$t8,$v1
/*  f0fb0c4:	00004012 */ 	mflo	$t0
/*  f0fb0c8:	14600002 */ 	bnez	$v1,.L0f0fb0d4
/*  f0fb0cc:	00000000 */ 	nop
/*  f0fb0d0:	0007000d */ 	break	0x7
.L0f0fb0d4:
/*  f0fb0d4:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0fb0d8:	14610004 */ 	bne	$v1,$at,.L0f0fb0ec
/*  f0fb0dc:	3c018000 */ 	lui	$at,0x8000
/*  f0fb0e0:	17010002 */ 	bne	$t8,$at,.L0f0fb0ec
/*  f0fb0e4:	00000000 */ 	nop
/*  f0fb0e8:	0006000d */ 	break	0x6
.L0f0fb0ec:
/*  f0fb0ec:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb0f0:	19600003 */ 	blez	$t3,.L0f0fb100
/*  f0fb0f4:	00000000 */ 	nop
/*  f0fb0f8:	10000001 */ 	b	.L0f0fb100
/*  f0fb0fc:	240cffff */ 	addiu	$t4,$zero,-1
.L0f0fb100:
/*  f0fb100:	10e80002 */ 	beq	$a3,$t0,.L0f0fb10c
/*  f0fb104:	008c7021 */ 	addu	$t6,$a0,$t4
/*  f0fb108:	240d0001 */ 	addiu	$t5,$zero,0x1
.L0f0fb10c:
/*  f0fb10c:	14800002 */ 	bnez	$a0,.L0f0fb118
/*  f0fb110:	00000000 */ 	nop
/*  f0fb114:	240d0001 */ 	addiu	$t5,$zero,0x1
.L0f0fb118:
/*  f0fb118:	51a00005 */ 	beqzl	$t5,.L0f0fb130
/*  f0fb11c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0fb120:	a46e0834 */ 	sh	$t6,0x834($v1)
/*  f0fb124:	3c06800a */ 	lui	$a2,%hi(g_Vars)
/*  f0fb128:	8cc69fc0 */ 	lw	$a2,%lo(g_Vars)($a2)
/*  f0fb12c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0fb130:
/*  f0fb130:	10000006 */ 	b	.L0f0fb14c
/*  f0fb134:	8c620830 */ 	lw	$v0,0x830($v1)
.L0f0fb138:
/*  f0fb138:	8fa300fc */ 	lw	$v1,0xfc($sp)
.L0f0fb13c:
/*  f0fb13c:	3c06800a */ 	lui	$a2,%hi(g_Vars)
/*  f0fb140:	a4600834 */ 	sh	$zero,0x834($v1)
/*  f0fb144:	8cc69fc0 */ 	lw	$a2,%lo(g_Vars)($a2)
/*  f0fb148:	8c620830 */ 	lw	$v0,0x830($v1)
.L0f0fb14c:
/*  f0fb14c:	10e80002 */ 	beq	$a3,$t0,.L0f0fb158
/*  f0fb150:	27b50104 */ 	addiu	$s5,$sp,0x104
/*  f0fb154:	240d0001 */ 	addiu	$t5,$zero,0x1
.L0f0fb158:
/*  f0fb158:	55a00003 */ 	bnezl	$t5,.L0f0fb168
/*  f0fb15c:	8c6f0824 */ 	lw	$t7,0x824($v1)
/*  f0fb160:	00006025 */ 	or	$t4,$zero,$zero
/*  f0fb164:	8c6f0824 */ 	lw	$t7,0x824($v1)
.L0f0fb168:
/*  f0fb168:	8fa500f8 */ 	lw	$a1,0xf8($sp)
/*  f0fb16c:	3c18800a */ 	lui	$t8,%hi(g_Vars)
/*  f0fb170:	01e6c821 */ 	addu	$t9,$t7,$a2
/*  f0fb174:	ac790824 */ 	sw	$t9,0x824($v1)
/*  f0fb178:	8f189fc0 */ 	lw	$t8,%lo(g_Vars)($t8)
/*  f0fb17c:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0fb180:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0fb184:	ac6e0830 */ 	sw	$t6,0x830($v1)
/*  f0fb188:	a3a90104 */ 	sb	$t1,0x104($sp)
/*  f0fb18c:	a3ac0105 */ 	sb	$t4,0x105($sp)
/*  f0fb190:	a3aa0108 */ 	sb	$t2,0x108($sp)
/*  f0fb194:	a3ab0109 */ 	sb	$t3,0x109($sp)
/*  f0fb198:	10a00003 */ 	beqz	$a1,.L0f0fb1a8
/*  f0fb19c:	a3a00118 */ 	sb	$zero,0x118($sp)
/*  f0fb1a0:	10000002 */ 	b	.L0f0fb1ac
/*  f0fb1a4:	a3af010e */ 	sb	$t7,0x10e($sp)
.L0f0fb1a8:
/*  f0fb1a8:	a3a0010e */ 	sb	$zero,0x10e($sp)
.L0f0fb1ac:
/*  f0fb1ac:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fb1b0:	007e0019 */ 	multu	$v1,$s8
/*  f0fb1b4:	0000c812 */ 	mflo	$t9
/*  f0fb1b8:	02d93821 */ 	addu	$a3,$s6,$t9
/*  f0fb1bc:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fb1c0:	50800016 */ 	beqzl	$a0,.L0f0fb21c
/*  f0fb1c4:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb1c8:	50a00014 */ 	beqzl	$a1,.L0f0fb21c
/*  f0fb1cc:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb1d0:	8c980000 */ 	lw	$t8,0x0($a0)
/*  f0fb1d4:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0fb1d8:	8f0e0010 */ 	lw	$t6,0x10($t8)
/*  f0fb1dc:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0fb1e0:	31cf0004 */ 	andi	$t7,$t6,0x4
/*  f0fb1e4:	51e00004 */ 	beqzl	$t7,.L0f0fb1f8
/*  f0fb1e8:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0fb1ec:	a3b90106 */ 	sb	$t9,0x106($sp)
/*  f0fb1f0:	afb8011c */ 	sw	$t8,0x11c($sp)
/*  f0fb1f4:	8c820008 */ 	lw	$v0,0x8($a0)
.L0f0fb1f8:
/*  f0fb1f8:	50400008 */ 	beqzl	$v0,.L0f0fb21c
/*  f0fb1fc:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb200:	904e0000 */ 	lbu	$t6,0x0($v0)
/*  f0fb204:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0fb208:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0fb20c:	55c10003 */ 	bnel	$t6,$at,.L0f0fb21c
/*  f0fb210:	84e204f4 */ 	lh	$v0,0x4f4($a3)
/*  f0fb214:	a3af0106 */ 	sb	$t7,0x106($sp)
/*  f0fb218:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.L0f0fb21c:
/*  f0fb21c:	00009825 */ 	or	$s3,$zero,$zero
/*  f0fb220:	0000a025 */ 	or	$s4,$zero,$zero
/*  f0fb224:	1840003a */ 	blez	$v0,.L0f0fb310
/*  f0fb228:	00000000 */ 	nop
/*  f0fb22c:	afa00044 */ 	sw	$zero,0x44($sp)
.L0f0fb230:
/*  f0fb230:	0003c8c0 */ 	sll	$t9,$v1,0x3
/*  f0fb234:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb238:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fb23c:	0323c821 */ 	addu	$t9,$t9,$v1
/*  f0fb240:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fb244:	8fae0044 */ 	lw	$t6,0x44($sp)
/*  f0fb248:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb24c:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fb250:	02d9c021 */ 	addu	$t8,$s6,$t9
/*  f0fb254:	030e9021 */ 	addu	$s2,$t8,$t6
/*  f0fb258:	824f0478 */ 	lb	$t7,0x478($s2)
/*  f0fb25c:	26520464 */ 	addiu	$s2,$s2,0x464
/*  f0fb260:	02408825 */ 	or	$s1,$s2,$zero
/*  f0fb264:	19e00024 */ 	blez	$t7,.L0f0fb2f8
/*  f0fb268:	00008025 */ 	or	$s0,$zero,$zero
/*  f0fb26c:	8ef80000 */ 	lw	$t8,0x0($s7)
.L0f0fb270:
/*  f0fb270:	26990001 */ 	addiu	$t9,$s4,0x1
/*  f0fb274:	00003025 */ 	or	$a2,$zero,$zero
/*  f0fb278:	031e0019 */ 	multu	$t8,$s8
/*  f0fb27c:	02a02825 */ 	or	$a1,$s5,$zero
/*  f0fb280:	00007012 */ 	mflo	$t6
/*  f0fb284:	02ce7821 */ 	addu	$t7,$s6,$t6
/*  f0fb288:	85f804f4 */ 	lh	$t8,0x4f4($t7)
/*  f0fb28c:	17380008 */ 	bne	$t9,$t8,.L0f0fb2b0
/*  f0fb290:	00000000 */ 	nop
/*  f0fb294:	824e0015 */ 	lb	$t6,0x15($s2)
/*  f0fb298:	160e0005 */ 	bne	$s0,$t6,.L0f0fb2b0
/*  f0fb29c:	00000000 */ 	nop
/*  f0fb2a0:	16600003 */ 	bnez	$s3,.L0f0fb2b0
/*  f0fb2a4:	00000000 */ 	nop
/*  f0fb2a8:	24060001 */ 	addiu	$a2,$zero,0x1
/*  f0fb2ac:	24130001 */ 	addiu	$s3,$zero,0x1
.L0f0fb2b0:
/*  f0fb2b0:	0fc3e4d7 */ 	jal	dialogTick
/*  f0fb2b4:	8e240000 */ 	lw	$a0,0x0($s1)
/*  f0fb2b8:	824f0014 */ 	lb	$t7,0x14($s2)
/*  f0fb2bc:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0fb2c0:	26310004 */ 	addiu	$s1,$s1,0x4
/*  f0fb2c4:	020f082a */ 	slt	$at,$s0,$t7
/*  f0fb2c8:	5420ffe9 */ 	bnezl	$at,.L0f0fb270
/*  f0fb2cc:	8ef80000 */ 	lw	$t8,0x0($s7)
/*  f0fb2d0:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0fb2d4:	0003c8c0 */ 	sll	$t9,$v1,0x3
/*  f0fb2d8:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb2dc:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fb2e0:	0323c821 */ 	addu	$t9,$t9,$v1
/*  f0fb2e4:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fb2e8:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0fb2ec:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fb2f0:	02d93821 */ 	addu	$a3,$s6,$t9
/*  f0fb2f4:	84e204f4 */ 	lh	$v0,0x4f4($a3)
.L0f0fb2f8:
/*  f0fb2f8:	8fb80044 */ 	lw	$t8,0x44($sp)
/*  f0fb2fc:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0fb300:	0282082a */ 	slt	$at,$s4,$v0
/*  f0fb304:	270e0018 */ 	addiu	$t6,$t8,0x18
/*  f0fb308:	1420ffc9 */ 	bnez	$at,.L0f0fb230
/*  f0fb30c:	afae0044 */ 	sw	$t6,0x44($sp)
.L0f0fb310:
/*  f0fb310:	3c0f800a */ 	lui	$t7,%hi(g_MenuData+0x4)
/*  f0fb314:	8def19c4 */ 	lw	$t7,%lo(g_MenuData+0x4)($t7)
/*  f0fb318:	25f9ffff */ 	addiu	$t9,$t7,-1
/*  f0fb31c:	2f21000d */ 	sltiu	$at,$t9,0xd
/*  f0fb320:	1020004d */ 	beqz	$at,.L0f0fb458
/*  f0fb324:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fb328:	3c017f1b */ 	lui	$at,%hi(var7f1b2a98)
/*  f0fb32c:	00390821 */ 	addu	$at,$at,$t9
/*  f0fb330:	8c392a98 */ 	lw	$t9,%lo(var7f1b2a98)($at)
/*  f0fb334:	03200008 */ 	jr	$t9
/*  f0fb338:	00000000 */ 	nop
/*  f0fb33c:	83b8010e */ 	lb	$t8,0x10e($sp)
/*  f0fb340:	8fae011c */ 	lw	$t6,0x11c($sp)
/*  f0fb344:	53000045 */ 	beqzl	$t8,.L0f0fb45c
/*  f0fb348:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb34c:	55c00043 */ 	bnezl	$t6,.L0f0fb45c
/*  f0fb350:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb354:	8ce404f8 */ 	lw	$a0,0x4f8($a3)
/*  f0fb358:	8faf0100 */ 	lw	$t7,0x100($sp)
/*  f0fb35c:	5080003f */ 	beqzl	$a0,.L0f0fb45c
/*  f0fb360:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb364:	8df9000c */ 	lw	$t9,0xc($t7)
/*  f0fb368:	3c18800a */ 	lui	$t8,%hi(g_Vars+0x490)
/*  f0fb36c:	5720003b */ 	bnezl	$t9,.L0f0fb45c
/*  f0fb370:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb374:	8f18a450 */ 	lw	$t8,%lo(g_Vars+0x490)($t8)
/*  f0fb378:	8c820000 */ 	lw	$v0,0x0($a0)
/*  f0fb37c:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0fb380:	13010008 */ 	beq	$t8,$at,.L0f0fb3a4
/*  f0fb384:	3c048008 */ 	lui	$a0,%hi(g_MpReadyMenuDialog)
/*  f0fb388:	248457b8 */ 	addiu	$a0,$a0,%lo(g_MpReadyMenuDialog)
/*  f0fb38c:	10820005 */ 	beq	$a0,$v0,.L0f0fb3a4
/*  f0fb390:	00000000 */ 	nop
/*  f0fb394:	0fc3cbd3 */ 	jal	menuPushDialog
/*  f0fb398:	00000000 */ 	nop
/*  f0fb39c:	1000002f */ 	b	.L0f0fb45c
/*  f0fb3a0:	8fbf003c */ 	lw	$ra,0x3c($sp)
.L0f0fb3a4:
/*  f0fb3a4:	3c0e8008 */ 	lui	$t6,%hi(g_MpQuickTeamGameSetupMenuDialog)
/*  f0fb3a8:	25ce6680 */ 	addiu	$t6,$t6,%lo(g_MpQuickTeamGameSetupMenuDialog)
/*  f0fb3ac:	55c2002b */ 	bnel	$t6,$v0,.L0f0fb45c
/*  f0fb3b0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb3b4:	0fc5fd0a */ 	jal	func0f17f428
/*  f0fb3b8:	00000000 */ 	nop
/*  f0fb3bc:	10000027 */ 	b	.L0f0fb45c
/*  f0fb3c0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb3c4:	3c0f8007 */ 	lui	$t7,%hi(g_InCutscene)
/*  f0fb3c8:	8def0764 */ 	lw	$t7,%lo(g_InCutscene)($t7)
/*  f0fb3cc:	51e00010 */ 	beqzl	$t7,.L0f0fb410
/*  f0fb3d0:	240f000a */ 	addiu	$t7,$zero,0xa
/*  f0fb3d4:	0fc3e048 */ 	jal	func0f0f8120
/*  f0fb3d8:	00000000 */ 	nop
/*  f0fb3dc:	3c198007 */ 	lui	$t9,%hi(g_MpPlayerNum)
/*  f0fb3e0:	8f391448 */ 	lw	$t9,%lo(g_MpPlayerNum)($t9)
/*  f0fb3e4:	3c0e800a */ 	lui	$t6,%hi(g_Menus)
/*  f0fb3e8:	25cee000 */ 	addiu	$t6,$t6,%lo(g_Menus)
/*  f0fb3ec:	0019c0c0 */ 	sll	$t8,$t9,0x3
/*  f0fb3f0:	0319c023 */ 	subu	$t8,$t8,$t9
/*  f0fb3f4:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0fb3f8:	0319c021 */ 	addu	$t8,$t8,$t9
/*  f0fb3fc:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0fb400:	0319c023 */ 	subu	$t8,$t8,$t9
/*  f0fb404:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0fb408:	030e3821 */ 	addu	$a3,$t8,$t6
/*  f0fb40c:	240f000a */ 	addiu	$t7,$zero,0xa
.L0f0fb410:
/*  f0fb410:	a0ef083c */ 	sb	$t7,0x83c($a3)
/*  f0fb414:	83b9010e */ 	lb	$t9,0x10e($sp)
/*  f0fb418:	8fb8011c */ 	lw	$t8,0x11c($sp)
/*  f0fb41c:	5320000f */ 	beqzl	$t9,.L0f0fb45c
/*  f0fb420:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb424:	5700000d */ 	bnezl	$t8,.L0f0fb45c
/*  f0fb428:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb42c:	8cee04f8 */ 	lw	$t6,0x4f8($a3)
/*  f0fb430:	8faf0100 */ 	lw	$t7,0x100($sp)
/*  f0fb434:	51c00009 */ 	beqzl	$t6,.L0f0fb45c
/*  f0fb438:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb43c:	8df90000 */ 	lw	$t9,0x0($t7)
/*  f0fb440:	8f380010 */ 	lw	$t8,0x10($t9)
/*  f0fb444:	330e0020 */ 	andi	$t6,$t8,0x20
/*  f0fb448:	55c00004 */ 	bnezl	$t6,.L0f0fb45c
/*  f0fb44c:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0fb450:	0fc3e048 */ 	jal	func0f0f8120
/*  f0fb454:	00000000 */ 	nop
.L0f0fb458:
/*  f0fb458:	8fbf003c */ 	lw	$ra,0x3c($sp)
.L0f0fb45c:
/*  f0fb45c:	8fb00018 */ 	lw	$s0,0x18($sp)
/*  f0fb460:	8fb1001c */ 	lw	$s1,0x1c($sp)
/*  f0fb464:	8fb20020 */ 	lw	$s2,0x20($sp)
/*  f0fb468:	8fb30024 */ 	lw	$s3,0x24($sp)
/*  f0fb46c:	8fb40028 */ 	lw	$s4,0x28($sp)
/*  f0fb470:	8fb5002c */ 	lw	$s5,0x2c($sp)
/*  f0fb474:	8fb60030 */ 	lw	$s6,0x30($sp)
/*  f0fb478:	8fb70034 */ 	lw	$s7,0x34($sp)
/*  f0fb47c:	8fbe0038 */ 	lw	$s8,0x38($sp)
/*  f0fb480:	03e00008 */ 	jr	$ra
/*  f0fb484:	27bd0130 */ 	addiu	$sp,$sp,0x130
);
#else
GLOBAL_ASM(
glabel menuProcessInput
.late_rodata
glabel var7f1b2a98
.word menuProcessInput+0xd10
glabel var7f1b2a9c
.word menuProcessInput+0xd10
glabel var7f1b2aa0
.word menuProcessInput+0xc38
glabel var7f1b2aa4
.word menuProcessInput+0xcc0
glabel var7f1b2aa8
.word menuProcessInput+0xd10
glabel var7f1b2aac
.word menuProcessInput+0xd54
glabel var7f1b2ab0
.word menuProcessInput+0xd54
glabel var7f1b2ab4
.word menuProcessInput+0xd54
glabel var7f1b2ab8
.word menuProcessInput+0xd54
glabel var7f1b2abc
.word menuProcessInput+0xd54
glabel var7f1b2ac0
.word menuProcessInput+0xc38
glabel var7f1b2ac4
.word menuProcessInput+0xd54
glabel var7f1b2ac8
.word menuProcessInput+0xd10
.text
/*  f0f6e54:	27bdfed0 */ 	addiu	$sp,$sp,-304
/*  f0f6e58:	afb70034 */ 	sw	$s7,0x34($sp)
/*  f0f6e5c:	3c178007 */ 	lui	$s7,0x8007
/*  f0f6e60:	26f73af0 */ 	addiu	$s7,$s7,0x3af0
/*  f0f6e64:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0f6e68:	afbe0038 */ 	sw	$s8,0x38($sp)
/*  f0f6e6c:	241e0dbc */ 	addiu	$s8,$zero,0xdbc
/*  f0f6e70:	01de0019 */ 	multu	$t6,$s8
/*  f0f6e74:	afb60030 */ 	sw	$s6,0x30($sp)
/*  f0f6e78:	3c16800a */ 	lui	$s6,0x800a
/*  f0f6e7c:	26d627c0 */ 	addiu	$s6,$s6,0x27c0
/*  f0f6e80:	3c19800a */ 	lui	$t9,0x800a
/*  f0f6e84:	8f395eb4 */ 	lw	$t9,0x5eb4($t9)
/*  f0f6e88:	afb5002c */ 	sw	$s5,0x2c($sp)
/*  f0f6e8c:	24010004 */ 	addiu	$at,$zero,0x4
/*  f0f6e90:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0f6e94:	afb40028 */ 	sw	$s4,0x28($sp)
/*  f0f6e98:	00007812 */ 	mflo	$t7
/*  f0f6e9c:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0f6ea0:	8cf80488 */ 	lw	$t8,0x488($a3)
/*  f0f6ea4:	afb30024 */ 	sw	$s3,0x24($sp)
/*  f0f6ea8:	afb20020 */ 	sw	$s2,0x20($sp)
/*  f0f6eac:	afb1001c */ 	sw	$s1,0x1c($sp)
/*  f0f6eb0:	afb00018 */ 	sw	$s0,0x18($sp)
/*  f0f6eb4:	00006025 */ 	or	$t4,$zero,$zero
/*  f0f6eb8:	00004825 */ 	or	$t1,$zero,$zero
/*  f0f6ebc:	00004025 */ 	or	$t0,$zero,$zero
/*  f0f6ec0:	0000a825 */ 	or	$s5,$zero,$zero
/*  f0f6ec4:	afa0011c */ 	sw	$zero,0x11c($sp)
/*  f0f6ec8:	afa700fc */ 	sw	$a3,0xfc($sp)
/*  f0f6ecc:	17210005 */ 	bne	$t9,$at,.NB0f0f6ee4
/*  f0f6ed0:	afb80100 */ 	sw	$t8,0x100($sp)
/*  f0f6ed4:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f6ed8:	8dcee94c */ 	lw	$t6,-0x16b4($t6)
/*  f0f6edc:	3c01800a */ 	lui	$at,0x800a
/*  f0f6ee0:	ac2e66a8 */ 	sw	$t6,0x66a8($at)
.NB0f0f6ee4:
/*  f0f6ee4:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0f6ee8:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0f6eec:	0fc3b809 */ 	jal	func0f0f1418
/*  f0f6ef0:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0f6ef4:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0f6ef8:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0f6efc:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0f6f00:	01fe0019 */ 	multu	$t7,$s8
/*  f0f6f04:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0f6f08:	a3a00106 */ 	sb	$zero,0x106($sp)
/*  f0f6f0c:	a3a00107 */ 	sb	$zero,0x107($sp)
/*  f0f6f10:	a3a0010a */ 	sb	$zero,0x10a($sp)
/*  f0f6f14:	a3a0010b */ 	sb	$zero,0x10b($sp)
/*  f0f6f18:	00005025 */ 	or	$t2,$zero,$zero
/*  f0f6f1c:	00005825 */ 	or	$t3,$zero,$zero
/*  f0f6f20:	3c0f800a */ 	lui	$t7,0x800a
/*  f0f6f24:	0000c012 */ 	mflo	$t8
/*  f0f6f28:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0f6f2c:	8f2e0488 */ 	lw	$t6,0x488($t9)
/*  f0f6f30:	51c0031e */ 	beqzl	$t6,.NB0f0f7bac
/*  f0f6f34:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f6f38:	8def5eb4 */ 	lw	$t7,0x5eb4($t7)
/*  f0f6f3c:	24010007 */ 	addiu	$at,$zero,0x7
/*  f0f6f40:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*  f0f6f44:	15e1000b */ 	bne	$t7,$at,.NB0f0f6f74
/*  f0f6f48:	afa000ec */ 	sw	$zero,0xec($sp)
/*  f0f6f4c:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f6f50:	24190002 */ 	addiu	$t9,$zero,0x2
/*  f0f6f54:	240e0003 */ 	addiu	$t6,$zero,0x3
/*  f0f6f58:	240f0004 */ 	addiu	$t7,$zero,0x4
/*  f0f6f5c:	afa000d8 */ 	sw	$zero,0xd8($sp)
/*  f0f6f60:	afb800dc */ 	sw	$t8,0xdc($sp)
/*  f0f6f64:	afb900e0 */ 	sw	$t9,0xe0($sp)
/*  f0f6f68:	afae00e4 */ 	sw	$t6,0xe4($sp)
/*  f0f6f6c:	1000001b */ 	beqz	$zero,.NB0f0f6fdc
/*  f0f6f70:	afaf00ec */ 	sw	$t7,0xec($sp)
.NB0f0f6f74:
/*  f0f6f74:	27a400d7 */ 	addiu	$a0,$sp,0xd7
/*  f0f6f78:	27a500d6 */ 	addiu	$a1,$sp,0xd6
/*  f0f6f7c:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0f6f80:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0f6f84:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0f6f88:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0f6f8c:	0fc3cf88 */ 	jal	menuGetContPads
/*  f0f6f90:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0f6f94:	83a200d7 */ 	lb	$v0,0xd7($sp)
/*  f0f6f98:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0f6f9c:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0f6fa0:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0f6fa4:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0f6fa8:	04400004 */ 	bltz	$v0,.NB0f0f6fbc
/*  f0f6fac:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0f6fb0:	24030001 */ 	addiu	$v1,$zero,0x1
/*  f0f6fb4:	afa200d8 */ 	sw	$v0,0xd8($sp)
/*  f0f6fb8:	afa300ec */ 	sw	$v1,0xec($sp)
.NB0f0f6fbc:
/*  f0f6fbc:	83a200d6 */ 	lb	$v0,0xd6($sp)
/*  f0f6fc0:	8fa300ec */ 	lw	$v1,0xec($sp)
/*  f0f6fc4:	04400005 */ 	bltz	$v0,.NB0f0f6fdc
/*  f0f6fc8:	0003c080 */ 	sll	$t8,$v1,0x2
/*  f0f6fcc:	03b8c821 */ 	addu	$t9,$sp,$t8
/*  f0f6fd0:	af2200d8 */ 	sw	$v0,0xd8($t9)
/*  f0f6fd4:	24630001 */ 	addiu	$v1,$v1,0x1
/*  f0f6fd8:	afa300ec */ 	sw	$v1,0xec($sp)
.NB0f0f6fdc:
/*  f0f6fdc:	8eee0000 */ 	lw	$t6,0x0($s7)
/*  f0f6fe0:	01de0019 */ 	multu	$t6,$s8
/*  f0f6fe4:	00007812 */ 	mflo	$t7
/*  f0f6fe8:	02cf3821 */ 	addu	$a3,$s6,$t7
/*  f0f6fec:	90e20d9d */ 	lbu	$v0,0xd9d($a3)
/*  f0f6ff0:	1840001a */ 	blez	$v0,.NB0f0f705c
/*  f0f6ff4:	2458ffff */ 	addiu	$t8,$v0,-1
/*  f0f6ff8:	331900ff */ 	andi	$t9,$t8,0xff
/*  f0f6ffc:	17200014 */ 	bnez	$t9,.NB0f0f7050
/*  f0f7000:	a0f80d9d */ 	sb	$t8,0xd9d($a3)
/*  f0f7004:	8cee0da4 */ 	lw	$t6,0xda4($a3)
/*  f0f7008:	8cef0da8 */ 	lw	$t7,0xda8($a3)
/*  f0f700c:	27a400cc */ 	addiu	$a0,$sp,0xcc
/*  f0f7010:	2405ffff */ 	addiu	$a1,$zero,-1
/*  f0f7014:	00003025 */ 	or	$a2,$zero,$zero
/*  f0f7018:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0f701c:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0f7020:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0f7024:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0f7028:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0f702c:	afae00cc */ 	sw	$t6,0xcc($sp)
/*  f0f7030:	0fc41358 */ 	jal	filemgrSaveOrLoad
/*  f0f7034:	a7af00d0 */ 	sh	$t7,0xd0($sp)
/*  f0f7038:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0f703c:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0f7040:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0f7044:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0f7048:	10000004 */ 	beqz	$zero,.NB0f0f705c
/*  f0f704c:	8fac012c */ 	lw	$t4,0x12c($sp)
.NB0f0f7050:
/*  f0f7050:	90f90d9c */ 	lbu	$t9,0xd9c($a3)
/*  f0f7054:	372e0080 */ 	ori	$t6,$t9,0x80
/*  f0f7058:	a0ee0d9c */ 	sb	$t6,0xd9c($a3)
.NB0f0f705c:
/*  f0f705c:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0f7060:	0000a025 */ 	or	$s4,$zero,$zero
/*  f0f7064:	19e00083 */ 	blez	$t7,.NB0f0f7274
/*  f0f7068:	27b300d8 */ 	addiu	$s3,$sp,0xd8
.NB0f0f706c:
/*  f0f706c:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0f7070:	afac012c */ 	sw	$t4,0x12c($sp)
/*  f0f7074:	afab00f0 */ 	sw	$t3,0xf0($sp)
/*  f0f7078:	afaa00f4 */ 	sw	$t2,0xf4($sp)
/*  f0f707c:	afa90128 */ 	sw	$t1,0x128($sp)
/*  f0f7080:	0c00573c */ 	jal	joyGetStickX
/*  f0f7084:	afa80124 */ 	sw	$t0,0x124($sp)
/*  f0f7088:	00028600 */ 	sll	$s0,$v0,0x18
/*  f0f708c:	0010c603 */ 	sra	$t8,$s0,0x18
/*  f0f7090:	03008025 */ 	or	$s0,$t8,$zero
/*  f0f7094:	0c005766 */ 	jal	joyGetStickY
/*  f0f7098:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0f709c:	00028e00 */ 	sll	$s1,$v0,0x18
/*  f0f70a0:	0011ce03 */ 	sra	$t9,$s1,0x18
/*  f0f70a4:	03208825 */ 	or	$s1,$t9,$zero
/*  f0f70a8:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0f70ac:	0c005790 */ 	jal	joyGetButtons
/*  f0f70b0:	3405ffff */ 	dli	$a1,0xffff
/*  f0f70b4:	3052ffff */ 	andi	$s2,$v0,0xffff
/*  f0f70b8:	82640003 */ 	lb	$a0,0x3($s3)
/*  f0f70bc:	0c0057c0 */ 	jal	joyGetButtonsPressedThisFrame
/*  f0f70c0:	3405ffff */ 	dli	$a1,0xffff
/*  f0f70c4:	304e8000 */ 	andi	$t6,$v0,0x8000
/*  f0f70c8:	8fa80124 */ 	lw	$t0,0x124($sp)
/*  f0f70cc:	8fa90128 */ 	lw	$t1,0x128($sp)
/*  f0f70d0:	8faa00f4 */ 	lw	$t2,0xf4($sp)
/*  f0f70d4:	8fab00f0 */ 	lw	$t3,0xf0($sp)
/*  f0f70d8:	8fac012c */ 	lw	$t4,0x12c($sp)
/*  f0f70dc:	11c00003 */ 	beqz	$t6,.NB0f0f70ec
/*  f0f70e0:	00403025 */ 	or	$a2,$v0,$zero
/*  f0f70e4:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f70e8:	a3af0106 */ 	sb	$t7,0x106($sp)
.NB0f0f70ec:
/*  f0f70ec:	30d84000 */ 	andi	$t8,$a2,0x4000
/*  f0f70f0:	13000003 */ 	beqz	$t8,.NB0f0f7100
/*  f0f70f4:	30ce2000 */ 	andi	$t6,$a2,0x2000
/*  f0f70f8:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f70fc:	a3b90107 */ 	sb	$t9,0x107($sp)
.NB0f0f7100:
/*  f0f7100:	11c00003 */ 	beqz	$t6,.NB0f0f7110
/*  f0f7104:	30d81000 */ 	andi	$t8,$a2,0x1000
/*  f0f7108:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f710c:	a3af0106 */ 	sb	$t7,0x106($sp)
.NB0f0f7110:
/*  f0f7110:	13000003 */ 	beqz	$t8,.NB0f0f7120
/*  f0f7114:	324e0010 */ 	andi	$t6,$s2,0x10
/*  f0f7118:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f711c:	afb900f8 */ 	sw	$t9,0xf8($sp)
.NB0f0f7120:
/*  f0f7120:	11c00003 */ 	beqz	$t6,.NB0f0f7130
/*  f0f7124:	02402825 */ 	or	$a1,$s2,$zero
/*  f0f7128:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f712c:	a3af010a */ 	sb	$t7,0x10a($sp)
.NB0f0f7130:
/*  f0f7130:	30b80020 */ 	andi	$t8,$a1,0x20
/*  f0f7134:	13000003 */ 	beqz	$t8,.NB0f0f7144
/*  f0f7138:	30ae0008 */ 	andi	$t6,$a1,0x8
/*  f0f713c:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f7140:	a3b9010a */ 	sb	$t9,0x10a($sp)
.NB0f0f7144:
/*  f0f7144:	05410003 */ 	bgez	$t2,.NB0f0f7154
/*  f0f7148:	30cf0008 */ 	andi	$t7,$a2,0x8
/*  f0f714c:	10000002 */ 	beqz	$zero,.NB0f0f7158
/*  f0f7150:	000a2023 */ 	negu	$a0,$t2
.NB0f0f7154:
/*  f0f7154:	01402025 */ 	or	$a0,$t2,$zero
.NB0f0f7158:
/*  f0f7158:	06010003 */ 	bgez	$s0,.NB0f0f7168
/*  f0f715c:	30b80004 */ 	andi	$t8,$a1,0x4
/*  f0f7160:	10000002 */ 	beqz	$zero,.NB0f0f716c
/*  f0f7164:	00101823 */ 	negu	$v1,$s0
.NB0f0f7168:
/*  f0f7168:	02001825 */ 	or	$v1,$s0,$zero
.NB0f0f716c:
/*  f0f716c:	0083082a */ 	slt	$at,$a0,$v1
/*  f0f7170:	10200002 */ 	beqz	$at,.NB0f0f717c
/*  f0f7174:	30d90004 */ 	andi	$t9,$a2,0x4
/*  f0f7178:	02005025 */ 	or	$t2,$s0,$zero
.NB0f0f717c:
/*  f0f717c:	05610003 */ 	bgez	$t3,.NB0f0f718c
/*  f0f7180:	01602025 */ 	or	$a0,$t3,$zero
/*  f0f7184:	10000001 */ 	beqz	$zero,.NB0f0f718c
/*  f0f7188:	000b2023 */ 	negu	$a0,$t3
.NB0f0f718c:
/*  f0f718c:	06210003 */ 	bgez	$s1,.NB0f0f719c
/*  f0f7190:	02201825 */ 	or	$v1,$s1,$zero
/*  f0f7194:	10000001 */ 	beqz	$zero,.NB0f0f719c
/*  f0f7198:	00111823 */ 	negu	$v1,$s1
.NB0f0f719c:
/*  f0f719c:	0083082a */ 	slt	$at,$a0,$v1
/*  f0f71a0:	10200002 */ 	beqz	$at,.NB0f0f71ac
/*  f0f71a4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f71a8:	02205825 */ 	or	$t3,$s1,$zero
.NB0f0f71ac:
/*  f0f71ac:	11c00002 */ 	beqz	$t6,.NB0f0f71b8
/*  f0f71b0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f71b4:	240cffff */ 	addiu	$t4,$zero,-1
.NB0f0f71b8:
/*  f0f71b8:	11e00002 */ 	beqz	$t7,.NB0f0f71c4
/*  f0f71bc:	30ae0002 */ 	andi	$t6,$a1,0x2
/*  f0f71c0:	2408ffff */ 	addiu	$t0,$zero,-1
.NB0f0f71c4:
/*  f0f71c4:	13000002 */ 	beqz	$t8,.NB0f0f71d0
/*  f0f71c8:	30cf0002 */ 	andi	$t7,$a2,0x2
/*  f0f71cc:	240c0001 */ 	addiu	$t4,$zero,0x1
.NB0f0f71d0:
/*  f0f71d0:	13200002 */ 	beqz	$t9,.NB0f0f71dc
/*  f0f71d4:	30b80001 */ 	andi	$t8,$a1,0x1
/*  f0f71d8:	24080001 */ 	addiu	$t0,$zero,0x1
.NB0f0f71dc:
/*  f0f71dc:	11c00002 */ 	beqz	$t6,.NB0f0f71e8
/*  f0f71e0:	30d90001 */ 	andi	$t9,$a2,0x1
/*  f0f71e4:	2409ffff */ 	addiu	$t1,$zero,-1
.NB0f0f71e8:
/*  f0f71e8:	11e00002 */ 	beqz	$t7,.NB0f0f71f4
/*  f0f71ec:	30ae0800 */ 	andi	$t6,$a1,0x800
/*  f0f71f0:	2415ffff */ 	addiu	$s5,$zero,-1
.NB0f0f71f4:
/*  f0f71f4:	13000002 */ 	beqz	$t8,.NB0f0f7200
/*  f0f71f8:	30cf0800 */ 	andi	$t7,$a2,0x800
/*  f0f71fc:	24090001 */ 	addiu	$t1,$zero,0x1
.NB0f0f7200:
/*  f0f7200:	13200002 */ 	beqz	$t9,.NB0f0f720c
/*  f0f7204:	30b80400 */ 	andi	$t8,$a1,0x400
/*  f0f7208:	24150001 */ 	addiu	$s5,$zero,0x1
.NB0f0f720c:
/*  f0f720c:	11c00002 */ 	beqz	$t6,.NB0f0f7218
/*  f0f7210:	30d90400 */ 	andi	$t9,$a2,0x400
/*  f0f7214:	240cffff */ 	addiu	$t4,$zero,-1
.NB0f0f7218:
/*  f0f7218:	11e00002 */ 	beqz	$t7,.NB0f0f7224
/*  f0f721c:	30ae0200 */ 	andi	$t6,$a1,0x200
/*  f0f7220:	2408ffff */ 	addiu	$t0,$zero,-1
.NB0f0f7224:
/*  f0f7224:	13000002 */ 	beqz	$t8,.NB0f0f7230
/*  f0f7228:	30cf0200 */ 	andi	$t7,$a2,0x200
/*  f0f722c:	240c0001 */ 	addiu	$t4,$zero,0x1
.NB0f0f7230:
/*  f0f7230:	13200002 */ 	beqz	$t9,.NB0f0f723c
/*  f0f7234:	30b80100 */ 	andi	$t8,$a1,0x100
/*  f0f7238:	24080001 */ 	addiu	$t0,$zero,0x1
.NB0f0f723c:
/*  f0f723c:	11c00002 */ 	beqz	$t6,.NB0f0f7248
/*  f0f7240:	30d90100 */ 	andi	$t9,$a2,0x100
/*  f0f7244:	2409ffff */ 	addiu	$t1,$zero,-1
.NB0f0f7248:
/*  f0f7248:	11e00002 */ 	beqz	$t7,.NB0f0f7254
/*  f0f724c:	8fae00ec */ 	lw	$t6,0xec($sp)
/*  f0f7250:	2415ffff */ 	addiu	$s5,$zero,-1
.NB0f0f7254:
/*  f0f7254:	13000002 */ 	beqz	$t8,.NB0f0f7260
/*  f0f7258:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0f725c:	24090001 */ 	addiu	$t1,$zero,0x1
.NB0f0f7260:
/*  f0f7260:	13200002 */ 	beqz	$t9,.NB0f0f726c
/*  f0f7264:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7268:	24150001 */ 	addiu	$s5,$zero,0x1
.NB0f0f726c:
/*  f0f726c:	168eff7f */ 	bne	$s4,$t6,.NB0f0f706c
/*  f0f7270:	26730004 */ 	addiu	$s3,$s3,0x4
.NB0f0f7274:
/*  f0f7274:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0f7278:	93ae0106 */ 	lbu	$t6,0x106($sp)
/*  f0f727c:	2410000a */ 	addiu	$s0,$zero,0xa
/*  f0f7280:	000fc0c0 */ 	sll	$t8,$t7,0x3
/*  f0f7284:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0f7288:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0f728c:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0f7290:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0f7294:	030fc023 */ 	subu	$t8,$t8,$t7
/*  f0f7298:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0f729c:	02d8c821 */ 	addu	$t9,$s6,$t8
/*  f0f72a0:	11c00002 */ 	beqz	$t6,.NB0f0f72ac
/*  f0f72a4:	8f240488 */ 	lw	$a0,0x488($t9)
/*  f0f72a8:	a3a00107 */ 	sb	$zero,0x107($sp)
.NB0f0f72ac:
/*  f0f72ac:	11000002 */ 	beqz	$t0,.NB0f0f72b8
/*  f0f72b0:	2406001e */ 	addiu	$a2,$zero,0x1e
/*  f0f72b4:	01006025 */ 	or	$t4,$t0,$zero
.NB0f0f72b8:
/*  f0f72b8:	12a00002 */ 	beqz	$s5,.NB0f0f72c4
/*  f0f72bc:	2413003c */ 	addiu	$s3,$zero,0x3c
/*  f0f72c0:	02a04825 */ 	or	$t1,$s5,$zero
.NB0f0f72c4:
/*  f0f72c4:	24110021 */ 	addiu	$s1,$zero,0x21
/*  f0f72c8:	2414002c */ 	addiu	$s4,$zero,0x2c
/*  f0f72cc:	10800019 */ 	beqz	$a0,.NB0f0f7334
/*  f0f72d0:	00009025 */ 	or	$s2,$zero,$zero
/*  f0f72d4:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0f72d8:	10400016 */ 	beqz	$v0,.NB0f0f7334
/*  f0f72dc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f72e0:	90430000 */ 	lbu	$v1,0x0($v0)
/*  f0f72e4:	24010008 */ 	addiu	$at,$zero,0x8
/*  f0f72e8:	10610003 */ 	beq	$v1,$at,.NB0f0f72f8
/*  f0f72ec:	24010010 */ 	addiu	$at,$zero,0x10
/*  f0f72f0:	54610009 */ 	bnel	$v1,$at,.NB0f0f7318
/*  f0f72f4:	2401000d */ 	addiu	$at,$zero,0xd
.NB0f0f72f8:
/*  f0f72f8:	8c8f000c */ 	lw	$t7,0xc($a0)
/*  f0f72fc:	51e00006 */ 	beqzl	$t7,.NB0f0f7318
/*  f0f7300:	2401000d */ 	addiu	$at,$zero,0xd
/*  f0f7304:	24100005 */ 	addiu	$s0,$zero,0x5
/*  f0f7308:	24060014 */ 	addiu	$a2,$zero,0x14
/*  f0f730c:	2413001e */ 	addiu	$s3,$zero,0x1e
/*  f0f7310:	2411000a */ 	addiu	$s1,$zero,0xa
/*  f0f7314:	2401000d */ 	addiu	$at,$zero,0xd
.NB0f0f7318:
/*  f0f7318:	14610006 */ 	bne	$v1,$at,.NB0f0f7334
/*  f0f731c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7320:	24120001 */ 	addiu	$s2,$zero,0x1
/*  f0f7324:	24100005 */ 	addiu	$s0,$zero,0x5
/*  f0f7328:	24060014 */ 	addiu	$a2,$zero,0x14
/*  f0f732c:	2411000a */ 	addiu	$s1,$zero,0xa
/*  f0f7330:	2414000a */ 	addiu	$s4,$zero,0xa
.NB0f0f7334:
/*  f0f7334:	15200004 */ 	bnez	$t1,.NB0f0f7348
/*  f0f7338:	0000f825 */ 	or	$ra,$zero,$zero
/*  f0f733c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7340:	2418ffff */ 	addiu	$t8,$zero,-1
/*  f0f7344:	a4780788 */ 	sh	$t8,0x788($v1)
.NB0f0f7348:
/*  f0f7348:	12a00006 */ 	beqz	$s5,.NB0f0f7364
/*  f0f734c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7350:	a4600788 */ 	sh	$zero,0x788($v1)
/*  f0f7354:	ac600780 */ 	sw	$zero,0x780($v1)
/*  f0f7358:	a4750786 */ 	sh	$s5,0x786($v1)
/*  f0f735c:	10000004 */ 	beqz	$zero,.NB0f0f7370
/*  f0f7360:	241f0001 */ 	addiu	$ra,$zero,0x1
.NB0f0f7364:
/*  f0f7364:	51200003 */ 	beqzl	$t1,.NB0f0f7374
/*  f0f7368:	8c620780 */ 	lw	$v0,0x780($v1)
/*  f0f736c:	84690786 */ 	lh	$t1,0x786($v1)
.NB0f0f7370:
/*  f0f7370:	8c620780 */ 	lw	$v0,0x780($v1)
.NB0f0f7374:
/*  f0f7374:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f7378:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0f737c:	14200004 */ 	bnez	$at,.NB0f0f7390
/*  f0f7380:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7384:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f7388:	a4790788 */ 	sh	$t9,0x788($v1)
/*  f0f738c:	8c620780 */ 	lw	$v0,0x780($v1)
.NB0f0f7390:
/*  f0f7390:	0050001a */ 	div	$zero,$v0,$s0
/*  f0f7394:	8dcee6c0 */ 	lw	$t6,-0x1940($t6)
/*  f0f7398:	00003812 */ 	mflo	$a3
/*  f0f739c:	84780788 */ 	lh	$t8,0x788($v1)
/*  f0f73a0:	004e7821 */ 	addu	$t7,$v0,$t6
/*  f0f73a4:	01f0001a */ 	div	$zero,$t7,$s0
/*  f0f73a8:	16000002 */ 	bnez	$s0,.NB0f0f73b4
/*  f0f73ac:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f73b0:	0007000d */ 	break	0x7
.NB0f0f73b4:
/*  f0f73b4:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f73b8:	16010004 */ 	bne	$s0,$at,.NB0f0f73cc
/*  f0f73bc:	3c018000 */ 	lui	$at,0x8000
/*  f0f73c0:	14410002 */ 	bne	$v0,$at,.NB0f0f73cc
/*  f0f73c4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f73c8:	0006000d */ 	break	0x6
.NB0f0f73cc:
/*  f0f73cc:	00006812 */ 	mflo	$t5
/*  f0f73d0:	16000002 */ 	bnez	$s0,.NB0f0f73dc
/*  f0f73d4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f73d8:	0007000d */ 	break	0x7
.NB0f0f73dc:
/*  f0f73dc:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f73e0:	16010004 */ 	bne	$s0,$at,.NB0f0f73f4
/*  f0f73e4:	3c018000 */ 	lui	$at,0x8000
/*  f0f73e8:	15e10002 */ 	bne	$t7,$at,.NB0f0f73f4
/*  f0f73ec:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f73f0:	0006000d */ 	break	0x6
.NB0f0f73f4:
/*  f0f73f4:	1700000b */ 	bnez	$t8,.NB0f0f7424
/*  f0f73f8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f73fc:	04e10003 */ 	bgez	$a3,.NB0f0f740c
/*  f0f7400:	0007c843 */ 	sra	$t9,$a3,0x1
/*  f0f7404:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0f7408:	0001c843 */ 	sra	$t9,$at,0x1
.NB0f0f740c:
/*  f0f740c:	03203825 */ 	or	$a3,$t9,$zero
/*  f0f7410:	05a10003 */ 	bgez	$t5,.NB0f0f7420
/*  f0f7414:	000d7043 */ 	sra	$t6,$t5,0x1
/*  f0f7418:	25a10001 */ 	addiu	$at,$t5,0x1
/*  f0f741c:	00017043 */ 	sra	$t6,$at,0x1
.NB0f0f7420:
/*  f0f7420:	01c06825 */ 	or	$t5,$t6,$zero
.NB0f0f7424:
/*  f0f7424:	05410003 */ 	bgez	$t2,.NB0f0f7434
/*  f0f7428:	a3a9010c */ 	sb	$t1,0x10c($sp)
/*  f0f742c:	10000002 */ 	beqz	$zero,.NB0f0f7438
/*  f0f7430:	000a1023 */ 	negu	$v0,$t2
.NB0f0f7434:
/*  f0f7434:	01401025 */ 	or	$v0,$t2,$zero
.NB0f0f7438:
/*  f0f7438:	05610003 */ 	bgez	$t3,.NB0f0f7448
/*  f0f743c:	0046082a */ 	slt	$at,$v0,$a2
/*  f0f7440:	10000002 */ 	beqz	$zero,.NB0f0f744c
/*  f0f7444:	000b1823 */ 	negu	$v1,$t3
.NB0f0f7448:
/*  f0f7448:	01601825 */ 	or	$v1,$t3,$zero
.NB0f0f744c:
/*  f0f744c:	14200065 */ 	bnez	$at,.NB0f0f75e4
/*  f0f7450:	0062082a */ 	slt	$at,$v1,$v0
/*  f0f7454:	14200003 */ 	bnez	$at,.NB0f0f7464
/*  f0f7458:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f745c:	52400062 */ 	beqzl	$s2,.NB0f0f75e8
/*  f0f7460:	8fb800fc */ 	lw	$t8,0xfc($sp)
.NB0f0f7464:
/*  f0f7464:	05410006 */ 	bgez	$t2,.NB0f0f7480
/*  f0f7468:	28410047 */ 	slti	$at,$v0,0x47
/*  f0f746c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7470:	846f0784 */ 	lh	$t7,0x784($v1)
/*  f0f7474:	59e00003 */ 	blezl	$t7,.NB0f0f7484
/*  f0f7478:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f747c:	a4600784 */ 	sh	$zero,0x784($v1)
.NB0f0f7480:
/*  f0f7480:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f7484:
/*  f0f7484:	19400005 */ 	blez	$t2,.NB0f0f749c
/*  f0f7488:	84640784 */ 	lh	$a0,0x784($v1)
/*  f0f748c:	04810003 */ 	bgez	$a0,.NB0f0f749c
/*  f0f7490:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7494:	a4600784 */ 	sh	$zero,0x784($v1)
/*  f0f7498:	84640784 */ 	lh	$a0,0x784($v1)
.NB0f0f749c:
/*  f0f749c:	14800003 */ 	bnez	$a0,.NB0f0f74ac
/*  f0f74a0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f74a4:	ac600780 */ 	sw	$zero,0x780($v1)
/*  f0f74a8:	84640784 */ 	lh	$a0,0x784($v1)
.NB0f0f74ac:
/*  f0f74ac:	54200003 */ 	bnezl	$at,.NB0f0f74bc
/*  f0f74b0:	00461023 */ 	subu	$v0,$v0,$a2
/*  f0f74b4:	24020046 */ 	addiu	$v0,$zero,0x46
/*  f0f74b8:	00461023 */ 	subu	$v0,$v0,$a2
.NB0f0f74bc:
/*  f0f74bc:	02220019 */ 	multu	$s1,$v0
/*  f0f74c0:	24190046 */ 	addiu	$t9,$zero,0x46
/*  f0f74c4:	03267023 */ 	subu	$t6,$t9,$a2
/*  f0f74c8:	0000c012 */ 	mflo	$t8
/*  f0f74cc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f74d0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f74d4:	030e001a */ 	div	$zero,$t8,$t6
/*  f0f74d8:	15c00002 */ 	bnez	$t6,.NB0f0f74e4
/*  f0f74dc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f74e0:	0007000d */ 	break	0x7
.NB0f0f74e4:
/*  f0f74e4:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f74e8:	15c10004 */ 	bne	$t6,$at,.NB0f0f74fc
/*  f0f74ec:	3c018000 */ 	lui	$at,0x8000
/*  f0f74f0:	17010002 */ 	bne	$t8,$at,.NB0f0f74fc
/*  f0f74f4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f74f8:	0006000d */ 	break	0x6
.NB0f0f74fc:
/*  f0f74fc:	00007812 */ 	mflo	$t7
/*  f0f7500:	026f2823 */ 	subu	$a1,$s3,$t7
/*  f0f7504:	28810003 */ 	slti	$at,$a0,0x3
/*  f0f7508:	10200004 */ 	beqz	$at,.NB0f0f751c
/*  f0f750c:	00a01825 */ 	or	$v1,$a1,$zero
/*  f0f7510:	2881fffe */ 	slti	$at,$a0,-2
/*  f0f7514:	10200005 */ 	beqz	$at,.NB0f0f752c
/*  f0f7518:	00000000 */ 	sll	$zero,$zero,0x0
.NB0f0f751c:
/*  f0f751c:	04a10003 */ 	bgez	$a1,.NB0f0f752c
/*  f0f7520:	00051843 */ 	sra	$v1,$a1,0x1
/*  f0f7524:	24a10001 */ 	addiu	$at,$a1,0x1
/*  f0f7528:	00011843 */ 	sra	$v1,$at,0x1
.NB0f0f752c:
/*  f0f752c:	1860002f */ 	blez	$v1,.NB0f0f75ec
/*  f0f7530:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7534:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0f7538:	3c18800a */ 	lui	$t8,0x800a
/*  f0f753c:	8f18e6c0 */ 	lw	$t8,-0x1940($t8)
/*  f0f7540:	8f220780 */ 	lw	$v0,0x780($t9)
/*  f0f7544:	24090001 */ 	addiu	$t1,$zero,0x1
/*  f0f7548:	0043001a */ 	div	$zero,$v0,$v1
/*  f0f754c:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0f7550:	00003812 */ 	mflo	$a3
/*  f0f7554:	14600002 */ 	bnez	$v1,.NB0f0f7560
/*  f0f7558:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f755c:	0007000d */ 	break	0x7
.NB0f0f7560:
/*  f0f7560:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f7564:	14610004 */ 	bne	$v1,$at,.NB0f0f7578
/*  f0f7568:	3c018000 */ 	lui	$at,0x8000
/*  f0f756c:	14410002 */ 	bne	$v0,$at,.NB0f0f7578
/*  f0f7570:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7574:	0006000d */ 	break	0x6
.NB0f0f7578:
/*  f0f7578:	01c3001a */ 	div	$zero,$t6,$v1
/*  f0f757c:	00006812 */ 	mflo	$t5
/*  f0f7580:	14600002 */ 	bnez	$v1,.NB0f0f758c
/*  f0f7584:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7588:	0007000d */ 	break	0x7
.NB0f0f758c:
/*  f0f758c:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f7590:	14610004 */ 	bne	$v1,$at,.NB0f0f75a4
/*  f0f7594:	3c018000 */ 	lui	$at,0x8000
/*  f0f7598:	15c10002 */ 	bne	$t6,$at,.NB0f0f75a4
/*  f0f759c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f75a0:	0006000d */ 	break	0x6
.NB0f0f75a4:
/*  f0f75a4:	05410003 */ 	bgez	$t2,.NB0f0f75b4
/*  f0f75a8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f75ac:	10000001 */ 	beqz	$zero,.NB0f0f75b4
/*  f0f75b0:	2409ffff */ 	addiu	$t1,$zero,-1
.NB0f0f75b4:
/*  f0f75b4:	10ed0002 */ 	beq	$a3,$t5,.NB0f0f75c0
/*  f0f75b8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f75bc:	241f0001 */ 	addiu	$ra,$zero,0x1
.NB0f0f75c0:
/*  f0f75c0:	14800002 */ 	bnez	$a0,.NB0f0f75cc
/*  f0f75c4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f75c8:	241f0001 */ 	addiu	$ra,$zero,0x1
.NB0f0f75cc:
/*  f0f75cc:	13e00007 */ 	beqz	$ra,.NB0f0f75ec
/*  f0f75d0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f75d4:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0f75d8:	00897821 */ 	addu	$t7,$a0,$t1
/*  f0f75dc:	10000003 */ 	beqz	$zero,.NB0f0f75ec
/*  f0f75e0:	a72f0784 */ 	sh	$t7,0x784($t9)
.NB0f0f75e4:
/*  f0f75e4:	8fb800fc */ 	lw	$t8,0xfc($sp)
.NB0f0f75e8:
/*  f0f75e8:	a7000784 */ 	sh	$zero,0x784($t8)
.NB0f0f75ec:
/*  f0f75ec:	10ed0002 */ 	beq	$a3,$t5,.NB0f0f75f8
/*  f0f75f0:	3c19800a */ 	lui	$t9,0x800a
/*  f0f75f4:	241f0001 */ 	addiu	$ra,$zero,0x1
.NB0f0f75f8:
/*  f0f75f8:	17e00002 */ 	bnez	$ra,.NB0f0f7604
/*  f0f75fc:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7600:	00004825 */ 	or	$t1,$zero,$zero
.NB0f0f7604:
/*  f0f7604:	11000007 */ 	beqz	$t0,.NB0f0f7624
/*  f0f7608:	00006825 */ 	or	$t5,$zero,$zero
/*  f0f760c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7610:	240d0001 */ 	addiu	$t5,$zero,0x1
/*  f0f7614:	a4600794 */ 	sh	$zero,0x794($v1)
/*  f0f7618:	ac60078c */ 	sw	$zero,0x78c($v1)
/*  f0f761c:	10000005 */ 	beqz	$zero,.NB0f0f7634
/*  f0f7620:	a4680792 */ 	sh	$t0,0x792($v1)
.NB0f0f7624:
/*  f0f7624:	51800004 */ 	beqzl	$t4,.NB0f0f7638
/*  f0f7628:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f762c:	8fae00fc */ 	lw	$t6,0xfc($sp)
/*  f0f7630:	85cc0792 */ 	lh	$t4,0x792($t6)
.NB0f0f7634:
/*  f0f7634:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f7638:
/*  f0f7638:	8c62078c */ 	lw	$v0,0x78c($v1)
/*  f0f763c:	2841003d */ 	slti	$at,$v0,0x3d
/*  f0f7640:	14200004 */ 	bnez	$at,.NB0f0f7654
/*  f0f7644:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7648:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f764c:	a46f0794 */ 	sh	$t7,0x794($v1)
/*  f0f7650:	8c62078c */ 	lw	$v0,0x78c($v1)
.NB0f0f7654:
/*  f0f7654:	0050001a */ 	div	$zero,$v0,$s0
/*  f0f7658:	8f39e6c0 */ 	lw	$t9,-0x1940($t9)
/*  f0f765c:	00003812 */ 	mflo	$a3
/*  f0f7660:	846e0794 */ 	lh	$t6,0x794($v1)
/*  f0f7664:	0059c021 */ 	addu	$t8,$v0,$t9
/*  f0f7668:	0310001a */ 	div	$zero,$t8,$s0
/*  f0f766c:	16000002 */ 	bnez	$s0,.NB0f0f7678
/*  f0f7670:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7674:	0007000d */ 	break	0x7
.NB0f0f7678:
/*  f0f7678:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f767c:	16010004 */ 	bne	$s0,$at,.NB0f0f7690
/*  f0f7680:	3c018000 */ 	lui	$at,0x8000
/*  f0f7684:	14410002 */ 	bne	$v0,$at,.NB0f0f7690
/*  f0f7688:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f768c:	0006000d */ 	break	0x6
.NB0f0f7690:
/*  f0f7690:	00004012 */ 	mflo	$t0
/*  f0f7694:	01601025 */ 	or	$v0,$t3,$zero
/*  f0f7698:	16000002 */ 	bnez	$s0,.NB0f0f76a4
/*  f0f769c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f76a0:	0007000d */ 	break	0x7
.NB0f0f76a4:
/*  f0f76a4:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f76a8:	16010004 */ 	bne	$s0,$at,.NB0f0f76bc
/*  f0f76ac:	3c018000 */ 	lui	$at,0x8000
/*  f0f76b0:	17010002 */ 	bne	$t8,$at,.NB0f0f76bc
/*  f0f76b4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f76b8:	0006000d */ 	break	0x6
.NB0f0f76bc:
/*  f0f76bc:	15c0000b */ 	bnez	$t6,.NB0f0f76ec
/*  f0f76c0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f76c4:	04e10003 */ 	bgez	$a3,.NB0f0f76d4
/*  f0f76c8:	00077843 */ 	sra	$t7,$a3,0x1
/*  f0f76cc:	24e10001 */ 	addiu	$at,$a3,0x1
/*  f0f76d0:	00017843 */ 	sra	$t7,$at,0x1
.NB0f0f76d4:
/*  f0f76d4:	01e03825 */ 	or	$a3,$t7,$zero
/*  f0f76d8:	05010003 */ 	bgez	$t0,.NB0f0f76e8
/*  f0f76dc:	0008c843 */ 	sra	$t9,$t0,0x1
/*  f0f76e0:	25010001 */ 	addiu	$at,$t0,0x1
/*  f0f76e4:	0001c843 */ 	sra	$t9,$at,0x1
.NB0f0f76e8:
/*  f0f76e8:	03204025 */ 	or	$t0,$t9,$zero
.NB0f0f76ec:
/*  f0f76ec:	05610003 */ 	bgez	$t3,.NB0f0f76fc
/*  f0f76f0:	a3ac010d */ 	sb	$t4,0x10d($sp)
/*  f0f76f4:	10000001 */ 	beqz	$zero,.NB0f0f76fc
/*  f0f76f8:	000b1023 */ 	negu	$v0,$t3
.NB0f0f76fc:
/*  f0f76fc:	05410003 */ 	bgez	$t2,.NB0f0f770c
/*  f0f7700:	28410014 */ 	slti	$at,$v0,0x14
/*  f0f7704:	10000002 */ 	beqz	$zero,.NB0f0f7710
/*  f0f7708:	000a1823 */ 	negu	$v1,$t2
.NB0f0f770c:
/*  f0f770c:	01401825 */ 	or	$v1,$t2,$zero
.NB0f0f7710:
/*  f0f7710:	1420005d */ 	bnez	$at,.NB0f0f7888
/*  f0f7714:	0062082a */ 	slt	$at,$v1,$v0
/*  f0f7718:	14200003 */ 	bnez	$at,.NB0f0f7728
/*  f0f771c:	3c06800a */ 	lui	$a2,0x800a
/*  f0f7720:	5240005a */ 	beqzl	$s2,.NB0f0f788c
/*  f0f7724:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f7728:
/*  f0f7728:	05610006 */ 	bgez	$t3,.NB0f0f7744
/*  f0f772c:	28410047 */ 	slti	$at,$v0,0x47
/*  f0f7730:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7734:	84780790 */ 	lh	$t8,0x790($v1)
/*  f0f7738:	07030003 */ 	bgezl	$t8,.NB0f0f7748
/*  f0f773c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7740:	a4600790 */ 	sh	$zero,0x790($v1)
.NB0f0f7744:
/*  f0f7744:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f7748:
/*  f0f7748:	19600005 */ 	blez	$t3,.NB0f0f7760
/*  f0f774c:	84640790 */ 	lh	$a0,0x790($v1)
/*  f0f7750:	18800003 */ 	blez	$a0,.NB0f0f7760
/*  f0f7754:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7758:	a4600790 */ 	sh	$zero,0x790($v1)
/*  f0f775c:	84640790 */ 	lh	$a0,0x790($v1)
.NB0f0f7760:
/*  f0f7760:	14800003 */ 	bnez	$a0,.NB0f0f7770
/*  f0f7764:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7768:	ac60078c */ 	sw	$zero,0x78c($v1)
/*  f0f776c:	84640790 */ 	lh	$a0,0x790($v1)
.NB0f0f7770:
/*  f0f7770:	14200002 */ 	bnez	$at,.NB0f0f777c
/*  f0f7774:	8cc6e6c0 */ 	lw	$a2,-0x1940($a2)
/*  f0f7778:	24020046 */ 	addiu	$v0,$zero,0x46
.NB0f0f777c:
/*  f0f777c:	2442ffec */ 	addiu	$v0,$v0,-20
/*  f0f7780:	02820019 */ 	multu	$s4,$v0
/*  f0f7784:	24010032 */ 	addiu	$at,$zero,0x32
/*  f0f7788:	8fb900fc */ 	lw	$t9,0xfc($sp)
/*  f0f778c:	00007012 */ 	mflo	$t6
/*  f0f7790:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7794:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7798:	01c1001a */ 	div	$zero,$t6,$at
/*  f0f779c:	00007812 */ 	mflo	$t7
/*  f0f77a0:	026f2823 */ 	subu	$a1,$s3,$t7
/*  f0f77a4:	28810003 */ 	slti	$at,$a0,0x3
/*  f0f77a8:	10200004 */ 	beqz	$at,.NB0f0f77bc
/*  f0f77ac:	00a01825 */ 	or	$v1,$a1,$zero
/*  f0f77b0:	2881fffe */ 	slti	$at,$a0,-2
/*  f0f77b4:	10200006 */ 	beqz	$at,.NB0f0f77d0
/*  f0f77b8:	00000000 */ 	sll	$zero,$zero,0x0
.NB0f0f77bc:
/*  f0f77bc:	24010003 */ 	addiu	$at,$zero,0x3
/*  f0f77c0:	00a1001a */ 	div	$zero,$a1,$at
/*  f0f77c4:	00001812 */ 	mflo	$v1
/*  f0f77c8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f77cc:	00000000 */ 	sll	$zero,$zero,0x0
.NB0f0f77d0:
/*  f0f77d0:	5860002b */ 	blezl	$v1,.NB0f0f7880
/*  f0f77d4:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f77d8:	8f22078c */ 	lw	$v0,0x78c($t9)
/*  f0f77dc:	240c0001 */ 	addiu	$t4,$zero,0x1
/*  f0f77e0:	0043001a */ 	div	$zero,$v0,$v1
/*  f0f77e4:	0046c021 */ 	addu	$t8,$v0,$a2
/*  f0f77e8:	00003812 */ 	mflo	$a3
/*  f0f77ec:	14600002 */ 	bnez	$v1,.NB0f0f77f8
/*  f0f77f0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f77f4:	0007000d */ 	break	0x7
.NB0f0f77f8:
/*  f0f77f8:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f77fc:	14610004 */ 	bne	$v1,$at,.NB0f0f7810
/*  f0f7800:	3c018000 */ 	lui	$at,0x8000
/*  f0f7804:	14410002 */ 	bne	$v0,$at,.NB0f0f7810
/*  f0f7808:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f780c:	0006000d */ 	break	0x6
.NB0f0f7810:
/*  f0f7810:	0303001a */ 	div	$zero,$t8,$v1
/*  f0f7814:	00004012 */ 	mflo	$t0
/*  f0f7818:	14600002 */ 	bnez	$v1,.NB0f0f7824
/*  f0f781c:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7820:	0007000d */ 	break	0x7
.NB0f0f7824:
/*  f0f7824:	2401ffff */ 	addiu	$at,$zero,-1
/*  f0f7828:	14610004 */ 	bne	$v1,$at,.NB0f0f783c
/*  f0f782c:	3c018000 */ 	lui	$at,0x8000
/*  f0f7830:	17010002 */ 	bne	$t8,$at,.NB0f0f783c
/*  f0f7834:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7838:	0006000d */ 	break	0x6
.NB0f0f783c:
/*  f0f783c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7840:	19600003 */ 	blez	$t3,.NB0f0f7850
/*  f0f7844:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7848:	10000001 */ 	beqz	$zero,.NB0f0f7850
/*  f0f784c:	240cffff */ 	addiu	$t4,$zero,-1
.NB0f0f7850:
/*  f0f7850:	10e80002 */ 	beq	$a3,$t0,.NB0f0f785c
/*  f0f7854:	008c7021 */ 	addu	$t6,$a0,$t4
/*  f0f7858:	240d0001 */ 	addiu	$t5,$zero,0x1
.NB0f0f785c:
/*  f0f785c:	14800002 */ 	bnez	$a0,.NB0f0f7868
/*  f0f7860:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7864:	240d0001 */ 	addiu	$t5,$zero,0x1
.NB0f0f7868:
/*  f0f7868:	51a00005 */ 	beqzl	$t5,.NB0f0f7880
/*  f0f786c:	8fa300fc */ 	lw	$v1,0xfc($sp)
/*  f0f7870:	a46e0790 */ 	sh	$t6,0x790($v1)
/*  f0f7874:	3c06800a */ 	lui	$a2,0x800a
/*  f0f7878:	8cc6e6c0 */ 	lw	$a2,-0x1940($a2)
/*  f0f787c:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f7880:
/*  f0f7880:	10000006 */ 	beqz	$zero,.NB0f0f789c
/*  f0f7884:	8c62078c */ 	lw	$v0,0x78c($v1)
.NB0f0f7888:
/*  f0f7888:	8fa300fc */ 	lw	$v1,0xfc($sp)
.NB0f0f788c:
/*  f0f788c:	3c06800a */ 	lui	$a2,0x800a
/*  f0f7890:	a4600790 */ 	sh	$zero,0x790($v1)
/*  f0f7894:	8cc6e6c0 */ 	lw	$a2,-0x1940($a2)
/*  f0f7898:	8c62078c */ 	lw	$v0,0x78c($v1)
.NB0f0f789c:
/*  f0f789c:	10e80002 */ 	beq	$a3,$t0,.NB0f0f78a8
/*  f0f78a0:	27b50104 */ 	addiu	$s5,$sp,0x104
/*  f0f78a4:	240d0001 */ 	addiu	$t5,$zero,0x1
.NB0f0f78a8:
/*  f0f78a8:	55a00003 */ 	bnezl	$t5,.NB0f0f78b8
/*  f0f78ac:	8c6f0780 */ 	lw	$t7,0x780($v1)
/*  f0f78b0:	00006025 */ 	or	$t4,$zero,$zero
/*  f0f78b4:	8c6f0780 */ 	lw	$t7,0x780($v1)
.NB0f0f78b8:
/*  f0f78b8:	8fa500f8 */ 	lw	$a1,0xf8($sp)
/*  f0f78bc:	3c18800a */ 	lui	$t8,0x800a
/*  f0f78c0:	01e6c821 */ 	addu	$t9,$t7,$a2
/*  f0f78c4:	ac790780 */ 	sw	$t9,0x780($v1)
/*  f0f78c8:	8f18e6c0 */ 	lw	$t8,-0x1940($t8)
/*  f0f78cc:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f78d0:	00587021 */ 	addu	$t6,$v0,$t8
/*  f0f78d4:	ac6e078c */ 	sw	$t6,0x78c($v1)
/*  f0f78d8:	a3a90104 */ 	sb	$t1,0x104($sp)
/*  f0f78dc:	a3ac0105 */ 	sb	$t4,0x105($sp)
/*  f0f78e0:	a3aa0108 */ 	sb	$t2,0x108($sp)
/*  f0f78e4:	a3ab0109 */ 	sb	$t3,0x109($sp)
/*  f0f78e8:	10a00003 */ 	beqz	$a1,.NB0f0f78f8
/*  f0f78ec:	a3a00118 */ 	sb	$zero,0x118($sp)
/*  f0f78f0:	10000002 */ 	beqz	$zero,.NB0f0f78fc
/*  f0f78f4:	a3af010e */ 	sb	$t7,0x10e($sp)
.NB0f0f78f8:
/*  f0f78f8:	a3a0010e */ 	sb	$zero,0x10e($sp)
.NB0f0f78fc:
/*  f0f78fc:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0f7900:	007e0019 */ 	multu	$v1,$s8
/*  f0f7904:	0000c812 */ 	mflo	$t9
/*  f0f7908:	02d93821 */ 	addu	$a3,$s6,$t9
/*  f0f790c:	8ce40488 */ 	lw	$a0,0x488($a3)
/*  f0f7910:	50800016 */ 	beqzl	$a0,.NB0f0f796c
/*  f0f7914:	84e20484 */ 	lh	$v0,0x484($a3)
/*  f0f7918:	50a00014 */ 	beqzl	$a1,.NB0f0f796c
/*  f0f791c:	84e20484 */ 	lh	$v0,0x484($a3)
/*  f0f7920:	8c980000 */ 	lw	$t8,0x0($a0)
/*  f0f7924:	24190001 */ 	addiu	$t9,$zero,0x1
/*  f0f7928:	8f0e0010 */ 	lw	$t6,0x10($t8)
/*  f0f792c:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0f7930:	31cf0004 */ 	andi	$t7,$t6,0x4
/*  f0f7934:	51e00004 */ 	beqzl	$t7,.NB0f0f7948
/*  f0f7938:	8c820008 */ 	lw	$v0,0x8($a0)
/*  f0f793c:	a3b90106 */ 	sb	$t9,0x106($sp)
/*  f0f7940:	afb8011c */ 	sw	$t8,0x11c($sp)
/*  f0f7944:	8c820008 */ 	lw	$v0,0x8($a0)
.NB0f0f7948:
/*  f0f7948:	50400008 */ 	beqzl	$v0,.NB0f0f796c
/*  f0f794c:	84e20484 */ 	lh	$v0,0x484($a3)
/*  f0f7950:	904e0000 */ 	lbu	$t6,0x0($v0)
/*  f0f7954:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0f7958:	240f0001 */ 	addiu	$t7,$zero,0x1
/*  f0f795c:	55c10003 */ 	bnel	$t6,$at,.NB0f0f796c
/*  f0f7960:	84e20484 */ 	lh	$v0,0x484($a3)
/*  f0f7964:	a3af0106 */ 	sb	$t7,0x106($sp)
/*  f0f7968:	84e20484 */ 	lh	$v0,0x484($a3)
.NB0f0f796c:
/*  f0f796c:	00009825 */ 	or	$s3,$zero,$zero
/*  f0f7970:	0000a025 */ 	or	$s4,$zero,$zero
/*  f0f7974:	1840003a */ 	blez	$v0,.NB0f0f7a60
/*  f0f7978:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f797c:	afa00044 */ 	sw	$zero,0x44($sp)
.NB0f0f7980:
/*  f0f7980:	0003c8c0 */ 	sll	$t9,$v1,0x3
/*  f0f7984:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f7988:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0f798c:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f7990:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0f7994:	8fae0044 */ 	lw	$t6,0x44($sp)
/*  f0f7998:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f799c:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0f79a0:	02d9c021 */ 	addu	$t8,$s6,$t9
/*  f0f79a4:	030e9021 */ 	addu	$s2,$t8,$t6
/*  f0f79a8:	824f0408 */ 	lb	$t7,0x408($s2)
/*  f0f79ac:	265203f4 */ 	addiu	$s2,$s2,0x3f4
/*  f0f79b0:	02408825 */ 	or	$s1,$s2,$zero
/*  f0f79b4:	19e00024 */ 	blez	$t7,.NB0f0f7a48
/*  f0f79b8:	00008025 */ 	or	$s0,$zero,$zero
/*  f0f79bc:	8ef80000 */ 	lw	$t8,0x0($s7)
.NB0f0f79c0:
/*  f0f79c0:	26990001 */ 	addiu	$t9,$s4,0x1
/*  f0f79c4:	00003025 */ 	or	$a2,$zero,$zero
/*  f0f79c8:	031e0019 */ 	multu	$t8,$s8
/*  f0f79cc:	02a02825 */ 	or	$a1,$s5,$zero
/*  f0f79d0:	00007012 */ 	mflo	$t6
/*  f0f79d4:	02ce7821 */ 	addu	$t7,$s6,$t6
/*  f0f79d8:	85f80484 */ 	lh	$t8,0x484($t7)
/*  f0f79dc:	17380008 */ 	bne	$t9,$t8,.NB0f0f7a00
/*  f0f79e0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f79e4:	824e0015 */ 	lb	$t6,0x15($s2)
/*  f0f79e8:	160e0005 */ 	bne	$s0,$t6,.NB0f0f7a00
/*  f0f79ec:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f79f0:	16600003 */ 	bnez	$s3,.NB0f0f7a00
/*  f0f79f4:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f79f8:	24060001 */ 	addiu	$a2,$zero,0x1
/*  f0f79fc:	24130001 */ 	addiu	$s3,$zero,0x1
.NB0f0f7a00:
/*  f0f7a00:	0fc3d6ec */ 	jal	dialogTick
/*  f0f7a04:	8e240000 */ 	lw	$a0,0x0($s1)
/*  f0f7a08:	824f0014 */ 	lb	$t7,0x14($s2)
/*  f0f7a0c:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0f7a10:	26310004 */ 	addiu	$s1,$s1,0x4
/*  f0f7a14:	020f082a */ 	slt	$at,$s0,$t7
/*  f0f7a18:	5420ffe9 */ 	bnezl	$at,.NB0f0f79c0
/*  f0f7a1c:	8ef80000 */ 	lw	$t8,0x0($s7)
/*  f0f7a20:	8ee30000 */ 	lw	$v1,0x0($s7)
/*  f0f7a24:	0003c8c0 */ 	sll	$t9,$v1,0x3
/*  f0f7a28:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f7a2c:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0f7a30:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f7a34:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0f7a38:	0323c823 */ 	subu	$t9,$t9,$v1
/*  f0f7a3c:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0f7a40:	02d93821 */ 	addu	$a3,$s6,$t9
/*  f0f7a44:	84e20484 */ 	lh	$v0,0x484($a3)
.NB0f0f7a48:
/*  f0f7a48:	8fb80044 */ 	lw	$t8,0x44($sp)
/*  f0f7a4c:	26940001 */ 	addiu	$s4,$s4,0x1
/*  f0f7a50:	0282082a */ 	slt	$at,$s4,$v0
/*  f0f7a54:	270e0018 */ 	addiu	$t6,$t8,0x18
/*  f0f7a58:	1420ffc9 */ 	bnez	$at,.NB0f0f7980
/*  f0f7a5c:	afae0044 */ 	sw	$t6,0x44($sp)
.NB0f0f7a60:
/*  f0f7a60:	3c0f800a */ 	lui	$t7,0x800a
/*  f0f7a64:	8def5eb4 */ 	lw	$t7,0x5eb4($t7)
/*  f0f7a68:	25f9ffff */ 	addiu	$t9,$t7,-1
/*  f0f7a6c:	2f21000d */ 	sltiu	$at,$t9,0xd
/*  f0f7a70:	1020004d */ 	beqz	$at,.NB0f0f7ba8
/*  f0f7a74:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0f7a78:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0f7a7c:	00390821 */ 	addu	$at,$at,$t9
/*  f0f7a80:	8c39cdb4 */ 	lw	$t9,-0x324c($at)
/*  f0f7a84:	03200008 */ 	jr	$t9
/*  f0f7a88:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7a8c:	83b8010e */ 	lb	$t8,0x10e($sp)
/*  f0f7a90:	8fae011c */ 	lw	$t6,0x11c($sp)
/*  f0f7a94:	53000045 */ 	beqzl	$t8,.NB0f0f7bac
/*  f0f7a98:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7a9c:	55c00043 */ 	bnezl	$t6,.NB0f0f7bac
/*  f0f7aa0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7aa4:	8ce40488 */ 	lw	$a0,0x488($a3)
/*  f0f7aa8:	8faf0100 */ 	lw	$t7,0x100($sp)
/*  f0f7aac:	5080003f */ 	beqzl	$a0,.NB0f0f7bac
/*  f0f7ab0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7ab4:	8df9000c */ 	lw	$t9,0xc($t7)
/*  f0f7ab8:	3c18800a */ 	lui	$t8,0x800a
/*  f0f7abc:	5720003b */ 	bnezl	$t9,.NB0f0f7bac
/*  f0f7ac0:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7ac4:	8f18eb50 */ 	lw	$t8,-0x14b0($t8)
/*  f0f7ac8:	8c820000 */ 	lw	$v0,0x0($a0)
/*  f0f7acc:	24010002 */ 	addiu	$at,$zero,0x2
/*  f0f7ad0:	13010008 */ 	beq	$t8,$at,.NB0f0f7af4
/*  f0f7ad4:	3c048008 */ 	lui	$a0,0x8008
/*  f0f7ad8:	24847f34 */ 	addiu	$a0,$a0,0x7f34
/*  f0f7adc:	10820005 */ 	beq	$a0,$v0,.NB0f0f7af4
/*  f0f7ae0:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7ae4:	0fc3beda */ 	jal	menuPushDialog
/*  f0f7ae8:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7aec:	1000002f */ 	beqz	$zero,.NB0f0f7bac
/*  f0f7af0:	8fbf003c */ 	lw	$ra,0x3c($sp)
.NB0f0f7af4:
/*  f0f7af4:	3c0e8009 */ 	lui	$t6,0x8009
/*  f0f7af8:	25ce8dfc */ 	addiu	$t6,$t6,-29188
/*  f0f7afc:	55c2002b */ 	bnel	$t6,$v0,.NB0f0f7bac
/*  f0f7b00:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7b04:	0fc5e6b1 */ 	jal	func0f17f428
/*  f0f7b08:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7b0c:	10000027 */ 	beqz	$zero,.NB0f0f7bac
/*  f0f7b10:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7b14:	3c0f8007 */ 	lui	$t7,0x8007
/*  f0f7b18:	8def2e24 */ 	lw	$t7,0x2e24($t7)
/*  f0f7b1c:	51e00010 */ 	beqzl	$t7,.NB0f0f7b60
/*  f0f7b20:	240f000a */ 	addiu	$t7,$zero,0xa
/*  f0f7b24:	0fc3d2a2 */ 	jal	func0f0f8120
/*  f0f7b28:	00000000 */ 	sll	$zero,$zero,0x0
/*  f0f7b2c:	3c198007 */ 	lui	$t9,0x8007
/*  f0f7b30:	8f393af0 */ 	lw	$t9,0x3af0($t9)
/*  f0f7b34:	3c0e800a */ 	lui	$t6,0x800a
/*  f0f7b38:	25ce27c0 */ 	addiu	$t6,$t6,0x27c0
/*  f0f7b3c:	0019c0c0 */ 	sll	$t8,$t9,0x3
/*  f0f7b40:	0319c023 */ 	subu	$t8,$t8,$t9
/*  f0f7b44:	0018c0c0 */ 	sll	$t8,$t8,0x3
/*  f0f7b48:	0319c023 */ 	subu	$t8,$t8,$t9
/*  f0f7b4c:	0018c100 */ 	sll	$t8,$t8,0x4
/*  f0f7b50:	0319c023 */ 	subu	$t8,$t8,$t9
/*  f0f7b54:	0018c080 */ 	sll	$t8,$t8,0x2
/*  f0f7b58:	030e3821 */ 	addu	$a3,$t8,$t6
/*  f0f7b5c:	240f000a */ 	addiu	$t7,$zero,0xa
.NB0f0f7b60:
/*  f0f7b60:	a0ef0798 */ 	sb	$t7,0x798($a3)
/*  f0f7b64:	83b9010e */ 	lb	$t9,0x10e($sp)
/*  f0f7b68:	8fb8011c */ 	lw	$t8,0x11c($sp)
/*  f0f7b6c:	5320000f */ 	beqzl	$t9,.NB0f0f7bac
/*  f0f7b70:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7b74:	5700000d */ 	bnezl	$t8,.NB0f0f7bac
/*  f0f7b78:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7b7c:	8cee0488 */ 	lw	$t6,0x488($a3)
/*  f0f7b80:	8faf0100 */ 	lw	$t7,0x100($sp)
/*  f0f7b84:	51c00009 */ 	beqzl	$t6,.NB0f0f7bac
/*  f0f7b88:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7b8c:	8df90000 */ 	lw	$t9,0x0($t7)
/*  f0f7b90:	8f380010 */ 	lw	$t8,0x10($t9)
/*  f0f7b94:	330e0020 */ 	andi	$t6,$t8,0x20
/*  f0f7b98:	55c00004 */ 	bnezl	$t6,.NB0f0f7bac
/*  f0f7b9c:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0f7ba0:	0fc3d2a2 */ 	jal	func0f0f8120
/*  f0f7ba4:	00000000 */ 	sll	$zero,$zero,0x0
.NB0f0f7ba8:
/*  f0f7ba8:	8fbf003c */ 	lw	$ra,0x3c($sp)
.NB0f0f7bac:
/*  f0f7bac:	8fb00018 */ 	lw	$s0,0x18($sp)
/*  f0f7bb0:	8fb1001c */ 	lw	$s1,0x1c($sp)
/*  f0f7bb4:	8fb20020 */ 	lw	$s2,0x20($sp)
/*  f0f7bb8:	8fb30024 */ 	lw	$s3,0x24($sp)
/*  f0f7bbc:	8fb40028 */ 	lw	$s4,0x28($sp)
/*  f0f7bc0:	8fb5002c */ 	lw	$s5,0x2c($sp)
/*  f0f7bc4:	8fb60030 */ 	lw	$s6,0x30($sp)
/*  f0f7bc8:	8fb70034 */ 	lw	$s7,0x34($sp)
/*  f0f7bcc:	8fbe0038 */ 	lw	$s8,0x38($sp)
/*  f0f7bd0:	03e00008 */ 	jr	$ra
/*  f0f7bd4:	27bd0130 */ 	addiu	$sp,$sp,0x130
);
#endif

// Mismatch: regalloc in first for loop
//void menuProcessInput(void)
//{
//	s32 yhelddir; // 12c
//	s32 xhelddir; // 128
//	s32 ytapdir; // 124
//	s32 xtapdir; // 120
//	bool starttoselect; // 11c
//	struct menuinputs inputs; // 104
//	struct menudialog *dialog; // 100
//	struct menu *menu; // fc
//	bool starttap; // f8
//	s32 stickx; // f4
//	s32 sticky; // f0
//	s32 numcontpads; // ec
//	s32 i;
//	s32 contpadnums[4]; // d8
//	s8 contpadnum1; // d7
//	s8 contpadnum2; // d6
//	struct fileguid guid; // cc
//	s32 xdeadzone;
//	s32 ydeadzone;
//	s32 digitalrepeatinterval;
//	s32 xstickintervalmult;
//	s32 stickintervalbase;
//	s32 ystickintervalmult;
//	s32 allowdiagonal;
//
//	yhelddir = 0;
//	xhelddir = 0;
//	ytapdir = 0;
//	xtapdir = 0;
//	starttoselect = false;
//
//	menu = &g_Menus[g_MpPlayerNum];
//	dialog = g_Menus[g_MpPlayerNum].curdialog;
//
//	if (g_MenuData.root == MENUROOT_MPPAUSE) {
//		g_AmIndex = g_Vars.currentplayernum;
//	}
//
//	func0f0f1418();
//
//	inputs.select = 0;
//	inputs.back = 0;
//	inputs.shoulder = 0;
//	inputs.back2 = 0;
//
//	if (g_Menus[g_MpPlayerNum].curdialog) {
//		stickx = 0;
//		sticky = 0;
//		starttap = false;
//		numcontpads = 0;
//
//		// Decide which controller pads will control this player's menu
//		if (g_MenuData.root == MENUROOT_BOOTPAKMGR) {
//			contpadnums[0] = 0;
//			contpadnums[1] = 1;
//			contpadnums[2] = 2;
//			contpadnums[3] = 3;
//			numcontpads = 4;
//		} else {
//			menuGetContPads(&contpadnum1, &contpadnum2);
//
//			if (contpadnum1 >= 0) {
//				contpadnums[numcontpads] = contpadnum1;
//				numcontpads++;
//			}
//
//			if (contpadnum2 >= 0) {
//				contpadnums[numcontpads] = contpadnum2;
//				numcontpads++;
//			}
//		}
//
//		// Handle some kind of file save/load timer
//		if (g_Menus[g_MpPlayerNum].fm.unke41 > 0) {
//			g_Menus[g_MpPlayerNum].fm.unke41--;
//
//			if (g_Menus[g_MpPlayerNum].fm.unke41 == 0) {
//				guid.fileid = g_Menus[g_MpPlayerNum].fm.fileid;
//				guid.deviceserial = g_Menus[g_MpPlayerNum].fm.deviceserial;
//
//				filemgrSaveOrLoad(&guid, -1, 0);
//			} else {
//				g_Menus[g_MpPlayerNum].fm.unke40_00 = true;
//			}
//		}
//
//		// Iterate controllers and figure out which buttons are being pressed.
//		// For the control stick input, take whichever stick is pressed the most.
//		for (i = 0; i < numcontpads; i++) {
//			s8 thisstickx = joyGetStickX(contpadnums[i]);
//			s8 thissticky = joyGetStickY(contpadnums[i]);
//			u16 buttons = joyGetButtons(contpadnums[i], 0xffff);
//			s32 buttonsnow = joyGetButtonsPressedThisFrame(contpadnums[i], 0xffff);
//
//			if (buttonsnow & A_BUTTON) {
//				inputs.select = 1;
//			}
//
//			if (buttonsnow & B_BUTTON) {
//				inputs.back = 1;
//			}
//
//			if (buttonsnow & Z_TRIG) {
//				inputs.select = 1;
//			}
//
//			if (buttonsnow & START_BUTTON) {
//				starttap = true;
//			}
//
//			if (buttons & R_TRIG) {
//				inputs.shoulder = 1;
//			}
//
//			if (buttons & L_TRIG) {
//				inputs.shoulder = 1;
//			}
//
//			if ((stickx < 0 ? -stickx : stickx) < (thisstickx < 0 ? -thisstickx : thisstickx)) {
//				stickx = thisstickx;
//			}
//
//			if ((sticky < 0 ? -sticky : sticky) < (thissticky < 0 ? -thissticky : thissticky)) {
//				sticky = thissticky;
//			}
//
//			if (buttons & U_CBUTTONS) {
//				yhelddir = -1;
//			}
//
//			if (buttonsnow & U_CBUTTONS) {
//				ytapdir = -1;
//			}
//
//			if (buttons & D_CBUTTONS) {
//				yhelddir = 1;
//			}
//
//			if (buttonsnow & D_CBUTTONS) {
//				ytapdir = 1;
//			}
//
//			if (buttons & L_CBUTTONS) {
//				xhelddir = -1;
//			}
//
//			if (buttonsnow & L_CBUTTONS) {
//				xtapdir = -1;
//			}
//
//			if (buttons & R_CBUTTONS) {
//				xhelddir = 1;
//			}
//
//			if (buttonsnow & R_CBUTTONS) {
//				xtapdir = 1;
//			}
//
//			if (buttons & U_JPAD) {
//				yhelddir = -1;
//			}
//
//			if (buttonsnow & U_JPAD) {
//				ytapdir = -1;
//			}
//
//			if (buttons & D_JPAD) {
//				yhelddir = 1;
//			}
//
//			if (buttonsnow & D_JPAD) {
//				ytapdir = 1;
//			}
//
//			if (buttons & L_JPAD) {
//				xhelddir = -1;
//			}
//
//			if (buttonsnow & L_JPAD) {
//				xtapdir = -1;
//			}
//
//			if (buttons & R_JPAD) {
//				xhelddir = 1;
//			}
//
//			if (buttonsnow & R_JPAD) {
//				xtapdir = 1;
//			}
//		}
//
//		// Prevent select and going back on the same frame
//		if (inputs.select) {
//			inputs.back = 0;
//		}
//
//		if (ytapdir != 0) {
//			yhelddir = ytapdir;
//		}
//
//		if (xtapdir != 0) {
//			xhelddir = xtapdir;
//		}
//
//		// Choose repeat rate settings
//		digitalrepeatinterval = 10;
//		xdeadzone = 30;
//		ydeadzone = 20;
//		stickintervalbase = 60;
//		xstickintervalmult = 33;
//		ystickintervalmult = 44;
//		allowdiagonal = false;
//
//		if (g_Menus[g_MpPlayerNum].curdialog) {
//			struct menuitem *item = g_Menus[g_MpPlayerNum].curdialog->focuseditem;
//
//			if (item) {
//				if (item->type == MENUITEMTYPE_SLIDER || item->type == MENUITEMTYPE_10) {
//					if (g_Menus[g_MpPlayerNum].curdialog->dimmed) {
//						digitalrepeatinterval = TICKS(5);
//						xdeadzone = 20;
//						stickintervalbase = 30;
//						xstickintervalmult = 10;
//					}
//				}
//
//				if (item->type == MENUITEMTYPE_KEYBOARD) {
//					allowdiagonal = true;
//					digitalrepeatinterval = TICKS(5);
//					xdeadzone = 20;
//					xstickintervalmult = 10;
//					ystickintervalmult = 10;
//				}
//			}
//		}
//
//		// Handle left/right repeat
//		{
//			s32 absstickx;
//			s32 abssticky;
//			s32 oldslot;
//			s32 newslot;
//			s32 interval;
//			bool apply = false;
//
//			// This check doesn't exist in the up/down code later on...
//			// It's likely unnecessary
//			if (xhelddir == 0) {
//				menu->xrepeatmode = MENUREPEATMODE_RELEASED;
//			}
//
//			if (xtapdir != 0) {
//				// Direction was pressed this frame - reset the repeat properties
//				menu->xrepeatmode = MENUREPEATMODE_SLOW;
//				menu->xrepeattimer60 = 0;
//				menu->xrepeatdir = xtapdir;
//				apply = true;
//			} else if (xhelddir != 0) {
//				xhelddir = menu->xrepeatdir;
//			}
//
//			// If held for 1 second, repeat faster
//			if (menu->xrepeattimer60 > TICKS(60)) {
//				menu->xrepeatmode = MENUREPEATMODE_FAST;
//			}
//
//			// Calculate the old and new repeat slots.
//			// If these are different, the repeat will be applied on this tick.
//			oldslot = menu->xrepeattimer60 / digitalrepeatinterval;
//			newslot = (menu->xrepeattimer60 + g_Vars.diffframe60) / digitalrepeatinterval;
//
//			if (menu->xrepeatmode == MENUREPEATMODE_SLOW) {
//				oldslot /= 2;
//				newslot /= 2;
//			}
//
//			inputs.leftrightheld = xhelddir;
//
//			// Check if the stick is being pushed left or right
//			absstickx = stickx < 0 ? -stickx : stickx;
//			abssticky = sticky < 0 ? -sticky : sticky;
//
//			if (absstickx >= xdeadzone && (absstickx > abssticky || allowdiagonal)) {
//				// Reset the repeat if it's a different direction
//				if (stickx < 0 && menu->xrepeatcount > 0) {
//					menu->xrepeatcount = 0;
//				}
//
//				if (stickx > 0 && menu->xrepeatcount < 0) {
//					menu->xrepeatcount = 0;
//				}
//
//				if (menu->xrepeatcount == 0) {
//					menu->xrepeattimer60 = 0;
//				}
//
//				// Calculate the repeat interval based on the stick pressure
//				if (absstickx > 70) {
//					absstickx = 70;
//				}
//
//				absstickx -= xdeadzone;
//				interval = stickintervalbase - xstickintervalmult * absstickx / (70 - xdeadzone);
//
//				// After 3 repeats, halve the interval (ie. make faster)
//				if (menu->xrepeatcount >= 3 || menu->xrepeatcount <= -3) {
//					interval /= 2;
//				}
//
//				if (interval > 0) {
//#if PAL
//					if (interval > 3) {
//						interval = TICKS(interval);
//					}
//#endif
//					oldslot = menu->xrepeattimer60 / interval;
//					newslot = (menu->xrepeattimer60 + g_Vars.diffframe60) / interval;
//
//					xhelddir = stickx < 0 ? -1 : 1;
//
//					if (oldslot != newslot) {
//						apply = true;
//					}
//
//					if (menu->xrepeatcount == 0) {
//						apply = true;
//					}
//
//					if (apply) {
//						menu->xrepeatcount += xhelddir;
//					}
//				}
//			} else {
//				menu->xrepeatcount = 0;
//			}
//
//			if (oldslot != newslot) {
//				apply = true;
//			}
//
//			if (!apply) {
//				xhelddir = 0;
//			}
//		}
//
//		// Handle up/down repeat
//		{
//			s32 absstickx;
//			s32 abssticky;
//			s32 oldslot;
//			s32 newslot;
//			s32 interval;
//			bool apply = false;
//
//			if (ytapdir != 0) {
//				// Direction was pressed this frame - reset the repeat properties
//				apply = true;
//				menu->yrepeatmode = MENUREPEATMODE_SLOW;
//				menu->yrepeattimer60 = 0;
//				menu->yrepeatdir = ytapdir;
//			} else if (yhelddir != 0) {
//				yhelddir = menu->yrepeatdir;
//			}
//
//			// If held for 1 second, repeat faster
//			if (menu->yrepeattimer60 > TICKS(60)) {
//				menu->yrepeatmode = MENUREPEATMODE_FAST;
//			}
//
//			// Calculate the old and new repeat slots.
//			// If these are different, the repeat will be applied on this tick.
//			oldslot = menu->yrepeattimer60 / digitalrepeatinterval;
//			newslot = (menu->yrepeattimer60 + g_Vars.diffframe60) / digitalrepeatinterval;
//
//			if (menu->yrepeatmode == MENUREPEATMODE_SLOW) {
//				oldslot /= 2;
//				newslot /= 2;
//			}
//
//			inputs.updownheld = yhelddir;
//
//			// Check if the stick is being pushed up or down
//			abssticky = sticky < 0 ? -sticky : sticky;
//			absstickx = stickx < 0 ? -stickx : stickx;
//
//			if (abssticky >= ydeadzone && (abssticky > absstickx || allowdiagonal)) {
//				// Reset the repeat if it's a different direction
//				if (sticky < 0 && menu->yrepeatcount < 0) {
//					menu->yrepeatcount = 0;
//				}
//
//				if (sticky > 0 && menu->yrepeatcount > 0) {
//					menu->yrepeatcount = 0;
//				}
//
//				if (menu->yrepeatcount == 0) {
//					menu->yrepeattimer60 = 0;
//				}
//
//				// Calculate the repeat interval based on the stick pressure
//				if (abssticky > 70) {
//					abssticky = 70;
//				}
//
//				abssticky -= ydeadzone;
//				interval = stickintervalbase - ystickintervalmult * abssticky / 50;
//
//				// After 3 repeats, third the interval (ie. make faster)
//				if (menu->yrepeatcount >= 3 || menu->yrepeatcount <= -3) {
//					interval /= 3;
//				}
//
//				if (interval > 0) {
//#if PAL
//					if (interval > 3) {
//						interval = TICKS(interval);
//					}
//#endif
//
//					oldslot = menu->yrepeattimer60 / interval;
//					newslot = (menu->yrepeattimer60 + g_Vars.diffframe60) / interval;
//
//					yhelddir = sticky > 0 ? -1 : 1;
//
//					if (oldslot != newslot) {
//						apply = true;
//					}
//
//					if (menu->yrepeatcount == 0) {
//						apply = true;
//					}
//
//					if (apply) {
//						menu->yrepeatcount += yhelddir;
//					}
//				}
//			} else {
//				menu->yrepeatcount = 0;
//			}
//
//			if (oldslot != newslot) {
//				apply = true;
//			}
//
//			if (!apply) {
//				yhelddir = 0;
//			}
//		}
//
//		menu->xrepeattimer60 += g_Vars.diffframe60;
//		menu->yrepeattimer60 += g_Vars.diffframe60;
//
//		inputs.leftright = xhelddir;
//		inputs.updown = yhelddir;
//		inputs.xaxis = stickx;
//		inputs.yaxis = sticky;
//		inputs.unk14 = 0;
//		inputs.start = starttap ? true : false;
//
//		// Handle dialogs that allow pressing start to select,
//		// and handle pressing start on a list item.
//		if (g_Menus[g_MpPlayerNum].curdialog && starttap) {
//			struct menuitem *item;
//
//			if (g_Menus[g_MpPlayerNum].curdialog->definition->flags & MENUDIALOGFLAG_STARTSELECTS) {
//				inputs.select = true;
//				starttoselect = true;
//			}
//
//			item = g_Menus[g_MpPlayerNum].curdialog->focuseditem;
//
//			if (item && item->type == MENUITEMTYPE_LIST) {
//				inputs.select = true;
//			}
//		}
//
//		// Iterate all dialogs and give them the input for processing
//		{
//			bool foundcurrent = false;
//			s32 i;
//			s32 j;
//
//			for (i = 0; i < g_Menus[g_MpPlayerNum].depth; i++) {
//				struct menulayer *layer = &g_Menus[g_MpPlayerNum].layers[i];
//
//				for (j = 0; j < layer->numsiblings; j++) {
//					u32 tickflags = 0;
//
//					if (i == g_Menus[g_MpPlayerNum].depth - 1 && j == layer->cursibling && !foundcurrent) {
//						tickflags |= MENUTICKFLAG_DIALOGISCURRENT;
//						foundcurrent = true;
//					}
//
//					dialogTick(layer->siblings[j], &inputs, tickflags);
//				}
//			}
//		}
//
//		switch (g_MenuData.root) {
//		case MENUROOT_MPSETUP:
//		case MENUROOT_4MBMAINMENU:
//			// Allow pressing start on most MP setup dialogs to jump straight to
//			// the Ready dialog, or apply the quick start setup.
//			if (inputs.start && !starttoselect && g_Menus[g_MpPlayerNum].curdialog && !dialog->dimmed) {
//				if (g_Menus[g_MpPlayerNum].curdialog->definition);
//
//				if (g_Vars.mpsetupmenu != MPSETUPMENU_GENERAL
//						&& g_Menus[g_MpPlayerNum].curdialog->definition != &g_MpReadyMenuDialog) {
//					menuPushDialog(&g_MpReadyMenuDialog);
//				} else if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_MpQuickTeamGameSetupMenuDialog) {
//					func0f17f428();
//				}
//			}
//			break;
//		case MENUROOT_MPPAUSE:
//			if (g_InCutscene) {
//				func0f0f8120();
//			}
//			g_Menus[g_MpPlayerNum].openinhibit = 10;
//			// fall-through
//		case MENUROOT_ENDSCREEN:
//		case MENUROOT_MAINMENU:
//		case MENUROOT_MPENDSCREEN:
//		case MENUROOT_TRAINING:
//			if (inputs.start && !starttoselect && g_Menus[g_MpPlayerNum].curdialog
//					&& (dialog->definition->flags & MENUDIALOGFLAG_IGNOREBACK) == 0) {
//				func0f0f8120();
//			}
//			break;
//		}
//	}
//}

Gfx *menugfxRenderBgFailureAlt(Gfx *gdl);

/**
 * Render layer 1 of the menu background.
 *
 * Layer 1 is drawn before the hud piece. Almost everything is in layer 1.
 *
 * frac is used when transitioning between two backgrounds.
 * A value of 1 means draw this background with full alpha.
 */
Gfx *menuRenderBackgroundLayer1(Gfx *gdl, u8 bg, f32 frac)
{
	static u32 bblur = 1;

	switch (bg) {
	case MENUBG_BLUR:
		{
			u32 alpha = 255 * frac;

			// Render the blurred background texture with full alpha
			gdl = menugfxRenderBgBlur(gdl, 0xffffff00 | alpha, 0, 0);

			// Render it twice more with half alpha and offset
			gdl = menugfxRenderBgBlur(gdl, 0xffffff00 | alpha >> 1, -30, -30);
			gdl = menugfxRenderBgBlur(gdl, 0xffffff00 | alpha >> 1, 30, 30);
		}
		break;
	case MENUBG_BLACK:
	case MENUBG_8:
		{
			u32 colour = 255 * frac;
			gSPDisplayList(gdl++, var800613a0);
			gdl = textSetPrimColour(gdl, colour);
			gDPFillRectangle(gdl++, 0, 0, viGetWidth(), viGetHeight());
			gdl = text0f153838(gdl);
		}
		break;
	case MENUBG_SUCCESS:
		{
			// Fill with black
			gSPDisplayList(gdl++, var800613a0);
			gdl = textSetPrimColour(gdl, 0x000000ff);
			gDPFillRectangle(gdl++, 0, 0, viGetWidth(), viGetHeight());
			gdl = text0f153838(gdl);

			// Render the success BG
			gdl = menugfxRenderBgSuccess(gdl);

			// Render alpha black if fading in
			{
				u32 alpha = (1.0f - frac) * 255;

				if (alpha) {
					gSPDisplayList(gdl++, var800613a0);
					gdl = textSetPrimColour(gdl, alpha);
					gDPFillRectangle(gdl++, 0, 0, viGetWidth(), viGetHeight());
					gdl = text0f153838(gdl);
				}
			}
		}
		break;
	case MENUBG_FAILURE:
		{
			// Fill with white -> black while fading in
			u32 stack;
			u32 channel = (1.0f - frac) * 255;
			gSPDisplayList(gdl++, var800613a0);
			gdl = textSetPrimColour(gdl, channel << 24 | channel << 16 | channel << 8 | 0xff);
			gDPFillRectangle(gdl++, 0, 0, viGetWidth(), viGetHeight());
			gdl = text0f153838(gdl);

			// Render the failure BG
			gdl = menugfxRenderBgFailure(gdl);
		}
		break;
	case MENUBG_CONEALPHA:
		mainOverrideVariable("bblur", &bblur);

		if (g_MenuData.screenshottimer) {
			return gdl;
		}

		if (bblur) {
			// Render the blurred background
			gdl = menugfxRenderBgBlur(gdl, 0xffffffff, 0, 0);

			// While fading, render red
			if (frac < 1.0f) {
				u32 alpha;
				u32 stack;

				gSPDisplayList(gdl++, var800613a0);
				alpha = (1.0f - frac) * 255;
				gdl = textSetPrimColour(gdl, 0xff000000 | alpha);
				gDPFillRectangle(gdl++, 0, 0, viGetWidth(), viGetHeight());
				gdl = text0f153838(gdl);
			}
		}
		break;
	case MENUBG_GRADIENT:
		// Blue to red
		gdl = menugfxRenderGradient(gdl, 0, 0, viGetWidth(), viGetHeight(), 0x00007f7f, 0x000000ff, 0x8f0000ff);
		break;
	case MENUBG_CONEOPAQUE:
		// Yellow to yellow (ie. not a gradient)
		gdl = menugfxRenderGradient(gdl, 0, 0, viGetWidth(), viGetHeight(), 0x3f3f00ff, 0x7f0000ff, 0x3f3f00ff);
		break;
	}

	return gdl;
}

u32 var800714f0 = 1;

Gfx *menuRenderBackgroundLayer2(Gfx *gdl, u8 bg, f32 frac)
{
	if (bg == MENUBG_CONEALPHA || bg == MENUBG_CONEOPAQUE) {
		mainOverrideVariable("cone", &var800714f0);

		if (var800714f0
				&& (g_MenuData.nextbg == MENUBG_CONEALPHA || g_MenuData.nextbg == 0 || g_MenuData.nextbg == 255)) {
			gdl = menugfxRenderBgCone(gdl);
		}
	}

	return gdl;
}

#if VERSION >= VERSION_JPN_FINAL
u32 usepiece = 1;
const char var7f1b34b4jf[] = "usePiece";
const char var7f1b34c0jf[] = "%s%s";
const char var7f1b34c8jf[] = "%s%s";

GLOBAL_ASM(
glabel menuRender
.late_rodata
glabel var7f1b381cjf
.word 0
.word 0x3f847ae1
.word 0x47ae147b
.word 0xc34d8000
.word 0x4374b333
.text
/*  f0fc924:	27bdfee8 */ 	addiu	$sp,$sp,-280
/*  f0fc928:	afb10038 */ 	sw	$s1,0x38($sp)
/*  f0fc92c:	3c118007 */ 	lui	$s1,0x8007
/*  f0fc930:	26311998 */ 	addiu	$s1,$s1,0x1998
/*  f0fc934:	ae200000 */ 	sw	$zero,0x0($s1)
/*  f0fc938:	3c0e8007 */ 	lui	$t6,0x8007
/*  f0fc93c:	8dce0c18 */ 	lw	$t6,0xc18($t6)
/*  f0fc940:	afb50048 */ 	sw	$s5,0x48($sp)
/*  f0fc944:	24020001 */ 	li	$v0,0x1
/*  f0fc948:	0080a825 */ 	move	$s5,$a0
/*  f0fc94c:	afbf0054 */ 	sw	$ra,0x54($sp)
/*  f0fc950:	afb70050 */ 	sw	$s7,0x50($sp)
/*  f0fc954:	afb6004c */ 	sw	$s6,0x4c($sp)
/*  f0fc958:	afb40044 */ 	sw	$s4,0x44($sp)
/*  f0fc95c:	afb30040 */ 	sw	$s3,0x40($sp)
/*  f0fc960:	afb2003c */ 	sw	$s2,0x3c($sp)
/*  f0fc964:	144e0005 */ 	bne	$v0,$t6,.JF0f0fc97c
/*  f0fc968:	afb00034 */ 	sw	$s0,0x34($sp)
/*  f0fc96c:	240f0002 */ 	li	$t7,0x2
/*  f0fc970:	3c018008 */ 	lui	$at,0x8008
/*  f0fc974:	10000003 */ 	b	.JF0f0fc984
/*  f0fc978:	ac2f00f4 */ 	sw	$t7,0xf4($at)
.JF0f0fc97c:
/*  f0fc97c:	3c018008 */ 	lui	$at,0x8008
/*  f0fc980:	ac2200f4 */ 	sw	$v0,0xf4($at)
.JF0f0fc984:
/*  f0fc984:	0fc35617 */ 	jal	func0f0d479c
/*  f0fc988:	02a02025 */ 	move	$a0,$s5
/*  f0fc98c:	3c138006 */ 	lui	$s3,0x8006
/*  f0fc990:	3c17800a */ 	lui	$s7,0x800a
/*  f0fc994:	26731280 */ 	addiu	$s3,$s3,0x1280
/*  f0fc998:	3c180600 */ 	lui	$t8,0x600
/*  f0fc99c:	26f720c0 */ 	addiu	$s7,$s7,0x20c0
/*  f0fc9a0:	ac580000 */ 	sw	$t8,0x0($v0)
/*  f0fc9a4:	ac530004 */ 	sw	$s3,0x4($v0)
/*  f0fc9a8:	92e30015 */ 	lbu	$v1,0x15($s7)
/*  f0fc9ac:	24550008 */ 	addiu	$s5,$v0,0x8
/*  f0fc9b0:	240100ff */ 	li	$at,0xff
/*  f0fc9b4:	10610017 */ 	beq	$v1,$at,.JF0f0fca14
/*  f0fc9b8:	02a02025 */ 	move	$a0,$s5
/*  f0fc9bc:	1460000c */ 	bnez	$v1,.JF0f0fc9f0
/*  f0fc9c0:	02a02025 */ 	move	$a0,$s5
/*  f0fc9c4:	3c013f80 */ 	lui	$at,0x3f80
/*  f0fc9c8:	44812000 */ 	mtc1	$at,$f4
/*  f0fc9cc:	c6e60010 */ 	lwc1	$f6,0x10($s7)
/*  f0fc9d0:	02a02025 */ 	move	$a0,$s5
/*  f0fc9d4:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fc9d8:	46062201 */ 	sub.s	$f8,$f4,$f6
/*  f0fc9dc:	44064000 */ 	mfc1	$a2,$f8
/*  f0fc9e0:	0fc3f05f */ 	jal	menuRenderBackgroundLayer1
/*  f0fc9e4:	00000000 */ 	nop
/*  f0fc9e8:	1000000e */ 	b	.JF0f0fca24
/*  f0fc9ec:	0040a825 */ 	move	$s5,$v0
.JF0f0fc9f0:
/*  f0fc9f0:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fc9f4:	0fc3f05f */ 	jal	menuRenderBackgroundLayer1
/*  f0fc9f8:	3c063f80 */ 	lui	$a2,0x3f80
/*  f0fc9fc:	00402025 */ 	move	$a0,$v0
/*  f0fca00:	92e50015 */ 	lbu	$a1,0x15($s7)
/*  f0fca04:	0fc3f05f */ 	jal	menuRenderBackgroundLayer1
/*  f0fca08:	8ee60010 */ 	lw	$a2,0x10($s7)
/*  f0fca0c:	10000005 */ 	b	.JF0f0fca24
/*  f0fca10:	0040a825 */ 	move	$s5,$v0
.JF0f0fca14:
/*  f0fca14:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fca18:	0fc3f05f */ 	jal	menuRenderBackgroundLayer1
/*  f0fca1c:	3c063f80 */ 	lui	$a2,0x3f80
/*  f0fca20:	0040a825 */ 	move	$s5,$v0
.JF0f0fca24:
/*  f0fca24:	8ef905d4 */ 	lw	$t9,0x5d4($s7)
/*  f0fca28:	240c040d */ 	li	$t4,0x40d
/*  f0fca2c:	240f0001 */ 	li	$t7,0x1
/*  f0fca30:	00195b00 */ 	sll	$t3,$t9,0xc
/*  f0fca34:	0561000b */ 	bgez	$t3,.JF0f0fca64
/*  f0fca38:	3c0a8009 */ 	lui	$t2,0x8009
/*  f0fca3c:	92ed05cd */ 	lbu	$t5,0x5cd($s7)
/*  f0fca40:	92f805d5 */ 	lbu	$t8,0x5d5($s7)
/*  f0fca44:	a6e0007a */ 	sh	$zero,0x7a($s7)
/*  f0fca48:	31aeffef */ 	andi	$t6,$t5,0xffef
/*  f0fca4c:	3319fff7 */ 	andi	$t9,$t8,0xfff7
/*  f0fca50:	a6ec0078 */ 	sh	$t4,0x78($s7)
/*  f0fca54:	aee0059c */ 	sw	$zero,0x59c($s7)
/*  f0fca58:	a2ee05cd */ 	sb	$t6,0x5cd($s7)
/*  f0fca5c:	a2ef05d4 */ 	sb	$t7,0x5d4($s7)
/*  f0fca60:	a2f905d5 */ 	sb	$t9,0x5d5($s7)
.JF0f0fca64:
/*  f0fca64:	914a1160 */ 	lbu	$t2,0x1160($t2)
/*  f0fca68:	24010001 */ 	li	$at,0x1
/*  f0fca6c:	114100ab */ 	beq	$t2,$at,.JF0f0fcd1c
/*  f0fca70:	00000000 */ 	nop
/*  f0fca74:	92eb05d4 */ 	lbu	$t3,0x5d4($s7)
/*  f0fca78:	02a01025 */ 	move	$v0,$s5
/*  f0fca7c:	3c0cb700 */ 	lui	$t4,0xb700
/*  f0fca80:	116000a6 */ 	beqz	$t3,.JF0f0fcd1c
/*  f0fca84:	240d0001 */ 	li	$t5,0x1
/*  f0fca88:	00008025 */ 	move	$s0,$zero
/*  f0fca8c:	26b50008 */ 	addiu	$s5,$s5,0x8
/*  f0fca90:	ac4c0000 */ 	sw	$t4,0x0($v0)
/*  f0fca94:	0c004b60 */ 	jal	random
/*  f0fca98:	ac4d0004 */ 	sw	$t5,0x4($v0)
/*  f0fca9c:	44825000 */ 	mtc1	$v0,$f10
/*  f0fcaa0:	3c014f80 */ 	lui	$at,0x4f80
/*  f0fcaa4:	04410004 */ 	bgez	$v0,.JF0f0fcab8
/*  f0fcaa8:	46805420 */ 	cvt.s.w	$f16,$f10
/*  f0fcaac:	44819000 */ 	mtc1	$at,$f18
/*  f0fcab0:	00000000 */ 	nop
/*  f0fcab4:	46128400 */ 	add.s	$f16,$f16,$f18
.JF0f0fcab8:
/*  f0fcab8:	3c012f80 */ 	lui	$at,0x2f80
/*  f0fcabc:	44812000 */ 	mtc1	$at,$f4
/*  f0fcac0:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fcac4:	d42a3820 */ 	ldc1	$f10,0x3820($at)
/*  f0fcac8:	46048182 */ 	mul.s	$f6,$f16,$f4
/*  f0fcacc:	46003221 */ 	cvt.d.s	$f8,$f6
/*  f0fcad0:	462a403c */ 	c.lt.d	$f8,$f10
/*  f0fcad4:	00000000 */ 	nop
/*  f0fcad8:	4502002d */ 	bc1fl	.JF0f0fcb90
/*  f0fcadc:	8ee30004 */ 	lw	$v1,0x4($s7)
/*  f0fcae0:	0c004b60 */ 	jal	random
/*  f0fcae4:	00000000 */ 	nop
/*  f0fcae8:	44829000 */ 	mtc1	$v0,$f18
/*  f0fcaec:	3c014f80 */ 	lui	$at,0x4f80
/*  f0fcaf0:	04410004 */ 	bgez	$v0,.JF0f0fcb04
/*  f0fcaf4:	46809420 */ 	cvt.s.w	$f16,$f18
/*  f0fcaf8:	44812000 */ 	mtc1	$at,$f4
/*  f0fcafc:	00000000 */ 	nop
/*  f0fcb00:	46048400 */ 	add.s	$f16,$f16,$f4
.JF0f0fcb04:
/*  f0fcb04:	3c012f80 */ 	lui	$at,0x2f80
/*  f0fcb08:	44813000 */ 	mtc1	$at,$f6
/*  f0fcb0c:	3c0142a0 */ 	lui	$at,0x42a0
/*  f0fcb10:	44815000 */ 	mtc1	$at,$f10
/*  f0fcb14:	46068202 */ 	mul.s	$f8,$f16,$f6
/*  f0fcb18:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fcb1c:	c4243828 */ 	lwc1	$f4,0x3828($at)
/*  f0fcb20:	3c014220 */ 	lui	$at,0x4220
/*  f0fcb24:	44813000 */ 	mtc1	$at,$f6
/*  f0fcb28:	460a4482 */ 	mul.s	$f18,$f8,$f10
/*  f0fcb2c:	46049400 */ 	add.s	$f16,$f18,$f4
/*  f0fcb30:	46068201 */ 	sub.s	$f8,$f16,$f6
/*  f0fcb34:	0c004b60 */ 	jal	random
/*  f0fcb38:	e6e80554 */ 	swc1	$f8,0x554($s7)
/*  f0fcb3c:	44825000 */ 	mtc1	$v0,$f10
/*  f0fcb40:	3c014f80 */ 	lui	$at,0x4f80
/*  f0fcb44:	04410004 */ 	bgez	$v0,.JF0f0fcb58
/*  f0fcb48:	468054a0 */ 	cvt.s.w	$f18,$f10
/*  f0fcb4c:	44812000 */ 	mtc1	$at,$f4
/*  f0fcb50:	00000000 */ 	nop
/*  f0fcb54:	46049480 */ 	add.s	$f18,$f18,$f4
.JF0f0fcb58:
/*  f0fcb58:	3c012f80 */ 	lui	$at,0x2f80
/*  f0fcb5c:	44818000 */ 	mtc1	$at,$f16
/*  f0fcb60:	3c0142a0 */ 	lui	$at,0x42a0
/*  f0fcb64:	44814000 */ 	mtc1	$at,$f8
/*  f0fcb68:	46109182 */ 	mul.s	$f6,$f18,$f16
/*  f0fcb6c:	3c017f1b */ 	lui	$at,0x7f1b
/*  f0fcb70:	c424382c */ 	lwc1	$f4,0x382c($at)
/*  f0fcb74:	3c014220 */ 	lui	$at,0x4220
/*  f0fcb78:	44818000 */ 	mtc1	$at,$f16
/*  f0fcb7c:	46083282 */ 	mul.s	$f10,$f6,$f8
/*  f0fcb80:	46045480 */ 	add.s	$f18,$f10,$f4
/*  f0fcb84:	46109181 */ 	sub.s	$f6,$f18,$f16
/*  f0fcb88:	e6e60558 */ 	swc1	$f6,0x558($s7)
/*  f0fcb8c:	8ee30004 */ 	lw	$v1,0x4($s7)
.JF0f0fcb90:
/*  f0fcb90:	3c12800a */ 	lui	$s2,0x800a
/*  f0fcb94:	3c14800a */ 	lui	$s4,0x800a
/*  f0fcb98:	2694e568 */ 	addiu	$s4,$s4,-6808
/*  f0fcb9c:	2652e56c */ 	addiu	$s2,$s2,-6804
/*  f0fcba0:	24060003 */ 	li	$a2,0x3
/*  f0fcba4:	ae400000 */ 	sw	$zero,0x0($s2)
/*  f0fcba8:	14c30005 */ 	bne	$a2,$v1,.JF0f0fcbc0
/*  f0fcbac:	ae800000 */ 	sw	$zero,0x0($s4)
/*  f0fcbb0:	8eef0000 */ 	lw	$t7,0x0($s7)
/*  f0fcbb4:	5de00003 */ 	bgtzl	$t7,.JF0f0fcbc4
/*  f0fcbb8:	24010002 */ 	li	$at,0x2
/*  f0fcbbc:	24100001 */ 	li	$s0,0x1
.JF0f0fcbc0:
/*  f0fcbc0:	24010002 */ 	li	$at,0x2
.JF0f0fcbc4:
/*  f0fcbc4:	10610008 */ 	beq	$v1,$at,.JF0f0fcbe8
/*  f0fcbc8:	3c16800a */ 	lui	$s6,0x800a
/*  f0fcbcc:	10c30006 */ 	beq	$a2,$v1,.JF0f0fcbe8
/*  f0fcbd0:	24010006 */ 	li	$at,0x6
/*  f0fcbd4:	10610004 */ 	beq	$v1,$at,.JF0f0fcbe8
/*  f0fcbd8:	2401000d */ 	li	$at,0xd
/*  f0fcbdc:	50610003 */ 	beql	$v1,$at,.JF0f0fcbec
/*  f0fcbe0:	8e380000 */ 	lw	$t8,0x0($s1)
/*  f0fcbe4:	24100001 */ 	li	$s0,0x1
.JF0f0fcbe8:
/*  f0fcbe8:	8e380000 */ 	lw	$t8,0x0($s1)
.JF0f0fcbec:
/*  f0fcbec:	3c0a800a */ 	lui	$t2,0x800a
/*  f0fcbf0:	3c047f1b */ 	lui	$a0,0x7f1b
/*  f0fcbf4:	0018c8c0 */ 	sll	$t9,$t8,0x3
/*  f0fcbf8:	0338c823 */ 	subu	$t9,$t9,$t8
/*  f0fcbfc:	0019c880 */ 	sll	$t9,$t9,0x2
/*  f0fcc00:	0338c821 */ 	addu	$t9,$t9,$t8
/*  f0fcc04:	0019c8c0 */ 	sll	$t9,$t9,0x3
/*  f0fcc08:	0338c823 */ 	subu	$t9,$t9,$t8
/*  f0fcc0c:	0019c900 */ 	sll	$t9,$t9,0x4
/*  f0fcc10:	01595021 */ 	addu	$t2,$t2,$t9
/*  f0fcc14:	8d4aebf8 */ 	lw	$t2,-0x1408($t2)
/*  f0fcc18:	26d6a630 */ 	addiu	$s6,$s6,-22992
/*  f0fcc1c:	248434b4 */ 	addiu	$a0,$a0,0x34b4
/*  f0fcc20:	15400004 */ 	bnez	$t2,.JF0f0fcc34
/*  f0fcc24:	3c058007 */ 	lui	$a1,0x8007
/*  f0fcc28:	50c30003 */ 	beql	$a2,$v1,.JF0f0fcc38
/*  f0fcc2c:	8ec30284 */ 	lw	$v1,0x284($s6)
/*  f0fcc30:	24100001 */ 	li	$s0,0x1
.JF0f0fcc34:
/*  f0fcc34:	8ec30284 */ 	lw	$v1,0x284($s6)
.JF0f0fcc38:
/*  f0fcc38:	8c620480 */ 	lw	$v0,0x480($v1)
/*  f0fcc3c:	1040000c */ 	beqz	$v0,.JF0f0fcc70
/*  f0fcc40:	00000000 */ 	nop
/*  f0fcc44:	8c6c1c54 */ 	lw	$t4,0x1c54($v1)
/*  f0fcc48:	8c6b00c4 */ 	lw	$t3,0xc4($v1)
/*  f0fcc4c:	01806827 */ 	nor	$t5,$t4,$zero
/*  f0fcc50:	016d7024 */ 	and	$t6,$t3,$t5
/*  f0fcc54:	31cf0004 */ 	andi	$t7,$t6,0x4
/*  f0fcc58:	11e00005 */ 	beqz	$t7,.JF0f0fcc70
/*  f0fcc5c:	00000000 */ 	nop
/*  f0fcc60:	80580037 */ 	lb	$t8,0x37($v0)
/*  f0fcc64:	13000002 */ 	beqz	$t8,.JF0f0fcc70
/*  f0fcc68:	00000000 */ 	nop
/*  f0fcc6c:	24100001 */ 	li	$s0,0x1
.JF0f0fcc70:
/*  f0fcc70:	12000012 */ 	beqz	$s0,.JF0f0fccbc
/*  f0fcc74:	00000000 */ 	nop
/*  f0fcc78:	8ef9059c */ 	lw	$t9,0x59c($s7)
/*  f0fcc7c:	240d040d */ 	li	$t5,0x40d
/*  f0fcc80:	240e0001 */ 	li	$t6,0x1
/*  f0fcc84:	57200009 */ 	bnezl	$t9,.JF0f0fccac
/*  f0fcc88:	86ef007a */ 	lh	$t7,0x7a($s7)
/*  f0fcc8c:	92ec05cd */ 	lbu	$t4,0x5cd($s7)
/*  f0fcc90:	a6e0007a */ 	sh	$zero,0x7a($s7)
/*  f0fcc94:	a6ed0078 */ 	sh	$t5,0x78($s7)
/*  f0fcc98:	358b0010 */ 	ori	$t3,$t4,0x10
/*  f0fcc9c:	a2eb05cd */ 	sb	$t3,0x5cd($s7)
/*  f0fcca0:	10000006 */ 	b	.JF0f0fccbc
/*  f0fcca4:	aeee059c */ 	sw	$t6,0x59c($s7)
/*  f0fcca8:	86ef007a */ 	lh	$t7,0x7a($s7)
.JF0f0fccac:
/*  f0fccac:	15e00003 */ 	bnez	$t7,.JF0f0fccbc
/*  f0fccb0:	00000000 */ 	nop
/*  f0fccb4:	aee0059c */ 	sw	$zero,0x59c($s7)
/*  f0fccb8:	a2e005d4 */ 	sb	$zero,0x5d4($s7)
.JF0f0fccbc:
/*  f0fccbc:	0c0036b4 */ 	jal	mainOverrideVariable
/*  f0fccc0:	24a51a44 */ 	addiu	$a1,$a1,0x1a44
/*  f0fccc4:	3c188007 */ 	lui	$t8,0x8007
/*  f0fccc8:	8f181a44 */ 	lw	$t8,0x1a44($t8)
/*  f0fcccc:	02a02025 */ 	move	$a0,$s5
/*  f0fccd0:	3c05800a */ 	lui	$a1,0x800a
/*  f0fccd4:	1300000f */ 	beqz	$t8,.JF0f0fcd14
/*  f0fccd8:	00000000 */ 	nop
/*  f0fccdc:	92f905d5 */ 	lbu	$t9,0x5d5($s7)
/*  f0fcce0:	24a520dc */ 	addiu	$a1,$a1,0x20dc
/*  f0fcce4:	24060001 */ 	li	$a2,0x1
/*  f0fcce8:	332affdf */ 	andi	$t2,$t9,0xffdf
/*  f0fccec:	0fc3d151 */ 	jal	menuRenderModels
/*  f0fccf0:	a2ea05d5 */ 	sb	$t2,0x5d5($s7)
/*  f0fccf4:	3c0cb600 */ 	lui	$t4,0xb600
/*  f0fccf8:	240b0001 */ 	li	$t3,0x1
/*  f0fccfc:	ac4b0004 */ 	sw	$t3,0x4($v0)
/*  f0fcd00:	ac4c0000 */ 	sw	$t4,0x0($v0)
/*  f0fcd04:	92ee05d5 */ 	lbu	$t6,0x5d5($s7)
/*  f0fcd08:	24550008 */ 	addiu	$s5,$v0,0x8
/*  f0fcd0c:	35cf0020 */ 	ori	$t7,$t6,0x20
/*  f0fcd10:	a2ef05d5 */ 	sb	$t7,0x5d5($s7)
.JF0f0fcd14:
/*  f0fcd14:	1000000a */ 	b	.JF0f0fcd40
/*  f0fcd18:	8ee205d4 */ 	lw	$v0,0x5d4($s7)
.JF0f0fcd1c:
/*  f0fcd1c:	3c12800a */ 	lui	$s2,0x800a
/*  f0fcd20:	3c14800a */ 	lui	$s4,0x800a
/*  f0fcd24:	2694e568 */ 	addiu	$s4,$s4,-6808
/*  f0fcd28:	2652e56c */ 	addiu	$s2,$s2,-6804
/*  f0fcd2c:	3c16800a */ 	lui	$s6,0x800a
/*  f0fcd30:	26d6a630 */ 	addiu	$s6,$s6,-22992
/*  f0fcd34:	ae400000 */ 	sw	$zero,0x0($s2)
/*  f0fcd38:	ae800000 */ 	sw	$zero,0x0($s4)
/*  f0fcd3c:	8ee205d4 */ 	lw	$v0,0x5d4($s7)
.JF0f0fcd40:
/*  f0fcd40:	000252c0 */ 	sll	$t2,$v0,0xb
/*  f0fcd44:	05410005 */ 	bgez	$t2,.JF0f0fcd5c
/*  f0fcd48:	02a02025 */ 	move	$a0,$s5
/*  f0fcd4c:	8eec0670 */ 	lw	$t4,0x670($s7)
/*  f0fcd50:	8eeb0674 */ 	lw	$t3,0x674($s7)
/*  f0fcd54:	ae8c0000 */ 	sw	$t4,0x0($s4)
/*  f0fcd58:	ae4b0000 */ 	sw	$t3,0x0($s2)
.JF0f0fcd5c:
/*  f0fcd5c:	92e30015 */ 	lbu	$v1,0x15($s7)
/*  f0fcd60:	240100ff */ 	li	$at,0xff
/*  f0fcd64:	3c063f80 */ 	lui	$a2,0x3f80
/*  f0fcd68:	10610017 */ 	beq	$v1,$at,.JF0f0fcdc8
/*  f0fcd6c:	00000000 */ 	nop
/*  f0fcd70:	1460000c */ 	bnez	$v1,.JF0f0fcda4
/*  f0fcd74:	02a02025 */ 	move	$a0,$s5
/*  f0fcd78:	3c013f80 */ 	lui	$at,0x3f80
/*  f0fcd7c:	44814000 */ 	mtc1	$at,$f8
/*  f0fcd80:	c6ea0010 */ 	lwc1	$f10,0x10($s7)
/*  f0fcd84:	02a02025 */ 	move	$a0,$s5
/*  f0fcd88:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fcd8c:	460a4101 */ 	sub.s	$f4,$f8,$f10
/*  f0fcd90:	44062000 */ 	mfc1	$a2,$f4
/*  f0fcd94:	0fc3f225 */ 	jal	menuRenderBackgroundLayer2
/*  f0fcd98:	00000000 */ 	nop
/*  f0fcd9c:	1000000d */ 	b	.JF0f0fcdd4
/*  f0fcda0:	0040a825 */ 	move	$s5,$v0
.JF0f0fcda4:
/*  f0fcda4:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fcda8:	0fc3f225 */ 	jal	menuRenderBackgroundLayer2
/*  f0fcdac:	3c063f80 */ 	lui	$a2,0x3f80
/*  f0fcdb0:	00402025 */ 	move	$a0,$v0
/*  f0fcdb4:	92e50015 */ 	lbu	$a1,0x15($s7)
/*  f0fcdb8:	0fc3f225 */ 	jal	menuRenderBackgroundLayer2
/*  f0fcdbc:	8ee60010 */ 	lw	$a2,0x10($s7)
/*  f0fcdc0:	10000004 */ 	b	.JF0f0fcdd4
/*  f0fcdc4:	0040a825 */ 	move	$s5,$v0
.JF0f0fcdc8:
/*  f0fcdc8:	0fc3f225 */ 	jal	menuRenderBackgroundLayer2
/*  f0fcdcc:	92e50014 */ 	lbu	$a1,0x14($s7)
/*  f0fcdd0:	0040a825 */ 	move	$s5,$v0
.JF0f0fcdd4:
/*  f0fcdd4:	92ed0014 */ 	lbu	$t5,0x14($s7)
/*  f0fcdd8:	55a00006 */ 	bnezl	$t5,.JF0f0fcdf4
/*  f0fcddc:	8ecf0284 */ 	lw	$t7,0x284($s6)
/*  f0fcde0:	92ee0015 */ 	lbu	$t6,0x15($s7)
/*  f0fcde4:	240100ff */ 	li	$at,0xff
/*  f0fcde8:	51c10010 */ 	beql	$t6,$at,.JF0f0fce2c
/*  f0fcdec:	8ef90000 */ 	lw	$t9,0x0($s7)
/*  f0fcdf0:	8ecf0284 */ 	lw	$t7,0x284($s6)
.JF0f0fcdf4:
/*  f0fcdf4:	8de20480 */ 	lw	$v0,0x480($t7)
/*  f0fcdf8:	10400004 */ 	beqz	$v0,.JF0f0fce0c
/*  f0fcdfc:	00000000 */ 	nop
/*  f0fce00:	80580037 */ 	lb	$t8,0x37($v0)
/*  f0fce04:	57000009 */ 	bnezl	$t8,.JF0f0fce2c
/*  f0fce08:	8ef90000 */ 	lw	$t9,0x0($s7)
.JF0f0fce0c:
/*  f0fce0c:	0fc356a2 */ 	jal	func0f0d49c8
/*  f0fce10:	02a02025 */ 	move	$a0,$s5
/*  f0fce14:	0fc2f3fe */ 	jal	playerRenderHealthBar
/*  f0fce18:	00402025 */ 	move	$a0,$v0
/*  f0fce1c:	0fc35617 */ 	jal	func0f0d479c
/*  f0fce20:	00402025 */ 	move	$a0,$v0
/*  f0fce24:	0040a825 */ 	move	$s5,$v0
/*  f0fce28:	8ef90000 */ 	lw	$t9,0x0($s7)
.JF0f0fce2c:
/*  f0fce2c:	5b20017c */ 	blezl	$t9,.JF0f0fd420
/*  f0fce30:	82ef001b */ 	lb	$t7,0x1b($s7)
/*  f0fce34:	0fc54c87 */ 	jal	text0f153ab0
/*  f0fce38:	02a02025 */ 	move	$a0,$s5
/*  f0fce3c:	8ee30004 */ 	lw	$v1,0x4($s7)
/*  f0fce40:	24010004 */ 	li	$at,0x4
/*  f0fce44:	0040a825 */ 	move	$s5,$v0
/*  f0fce48:	10610003 */ 	beq	$v1,$at,.JF0f0fce58
/*  f0fce4c:	24010005 */ 	li	$at,0x5
/*  f0fce50:	14610008 */ 	bne	$v1,$at,.JF0f0fce74
/*  f0fce54:	00008025 */ 	move	$s0,$zero
.JF0f0fce58:
/*  f0fce58:	8eca0288 */ 	lw	$t2,0x288($s6)
/*  f0fce5c:	02a02025 */ 	move	$a0,$s5
/*  f0fce60:	8d4c0070 */ 	lw	$t4,0x70($t2)
/*  f0fce64:	0fc3e4e8 */ 	jal	menuRenderDialogs
/*  f0fce68:	ae2c0000 */ 	sw	$t4,0x0($s1)
/*  f0fce6c:	10000008 */ 	b	.JF0f0fce90
/*  f0fce70:	0040a825 */ 	move	$s5,$v0
.JF0f0fce74:
/*  f0fce74:	ae300000 */ 	sw	$s0,0x0($s1)
/*  f0fce78:	0fc3e4e8 */ 	jal	menuRenderDialogs
/*  f0fce7c:	02a02025 */ 	move	$a0,$s5
/*  f0fce80:	26100001 */ 	addiu	$s0,$s0,0x1
/*  f0fce84:	24010004 */ 	li	$at,0x4
/*  f0fce88:	1601fffa */ 	bne	$s0,$at,.JF0f0fce74
/*  f0fce8c:	0040a825 */ 	move	$s5,$v0
.JF0f0fce90:
/*  f0fce90:	3c0b0103 */ 	lui	$t3,0x103
/*  f0fce94:	ae200000 */ 	sw	$zero,0x0($s1)
/*  f0fce98:	356b0040 */ 	ori	$t3,$t3,0x40
/*  f0fce9c:	02a08025 */ 	move	$s0,$s5
/*  f0fcea0:	ae0b0000 */ 	sw	$t3,0x0($s0)
/*  f0fcea4:	0fc2d81a */ 	jal	camGetPerspectiveMtxL
/*  f0fcea8:	26b50008 */ 	addiu	$s5,$s5,0x8
/*  f0fceac:	0c012b34 */ 	jal	osVirtualToPhysical
/*  f0fceb0:	00402025 */ 	move	$a0,$v0
/*  f0fceb4:	02a01825 */ 	move	$v1,$s5
/*  f0fceb8:	ae020004 */ 	sw	$v0,0x4($s0)
/*  f0fcebc:	3c0d0600 */ 	lui	$t5,0x600
/*  f0fcec0:	ac6d0000 */ 	sw	$t5,0x0($v1)
/*  f0fcec4:	ac730004 */ 	sw	$s3,0x4($v1)
/*  f0fcec8:	0fc54cab */ 	jal	text0f153b40
/*  f0fcecc:	26b50008 */ 	addiu	$s5,$s5,0x8
/*  f0fced0:	8ee30004 */ 	lw	$v1,0x4($s7)
/*  f0fced4:	24010003 */ 	li	$at,0x3
/*  f0fced8:	10610003 */ 	beq	$v1,$at,.JF0f0fcee8
/*  f0fcedc:	2401000b */ 	li	$at,0xb
/*  f0fcee0:	54610149 */ 	bnel	$v1,$at,.JF0f0fd408
/*  f0fcee4:	02a01025 */ 	move	$v0,$s5
.JF0f0fcee8:
/*  f0fcee8:	0c002f29 */ 	jal	viGetViewLeft
/*  f0fceec:	00000000 */ 	nop
/*  f0fcef0:	3c0e8008 */ 	lui	$t6,0x8008
/*  f0fcef4:	8dce00f4 */ 	lw	$t6,0xf4($t6)
/*  f0fcef8:	004e001a */ 	div	$zero,$v0,$t6
/*  f0fcefc:	00007812 */ 	mflo	$t7
/*  f0fcf00:	25f80014 */ 	addiu	$t8,$t7,0x14
/*  f0fcf04:	15c00002 */ 	bnez	$t6,.JF0f0fcf10
/*  f0fcf08:	00000000 */ 	nop
/*  f0fcf0c:	0007000d */ 	break	0x7
.JF0f0fcf10:
/*  f0fcf10:	2401ffff */ 	li	$at,-1
/*  f0fcf14:	15c10004 */ 	bne	$t6,$at,.JF0f0fcf28
/*  f0fcf18:	3c018000 */ 	lui	$at,0x8000
/*  f0fcf1c:	14410002 */ 	bne	$v0,$at,.JF0f0fcf28
/*  f0fcf20:	00000000 */ 	nop
/*  f0fcf24:	0006000d */ 	break	0x6
.JF0f0fcf28:
/*  f0fcf28:	afb800f0 */ 	sw	$t8,0xf0($sp)
/*  f0fcf2c:	0c002f2d */ 	jal	viGetViewTop
/*  f0fcf30:	00000000 */ 	nop
/*  f0fcf34:	24590004 */ 	addiu	$t9,$v0,0x4
/*  f0fcf38:	0c002f29 */ 	jal	viGetViewLeft
/*  f0fcf3c:	afb900ec */ 	sw	$t9,0xec($sp)
/*  f0fcf40:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fcf44:	00105403 */ 	sra	$t2,$s0,0x10
/*  f0fcf48:	0c002f0b */ 	jal	viGetViewWidth
/*  f0fcf4c:	01408025 */ 	move	$s0,$t2
/*  f0fcf50:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0fcf54:	8d6b00f4 */ 	lw	$t3,0xf4($t3)
/*  f0fcf58:	00506021 */ 	addu	$t4,$v0,$s0
/*  f0fcf5c:	018b001a */ 	div	$zero,$t4,$t3
/*  f0fcf60:	00006812 */ 	mflo	$t5
/*  f0fcf64:	25aeffec */ 	addiu	$t6,$t5,-20
/*  f0fcf68:	15600002 */ 	bnez	$t3,.JF0f0fcf74
/*  f0fcf6c:	00000000 */ 	nop
/*  f0fcf70:	0007000d */ 	break	0x7
.JF0f0fcf74:
/*  f0fcf74:	2401ffff */ 	li	$at,-1
/*  f0fcf78:	15610004 */ 	bne	$t3,$at,.JF0f0fcf8c
/*  f0fcf7c:	3c018000 */ 	lui	$at,0x8000
/*  f0fcf80:	15810002 */ 	bne	$t4,$at,.JF0f0fcf8c
/*  f0fcf84:	00000000 */ 	nop
/*  f0fcf88:	0006000d */ 	break	0x6
.JF0f0fcf8c:
/*  f0fcf8c:	afae00e8 */ 	sw	$t6,0xe8($sp)
/*  f0fcf90:	0c002f2d */ 	jal	viGetViewTop
/*  f0fcf94:	00000000 */ 	nop
/*  f0fcf98:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fcf9c:	00107c03 */ 	sra	$t7,$s0,0x10
/*  f0fcfa0:	0c002f0f */ 	jal	viGetViewHeight
/*  f0fcfa4:	01e08025 */ 	move	$s0,$t7
/*  f0fcfa8:	0050c021 */ 	addu	$t8,$v0,$s0
/*  f0fcfac:	2719fffc */ 	addiu	$t9,$t8,-4
/*  f0fcfb0:	afb900e4 */ 	sw	$t9,0xe4($sp)
/*  f0fcfb4:	0fc54b55 */ 	jal	text0f153628
/*  f0fcfb8:	02a02025 */ 	move	$a0,$s5
/*  f0fcfbc:	3c13800a */ 	lui	$s3,0x800a
/*  f0fcfc0:	0040a825 */ 	move	$s5,$v0
/*  f0fcfc4:	2673a634 */ 	addiu	$s3,$s3,-22988
/*  f0fcfc8:	00009025 */ 	move	$s2,$zero
/*  f0fcfcc:	27b400b8 */ 	addiu	$s4,$sp,0xb8
/*  f0fcfd0:	8ec20490 */ 	lw	$v0,0x490($s6)
.JF0f0fcfd4:
/*  f0fcfd4:	24030002 */ 	li	$v1,0x2
/*  f0fcfd8:	02d25021 */ 	addu	$t2,$s6,$s2
/*  f0fcfdc:	54620013 */ 	bnel	$v1,$v0,.JF0f0fd02c
/*  f0fcfe0:	8eeb0004 */ 	lw	$t3,0x4($s7)
/*  f0fcfe4:	814c0494 */ 	lb	$t4,0x494($t2)
/*  f0fcfe8:	24100001 */ 	li	$s0,0x1
/*  f0fcfec:	5180000f */ 	beqzl	$t4,.JF0f0fd02c
/*  f0fcff0:	8eeb0004 */ 	lw	$t3,0x4($s7)
/*  f0fcff4:	0fc5baa5 */ 	jal	langGet
/*  f0fcff8:	240451e2 */ 	li	$a0,0x51e2
/*  f0fcffc:	00408825 */ 	move	$s1,$v0
/*  f0fd000:	0fc5baa5 */ 	jal	langGet
/*  f0fd004:	240458ca */ 	li	$a0,0x58ca
/*  f0fd008:	3c057f1b */ 	lui	$a1,0x7f1b
/*  f0fd00c:	24a534c0 */ 	addiu	$a1,$a1,0x34c0
/*  f0fd010:	02802025 */ 	move	$a0,$s4
/*  f0fd014:	02203025 */ 	move	$a2,$s1
/*  f0fd018:	0c004d95 */ 	jal	sprintf
/*  f0fd01c:	00403825 */ 	move	$a3,$v0
/*  f0fd020:	10000023 */ 	b	.JF0f0fd0b0
/*  f0fd024:	00000000 */ 	nop
/*  f0fd028:	8eeb0004 */ 	lw	$t3,0x4($s7)
.JF0f0fd02c:
/*  f0fd02c:	2401000b */ 	li	$at,0xb
/*  f0fd030:	24100001 */ 	li	$s0,0x1
/*  f0fd034:	15610013 */ 	bne	$t3,$at,.JF0f0fd084
/*  f0fd038:	00000000 */ 	nop
/*  f0fd03c:	1462000d */ 	bne	$v1,$v0,.JF0f0fd074
/*  f0fd040:	3c108007 */ 	lui	$s0,0x8007
/*  f0fd044:	3c02800a */ 	lui	$v0,0x800a
/*  f0fd048:	24100001 */ 	li	$s0,0x1
/*  f0fd04c:	2442a630 */ 	addiu	$v0,$v0,-22992
/*  f0fd050:	804d0494 */ 	lb	$t5,0x494($v0)
.JF0f0fd054:
/*  f0fd054:	24420001 */ 	addiu	$v0,$v0,0x1
/*  f0fd058:	11a00002 */ 	beqz	$t5,.JF0f0fd064
/*  f0fd05c:	00000000 */ 	nop
/*  f0fd060:	00008025 */ 	move	$s0,$zero
.JF0f0fd064:
/*  f0fd064:	5453fffb */ 	bnel	$v0,$s3,.JF0f0fd054
/*  f0fd068:	804d0494 */ 	lb	$t5,0x494($v0)
/*  f0fd06c:	10000005 */ 	b	.JF0f0fd084
/*  f0fd070:	00000000 */ 	nop
.JF0f0fd074:
/*  f0fd074:	8e101a28 */ 	lw	$s0,0x1a28($s0)
/*  f0fd078:	2e0e0002 */ 	sltiu	$t6,$s0,0x2
/*  f0fd07c:	10000001 */ 	b	.JF0f0fd084
/*  f0fd080:	01c08025 */ 	move	$s0,$t6
.JF0f0fd084:
/*  f0fd084:	0fc5baa5 */ 	jal	langGet
/*  f0fd088:	240451e2 */ 	li	$a0,0x51e2
/*  f0fd08c:	00408825 */ 	move	$s1,$v0
/*  f0fd090:	0fc5baa5 */ 	jal	langGet
/*  f0fd094:	240451e3 */ 	li	$a0,0x51e3
/*  f0fd098:	3c057f1b */ 	lui	$a1,0x7f1b
/*  f0fd09c:	24a534c8 */ 	addiu	$a1,$a1,0x34c8
/*  f0fd0a0:	02802025 */ 	move	$a0,$s4
/*  f0fd0a4:	02203025 */ 	move	$a2,$s1
/*  f0fd0a8:	0c004d95 */ 	jal	sprintf
/*  f0fd0ac:	00403825 */ 	move	$a3,$v0
.JF0f0fd0b0:
/*  f0fd0b0:	120000cd */ 	beqz	$s0,.JF0f0fd3e8
/*  f0fd0b4:	27a400e0 */ 	addiu	$a0,$sp,0xe0
/*  f0fd0b8:	3c0f8008 */ 	lui	$t7,0x8008
/*  f0fd0bc:	8def0148 */ 	lw	$t7,0x148($t7)
/*  f0fd0c0:	3c078008 */ 	lui	$a3,0x8008
/*  f0fd0c4:	8ce7014c */ 	lw	$a3,0x14c($a3)
/*  f0fd0c8:	27a500dc */ 	addiu	$a1,$sp,0xdc
/*  f0fd0cc:	02803025 */ 	move	$a2,$s4
/*  f0fd0d0:	afa00014 */ 	sw	$zero,0x14($sp)
/*  f0fd0d4:	02f28821 */ 	addu	$s1,$s7,$s2
/*  f0fd0d8:	0fc55d49 */ 	jal	textMeasure
/*  f0fd0dc:	afaf0010 */ 	sw	$t7,0x10($sp)
/*  f0fd0e0:	0c004ff9 */ 	jal	joyGetConnectedControllers
/*  f0fd0e4:	00000000 */ 	nop
/*  f0fd0e8:	3c19800b */ 	lui	$t9,0x800b
/*  f0fd0ec:	9739d5ce */ 	lhu	$t9,-0x2a32($t9)
/*  f0fd0f0:	0040c027 */ 	nor	$t8,$v0,$zero
/*  f0fd0f4:	240c0001 */ 	li	$t4,0x1
/*  f0fd0f8:	024c5804 */ 	sllv	$t3,$t4,$s2
/*  f0fd0fc:	03195025 */ 	or	$t2,$t8,$t9
/*  f0fd100:	014b6824 */ 	and	$t5,$t2,$t3
/*  f0fd104:	15a00016 */ 	bnez	$t5,.JF0f0fd160
/*  f0fd108:	3c014110 */ 	lui	$at,0x4110
/*  f0fd10c:	3c014040 */ 	lui	$at,0x4040
/*  f0fd110:	44818000 */ 	mtc1	$at,$f16
/*  f0fd114:	c6d20010 */ 	lwc1	$f18,0x10($s6)
/*  f0fd118:	92220017 */ 	lbu	$v0,0x17($s1)
/*  f0fd11c:	240f00ff */ 	li	$t7,0xff
/*  f0fd120:	46109182 */ 	mul.s	$f6,$f18,$f16
/*  f0fd124:	284100ff */ 	slti	$at,$v0,0xff
/*  f0fd128:	01e2c023 */ 	subu	$t8,$t7,$v0
/*  f0fd12c:	4600320d */ 	trunc.w.s	$f8,$f6
/*  f0fd130:	44034000 */ 	mfc1	$v1,$f8
/*  f0fd134:	10200019 */ 	beqz	$at,.JF0f0fd19c
/*  f0fd138:	0078082a */ 	slt	$at,$v1,$t8
/*  f0fd13c:	10200005 */ 	beqz	$at,.JF0f0fd154
/*  f0fd140:	240c00ff */ 	li	$t4,0xff
/*  f0fd144:	0043c821 */ 	addu	$t9,$v0,$v1
/*  f0fd148:	a2390017 */ 	sb	$t9,0x17($s1)
/*  f0fd14c:	10000013 */ 	b	.JF0f0fd19c
/*  f0fd150:	332200ff */ 	andi	$v0,$t9,0xff
.JF0f0fd154:
/*  f0fd154:	a22c0017 */ 	sb	$t4,0x17($s1)
/*  f0fd158:	10000010 */ 	b	.JF0f0fd19c
/*  f0fd15c:	318200ff */ 	andi	$v0,$t4,0xff
.JF0f0fd160:
/*  f0fd160:	c6ca0010 */ 	lwc1	$f10,0x10($s6)
/*  f0fd164:	44812000 */ 	mtc1	$at,$f4
/*  f0fd168:	92220017 */ 	lbu	$v0,0x17($s1)
/*  f0fd16c:	46045482 */ 	mul.s	$f18,$f10,$f4
/*  f0fd170:	4600940d */ 	trunc.w.s	$f16,$f18
/*  f0fd174:	44038000 */ 	mfc1	$v1,$f16
/*  f0fd178:	18400008 */ 	blez	$v0,.JF0f0fd19c
/*  f0fd17c:	0062082a */ 	slt	$at,$v1,$v0
/*  f0fd180:	10200004 */ 	beqz	$at,.JF0f0fd194
/*  f0fd184:	00435823 */ 	subu	$t3,$v0,$v1
/*  f0fd188:	a22b0017 */ 	sb	$t3,0x17($s1)
/*  f0fd18c:	10000003 */ 	b	.JF0f0fd19c
/*  f0fd190:	316200ff */ 	andi	$v0,$t3,0xff
.JF0f0fd194:
/*  f0fd194:	a2200017 */ 	sb	$zero,0x17($s1)
/*  f0fd198:	300200ff */ 	andi	$v0,$zero,0xff
.JF0f0fd19c:
/*  f0fd19c:	18400092 */ 	blez	$v0,.JF0f0fd3e8
/*  f0fd1a0:	3c0141a0 */ 	lui	$at,0x41a0
/*  f0fd1a4:	44816000 */ 	mtc1	$at,$f12
/*  f0fd1a8:	0fc01ae2 */ 	jal	func0f006b08
/*  f0fd1ac:	00000000 */ 	nop
/*  f0fd1b0:	3c01437f */ 	lui	$at,0x437f
/*  f0fd1b4:	44813000 */ 	mtc1	$at,$f6
/*  f0fd1b8:	240e0001 */ 	li	$t6,0x1
/*  f0fd1bc:	3c014f00 */ 	lui	$at,0x4f00
/*  f0fd1c0:	46060202 */ 	mul.s	$f8,$f0,$f6
/*  f0fd1c4:	240451e2 */ 	li	$a0,0x51e2
/*  f0fd1c8:	444df800 */ 	cfc1	$t5,$31
/*  f0fd1cc:	44cef800 */ 	ctc1	$t6,$31
/*  f0fd1d0:	00000000 */ 	nop
/*  f0fd1d4:	460042a4 */ 	cvt.w.s	$f10,$f8
/*  f0fd1d8:	444ef800 */ 	cfc1	$t6,$31
/*  f0fd1dc:	00000000 */ 	nop
/*  f0fd1e0:	31ce0078 */ 	andi	$t6,$t6,0x78
/*  f0fd1e4:	51c00013 */ 	beqzl	$t6,.JF0f0fd234
/*  f0fd1e8:	440e5000 */ 	mfc1	$t6,$f10
/*  f0fd1ec:	44815000 */ 	mtc1	$at,$f10
/*  f0fd1f0:	240e0001 */ 	li	$t6,0x1
/*  f0fd1f4:	460a4281 */ 	sub.s	$f10,$f8,$f10
/*  f0fd1f8:	44cef800 */ 	ctc1	$t6,$31
/*  f0fd1fc:	00000000 */ 	nop
/*  f0fd200:	460052a4 */ 	cvt.w.s	$f10,$f10
/*  f0fd204:	444ef800 */ 	cfc1	$t6,$31
/*  f0fd208:	00000000 */ 	nop
/*  f0fd20c:	31ce0078 */ 	andi	$t6,$t6,0x78
/*  f0fd210:	15c00005 */ 	bnez	$t6,.JF0f0fd228
/*  f0fd214:	00000000 */ 	nop
/*  f0fd218:	440e5000 */ 	mfc1	$t6,$f10
/*  f0fd21c:	3c018000 */ 	lui	$at,0x8000
/*  f0fd220:	10000007 */ 	b	.JF0f0fd240
/*  f0fd224:	01c17025 */ 	or	$t6,$t6,$at
.JF0f0fd228:
/*  f0fd228:	10000005 */ 	b	.JF0f0fd240
/*  f0fd22c:	240effff */ 	li	$t6,-1
/*  f0fd230:	440e5000 */ 	mfc1	$t6,$f10
.JF0f0fd234:
/*  f0fd234:	00000000 */ 	nop
/*  f0fd238:	05c0fffb */ 	bltz	$t6,.JF0f0fd228
/*  f0fd23c:	00000000 */ 	nop
.JF0f0fd240:
/*  f0fd240:	44cdf800 */ 	ctc1	$t5,$31
/*  f0fd244:	0fc5baa5 */ 	jal	langGet
/*  f0fd248:	afae00a0 */ 	sw	$t6,0xa0($sp)
/*  f0fd24c:	02802025 */ 	move	$a0,$s4
/*  f0fd250:	00402825 */ 	move	$a1,$v0
/*  f0fd254:	0c004d95 */ 	jal	sprintf
/*  f0fd258:	26460001 */ 	addiu	$a2,$s2,0x1
/*  f0fd25c:	2a410002 */ 	slti	$at,$s2,0x2
/*  f0fd260:	10200005 */ 	beqz	$at,.JF0f0fd278
/*  f0fd264:	8fb900e4 */ 	lw	$t9,0xe4($sp)
/*  f0fd268:	8faf00ec */ 	lw	$t7,0xec($sp)
/*  f0fd26c:	25f80002 */ 	addiu	$t8,$t7,0x2
/*  f0fd270:	10000003 */ 	b	.JF0f0fd280
/*  f0fd274:	afb800a8 */ 	sw	$t8,0xa8($sp)
.JF0f0fd278:
/*  f0fd278:	272cfff7 */ 	addiu	$t4,$t9,-9
/*  f0fd27c:	afac00a8 */ 	sw	$t4,0xa8($sp)
.JF0f0fd280:
/*  f0fd280:	24010001 */ 	li	$at,0x1
/*  f0fd284:	12410004 */ 	beq	$s2,$at,.JF0f0fd298
/*  f0fd288:	8faa00e8 */ 	lw	$t2,0xe8($sp)
/*  f0fd28c:	24010003 */ 	li	$at,0x3
/*  f0fd290:	16410006 */ 	bne	$s2,$at,.JF0f0fd2ac
/*  f0fd294:	8faf00f0 */ 	lw	$t7,0xf0($sp)
.JF0f0fd298:
/*  f0fd298:	8fab00dc */ 	lw	$t3,0xdc($sp)
/*  f0fd29c:	014b6823 */ 	subu	$t5,$t2,$t3
/*  f0fd2a0:	25aefffe */ 	addiu	$t6,$t5,-2
/*  f0fd2a4:	10000003 */ 	b	.JF0f0fd2b4
/*  f0fd2a8:	afae00ac */ 	sw	$t6,0xac($sp)
.JF0f0fd2ac:
/*  f0fd2ac:	25f80002 */ 	addiu	$t8,$t7,0x2
/*  f0fd2b0:	afb800ac */ 	sw	$t8,0xac($sp)
.JF0f0fd2b4:
/*  f0fd2b4:	0c002eeb */ 	jal	viGetWidth
/*  f0fd2b8:	00000000 */ 	nop
/*  f0fd2bc:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fd2c0:	0010cc03 */ 	sra	$t9,$s0,0x10
/*  f0fd2c4:	0c002eef */ 	jal	viGetHeight
/*  f0fd2c8:	03208025 */ 	move	$s0,$t9
/*  f0fd2cc:	922b0017 */ 	lbu	$t3,0x17($s1)
/*  f0fd2d0:	3c0c8008 */ 	lui	$t4,0x8008
/*  f0fd2d4:	3c0a8008 */ 	lui	$t2,0x8008
/*  f0fd2d8:	3c015070 */ 	lui	$at,0x5070
/*  f0fd2dc:	8d4a0148 */ 	lw	$t2,0x148($t2)
/*  f0fd2e0:	8d8c014c */ 	lw	$t4,0x14c($t4)
/*  f0fd2e4:	3421ff00 */ 	ori	$at,$at,0xff00
/*  f0fd2e8:	01616825 */ 	or	$t5,$t3,$at
/*  f0fd2ec:	afad0018 */ 	sw	$t5,0x18($sp)
/*  f0fd2f0:	02a02025 */ 	move	$a0,$s5
/*  f0fd2f4:	27a500ac */ 	addiu	$a1,$sp,0xac
/*  f0fd2f8:	27a600a8 */ 	addiu	$a2,$sp,0xa8
/*  f0fd2fc:	02803825 */ 	move	$a3,$s4
/*  f0fd300:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0fd304:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0fd308:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0fd30c:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0fd310:	afaa0014 */ 	sw	$t2,0x14($sp)
/*  f0fd314:	0fc55886 */ 	jal	textRenderProjected
/*  f0fd318:	afac0010 */ 	sw	$t4,0x10($sp)
/*  f0fd31c:	8ece0490 */ 	lw	$t6,0x490($s6)
/*  f0fd320:	24010002 */ 	li	$at,0x2
/*  f0fd324:	0040a825 */ 	move	$s5,$v0
/*  f0fd328:	15c1000b */ 	bne	$t6,$at,.JF0f0fd358
/*  f0fd32c:	02d27821 */ 	addu	$t7,$s6,$s2
/*  f0fd330:	81f80494 */ 	lb	$t8,0x494($t7)
/*  f0fd334:	13000008 */ 	beqz	$t8,.JF0f0fd358
/*  f0fd338:	00000000 */ 	nop
/*  f0fd33c:	0fc5baa5 */ 	jal	langGet
/*  f0fd340:	240458ca */ 	li	$a0,0x58ca
/*  f0fd344:	02802025 */ 	move	$a0,$s4
/*  f0fd348:	0c004c34 */ 	jal	strcpy
/*  f0fd34c:	00402825 */ 	move	$a1,$v0
/*  f0fd350:	1000000d */ 	b	.JF0f0fd388
/*  f0fd354:	2403ffff */ 	li	$v1,-1
.JF0f0fd358:
/*  f0fd358:	0fc5baa5 */ 	jal	langGet
/*  f0fd35c:	240451e3 */ 	li	$a0,0x51e3
/*  f0fd360:	02802025 */ 	move	$a0,$s4
/*  f0fd364:	0c004c34 */ 	jal	strcpy
/*  f0fd368:	00402825 */ 	move	$a1,$v0
/*  f0fd36c:	3c0400ff */ 	lui	$a0,0xff
/*  f0fd370:	3484ff00 */ 	ori	$a0,$a0,0xff00
/*  f0fd374:	2405ff00 */ 	li	$a1,-256
/*  f0fd378:	0fc01a60 */ 	jal	colourBlend
/*  f0fd37c:	8fa600a0 */ 	lw	$a2,0xa0($sp)
/*  f0fd380:	92390017 */ 	lbu	$t9,0x17($s1)
/*  f0fd384:	03221825 */ 	or	$v1,$t9,$v0
.JF0f0fd388:
/*  f0fd388:	0c002eeb */ 	jal	viGetWidth
/*  f0fd38c:	afa300a4 */ 	sw	$v1,0xa4($sp)
/*  f0fd390:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fd394:	00106403 */ 	sra	$t4,$s0,0x10
/*  f0fd398:	0c002eef */ 	jal	viGetHeight
/*  f0fd39c:	01808025 */ 	move	$s0,$t4
/*  f0fd3a0:	3c0a8008 */ 	lui	$t2,0x8008
/*  f0fd3a4:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0fd3a8:	8d6b0148 */ 	lw	$t3,0x148($t3)
/*  f0fd3ac:	8d4a014c */ 	lw	$t2,0x14c($t2)
/*  f0fd3b0:	8fa300a4 */ 	lw	$v1,0xa4($sp)
/*  f0fd3b4:	02a02025 */ 	move	$a0,$s5
/*  f0fd3b8:	27a500ac */ 	addiu	$a1,$sp,0xac
/*  f0fd3bc:	27a600a8 */ 	addiu	$a2,$sp,0xa8
/*  f0fd3c0:	02803825 */ 	move	$a3,$s4
/*  f0fd3c4:	afb0001c */ 	sw	$s0,0x1c($sp)
/*  f0fd3c8:	afa20020 */ 	sw	$v0,0x20($sp)
/*  f0fd3cc:	afa00024 */ 	sw	$zero,0x24($sp)
/*  f0fd3d0:	afa00028 */ 	sw	$zero,0x28($sp)
/*  f0fd3d4:	afab0014 */ 	sw	$t3,0x14($sp)
/*  f0fd3d8:	afaa0010 */ 	sw	$t2,0x10($sp)
/*  f0fd3dc:	0fc55886 */ 	jal	textRenderProjected
/*  f0fd3e0:	afa30018 */ 	sw	$v1,0x18($sp)
/*  f0fd3e4:	0040a825 */ 	move	$s5,$v0
.JF0f0fd3e8:
/*  f0fd3e8:	26520001 */ 	addiu	$s2,$s2,0x1
/*  f0fd3ec:	24010004 */ 	li	$at,0x4
/*  f0fd3f0:	5641fef8 */ 	bnel	$s2,$at,.JF0f0fcfd4
/*  f0fd3f4:	8ec20490 */ 	lw	$v0,0x490($s6)
/*  f0fd3f8:	0fc54bb7 */ 	jal	text0f153780
/*  f0fd3fc:	02a02025 */ 	move	$a0,$s5
/*  f0fd400:	0040a825 */ 	move	$s5,$v0
/*  f0fd404:	02a01025 */ 	move	$v0,$s5
.JF0f0fd408:
/*  f0fd408:	3c0db700 */ 	lui	$t5,0xb700
/*  f0fd40c:	240e0001 */ 	li	$t6,0x1
/*  f0fd410:	ac4e0004 */ 	sw	$t6,0x4($v0)
/*  f0fd414:	ac4d0000 */ 	sw	$t5,0x0($v0)
/*  f0fd418:	26b50008 */ 	addiu	$s5,$s5,0x8
/*  f0fd41c:	82ef001b */ 	lb	$t7,0x1b($s7)
.JF0f0fd420:
/*  f0fd420:	2401ffff */ 	li	$at,-1
/*  f0fd424:	11e100a1 */ 	beq	$t7,$at,.JF0f0fd6ac
/*  f0fd428:	00000000 */ 	nop
/*  f0fd42c:	0c002f29 */ 	jal	viGetViewLeft
/*  f0fd430:	00000000 */ 	nop
/*  f0fd434:	3c188008 */ 	lui	$t8,0x8008
/*  f0fd438:	8f1800f4 */ 	lw	$t8,0xf4($t8)
/*  f0fd43c:	0058001a */ 	div	$zero,$v0,$t8
/*  f0fd440:	0000c812 */ 	mflo	$t9
/*  f0fd444:	afb90098 */ 	sw	$t9,0x98($sp)
/*  f0fd448:	17000002 */ 	bnez	$t8,.JF0f0fd454
/*  f0fd44c:	00000000 */ 	nop
/*  f0fd450:	0007000d */ 	break	0x7
.JF0f0fd454:
/*  f0fd454:	2401ffff */ 	li	$at,-1
/*  f0fd458:	17010004 */ 	bne	$t8,$at,.JF0f0fd46c
/*  f0fd45c:	3c018000 */ 	lui	$at,0x8000
/*  f0fd460:	14410002 */ 	bne	$v0,$at,.JF0f0fd46c
/*  f0fd464:	00000000 */ 	nop
/*  f0fd468:	0006000d */ 	break	0x6
.JF0f0fd46c:
/*  f0fd46c:	0c002f2d */ 	jal	viGetViewTop
/*  f0fd470:	00000000 */ 	nop
/*  f0fd474:	0c002f29 */ 	jal	viGetViewLeft
/*  f0fd478:	afa20094 */ 	sw	$v0,0x94($sp)
/*  f0fd47c:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fd480:	00106403 */ 	sra	$t4,$s0,0x10
/*  f0fd484:	0c002f0b */ 	jal	viGetViewWidth
/*  f0fd488:	01808025 */ 	move	$s0,$t4
/*  f0fd48c:	3c0b8008 */ 	lui	$t3,0x8008
/*  f0fd490:	8d6b00f4 */ 	lw	$t3,0xf4($t3)
/*  f0fd494:	00505021 */ 	addu	$t2,$v0,$s0
/*  f0fd498:	014b001a */ 	div	$zero,$t2,$t3
/*  f0fd49c:	0000a012 */ 	mflo	$s4
/*  f0fd4a0:	15600002 */ 	bnez	$t3,.JF0f0fd4ac
/*  f0fd4a4:	00000000 */ 	nop
/*  f0fd4a8:	0007000d */ 	break	0x7
.JF0f0fd4ac:
/*  f0fd4ac:	2401ffff */ 	li	$at,-1
/*  f0fd4b0:	15610004 */ 	bne	$t3,$at,.JF0f0fd4c4
/*  f0fd4b4:	3c018000 */ 	lui	$at,0x8000
/*  f0fd4b8:	15410002 */ 	bne	$t2,$at,.JF0f0fd4c4
/*  f0fd4bc:	00000000 */ 	nop
/*  f0fd4c0:	0006000d */ 	break	0x6
.JF0f0fd4c4:
/*  f0fd4c4:	0c002f2d */ 	jal	viGetViewTop
/*  f0fd4c8:	00000000 */ 	nop
/*  f0fd4cc:	00028400 */ 	sll	$s0,$v0,0x10
/*  f0fd4d0:	00106c03 */ 	sra	$t5,$s0,0x10
/*  f0fd4d4:	0c002f0f */ 	jal	viGetViewHeight
/*  f0fd4d8:	01a08025 */ 	move	$s0,$t5
/*  f0fd4dc:	8ec7006c */ 	lw	$a3,0x6c($s6)
/*  f0fd4e0:	00504821 */ 	addu	$t1,$v0,$s0
/*  f0fd4e4:	00009025 */ 	move	$s2,$zero
/*  f0fd4e8:	10e00003 */ 	beqz	$a3,.JF0f0fd4f8
/*  f0fd4ec:	00009825 */ 	move	$s3,$zero
/*  f0fd4f0:	10000002 */ 	b	.JF0f0fd4fc
/*  f0fd4f4:	24080001 */ 	li	$t0,0x1
.JF0f0fd4f8:
/*  f0fd4f8:	00004025 */ 	move	$t0,$zero
.JF0f0fd4fc:
/*  f0fd4fc:	8ec40068 */ 	lw	$a0,0x68($s6)
/*  f0fd500:	50800004 */ 	beqzl	$a0,.JF0f0fd514
/*  f0fd504:	00001825 */ 	move	$v1,$zero
/*  f0fd508:	10000002 */ 	b	.JF0f0fd514
/*  f0fd50c:	24030001 */ 	li	$v1,0x1
/*  f0fd510:	00001825 */ 	move	$v1,$zero
.JF0f0fd514:
/*  f0fd514:	8ec50064 */ 	lw	$a1,0x64($s6)
/*  f0fd518:	50a00004 */ 	beqzl	$a1,.JF0f0fd52c
/*  f0fd51c:	00008825 */ 	move	$s1,$zero
/*  f0fd520:	10000002 */ 	b	.JF0f0fd52c
/*  f0fd524:	24110001 */ 	li	$s1,0x1
/*  f0fd528:	00008825 */ 	move	$s1,$zero
.JF0f0fd52c:
/*  f0fd52c:	8ec60070 */ 	lw	$a2,0x70($s6)
/*  f0fd530:	50c00004 */ 	beqzl	$a2,.JF0f0fd544
/*  f0fd534:	00001025 */ 	move	$v0,$zero
/*  f0fd538:	10000002 */ 	b	.JF0f0fd544
/*  f0fd53c:	24020001 */ 	li	$v0,0x1
/*  f0fd540:	00001025 */ 	move	$v0,$zero
.JF0f0fd544:
/*  f0fd544:	00517021 */ 	addu	$t6,$v0,$s1
/*  f0fd548:	01c37821 */ 	addu	$t7,$t6,$v1
/*  f0fd54c:	01e8c021 */ 	addu	$t8,$t7,$t0
/*  f0fd550:	2b010003 */ 	slti	$at,$t8,0x3
/*  f0fd554:	1420000a */ 	bnez	$at,.JF0f0fd580
/*  f0fd558:	00000000 */ 	nop
/*  f0fd55c:	8ec2028c */ 	lw	$v0,0x28c($s6)
/*  f0fd560:	24010001 */ 	li	$at,0x1
/*  f0fd564:	10410003 */ 	beq	$v0,$at,.JF0f0fd574
/*  f0fd568:	24010003 */ 	li	$at,0x3
/*  f0fd56c:	54410004 */ 	bnel	$v0,$at,.JF0f0fd580
/*  f0fd570:	2412000f */ 	li	$s2,0xf
.JF0f0fd574:
/*  f0fd574:	10000002 */ 	b	.JF0f0fd580
/*  f0fd578:	2413000f */ 	li	$s3,0xf
/*  f0fd57c:	2412000f */ 	li	$s2,0xf
.JF0f0fd580:
/*  f0fd580:	10e00003 */ 	beqz	$a3,.JF0f0fd590
/*  f0fd584:	24010002 */ 	li	$at,0x2
/*  f0fd588:	10000002 */ 	b	.JF0f0fd594
/*  f0fd58c:	24080001 */ 	li	$t0,0x1
.JF0f0fd590:
/*  f0fd590:	00004025 */ 	move	$t0,$zero
.JF0f0fd594:
/*  f0fd594:	10800003 */ 	beqz	$a0,.JF0f0fd5a4
/*  f0fd598:	00001825 */ 	move	$v1,$zero
/*  f0fd59c:	10000001 */ 	b	.JF0f0fd5a4
/*  f0fd5a0:	24030001 */ 	li	$v1,0x1
.JF0f0fd5a4:
/*  f0fd5a4:	10a00003 */ 	beqz	$a1,.JF0f0fd5b4
/*  f0fd5a8:	00008825 */ 	move	$s1,$zero
/*  f0fd5ac:	10000001 */ 	b	.JF0f0fd5b4
/*  f0fd5b0:	24110001 */ 	li	$s1,0x1
.JF0f0fd5b4:
/*  f0fd5b4:	10c00003 */ 	beqz	$a2,.JF0f0fd5c4
/*  f0fd5b8:	00001025 */ 	move	$v0,$zero
/*  f0fd5bc:	10000001 */ 	b	.JF0f0fd5c4
/*  f0fd5c0:	24020001 */ 	li	$v0,0x1
.JF0f0fd5c4:
/*  f0fd5c4:	0051c821 */ 	addu	$t9,$v0,$s1
/*  f0fd5c8:	03236021 */ 	addu	$t4,$t9,$v1
/*  f0fd5cc:	01885021 */ 	addu	$t2,$t4,$t0
/*  f0fd5d0:	55410013 */ 	bnel	$t2,$at,.JF0f0fd620
/*  f0fd5d4:	8ece006c */ 	lw	$t6,0x6c($s6)
/*  f0fd5d8:	0fc54978 */ 	jal	optionsGetScreenSplit
/*  f0fd5dc:	afa9008c */ 	sw	$t1,0x8c($sp)
/*  f0fd5e0:	24010001 */ 	li	$at,0x1
/*  f0fd5e4:	10410006 */ 	beq	$v0,$at,.JF0f0fd600
/*  f0fd5e8:	8fa9008c */ 	lw	$t1,0x8c($sp)
/*  f0fd5ec:	3c0b8009 */ 	lui	$t3,0x8009
/*  f0fd5f0:	916b1160 */ 	lbu	$t3,0x1160($t3)
/*  f0fd5f4:	24010001 */ 	li	$at,0x1
/*  f0fd5f8:	55610009 */ 	bnel	$t3,$at,.JF0f0fd620
/*  f0fd5fc:	8ece006c */ 	lw	$t6,0x6c($s6)
.JF0f0fd600:
/*  f0fd600:	8ecd028c */ 	lw	$t5,0x28c($s6)
/*  f0fd604:	24010001 */ 	li	$at,0x1
/*  f0fd608:	55a10004 */ 	bnel	$t5,$at,.JF0f0fd61c
/*  f0fd60c:	2412000f */ 	li	$s2,0xf
/*  f0fd610:	10000002 */ 	b	.JF0f0fd61c
/*  f0fd614:	2413000f */ 	li	$s3,0xf
/*  f0fd618:	2412000f */ 	li	$s2,0xf
.JF0f0fd61c:
/*  f0fd61c:	8ece006c */ 	lw	$t6,0x6c($s6)
.JF0f0fd620:
/*  f0fd620:	8ec40068 */ 	lw	$a0,0x68($s6)
/*  f0fd624:	8ec50064 */ 	lw	$a1,0x64($s6)
/*  f0fd628:	11c00003 */ 	beqz	$t6,.JF0f0fd638
/*  f0fd62c:	8ec60070 */ 	lw	$a2,0x70($s6)
/*  f0fd630:	10000002 */ 	b	.JF0f0fd63c
/*  f0fd634:	24080001 */ 	li	$t0,0x1
.JF0f0fd638:
/*  f0fd638:	00004025 */ 	move	$t0,$zero
.JF0f0fd63c:
/*  f0fd63c:	10800003 */ 	beqz	$a0,.JF0f0fd64c
/*  f0fd640:	02803825 */ 	move	$a3,$s4
/*  f0fd644:	10000002 */ 	b	.JF0f0fd650
/*  f0fd648:	24030001 */ 	li	$v1,0x1
.JF0f0fd64c:
/*  f0fd64c:	00001825 */ 	move	$v1,$zero
.JF0f0fd650:
/*  f0fd650:	10a00003 */ 	beqz	$a1,.JF0f0fd660
/*  f0fd654:	02a02025 */ 	move	$a0,$s5
/*  f0fd658:	10000002 */ 	b	.JF0f0fd664
/*  f0fd65c:	24110001 */ 	li	$s1,0x1
.JF0f0fd660:
/*  f0fd660:	00008825 */ 	move	$s1,$zero
.JF0f0fd664:
/*  f0fd664:	10c00003 */ 	beqz	$a2,.JF0f0fd674
/*  f0fd668:	8fa50098 */ 	lw	$a1,0x98($sp)
/*  f0fd66c:	10000002 */ 	b	.JF0f0fd678
/*  f0fd670:	24020001 */ 	li	$v0,0x1
.JF0f0fd674:
/*  f0fd674:	00001025 */ 	move	$v0,$zero
.JF0f0fd678:
/*  f0fd678:	00517821 */ 	addu	$t7,$v0,$s1
/*  f0fd67c:	01e3c021 */ 	addu	$t8,$t7,$v1
/*  f0fd680:	82ea001b */ 	lb	$t2,0x1b($s7)
/*  f0fd684:	0308c821 */ 	addu	$t9,$t8,$t0
/*  f0fd688:	2b2c0002 */ 	slti	$t4,$t9,0x2
/*  f0fd68c:	afac0014 */ 	sw	$t4,0x14($sp)
/*  f0fd690:	8fa60094 */ 	lw	$a2,0x94($sp)
/*  f0fd694:	afa90010 */ 	sw	$t1,0x10($sp)
/*  f0fd698:	afb2001c */ 	sw	$s2,0x1c($sp)
/*  f0fd69c:	afb30020 */ 	sw	$s3,0x20($sp)
/*  f0fd6a0:	0fc3c68e */ 	jal	menuRenderBanner
/*  f0fd6a4:	afaa0018 */ 	sw	$t2,0x18($sp)
/*  f0fd6a8:	0040a825 */ 	move	$s5,$v0
.JF0f0fd6ac:
/*  f0fd6ac:	0fc356a2 */ 	jal	func0f0d49c8
/*  f0fd6b0:	02a02025 */ 	move	$a0,$s5
/*  f0fd6b4:	8fbf0054 */ 	lw	$ra,0x54($sp)
/*  f0fd6b8:	240b0001 */ 	li	$t3,0x1
/*  f0fd6bc:	3c018008 */ 	lui	$at,0x8008
/*  f0fd6c0:	8fb00034 */ 	lw	$s0,0x34($sp)
/*  f0fd6c4:	8fb10038 */ 	lw	$s1,0x38($sp)
/*  f0fd6c8:	8fb2003c */ 	lw	$s2,0x3c($sp)
/*  f0fd6cc:	8fb30040 */ 	lw	$s3,0x40($sp)
/*  f0fd6d0:	8fb40044 */ 	lw	$s4,0x44($sp)
/*  f0fd6d4:	8fb50048 */ 	lw	$s5,0x48($sp)
/*  f0fd6d8:	8fb6004c */ 	lw	$s6,0x4c($sp)
/*  f0fd6dc:	8fb70050 */ 	lw	$s7,0x50($sp)
/*  f0fd6e0:	ac2b00f4 */ 	sw	$t3,0xf4($at)
/*  f0fd6e4:	03e00008 */ 	jr	$ra
/*  f0fd6e8:	27bd0118 */ 	addiu	$sp,$sp,0x118
);
#else
Gfx *menuRender(Gfx *gdl)
{
	static u32 usepiece = 1;

	g_MpPlayerNum = 0;

#if PAL
	g_ScaleX = 1;
#else
	g_ScaleX = g_ViRes == VIRES_HI ? 2 : 1;
#endif

	gdl = func0f0d479c(gdl);

	gSPDisplayList(gdl++, var800613a0);

	// Render the background
	if (g_MenuData.nextbg != 255) {
		if (g_MenuData.nextbg == 0) {
			gdl = menuRenderBackgroundLayer1(gdl, g_MenuData.bg, 1.0f - g_MenuData.unk010);
		} else {
			gdl = menuRenderBackgroundLayer1(gdl, g_MenuData.bg, 1.0f);
			gdl = menuRenderBackgroundLayer1(gdl, g_MenuData.nextbg, g_MenuData.unk010);
		}
	} else {
		gdl = menuRenderBackgroundLayer1(gdl, g_MenuData.bg, 1.0f);
	}

	// Calculate hudpiece things then render it
	if (g_MenuData.unk5d5_05) {
		g_MenuData.unk01c.unk05e = 0;
		g_MenuData.unk01c.unk05c = 0x40d;
		g_MenuData.unk01c.unk580 = 0;
		g_MenuData.unk01c.unk5b1_04 = false;
		g_MenuData.unk5d4 = 1;
		g_MenuData.unk5d5_05 = false;
	}

	if (IS8MB() && g_MenuData.unk5d4) {
		bool removepiece = false;

		gSPSetGeometryMode(gdl++, G_ZBUFFER);

		// Everyone 1 in 100 frames on average, calculate a new X/Y for the hudpiece
		// Note: unintentional 64-bit float comparison done here
		if (RANDOMFRAC() < 0.01) {
			g_MenuData.unk01c.unk538 = RANDOMFRAC() * 80.0f + -205.5f - 40.0f;
			g_MenuData.unk01c.unk53c = RANDOMFRAC() * 80.0f + 244.7f - 40.0f;
		}

		var8009de98 = var8009de9c = 0;

		if (g_MenuData.root == MENUROOT_MPSETUP) {
			if (g_MenuData.count <= 0) {
				removepiece = true;
			}
		}

		if (g_MenuData.root != MENUROOT_MAINMENU
				&& g_MenuData.root != MENUROOT_MPSETUP
				&& g_MenuData.root != MENUROOT_FILEMGR
				&& g_MenuData.root != MENUROOT_TRAINING) {
			removepiece = true;
		}

		if (g_Menus[g_MpPlayerNum].curdialog == NULL) {
			if (g_MenuData.root != MENUROOT_MPSETUP) {
				removepiece = true;
			}
		}

		if (g_Vars.currentplayer->eyespy
				&& (g_Vars.currentplayer->devicesactive & ~g_Vars.currentplayer->devicesinhibit & DEVICE_EYESPY)
				&& g_Vars.currentplayer->eyespy->active) {
			removepiece = true;
		}

		if (removepiece) {
			if (g_MenuData.unk01c.unk580 == 0) {
				g_MenuData.unk01c.unk5b1_04 = true;
				g_MenuData.unk01c.unk05e = 0;
				g_MenuData.unk01c.unk05c = 0x40d;
				g_MenuData.unk01c.unk580 = 1;
			} else if (g_MenuData.unk01c.unk05e == 0) {
				g_MenuData.unk01c.unk580 = 0;
				g_MenuData.unk5d4 = 0;
			}
		}

		mainOverrideVariable("usePiece", &usepiece);

		if (usepiece) {
			g_MenuData.unk5d5_03 = false;

			gdl = menuRenderModels(gdl, &g_MenuData.unk01c, 1);
			gSPClearGeometryMode(gdl++, G_ZBUFFER);

			g_MenuData.unk5d5_03 = true;
		}
	} else {
		var8009de98 = var8009de9c = 0;
	}

	if (g_MenuData.unk5d5_04) {
		var8009de98 = g_MenuData.unk670;
		var8009de9c = g_MenuData.unk674;
	}

	// Render the second layer of the background (for the combat simulator cone,
	// which draws over the top of the hud piece)
	if (g_MenuData.nextbg != 255) {
		if (g_MenuData.nextbg == 0) {
			gdl = menuRenderBackgroundLayer2(gdl, g_MenuData.bg, 1.0f - g_MenuData.unk010);
		} else {
			gdl = menuRenderBackgroundLayer2(gdl, g_MenuData.bg, 1.0f);
			gdl = menuRenderBackgroundLayer2(gdl, g_MenuData.nextbg, g_MenuData.unk010);
		}
	} else {
		gdl = menuRenderBackgroundLayer2(gdl, g_MenuData.bg, 1.0f);
	}

	// Render the health bar (playerRenderHealthBar may choose not to render)
	if ((g_MenuData.bg || g_MenuData.nextbg != 255)
			&& (!g_Vars.currentplayer->eyespy || !g_Vars.currentplayer->eyespy->active)) {
		gdl = func0f0d49c8(gdl);
		gdl = playerRenderHealthBar(gdl);
		gdl = func0f0d479c(gdl);
	}

	if (g_MenuData.count > 0) {
		// Render dialogs
		gdl = text0f153ab0(gdl);

		if (g_MenuData.root == MENUROOT_MPPAUSE || g_MenuData.root == MENUROOT_MPENDSCREEN) {
			g_MpPlayerNum = g_Vars.currentplayerstats->mpindex;
			gdl = menuRenderDialogs(gdl);
		} else {
			s32 i;

			for (i = 0; i < 4; i++) {
				g_MpPlayerNum = i;
				gdl = menuRenderDialogs(gdl);
			}
		}

		g_MpPlayerNum = 0;

		gSPMatrix(gdl++, osVirtualToPhysical(camGetPerspectiveMtxL()), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
		gSPDisplayList(gdl++, var800613a0);

		text0f153b40();

		// Render corner texts in combat simulator
		if (g_MenuData.root == MENUROOT_MPSETUP || g_MenuData.root == MENUROOT_4MBMAINMENU) {
			s32 i;
			s32 j;
			s32 viewleft = viGetViewLeft() / g_ScaleX + 20;
			s32 viewtop = viGetViewTop() + 4;
			s32 viewright = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX - 20;
			s32 viewbottom = viGetViewTop() + viGetViewHeight() - 4;
			s32 textheight;
			s32 textwidth;
			bool renderit;
			char text[32];
			s32 tmp1;
			s32 tmp2;
			s32 x;
			s32 y;
			s32 colour;

			gdl = text0f153628(gdl);

			for (i = 0; i < 4; i++) {
				// Figure out what text will be displayed. The text calculated
				// here is for measuring purposes only and isn't rendered.
				// Amusingly, there's a %d placeholder in the text which isn't
				// replaced prior to measuring, so the width is slightly wrong.
				if (g_Vars.mpsetupmenu == MPSETUPMENU_GENERAL && g_Vars.waitingtojoin[i]) {
					// Player has pressed start but they can't open the player-specific
					// dialog yet because they're still on the Combat Simulator dialog
					// or similar. Show "Ready" in their corner.
					renderit = true;
					// "Player %d: " and "Ready!"
					sprintf(text, "%s%s", langGet(L_MPMENU_482), langGet(L_MISC_461));
				} else {
					if (g_MenuData.root == MENUROOT_4MBMAINMENU) {
						if (g_Vars.mpsetupmenu == MPSETUPMENU_GENERAL) {
							renderit = true;

							for (j = 0; j < 4; j++) {
								if (g_Vars.waitingtojoin[j]) {
									renderit = false;
								}
							}
						} else {
							renderit = g_MpNumJoined < 2;
						}
					} else {
						renderit = true;
					}

					// "Player %d: " and "Press START!"
					sprintf(text, "%s%s", langGet(L_MPMENU_482), langGet(L_MPMENU_483));
				}

				if (renderit) {
					textMeasure(&textheight, &textwidth, text, g_CharsHandelGothicSm, g_FontHandelGothicSm, 0);

					// Check which controllers are connected
					// and update the alpha of the label
					if (((g_MpSetup.chrslots | ~joyGetConnectedControllers()) & (1 << i)) == 0) {
#if VERSION >= VERSION_PAL_BETA
						tmp1 = g_Vars.diffframe60freal * 3;
#else
						tmp1 = g_Vars.diffframe60 * 3;
#endif

						if (g_MenuData.playerjoinalpha[i] < 255) {
							if (255 - g_MenuData.playerjoinalpha[i] > tmp1) {
								g_MenuData.playerjoinalpha[i] += tmp1;
							} else {
								g_MenuData.playerjoinalpha[i] = 255;
							}
						}
					} else {
#if VERSION >= VERSION_PAL_BETA
						tmp2 = g_Vars.diffframe60freal * 9;
#else
						tmp2 = g_Vars.diffframe60 * 9;
#endif

						if (g_MenuData.playerjoinalpha[i] > 0) {
							if (g_MenuData.playerjoinalpha[i] > tmp2) {
								g_MenuData.playerjoinalpha[i] -= tmp2;
							} else {
								g_MenuData.playerjoinalpha[i] = 0;
							}
						}
					}

					if (g_MenuData.playerjoinalpha[i] > 0) {
						u32 weight = func0f006b08(20) * 255.0f;

						// "Player %d: "
						sprintf(text, langGet(L_MPMENU_482), i + 1);

						if (i < 2) {
							y = viewtop + 2;
						} else {
							y = viewbottom - 9;
						}

						if (i == 1 || i == 3) {
							x = viewright - textwidth - 2;
						} else {
							x = viewleft + 2;
						}

						gdl = textRenderProjected(gdl, &x, &y, text, g_CharsHandelGothicSm, g_FontHandelGothicSm, g_MenuData.playerjoinalpha[i] | 0x5070ff00, viGetWidth(), viGetHeight(), 0, 0);

						if (g_Vars.mpsetupmenu == MPSETUPMENU_GENERAL && g_Vars.waitingtojoin[i]) {
							// "Ready!"
							strcpy(text, langGet(L_MISC_461));
							colour = g_MenuData.playerjoinalpha[i] | 0xd00020ff;
						} else {
							// "Press START!"
							strcpy(text, langGet(L_MPMENU_483));
							colour = colourBlend(0x00ffff00, 0xffffff00, weight) | g_MenuData.playerjoinalpha[i];
						}

						gdl = textRenderProjected(gdl, &x, &y, text, g_CharsHandelGothicSm, g_FontHandelGothicSm, colour, viGetWidth(), viGetHeight(), 0, 0);
					}
				}
			}

			gdl = text0f153780(gdl);
		}

		gSPSetGeometryMode(gdl++, G_ZBUFFER);
	}

	// Render banner messages, such as "Please Wait...",
	// "Checking Controller Pak" and some unused game boy camera texts.
	if (g_MenuData.bannernum != -1) {
		s32 x1 = viGetViewLeft() / g_ScaleX;
		s32 y1 = viGetViewTop();
		s32 x2 = (viGetViewLeft() + viGetViewWidth()) / g_ScaleX;
		s32 y2 = viGetViewTop() + viGetViewHeight();

#if VERSION >= VERSION_NTSC_1_0
		s32 left = 0;
		s32 right = 0;

		if (PLAYERCOUNT() >= 3) {
			if (g_Vars.currentplayernum == 1 || g_Vars.currentplayernum == 3) {
				right = 15;
			} else {
				left = 15;
			}
		}

		if (PLAYERCOUNT() == 2 && (optionsGetScreenSplit() == SCREENSPLIT_VERTICAL || IS4MB())) {
			if (g_Vars.currentplayernum == 1) {
				right = 15;
			} else {
				left = 15;
			}
		}

		gdl = menuRenderBanner(gdl, x1, y1, x2, y2, PLAYERCOUNT() < 2, g_MenuData.bannernum, left, right);
#else
		if (PLAYERCOUNT() >= 3) {
			if (g_Vars.currentplayernum == 1 || g_Vars.currentplayernum == 3) {
				x2 -= 10;
			} else {
				x1 += 10;
			}
		}

		if (PLAYERCOUNT() == 2 && (optionsGetScreenSplit() == SCREENSPLIT_VERTICAL || IS4MB())) {
			if (g_Vars.currentplayernum == 1) {
				x2 -= 10;
			} else {
				x1 += 10;
			}
		}

		gdl = menuRenderBanner(gdl, x1, y1, x2, y2, PLAYERCOUNT() < 2, g_MenuData.bannernum);
#endif
	}

	gdl = func0f0d49c8(gdl);

	g_ScaleX = 1;

	return gdl;
}
#endif

const char var7f1b27a4[] = "Tune Selector - mode %d\n";

u32 menuChooseMusic(void)
{
	s32 missionsuccess = MUSIC_MISSION_SUCCESS;

	if (g_StageIndex == STAGEINDEX_DEFENSE) {
		missionsuccess = MUSIC_MISSION_UNKNOWN;
	}

	if (g_MenuData.root == MENUROOT_ENDSCREEN) {
		if (g_Vars.bond->isdead || g_Vars.bond->aborted || !objectiveIsAllComplete()) {
			return MUSIC_MISSION_FAILED;
		}

		return missionsuccess;
	}

	if (g_MenuData.root == MENUROOT_MPENDSCREEN) {
		if (g_Vars.coopplayernum >= 0) {
			if ((g_Vars.bond->isdead && g_Vars.coop->isdead)
					|| g_Vars.bond->aborted
					|| g_Vars.coop->aborted
					|| !objectiveIsAllComplete()) {
				return MUSIC_MISSION_FAILED;
			}

			return missionsuccess;
		}

		if (g_Vars.antiplayernum >= 0) {
			if (g_Vars.bond->isdead || g_Vars.bond->aborted || !objectiveIsAllComplete()) {
				return MUSIC_MISSION_FAILED;
			}

			return missionsuccess;
		}
	}

	if (g_MenuData.root == MENUROOT_FILEMGR) {
		return MUSIC_MAINMENU;
	}

	if (g_MenuData.root == MENUROOT_MPSETUP || g_MenuData.root == MENUROOT_4MBMAINMENU) {
		return MUSIC_COMBATSIM_MENU;
	}

	if (g_MenuData.root == MENUROOT_MPPAUSE) {
		return MUSIC_COMBATSIM_COMPLETE;
	}

	if (g_MenuData.root == MENUROOT_BOOTPAKMGR) {
		return MUSIC_MISSION_UNKNOWN;
	}

	if (g_Vars.mplayerisrunning) {
		return MUSIC_COMBATSIM_COMPLETE;
	}

	if (g_Vars.stagenum == STAGE_CITRAINING || g_Vars.stagenum == STAGE_4MBMENU) {
		return MUSIC_MAINMENU;
	}

	return MUSIC_PAUSEMENU;
}

bool func0f0fcbcc(void)
{
	if (g_FileState == FILESTATE_UNSELECTED && g_Vars.stagenum == STAGE_CITRAINING) {
		return true;
	}

	return false;
}

bool func0f0fcc04(void)
{
	return false;
}

u32 menuGetRoot(void)
{
	if (g_MenuData.count == 0) {
		return 0;
	}

	return g_MenuData.root;
}

#if VERSION >= VERSION_NTSC_1_0
struct menudialogdef g_PakAttemptRepairMenuDialog;

s32 menuhandler000fcc34(s32 operation, struct menuitem *item, union handlerdata *data)
{
	bool done = false;

	if (operation == MENUOP_SET) {
		while (!done) {
			done = true;

			if (g_Menus[g_MpPlayerNum].curdialog) {
				if (g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakRepairSuccessMenuDialog
						|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakRemovedMenuDialog
						|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakRepairFailedMenuDialog
						|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakAttemptRepairMenuDialog
						|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakDamagedMenuDialog
						|| g_Menus[g_MpPlayerNum].curdialog->definition == &g_PakFullMenuDialog) {
					done = false;
					menuPopDialog();
				}
			}
		}
	}

	return 0;
}
#endif

s32 menudialog000fcd48(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data)
{
	if (operation == MENUOP_TICK) {
		if (g_Menus[g_MpPlayerNum].curdialog
				&& g_Menus[g_MpPlayerNum].curdialog->definition == dialogdef
				&& joy000155b4(g_Menus[g_MpPlayerNum].fm.device3) == 0) {
			func0f0f3704(&g_PakRemovedMenuDialog);
		}
	}

	return 0;
}

#if VERSION >= VERSION_NTSC_1_0
s32 func0f0fcdd0(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
		func0f0f3704(&g_PakDamagedMenuDialog);
	}

	return 0;
}
#endif

s32 menuhandlerRepairPak(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
		if (pakRepair(g_Menus[g_MpPlayerNum].fm.device3)) {
			func0f0f3704(&g_PakRepairSuccessMenuDialog);
		} else {
			func0f0f3704(&g_PakRepairFailedMenuDialog);
		}
	}

	return 0;
}

void func0f0fce8c(struct menudialogdef *dialogdef, s32 playernum, s32 arg2)
{
	s32 prevplayernum = g_MpPlayerNum;

	g_MpPlayerNum = playernum;
	g_Menus[g_MpPlayerNum].fm.device3 = arg2;

	if (g_Menus[g_MpPlayerNum].curdialog == NULL) {
		if (PLAYERCOUNT() == 1) {
			menuPushRootDialog(dialogdef, MENUROOT_MAINMENU);
			lvSetPaused(true);
			g_Vars.currentplayer->pausemode = PAUSEMODE_PAUSED;
		} else {
			menuPushRootDialog(dialogdef, MENUROOT_MPPAUSE);
		}
	} else {
		menuPushDialog(dialogdef);
	}

	g_MpPlayerNum = prevplayernum;
}

struct menuitem g_PakRemovedMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_174, // "The Controller Pak has been removed."
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
#if VERSION >= VERSION_NTSC_1_0
		MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		menuhandler000fcc34,
#else
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		NULL,
#endif
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakRemovedMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_173, // "Error"
	g_PakRemovedMenuItems,
	NULL,
	0,
	NULL,
};

struct menuitem g_PakRepairSuccessMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_181, // "The Controller Pak has been repaired."
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
#if VERSION >= VERSION_NTSC_1_0
		MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		menuhandler000fcc34,
#else
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		NULL,
#endif
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakRepairSuccessMenuDialog = {
	MENUDIALOGTYPE_SUCCESS,
	L_MPWEAPONS_180, // "Repair Successful"
	g_PakRepairSuccessMenuItems,
	menudialog000fcd48,
	0,
	NULL,
};

struct menuitem g_PakRepairFailedMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_183, // "The Controller Pak  cannot be repaired. You will not be able to load from or save to this Controller Pak."
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakRepairFailedMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_182, // "Repair Failed"
	g_PakRepairFailedMenuItems,
	menudialog000fcd48,
	0,
	NULL,
};

struct menuitem g_PakAttemptRepairMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_176, // "Are you sure you want to attempt repair of this Controller Pak?"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_177, // "Data may be lost!"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_178, // "Cancel"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_179, // "Repair"
		0,
		menuhandlerRepairPak,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakAttemptRepairMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_175, // "Attempt Repair"
	g_PakAttemptRepairMenuItems,
	menudialog000fcd48,
	0,
	NULL,
};

char *menuTextSaveDeviceName(struct menuitem *item)
{
	u16 devices[] = {
		L_OPTIONS_112, // "Controller Pak 1"
		L_OPTIONS_113, // "Controller Pak 2"
		L_OPTIONS_114, // "Controller Pak 3"
		L_OPTIONS_115, // "Controller Pak 4"
#if VERSION >= VERSION_NTSC_1_0
		L_OPTIONS_111, // "Game Pak"
#endif
	};

	if ((u8)g_Menus[g_MpPlayerNum].fm.device3 < ARRAYCOUNT(devices)) {
		return langGet(devices[(u8)g_Menus[g_MpPlayerNum].fm.device3]);
	}

	return NULL;
}

s32 menuhandlerRetrySavePak(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
		menuPopDialog();

#if VERSION >= VERSION_NTSC_1_0
		g_Vars.unk0004e4 &= 0xfff0;
		g_Vars.unk0004e4 |= 8;
		g_Vars.unk0004e4 |= 1 << ((u8)g_Menus[g_MpPlayerNum].fm.device3 + 8);
#else
		pak0f1169c8(g_Menus[g_MpPlayerNum].fm.device3, 0);
#endif
	}

	return 0;
}

s32 menuhandlerWarnRepairPak(s32 operation, struct menuitem *item, union handlerdata *data)
{
	if (operation == MENUOP_SET) {
#if VERSION >= VERSION_NTSC_1_0
		menuPushDialog(&g_PakAttemptRepairMenuDialog);
#else
		func0f0f3704(&g_PakAttemptRepairMenuDialog);
#endif
	}

	return 0;
}

u32 func0f0fd118(u32 playernum)
{
	u32 result = 0;

	if (g_Vars.normmplayerisrunning) {
		if (g_MpSetup.chrslots & (1 << playernum)) {
			result = playernum;
		}
	} else {
		if ((g_Vars.coopplayernum >= 0 || g_Vars.antiplayernum >= 0)
				&& PLAYERCOUNT() >= 2
				&& playernum == 1) {
			result = 1;
		}
	}

	return result;
}

bool func0f0fd1f4(s32 arg0, s32 arg1)
{
	s32 playernum = func0f0fd118(arg0);
	bool result = true;

	if (g_Vars.lvframenum < 20) {
		result = false;
	}

	if (g_FileState == FILESTATE_UNSELECTED) {
		result = false;
	}

	if (g_Vars.stagenum == STAGE_BOOTPAKMENU) {
		result = true;
	}

	if (g_Menus[playernum].curdialog) {
		if (g_Menus[playernum].curdialog->definition == &g_PakDamagedMenuDialog
#if VERSION >= VERSION_NTSC_1_0
				|| g_Menus[playernum].curdialog->definition == &g_PakCannotReadGameBoyMenuDialog
				|| g_Menus[playernum].curdialog->definition == &g_PakDataLostMenuDialog
#endif
				|| g_Menus[playernum].curdialog->definition == &g_PakFullMenuDialog
				|| g_Menus[playernum].curdialog->definition == &g_PakAttemptRepairMenuDialog
				|| g_Menus[playernum].curdialog->definition == &g_PakRemovedMenuDialog
				|| g_Menus[playernum].curdialog->definition == &g_PakRepairSuccessMenuDialog
				|| g_Menus[playernum].curdialog->definition == &g_PakRepairFailedMenuDialog
				) {
			result = false;
		}
	} else if (g_MenuData.nextbg != 255 || g_MenuData.bg || g_MenuData.unk5d4) {
		result = false;
	}

	return result;
}

void func0f0fd320(s32 arg0, s32 arg1)
{
	s32 prevplayernum = g_MpPlayerNum;
	s32 playernum = func0f0fd118(arg0);
	bool found;
	s32 i;

	g_MpPlayerNum = playernum;

	switch (arg1) {
	case 1:
	case 2:
		func0f0fce8c(&g_PakDamagedMenuDialog, playernum, arg0);
		break;
	case 0:
		found = false;

		for (i = 0; i < g_Menus[g_MpPlayerNum].depth; i++) {
			if (g_Menus[g_MpPlayerNum].layers[i].siblings[0]
					&& g_Menus[g_MpPlayerNum].layers[i].siblings[0]->definition == &g_PakChoosePakMenuDialog) {
				found = true;
			}
		}

		if (!found) {
			func0f0fce8c(&g_PakFullMenuDialog, playernum, arg0);
		}
		break;
#if VERSION >= VERSION_NTSC_1_0
	case 3:
		func0f0fce8c(&g_PakCannotReadGameBoyMenuDialog, playernum, arg0);
		break;
	case 4:
		func0f0fce8c(&g_PakDataLostMenuDialog, playernum, arg0);
		break;
#endif
	}

	g_MpPlayerNum = prevplayernum;
}

void func0f0fd494(struct coord *pos)
{
	f32 xy[2];
	struct coord coord;
	Mtxf *matrix;

	g_MenuData.unk5d5_04 = true;

	matrix = camGetWorldToScreenMtxf();

	mtx4TransformVec(matrix, pos, &coord);
	cam0f0b4d04(&coord, xy);

	g_MenuData.unk670 = (s32)xy[0] - viGetWidth() / 2;
	g_MenuData.unk674 = (s32)xy[1] - viGetHeight() / 2;

	g_MenuData.unk5d5_05 = false;
}

void func0f0fd548(s32 arg0)
{
	g_MenuData.unk669[g_MenuData.unk66e++] = arg0;
	g_MenuData.unk66f = 0;
}

struct menudialog *menuIsDialogOpen(struct menudialogdef *dialogdef)
{
	s32 i;
	s32 j;

	if (g_Menus[g_MpPlayerNum].curdialog) {
		for (i = 0; i < g_Menus[g_MpPlayerNum].depth; i++) {
			for (j = 0; j < g_Menus[g_MpPlayerNum].layers[i].numsiblings; j++) {
				if (g_Menus[g_MpPlayerNum].layers[i].siblings[j]
						&& g_Menus[g_MpPlayerNum].layers[i].siblings[j]->definition == dialogdef) {
					return g_Menus[g_MpPlayerNum].layers[i].siblings[j];
				}
			}
		}
	}

	return false;
}

struct menuitem g_PakDamagedMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE,
		(u32)&menuTextSaveDeviceName,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_065, // "is damaged or"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_066, // "inserted incorrectly."
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
		0,
		L_MPWEAPONS_067, // "Attempt Repair"
		0,
		menuhandlerWarnRepairPak,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		0,
		L_MPWEAPONS_068, // "Retry"
		0,
		menuhandlerRetrySavePak,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_MPWEAPONS_069, // "Continue without using the Controller Pak"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakDamagedMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_064, // "Damaged Controller Pak"
	g_PakDamagedMenuItems,
	menudialog000fcd48,
#if VERSION >= VERSION_NTSC_1_0
	0x00000020,
#else
	0,
#endif
	NULL,
};

struct menuitem g_PakFullMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_SELECTABLE_CENTRE,
		(u32)&menuTextSaveDeviceName,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_071, // "is too full to save note - 1 note and 28 pages required to save."
		0,
		NULL,
	},
#if VERSION != VERSION_JPN_FINAL
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_LESSHEIGHT,
		L_OPTIONS_003, // ""
		0,
		NULL,
	},
#endif
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SMALLFONT | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_072, // "Enter the Controller Pak Menu to free some space (hold START while powering up.)"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SEPARATOR,
		0,
		0,
		0,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_SELECTABLE,
		0,
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG | MENUITEMFLAG_SELECTABLE_CENTRE,
		L_MPWEAPONS_073, // "OK"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakFullMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_070, // "Full Controller Pak"
	g_PakFullMenuItems,
	menudialog000fcd48,
#if VERSION >= VERSION_NTSC_1_0
	0x00000020,
#else
	0,
#endif
	NULL,
};

#if VERSION >= VERSION_NTSC_1_0
struct menuitem g_PakCannotReadGameBoyMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING,
		L_MPWEAPONS_254, // "Cannot read Game Boy Game Pak. Check connections and make sure correct Game Boy Game Pak is being used."
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
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_MPWEAPONS_255, // "Cancel"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakCannotReadGameBoyMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_253, // "Error"
	g_PakCannotReadGameBoyMenuItems,
	NULL,
	MENUDIALOGFLAG_IGNOREBACK,
	NULL,
};

struct menuitem g_PakDataLostMenuItems[] = {
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE,
		(u32)&menuTextSaveDeviceName,
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_257, // "The saved data has"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_258, // "been erased due to"
		0,
		NULL,
	},
	{
		MENUITEMTYPE_LABEL,
		0,
		MENUITEMFLAG_LESSLEFTPADDING | MENUITEMFLAG_SELECTABLE_CENTRE | MENUITEMFLAG_LESSHEIGHT,
		L_MPWEAPONS_259, // "corruption or damage."
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
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_MPWEAPONS_260, // "Cancel"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_PakDataLostMenuDialog = {
	MENUDIALOGTYPE_DANGER,
	L_MPWEAPONS_256, // "Error"
	g_PakDataLostMenuItems,
	NULL,
	MENUDIALOGFLAG_IGNOREBACK,
	NULL,
};
#endif
