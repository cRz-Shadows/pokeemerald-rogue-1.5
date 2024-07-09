#include "global.h"
#include "constants/game_stat.h"
#include "constants/items.h"

#include "battle.h"
#include "event_data.h"
#include "data.h"
#include "item.h"
#include "money.h"
#include "pokedex.h"
#include "string_util.h"

#include "rogue.h"
#include "rogue_adventurepaths.h"
#include "rogue_controller.h"
#include "rogue_quest.h"
#include "rogue_popup.h"

extern const u8 gText_QuestRewardGive[];
extern const u8 gText_QuestRewardGiveMoney[];
extern const u8 gText_QuestRewardGiveMon[];
extern const u8 gText_QuestRewardGiveShinyMon[];
extern const u8 gText_QuestLogStatusIncomplete[];

extern EWRAM_DATA struct RogueGlobalData gRogueGlobalData;

static EWRAM_DATA u8 sRewardQuest = 0;
static EWRAM_DATA u8 sRewardParam = 0;
static EWRAM_DATA u8 sPreviousRouteType = 0;

typedef void (*QuestCallback)(u16 questId, struct RogueQuestState* state);

static const u16 TypeToMonoQuest[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL] = QUEST_NORMAL_Champion,
    [TYPE_FIGHTING] = QUEST_FIGHTING_Champion,
    [TYPE_FLYING] = QUEST_FLYING_Champion,
    [TYPE_POISON] = QUEST_POISON_Champion,
    [TYPE_GROUND] = QUEST_GROUND_Champion,
    [TYPE_ROCK] = QUEST_ROCK_Champion,
    [TYPE_BUG] = QUEST_BUG_Champion,
    [TYPE_GHOST] = QUEST_GHOST_Champion,
    [TYPE_STEEL] = QUEST_STEEL_Champion,
    [TYPE_MYSTERY] = QUEST_NONE,
    [TYPE_FIRE] = QUEST_FIRE_Champion,
    [TYPE_WATER] = QUEST_WATER_Champion,
    [TYPE_GRASS] = QUEST_GRASS_Champion,
    [TYPE_ELECTRIC] = QUEST_ELECTRIC_Champion,
    [TYPE_PSYCHIC] = QUEST_PSYCHIC_Champion,
    [TYPE_ICE] = QUEST_ICE_Champion,
    [TYPE_DRAGON] = QUEST_DRAGON_Champion,
    [TYPE_DARK] = QUEST_DARK_Champion,
#ifdef ROGUE_EXPANSION
    [TYPE_FAIRY] = QUEST_FAIRY_Champion,
#endif
};

bool8 IsSpeciesType(u16 species, u8 type);
bool8 PartyContainsSpecies(struct Pokemon *party, u8 partyCount, u16 species);
bool8 IsSpeciesLegendary(u16 species);


static void UpdateMonoQuests(void);
static void ForEachUnlockedQuest(QuestCallback callback);
static void ActivateAdventureQuests(u16 questId, struct RogueQuestState* state);
static void ActivateHubQuests(u16 questId, struct RogueQuestState* state);

static void UnlockDefaultQuests()
{
    u16 i;

    for(i = QUEST_FirstAdventure; i <= QUEST_MeetPokabbie; ++i)
    {
        TryUnlockQuest(i);
    }

    TryUnlockQuest(QUEST_Collector1);
    TryUnlockQuest(QUEST_ShoppingSpree);
    TryUnlockQuest(QUEST_Bike1);
    TryUnlockQuest(QUEST_NoFainting1);
    TryUnlockQuest(QUEST_MrRandoman);
    TryUnlockQuest(QUEST_DevilDeal);
    TryUnlockQuest(QUEST_BerryCollector);

    // Make sure following quests are unlocked
    {
        u16 i;
        for(i = QUEST_NONE + 1; i < QUEST_CAPACITY; ++i)
        {
            if(IsQuestCollected(i))
                UnlockFollowingQuests(i);
        }
    }
}

void ResetQuestStateAfter(u16 loadedQuestCapacity)
{
    u16 i;

    if(loadedQuestCapacity < QUEST_CAPACITY)
    {
        if(loadedQuestCapacity == 0)
        {
            // Clear these flags here, as they are quest related so we want to make sure they're setup correctly
            FlagClear(FLAG_ROGUE_MET_POKABBIE);
            FlagClear(FLAG_ROGUE_UNCOVERRED_POKABBIE);
        }

        // Reset the state for any new quests
        for(i = loadedQuestCapacity; i < QUEST_CAPACITY; ++i)
        {
            memset(&gRogueGlobalData.questStates[i], 0, sizeof(struct RogueQuestState));
        }

        // Always make sure default quests are unlocked
        UnlockDefaultQuests();

        ForEachUnlockedQuest(ActivateHubQuests);
    }
}

bool8 AnyNewQuests(void)
{
    u16 i;
    struct RogueQuestState* state;

    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked && state->hasNewMarker)
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool8 AnyQuestRewardsPending(void)
{
    u16 i;
    struct RogueQuestState* state;

    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked && state->hasPendingRewards)
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool8 AnyNewQuestsPending(void)
{
    u16 i;
    struct RogueQuestState* state;

    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked && state->hasPendingRewards && DoesQuestHaveUnlocks(i))
        {
            return TRUE;
        }
    }

    return FALSE;
}

