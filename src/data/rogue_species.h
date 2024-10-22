static const u16 sEncounters_Test0[] = {
    SPECIES_YANMA, SPECIES_TREECKO
};

static const u16 sEncounters_Test1[] = {
    SPECIES_TORCHIC, SPECIES_MUDKIP
};

const struct SpeciesTable gRogueSpeciesTable[] = 
{
    {
        .wildSpeciesCount = ARRAY_COUNT(sEncounters_Test0),
        .wildSpecies = sEncounters_Test0,
        .trainerSpeciesCount = ARRAY_COUNT(sEncounters_Test1),
        .trainerSpecies = sEncounters_Test1
    },
};

static const u16 sDexSpecies_Kanto[] = 
{
    SPECIES_BULBASAUR,
    SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE,
    SPECIES_CATERPIE,
    SPECIES_WEEDLE,
    SPECIES_PIDGEY,
    SPECIES_RATTATA,
    SPECIES_SPEAROW,
    SPECIES_EKANS,
    SPECIES_PIKACHU,
    SPECIES_SANDSHREW,
    SPECIES_NIDORAN_F,
    SPECIES_NIDORAN_M,
    SPECIES_CLEFAIRY,
    SPECIES_VULPIX,
    SPECIES_JIGGLYPUFF,
    SPECIES_ZUBAT,
    SPECIES_ODDISH,
    SPECIES_PARAS,
    SPECIES_VENONAT,
    SPECIES_DIGLETT,
    SPECIES_MEOWTH,
    SPECIES_PSYDUCK,
    SPECIES_MANKEY,
    SPECIES_GROWLITHE,
    SPECIES_POLIWAG,
    SPECIES_ABRA,
    SPECIES_MACHOP,
    SPECIES_BELLSPROUT,
    SPECIES_TENTACOOL,
    SPECIES_GEODUDE,
    SPECIES_PONYTA,
    SPECIES_SLOWPOKE,
    SPECIES_MAGNEMITE,
    SPECIES_FARFETCHD,
    SPECIES_DODUO,
    SPECIES_SEEL,
    SPECIES_GRIMER,
    SPECIES_SHELLDER,
    SPECIES_GASTLY,
    SPECIES_ONIX,
    SPECIES_DROWZEE,
    SPECIES_KRABBY,
    SPECIES_VOLTORB,
    SPECIES_EXEGGCUTE,
    SPECIES_CUBONE,
    SPECIES_HITMONLEE,
    SPECIES_HITMONCHAN,
    SPECIES_LICKITUNG,
    SPECIES_KOFFING,
    SPECIES_RHYHORN,
    SPECIES_CHANSEY,
    SPECIES_TANGELA,
    SPECIES_KANGASKHAN,
    SPECIES_HORSEA,
    SPECIES_GOLDEEN,
    SPECIES_STARYU,
    SPECIES_MR_MIME,
    SPECIES_SCYTHER,
    SPECIES_JYNX,
    SPECIES_ELECTABUZZ,
    SPECIES_MAGMAR,
    SPECIES_PINSIR,
    SPECIES_TAUROS,
    SPECIES_MAGIKARP,
    SPECIES_LAPRAS,
    SPECIES_DITTO,
    SPECIES_EEVEE,
    SPECIES_PORYGON,
    SPECIES_OMANYTE,
    SPECIES_KABUTO,
    SPECIES_AERODACTYL,
    SPECIES_SNORLAX,
    SPECIES_ARTICUNO,
    SPECIES_ZAPDOS,
    SPECIES_MOLTRES,
    SPECIES_DRATINI,
    SPECIES_MEWTWO,
    SPECIES_MEW,
};

