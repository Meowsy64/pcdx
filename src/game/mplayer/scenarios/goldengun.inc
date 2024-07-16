/**
 * The Man With the Golden Gun
 *
 * There is one "Golden Gun", and only kills made with it count.
 */

bool invHasGoldenGun(void)
{
	if (g_MpSetup.scenario != MPSCENARIO_GOLDENGUN) {
		return false;
	}

	if (g_Vars.currentplayer->isdead == false) {
		return invHasSingleWeaponExcAllGuns(g_Vars.mpmgg_weaponnum);
	}

	return false;
}

bool invGoldenGunEquipped(s32 mpchrnum)
{
	if (g_MpSetup.scenario != MPSCENARIO_GOLDENGUN) {
		return false;
	}

	if (mpchrnum < PLAYERCOUNT()) { // Human
		return g_Vars.players[mpchrnum]->gunctrl.weaponnum == g_Vars.mpmgg_weaponnum;
	} else { // Simulant
		return g_MpAllChrPtrs[mpchrnum]->aibot->weaponnum == g_Vars.mpmgg_weaponnum;
	}
}

struct menuitem g_MggOptionsMenuItems[] = {
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_222, // "One-Hit Kills"
		MPOPTION_ONEHITKILLS,
		menuhandlerMpOneHitKills,
	},
	{
		MENUITEMTYPE_DROPDOWN,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_223, // "Slow Motion"
		0,
		menuhandlerMpSlowMotion,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_224, // "Fast Movement"
		MPOPTION_FASTMOVEMENT,
		menuhandlerMpCheckboxOption,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_225, // "Display Team"
		MPOPTION_DISPLAYTEAM,
		menuhandlerMpDisplayTeam,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_226, // "No Radar"
		MPOPTION_NORADAR,
		menuhandlerMpCheckboxOption,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_227, // "No Auto-Aim"
		MPOPTION_NOAUTOAIM,
		menuhandlerMpCheckboxOption,
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
		MENUITEMTYPE_DROPDOWN,
		0,
		MENUITEMFLAG_LOCKABLEMINOR | MENUITEMFLAG_DROPDOWN_BELOW,
		L_MPMENU_GOLDENGUNOPT, // "Golden Gun"
		0x00000006,
		menuhandlerMpWeaponSlot,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_HIGHLIGHTGOLDENGUN, // "Highlight Golden Gun"
		MPOPTION_MGG_HIGHLIGHTGOLDENGUN,
		menuhandlerMpCheckboxOption,
	},
	{
		MENUITEMTYPE_CHECKBOX,
		0,
		MENUITEMFLAG_LOCKABLEMINOR,
		L_MPMENU_238, // "Show on Radar"
		MPOPTION_MGG_SHOWONRADAR,
		menuhandlerMpCheckboxOption,
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
		MENUITEMFLAG_SELECTABLE_CLOSESDIALOG,
		L_MPMENU_239, // "Back"
		0,
		NULL,
	},
	{ MENUITEMTYPE_END },
};

struct menudialogdef g_MggOptionsMenuDialog = {
	MENUDIALOGTYPE_DEFAULT,
	L_MPMENU_GOLDENGUNOPTIONS, // "Golden Gun Options"
	g_MggOptionsMenuItems,
	mpOptionsMenuDialog,
	MENUDIALOGFLAG_MPLOCKABLE,
	NULL,
};

void mggReset(void)
{
	s32 i;

	g_ScenarioData.mgg.goldengun = NULL;

	for (i = 0; i < ARRAYCOUNT(g_ScenarioData.mgg.numpoints); i++) {
		g_ScenarioData.mgg.numpoints[i] = 0;
	}
}

void mggInit(void)
{
	if (!mpIsValidGoldenGun(g_Vars.mpmgg_mpweaponnum)) {
		g_Vars.mpmgg_mpweaponnum = MPWEAPON_DY357LX;
		g_Vars.mpmgg_weaponnum = WEAPON_DY357LX;
	}

    mggReset();
}