u16 GetCompletedQuestCount(void)
{
    u16 i;
    struct RogueQuestState* state;
    u16 count = 0;

    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked && state->isCompleted)
            ++count;
    }

    return count;
}

u16 GetUnlockedQuestCount(void)
{
    u16 i;
    struct RogueQuestState* state;
    u16 count = 0;

    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked)
            ++count;
    }

    return count;
}

u8 GetCompletedQuestPerc(void)
{
    return (GetCompletedQuestCount() * 100) / (QUEST_CAPACITY - 1);
}

bool8 GetQuestState(u16 questId, struct RogueQuestState* outState)
{
    if(questId < QUEST_CAPACITY)
    {
        memcpy(outState, &gRogueGlobalData.questStates[questId], sizeof(struct RogueQuestState));
        return outState->isUnlocked;
    }

    return FALSE;
}

void SetQuestState(u16 questId, struct RogueQuestState* state)
{
    if(questId < QUEST_CAPACITY)
    {
        memcpy(&gRogueGlobalData.questStates[questId], state, sizeof(struct RogueQuestState));
    }
}

bool8 IsQuestRepeatable(u16 questId)
{
    return (gRogueQuests[questId].flags & QUEST_FLAGS_REPEATABLE) != 0;
}

bool8 IsQuestCollected(u16 questId)
{
    struct RogueQuestState state;
    if (GetQuestState(questId, &state))
    {
        return state.isCompleted && !state.hasPendingRewards;
    }

    return FALSE;
}

bool8 IsQuestGloballyTracked(u16 questId)
{
    return (gRogueQuests[questId].flags & QUEST_FLAGS_GLOBALALLY_TRACKED) != 0;
}

bool8 IsQuestActive(u16 questId)
{
    struct RogueQuestState state;
    if (GetQuestState(questId, &state))
    {
        return state.isValid;
    }

    return FALSE;
}

bool8 DoesQuestHaveUnlocks(u16 questId)
{
    return gRogueQuests[questId].unlockedQuests[0] != QUEST_NONE;
}


static struct RogueQuestReward const* GetCurrentRewardTarget()
{
    return &gRogueQuests[sRewardQuest].rewards[sRewardParam];
}

static bool8 QueueTargetRewardQuest()
{
    u16 i;
    struct RogueQuestState* state;
    for(i = 0; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];

        if(state->hasPendingRewards)
        {
            if(sRewardQuest != i)
            {
                sRewardQuest = i;
                sRewardParam = 0;
                UnlockFollowingQuests(sRewardQuest);
                // TODO - Check to see if we have enough space for all of these items
            }
            else
            {
                ++sRewardParam;
            }
            return TRUE;
        }
    }

    sRewardQuest = QUEST_NONE;
    sRewardParam = 0;
    return FALSE;
}