static const u16 sDexSpecies_Johto[] = 
{
    SPECIES_CHIKORITA,
    SPECIES_CYNDAQUIL,
    SPECIES_TOTODILE,
    SPECIES_PIDGEY,
    SPECIES_HOOTHOOT,
    SPECIES_RATTATA,
    SPECIES_SENTRET,
    SPECIES_PICHU,
    SPECIES_CATERPIE,
    SPECIES_WEEDLE,
    SPECIES_LEDYBA,
    SPECIES_SPINARAK,
    SPECIES_GEODUDE,
    SPECIES_ZUBAT,
    SPECIES_CLEFFA,
    SPECIES_IGGLYBUFF,
    SPECIES_TOGEPI,
    SPECIES_SANDSHREW,
    SPECIES_EKANS,
    SPECIES_DUNSPARCE,
    SPECIES_MAREEP,
    SPECIES_WOOPER,
    SPECIES_GASTLY,
    SPECIES_UNOWN,
    SPECIES_ONIX,
    SPECIES_BELLSPROUT,
    SPECIES_HOPPIP,
    SPECIES_PARAS,
    SPECIES_POLIWAG,
    SPECIES_MAGIKARP,
    SPECIES_GOLDEEN,
    SPECIES_SLOWPOKE,
    SPECIES_ODDISH,
    SPECIES_DROWZEE,
    SPECIES_ABRA,
    SPECIES_DITTO,
    SPECIES_PINECO,
    SPECIES_NIDORAN_F,
    SPECIES_NIDORAN_M,
    SPECIES_YANMA,
    SPECIES_SUNKERN,
    SPECIES_EXEGGCUTE,
    SPECIES_SUDOWOODO,
    SPECIES_WOBBUFFET,
    SPECIES_VENONAT,
    SPECIES_SCYTHER,
    SPECIES_PINSIR,
    SPECIES_HERACROSS,
    SPECIES_KOFFING,
    SPECIES_GRIMER,
    SPECIES_MAGNEMITE,
    SPECIES_VOLTORB,
    SPECIES_AIPOM,
    SPECIES_SNUBBULL,
    SPECIES_VULPIX,
    SPECIES_GROWLITHE,
    SPECIES_STANTLER,
    SPECIES_MARILL,
    SPECIES_DIGLETT,
    SPECIES_MANKEY,
    SPECIES_MEOWTH,
    SPECIES_PSYDUCK,
    SPECIES_MACHOP,
    SPECIES_TYROGUE,
    SPECIES_GIRAFARIG,
    SPECIES_TAUROS,
    SPECIES_MILTANK,
    SPECIES_MAGBY,
    SPECIES_SMOOCHUM,
    SPECIES_ELEKID,
    SPECIES_MR_MIME,
    SPECIES_SMEARGLE,
    SPECIES_FARFETCHD,
    SPECIES_NATU,
    SPECIES_QWILFISH,
    SPECIES_TENTACOOL,
    SPECIES_KRABBY,
    SPECIES_SHUCKLE,
    SPECIES_STARYU,
    SPECIES_SHELLDER,
    SPECIES_CORSOLA,
    SPECIES_REMORAID,
    SPECIES_CHINCHOU,
    SPECIES_SEEL,
    SPECIES_LICKITUNG,
    SPECIES_TANGELA,
    SPECIES_EEVEE,
    SPECIES_HORSEA,
    SPECIES_GLIGAR,
    SPECIES_DELIBIRD,
    SPECIES_SWINUB,
    SPECIES_TEDDIURSA,
    SPECIES_PHANPY,
    SPECIES_MANTINE,
    SPECIES_SKARMORY,
    SPECIES_DODUO,
    SPECIES_PONYTA,
    SPECIES_CUBONE,
    SPECIES_KANGASKHAN,
    SPECIES_RHYHORN,
    SPECIES_MURKROW,
    SPECIES_HOUNDOUR,
    SPECIES_SLUGMA,
    SPECIES_SNEASEL,
    SPECIES_MISDREAVUS,
    SPECIES_PORYGON,
    SPECIES_CHANSEY,
    SPECIES_LAPRAS,
    SPECIES_OMANYTE,
    SPECIES_KABUTO,
    SPECIES_AERODACTYL,
    SPECIES_SNORLAX,
    SPECIES_BULBASAUR,
    SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE,
    SPECIES_ARTICUNO,
    SPECIES_ZAPDOS,
    SPECIES_MOLTRES,
    SPECIES_RAIKOU,
    SPECIES_ENTEI,
    SPECIES_SUICUNE,
    SPECIES_DRATINI,
    SPECIES_LARVITAR,
    SPECIES_LUGIA,
    SPECIES_HO_OH,
    SPECIES_MEWTWO,
    SPECIES_MEW,
    SPECIES_CELEBI,
};

static const u16 sDexSpecies_Hoenn[] = 
{
    SPECIES_TREECKO,
    SPECIES_TORCHIC,
    SPECIES_MUDKIP,
    SPECIES_POOCHYENA,
    SPECIES_ZIGZAGOON,
    SPECIES_WURMPLE,
    SPECIES_LOTAD,
    SPECIES_SEEDOT,
    SPECIES_TAILLOW,
    SPECIES_WINGULL,
    SPECIES_RALTS,
    SPECIES_SURSKIT,
    SPECIES_SHROOMISH,
    SPECIES_SLAKOTH,
    SPECIES_ABRA,
    SPECIES_NINCADA,
    SPECIES_WHISMUR,
    SPECIES_MAKUHITA,
    SPECIES_GOLDEEN,
    SPECIES_MAGIKARP,
    SPECIES_AZURILL,
    SPECIES_GEODUDE,
    SPECIES_NOSEPASS,
    SPECIES_SKITTY,
    SPECIES_ZUBAT,
    SPECIES_TENTACOOL,
    SPECIES_SABLEYE,
    SPECIES_MAWILE,
    SPECIES_ARON,
    SPECIES_MACHOP,
    SPECIES_MEDITITE,
    SPECIES_ELECTRIKE,
    SPECIES_PLUSLE,
    SPECIES_MINUN,
    SPECIES_MAGNEMITE,
    SPECIES_VOLTORB,
    SPECIES_VOLBEAT,
    SPECIES_ODDISH,
    SPECIES_DODUO,
    SPECIES_ROSELIA,
    SPECIES_GULPIN,
    SPECIES_CARVANHA,
    SPECIES_WAILMER,
    SPECIES_NUMEL,
    SPECIES_SLUGMA,
    SPECIES_TORKOAL,
    SPECIES_GRIMER,
    SPECIES_KOFFING,
    SPECIES_SPOINK,
    SPECIES_SANDSHREW,
    SPECIES_SPINDA,
    SPECIES_SKARMORY,
    SPECIES_TRAPINCH,
    SPECIES_CACNEA,
    SPECIES_SWABLU,
    SPECIES_ZANGOOSE,
    SPECIES_SEVIPER,
    SPECIES_LUNATONE,
    SPECIES_SOLROCK,
    SPECIES_BARBOACH,
    SPECIES_CORPHISH,
    SPECIES_BALTOY,
    SPECIES_LILEEP,
    SPECIES_ANORITH,
    SPECIES_IGGLYBUFF,
    SPECIES_FEEBAS,
    SPECIES_CASTFORM,
    SPECIES_STARYU,
    SPECIES_KECLEON,
    SPECIES_SHUPPET,
    SPECIES_DUSKULL,
    SPECIES_TROPIUS,
    SPECIES_CHIMECHO,
    SPECIES_ABSOL,
    SPECIES_VULPIX,
    SPECIES_NINETALES,
    SPECIES_PICHU,
    SPECIES_PSYDUCK,
    SPECIES_WYNAUT,
    SPECIES_NATU,
    SPECIES_GIRAFARIG,
    SPECIES_PHANPY,
    SPECIES_PINSIR,
    SPECIES_HERACROSS,
    SPECIES_RHYHORN,
    SPECIES_SNORUNT,
    SPECIES_SPHEAL,
    SPECIES_CLAMPERL,
    SPECIES_RELICANTH,
    SPECIES_CORSOLA,
    SPECIES_CHINCHOU,
    SPECIES_LUVDISC,
    SPECIES_HORSEA,
    SPECIES_BAGON,
    SPECIES_BELDUM,
    SPECIES_REGIROCK,
    SPECIES_REGICE,
    SPECIES_REGISTEEL,
    SPECIES_LATIAS,
    SPECIES_LATIOS,
    SPECIES_KYOGRE,
    SPECIES_GROUDON,
    SPECIES_RAYQUAZA,
    SPECIES_JIRACHI,
    SPECIES_DEOXYS,
};

