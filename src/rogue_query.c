#include "global.h"
#include "constants/abilities.h"
//#include "constants/heal_locations.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
//#include "constants/layouts.h"
//#include "constants/rogue.h"
#include "data.h"
//
//#include "battle.h"
//#include "battle_setup.h"
#include "event_data.h"
#include "item.h"
#include "item_use.h"
//#include "money.h"
//#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
//#include "random.h"
//#include "strings.h"
//#include "string_util.h"
//#include "text.h"

#include "rogue_query.h"
#include "rogue_baked.h"
#include "rogue_campaign.h"
#include "rogue_controller.h"

#define QUERY_BUFFER_COUNT 128
#define QUERY_NUM_SPECIES NUM_SPECIES

#define MAX_QUERY_BIT_COUNT (max(ITEMS_COUNT, QUERY_NUM_SPECIES))

EWRAM_DATA u16 gRogueQueryBufferSize = 0;
EWRAM_DATA u8 gRogueQueryBits[1 + MAX_QUERY_BIT_COUNT / 8];
EWRAM_DATA u16 gRogueQueryBuffer[QUERY_BUFFER_COUNT];

extern const u16* const gRegionalDexSpecies[];
extern u16 gRegionalDexSpeciesCount[];
//extern struct Evolution gEvolutionTable[][EVOS_PER_MON];

static void SetQueryState(u16 elem, bool8 state)
{
    u16 idx = elem / 8;
    u16 bit = elem % 8;

    u8 bitMask = 1 << bit;
    
    AGB_ASSERT(idx < ARRAY_COUNT(gRogueQueryBits));
    if(state)
    {
        gRogueQueryBits[idx] |= bitMask;
    }
    else
    {
        gRogueQueryBits[idx] &= ~bitMask;
    }
}

static bool8 GetQueryState(u16 elem)
{
    u16 idx = elem / 8;
    u16 bit = elem % 8;

    u8 bitMask = 1 << bit;

    AGB_ASSERT(idx < ARRAY_COUNT(gRogueQueryBits));
    return gRogueQueryBits[idx] & bitMask;
}

void RogueQuery_Clear(void)
{
    gRogueQueryBufferSize = 0;
    memset(&gRogueQueryBits[0], 255, sizeof(u8) * ARRAY_COUNT(gRogueQueryBits));
}

void RogueQuery_CollapseSpeciesBuffer(void)
{
    u16 species;
    gRogueQueryBufferSize = 0;
    
    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES && gRogueQueryBufferSize < (QUERY_BUFFER_COUNT - 1); ++species)
    {
        if(GetQueryState(species))
        {
            gRogueQueryBuffer[gRogueQueryBufferSize++] = species;
        }
    }

    if(gRogueQueryBufferSize < QUERY_BUFFER_COUNT)
        gRogueQueryBuffer[gRogueQueryBufferSize] = 0;
}

void RogueQuery_CollapseItemBuffer(void)
{
    u16 itemId;
    gRogueQueryBufferSize = 0;
    
    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT && gRogueQueryBufferSize < (QUERY_BUFFER_COUNT - 1); ++itemId)
    {
        if(GetQueryState(itemId))
        {
            gRogueQueryBuffer[gRogueQueryBufferSize++] = itemId;
        }
    }

    if(gRogueQueryBufferSize < QUERY_BUFFER_COUNT)
        gRogueQueryBuffer[gRogueQueryBufferSize] = 0;
}

u16* RogueQuery_BufferPtr(void)
{
    return &gRogueQueryBuffer[0];
}

u16 RogueQuery_BufferSize(void)
{
    return gRogueQueryBufferSize;
}

bool8 RogueQuery_CheckIncluded(u16 id)
{
    return GetQueryState(id);
}

u16 RogueQuery_UncollapsedSpeciesSize(void)
{
    u16 species;
    u16 count = 0;
    
    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            ++count;
        }
    }

    return count;
}

u16 RogueQuery_UncollapsedItemSize(void)
{
    u16 itemId;
    u16 count = 0;
    
    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            ++count;
        }
    }

    return count;
}

u16 RogueQuery_AtUncollapsedIndex(u16 idx)
{
    u16 i;
    u16 counter = 0;
    
    for(i = 1; i < MAX_QUERY_BIT_COUNT; ++i)
    {
        if(GetQueryState(i))
        {
            if(idx == counter++)
                return i;
        }
    }

    return 0;
}