static bool8 QueueNextReward()
{
    while(QueueTargetRewardQuest())
    {
        struct RogueQuestReward const* reward = GetCurrentRewardTarget();

        if(sRewardParam >= QUEST_MAX_REWARD_COUNT || reward->type == QUEST_REWARD_NONE)
        {
            // We've cleared out this quest's rewards
            gRogueGlobalData.questStates[sRewardQuest].hasPendingRewards = FALSE;

            sRewardQuest = QUEST_NONE;
            sRewardParam = 0;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}

static bool8 GiveAndGetNextAnnouncedReward()
{
    while(QueueNextReward())
    {
        struct RogueQuestReward const* reward = GetCurrentRewardTarget();
        bool8 shouldAnnounce = reward->giveText != NULL;

        // Actually give the reward here
        switch(reward->type)
        {
            case QUEST_REWARD_SET_FLAG:
                FlagSet(reward->params[0]);
                break;

            case QUEST_REWARD_CLEAR_FLAG:
                FlagClear(reward->params[0]);
                break;

            case QUEST_REWARD_GIVE_ITEM:
                if(!AddBagItem(reward->params[0], reward->params[1]))
                {
                    AddPCItem(reward->params[0], reward->params[1]);
                }
                shouldAnnounce = TRUE;
                break;

            case QUEST_REWARD_GIVE_MONEY:
                AddMoney(&gSaveBlock1Ptr->money, reward->params[0]);
                shouldAnnounce = TRUE;
                break;

            case QUEST_REWARD_GIVE_POKEMON:
                // Force shiny
                if(reward->params[2] == TRUE)
                    CreateMonForcedShiny(&gEnemyParty[0], reward->params[0], reward->params[1], USE_RANDOM_IVS, OT_ID_PLAYER_ID, 0);
                else 
                    CreateMon(&gEnemyParty[0], reward->params[0], reward->params[1], USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);

                GiveMonToPlayer(&gEnemyParty[0]);
                shouldAnnounce = TRUE;
                break;

            //case QUEST_REWARD_CUSTOM_TEXT:
            //    break;
        }
        
        if(shouldAnnounce)
        {
            return TRUE;
        }
    }

    return FALSE;
}


bool8 GiveNextRewardAndFormat(u8* str, u8* type)
{
    if(GiveAndGetNextAnnouncedReward())
    {
        struct RogueQuestReward const* reward = GetCurrentRewardTarget();
        *type = reward->type;

        if(reward->giveText)
        {
            StringCopy(str, reward->giveText);
        }
        else
        {
            switch(reward->type)
            {
                case QUEST_REWARD_GIVE_ITEM:
                    CopyItemNameHandlePlural(reward->params[0], gStringVar1, reward->params[1]);
                    StringExpandPlaceholders(str, gText_QuestRewardGive);
                    break;

                case QUEST_REWARD_GIVE_MONEY:
                    ConvertUIntToDecimalStringN(gStringVar1, reward->params[0], STR_CONV_MODE_LEFT_ALIGN, 7);
                    StringExpandPlaceholders(str, gText_QuestRewardGiveMoney);
                    break;

                case QUEST_REWARD_GIVE_POKEMON:
                    StringCopy(gStringVar1, gSpeciesNames[reward->params[0]]);

                    if(reward->params[2] == TRUE)
                        StringExpandPlaceholders(str, gText_QuestRewardGiveShinyMon);
                    else
                        StringExpandPlaceholders(str, gText_QuestRewardGiveMon);
                    break;
                
                default:
                    // Just return an obviously broken message
                    StringCopy(str, gText_QuestLogStatusIncomplete);
                    break;
            }
        }
        return TRUE;
    }

    return FALSE;
}

bool8 TryUnlockQuest(u16 questId)
{
    struct RogueQuestState* state = &gRogueGlobalData.questStates[questId];

    if(!state->isUnlocked)
    {
        state->isUnlocked = TRUE;
        state->isValid = FALSE;
        state->isCompleted = FALSE;
        state->hasNewMarker = TRUE;

        if(questId == QUEST_Collector1)
        {
            // Instant complete
            u16 caughtCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
            if(caughtCount >= 15)
            {
                state->isValid = TRUE;
                TryMarkQuestAsComplete(QUEST_Collector1);
            }
        }

        if(questId == QUEST_Collector2)
        {
            // Instant complete
            u16 caughtCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
            if(caughtCount >= 100)
            {
                state->isValid = TRUE;
                TryMarkQuestAsComplete(QUEST_Collector2);
            }
        }

        return TRUE;
    }

    return FALSE;
}

bool8 TryMarkQuestAsComplete(u16 questId)
{
    struct RogueQuestState* state = &gRogueGlobalData.questStates[questId];

    if(state->isValid)
    {
        state->isValid = FALSE;

        if(!state->isCompleted)
        {
            // First time finishing
            state->isCompleted = TRUE;
            state->hasPendingRewards = TRUE;

            FlagSet(FLAG_ROGUE_QUESTS_ASK_FOR_RETIRE);
        }
        else if(IsQuestRepeatable(questId))
        {
            // Has already completed this once before so has rewards pending
            state->hasPendingRewards = TRUE;
        }

        Rogue_PushPopup(POPUP_MSG_QUEST_COMPLETE, questId);
        return TRUE;
    }

    return FALSE;
}

bool8 TryDeactivateQuest(u16 questId)
{
    struct RogueQuestState* state = &gRogueGlobalData.questStates[questId];

    if(state->isValid)
    {
        state->isValid = FALSE;

        if(state->isPinned)
            Rogue_PushPopup(POPUP_MSG_QUEST_FAIL, questId);

        return TRUE;
    }

    return FALSE;
}

void UnlockFollowingQuests(u16 questId)
{
    u8 i;

    for(i = 0; i < QUEST_MAX_FOLLOWING_QUESTS; ++i)
    {
        if(gRogueQuests[questId].unlockedQuests[i] == QUEST_NONE)
            break;

        TryUnlockQuest(gRogueQuests[questId].unlockedQuests[i]);
    }
}

static void ForEachUnlockedQuest(QuestCallback callback)
{
    u16 i;
    struct RogueQuestState* state;

    for(i = QUEST_NONE + 1; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isUnlocked)
        {
            callback(i, state);
        }
    }
}

static void ForEachActiveQuest(QuestCallback callback)
{
    u16 i;
    struct RogueQuestState* state;

    for(i = QUEST_NONE + 1; i < QUEST_CAPACITY; ++i)
    {
        state = &gRogueGlobalData.questStates[i];
        if(state->isValid && !state->isCompleted)
        {
            callback(i, state);
        }
    }
}

static void TryActivateQuestInternal(u16 questId, struct RogueQuestState* state)
{
    if(state->isUnlocked)
    {
        if(IsQuestRepeatable(questId))
        {
            // Don't reactivate quests if we have rewards pending
            if(!state->hasPendingRewards)
                state->isValid = TRUE;
        }
        else if(!state->isCompleted)
        {
            state->isValid = TRUE;
        }
    }
}


static void ActivateAdventureQuests(u16 questId, struct RogueQuestState* state)
{
    bool8 activeInHub = (gRogueQuests[questId].flags & QUEST_FLAGS_ACTIVE_IN_HUB) != 0;

    if(!activeInHub || IsQuestGloballyTracked(questId))
    {
        TryActivateQuestInternal(questId, state);
    }
    else
    {
        state->isValid = FALSE;
    }
}

static void ActivateGauntletAdventureQuests(u16 questId, struct RogueQuestState* state)
{
    // The quests we allow for gauntlet mode
    switch(questId)
    {
        case QUEST_FirstAdventure:
        case QUEST_GymChallenge:
        case QUEST_GymMaster:
        case QUEST_EliteMaster:
        case QUEST_Champion:
        case QUEST_GauntletMode:
            ActivateAdventureQuests(questId, state);
            break;

        default:
            state->isValid = FALSE;
            break;
    }
}

static void ActivateHubQuests(u16 questId, struct RogueQuestState* state)
{
    bool8 activeInHub = (gRogueQuests[questId].flags & QUEST_FLAGS_ACTIVE_IN_HUB) != 0;

    if(activeInHub || IsQuestGloballyTracked(questId))
    {
        TryActivateQuestInternal(questId, state);
    }
    else
    {
        state->isValid = FALSE;
    }
}

// External callbacks
//

static void UpdateChaosChampion(bool8 enteringPotentialEncounter)
{
    struct RogueQuestState state;

    if(IsQuestActive(QUEST_ChaosChampion) && GetQuestState(QUEST_ChaosChampion, &state))
    {
        bool8 isRandomanDisabled = FlagGet(FLAG_ROGUE_RANDOM_TRADE_DISABLED);

        if(enteringPotentialEncounter)
        {
            state.data.half = isRandomanDisabled ? 0 : 1;
            SetQuestState(QUEST_ChaosChampion, &state);
        }
        else if(state.data.half)
        {
            if(!isRandomanDisabled)
            {
                // Randoman was still active i.e. we didn't use him
                TryDeactivateQuest(QUEST_ChaosChampion);
            }
            else
            {
                state.data.half = 0;
                SetQuestState(QUEST_ChaosChampion, &state);
            }
        }
    }

    // Check for random starter
    {
        // Only care about the very first check
        if(VarGet(VAR_ROGUE_CURRENT_ROOM_IDX) == 0)
        {
            bool8 isRandomanDisabled = FlagGet(FLAG_ROGUE_RANDOM_TRADE_DISABLED);

            if(!enteringPotentialEncounter && !isRandomanDisabled)
            {
                TryDeactivateQuest(QUEST_Nuzlocke);
                TryDeactivateQuest(QUEST_IronMono2);
                TryDeactivateQuest(QUEST_Hardcore4);
            }
        }
    }
}

static void CheckCurseQuests(void)
{
    u16 i;

    // Check for all curse items
    for(i = FIRST_ITEM_CURSE; i != LAST_ITEM_CURSE + 1; ++i)
    {
        if(!CheckBagHasItem(i, 1))
        {
            TryDeactivateQuest(QUEST_CursedBody);
            break;
        }
    }

    if(!CheckBagHasItem(ITEM_PARTY_CURSE, 5))
    {
        TryDeactivateQuest(QUEST_IronMono1);
        TryDeactivateQuest(QUEST_IronMono2);
    }

    if(!CheckBagHasItem(ITEM_WILD_ENCOUNTER_CURSE, 10))
    {
        TryDeactivateQuest(QUEST_Nuzlocke);
    }

    if(!CheckBagHasItem(ITEM_SHOP_PRICE_CURSE, 99))
    {
        TryDeactivateQuest(QUEST_IronMono2);
    }

    if(!CheckBagHasItem(ITEM_BATTLE_ITEM_CURSE, 1))
    {
        TryDeactivateQuest(QUEST_Nuzlocke);
        TryDeactivateQuest(QUEST_IronMono2);
        TryDeactivateQuest(QUEST_Hardcore);
        TryDeactivateQuest(QUEST_Hardcore2);
        TryDeactivateQuest(QUEST_Hardcore3);
        TryDeactivateQuest(QUEST_Hardcore4);
    }

    if(!CheckBagHasItem(ITEM_SPECIES_CLAUSE_CURSE, 1))
    {
        TryDeactivateQuest(QUEST_Nuzlocke);
        TryDeactivateQuest(QUEST_Hardcore3);
        TryDeactivateQuest(QUEST_Hardcore4);
    }
}

void QuestNotify_BeginAdventure(void)
{
    FlagClear(FLAG_ROGUE_QUESTS_ASK_FOR_RETIRE);
    FlagClear(FLAG_ROGUE_QUESTS_NEVER_ASK_FOR_RETIRE);

    sPreviousRouteType = 0;

    // Cannot activate quests on Gauntlet mode
    if(FlagGet(FLAG_ROGUE_GAUNTLET_MODE))
    {
        ForEachUnlockedQuest(ActivateGauntletAdventureQuests);
    }
    else
    {
        ForEachUnlockedQuest(ActivateAdventureQuests);
    }

    // Handle skip difficulty
    if(gRogueRun.currentDifficulty > 0)
    {
        u16 i;

        TryDeactivateQuest(QUEST_GymChallenge);
        TryDeactivateQuest(QUEST_GymMaster);
        TryDeactivateQuest(QUEST_NoFainting2);
        TryDeactivateQuest(QUEST_NoFainting3);
        TryDeactivateQuest(QUEST_Hardcore);
        TryDeactivateQuest(QUEST_Hardcore2);
        TryDeactivateQuest(QUEST_Hardcore3);
        TryDeactivateQuest(QUEST_Hardcore4);
        TryDeactivateQuest(QUEST_CursedBody);
        TryDeactivateQuest(QUEST_Nuzlocke);
        TryDeactivateQuest(QUEST_IronMono1);
        TryDeactivateQuest(QUEST_IronMono2);
        TryDeactivateQuest(QUEST_LegendOnly);

        TryDeactivateQuest(QUEST_KantoMode);
        TryDeactivateQuest(QUEST_JohtoMode);
        TryDeactivateQuest(QUEST_HoennMode);
        TryDeactivateQuest(QUEST_GlitchMode);
#ifdef ROGUE_EXPANSION
        TryDeactivateQuest(QUEST_SinnohMode);
        TryDeactivateQuest(QUEST_UnovaMode);
        TryDeactivateQuest(QUEST_KalosMode);
        TryDeactivateQuest(QUEST_AlolaMode);
        TryDeactivateQuest(QUEST_GalarMode);
#endif

        for(i = TYPE_NORMAL; i < NUMBER_OF_MON_TYPES; ++i)
            TryDeactivateQuest(TypeToMonoQuest[i]);
    }

    if(gRogueRun.currentDifficulty > 4)
    {
        TryDeactivateQuest(QUEST_NoFainting1);
    }

    if(gRogueRun.currentDifficulty > 8)
    {
        // Can't technically happen atm
        TryDeactivateQuest(QUEST_EliteMaster);
    }

    if(!FlagGet(FLAG_ROGUE_HARD_TRAINERS))
    {
        TryDeactivateQuest(QUEST_Hardcore2);
        TryDeactivateQuest(QUEST_Hardcore3);
        TryDeactivateQuest(QUEST_Hardcore4);
    }

    if(!FlagGet(FLAG_ROGUE_FORCE_BASIC_BAG))
    {
        TryDeactivateQuest(QUEST_Nuzlocke);
        TryDeactivateQuest(QUEST_IronMono2);
        TryDeactivateQuest(QUEST_Hardcore4);
    }

    if(!FlagGet(FLAG_ROGUE_HARD_ITEMS) || FlagGet(FLAG_ROGUE_EV_GAIN_ENABLED) || FlagGet(FLAG_ROGUE_CAN_OVERLVL))
    {
        TryDeactivateQuest(QUEST_Hardcore4);
    }

    if(!FlagGet(FLAG_ROGUE_DOUBLE_BATTLES))
    {
        TryDeactivateQuest(QUEST_OrreMode);
    }

    if(!FlagGet(FLAG_ROGUE_GAUNTLET_MODE))
    {
        TryDeactivateQuest(QUEST_GauntletMode);
    }

    {
        bool8 rainbowMode = FlagGet(FLAG_ROGUE_RAINBOW_MODE);
        u16 dexLimit = VarGet(VAR_ROGUE_REGION_DEX_LIMIT);
        u16 genLimit = VarGet(VAR_ROGUE_ENABLED_GEN_LIMIT);

        bool8 kantoBosses = FlagGet(FLAG_ROGUE_KANTO_BOSSES);
        bool8 johtoBosses = FlagGet(FLAG_ROGUE_JOHTO_BOSSES);
        bool8 hoennBosses = FlagGet(FLAG_ROGUE_HOENN_BOSSES);

        bool8 justKantoBosses = kantoBosses && !johtoBosses && !hoennBosses;
        bool8 justJohtoBosses = !kantoBosses && johtoBosses && !hoennBosses;
        bool8 glitchBosses = !kantoBosses && !johtoBosses && !hoennBosses;

        // Equiv to dex limit
        if(dexLimit == 0 && genLimit == 1)
            dexLimit = 1;

        if(rainbowMode || dexLimit != 1 || !justKantoBosses)
            TryDeactivateQuest(QUEST_KantoMode);

        if(rainbowMode || dexLimit != 2 || !justJohtoBosses)
            TryDeactivateQuest(QUEST_JohtoMode);

        if(!rainbowMode || dexLimit != 3)
            TryDeactivateQuest(QUEST_HoennMode);

#ifdef ROGUE_EXPANSION
        if(rainbowMode || dexLimit != 0 || genLimit != 8 || !glitchBosses)
#else
        if(rainbowMode || dexLimit != 0 || genLimit != 3 || !glitchBosses)
#endif
            TryDeactivateQuest(QUEST_GlitchMode);

#ifdef ROGUE_EXPANSION
        if(!rainbowMode || dexLimit != 4)
            TryDeactivateQuest(QUEST_SinnohMode);

        if(!rainbowMode || dexLimit != 5)
            TryDeactivateQuest(QUEST_UnovaMode);

        if(!rainbowMode || dexLimit != 6)
            TryDeactivateQuest(QUEST_KalosMode);

        if(!rainbowMode || dexLimit != 7)
            TryDeactivateQuest(QUEST_AlolaMode);

        if(!rainbowMode || dexLimit != 8)
            TryDeactivateQuest(QUEST_GalarMode);
#endif
    }

    UpdateChaosChampion(TRUE);
    CheckCurseQuests();
}

static void OnStartBattle(void)
{
    UpdateMonoQuests();
}

static void OnEndBattle(void)
{
    struct RogueQuestState state;

    if(IsQuestActive(QUEST_Collector1))
    {
        u16 caughtCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        if(caughtCount >= 15)
            TryMarkQuestAsComplete(QUEST_Collector1);
    }

    if(IsQuestActive(QUEST_Collector2))
    {
        u16 caughtCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        if(caughtCount >= 100)
            TryMarkQuestAsComplete(QUEST_Collector2);
    }

    if(IsQuestActive(QUEST_NoFainting1) && GetQuestState(QUEST_NoFainting1, &state))
    {
        if(Rogue_IsPartnerMonInTeam() == FALSE)
        {
            state.isValid = FALSE;
            SetQuestState(QUEST_NoFainting1, &state);
        }
    }

    if(IsQuestActive(QUEST_Hardcore3) || IsQuestActive(QUEST_Hardcore4) || IsQuestActive(QUEST_LegendOnly))
    {
        u16 i;

        for(i = 0; i < PARTY_SIZE; ++i)
        {
            u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
            if(species != SPECIES_NONE)
            {
                if(IsSpeciesLegendary(species))
                {
                    TryDeactivateQuest(QUEST_Hardcore3);
                    TryDeactivateQuest(QUEST_Hardcore4);
                }
                else
                {
                    TryDeactivateQuest(QUEST_LegendOnly);
                }
            }
        }
    }
}

void QuestNotify_EndAdventure(void)
{
    TryMarkQuestAsComplete(QUEST_FirstAdventure);

    ForEachUnlockedQuest(ActivateHubQuests);
}

void QuestNotify_OnWildBattleEnd(void)
{
    if(gBattleOutcome == B_OUTCOME_CAUGHT)
    {
        if(IsQuestActive(QUEST_DenExplorer) && gRogueAdvPath.currentRoomType == ADVPATH_ROOM_WILD_DEN)
            TryMarkQuestAsComplete(QUEST_DenExplorer);
    }
    //

    OnEndBattle();
}

static void UpdateMonoQuests(void)
{
    u16 type;
    u16 questId;
    u8 i;

    for(type = TYPE_NORMAL; type < NUMBER_OF_MON_TYPES; ++type)
    {
        questId = TypeToMonoQuest[type];

        if(IsQuestActive(questId))
        {
            for(i = 0; i < gPlayerPartyCount; ++i)
            {
                u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
                if(species != SPECIES_NONE && !IsSpeciesType(species, type))
                {
                    TryDeactivateQuest(questId);
                    break;
                }
            }
        }
    }
}

static void CompleteMonoQuests(void)
{
    u16 type;
    u16 questId;
    u8 i;

    for(type = TYPE_NORMAL; type < NUMBER_OF_MON_TYPES; ++type)
    {
        questId = TypeToMonoQuest[type];

        if(IsQuestActive(questId))
            TryMarkQuestAsComplete(questId);
    }
}

void QuestNotify_OnTrainerBattleEnd(bool8 isBossTrainer)
{
    u8 i;

    if(isBossTrainer)
    {
        u16 relativeDifficulty = gRogueRun.currentDifficulty - VarGet(VAR_ROGUE_SKIP_TO_DIFFICULTY);

        switch(gRogueRun.currentDifficulty)
        {
            case 1:
                TryMarkQuestAsComplete(QUEST_Gym1);
                break;
            case 2:
                TryMarkQuestAsComplete(QUEST_Gym2);
                break;
            case 3:
                TryMarkQuestAsComplete(QUEST_Gym3);
                break;
            case 4:
                TryMarkQuestAsComplete(QUEST_Gym4);
                break;
            case 5:
                TryMarkQuestAsComplete(QUEST_Gym5);
                break;
            case 6:
                TryMarkQuestAsComplete(QUEST_Gym6);
                break;
            case 7:
                TryMarkQuestAsComplete(QUEST_Gym7);
                break;
            case 8: // Just beat last Gym
                TryMarkQuestAsComplete(QUEST_Gym8);
                TryMarkQuestAsComplete(QUEST_NoFainting2);
                break;

            case 12: // Just beat last E4
                if(IsQuestActive(QUEST_CollectorLegend))
                {
                    for(i = 0; i < PARTY_SIZE; ++i)
                    {
                        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
                        if(IsSpeciesLegendary(species))
                        {
                            TryMarkQuestAsComplete(QUEST_CollectorLegend);
                            break;
                        }
                    }
                }
                break;

            case 14: // Just beat final champ
                TryMarkQuestAsComplete(QUEST_Champion);
                TryMarkQuestAsComplete(QUEST_NoFainting3);
                TryMarkQuestAsComplete(QUEST_ChaosChampion);
                TryMarkQuestAsComplete(QUEST_Hardcore);
                TryMarkQuestAsComplete(QUEST_Hardcore2);
                TryMarkQuestAsComplete(QUEST_Hardcore3);
                TryMarkQuestAsComplete(QUEST_Hardcore4);
                TryMarkQuestAsComplete(QUEST_CursedBody);
                TryMarkQuestAsComplete(QUEST_Nuzlocke);
                TryMarkQuestAsComplete(QUEST_IronMono1);
                TryMarkQuestAsComplete(QUEST_IronMono2);
                TryMarkQuestAsComplete(QUEST_LegendOnly);
                TryMarkQuestAsComplete(QUEST_GauntletMode);

                TryMarkQuestAsComplete(QUEST_KantoMode);
                TryMarkQuestAsComplete(QUEST_JohtoMode);
                TryMarkQuestAsComplete(QUEST_HoennMode);
                TryMarkQuestAsComplete(QUEST_GlitchMode);
#ifdef ROGUE_EXPANSION
                TryMarkQuestAsComplete(QUEST_SinnohMode);
                TryMarkQuestAsComplete(QUEST_UnovaMode);
                TryMarkQuestAsComplete(QUEST_KalosMode);
                TryMarkQuestAsComplete(QUEST_AlolaMode);
                TryMarkQuestAsComplete(QUEST_GalarMode);
#endif

                CompleteMonoQuests();
                break;
        }

        if(gRogueRun.currentDifficulty >= 4)
            TryMarkQuestAsComplete(QUEST_GymChallenge);

        if(gRogueRun.currentDifficulty >= 8)
            TryMarkQuestAsComplete(QUEST_GymMaster);

        if(gRogueRun.currentDifficulty >= 12)
            TryMarkQuestAsComplete(QUEST_EliteMaster);

        if(relativeDifficulty == 4)
        {
            if(IsQuestActive(QUEST_NoFainting1))
                TryMarkQuestAsComplete(QUEST_NoFainting1);
        }
    }
    
    OnEndBattle();
}

void QuestNotify_OnMonFainted()
{
    TryDeactivateQuest(QUEST_NoFainting2);
    TryDeactivateQuest(QUEST_NoFainting3);

    if(IsQuestActive(QUEST_WobFate))
    {
        bool8 isTrainerBattle = (gBattleTypeFlags & BATTLE_TYPE_TRAINER) != 0;
        if(!isTrainerBattle)
        {
            u16 species = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker ^ BIT_SIDE]], MON_DATA_SPECIES);

            if(species == SPECIES_WOBBUFFET)
                TryMarkQuestAsComplete(QUEST_WobFate);
        }
    }
}