s32 mggGetRandomPad(void)
{
	struct prop *prop = g_Vars.activeprops;
	struct defaultobj *obj;
	s32 count = 0;
	struct defaultobj *candidates[20];

	while (prop && count < 20) {
		if (prop->type == PROPTYPE_OBJ)
		{
			obj = prop->obj;

			if (obj->type == OBJTYPE_MULTIAMMOCRATE) {
				candidates[count] = obj;
				count++;
			}
		}

		prop = prop->next;
	}

	if (count > 0) {
		count = random() % count;
		return candidates[count]->pad;
	}

    return 0;
}

struct weaponobj g_MggGoldenGunObj;

struct prop *mggSpawnRandomGoldenGunWeapon(void)
{
	struct weaponobj template = {
		512,                    // extrascale
		0,                      // hidden2
		OBJTYPE_WEAPON,         // type
		MODEL_CHRDATATHIEF,     // modelnum
		0,                      // pad
		OBJFLAG_FALL | OBJFLAG_INVINCIBLE | OBJFLAG_FORCENOBOUNCE,
		OBJFLAG2_IMMUNETOGUNFIRE | OBJFLAG2_IMMUNETOEXPLOSIONS,
		0,                      // flags3
		NULL,                   // prop
		NULL,                   // model
		1, 0, 0,                // realrot
		0, 1, 0,
		0, 0, 1,
		0,                      // hidden
		NULL,                   // geo
		NULL,                   // projectile
		0,                      // damage
		1000,                   // maxdamage
		0xff, 0xff, 0xff, 0x00, // shadecol
		0xff, 0xff, 0xff, 0x00, // nextcol
		0x0fff,                 // floorcol
		0,                      // tiles
		g_Vars.mpmgg_weaponnum, // weaponnum
		0,                      // unk5d
		0,                      // unk5e
		FUNC_PRIMARY,           // gunfunc
		0,                      // fadeouttimer60
		-1,                     // dualweaponnum
		-1,                     // timer240
		NULL,                   // dualweapon
	};

	g_MggGoldenGunObj = template;
	g_MggGoldenGunObj.base.pad = mggGetRandomPad();

	setupPlaceWeapon(&g_MggGoldenGunObj, 999);

	g_MggGoldenGunObj.base.hidden2 &= ~OBJH2FLAG_CANREGEN;

	if (g_MggGoldenGunObj.base.prop) {
		g_MggGoldenGunObj.base.prop->forcetick = true;
	}

    return g_MggGoldenGunObj.base.prop;
}

struct ammocrateobj g_MggGoldenGunAmmoObj;

struct prop *mggSpawnRandomGoldenGunAmmo(void)
{
    s32 ammotype = bgunGetAmmoTypeForWeapon(g_Vars.mpmgg_weaponnum, FUNC_PRIMARY);
    if (ammotype == AMMOTYPE_NONE) {
        return NULL;
    }

	struct ammocrateobj template = {
		512,                    // extrascale
		0,                      // hidden2
		OBJTYPE_AMMOCRATE,      // type
		MODEL_MULTI_AMMO_CRATE, // modelnum
		0,                      // pad
		OBJFLAG_FALL | OBJFLAG_INVINCIBLE | OBJFLAG_FORCENOBOUNCE,
		OBJFLAG2_IMMUNETOGUNFIRE | OBJFLAG2_IMMUNETOEXPLOSIONS,
		0,                      // flags3
		NULL,                   // prop
		NULL,                   // model
		1, 0, 0,                // realrot
		0, 1, 0,
		0, 0, 1,
		0,                      // hidden
		NULL,                   // geo
		NULL,                   // projectile
		0,                      // damage
		1000,                   // maxdamage
		0xff, 0xff, 0xff, 0x00, // shadecol
		0xff, 0xff, 0xff, 0x00, // nextcol
		0x0fff,                 // floorcol
		0,                      // tiles
        ammotype,               // ammotype
	};

	g_MggGoldenGunAmmoObj = template;
	g_MggGoldenGunAmmoObj.base.pad = mggGetRandomPad();

    setupCreateObject(&g_MggGoldenGunAmmoObj.base, 999);

	//g_MggGoldenGunAmmoObj.base.hidden2 &= ~OBJH2FLAG_CANREGEN;