u16 RogueQuery_PopCollapsedIndex(u16 idx)
{
    u16 i;
    u16 value = gRogueQueryBuffer[idx];

    AGB_ASSERT(idx < ARRAY_COUNT(gRogueQueryBuffer));

    SetQueryState(value, FALSE);
    --gRogueQueryBufferSize;

    for(i = idx; i < gRogueQueryBufferSize; ++i)
    {
        AGB_ASSERT(i + 1 < ARRAY_COUNT(gRogueQueryBuffer));
        gRogueQueryBuffer[i] = gRogueQueryBuffer[i + 1];
    }

    gRogueQueryBuffer[gRogueQueryBufferSize] = 0;
    return value;
}

u16 RogueQuery_PopUncollapsedIndex(u16 idx)
{
    u16 i;
    u16 counter = 0;
    
    for(i = 1; i < MAX_QUERY_BIT_COUNT; ++i)
    {
        if(GetQueryState(i))
        {
            if(idx == counter++)
            {
                SetQueryState(i, FALSE);
                return i;
            }
        }
    }

    return 0;
}

void RogueQuery_Include(u16 idx)
{
    SetQueryState(idx, TRUE);
}

void RogueQuery_IncludeRange(u16 fromId, u16 toId)
{
    u16 i;
    for(i = fromId; i <= toId; ++i)
    {
        SetQueryState(i, TRUE);
    }
}

void RogueQuery_Exclude(u16 idx)
{
    SetQueryState(idx, FALSE);
}

void RogueQuery_ExcludeAll(void)
{
    memset(&gRogueQueryBits[0], 0, sizeof(u8) * ARRAY_COUNT(gRogueQueryBits));
}