void QuestNotify_OnExitHubTransition(void)
{
    // Could've traded by event so update mono quests
    UpdateMonoQuests();
}

void QuestNotify_OnWarp(struct WarpData* warp)
{
    if(Rogue_IsRunActive())
    {
        struct RogueQuestState state;

        // Warped into
        switch(gRogueAdvPath.currentRoomType)
        {
            case ADVPATH_ROOM_ROUTE:
                if(gRogueAdvPath.currentRoomParams.perType.route.difficulty == 2)
                {
                    if(IsQuestActive(QUEST_Bike1) && GetQuestState(QUEST_Bike1, &state))
                    {
                        state.data.byte[0] = gSaveBlock2Ptr->playTimeHours;
                        state.data.byte[1] = gSaveBlock2Ptr->playTimeMinutes;
                        SetQuestState(QUEST_Bike1, &state);
                    }

                    if(gRogueRun.currentDifficulty >= 8)
                    {
                        if(IsQuestActive(QUEST_Bike2) && GetQuestState(QUEST_Bike2, &state))
                        {
                            state.data.byte[0] = gSaveBlock2Ptr->playTimeHours;
                            state.data.byte[1] = gSaveBlock2Ptr->playTimeMinutes;
                            SetQuestState(QUEST_Bike2, &state);
                        }
                    }
                }
                break;

            case ADVPATH_ROOM_RESTSTOP:
                UpdateChaosChampion(TRUE);
                break;

            case ADVPATH_ROOM_BOSS:
                // About to face final champ
                if(gRogueRun.currentDifficulty == 13)
                {
                    if(IsQuestActive(QUEST_OrreMode) 
                    && PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_ESPEON)
                    && PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_UMBREON)
                    )
                        TryMarkQuestAsComplete(QUEST_OrreMode);

#ifdef ROGUE_EXPANSION
                    if(IsQuestActive(QUEST_ShayminItem) && PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_SHAYMIN))
                        TryMarkQuestAsComplete(QUEST_ShayminItem);

                    if(IsQuestActive(QUEST_HoopaItem) && PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_HOOPA))
                        TryMarkQuestAsComplete(QUEST_HoopaItem);

                    if(IsQuestActive(QUEST_NatureItem))
                    {
                        if(PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_TORNADUS)
                        || PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_THUNDURUS)
                        || PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_LANDORUS)
                        )
                            TryMarkQuestAsComplete(QUEST_NatureItem);
                    }
    
                    if(IsQuestActive(QUEST_DeoxysItem) && PartyContainsSpecies(gPlayerParty, gPlayerPartyCount, SPECIES_DEOXYS))
                        TryMarkQuestAsComplete(QUEST_DeoxysItem);