#ifdef ROGUE_EXPANSION
static const u16 sDexSpecies_Sinnoh[] = 
{
    SPECIES_TURTWIG,
    SPECIES_CHIMCHAR,
    SPECIES_PIPLUP,
    SPECIES_STARLY,
    SPECIES_BIDOOF,
    SPECIES_KRICKETOT,
    SPECIES_SHINX,
    SPECIES_ABRA,
    SPECIES_MAGIKARP,
    SPECIES_BUDEW,
    SPECIES_ZUBAT,
    SPECIES_GEODUDE,
    SPECIES_ONIX,
    SPECIES_CRANIDOS,
    SPECIES_SHIELDON,
    SPECIES_MACHOP,
    SPECIES_PSYDUCK,
    SPECIES_BURMY,
    SPECIES_WURMPLE,
    SPECIES_COMBEE,
    SPECIES_PACHIRISU,
    SPECIES_BUIZEL,
    SPECIES_CHERUBI,
    SPECIES_SHELLOS,
    SPECIES_HERACROSS,
    SPECIES_AIPOM,
    SPECIES_DRIFLOON,
    SPECIES_BUNEARY,
    SPECIES_GASTLY,
    SPECIES_MISDREAVUS,
    SPECIES_MURKROW,
    SPECIES_GLAMEOW,
    SPECIES_GOLDEEN,
    SPECIES_BARBOACH,
    SPECIES_CHINGLING,
    SPECIES_STUNKY,
    SPECIES_MEDITITE,
    SPECIES_BRONZOR,
    SPECIES_PONYTA,
    SPECIES_BONSLY,
    SPECIES_MIME_JR,
    SPECIES_HAPPINY,
    SPECIES_CLEFFA,
    SPECIES_CHATOT,
    SPECIES_PICHU,
    SPECIES_HOOTHOOT,
    SPECIES_SPIRITOMB,
    SPECIES_GIBLE,
    SPECIES_MUNCHLAX,
    SPECIES_UNOWN,
    SPECIES_RIOLU,
    SPECIES_WOOPER,
    SPECIES_WINGULL,
    SPECIES_GIRAFARIG,
    SPECIES_HIPPOPOTAS,
    SPECIES_AZURILL,
    SPECIES_SKORUPI,
    SPECIES_CROAGUNK,
    SPECIES_CARNIVINE,
    SPECIES_REMORAID,
    SPECIES_FINNEON,
    SPECIES_TENTACOOL,
    SPECIES_FEEBAS,
    SPECIES_MANTYKE,
    SPECIES_SNOVER,
    SPECIES_SNEASEL,
    SPECIES_UXIE,
    SPECIES_MESPRIT,
    SPECIES_AZELF,
    SPECIES_DIALGA,
    SPECIES_PALKIA,
    SPECIES_MANAPHY,

    SPECIES_ROTOM,
    SPECIES_GLIGAR,
    SPECIES_NOSEPASS,
    SPECIES_RALTS,
    SPECIES_LICKITUNG,
    SPECIES_EEVEE,
    SPECIES_SWABLU,
    SPECIES_TOGEPI,
    SPECIES_HOUNDOUR,
    SPECIES_MAGNEMITE,
    SPECIES_TANGELA,
    SPECIES_YANMA,
    SPECIES_TROPIUS,
    SPECIES_RHYHORN,
    SPECIES_DUSKULL,
    SPECIES_PORYGON,
    SPECIES_SCYTHER,
    SPECIES_ELEKID,
    SPECIES_MAGBY,
    SPECIES_SWINUB,
    SPECIES_SNORUNT,
    SPECIES_ABSOL,
    SPECIES_GIRATINA,
    
    SPECIES_HEATRAN,
    SPECIES_REGIGIGAS,
    SPECIES_CRESSELIA,
    SPECIES_PHIONE,
    SPECIES_DARKRAI,
    SPECIES_SHAYMIN,
    SPECIES_ARCEUS,
};

