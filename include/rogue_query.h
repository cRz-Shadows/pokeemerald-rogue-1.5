#ifndef ROGUE_QUERY_H
#define ROGUE_QUERY_H

typedef bool8 (*QueryCallback)(u16 elem, u16 usrData);

u16 RogueUtil_GetEggSpecies(u16 species);

void RogueQuery_Clear(void);
void RogueQuery_CollapseSpeciesBuffer(void);
void RogueQuery_CollapseItemBuffer(void);
u16* RogueQuery_BufferPtr(void);
u16 RogueQuery_BufferSize(void);
bool8 RogueQuery_CheckIncluded(u16 id);

// Uncollapsed queries are slow but can be used to query the entire range
u16 RogueQuery_UncollapsedSpeciesSize(void);
u16 RogueQuery_UncollapsedItemSize(void);
u16 RogueQuery_AtUncollapsedIndex(u16 idx);

u16 RogueQuery_PopCollapsedIndex(u16 idx);
u16 RogueQuery_PopUncollapsedIndex(u16 idx);

void RogueQuery_Include(u16 idx);
void RogueQuery_IncludeRange(u16 fromId, u16 toId);
void RogueQuery_Exclude(u16 idx);
void RogueQuery_ExcludeAll(void);
void RogueQuery_CustomSpecies(QueryCallback query, u16 usrData);
void RogueQuery_CustomItems(QueryCallback query, u16 usrData);

// Species
void RogueQuery_SpeciesIsValid(u8 earlyCullType1, u8 earlyCullType2, u8 earlyCullType3);
void RogueQuery_SpeciesExcludeCommon(void);
void RogueQuery_SpeciesExcludeRange(u16 fromId, u16 toId);
void RogueQuery_SpeciesInPokedex(void);
void RogueQuery_SpeciesInGeneration(u8 gen);
void RogueQuery_SpeciesOfType(u8 type);
void RogueQuery_SpeciesNotOfType(u8 type);
void RogueQuery_SpeciesOfTypes(const u8* types, u8 count);
void RogueQuery_SpeciesNotOfTypes(const u8* types, u8 count);
void RogueQuery_SpeciesIsFinalEvolution(void);
void RogueQuery_TransformToEggSpecies(void);
void RogueQuery_SpeciesWithAtLeastEvolutionStages(u8 count);
void RogueQuery_EvolveSpecies(u8 level, bool8 itemEvos);
void RogueQuery_EvolveSpeciesAndKeepPreEvo(u8 level, bool8 itemEvos);
void RogueQuery_SpeciesIsLegendary(void);
void RogueQuery_SpeciesIsNotLegendary(void);
void RogueQuery_SpeciesIsNotWeakLegendary(void);
void RogueQuery_SpeciesIsNotStrongLegendary(void);
void RogueQuery_SpeciesIncludeMonFlags(u16 flags);
void RogueQuery_SpeciesExcludeMonFlags(u16 flags);

// Items
void RogueQuery_ItemsIsValid(void);
void RogueQuery_ItemsExcludeCommon(void);
void RogueQuery_ItemsInPocket(u8 pocket);
void RogueQuery_ItemsNotInPocket(u8 pocket);
void RogueQuery_ItemsHeldItem(void);
void RogueQuery_ItemsNotHeldItem(void);
void RogueQuery_ItemsRareHeldItem(void);
void RogueQuery_ItemsNotRareHeldItem(void);
void RogueQuery_ItemsMedicine(void);
void RogueQuery_ItemsNotMedicine(void);
void RogueQuery_ItemsBattleEnchancer(void);
void RogueQuery_ItemsNotBattleEnchancer(void);
void RogueQuery_ItemsExcludeRange(u16 fromId, u16 toId);
void RogueQuery_ItemsInPriceRange(u16 minPrice, u16 maxPrice);

#endif