	if (g_MggGoldenGunAmmoObj.base.prop) {
		g_MggGoldenGunAmmoObj.base.prop->forcetick = true;
	}

    return g_MggGoldenGunAmmoObj.base.prop;
}

void mggTick(void)
{
	s32 i;
	u32 prevplayernum = g_Vars.currentplayernum;
	struct prop *prop;

	g_ScenarioData.mgg.goldengun = NULL;

	// Check if a player possesses it
	for (i = 0; i < PLAYERCOUNT(); i++) {
		setCurrentPlayerNum(i);

		if (invHasGoldenGun()) {
			g_ScenarioData.mgg.goldengun = g_Vars.currentplayer->prop;
			return;
		}
	}

	setCurrentPlayerNum(prevplayernum);

	// Check if a simulant possesses it
	if (g_ScenarioData.mgg.goldengun == NULL) {
		for (i = PLAYERCOUNT(); i < g_MpNumChrs; i++) {
			if (g_MpAllChrPtrs[i]->aibot->hasuplink) {
				g_ScenarioData.mgg.goldengun = g_MpAllChrPtrs[i]->prop;
				return;
			}
		}
	}

	if (g_ScenarioData.mgg.goldengun == NULL && g_Vars.mpmgg_weaponnum == WEAPON_LAPTOPGUN) {
		for (i = 0; i < g_MaxThrownLaptops; i++) {
			if (g_ThrownLaptops[i].weaponnum == g_Vars.mpmgg_weaponnum) {
				g_ScenarioData.mgg.goldengun = g_ThrownLaptops[i].base.prop;
				return;
			}
		}
	}

	if (g_ScenarioData.mgg.goldengun == NULL) {
		// Check if the Golden Gun is on the ground
		struct prop *weapongoldengun = NULL;

		prop = g_Vars.activeprops;

		while (prop) {
			if (prop->type == PROPTYPE_WEAPON && weapongoldengun == NULL) {
				struct weaponobj *weapon = prop->weapon;

				if (weapon->weaponnum == g_Vars.mpmgg_weaponnum && prop->timetoregen == 0) {
					weapongoldengun = prop;
				}
			} else if (prop->type == PROPTYPE_OBJ && prop->obj->type == OBJTYPE_AUTOGUN) {
				struct autogunobj *laptop = (struct autogunobj *)prop->obj;

				if (laptop->weaponnum == g_Vars.mpmgg_weaponnum) {
					g_ScenarioData.mgg.goldengun = prop;
					return;
				}
			}

			prop = prop->next;
		}

		if (weapongoldengun) {
			g_ScenarioData.mgg.goldengun = weapongoldengun;
			return;
		}
	}

    if (g_ScenarioData.mgg.goldengun == NULL) {
        bool hasGoldenGunInSetup = false;
        int max = ARRAYCOUNT(g_MpSetup.weapons) + 1;

        if (g_Vars.stagenum == STAGE_MP_WAREHOUSE) {
            max = 6;
        } else if (g_Vars.stagenum == STAGE_MP_FORTRESS) {
            max = 5;
        }

        for (int i = 0; i < max; i++) {
            if (mpGetMpWeaponnumByLocation(i) == g_Vars.mpmgg_mpweaponnum) {
                hasGoldenGunInSetup = true;
                break;
            }
        }

        if (!hasGoldenGunInSetup) {
            g_ScenarioData.mgg.goldengun = mggSpawnRandomGoldenGunWeapon();
            mggSpawnRandomGoldenGunAmmo();
        }
        return;
    }
}

void mggReadSave(struct savebuffer *buffer)
{
	g_Vars.mpmgg_mpweaponnum = savebufferReadBits(buffer, 8);

	if (!mpIsValidGoldenGun(g_Vars.mpmgg_mpweaponnum)) {
		g_Vars.mpmgg_mpweaponnum = MPWEAPON_DY357LX;
	}

	g_Vars.mpmgg_weaponnum = g_MpWeapons[g_Vars.mpmgg_mpweaponnum].weaponnum;
}