static const u16 sDexSpecies_Unova[] = 
{
    SPECIES_VICTINI,
    SPECIES_SNIVY,
    SPECIES_TEPIG,
    SPECIES_OSHAWOTT,
    SPECIES_PATRAT,
    SPECIES_LILLIPUP,
    SPECIES_PURRLOIN,
    SPECIES_PANSAGE,
    SPECIES_PANSEAR,
    SPECIES_PANPOUR,
    SPECIES_MUNNA,
    SPECIES_PIDOVE,
    SPECIES_BLITZLE,
    SPECIES_ROGGENROLA,
    SPECIES_WOOBAT,
    SPECIES_DRILBUR,
    SPECIES_AUDINO,
    SPECIES_TIMBURR,
    SPECIES_TYMPOLE,
    SPECIES_THROH,
    SPECIES_SAWK,
    SPECIES_SEWADDLE,
    SPECIES_VENIPEDE,
    SPECIES_COTTONEE,
    SPECIES_PETILIL,
    SPECIES_BASCULIN,
    SPECIES_SANDILE,
    SPECIES_DARUMAKA,
    SPECIES_MARACTUS,
    SPECIES_DWEBBLE,
    SPECIES_SCRAGGY,
    SPECIES_SIGILYPH,
    SPECIES_YAMASK,
    SPECIES_COFAGRIGUS,
    SPECIES_TIRTOUGA,
    SPECIES_ARCHEN,
    SPECIES_TRUBBISH,
    SPECIES_ZORUA,
    SPECIES_MINCCINO,
    SPECIES_GOTHITA,
    SPECIES_SOLOSIS,
    SPECIES_DUCKLETT,
    SPECIES_VANILLITE,
    SPECIES_DEERLING,
    SPECIES_EMOLGA,
    SPECIES_KARRABLAST,
    SPECIES_FOONGUS,
    SPECIES_FRILLISH,
    SPECIES_ALOMOMOLA,
    SPECIES_JOLTIK,
    SPECIES_FERROSEED,
    SPECIES_KLINK,
    SPECIES_TYNAMO,
    SPECIES_ELGYEM,
    SPECIES_LITWICK,
    SPECIES_AXEW,
    SPECIES_CUBCHOO,
    SPECIES_SHELMET,
    SPECIES_STUNFISK,
    SPECIES_MIENFOO,
    SPECIES_DRUDDIGON,
    SPECIES_GOLETT,
    SPECIES_PAWNIARD,
    SPECIES_BOUFFALANT,
    SPECIES_RUFFLET,
    SPECIES_VULLABY,
    SPECIES_HEATMOR,
    SPECIES_DURANT,
    SPECIES_DEINO,
    SPECIES_LARVESTA,
    SPECIES_COBALION,
    SPECIES_TERRAKION,
    SPECIES_VIRIZION,
    SPECIES_THUNDURUS,
    SPECIES_TORNADUS,
    SPECIES_RESHIRAM,
    SPECIES_ZEKROM,
    SPECIES_LANDORUS,
    SPECIES_KYUREM,
    SPECIES_KELDEO,
    SPECIES_MELOETTA,
    SPECIES_GENESECT,

    SPECIES_KYUREM_WHITE,
    SPECIES_KYUREM_BLACK,
};

