#include "global.h"
#include "main.h"
#include "constants/songs.h"
#include "constants/event_objects.h"
#include "mauville_old_man.h"
#include "event_data.h"
#include "string_util.h"
#include "text.h"
#include "easy_chat.h"
#include "script.h"
#include "random.h"
#include "event_scripts.h"
#include "task.h"
#include "menu.h"
#include "m4a.h"
#include "sound.h"
#include "strings.h"
#include "overworld.h"
#include "field_message_box.h"
#include "script_menu.h"
#include "trader.h"
#include "m4a.h"
#include "constants/mauville_old_man.h"

static u8 sSelectedStory;

static void SetupTrader(void)
{
    TraderSetup();
}

void SetMauvilleOldMan(void)
{
    SetupTrader();
    SetMauvilleOldManObjEventGfx();
}

u8 GetCurrentMauvilleOldMan(void)
{
    return 2; //trader
    //return gSaveBlock1Ptr->oldMan.common.id;
}

void Script_GetCurrentMauvilleMan(void)
{
    gSpecialVar_Result = GetCurrentMauvilleOldMan();
}

static void ResetTraderFlag(void)
{
    Trader_ResetFlag();
}

void ResetMauvilleOldManFlag(void)
{
    ResetTraderFlag();
    SetMauvilleOldManObjEventGfx();
}

static void EnableTextPrinters(void)
{
    gDisableTextPrinters = FALSE;
}

static void DisableTextPrinters(struct TextPrinterTemplate * printer, u16 renderCmd)
{
    gDisableTextPrinters = TRUE;
}

enum {
    SOUND_STATE_START,
    SOUND_STATE_PLAY,
    SOUND_STATE_SET_BASE,
    SOUND_STATE_END,
    SOUND_STATE_WAIT,
};

void SetMauvilleOldManObjEventGfx(void)
{
    VarSet(VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_BARD);
}