#endif
                }

                break;
        }

        // Warped out of
        switch(sPreviousRouteType)
        {
            case ADVPATH_ROOM_ROUTE:
                if(gRogueAdvPath.currentRoomParams.perType.route.difficulty == 2)
                {
                    if(IsQuestActive(QUEST_Bike1) && GetQuestState(QUEST_Bike1, &state))
                    {
                        u16 startTime = ((u16)state.data.byte[0]) * 60 + ((u16)state.data.byte[1]);
                        u16 exitTime = ((u16)gSaveBlock2Ptr->playTimeHours) * 60 + ((u16)gSaveBlock2Ptr->playTimeMinutes);

                        if((exitTime - startTime) < 120)
                            TryMarkQuestAsComplete(QUEST_Bike1);
                    }

                    if(gRogueRun.currentDifficulty >= 8)
                    {
                        if(IsQuestActive(QUEST_Bike2) && GetQuestState(QUEST_Bike2, &state))
                        {
                            u16 startTime = ((u16)state.data.byte[0]) * 60 + ((u16)state.data.byte[1]);
                            u16 exitTime = ((u16)gSaveBlock2Ptr->playTimeHours) * 60 + ((u16)gSaveBlock2Ptr->playTimeMinutes);

                            if((exitTime - startTime) < 60)
                                TryMarkQuestAsComplete(QUEST_Bike2);
                        }
                    }
                }
                break;

            case ADVPATH_ROOM_RESTSTOP:
                if(IsQuestActive(QUEST_BigSaver) && GetMoney(&gSaveBlock1Ptr->money) >= 50000)
                    TryMarkQuestAsComplete(QUEST_BigSaver);
                break;
        }

        if(gRogueAdvPath.currentRoomType != ADVPATH_ROOM_RESTSTOP)
        {
            UpdateChaosChampion(FALSE);
        }

        sPreviousRouteType = gRogueAdvPath.currentRoomType;
    }
}

