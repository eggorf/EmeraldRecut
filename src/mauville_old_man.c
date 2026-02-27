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

static void InitGiddyTaleList(void);
static void StorytellerSetup(void);
static void Storyteller_ResetFlag(void);

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

void HasHipsterTaughtWord(void)
{
    gSpecialVar_Result = (&gSaveBlock1Ptr->oldMan.hipster)->taughtWord;
}

void SetHipsterTaughtWord(void)
{
    (&gSaveBlock1Ptr->oldMan.hipster)->taughtWord = TRUE;
}

void HipsterTryTeachWord(void)
{
    u16 word = UnlockRandomTrendySaying();

    if (word == EC_EMPTY_WORD)
    {
        // All words already unlocked
        gSpecialVar_Result = FALSE;
    }
    else
    {
        CopyEasyChatWord(gStringVar1, word);
        gSpecialVar_Result = TRUE;
    }
}

void GiddyShouldTellAnotherTale(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;

    if (giddy->taleCounter == GIDDY_MAX_TALES)
    {
        gSpecialVar_Result = FALSE;
        giddy->taleCounter = 0;
    }
    else
    {
        gSpecialVar_Result = TRUE;
    }
}

void GenerateGiddyLine(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;

    if (giddy->taleCounter == 0)
        InitGiddyTaleList();

    // A line from Giddy is either a line following this format:
    // "{random word} is so {adjective}! Don't you agree?",
    // or one of the texts in sGiddyQuestions.
    if (giddy->randomWords[giddy->taleCounter] != EC_EMPTY_WORD)
    {
        u8 *stringPtr;
        u32 adjective = Random();
        adjective %= ARRAY_COUNT(sGiddyAdjectives);

        stringPtr = CopyEasyChatWord(gStringVar4, giddy->randomWords[giddy->taleCounter]);
        stringPtr = StringCopy(stringPtr, GiddyText_Is);
        stringPtr = StringCopy(stringPtr, sGiddyAdjectives[adjective]);
        StringCopy(stringPtr, GiddyText_DontYouAgree);
    }
    else
    {
        StringCopy(gStringVar4, sGiddyQuestions[giddy->questionList[giddy->questionNum++]]);
    }

    // 10% chance for Giddy to stop telling tales.
    if (!(Random() % 10))
        giddy->taleCounter = GIDDY_MAX_TALES;
    else
        giddy->taleCounter++;

    gSpecialVar_Result = TRUE;
}

static void InitGiddyTaleList(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;
    u16 wordGroupsAndCount[][2] = {
        {EC_GROUP_POKEMON,   0},
        {EC_GROUP_LIFESTYLE, 0},
        {EC_GROUP_HOBBIES,   0},
        {EC_GROUP_MOVE_1,    0},
        {EC_GROUP_MOVE_2,    0},
        {EC_GROUP_POKEMON_NATIONAL, 0}
    };
    u16 i;
    u16 totalWords;
    u16 temp;
    u16 var; // re-used

    // Shuffle question list
    for (i = 0; i < GIDDY_MAX_QUESTIONS; i++)
        giddy->questionList[i] = i;
    for (i = 0; i < GIDDY_MAX_QUESTIONS; i++)
    {
        var = Random() % (i + 1);
        SWAP(giddy->questionList[i], giddy->questionList[var], temp);
    }

    // Count total number of words in above word groups
    totalWords = 0;
    for (i = 0; i < ARRAY_COUNT(wordGroupsAndCount); i++)
    {
        wordGroupsAndCount[i][1] = EasyChat_GetNumWordsInGroup(wordGroupsAndCount[i][0]);
        totalWords += wordGroupsAndCount[i][1];
    }

    giddy->questionNum = 0;
    temp = 0;
    for (i = 0; i < GIDDY_MAX_TALES; i++)
    {
        var = Random() % 10;
        if (var < 3 && temp < GIDDY_MAX_QUESTIONS)
        {
            // 30% chance for word to be empty (in which case Giddy
            // will say one of his non-random questions), unless
            // the limit for questions has been reached already.
            giddy->randomWords[i] = EC_EMPTY_WORD;
            temp++;
        }
        else
        {
            // Pick a random word id, then advance through the word
            // groups until the group where that id landed.
            s16 randWord = Random() % totalWords;
            for (var = 0; i < ARRAY_COUNT(wordGroupsAndCount); var++)
                if ((randWord -= wordGroupsAndCount[var][1]) <= 0)
                    break;
            if (var == ARRAY_COUNT(wordGroupsAndCount))
                var = 0;

            // Save the randomly selected word
            giddy->randomWords[i] = GetRandomEasyChatWordFromUnlockedGroup(wordGroupsAndCount[var][0]);
        }
    }
}
static void ResetBardFlag(void)
{
    (&gSaveBlock1Ptr->oldMan.bard)->hasChangedSong = FALSE;
}

static void ResetHipsterFlag(void)
{
    (&gSaveBlock1Ptr->oldMan.hipster)->taughtWord = FALSE;
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