void mggWriteSave(struct savebuffer *buffer)
{
	if (!mpIsValidGoldenGun(g_Vars.mpmgg_mpweaponnum)) {
		g_Vars.mpmgg_mpweaponnum = MPWEAPON_DY357LX;
		g_Vars.mpmgg_weaponnum = WEAPON_DY357LX;
	}

	savebufferOr(buffer, g_Vars.mpmgg_mpweaponnum, 8);
}

void mggCalculatePlayerScore(struct mpchrconfig *mpchr, s32 mpchrnum, s32 *score, s32 *deaths)
{
	s32 index = func0f18d0e8(mpchrnum);

	*score = 0;

	if (index >= 0) {
		*score += g_ScenarioData.mgg.numpoints[index];
	}

	*deaths = mpchr->numdeaths;
}

Gfx *mggRadarExtra(Gfx *gdl)
{
	struct scenariodata_mgg *data = &g_ScenarioData.mgg;
	struct coord dist;

	if (g_MpSetup.options & MPOPTION_MGG_SHOWONRADAR) {
		// Show the Golden Gun
		if (data->goldengun && data->goldengun->type != PROPTYPE_PLAYER && data->goldengun->type != PROPTYPE_CHR) {
			dist.x = data->goldengun->pos.x - g_Vars.currentplayer->prop->pos.x;
			dist.y = data->goldengun->pos.y - g_Vars.currentplayer->prop->pos.y;
			dist.z = data->goldengun->pos.z - g_Vars.currentplayer->prop->pos.z;

			gdl = radarDrawDot(gdl, data->goldengun, &dist, 0x00ff0000, 0x00000000, true);
		}
	}

	return gdl;
}

bool mggRadarChr(Gfx **gdl, struct prop *prop)
{
	if ((g_MpSetup.options & MPOPTION_MGG_SHOWONRADAR) && g_ScenarioData.mgg.goldengun) {
		if (prop == g_ScenarioData.mgg.goldengun &&
				(prop->type == PROPTYPE_PLAYER || prop->type == PROPTYPE_CHR)) {
			struct coord dist;
			dist.x = prop->pos.x - g_Vars.currentplayer->prop->pos.x;
			dist.y = prop->pos.y - g_Vars.currentplayer->prop->pos.y;
			dist.z = prop->pos.z - g_Vars.currentplayer->prop->pos.z;

			if (g_MpSetup.options & MPOPTION_TEAMSENABLED) {
				u32 colour = g_TeamColours[radarGetTeamIndex(prop->chr->team)];
				*gdl = radarDrawDot(*gdl, g_ScenarioData.mgg.goldengun, &dist, colour, 0, 1);
			} else {
				*gdl = radarDrawDot(*gdl, g_ScenarioData.mgg.goldengun, &dist, 0x00ff0000, 0, 1);
			}

			return true;
		}
	}

	return false;
}

bool mggHighlightProp(struct prop *prop, s32 *colour)
{
	if (g_MpSetup.options & MPOPTION_MGG_HIGHLIGHTGOLDENGUN) {
		bool highlight = false;

		if (prop == g_ScenarioData.mgg.goldengun) {
			colour[0] = 0;
			colour[1] = 0xff;
			colour[2] = 0;
			colour[3] = 0x40;

			return true;
		}
	}

	return false;
}

void mggHandleDeath(s32 aplayernum, s32 vplayernum, struct gset gset)
{
	char text[64];

	if (aplayernum != vplayernum) {
		if (aplayernum >= 0) {
			if (scenarioChrsAreSameTeam(aplayernum, vplayernum)) {
				sprintf(text, "%s%s", langGet(L_MPWEAPONS_008), langGet(L_MPWEAPONS_009)); // "You're supposed to look\nafter your friends!"
				scenarioCreateHudmsg(aplayernum, text);
			} else if (gset.weaponnum == g_Vars.mpmgg_weaponnum) {
				g_ScenarioData.mgg.numpoints[aplayernum]++;
				scenarioCreateHudmsg(aplayernum, langGet(L_MPWEAPONS_GOLDENGUNKILL)); // "Another glittering end!"
			}
		}
	}
}