void QuestNotify_OnAddMoney(u32 amount)
{

}

void QuestNotify_OnRemoveMoney(u32 amount)
{
    if(Rogue_IsRunActive())
    {
        struct RogueQuestState state;

        if(gRogueAdvPath.currentRoomType == ADVPATH_ROOM_RESTSTOP)
        {
            if(IsQuestActive(QUEST_ShoppingSpree) && GetQuestState(QUEST_ShoppingSpree, &state))
            {
                state.data.half += amount;
                SetQuestState(QUEST_ShoppingSpree, &state);

                if(state.data.half >= 20000)
                    TryMarkQuestAsComplete(QUEST_ShoppingSpree);
            }
        }
    }
}

void QuestNotify_OnAddBagItem(u16 itemId, u16 count)
{
    if(IsQuestActive(QUEST_BerryCollector))
    {
        if(itemId >= FIRST_BERRY_INDEX && itemId <= LAST_BERRY_INDEX)
        {
            u16 i;
            u16 uniqueBerryCount = 0;

            for(i = FIRST_BERRY_INDEX; i <= LAST_BERRY_INDEX; ++i)
            {
                if(CheckBagHasItem(i,1))
                    ++uniqueBerryCount;
            }

            if(uniqueBerryCount >= 10)
                TryMarkQuestAsComplete(QUEST_BerryCollector);
        }

    }
}

