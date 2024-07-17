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
#include "game/shadowcampaign.h"
#include "bss.h"
#include "data.h"
#include "string.h"
#include "types.h"

bool shadowCampaignIsLogicRunning(void)
{
    if (cheatIsActive(CHEAT_SHADOWCAMPAIGN)) {
        return true;
    }

    return false;
}