void RogueQuery_CustomSpecies(QueryCallback query, u16 usrData)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(query(species, usrData) == FALSE)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_CustomItems(QueryCallback query, u16 usrData)
{
    u16 itemId;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            if(query(itemId, usrData) == FALSE)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

// Species
//

bool8 IsSpeciesType(u16 species, u8 type)
{
    return gBaseStats[species].type1 == type || gBaseStats[species].type2 == type;
}

static bool8 IsFinalEvolution(u16 species)
{
    u16 s, e;
    struct Evolution evo;

    for (e = 0; e < EVOS_PER_MON; e++)
    {
        Rogue_ModifyEvolution(species, e, &evo);

        if (evo.targetSpecies != SPECIES_NONE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

bool8 IsSpeciesLegendary(u16 species)
{
#ifdef ROGUE_EXPANSION
    species = GET_BASE_SPECIES_ID(species);
#endif

    switch(species)
    {
        case SPECIES_ARTICUNO:
        case SPECIES_ZAPDOS:
        case SPECIES_MOLTRES:
        case SPECIES_MEWTWO:
        case SPECIES_MEW:

        case SPECIES_RAIKOU:
        case SPECIES_ENTEI:
        case SPECIES_SUICUNE:
        case SPECIES_LUGIA:
        case SPECIES_HO_OH:
        case SPECIES_CELEBI:

        case SPECIES_REGIROCK:
        case SPECIES_REGICE:
        case SPECIES_REGISTEEL:
        case SPECIES_KYOGRE:
        case SPECIES_GROUDON:
        case SPECIES_RAYQUAZA:
        case SPECIES_LATIAS:
        case SPECIES_LATIOS:
        case SPECIES_JIRACHI:
        case SPECIES_DEOXYS:
#ifdef ROGUE_EXPANSION

        case SPECIES_UXIE:
        case SPECIES_MESPRIT:
        case SPECIES_AZELF:
        case SPECIES_DIALGA:
        case SPECIES_PALKIA:
        case SPECIES_HEATRAN:
        case SPECIES_REGIGIGAS:
        case SPECIES_GIRATINA:
        case SPECIES_CRESSELIA:
        case SPECIES_PHIONE:
        case SPECIES_MANAPHY:
        case SPECIES_DARKRAI:
        case SPECIES_SHAYMIN:
        case SPECIES_ARCEUS:

        case SPECIES_VICTINI:
        case SPECIES_COBALION:
        case SPECIES_TERRAKION:
        case SPECIES_VIRIZION:
        case SPECIES_TORNADUS:
        case SPECIES_THUNDURUS:
        case SPECIES_RESHIRAM:
        case SPECIES_ZEKROM:
        case SPECIES_LANDORUS:
        case SPECIES_KYUREM:
        case SPECIES_KELDEO:
        case SPECIES_MELOETTA:
        case SPECIES_GENESECT:

        case SPECIES_XERNEAS:
        case SPECIES_YVELTAL:
        case SPECIES_ZYGARDE:
        case SPECIES_DIANCIE:
        case SPECIES_HOOPA:
        case SPECIES_VOLCANION:
        
        case SPECIES_TYPE_NULL:
        case SPECIES_SILVALLY:
        case SPECIES_TAPU_KOKO:
        case SPECIES_TAPU_LELE:
        case SPECIES_TAPU_BULU:
        case SPECIES_TAPU_FINI:
        case SPECIES_COSMOG:
        case SPECIES_COSMOEM:
        case SPECIES_SOLGALEO:
        case SPECIES_LUNALA:
        case SPECIES_NIHILEGO:
        case SPECIES_BUZZWOLE:
        case SPECIES_PHEROMOSA:
        case SPECIES_XURKITREE:
        case SPECIES_CELESTEELA:
        case SPECIES_KARTANA:
        case SPECIES_GUZZLORD:
        case SPECIES_NECROZMA:
        case SPECIES_MAGEARNA:
        case SPECIES_MARSHADOW:
        case SPECIES_POIPOLE:
        case SPECIES_NAGANADEL:
        case SPECIES_STAKATAKA:
        case SPECIES_BLACEPHALON:
        case SPECIES_ZERAORA:
        case SPECIES_MELTAN:
        case SPECIES_MELMETAL:

        case SPECIES_ZACIAN:
        case SPECIES_ZAMAZENTA:
        case SPECIES_ETERNATUS:
        case SPECIES_KUBFU:
        case SPECIES_URSHIFU:
        case SPECIES_ZARUDE:
        case SPECIES_REGIELEKI:
        case SPECIES_REGIDRAGO:
        case SPECIES_GLASTRIER:
        case SPECIES_SPECTRIER:
        case SPECIES_CALYREX:

        // Forms
        case SPECIES_KYUREM_WHITE:
        case SPECIES_KYUREM_BLACK:
        
        case SPECIES_NECROZMA_DUSK_MANE:
        case SPECIES_NECROZMA_DAWN_WINGS:
        case SPECIES_NECROZMA_ULTRA:

        case SPECIES_ZACIAN_CROWNED_SWORD:
        case SPECIES_ZAMAZENTA_CROWNED_SHIELD:
        case SPECIES_ETERNATUS_ETERNAMAX:
        case SPECIES_URSHIFU_RAPID_STRIKE_STYLE:
        case SPECIES_ZARUDE_DADA:
        case SPECIES_CALYREX_ICE_RIDER:
        case SPECIES_CALYREX_SHADOW_RIDER:

        case SPECIES_ARTICUNO_GALARIAN:
        case SPECIES_ZAPDOS_GALARIAN:
        case SPECIES_MOLTRES_GALARIAN:
#endif
            return TRUE;
    };

    return FALSE;
}

bool8 IsLegendaryEnabled(u16 species)
{
#ifdef ROGUE_EXPANSION
    u16 eggSpecies =  Rogue_GetEggSpecies(GET_BASE_SPECIES_ID(species));
#else
    u16 eggSpecies =  Rogue_GetEggSpecies(species);
#endif
    u16 dexLimit = VarGet(VAR_ROGUE_REGION_DEX_LIMIT);

    // Use a specific regional dex (Ignore previous state)
    if(dexLimit != 0)
    {
        u16 i;
        u16 checkSpecies;
        const u16 targetDex = dexLimit - 1;

        for(i = 0; i < gRegionalDexSpeciesCount[targetDex]; ++i)
        {
            checkSpecies = gRegionalDexSpecies[targetDex][i];

            if(eggSpecies == Rogue_GetEggSpecies(checkSpecies))
                return TRUE;
        }

        return FALSE;
    }
    // Using national mode gen limiter
    else
    {
        return IsGenEnabled(SpeciesToGen(species));
    }
}

static void TryApplyTypeEarlyCull(u8 type)
{
    if(type != TYPE_NONE)
    {
        u16 i;
        u16 species;
        const u16* typeTable = Rogue_GetSpeciesTypeTables(type);

        for(i = 0; ; ++i)
        {
            species = typeTable[i];

            if(species == SPECIES_NONE)
                break;

            RogueQuery_Include(species);
        }
    }
}

void RogueQuery_SpeciesIsValid(u8 earlyCullType1, u8 earlyCullType2, u8 earlyCullType3)
{
    // Handle for ?? species mainly
    // Just going to base this off ability 1 being none as that seems safest whilst allowing new mons
    u16 species;
    bool8 state;
    u16 dexLimit = VarGet(VAR_ROGUE_REGION_DEX_LIMIT);

    // No need to do this, as we're going to exclude everything below anyway
    if(dexLimit != 0) 
        return;

    if(earlyCullType1 != TYPE_NONE || earlyCullType2 != TYPE_NONE)
    {
        RogueQuery_ExcludeAll();
        TryApplyTypeEarlyCull(earlyCullType1);
        TryApplyTypeEarlyCull(earlyCullType2);
        TryApplyTypeEarlyCull(earlyCullType3);
    }

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(gBaseStats[species].abilities[0] == ABILITY_NONE || gBaseStats[species].catchRate == 0)
            {
                SetQueryState(species, FALSE);
            }
#ifdef ROGUE_EXPANSION
            else if(species > FORMS_START)
            {
                // Only validate certain forms here
                // (A lot of them are manual transform methods)
                state = FALSE;

                if(species >= SPECIES_RATTATA_ALOLAN && species <= SPECIES_STUNFISK_GALARIAN)
                {
                    state = TRUE;
                }
                else if(species >= SPECIES_BURMY_SANDY_CLOAK && species <= SPECIES_WORMADAM_TRASH_CLOAK)
                {
                    state = TRUE;
                }
                else if(species >= SPECIES_SHELLOS_EAST_SEA && species <= SPECIES_ROTOM_MOW)
                {
                    state = TRUE;
                }

                SetQueryState(species, state);
            }
#endif
        }
    }
}

void RogueQuery_SpeciesExcludeCommon(void)
{
    u16 species;
    u16 dexLimit = VarGet(VAR_ROGUE_REGION_DEX_LIMIT);
    u16 maxGen = VarGet(VAR_ROGUE_ENABLED_GEN_LIMIT);

    // Use a specific regional dex (Ignore previous state)
    if(dexLimit != 0)
    {
        u16 i;
        const u16 targetDex = dexLimit - 1;
        
        RogueQuery_ExcludeAll();

        for(i = 0; i < gRegionalDexSpeciesCount[targetDex]; ++i)
        {
            species = gRegionalDexSpecies[targetDex][i];
            SetQueryState(Rogue_GetEggSpecies(species), TRUE);
        }

        // Re-apply to remove any invalid mons
        RogueQuery_SpeciesIsValid(TYPE_NONE, TYPE_NONE, TYPE_NONE);
    }
    // Using national mode gen limiter
    else
    {
        for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
        {
            if(GetQueryState(species))
            {
                if(!IsGenEnabled(SpeciesToGen(species)))
                {
                    SetQueryState(species, FALSE);
                }
            }
        }
    }

#ifdef ROGUE_EXPANSION
    // We can only evolve into these forms, they will never spawn otherwise
    RogueQuery_SpeciesExcludeRange(SPECIES_ALCREMIE_RUBY_CREAM, SPECIES_ALCREMIE_RAINBOW_SWIRL);
#endif
}

void RogueQuery_SpeciesExcludeRange(u16 fromId, u16 toId)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(species >= fromId && species <= toId)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesInPokedex(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesInGeneration(u8 gen)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(SpeciesToGen(species) != gen)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesOfType(u8 type)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(!IsSpeciesType(species, type))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesNotOfType(u8 type)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(IsSpeciesType(species, type))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesOfTypes(const u8* types, u8 count)
{
    u8 t;
    bool8 isValid;
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            isValid = FALSE;
            for(t = 0; t < count; ++t)
            {
                if(types[t] == TYPE_NONE)
                    continue;

                if(IsSpeciesType(species, types[t]))
                {
                    isValid = TRUE;
                    break;
                }
            }

            if(!isValid)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesNotOfTypes(const u8* types, u8 count)
{
    u8 t;
    bool8 isValid;
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            isValid = FALSE;
            for(t = 0; t < count; ++t)
            {
                if(types[t] == TYPE_NONE)
                    continue;

                if(!IsSpeciesType(species, types[t]))
                {
                    isValid = TRUE;
                    break;
                }
            }

            if(!isValid)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesIsFinalEvolution(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(!IsFinalEvolution(species))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_TransformToEggSpecies(void)
{
    u16 species;
    u16 eggSpecies;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            eggSpecies = Rogue_GetEggSpecies(species);
            if(eggSpecies != species)
            {
                SetQueryState(eggSpecies, TRUE);
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesWithAtLeastEvolutionStages(u8 count)
{
    u8 evo;
    u16 species;
    bool8 removeChild = TRUE;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(Rogue_GetEvolutionCount(species) < count)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

static bool8 IsValidLevelEvo(struct Evolution* evo, u8 level)
{
    switch(evo->method)
    {
        case EVO_LEVEL:
        case EVO_LEVEL_ATK_GT_DEF:
        case EVO_LEVEL_ATK_EQ_DEF:
        case EVO_LEVEL_ATK_LT_DEF:
        case EVO_LEVEL_SILCOON:
        case EVO_LEVEL_CASCOON:
        case EVO_LEVEL_NINJASK:
        case EVO_LEVEL_SHEDINJA:
    #ifdef ROGUE_EXPANSION
        case EVO_LEVEL_FEMALE:
        case EVO_LEVEL_MALE:
        case EVO_LEVEL_DAY:
        case EVO_LEVEL_DUSK:
        case EVO_LEVEL_NATURE_AMPED:
        case EVO_LEVEL_NATURE_LOW_KEY:
        case EVO_CRITICAL_HITS:
    #endif
        if (evo->param <= level)
            return TRUE;
        break;
    };

    return FALSE;
}

static bool8 IsValidItemEvo(struct Evolution* evo, bool8 itemEvos)
{
    if(!itemEvos)
        return FALSE;

    switch(evo->method)
    {
        case EVO_ITEM:
        case EVO_LEVEL_ITEM:
    #ifdef ROGUE_EXPANSION
        case EVO_ITEM_HOLD_DAY:
        case EVO_ITEM_HOLD_NIGHT:
        case EVO_MOVE:
        case EVO_MOVE_TYPE:
        case EVO_MAPSEC:
        case EVO_ITEM_MALE:
        case EVO_ITEM_FEMALE:
        case EVO_LEVEL_RAIN:
        case EVO_SPECIFIC_MON_IN_PARTY:
        case EVO_LEVEL_DARK_TYPE_MON_IN_PARTY:
        case EVO_SPECIFIC_MAP:
        case EVO_SCRIPT_TRIGGER_DMG:
        case EVO_DARK_SCROLL:
        case EVO_WATER_SCROLL:
    #endif
        return TRUE;
    };

    return FALSE;
}

static void RogueQuery_ReconsiderEvolveSpecies(u16 targetSpecies, u16 originSpecies, u8 level, bool8 itemEvos, bool8 removeChild)
{
    u8 i;
    u8 e;
    struct Evolution evo;
    bool8 shouldEvolve = FALSE;

    if(Rogue_GetEvolutionCount(targetSpecies))
    {
        for(e = 0; e < EVOS_PER_MON; ++e)
        {
            Rogue_ModifyEvolution(targetSpecies, e, &evo);
            shouldEvolve = FALSE;

            if(evo.method != 0 && !IsGenEnabled(SpeciesToGen(targetSpecies)))
            {
                // If the baby mon didn't exist for the enabled gen we will force it to evolve
                shouldEvolve = TRUE;
            }
            else if(IsValidLevelEvo(&evo, level) || IsValidItemEvo(&evo, itemEvos))
            {
                shouldEvolve = TRUE;
            }

            if(shouldEvolve)
            {
                SetQueryState(evo.targetSpecies, TRUE);
                if(removeChild)
                {
                    SetQueryState(targetSpecies, FALSE);
                }

                // Check against origin species, as if it is greater than, we have yet to process it so can wait until later
                if(evo.targetSpecies < originSpecies)
                {
                    // We've already considered this species so we must reconsider it e.g. if a baby mon was introduced in later gen 
                    // (Azuril is a good example as it will miss out on full evo chain)
                    RogueQuery_ReconsiderEvolveSpecies(evo.targetSpecies, originSpecies, level, itemEvos, removeChild);
                }
            }
        }
    }
}

static void RogueQuery_EvolveSpeciesInternal(u8 level, bool8 itemEvos, bool8 removeChild)
{
    u8 i;
    u8 e;
    u16 species;
    struct Evolution evo;
    bool8 shouldEvolve = FALSE;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species) && Rogue_GetEvolutionCount(species))
        {
            for(e = 0; e < EVOS_PER_MON; ++e)
            {
                Rogue_ModifyEvolution(species, e, &evo);
                shouldEvolve = FALSE;

                if(evo.method != 0 && !IsGenEnabled(SpeciesToGen(species)))
                {
                    // If the baby mon didn't exist for the enabled gen we will force it to evolve
                    shouldEvolve = TRUE;
                }
                else if(IsValidLevelEvo(&evo, level) || IsValidItemEvo(&evo, itemEvos))
                {
                    shouldEvolve = TRUE;
                }

                if(shouldEvolve)
                {
                    SetQueryState(evo.targetSpecies, TRUE);
                    if(removeChild)
                    {
                        SetQueryState(species, FALSE);
                    }

                    if(evo.targetSpecies < species)
                    {
                        // We've already considered this species so we must reconsider it e.g. if a baby mon was introduced in later gen 
                        // (Azuril is a good example as it will miss out on full evo chain)
                        RogueQuery_ReconsiderEvolveSpecies(evo.targetSpecies, species, level, itemEvos, removeChild);
                    }
                }
            }
        }
    }
}

void RogueQuery_EvolveSpecies(u8 level, bool8 itemEvos)
{
    RogueQuery_EvolveSpeciesInternal(level, itemEvos, TRUE);
}

void RogueQuery_EvolveSpeciesAndKeepPreEvo(u8 level, bool8 itemEvos)
{
    RogueQuery_EvolveSpeciesInternal(level, itemEvos, FALSE);
}

void RogueQuery_SpeciesIsLegendary(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(!IsSpeciesLegendary(species))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesIsNotLegendary(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(IsSpeciesLegendary(species))
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesIsNotWeakLegendary(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(IsSpeciesLegendary(species))
            {
                bool8 isStrong = ((gPresetMonTable[species].flags & MON_FLAG_STRONG_WILD) != 0);

                if(!isStrong)
                {
                    SetQueryState(species, FALSE);
                }
            }
        }
    }
}

void RogueQuery_SpeciesIsNotStrongLegendary(void)
{
    u16 species;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if(IsSpeciesLegendary(species))
            {
                bool8 isStrong = ((gPresetMonTable[species].flags & MON_FLAG_STRONG_WILD) != 0);

                if(isStrong)
                {
                    SetQueryState(species, FALSE);
                }
            }
        }
    }
}

void RogueQuery_SpeciesIncludeMonFlags(u16 flags)
{
    u16 species;

    if(flags == MON_FLAG_NONE)
        return;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if((gPresetMonTable[species].flags & flags) == 0)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

void RogueQuery_SpeciesExcludeMonFlags(u16 flags)
{
    u16 species;

    if(flags == MON_FLAG_NONE)
        return;

    for(species = SPECIES_NONE + 1; species < QUERY_NUM_SPECIES; ++species)
    {
        if(GetQueryState(species))
        {
            if((gPresetMonTable[species].flags & flags) != 0)
            {
                SetQueryState(species, FALSE);
            }
        }
    }
}

// Items
//

void RogueQuery_ItemsIsValid(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(item.itemId != itemId)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsExcludeCommon(void)
{
    u16 itemId;
    u16 maxGen = VarGet(VAR_ROGUE_ENABLED_GEN_LIMIT);

    RogueQuery_Exclude(ITEM_SACRED_ASH);
    RogueQuery_Exclude(ITEM_REVIVAL_HERB);
    RogueQuery_Exclude(ITEM_REVIVE);
    RogueQuery_Exclude(ITEM_MAX_REVIVE);
    RogueQuery_Exclude(ITEM_RARE_CANDY);
    RogueQuery_Exclude(ITEM_HEART_SCALE);
    RogueQuery_Exclude(ITEM_LUCKY_EGG);

    RogueQuery_ItemsExcludeRange(FIRST_MAIL_INDEX, LAST_MAIL_INDEX);
    RogueQuery_ItemsExcludeRange(ITEM_RED_SCARF, ITEM_YELLOW_SCARF);
    RogueQuery_ItemsExcludeRange(ITEM_RED_SHARD, ITEM_GREEN_SHARD);
    RogueQuery_ItemsExcludeRange(ITEM_BLUE_FLUTE, ITEM_WHITE_FLUTE);
    
#ifdef ROGUE_EXPANSION
    // Not implemented
    RogueQuery_Exclude(ITEM_MAX_HONEY);
    RogueQuery_Exclude(ITEM_LURE);
    RogueQuery_Exclude(ITEM_SUPER_LURE);
    RogueQuery_Exclude(ITEM_MAX_LURE);

    RogueQuery_Exclude(ITEM_PRISM_SCALE); // Not needed as is not a lvl up evo
    RogueQuery_ItemsExcludeRange(ITEM_GROWTH_MULCH, ITEM_BLACK_APRICORN);

    // Exclude all treasures then turn on the ones we want to use
    RogueQuery_ItemsExcludeRange(ITEM_BOTTLE_CAP, ITEM_STRANGE_SOUVENIR);
    RogueQuery_Include(ITEM_NUGGET);
    RogueQuery_Include(ITEM_PEARL);
    RogueQuery_Include(ITEM_BIG_PEARL);
    RogueQuery_Include(ITEM_STARDUST);
    RogueQuery_Include(ITEM_STAR_PIECE);

    // These TMs aren't setup
    RogueQuery_ItemsExcludeRange(ITEM_TM51, ITEM_TM100);

    // Regional treat (Avoid spawning in multiple)
    RogueQuery_ItemsExcludeRange(ITEM_PEWTER_CRUNCHIES, ITEM_BIG_MALASADA);

    // Ignore fossils for now
    RogueQuery_ItemsExcludeRange(ITEM_HELIX_FOSSIL, ITEM_FOSSILIZED_DINO);

    // Ignore sweets, as they are not used
    RogueQuery_ItemsExcludeRange(ITEM_STRAWBERRY_SWEET, ITEM_RIBBON_SWEET);
    
    // Ignroe these, as mons/form swaps currently not enabled
    RogueQuery_Exclude(ITEM_PIKASHUNIUM_Z);
    RogueQuery_Exclude(ITEM_ULTRANECROZIUM_Z);

    // Exclude everything but plates
    //RogueQuery_ItemsExcludeRange(ITEM_FLAME_PLATE, ITEM_FAIRY_MEMORY);
    RogueQuery_ItemsExcludeRange(ITEM_DOUSE_DRIVE, ITEM_CHILL_DRIVE);
    RogueQuery_ItemsExcludeRange(ITEM_FIRE_MEMORY, ITEM_FAIRY_MEMORY);

    switch(maxGen)
    {
        case 1:
            RogueQuery_Include(ITEM_PEWTER_CRUNCHIES);
            break;
        case 2:
            RogueQuery_Include(ITEM_RAGE_CANDY_BAR);
            break;
        case 3:
            RogueQuery_Include(ITEM_LAVA_COOKIE);
            break;
        case 4:
            RogueQuery_Include(ITEM_OLD_GATEAU);
            break;
        case 5:
            RogueQuery_Include(ITEM_CASTELIACONE);
            break;
        case 6:
            RogueQuery_Include(ITEM_LUMIOSE_GALETTE);
            break;
        case 7:
            RogueQuery_Include(ITEM_SHALOUR_SABLE);
            break;
        //case 8:
        default:
            RogueQuery_Include(ITEM_BIG_MALASADA);
            break;
    }

    if(!IsMegaEvolutionEnabled())
    {
        RogueQuery_ItemsExcludeRange(ITEM_RED_ORB, ITEM_DIANCITE);
    }

    if(!IsZMovesEnabled())
    {
        RogueQuery_ItemsExcludeRange(ITEM_NORMALIUM_Z, ITEM_ULTRANECROZIUM_Z);
    }

    if(!IsDynamaxEnabled())
    {
        RogueQuery_ItemsExcludeRange(ITEM_EXP_CANDY_XS, ITEM_DYNAMAX_CANDY);
        RogueQuery_Exclude(ITEM_MAX_MUSHROOMS);
    }

    if(!FlagGet(FLAG_ROGUE_EV_GAIN_ENABLED))
    {
        RogueQuery_ItemsExcludeRange(ITEM_HEALTH_FEATHER, ITEM_SWIFT_FEATHER);
        RogueQuery_ItemsExcludeRange(ITEM_HP_UP, ITEM_CARBOS);
        RogueQuery_ItemsExcludeRange(ITEM_MACHO_BRACE, ITEM_POWER_ANKLET);
    }
#else
    if(!FlagGet(FLAG_ROGUE_EV_GAIN_ENABLED))
    {
        // These items aren't next to each other in vanilla
        RogueQuery_ItemsExcludeRange(ITEM_HP_UP, ITEM_CALCIUM);
        RogueQuery_Exclude(ITEM_ZINC);
    }
#endif

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            if(!IsGenEnabled(ItemToGen(itemId)) || Rogue_CheckCampaignBansItem(itemId))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsInPocket(u8 pocket)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(item.pocket != pocket)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsNotInPocket(u8 pocket)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(item.pocket == pocket)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsInPriceRange(u16 minPrice, u16 maxPrice)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(item.price < minPrice || item.price > maxPrice)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

static bool8 IsExtraEvolutionItem(struct Item* item)
{
    switch(item->itemId)
    {
        case ITEM_LINK_CABLE:
        case ITEM_KINGS_ROCK:
        case ITEM_METAL_COAT:
        case ITEM_DRAGON_SCALE:
        case ITEM_DEEP_SEA_TOOTH:
        case ITEM_DEEP_SEA_SCALE:
#ifdef ROGUE_EXPANSION
        case ITEM_UPGRADE:
        case ITEM_RAZOR_FANG:
        case ITEM_RAZOR_CLAW:
#else
        case ITEM_UP_GRADE:
#endif
            return TRUE;
    }

#ifdef ROGUE_EXPANSION
    if(item->itemId >= ITEM_FIRE_STONE && item->itemId <= ITEM_RIBBON_SWEET)
    {
        return TRUE;
    }

    if(item->itemId >= ITEM_RED_ORB && item->itemId <= ITEM_DIANCITE)
    {
        return TRUE;
    }

    if(item->itemId >= ITEM_RED_NECTAR && item->itemId <= ITEM_PURPLE_NECTAR)
    {
        return TRUE;
    }
#endif

    return FALSE;
}

static bool8 IsRareHeldItem(struct Item* item);

static bool8 IsBattleEnchancer(struct Item* item)
{
    return (item->itemId >= ITEM_HP_UP && item->itemId <= ITEM_PP_MAX) ||
        item->fieldUseFunc == ItemUseOutOfBattle_EvolutionStone ||
        item->battleUseFunc == ItemUseInBattle_StatIncrease ||
#ifdef ROGUE_EXPANSION
        (item->itemId >= ITEM_HEALTH_FEATHER && item->itemId <= ITEM_SERIOUS_MINT) ||
#endif
        IsExtraEvolutionItem(item);
}

static bool8 IsMedicine(struct Item* item)
{
    return (item->fieldUseFunc == ItemUseOutOfBattle_Medicine || 
        item->fieldUseFunc == ItemUseOutOfBattle_PPRecovery ||
        item->fieldUseFunc == ItemUseOutOfBattle_Repel) 
        && !IsBattleEnchancer(item);
}

static bool8 IsHeldItem(struct Item* item)
{
    return item->holdEffect != HOLD_EFFECT_NONE && !IsBattleEnchancer(item);
}

static bool8 IsRareHeldItem(struct Item* item)
{
#ifdef ROGUE_EXPANSION
    return (item->itemId >= ITEM_RED_ORB && item->itemId <= ITEM_DIANCITE) ||
        (item->itemId >= ITEM_NORMALIUM_Z && item->itemId <= ITEM_ULTRANECROZIUM_Z) ||
        (item->itemId == ITEM_RUSTED_SWORD || item->itemId == ITEM_RUSTED_SHIELD) ||
        (item->itemId == ITEM_ADAMANT_ORB || item->itemId == ITEM_LUSTROUS_ORB || item->itemId == ITEM_GRISEOUS_ORB) ||
        item->itemId == ITEM_SOUL_DEW ||
        item->itemId == ITEM_DYNAMAX_CANDY ||
        item->itemId == ITEM_MAX_MUSHROOMS;
#else
    return FALSE;
#endif
}

void RogueQuery_ItemsHeldItem(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(!IsHeldItem(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsNotHeldItem(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(IsHeldItem(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsRareHeldItem(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(!IsRareHeldItem(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsNotRareHeldItem(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(IsRareHeldItem(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsMedicine(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(!IsMedicine(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsNotMedicine(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(IsMedicine(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsBattleEnchancer(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(!IsBattleEnchancer(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsNotBattleEnchancer(void)
{
    u16 itemId;
    struct Item item;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            Rogue_ModifyItem(itemId, &item);

            if(IsBattleEnchancer(&item))
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}

void RogueQuery_ItemsExcludeRange(u16 fromId, u16 toId)
{
    u16 itemId;

    for(itemId = ITEM_NONE + 1; itemId < ITEMS_COUNT; ++itemId)
    {
        if(GetQueryState(itemId))
        {
            if(itemId >= fromId && itemId <= toId)
            {
                SetQueryState(itemId, FALSE);
            }
        }
    }
}