void QuestNotify_OnRemoveBagItem(u16 itemId, u16 count)
{

}

void QuestNotify_OnUseBattleItem(u16 itemId)
{
    //bool8 isPokeball = itemId >= FIRST_BALL && itemId <= LAST_BALL;
    //if(!isPokeball)
    //{
    //    if(IsQuestActive(QUEST_Hardcore))
    //        TryDeactivateQuest(QUEST_Hardcore);
//
    //    if(IsQuestActive(QUEST_Hardcore2))
    //        TryDeactivateQuest(QUEST_Hardcore2);
//
    //    if(IsQuestActive(QUEST_Hardcore3))
    //        TryDeactivateQuest(QUEST_Hardcore3);
//
    //    if(IsQuestActive(QUEST_Hardcore4))
    //        TryDeactivateQuest(QUEST_Hardcore4);
    //}
}

void QuestNotify_OnMegaEvolve(u16 species)
{
#ifdef ROGUE_EXPANSION
    if(gRogueRun.currentDifficulty >= 13)
    {
        if(IsQuestActive(QUEST_MegaEvo))
            TryMarkQuestAsComplete(QUEST_MegaEvo);
    }
#endif
}

void QuestNotify_OnZMoveUsed(u16 move)
{
#ifdef ROGUE_EXPANSION
    if(gRogueRun.currentDifficulty >= 13)
    {
        if(IsQuestActive(QUEST_ZMove))
            TryMarkQuestAsComplete(QUEST_ZMove);
    }
#endif
}

void QuestNotify_StatIncrement(u8 statIndex)
{
    switch (statIndex)
    {
    case GAME_STAT_TOTAL_BATTLES:
        OnStartBattle();
        break;
    } 
}