static const u16 sDexSpecies_Kalos[] = 
{
    SPECIES_CHESPIN,
    SPECIES_FENNEKIN,
    SPECIES_FROAKIE,
    SPECIES_BUNNELBY,
    SPECIES_ZIGZAGOON,
    SPECIES_FLETCHLING,
    SPECIES_PIDGEY,
    SPECIES_SCATTERBUG,
    SPECIES_CATERPIE,
    SPECIES_WEEDLE,
    SPECIES_PANSAGE,
    SPECIES_PANSEAR,
    SPECIES_PANPOUR,
    SPECIES_PICHU,
    SPECIES_BIDOOF,
    SPECIES_DUNSPARCE,
    SPECIES_AZURILL,
    SPECIES_BURMY,
    SPECIES_SURSKIT,
    SPECIES_MAGIKARP,
    SPECIES_CORPHISH,
    SPECIES_GOLDEEN,
    SPECIES_CARVANHA,
    SPECIES_LITLEO,
    SPECIES_PSYDUCK,
    SPECIES_FARFETCHD,
    SPECIES_RIOLU,
    SPECIES_RALTS,
    SPECIES_FLABEBE,
    SPECIES_BUDEW,
    SPECIES_LEDYBA,
    SPECIES_COMBEE,
    SPECIES_SKITTY,
    SPECIES_BULBASAUR,
    SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE,
    SPECIES_SKIDDO,
    SPECIES_PANCHAM,
    SPECIES_FURFROU,
    SPECIES_DODUO,
    SPECIES_PLUSLE,
    SPECIES_MINUN,
    SPECIES_GULPIN,
    SPECIES_SCRAGGY,
    SPECIES_ABRA,
    SPECIES_ODDISH,
    SPECIES_SENTRET,
    SPECIES_NINCADA,
    SPECIES_ESPURR,
    SPECIES_KECLEON,
    SPECIES_HONEDGE,
    SPECIES_VENIPEDE,
    SPECIES_AUDINO,
    SPECIES_SMEARGLE,
    SPECIES_CROAGUNK,
    SPECIES_DUCKLETT,
    SPECIES_SPRITZEE,
    SPECIES_SWIRLIX,
    SPECIES_VOLBEAT,
    SPECIES_HOPPIP,
    SPECIES_MUNCHLAX,
    SPECIES_WHISMUR,
    SPECIES_MEDITITE,
    SPECIES_ZUBAT,
    SPECIES_AXEW,
    SPECIES_DIANCIE,
    SPECIES_HOOPA,
    SPECIES_VOLCANION,
    
    SPECIES_DRIFLOON,
    SPECIES_MIENFOO,
    SPECIES_ZANGOOSE,
    SPECIES_SEVIPER,
    SPECIES_SPOINK,
    SPECIES_ABSOL,
    SPECIES_INKAY,
    SPECIES_LUNATONE,
    SPECIES_SOLROCK,
    SPECIES_BAGON,
    SPECIES_WINGULL,
    SPECIES_TAILLOW,
    SPECIES_BINACLE,
    SPECIES_DWEBBLE,
    SPECIES_TENTACOOL,
    SPECIES_WAILMER,
    SPECIES_LUVDISC,
    SPECIES_SKRELP,
    SPECIES_CLAUNCHER,
    SPECIES_STARYU,
    SPECIES_SHELLDER,
    SPECIES_QWILFISH,
    SPECIES_HORSEA,
    SPECIES_RELICANTH,
    SPECIES_SANDILE,
    SPECIES_HELIOPTILE,
    SPECIES_HIPPOPOTAS,
    SPECIES_RHYHORN,
    SPECIES_ONIX,
    SPECIES_WOOBAT,
    SPECIES_MACHOP,
    SPECIES_CUBONE,
    SPECIES_KANGASKHAN,
    SPECIES_MAWILE,
    SPECIES_TYRUNT,
    SPECIES_AMAURA,
    SPECIES_AERODACTYL,
    SPECIES_FERROSEED,
    SPECIES_SNUBBULL,
    SPECIES_ELECTRIKE,
    SPECIES_HOUNDOUR,
    SPECIES_EEVEE,
    SPECIES_EMOLGA,
    SPECIES_YANMA,
    SPECIES_HAWLUCHA,
    SPECIES_SIGILYPH,
    SPECIES_GOLETT,
    SPECIES_NOSEPASS,
    SPECIES_MAKUHITA,
    SPECIES_THROH,
    SPECIES_SAWK,
    SPECIES_STARLY,
    SPECIES_STUNKY,
    SPECIES_NIDORAN_F,
    SPECIES_NIDORAN_M,
    SPECIES_DEDENNE,
    SPECIES_CHINGLING,
    SPECIES_MIME_JR,
    SPECIES_SOLOSIS,
    SPECIES_WYNAUT,
    SPECIES_ROGGENROLA,
    SPECIES_SABLEYE,
    SPECIES_CARBINK,
    SPECIES_TAUROS,
    SPECIES_MILTANK,
    SPECIES_MAREEP,
    SPECIES_PINSIR,
    SPECIES_HERACROSS,
    SPECIES_PACHIRISU,
    SPECIES_SLOWPOKE,
    SPECIES_EXEGGCUTE,
    SPECIES_CHATOT,
    SPECIES_MANTYKE,
    SPECIES_CLAMPERL,
    SPECIES_HUNTAIL,
    SPECIES_GOREBYSS,
    SPECIES_REMORAID,
    SPECIES_CORSOLA,
    SPECIES_CHINCHOU,
    SPECIES_ALOMOMOLA,
    SPECIES_LAPRAS,
    SPECIES_ARTICUNO,
    SPECIES_ZAPDOS,
    SPECIES_MOLTRES,
    
    SPECIES_DIGLETT,
    SPECIES_TRAPINCH,
    SPECIES_GIBLE,
    SPECIES_GEODUDE,
    SPECIES_SLUGMA,
    SPECIES_SHUCKLE,
    SPECIES_SKORUPI,
    SPECIES_WOOPER,
    SPECIES_GOOMY,
    SPECIES_KARRABLAST,
    SPECIES_SHELMET,
    SPECIES_BELLSPROUT,
    SPECIES_CARNIVINE,
    SPECIES_GASTLY,
    SPECIES_POLIWAG,
    SPECIES_EKANS,
    SPECIES_STUNFISK,
    SPECIES_BARBOACH,
    SPECIES_PURRLOIN,
    SPECIES_POOCHYENA,
    SPECIES_PATRAT,
    SPECIES_PAWNIARD,
    SPECIES_KLEFKI,
    SPECIES_MURKROW,
    SPECIES_FOONGUS,
    SPECIES_LOTAD,
    SPECIES_BUIZEL,
    SPECIES_BASCULIN,
    SPECIES_PHANTUMP,
    SPECIES_TREVENANT,
    SPECIES_PUMPKABOO,
    SPECIES_LITWICK,
    SPECIES_ROTOM,
    SPECIES_MAGNEMITE,
    SPECIES_VOLTORB,
    SPECIES_TRUBBISH,
    SPECIES_SWINUB,
    SPECIES_BERGMITE,
    SPECIES_CUBCHOO,
    SPECIES_SMOOCHUM,
    SPECIES_VANILLITE,
    SPECIES_SNOVER,
    SPECIES_DELIBIRD,
    SPECIES_SNEASEL,
    SPECIES_TIMBURR,
    SPECIES_TORKOAL,
    SPECIES_SANDSHREW,
    SPECIES_ARON,
    SPECIES_LARVITAR,
    SPECIES_HEATMOR,
    SPECIES_DURANT,
    SPECIES_SPINARAK,
    SPECIES_SPEAROW,
    SPECIES_CRYOGONAL,
    SPECIES_SKARMORY,
    SPECIES_NOIBAT,
    SPECIES_GLIGAR,
    SPECIES_HOOTHOOT,
    SPECIES_IGGLYBUFF,
    SPECIES_SHUPPET,
    SPECIES_ZORUA,
    SPECIES_GOTHITA,
    SPECIES_BONSLY,
    SPECIES_SPINDA,
    SPECIES_TEDDIURSA,
    SPECIES_LICKITUNG,
    SPECIES_SCYTHER,
    SPECIES_DITTO,
    SPECIES_SWABLU,
    SPECIES_DRUDDIGON,
    SPECIES_DEINO,
    SPECIES_DRATINI,
    SPECIES_XERNEAS,
    SPECIES_YVELTAL,
    SPECIES_MEWTWO,
};

