#ifndef IN_GAME_GAME_0B0FD0_H
#define IN_GAME_GAME_0B0FD0_H
#include <ultra64.h>
#include "data.h"
#include "types.h"

struct weapon *weaponFindById(s32 itemid);
struct weaponfunc *weaponGetFunctionById(u32 weaponnum, u32 which);
struct weaponfunc *gsetGetWeaponFunction2(struct gset *gset);
struct weaponfunc *gsetGetWeaponFunction(struct gset *gset);
struct weaponfunc *weaponGetFunction(struct gset *gset, s32 which);
struct weaponfunc *currentPlayerGetWeaponFunction(u32 hand);
struct invaimsettings *gsetGetAimSettings(struct gset *gset);
struct inventory_ammo *weaponGetAmmoByFunction(u32 weaponnum, u32 funcnum);
f32 handGetXShift(s32 handnum);
f32 func0f0b131c(s32 handnum);
f32 currentPlayerGetGunZoomFov(void);
void currentPlayerZoomOut(f32 fovpersec);
void currentPlayerZoomIn(f32 fovpersec);
bool weaponHasFlag(s32 itemid, u32 flag);
bool weaponHasFlag2(s32 itemid, u64 flag);
bool weaponHasAimFlag(s32 weaponnum, u32 flag);
bool weaponHasAmmoFlag(s32 weaponnum, s32 funcnum, u32 flag);
s32 currentPlayerGetDeviceState(s32 weaponnum);
void currentPlayerSetDeviceActive(s32 weaponum, bool active);
u16 weaponGetFileNum(s32 weaponnum);
u16 weaponGetFileNum2(s32 weaponnum);
s32 weaponGetChrModel(s32 weaponnum);
s32 weaponGetExtraScale(s32 weaponnum);
void gsetPopulateFromCurrentPlayer(s32 handnum, struct gset *gset);
struct inventory_ammo *gsetGetAmmoDefinition(struct gset *gset);
u8 gsetGetSinglePenetration(struct gset *gset);
f32 gsetGetImpactForce(struct gset *gset);
f32 gsetGetDamage(struct gset *gset);
u8 gsetGetFireslotDuration(struct gset *gset);
u16 gsetGetSingleShootSound(struct gset *gset);
bool gsetHasFunctionFlags(struct gset *gset, u32 flags);
s8 weaponGetNumTicksPerShot(u32 weaponnum, u32 funcindex);
u32 currentPlayerGetSight(void);
void gsetGetNoiseSettings(struct gset *gset, struct noisesettings *settings);
struct guncmd *gsetGetPriToSecAnim(struct gset *gset);
struct guncmd *gsetGetSecToPriAnim(struct gset *gset);
struct guncmd *weaponGetReloadAnim(s32 weaponnum, u8 weaponfunc);
s32 weaponGetReplacement(s32 weaponnum, bool isplayer);
s32 ammoGetReplacement(s32 ammonum);
bool weaponHasLaserSight(s32 weaponnum);
u32 weaponGetCustomTint(s32 weaponnum);

#endif