static const u16 sDexSpecies_Alola[] = 
{
    SPECIES_ROWLET,
    SPECIES_LITTEN,
    SPECIES_POPPLIO,
    SPECIES_PIKIPEK,
    SPECIES_YUNGOOS,
    SPECIES_RATTATA_ALOLAN,
    SPECIES_CATERPIE,
    SPECIES_BUTTERFREE,
    SPECIES_LEDYBA,
    SPECIES_SPINARAK,
    SPECIES_BUNEARY,
    SPECIES_INKAY,
    SPECIES_ZORUA,
    SPECIES_FURFROU,
    SPECIES_PICHU,
    SPECIES_GRUBBIN,
    SPECIES_BONSLY,
    SPECIES_HAPPINY,
    SPECIES_MUNCHLAX,
    SPECIES_SLOWPOKE,
    SPECIES_WINGULL,
    SPECIES_ABRA,
    SPECIES_MEOWTH_ALOLAN,
    SPECIES_MAGNEMITE,
    SPECIES_GRIMER_ALOLAN,
    SPECIES_MIME_JR,
    SPECIES_EKANS,
    SPECIES_DUNSPARCE,
    SPECIES_GROWLITHE,
    SPECIES_DROWZEE,
    SPECIES_MAKUHITA,
    SPECIES_SMEARGLE,
    SPECIES_CRABRAWLER,
    SPECIES_GASTLY,
    SPECIES_DRIFLOON,
    SPECIES_MURKROW,
    SPECIES_ZUBAT,
    SPECIES_NOIBAT,
    SPECIES_DIGLETT_ALOLAN,
    SPECIES_SPEAROW,
    SPECIES_RUFFLET,
    SPECIES_BRAVIARY,
    SPECIES_VULLABY,
    SPECIES_MANKEY,
    SPECIES_DELIBIRD,
    SPECIES_HAWLUCHA,
    SPECIES_ORICORIO,
    SPECIES_CUTIEFLY,
    SPECIES_FLABEBE,
    SPECIES_PETILIL,
    SPECIES_COTTONEE,
    SPECIES_PSYDUCK,
    SPECIES_SMOOCHUM,
    SPECIES_MAGIKARP,
    SPECIES_BARBOACH,
    SPECIES_SEEL,
    SPECIES_MACHOP,
    SPECIES_ROGGENROLA,
    SPECIES_CARBINK,
    SPECIES_SABLEYE,
    SPECIES_MAWILE,
    SPECIES_ROCKRUFF,
    SPECIES_SPINDA,
    SPECIES_TENTACOOL,
    SPECIES_FINNEON,
    SPECIES_WISHIWASHI,
    SPECIES_LUVDISC,
    SPECIES_CORSOLA,
    SPECIES_MAREANIE,
    SPECIES_SHELLDER,
    SPECIES_CLOYSTER,
    SPECIES_CLAMPERL,
    SPECIES_REMORAID,
    SPECIES_MANTYKE,
    SPECIES_BAGON,
    SPECIES_LILLIPUP,
    SPECIES_EEVEE,
    SPECIES_MAREEP,
    SPECIES_MUDBRAY,
    SPECIES_IGGLYBUFF,
    SPECIES_TAUROS,
    SPECIES_MILTANK,
    SPECIES_SURSKIT,
    SPECIES_DEWPIDER,
    SPECIES_FOMANTIS,
    SPECIES_MORELULL,
    SPECIES_PARAS,
    SPECIES_POLIWAG,
    SPECIES_GOLDEEN,
    SPECIES_BASCULIN,
    SPECIES_FEEBAS,
    SPECIES_MILOTIC,
    SPECIES_ALOMOMOLA,
    SPECIES_FLETCHLING,
    SPECIES_SALANDIT,
    SPECIES_CUBONE,
    SPECIES_KANGASKHAN,
    SPECIES_MAGBY,
    SPECIES_LARVESTA,
    SPECIES_STUFFUL,
    SPECIES_BOUNSWEET,
    SPECIES_COMFEY,
    SPECIES_PINSIR,
    SPECIES_HOOTHOOT,
    SPECIES_KECLEON,
    SPECIES_ORANGURU,
    SPECIES_PASSIMIAN,
    SPECIES_GOOMY,
    SPECIES_CASTFORM,
    SPECIES_WIMPOD,
    SPECIES_STARYU,
    SPECIES_SANDYGAST,
    SPECIES_OMANYTE,
    SPECIES_KABUTO,
    SPECIES_LILEEP,
    SPECIES_ANORITH,
    SPECIES_CRANIDOS,
    SPECIES_SHIELDON,
    SPECIES_ARCHEN,
    SPECIES_TIRTOUGA,
    SPECIES_TYRUNT,
    SPECIES_AMAURA,
    SPECIES_LARVITAR,
    SPECIES_PHANTUMP,
    SPECIES_NATU,
    SPECIES_NOSEPASS,
    SPECIES_PYUKUMUKU,
    SPECIES_CHINCHOU,
    SPECIES_TYPE_NULL,
    SPECIES_POIPOLE,
    SPECIES_ZYGARDE,
    SPECIES_TRUBBISH,
    SPECIES_GARBODOR,
    SPECIES_MINCCINO,
    SPECIES_PINECO,
    SPECIES_SKARMORY,
    SPECIES_DITTO,
    SPECIES_CLEFFA,
    SPECIES_ELGYEM,
    SPECIES_MINIOR,
    SPECIES_BELDUM,
    SPECIES_PORYGON,
    SPECIES_PANCHAM,
    SPECIES_KOMALA,
    SPECIES_TORKOAL,
    SPECIES_TURTONATOR,
    SPECIES_HOUNDOUR,
    SPECIES_DEDENNE,
    SPECIES_TOGEDEMARU,
    SPECIES_ELECTRIKE,
    SPECIES_ELEKID,
    SPECIES_GEODUDE_ALOLAN,
    SPECIES_SANDILE,
    SPECIES_TRAPINCH,
    SPECIES_GIBLE,
    SPECIES_BALTOY,
    SPECIES_GOLETT,
    SPECIES_KLEFKI,
    SPECIES_MIMIKYU,
    SPECIES_SHUPPET,
    SPECIES_FRILLISH,
    SPECIES_BRUXISH,
    SPECIES_DRAMPA,
    SPECIES_ABSOL,
    SPECIES_SNORUNT,
    SPECIES_SNEASEL,
    SPECIES_SANDSHREW_ALOLAN,
    SPECIES_VULPIX_ALOLAN,
    SPECIES_VANILLITE,
    SPECIES_SCRAGGY,
    SPECIES_PAWNIARD,
    SPECIES_SNUBBULL,
    SPECIES_SHELLOS,
    SPECIES_RELICANTH,
    SPECIES_DHELMISE,
    SPECIES_CARVANHA,
    SPECIES_SKRELP,
    SPECIES_CLAUNCHER,
    SPECIES_WAILMER,
    SPECIES_LAPRAS,
    SPECIES_TROPIUS,
    SPECIES_EXEGGCUTE,
    SPECIES_CORPHISH,
    SPECIES_MIENFOO,
    SPECIES_JANGMO_O,
    SPECIES_EMOLGA,
    SPECIES_SCYTHER,
    SPECIES_HERACROSS,
    SPECIES_AIPOM,
    SPECIES_LITLEO,
    SPECIES_MISDREAVUS,
    SPECIES_DRUDDIGON,
    SPECIES_LICKITUNG,
    SPECIES_RIOLU,
    SPECIES_DRATINI,
    SPECIES_AERODACTYL,
    SPECIES_TAPU_KOKO,
    SPECIES_TAPU_LELE,
    SPECIES_TAPU_BULU,
    SPECIES_TAPU_FINI,
    SPECIES_COSMOG,
    SPECIES_NIHILEGO,
    SPECIES_STAKATAKA,
    SPECIES_BLACEPHALON,
    SPECIES_BUZZWOLE,
    SPECIES_PHEROMOSA,
    SPECIES_XURKITREE,
    SPECIES_CELESTEELA,
    SPECIES_KARTANA,
    SPECIES_GUZZLORD,
    SPECIES_NECROZMA,
    SPECIES_MAGEARNA,
    SPECIES_MARSHADOW,
    SPECIES_ZERAORA,
};

static const u16 sDexSpecies_Galar[] = 
{
    SPECIES_GROOKEY,
    SPECIES_SCORBUNNY,
    SPECIES_SOBBLE,
    SPECIES_BLIPBUG,
    SPECIES_CATERPIE,
    SPECIES_GRUBBIN,
    SPECIES_HOOTHOOT,
    SPECIES_ROOKIDEE,
    SPECIES_SKWOVET,
    SPECIES_PIDOVE,
    SPECIES_NICKIT,
    SPECIES_ZIGZAGOON_GALARIAN,
    SPECIES_ZIGZAGOON,
    SPECIES_WOOLOO,
    SPECIES_LOTAD,
    SPECIES_SEEDOT,
    SPECIES_CHEWTLE,
    SPECIES_PURRLOIN,
    SPECIES_YAMPER,
    SPECIES_BUNNELBY,
    SPECIES_MINCCINO,
    SPECIES_BOUNSWEET,
    SPECIES_ODDISH,
    SPECIES_BUDEW,
    SPECIES_WINGULL,
    SPECIES_JOLTIK,
    SPECIES_ELECTRIKE,
    SPECIES_VULPIX,
    SPECIES_VULPIX_ALOLAN,
    SPECIES_GROWLITHE,
    SPECIES_VANILLITE,
    SPECIES_SWINUB,
    SPECIES_DELIBIRD,
    SPECIES_SNORUNT,
    SPECIES_BALTOY,
    SPECIES_MUDBRAY,
    SPECIES_DWEBBLE,
    SPECIES_GOLETT,
    SPECIES_MUNNA,
    SPECIES_NATU,
    SPECIES_STUFFUL,
    SPECIES_SNOVER,
    SPECIES_KRABBY,
    SPECIES_WOOPER,
    SPECIES_CORPHISH,
    SPECIES_NINCADA,
    SPECIES_TYROGUE,
    SPECIES_PANCHAM,
    SPECIES_KLINK,
    SPECIES_COMBEE,
    SPECIES_BRONZOR,
    SPECIES_RALTS,
    SPECIES_DRIFLOON,
    SPECIES_GOSSIFLEUR,
    SPECIES_CHERUBI,
    SPECIES_STUNKY,
    SPECIES_TYMPOLE,
    SPECIES_DUSKULL,
    SPECIES_MACHOP,
    SPECIES_GASTLY,
    SPECIES_MAGIKARP,
    SPECIES_GOLDEEN,
    SPECIES_REMORAID,
    SPECIES_SHELLDER,
    SPECIES_FEEBAS,
    SPECIES_BASCULIN,
    SPECIES_WISHIWASHI,
    SPECIES_PYUKUMUKU,
    SPECIES_TRUBBISH,
    SPECIES_SIZZLIPEDE,
    SPECIES_ROLYCOLY,
    SPECIES_DIGLETT,
    SPECIES_DIGLETT_ALOLAN,
    SPECIES_DRILBUR,
    SPECIES_ROGGENROLA,
    SPECIES_TIMBURR,
    SPECIES_WOOBAT,
    SPECIES_NOIBAT,
    SPECIES_ONIX,
    SPECIES_ARROKUDA,
    SPECIES_MEOWTH,
    SPECIES_MEOWTH_ALOLAN,
    SPECIES_MEOWTH_GALARIAN,
    SPECIES_MILCERY,
    SPECIES_CUTIEFLY,
    SPECIES_FERROSEED,
    SPECIES_PUMPKABOO,
    SPECIES_PICHU,
    SPECIES_EEVEE,
    SPECIES_APPLIN,
    SPECIES_ESPURR,
    SPECIES_SWIRLIX,
    SPECIES_SPRITZEE,
    SPECIES_DEWPIDER,
    SPECIES_WYNAUT,
    SPECIES_FARFETCHD,
    SPECIES_FARFETCHD_GALARIAN,
    SPECIES_CHINCHOU,
    SPECIES_CROAGUNK,
    SPECIES_SCRAGGY,
    SPECIES_STUNFISK,
    SPECIES_STUNFISK_GALARIAN,
    SPECIES_SHUCKLE,
    SPECIES_BARBOACH,
    SPECIES_SHELLOS,
    SPECIES_WIMPOD,
    SPECIES_BINACLE,
    SPECIES_CORSOLA,
    SPECIES_CORSOLA_GALARIAN,
    SPECIES_IMPIDIMP,
    SPECIES_HATENNA,
    SPECIES_SALANDIT,
    SPECIES_PAWNIARD,
    SPECIES_THROH,
    SPECIES_SAWK,
    SPECIES_KOFFING,
    SPECIES_BONSLY,
    SPECIES_CLEFFA,
    SPECIES_TOGEPI,
    SPECIES_MUNCHLAX,
    SPECIES_COTTONEE,
    SPECIES_RHYHORN,
    SPECIES_GOTHITA,
    SPECIES_SOLOSIS,
    SPECIES_KARRABLAST,
    SPECIES_SHELMET,
    SPECIES_ELGYEM,
    SPECIES_CUBCHOO,
    SPECIES_RUFFLET,
    SPECIES_VULLABY,
    SPECIES_SKORUPI,
    SPECIES_LITWICK,
    SPECIES_INKAY,
    SPECIES_SNEASEL,
    SPECIES_SABLEYE,
    SPECIES_MAWILE,
    SPECIES_MARACTUS,
    SPECIES_SIGILYPH,
    SPECIES_RIOLU,
    SPECIES_TORKOAL,
    SPECIES_MIMIKYU,
    SPECIES_CUFANT,
    SPECIES_QWILFISH,
    SPECIES_FRILLISH,
    SPECIES_MAREANIE,
    SPECIES_CRAMORANT,
    SPECIES_TOXEL,
    SPECIES_SILICOBRA,
    SPECIES_HIPPOPOTAS,
    SPECIES_DURANT,
    SPECIES_HEATMOR,
    SPECIES_HELIOPTILE,
    SPECIES_HAWLUCHA,
    SPECIES_TRAPINCH,
    SPECIES_AXEW,
    SPECIES_YAMASK,
    SPECIES_YAMASK_GALARIAN,
    SPECIES_HONEDGE,
    SPECIES_PONYTA,
    SPECIES_PONYTA_GALARIAN,
    SPECIES_SINISTEA,
    SPECIES_INDEEDEE,
    SPECIES_PHANTUMP,
    SPECIES_MORELULL,
    SPECIES_ORANGURU,
    SPECIES_PASSIMIAN,
    SPECIES_MORPEKO,
    SPECIES_FALINKS,
    SPECIES_DRAMPA,
    SPECIES_TURTONATOR,
    SPECIES_TOGEDEMARU,
    SPECIES_SNOM,
    SPECIES_CLOBBOPUS,
    SPECIES_PINCURCHIN,
    SPECIES_MANTYKE,
    SPECIES_WAILMER,
    SPECIES_BERGMITE,
    SPECIES_DHELMISE,
    SPECIES_LAPRAS,
    SPECIES_LUNATONE,
    SPECIES_SOLROCK,
    SPECIES_MIME_JR,
    SPECIES_DARUMAKA,
    SPECIES_DARUMAKA_GALARIAN,
    SPECIES_STONJOURNER,
    SPECIES_EISCUE,
    SPECIES_DURALUDON,
    SPECIES_ROTOM,
    SPECIES_DITTO,
    SPECIES_DRACOZOLT,
    SPECIES_ARCTOZOLT,
    SPECIES_DRACOVISH,
    SPECIES_ARCTOVISH,
    SPECIES_CHARMANDER,
    SPECIES_TYPE_NULL,
    SPECIES_LARVITAR,
    SPECIES_DEINO,
    SPECIES_GOOMY,
    SPECIES_JANGMO_O,
    SPECIES_DREEPY,
    SPECIES_ZACIAN,
    SPECIES_ZAMAZENTA,
    SPECIES_ETERNATUS,

    SPECIES_SLOWPOKE_GALARIAN,
    SPECIES_KUBFU,
    SPECIES_ZARUDE,
    SPECIES_REGIELEKI,
    SPECIES_REGIDRAGO,
    SPECIES_ARTICUNO_GALARIAN,
    SPECIES_ZAPDOS_GALARIAN,
    SPECIES_MOLTRES_GALARIAN,
    SPECIES_GLASTRIER,
    SPECIES_SPECTRIER,
    SPECIES_CALYREX,
};
#endif

const u16* const gRegionalDexSpecies[] = {
    sDexSpecies_Kanto,
    sDexSpecies_Johto,
    sDexSpecies_Hoenn,
#ifdef ROGUE_EXPANSION
    sDexSpecies_Sinnoh,
    sDexSpecies_Unova,
    sDexSpecies_Kalos,
    sDexSpecies_Alola,
    sDexSpecies_Galar,
#endif
};

const u16 gRegionalDexSpeciesCount[] = {
    ARRAY_COUNT(sDexSpecies_Kanto),
    ARRAY_COUNT(sDexSpecies_Johto),
    ARRAY_COUNT(sDexSpecies_Hoenn),
#ifdef ROGUE_EXPANSION
    ARRAY_COUNT(sDexSpecies_Sinnoh),
    ARRAY_COUNT(sDexSpecies_Unova),
    ARRAY_COUNT(sDexSpecies_Kalos),
    ARRAY_COUNT(sDexSpecies_Alola),
    ARRAY_COUNT(sDexSpecies_Galar),
#endif
};