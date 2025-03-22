#ifndef __POKEDEX_ASSETS__
#define __POKEDEX_ASSETS__

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <unordered_map>

class PokedexAssets
{
  public:
    // Note to self.. This order defines the order assets should be loaded into map.
    typedef enum _assetType
    {
        FONT,
        MISC,
        POKEMON_SPRITES,
        POKEMON_ICON,
        TYPES,
        ITEMS,
        POKEMON_CRY,
        SOUND_EFFECT,
    } t_assetType;

    typedef struct _asset
    {
        std::string name;
        std::string path;
        t_assetType type;
        SDL_Surface *surface    = nullptr;
        TTF_Font *font          = nullptr;
        Mix_Chunk *sound_effect = nullptr;
        SDL_Rect size;
        bool isLoaded = false;

        // constructor
        _asset() = default;
        _asset(std::string p) : path(p) {}
        _asset(std::string p, std::pair<int, int> s) : path(p), size({0, 0, s.first, s.second}) {}
        _asset(std::string p, t_assetType t, std::pair<int, int> s)
            : path(p), type(t), size({0, 0, s.first, s.second})
        {
        }
        _asset(std::string n, std::string p, t_assetType t, std::pair<int, int> s)
            : name(n), path(p), type(t), size({0, 0, s.first, s.second})
        {
        }
        _asset(std::string p, SDL_Surface *sf, SDL_Rect r, bool loadState)
            : path(p), surface(sf), size(r), isLoaded(loadState)
        {
        }
    } t_asset;

    typedef enum _assetID
    {
        //=================================================AUDIO================================================================

        /* // AUDIO */
        /* AUDIO_BACK_SOUNDEFFECT, */
        /* AUDIO_LEFT_RIGHT_SOUNDEFFECT, */
        /* AUDIO_LIST_BACK_SOUNDEFFECT, */
        /* AUDIO_LIST_START_SOUNDEFFECT, */
        /* AUDIO_UP_DOWN_SOUNDEFFECT, */
        /* AUDIO_UP_DOWN_BACK_SOUNDEFFECT, */
        /* AUDIO_SELECT_1_SOUNDEFFECT, */

        //=================================================MISC================================================================

        // MISC
        SURFACE_APP_ICON,
        SURFACE_POKEMON_GENDER_MALE_ICON,
        SURFACE_POKEMON_GENDER_FEMALE_ICON,

        //=================================================FONTS================================================================

        // FONT
        FONT_POKEMON_DPPT_S,
        FONT_POKEMON_DPPT_M,
        FONT_POKEMON_DPPT_L,
        FONT_POKEMON_ADVANCED_BATTLE_S,
        FONT_POKEMON_ADVANCED_BATTLE_M,
        FONT_POKEMON_ADVANCED_BATTLE_L,

        //=================================================SCREEN_ASSETS================================================================

        // MENU SCREEN
        SURFACE_MAIN_MENU_BACKGROUND,
        SURFACE_MENU_ITEM_BACKGROUND_DEFAULT,
        SURFACE_MENU_ITEM_BACKGROUND_SELECTED,

        // LIST SCREEN
        SURFACE_POKEDEX_BACKGROUND,
        SURFACE_POKEDEX_ITEM_BACKGROUND_DEFAULT,
        SURFACE_POKEDEX_ITEM_BACKGROUND_SELECTED,

        // POKEMON INFO SCREEEN
        SURFACE_INFO_SCREEN_BACKGROUND,

        // POKEMON STATS SCREEEN
        SURFACE_STATS_SCREEN_BACKGROUND,

        // POKEMON MOVES SCREEEN
        SURFACE_MOVES_SCREEN_BACKGROUND,

        // POKEMON LOCATION SCREEEN
        SURFACE_LOCATION_SCREEN_BACKGROUND,

        // POKEMON EVOLUTION SCREEEN
        SURFACE_EVOLUTION_SCREEN_BACKGROUND,
        SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_DEFAULT,
        SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_SELECTED,

        // POKEMON SETTINGS SCREEEN
        SURFACE_SETTINGS_SCREEN_BACKGROUND,
        SURFACE_SETTINGS_LIST_ITEM_BACKGROUND,

        //=================================================POKEMON_TYPES================================================================

        // TYPES SPRITES
        SURFACE_BUG_SPRITE,
        SURFACE_DARK_SPRITE,
        SURFACE_DRAGON_SPRITE,
        SURFACE_ELECTRIC_SPRITE,
        SURFACE_FAIRY_SPRITE,
        SURFACE_FIGHTING_SPRITE,
        SURFACE_FIRE_SPRITE,
        SURFACE_FLYING_SPRITE,
        SURFACE_GHOST_SPRITE,
        SURFACE_GRASS_SPRITE,
        SURFACE_GROUND_SPRITE,
        SURFACE_ICE_SPRITE,
        SURFACE_NORMAL_SPRITE,
        SURFACE_PHYSICAL_SPRITE,
        SURFACE_POISON_SPRITE,
        SURFACE_PSYCHIC_SPRITE,
        SURFACE_ROCK_SPRITE,
        SURFACE_SPECIAL_SPRITE,
        SURFACE_STATUS_SPRITE,
        SURFACE_STEEL_SPRITE,
        SURFACE_WATER_SPRITE,

        //=================================================ENCOUNTER_ICONS================================================================

        /* // ENCOUNTER ICONS */
        SURFACE_GENDER_FEMALE_ICON,
        SURFACE_GENDER_MALE_ICON,
        SURFACE_AFFECTION_ICON,
        SURFACE_BEAUTY_ICON,
        SURFACE_BRIDGE_SPOTS_ICON,
        SURFACE_CAVE_SPOTS_ICON,
        SURFACE_DARK_GRASS_ICON,
        SURFACE_DAY_ICON,
        SURFACE_EGG_ICON,
        SURFACE_GIFT_BIG_ICON,
        SURFACE_GIFT_EGG_ICON,
        SURFACE_GIFT_ICON,
        SURFACE_GOOD_ROD_ICON,
        SURFACE_GRASS_ICON,
        SURFACE_HAPPINESS_ICON,
        SURFACE_HEADBUTT_ICON,
        SURFACE_HELD_ICON_IV_ICON,
        SURFACE_LEVEL_UP_ICON,
        SURFACE_LOCATION_ICON,
        SURFACE_MACHINE_ICON,
        SURFACE_MORNING_ICON,
        SURFACE_NIGHT_ICON,
        SURFACE_OLD_ROD_ICON,
        SURFACE_RADAR_OFF_ICON,
        SURFACE_RADAR_ON_ICON,
        SURFACE_RADIO_HOENN_ICON,
        SURFACE_RADIO_OFF_ICON,
        SURFACE_RADIO_SINNOH_ICON,
        SURFACE_ROCK_SMASH_ICON,
        SURFACE_SEASON_AUTUMN_ICON,
        SURFACE_SEASON_SPRING_ICON,
        SURFACE_SEASON_SUMMER_ICON,
        SURFACE_SEASON_WINTER_ICON,
        SURFACE_SHED_ICON,
        SURFACE_SLOT2_EMERALD_ICON,
        SURFACE_SLOT2_FIRERED_ICON,
        SURFACE_SLOT2_LEAFGREEN_ICON,
        SURFACE_SLOT2_NONE_ICON,
        SURFACE_SLOT2_RUBY_ICON,
        SURFACE_SLOT2_SAPPHIRE_ICON,
        SURFACE_SPIN_ICON,
        SURFACE_STADIUM_SURFING_PIKACHU_ICON,
        SURFACE_STATS_ATK_ICON,
        SURFACE_STATS_DEF_ICON,
        SURFACE_SUPER_ROD_ICON,
        SURFACE_SWARM_NO_ICON,
        SURFACE_SWARM_YES_ICON,
        SURFACE_TIME_DAY_ICON,
        SURFACE_TIME_MORNING_ICON,
        SURFACE_TIME_NIGHT_ICON,
        SURFACE_TRADE_ICON,
        SURFACE_TRADE_NA_ICON,
        SURFACE_TUTOR_ICON,
        SURFACE_UNKNOWN_ICON,
        SURFACE_USE_ITEM_ICON,
        SURFACE_VOLT_TACKLE_PIKACHU_ICON,
        SURFACE_WATER_SPOTS_ICON,
        SURFACE_WATER_ICON,

        //====================================================POKEMON_SPRITES==============================================================

        // Kanto Pokémon (Generation 1) - National Dex IDs 1–151
        // =========================================================
        SURFACE_BULBASAUR_SPRITE  = 1,   // Bulbasaur
        SURFACE_IVYSAUR_SPRITE    = 2,   // Ivysaur
        SURFACE_VENUSAUR_SPRITE   = 3,   // Venusaur
        SURFACE_CHARMANDER_SPRITE = 4,   // Charmander
        SURFACE_CHARMELEON_SPRITE = 5,   // Charmeleon
        SURFACE_CHARIZARD_SPRITE  = 6,   // Charizard
        SURFACE_SQUIRTLE_SPRITE   = 7,   // Squirtle
        SURFACE_WARTORTLE_SPRITE  = 8,   // Wartortle
        SURFACE_BLASTOISE_SPRITE  = 9,   // Blastoise
        SURFACE_CATERPIE_SPRITE   = 10,  // Caterpie
        SURFACE_METAPOD_SPRITE    = 11,  // Metapod
        SURFACE_BUTTERFREE_SPRITE = 12,  // Butterfree
        SURFACE_WEEDLE_SPRITE     = 13,  // Weedle
        SURFACE_KAKUNA_SPRITE     = 14,  // Kakuna
        SURFACE_BEEDRILL_SPRITE   = 15,  // Beedrill
        SURFACE_PIDGEY_SPRITE     = 16,  // Pidgey
        SURFACE_PIDGEOTTO_SPRITE  = 17,  // Pidgeotto
        SURFACE_PIDGEOT_SPRITE    = 18,  // Pidgeot
        SURFACE_RATTATA_SPRITE    = 19,  // Rattata
        SURFACE_RATICATE_SPRITE   = 20,  // Raticate
        SURFACE_SPEAROW_SPRITE    = 21,  // Spearow
        SURFACE_FEAROW_SPRITE     = 22,  // Fearow
        SURFACE_EKANS_SPRITE      = 23,  // Ekans
        SURFACE_ARBOK_SPRITE      = 24,  // Arbok
        SURFACE_PIKACHU_SPRITE    = 25,  // Pikachu
        SURFACE_RAICHU_SPRITE     = 26,  // Raichu
        SURFACE_SANDSHREW_SPRITE  = 27,  // Sandshrew
        SURFACE_SANDSLASH_SPRITE  = 28,  // Sandslash
        SURFACE_NIDORAN_F_SPRITE  = 29,  // Nidoran♀
        SURFACE_NIDORINA_SPRITE   = 30,  // Nidorina
        SURFACE_NIDOQUEEN_SPRITE  = 31,  // Nidoqueen
        SURFACE_NIDORAN_M_SPRITE  = 32,  // Nidoran♂
        SURFACE_NIDORINO_SPRITE   = 33,  // Nidorino
        SURFACE_NIDOKING_SPRITE   = 34,  // Nidoking
        SURFACE_CLEFAIRY_SPRITE   = 35,  // Clefairy
        SURFACE_CLEFABLE_SPRITE   = 36,  // Clefable
        SURFACE_VULPIX_SPRITE     = 37,  // Vulpix
        SURFACE_NINETALES_SPRITE  = 38,  // Ninetales
        SURFACE_JIGGLYPUFF_SPRITE = 39,  // Jigglypuff
        SURFACE_WIGGLYTUFF_SPRITE = 40,  // Wigglytuff
        SURFACE_ZUBAT_SPRITE      = 41,  // Zubat
        SURFACE_GOLBAT_SPRITE     = 42,  // Golbat
        SURFACE_ODDISH_SPRITE     = 43,  // Oddish
        SURFACE_GLOOM_SPRITE      = 44,  // Gloom
        SURFACE_VILEPLUME_SPRITE  = 45,  // Vileplume
        SURFACE_PARAS_SPRITE      = 46,  // Paras
        SURFACE_PARASECT_SPRITE   = 47,  // Parasect
        SURFACE_VENONAT_SPRITE    = 48,  // Venonat
        SURFACE_VENOMOTH_SPRITE   = 49,  // Venomoth
        SURFACE_DIGLETT_SPRITE    = 50,  // Diglett
        SURFACE_DUGTRIO_SPRITE    = 51,  // Dugtrio
        SURFACE_MEOWTH_SPRITE     = 52,  // Meowth
        SURFACE_PERSIAN_SPRITE    = 53,  // Persian
        SURFACE_PSYDUCK_SPRITE    = 54,  // Psyduck
        SURFACE_GOLDUCK_SPRITE    = 55,  // Golduck
        SURFACE_MANKEY_SPRITE     = 56,  // Mankey
        SURFACE_PRIMEAPE_SPRITE   = 57,  // Primeape
        SURFACE_GROWLITHE_SPRITE  = 58,  // Growlithe
        SURFACE_ARCANINE_SPRITE   = 59,  // Arcanine
        SURFACE_POLIWAG_SPRITE    = 60,  // Poliwag
        SURFACE_POLIWHIRL_SPRITE  = 61,  // Poliwhirl
        SURFACE_POLIWRATH_SPRITE  = 62,  // Poliwrath
        SURFACE_ABRA_SPRITE       = 63,  // Abra
        SURFACE_KADABRA_SPRITE    = 64,  // Kadabra
        SURFACE_ALAKAZAM_SPRITE   = 65,  // Alakazam
        SURFACE_MACHOP_SPRITE     = 66,  // Machop
        SURFACE_MACHOKE_SPRITE    = 67,  // Machoke
        SURFACE_MACHAMP_SPRITE    = 68,  // Machamp
        SURFACE_BELLSPROUT_SPRITE = 69,  // Bellsprout
        SURFACE_WEEPINBELL_SPRITE = 70,  // Weepinbell
        SURFACE_VICTREEBEL_SPRITE = 71,  // Victreebel
        SURFACE_TENTACOOL_SPRITE  = 72,  // Tentacool
        SURFACE_TENTACRUEL_SPRITE = 73,  // Tentacruel
        SURFACE_GEODUDE_SPRITE    = 74,  // Geodude
        SURFACE_GRAVELER_SPRITE   = 75,  // Graveler
        SURFACE_GOLEM_SPRITE      = 76,  // Golem
        SURFACE_PONYTA_SPRITE     = 77,  // Ponyta
        SURFACE_RAPIDASH_SPRITE   = 78,  // Rapidash
        SURFACE_SLOWPOKE_SPRITE   = 79,  // Slowpoke
        SURFACE_SLOWBRO_SPRITE    = 80,  // Slowbro
        SURFACE_MAGNEMITE_SPRITE  = 81,  // Magnemite
        SURFACE_MAGNETON_SPRITE   = 82,  // Magneton
        SURFACE_FARFETCHD_SPRITE  = 83,  // Farfetch'd
        SURFACE_DODUO_SPRITE      = 84,  // Doduo
        SURFACE_DODRIO_SPRITE     = 85,  // Dodrio
        SURFACE_SEEL_SPRITE       = 86,  // Seel
        SURFACE_DEWGONG_SPRITE    = 87,  // Dewgong
        SURFACE_GRIMER_SPRITE     = 88,  // Grimer
        SURFACE_MUK_SPRITE        = 89,  // Muk
        SURFACE_SHELLDER_SPRITE   = 90,  // Shellder
        SURFACE_CLOYSTER_SPRITE   = 91,  // Cloyster
        SURFACE_GASTLY_SPRITE     = 92,  // Gastly
        SURFACE_HAUNTER_SPRITE    = 93,  // Haunter
        SURFACE_GENGAR_SPRITE     = 94,  // Gengar
        SURFACE_ONIX_SPRITE       = 95,  // Onix
        SURFACE_DROWZEE_SPRITE    = 96,  // Drowzee
        SURFACE_HYPNO_SPRITE      = 97,  // Hypno
        SURFACE_KRABBY_SPRITE     = 98,  // Krabby
        SURFACE_KINGLER_SPRITE    = 99,  // Kingler
        SURFACE_VOLTORB_SPRITE    = 100, // Voltorb
        SURFACE_ELECTRODE_SPRITE  = 101, // Electrode
        SURFACE_EXEGGCUTE_SPRITE  = 102, // Exeggcute
        SURFACE_EXEGGUTOR_SPRITE  = 103, // Exeggutor
        SURFACE_CUBONE_SPRITE     = 104, // Cubone
        SURFACE_MAROWAK_SPRITE    = 105, // Marowak
        SURFACE_HITMONLEE_SPRITE  = 106, // Hitmonlee
        SURFACE_HITMONCHAN_SPRITE = 107, // Hitmonchan
        SURFACE_LICKITUNG_SPRITE  = 108, // Lickitung
        SURFACE_KOFFING_SPRITE    = 109, // Koffing
        SURFACE_WEEZING_SPRITE    = 110, // Weezing
        SURFACE_RHYHORN_SPRITE    = 111, // Rhyhorn
        SURFACE_RHYDON_SPRITE     = 112, // Rhydon
        SURFACE_CHANSEY_SPRITE    = 113, // Chansey
        SURFACE_TANGELA_SPRITE    = 114, // Tangela
        SURFACE_KANGASKHAN_SPRITE = 115, // Kangaskhan
        SURFACE_HORSEA_SPRITE     = 116, // Horsea
        SURFACE_SEADRA_SPRITE     = 117, // Seadra
        SURFACE_GOLDEEN_SPRITE    = 118, // Goldeen
        SURFACE_SEAKING_SPRITE    = 119, // Seaking
        SURFACE_STARYU_SPRITE     = 120, // Staryu
        SURFACE_STARMIE_SPRITE    = 121, // Starmie
        SURFACE_MRMIME_SPRITE     = 122, // Mr. Mime
        SURFACE_SCYTHER_SPRITE    = 123, // Scyther
        SURFACE_JYNX_SPRITE       = 124, // Jynx
        SURFACE_ELECTABUZZ_SPRITE = 125, // Electabuzz
        SURFACE_MAGMAR_SPRITE     = 126, // Magmar
        SURFACE_PINSIR_SPRITE     = 127, // Pinsir
        SURFACE_TAUROS_SPRITE     = 128, // Tauros
        SURFACE_MAGIKARP_SPRITE   = 129, // Magikarp
        SURFACE_GYARADOS_SPRITE   = 130, // Gyarados
        SURFACE_LAPRAS_SPRITE     = 131, // Lapras
        SURFACE_DITTO_SPRITE      = 132, // Ditto
        SURFACE_EEVEE_SPRITE      = 133, // Eevee
        SURFACE_VAPOREON_SPRITE   = 134, // Vaporeon
        SURFACE_JOLTEON_SPRITE    = 135, // Jolteon
        SURFACE_FLAREON_SPRITE    = 136, // Flareon
        SURFACE_PORYGON_SPRITE    = 137, // Porygon
        SURFACE_OMANYTE_SPRITE    = 138, // Omanyte
        SURFACE_OMASTAR_SPRITE    = 139, // Omastar
        SURFACE_KABUTO_SPRITE     = 140, // Kabuto
        SURFACE_KABUTOPS_SPRITE   = 141, // Kabutops
        SURFACE_AERODACTYL_SPRITE = 142, // Aerodactyl
        SURFACE_SNORLAX_SPRITE    = 143, // Snorlax
        SURFACE_ARTICUNO_SPRITE   = 144, // Articuno
        SURFACE_ZAPDOS_SPRITE     = 145, // Zapdos
        SURFACE_MOLTRES_SPRITE    = 146, // Moltres
        SURFACE_DRATINI_SPRITE    = 147, // Dratini
        SURFACE_DRAGONAIR_SPRITE  = 148, // Dragonair
        SURFACE_DRAGONITE_SPRITE  = 149, // Dragonite
        SURFACE_MEWTWO_SPRITE     = 150, // Mewtwo
        SURFACE_MEW_SPRITE        = 151, // Mew

        // Johto Pokémon (Generation 2) - National Dex IDs 152–251
        // =========================================================
        SURFACE_CHIKORITA_SPRITE  = 152, // Chikorita
        SURFACE_BAYLEEF_SPRITE    = 153, // Bayleef
        SURFACE_MEGANIUM_SPRITE   = 154, // Meganium
        SURFACE_CYNDAQUIL_SPRITE  = 155, // Cyndaquil
        SURFACE_QUILAVA_SPRITE    = 156, // Quilava
        SURFACE_TYPHLOSION_SPRITE = 157, // Typhlosion
        SURFACE_TOTODILE_SPRITE   = 158, // Totodile
        SURFACE_CROCONAW_SPRITE   = 159, // Croconaw
        SURFACE_FERALIGATR_SPRITE = 160, // Feraligatr
        SURFACE_SENTRET_SPRITE    = 161, // Sentret
        SURFACE_FURRET_SPRITE     = 162, // Furret
        SURFACE_HOOTHOOT_SPRITE   = 163, // Hoothoot
        SURFACE_NOCTOWL_SPRITE    = 164, // Noctowl
        SURFACE_LEDYBA_SPRITE     = 165, // Ledyba
        SURFACE_LEDIAN_SPRITE     = 166, // Ledian
        SURFACE_SPINARAK_SPRITE   = 167, // Spinarak
        SURFACE_ARIADOS_SPRITE    = 168, // Ariados
        SURFACE_CROBAT_SPRITE     = 169, // Crobat
        SURFACE_CHINCHOU_SPRITE   = 170, // Chinchou
        SURFACE_LANTURN_SPRITE    = 171, // Lanturn
        SURFACE_PICHU_SPRITE      = 172, // Pichu
        SURFACE_CLEFFA_SPRITE     = 173, // Cleffa
        SURFACE_IGGLYBUFF_SPRITE  = 174, // Igglybuff
        SURFACE_TOGEPI_SPRITE     = 175, // Togepi
        SURFACE_TOGETIC_SPRITE    = 176, // Togetic
        SURFACE_NATU_SPRITE       = 177, // Natu
        SURFACE_XATU_SPRITE       = 178, // Xatu
        SURFACE_MAREEP_SPRITE     = 179, // Mareep
        SURFACE_FLAAFFY_SPRITE    = 180, // Flaaffy
        SURFACE_AMPHAROS_SPRITE   = 181, // Ampharos
        SURFACE_BELLOSSOM_SPRITE  = 182, // Bellossom
        SURFACE_MARILL_SPRITE     = 183, // Marill
        SURFACE_AZUMARILL_SPRITE  = 184, // Azumarill
        SURFACE_SUDOWOODO_SPRITE  = 185, // Sudowoodo
        SURFACE_POLITOED_SPRITE   = 186, // Politoed
        SURFACE_HOPPIP_SPRITE     = 187, // Hoppip
        SURFACE_SKIPLOOM_SPRITE   = 188, // Skiploom
        SURFACE_JUMPLUFF_SPRITE   = 189, // Jumpluff
        SURFACE_AIPOM_SPRITE      = 190, // Aipom
        SURFACE_SUNKERN_SPRITE    = 191, // Sunkern
        SURFACE_SUNFLORA_SPRITE   = 192, // Sunflora
        SURFACE_YANMA_SPRITE      = 193, // Yanma
        SURFACE_WOOPER_SPRITE     = 194, // Wooper
        SURFACE_QUAGSIRE_SPRITE   = 195, // Quagsire
        SURFACE_ESPEON_SPRITE     = 196, // Espeon
        SURFACE_UMBREON_SPRITE    = 197, // Umbreon
        SURFACE_MURKROW_SPRITE    = 198, // Murkrow
        SURFACE_SLOWKING_SPRITE   = 199, // Slowking
        SURFACE_MISDREAVUS_SPRITE = 200, // Misdreavus
        SURFACE_UNOWN_SPRITE      = 201, // Unown
        SURFACE_WOBBUFFET_SPRITE  = 202, // Wobbuffet
        SURFACE_GIRAFARIG_SPRITE  = 203, // Girafarig
        SURFACE_PINECO_SPRITE     = 204, // Pineco
        SURFACE_FORRETRESS_SPRITE = 205, // Forretress
        SURFACE_DUNSPARCE_SPRITE  = 206, // Dunsparce
        SURFACE_GLIGAR_SPRITE     = 207, // Gligar
        SURFACE_STEELIX_SPRITE    = 208, // Steelix
        SURFACE_SNUBBULL_SPRITE   = 209, // Snubbull
        SURFACE_GRANBULL_SPRITE   = 210, // Granbull
        SURFACE_QWILFISH_SPRITE   = 211, // Qwilfish
        SURFACE_SCIZOR_SPRITE     = 212, // Scizor
        SURFACE_SHUCKLE_SPRITE    = 213, // Shuckle
        SURFACE_HERACROSS_SPRITE  = 214, // Heracross
        SURFACE_SNEASEL_SPRITE    = 215, // Sneasel
        SURFACE_TEDDIURSA_SPRITE  = 216, // Teddiursa
        SURFACE_URSARING_SPRITE   = 217, // Ursaring
        SURFACE_SLUGMA_SPRITE     = 218, // Slugma
        SURFACE_MAGCARGO_SPRITE   = 219, // Magcargo
        SURFACE_SWINUB_SPRITE     = 220, // Swinub
        SURFACE_PILOSWINE_SPRITE  = 221, // Piloswine
        SURFACE_CORSOLA_SPRITE    = 222, // Corsola
        SURFACE_REMORAID_SPRITE   = 223, // Remoraid
        SURFACE_OCTILLERY_SPRITE  = 224, // Octillery
        SURFACE_DELIBIRD_SPRITE   = 225, // Delibird
        SURFACE_MANTINE_SPRITE    = 226, // Mantine
        SURFACE_SKARMORY_SPRITE   = 227, // Skarmory
        SURFACE_HOUNDOUR_SPRITE   = 228, // Houndour
        SURFACE_HOUNDOOM_SPRITE   = 229, // Houndoom
        SURFACE_KINGDRA_SPRITE    = 230, // Kingdra
        SURFACE_PHANPY_SPRITE     = 231, // Phanpy
        SURFACE_DONPHAN_SPRITE    = 232, // Donphan
        SURFACE_PORYGON2_SPRITE   = 233, // Porygon2
        SURFACE_STANTLER_SPRITE   = 234, // Stantler
        SURFACE_SMEARGLE_SPRITE   = 235, // Smeargle
        SURFACE_TYROGUE_SPRITE    = 236, // Tyrogue
        SURFACE_HITMONTOP_SPRITE  = 237, // Hitmontop
        SURFACE_SMOOCHUM_SPRITE   = 238, // Smoochum
        SURFACE_ELEKID_SPRITE     = 239, // Elekid
        SURFACE_MAGBY_SPRITE      = 240, // Magby
        SURFACE_MILTANK_SPRITE    = 241, // Miltank
        SURFACE_BLISSEY_SPRITE    = 242, // Blissey
        SURFACE_RAIKOU_SPRITE     = 243, // Raikou
        SURFACE_ENTEI_SPRITE      = 244, // Entei
        SURFACE_SUICUNE_SPRITE    = 245, // Suicune
        SURFACE_LARVITAR_SPRITE   = 246, // Larvitar
        SURFACE_PUPITAR_SPRITE    = 247, // Pupitar
        SURFACE_TYRANITAR_SPRITE  = 248, // Tyranitar
        SURFACE_LUGIA_SPRITE      = 249, // Lugia
        SURFACE_HOOH_SPRITE       = 250, // Ho-Oh
        SURFACE_CELEBI_SPRITE     = 251, // Celebi

        // Hoenn Pokémon (Generation 3) - National Dex IDs 252–386
        // =========================================================
        SURFACE_TREECKO_SPRITE       = 252, // Treecko
        SURFACE_GROVYLE_SPRITE       = 253, // Grovyle
        SURFACE_SCEPTILE_SPRITE      = 254, // Sceptile
        SURFACE_TORCHIC_SPRITE       = 255, // Torchic
        SURFACE_COMBUSKEN_SPRITE     = 256, // Combusken
        SURFACE_BLAZIKEN_SPRITE      = 257, // Blaziken
        SURFACE_MUDKIP_SPRITE        = 258, // Mudkip
        SURFACE_MARSHTOMP_SPRITE     = 259, // Marshtomp
        SURFACE_SWAMPERT_SPRITE      = 260, // Swampert
        SURFACE_POOCHYENA_SPRITE     = 261, // Poochyena
        SURFACE_MIGHTYENA_SPRITE     = 262, // Mightyena
        SURFACE_ZIGZAGOON_SPRITE     = 263, // Zigzagoon
        SURFACE_LINOONE_SPRITE       = 264, // Linoone
        SURFACE_WURMPLE_SPRITE       = 265, // Wurmple
        SURFACE_SILCOON_SPRITE       = 266, // Silcoon
        SURFACE_BEAUTIFLY_SPRITE     = 267, // Beautifly
        SURFACE_CASCOON_SPRITE       = 268, // Cascoon
        SURFACE_DUSTOX_SPRITE        = 269, // Dustox
        SURFACE_LOTAD_SPRITE         = 270, // Lotad
        SURFACE_LOMBRE_SPRITE        = 271, // Lombre
        SURFACE_LUDICOLO_SPRITE      = 272, // Ludicolo
        SURFACE_SEEDOT_SPRITE        = 273, // Seedot
        SURFACE_NUZLEAF_SPRITE       = 274, // Nuzleaf
        SURFACE_SHIFTRY_SPRITE       = 275, // Shiftry
        SURFACE_TAILLOW_SPRITE       = 276, // Taillow
        SURFACE_SWELLOW_SPRITE       = 277, // Swellow
        SURFACE_WINGULL_SPRITE       = 278, // Wingull
        SURFACE_PELIPPER_SPRITE      = 279, // Pelipper
        SURFACE_RALTS_SPRITE         = 280, // Ralts
        SURFACE_KIRLIA_SPRITE        = 281, // Kirlia
        SURFACE_GARDEVOIR_SPRITE     = 282, // Gardevoir
        SURFACE_SURSKIT_SPRITE       = 283, // Surskit
        SURFACE_MASQUERAIN_SPRITE    = 284, // Masquerain
        SURFACE_SHROOMISH_SPRITE     = 285, // Shroomish
        SURFACE_BRELOOM_SPRITE       = 286, // Breloom
        SURFACE_SLAKOTH_SPRITE       = 287, // Slakoth
        SURFACE_VIGOROTH_SPRITE      = 288, // Vigoroth
        SURFACE_SLAKING_SPRITE       = 289, // Slaking
        SURFACE_NINCADA_SPRITE       = 290, // Nincada
        SURFACE_NINJASK_SPRITE       = 291, // Ninjask
        SURFACE_SHEDINJA_SPRITE      = 292, // Shedinja
        SURFACE_WHISMUR_SPRITE       = 293, // Whismur
        SURFACE_LOUDRED_SPRITE       = 294, // Loudred
        SURFACE_EXPLOUD_SPRITE       = 295, // Exploud
        SURFACE_MAKUHITA_SPRITE      = 296, // Makuhita
        SURFACE_HARIYAMA_SPRITE      = 297, // Hariyama
        SURFACE_AZURILL_SPRITE       = 298, // Azurill
        SURFACE_NOSEPASS_SPRITE      = 299, // Nosepass
        SURFACE_SKITTY_SPRITE        = 300, // Skitty
        SURFACE_DELCATTY_SPRITE      = 301, // Delcatty
        SURFACE_SABLEYE_SPRITE       = 302, // Sableye
        SURFACE_MAWILE_SPRITE        = 303, // Mawile
        SURFACE_ARON_SPRITE          = 304, // Aron
        SURFACE_LAIRON_SPRITE        = 305, // Lairon
        SURFACE_AGGRON_SPRITE        = 306, // Aggron
        SURFACE_MEDITITE_SPRITE      = 307, // Meditite
        SURFACE_MEDICHAM_SPRITE      = 308, // Medicham
        SURFACE_ELECTRIKE_SPRITE     = 309, // Electrike
        SURFACE_MANECTRIC_SPRITE     = 310, // Manectric
        SURFACE_PLUSLE_SPRITE        = 311, // Plusle
        SURFACE_MINUN_SPRITE         = 312, // Minun
        SURFACE_VOLBEAT_SPRITE       = 313, // Volbeat
        SURFACE_ILLUMISE_SPRITE      = 314, // Illumise
        SURFACE_ROSELIA_SPRITE       = 315, // Roselia
        SURFACE_GULPIN_SPRITE        = 316, // Gulpin
        SURFACE_SWALOT_SPRITE        = 317, // Swalot
        SURFACE_CARVANHA_SPRITE      = 318, // Carvanha
        SURFACE_SHARPEDO_SPRITE      = 319, // Sharpedo
        SURFACE_WAILMER_SPRITE       = 320, // Wailmer
        SURFACE_WAILORD_SPRITE       = 321, // Wailord
        SURFACE_NUMEL_SPRITE         = 322, // Numel
        SURFACE_CAMERUPT_SPRITE      = 323, // Camerupt
        SURFACE_TORKOAL_SPRITE       = 324, // Torkal
        SURFACE_SPOINK_SPRITE        = 325, // Spoink
        SURFACE_GRUMPIG_SPRITE       = 326, // Grumpig
        SURFACE_SPINDA_SPRITE        = 327, // Spinda
        SURFACE_TRAPINCH_SPRITE      = 328, // Trapinch
        SURFACE_VIBRAVA_SPRITE       = 329, // Vibrava
        SURFACE_FLYGON_SPRITE        = 330, // Flygon
        SURFACE_CACNEA_SPRITE        = 331, // Cacnea
        SURFACE_CACTURNE_SPRITE      = 332, // Cacturne
        SURFACE_SWABLU_SPRITE        = 333, // Swablu
        SURFACE_ALTARIA_SPRITE       = 334, // Altaria
        SURFACE_ZANGOOSE_SPRITE      = 335, // Zangoose
        SURFACE_SEVIPER_SPRITE       = 336, // Seviper
        SURFACE_LUNATONE_SPRITE      = 337, // Lunatone
        SURFACE_SOLROCK_SPRITE       = 338, // Solrock
        SURFACE_BARBOACH_SPRITE      = 339, // Barboach
        SURFACE_WHISCASH_SPRITE      = 340, // Whiscash
        SURFACE_CORPHISH_SPRITE      = 341, // Corphish
        SURFACE_CRAWDAUNT_SPRITE     = 342, // Crawdaunt
        SURFACE_BALTOY_SPRITE        = 343, // Baltoy
        SURFACE_CLAYDOL_SPRITE       = 344, // Claydol
        SURFACE_LILEEP_SPRITE        = 345, // Lileep
        SURFACE_CRADILY_SPRITE       = 346, // Cradily
        SURFACE_ANORITH_SPRITE       = 347, // Anorith
        SURFACE_ARMALDO_SPRITE       = 348, // Armaldo
        SURFACE_FEEBAS_SPRITE        = 349, // Feebas
        SURFACE_MILOTIC_SPRITE       = 350, // Milotic
        SURFACE_CASTFORM_SPRITE      = 351, // Castform
        SURFACE_KECLEON_SPRITE       = 352, // Kecleon
        SURFACE_SHUPPET_SPRITE       = 353, // Shuppet
        SURFACE_BANETTE_SPRITE       = 354, // Banette
        SURFACE_DUSKULL_SPRITE       = 355, // Duskull
        SURFACE_DUSCLOPS_SPRITE      = 356, // Dusclops
        SURFACE_TROPIUS_SPRITE       = 357, // Tropius
        SURFACE_CHIMECHO_SPRITE      = 358, // Chimecho
        SURFACE_ABSOL_SPRITE         = 359, // Absol
        SURFACE_WYNAUT_SPRITE        = 360, // Wynaut
        SURFACE_SNORUNT_SPRITE       = 361, // Snorunt
        SURFACE_GLALIE_SPRITE        = 362, // Glalie
        SURFACE_SPHEAL_SPRITE        = 363, // Spheal
        SURFACE_SEALEO_SPRITE        = 364, // Sealeo
        SURFACE_WALREIN_SPRITE       = 365, // Walrein
        SURFACE_CLAMPERL_SPRITE      = 366, // Clamperl
        SURFACE_HUNTAIL_SPRITE       = 367, // Huntail
        SURFACE_GOREBYSS_SPRITE      = 368, // Gorebyss
        SURFACE_RELICANTH_SPRITE     = 369, // Relicanth
        SURFACE_LUVDISC_SPRITE       = 370, // Luvdisc
        SURFACE_BAGON_SPRITE         = 371, // Bagon
        SURFACE_SHELGON_SPRITE       = 372, // Shelgon
        SURFACE_SALAMENCE_SPRITE     = 373, // Salamence
        SURFACE_BELDUM_SPRITE        = 374, // Beldum
        SURFACE_METANG_SPRITE        = 375, // Metang
        SURFACE_METAGROSS_SPRITE     = 376, // Metagross
        SURFACE_REGIROCK_SPRITE      = 377, // Regirock
        SURFACE_REGICE_SPRITE        = 378, // Regice
        SURFACE_REGISTEEL_SPRITE     = 379, // Registeel
        SURFACE_LATIAS_SPRITE        = 380, // Latias
        SURFACE_LATIOS_SPRITE        = 381, // Latios
        SURFACE_KYOGRE_SPRITE        = 382, // Kyogre
        SURFACE_GROUDON_SPRITE       = 383, // Groudon
        SURFACE_RAYQUAZA_SPRITE      = 384, // Rayquaza
        SURFACE_JIRACHI_SPRITE       = 385, // Jirachi
        SURFACE_DEOXYS_NORMAL_SPRITE = 386, // Deoxys

        // Sinnoh Pokémon (Generation 4) - National Dex IDs 387–493
        // =========================================================
        SURFACE_TURTWIG_SPRITE        = 387, // Turtwig
        SURFACE_GROTLE_SPRITE         = 388, // Grotle
        SURFACE_TORTERRA_SPRITE       = 389, // Torterra
        SURFACE_CHIMCHAR_SPRITE       = 390, // Chimchar
        SURFACE_MONFERNO_SPRITE       = 391, // Monferno
        SURFACE_INFERNAPE_SPRITE      = 392, // Infernape
        SURFACE_PIPLUP_SPRITE         = 393, // Piplup
        SURFACE_PRINPLUP_SPRITE       = 394, // Prinplup
        SURFACE_EMPOLEON_SPRITE       = 395, // Empoleon
        SURFACE_STARLY_SPRITE         = 396, // Starly
        SURFACE_STARAVIA_SPRITE       = 397, // Staravia
        SURFACE_STARAPTOR_SPRITE      = 398, // Staraptor
        SURFACE_BIDOOF_SPRITE         = 399, // Bidoof
        SURFACE_BIBAREL_SPRITE        = 400, // Bibarel
        SURFACE_KRICKETOT_SPRITE      = 401, // Kricketot
        SURFACE_KRICKETUNE_SPRITE     = 402, // Kricketune
        SURFACE_SHINX_SPRITE          = 403, // Shinx
        SURFACE_LUXIO_SPRITE          = 404, // Luxio
        SURFACE_LUXRAY_SPRITE         = 405, // Luxray
        SURFACE_BUDEW_SPRITE          = 406, // Budew
        SURFACE_ROSERADE_SPRITE       = 407, // Roserade
        SURFACE_CRANIDOS_SPRITE       = 408, // Cranidos
        SURFACE_RAMPARDOS_SPRITE      = 409, // Rampardos
        SURFACE_SHIELDON_SPRITE       = 410, // Shieldon
        SURFACE_BASTIODON_SPRITE      = 411, // Bastiodon
        SURFACE_BURMY_SPRITE          = 412, // Burmy
        SURFACE_WORMADAM_PLANT_SPRITE = 413, // Wormadam
        SURFACE_MOTHIM_SPRITE         = 414, // Mothim
        SURFACE_COMBEE_SPRITE         = 415, // Combee
        SURFACE_VESPIQUEN_SPRITE      = 416, // Vespiquen
        SURFACE_PACHIRISU_SPRITE      = 417, // Pachirisu
        SURFACE_BUIZEL_SPRITE         = 418, // Buizel
        SURFACE_FLOATZEL_SPRITE       = 419, // Floatzel
        SURFACE_CHERUBI_SPRITE        = 420, // Cherubi
        SURFACE_CHERRIM_SPRITE        = 421, // Cherrim
        SURFACE_SHELLOS_SPRITE        = 422, // Shellos
        SURFACE_GASTRODON_SPRITE      = 423, // Gastrodon
        SURFACE_AMBIPOM_SPRITE        = 424, // Ambipom
        SURFACE_DRIFLOON_SPRITE       = 425, // Drifloon
        SURFACE_DRIFBLIM_SPRITE       = 426, // Drifblim
        SURFACE_BUNEARY_SPRITE        = 427, // Buneary
        SURFACE_LOPUNNY_SPRITE        = 428, // Lopunny
        SURFACE_MISMAGIUS_SPRITE      = 429, // Mismagius
        SURFACE_HONCHKROW_SPRITE      = 430, // Honchkrow
        SURFACE_GLAMEOW_SPRITE        = 431, // Glameow
        SURFACE_PURUGLY_SPRITE        = 432, // Purugly
        SURFACE_CHINGLING_SPRITE      = 433, // Chingling
        SURFACE_STUNKY_SPRITE         = 434, // Stunky
        SURFACE_SKUNTANK_SPRITE       = 435, // Skuntank
        SURFACE_BRONZOR_SPRITE        = 436, // Bronzor
        SURFACE_BRONZONG_SPRITE       = 437, // Bronzong
        SURFACE_BONSLY_SPRITE         = 438, // Bonsly
        SURFACE_MIMEJR_SPRITE         = 439, // Mime Jr.
        SURFACE_HAPPINY_SPRITE        = 440, // Happiny
        SURFACE_CHATOT_SPRITE         = 441, // Chatot
        SURFACE_SPIRITOMB_SPRITE      = 442, // Spiritomb
        SURFACE_GIBLE_SPRITE          = 443, // Gible
        SURFACE_GABITE_SPRITE         = 444, // Gabite
        SURFACE_GARCHOMP_SPRITE       = 445, // Garchomp
        SURFACE_MUNCHLAX_SPRITE       = 446, // Munchlax
        SURFACE_RIOLU_SPRITE          = 447, // Riolu
        SURFACE_LUCARIO_SPRITE        = 448, // Lucario
        SURFACE_HIPPOPOTAS_SPRITE     = 449, // Hippopotas
        SURFACE_HIPPOWDON_SPRITE      = 450, // Hippowdon
        SURFACE_SKORUPI_SPRITE        = 451, // Skorupi
        SURFACE_DRAPION_SPRITE        = 452, // Drapion
        SURFACE_CROAGUNK_SPRITE       = 453, // Croagunk
        SURFACE_TOXICROAK_SPRITE      = 454, // Toxicroak
        SURFACE_CARNIVINE_SPRITE      = 455, // Carnivine
        SURFACE_FINNEON_SPRITE        = 456, // Finneon
        SURFACE_LUMINEON_SPRITE       = 457, // Lumineon
        SURFACE_MANTYKE_SPRITE        = 458, // Mantyke
        SURFACE_SNOVER_SPRITE         = 459, // Snover
        SURFACE_ABOMASNOW_SPRITE      = 460, // Abomasnow
        SURFACE_WEAVILE_SPRITE        = 461, // Weavile
        SURFACE_MAGNEZONE_SPRITE      = 462, // Magnezone
        SURFACE_LICKILICKY_SPRITE     = 463, // Lickilicky
        SURFACE_RHYPERIOR_SPRITE      = 464, // Rhyperior
        SURFACE_TANGROWTH_SPRITE      = 465, // Tangrowth
        SURFACE_ELECTIVIRE_SPRITE     = 466, // Electivire
        SURFACE_MAGMORTAR_SPRITE      = 467, // Magmortar
        SURFACE_TOGEKISS_SPRITE       = 468, // Togekiss
        SURFACE_YANMEGA_SPRITE        = 469, // Yanmega
        SURFACE_LEAFEON_SPRITE        = 470, // Leafeon
        SURFACE_GLACEON_SPRITE        = 471, // Glaceon
        SURFACE_GLISCOR_SPRITE        = 472, // Gliscor
        SURFACE_MAMOSWINE_SPRITE      = 473, // Mamoswine
        SURFACE_PORYGON_Z_SPRITE      = 474, // Porygon-Z
        SURFACE_GALLADE_SPRITE        = 475, // Gallade
        SURFACE_PROBOPASS_SPRITE      = 476, // Probopass
        SURFACE_DUSKNOIR_SPRITE       = 477, // Dusknoir
        SURFACE_FROSLASS_SPRITE       = 478, // Froslass
        SURFACE_ROTOM_SPRITE          = 479, // Rotom
        SURFACE_UXIE_SPRITE           = 480, // Uxie
        SURFACE_MESPRIT_SPRITE        = 481, // Mesprit
        SURFACE_AZELF_SPRITE          = 482, // Azelf
        SURFACE_DIALGA_SPRITE         = 483, // Dialga
        SURFACE_PALKIA_SPRITE         = 484, // Palkia
        SURFACE_HEATRAN_SPRITE        = 485, // Heatran
        SURFACE_REGIGIGAS_SPRITE      = 486, // Regigigas
        SURFACE_GIRATINA_SPRITE       = 487, // Giratina
        SURFACE_CRESSELIA_SPRITE      = 488, // Cresselia
        SURFACE_PHIONE_SPRITE         = 489, // Phione
        SURFACE_MANAPHY_SPRITE        = 490, // Manaphy
        SURFACE_DARKRAI_SPRITE        = 491, // Darkrai
        SURFACE_SHAYMIN_LAND_SPRITE   = 492, // Shaymin
        SURFACE_ARCEUS_SPRITE         = 493, // Arceus

        // Unova Pokémon (Generation 5) - National Dex IDs 494–649
        // =========================================================
        SURFACE_VICTINI_SPRITE              = 494, // Victini
        SURFACE_SNIVY_SPRITE                = 495, // Snivy
        SURFACE_SERVINE_SPRITE              = 496, // Servine
        SURFACE_SERPERIOR_SPRITE            = 497, // Serperior
        SURFACE_TEPIG_SPRITE                = 498, // Tepig
        SURFACE_PIGNITE_SPRITE              = 499, // Pignite
        SURFACE_EMBOAR_SPRITE               = 500, // Emboar
        SURFACE_OSHAWOTT_SPRITE             = 501, // Oshawott
        SURFACE_DEWOTT_SPRITE               = 502, // Dewott
        SURFACE_SAMUROTT_SPRITE             = 503, // Samurott
        SURFACE_PATRAT_SPRITE               = 504, // Patrat
        SURFACE_WATCHOG_SPRITE              = 505, // Watchog
        SURFACE_LILLIPUP_SPRITE             = 506, // Lillipup
        SURFACE_HERDIER_SPRITE              = 507, // Herdier
        SURFACE_STOUTLAND_SPRITE            = 508, // Stoutland
        SURFACE_PURRLOIN_SPRITE             = 509, // Purrloin
        SURFACE_LIEPARD_SPRITE              = 510, // Liepard
        SURFACE_PANSAGE_SPRITE              = 511, // Pansage
        SURFACE_SIMISAGE_SPRITE             = 512, // Simisage
        SURFACE_PANSEAR_SPRITE              = 513, // Pansear
        SURFACE_SIMISEAR_SPRITE             = 514, // Simisear
        SURFACE_PANPOUR_SPRITE              = 515, // Panpour
        SURFACE_SIMIPOUR_SPRITE             = 516, // Simipour
        SURFACE_MUNNA_SPRITE                = 517, // Munna
        SURFACE_MUSHARNA_SPRITE             = 518, // Musharna
        SURFACE_PIDOVE_SPRITE               = 519, // Pidove
        SURFACE_TRANQUILL_SPRITE            = 520, // Tranquill
        SURFACE_UNFEZANT_SPRITE             = 521, // Unfezant
        SURFACE_BLITZLE_SPRITE              = 522, // Blitzle
        SURFACE_ZEBSTRIKA_SPRITE            = 523, // Zebstrika
        SURFACE_ROGGENROLA_SPRITE           = 524, // Roggenrola
        SURFACE_BOLDORE_SPRITE              = 525, // Boldore
        SURFACE_GIGALITH_SPRITE             = 526, // Gigalith
        SURFACE_WOOBAT_SPRITE               = 527, // Woobat
        SURFACE_SWOOBAT_SPRITE              = 528, // Swoobat
        SURFACE_DRILBUR_SPRITE              = 529, // Drilbur
        SURFACE_EXCADRILL_SPRITE            = 530, // Excadrill
        SURFACE_AUDINO_SPRITE               = 531, // Audino
        SURFACE_TIMBURR_SPRITE              = 532, // Timburr
        SURFACE_GURDURR_SPRITE              = 533, // Gurdurr
        SURFACE_CONKELDURR_SPRITE           = 534, // Conkeldurr
        SURFACE_TYMPOLE_SPRITE              = 535, // Tympole
        SURFACE_PALPITOAD_SPRITE            = 536, // Palpitoad
        SURFACE_SEISMITOAD_SPRITE           = 537, // Seismitoad
        SURFACE_THROH_SPRITE                = 538, // Throh
        SURFACE_SAWK_SPRITE                 = 539, // Sawk
        SURFACE_SEWADDLE_SPRITE             = 540, // Sewaddle
        SURFACE_SWADLOON_SPRITE             = 541, // Swadloon
        SURFACE_LEAVANNY_SPRITE             = 542, // Leavanny
        SURFACE_VENIPEDE_SPRITE             = 543, // Venipede
        SURFACE_WHIRLIPEDE_SPRITE           = 544, // Whirlipede
        SURFACE_SCOLIPEDE_SPRITE            = 545, // Scolipede
        SURFACE_COTTONEE_SPRITE             = 546, // Cottonee
        SURFACE_WHIMSICOTT_SPRITE           = 547, // Whimsicott
        SURFACE_PETILIL_SPRITE              = 548, // Petilil
        SURFACE_LILLIGANT_SPRITE            = 549, // Lilligant
        SURFACE_BASCULIN_RED_STRIPED_SPRITE = 550, // Basculin
        SURFACE_SANDILE_SPRITE              = 551, // Sandile
        SURFACE_KROKOROK_SPRITE             = 552, // Krokorok
        SURFACE_KROOKODILE_SPRITE           = 553, // Krookodile
        SURFACE_DARUMAKA_SPRITE             = 554, // Darumaka
        SURFACE_DARMANITAN_STANDARD_SPRITE  = 555, // Darmanitan
        SURFACE_MARACTUS_SPRITE             = 556, // Maractus
        SURFACE_DWEBBLE_SPRITE              = 557, // Dwebble
        SURFACE_CRUSTLE_SPRITE              = 558, // Crustle
        SURFACE_SCRAGGY_SPRITE              = 559, // Scraggy
        SURFACE_SCRAFTY_SPRITE              = 560, // Scrafty
        SURFACE_SIGILYPH_SPRITE             = 561, // Sigilyph
        SURFACE_YAMASK_SPRITE               = 562, // Yamask
        SURFACE_COFAGRIGUS_SPRITE           = 563, // Cofagrigus
        SURFACE_TIRTOUGA_SPRITE             = 564, // Tirtouga
        SURFACE_CARRACOSTA_SPRITE           = 565, // Carracosta
        SURFACE_ARCHEN_SPRITE               = 566, // Archen
        SURFACE_ARCHEOPS_SPRITE             = 567, // Archeops
        SURFACE_TRUBBISH_SPRITE             = 568, // Trubbish
        SURFACE_GARBODOR_SPRITE             = 569, // Garbodor
        SURFACE_ZORUA_SPRITE                = 570, // Zorua
        SURFACE_ZOROARK_SPRITE              = 571, // Zoroark
        SURFACE_MINCCINO_SPRITE             = 572, // Minccino
        SURFACE_CINCCINO_SPRITE             = 573, // Cinccino
        SURFACE_GOTHITA_SPRITE              = 574, // Gothita
        SURFACE_GOTHORITA_SPRITE            = 575, // Gothorita
        SURFACE_GOTHITELLE_SPRITE           = 576, // Gothitelle
        SURFACE_SOLOSIS_SPRITE              = 577, // Solosis
        SURFACE_DUOSION_SPRITE              = 578, // Duosion
        SURFACE_REUNICLUS_SPRITE            = 579, // Reuniclus
        SURFACE_DUCKLETT_SPRITE             = 580, // Ducklett
        SURFACE_SWANNA_SPRITE               = 581, // Swanna
        SURFACE_VANILLITE_SPRITE            = 582, // Vanillite
        SURFACE_VANILLISH_SPRITE            = 583, // Vanillish
        SURFACE_VANILLUXE_SPRITE            = 584, // Vanilluxe
        SURFACE_DEERLING_SPRITE             = 585, // Deerling
        SURFACE_SAWSBUCK_SPRITE             = 586, // Sawsbuck
        SURFACE_EMOLGA_SPRITE               = 587, // Emolga
        SURFACE_KARRABLAST_SPRITE           = 588, // Karrablast
        SURFACE_ESCAVALIER_SPRITE           = 589, // Escavalier
        SURFACE_FOONGUS_SPRITE              = 590, // Foongus
        SURFACE_AMOONGUSS_SPRITE            = 591, // Amoonguss
        SURFACE_FRILLISH_SPRITE             = 592, // Frillish
        SURFACE_JELLICENT_SPRITE            = 593, // Jellicent
        SURFACE_ALOMOMOLA_SPRITE            = 594, // Alomomola
        SURFACE_JOLTIK_SPRITE               = 595, // Joltik
        SURFACE_GALVANTULA_SPRITE           = 596, // Galvantula
        SURFACE_FERROSEED_SPRITE            = 597, // Ferroseed
        SURFACE_FERROTHORN_SPRITE           = 598, // Ferrothorn
        SURFACE_KLINK_SPRITE                = 599, // Klink
        SURFACE_KLANG_SPRITE                = 600, // Klang
        SURFACE_KLINKLANG_SPRITE            = 601, // Klinklang
        SURFACE_TYNAMO_SPRITE               = 602, // Tynamo
        SURFACE_EELEKTRIK_SPRITE            = 603, // Eelektrik
        SURFACE_EELEKTROSS_SPRITE           = 604, // Eelektross
        SURFACE_ELGYEM_SPRITE               = 605, // Elgyem
        SURFACE_BEHEEYEM_SPRITE             = 606, // Beheeyem
        SURFACE_LITWICK_SPRITE              = 607, // Litwick
        SURFACE_LAMPENT_SPRITE              = 608, // Lampent
        SURFACE_CHANDELURE_SPRITE           = 609, // Chandelure
        SURFACE_AXEW_SPRITE                 = 610, // Axew
        SURFACE_FRAXURE_SPRITE              = 611, // Fraxure
        SURFACE_HAXORUS_SPRITE              = 612, // Haxorus
        SURFACE_CUBCHOO_SPRITE              = 613, // Cubchoo
        SURFACE_BEARTIC_SPRITE              = 614, // Beartic
        SURFACE_CRYOGONAL_SPRITE            = 615, // Cryogonal
        SURFACE_SHELMET_SPRITE              = 616, // Shelmet
        SURFACE_ACCELGOR_SPRITE             = 617, // Accelgor
        SURFACE_STUNFISK_SPRITE             = 618, // Stunfisk
        SURFACE_MIENFOO_SPRITE              = 619, // Mienfoo
        SURFACE_MIENSHAO_SPRITE             = 620, // Mienshao
        SURFACE_DRUDDIGON_SPRITE            = 621, // Druddigon
        SURFACE_GOLETT_SPRITE               = 622, // Golett
        SURFACE_GOLURK_SPRITE               = 623, // Golurk
        SURFACE_PAWNIARD_SPRITE             = 624, // Pawniard
        SURFACE_BISHARP_SPRITE              = 625, // Bisharp
        SURFACE_BOUFFALANT_SPRITE           = 626, // Bouffalant
        SURFACE_RUFFLET_SPRITE              = 627, // Rufflet
        SURFACE_BRAVIARY_SPRITE             = 628, // Braviary
        SURFACE_VULLABY_SPRITE              = 629, // Vullaby
        SURFACE_MANDIBUZZ_SPRITE            = 630, // Mandibuzz
        SURFACE_HEATMOR_SPRITE              = 631, // Heatmor
        SURFACE_DURANT_SPRITE               = 632, // Durant
        SURFACE_DEINO_SPRITE                = 633, // Deino
        SURFACE_ZWEILOUS_SPRITE             = 634, // Zweilous
        SURFACE_HYDREIGON_SPRITE            = 635, // Hydreigon
        SURFACE_LARVESTA_SPRITE             = 636, // Larvesta
        SURFACE_VOLCARONA_SPRITE            = 637, // Volcarona
        SURFACE_COBALION_SPRITE             = 638, // Cobalion
        SURFACE_TERRAKION_SPRITE            = 639, // Terrakion
        SURFACE_VIRIZION_SPRITE             = 640, // Virizion
        SURFACE_TORNADUS_INCARNATE_SPRITE   = 641, // Tornadus
        SURFACE_THUNDURUS_INCARNATE_SPRITE  = 642, // Thundurus
        SURFACE_RESHIRAM_SPRITE             = 643, // Reshiram
        SURFACE_ZEKROM_SPRITE               = 644, // Zekrom
        SURFACE_LANDORUSINCARNATE_SPRITE    = 645, // Landorus
        SURFACE_KYUREM_SPRITE               = 646, // Kyurem
        SURFACE_KELDEOORDINARY_SPRITE       = 647, // Keldeo
        SURFACE_MELOETTA_ARIA_SPRITE        = 648, // Meloetta
        SURFACE_GENESECT_SPRITE             = 649, // Genesect

        //====================================================POKEMON_ICONS==============================================================

        // Kanto Pokémon (Generation 1) - National Dex IDs 1–151
        // =========================================================
        SURFACE_BULBASAUR_ICON  = 1,   // Bulbasaur (#001)
        SURFACE_IVYSAUR_ICON    = 2,   // Ivysaur (#002)
        SURFACE_VENUSAUR_ICON   = 3,   // Venusaur (#003)
        SURFACE_CHARMANDER_ICON = 4,   // Charmander (#004)
        SURFACE_CHARMELEON_ICON = 5,   // Charmeleon (#005)
        SURFACE_CHARIZARD_ICON  = 6,   // Charizard (#006)
        SURFACE_SQUIRTLE_ICON   = 7,   // Squirtle (#007)
        SURFACE_WARTORTLE_ICON  = 8,   // Wartortle (#008)
        SURFACE_BLASTOISE_ICON  = 9,   // Blastoise (#009)
        SURFACE_CATERPIE_ICON   = 10,  // Caterpie (#010)
        SURFACE_METAPOD_ICON    = 11,  // Metapod (#011)
        SURFACE_BUTTERFREE_ICON = 12,  // Butterfree (#012)
        SURFACE_WEEDLE_ICON     = 13,  // Weedle (#013)
        SURFACE_KAKUNA_ICON     = 14,  // Kakuna (#014)
        SURFACE_BEEDRILL_ICON   = 15,  // Beedrill (#015)
        SURFACE_PIDGEY_ICON     = 16,  // Pidgey (#016)
        SURFACE_PIDGEOTTO_ICON  = 17,  // Pidgeotto (#017)
        SURFACE_PIDGEOT_ICON    = 18,  // Pidgeot (#018)
        SURFACE_RATTATA_ICON    = 19,  // Rattata (#019)
        SURFACE_RATICATE_ICON   = 20,  // Raticate (#020)
        SURFACE_SPEAROW_ICON    = 21,  // Spearow (#021)
        SURFACE_FEAROW_ICON     = 22,  // Fearow (#022)
        SURFACE_EKANS_ICON      = 23,  // Ekans (#023)
        SURFACE_ARBOK_ICON      = 24,  // Arbok (#024)
        SURFACE_PIKACHU_ICON    = 25,  // Pikachu (#025)
        SURFACE_RAICHU_ICON     = 26,  // Raichu (#026)
        SURFACE_SANDSHREW_ICON  = 27,  // Sandshrew (#027)
        SURFACE_SANDSLASH_ICON  = 28,  // Sandslash (#028)
        SURFACE_NIDORAN_F_ICON  = 29,  // Nidoran♀ (#029)
        SURFACE_NIDORINA_ICON   = 30,  // Nidorina (#030)
        SURFACE_NIDOQUEEN_ICON  = 31,  // Nidoqueen (#031)
        SURFACE_NIDORAN_M_ICON  = 32,  // Nidoran♂ (#032)
        SURFACE_NIDORINO_ICON   = 33,  // Nidorino (#033)
        SURFACE_NIDOKING_ICON   = 34,  // Nidoking (#034)
        SURFACE_CLEFAIRY_ICON   = 35,  // Clefairy (#035)
        SURFACE_CLEFABLE_ICON   = 36,  // Clefable (#036)
        SURFACE_VULPIX_ICON     = 37,  // Vulpix (#037)
        SURFACE_NINETALES_ICON  = 38,  // Ninetales (#038)
        SURFACE_JIGGLYPUFF_ICON = 39,  // Jigglypuff (#039)
        SURFACE_WIGGLYTUFF_ICON = 40,  // Wigglytuff (#040)
        SURFACE_ZUBAT_ICON      = 41,  // Zubat (#041)
        SURFACE_GOLBAT_ICON     = 42,  // Golbat (#042)
        SURFACE_ODDISH_ICON     = 43,  // Oddish (#043)
        SURFACE_GLOOM_ICON      = 44,  // Gloom (#044)
        SURFACE_VILEPLUME_ICON  = 45,  // Vileplume (#045)
        SURFACE_PARAS_ICON      = 46,  // Paras (#046)
        SURFACE_PARASECT_ICON   = 47,  // Parasect (#047)
        SURFACE_VENONAT_ICON    = 48,  // Venonat (#048)
        SURFACE_VENOMOTH_ICON   = 49,  // Venomoth (#049)
        SURFACE_DIGLETT_ICON    = 50,  // Diglett (#050)
        SURFACE_DUGTRIO_ICON    = 51,  // Dugtrio (#051)
        SURFACE_MEOWTH_ICON     = 52,  // Meowth (#052)
        SURFACE_PERSIAN_ICON    = 53,  // Persian (#053)
        SURFACE_PSYDUCK_ICON    = 54,  // Psyduck (#054)
        SURFACE_GOLDUCK_ICON    = 55,  // Golduck (#055)
        SURFACE_MANKEY_ICON     = 56,  // Mankey (#056)
        SURFACE_PRIMEAPE_ICON   = 57,  // Primeape (#057)
        SURFACE_GROWLITHE_ICON  = 58,  // Growlithe (#058)
        SURFACE_ARCANINE_ICON   = 59,  // Arcanine (#059)
        SURFACE_POLIWAG_ICON    = 60,  // Poliwag (#060)
        SURFACE_POLIWHIRL_ICON  = 61,  // Poliwhirl (#061)
        SURFACE_POLIWRATH_ICON  = 62,  // Poliwrath (#062)
        SURFACE_ABRA_ICON       = 63,  // Abra (#063)
        SURFACE_KADABRA_ICON    = 64,  // Kadabra (#064)
        SURFACE_ALAKAZAM_ICON   = 65,  // Alakazam (#065)
        SURFACE_MACHOP_ICON     = 66,  // Machop (#066)
        SURFACE_MACHOKE_ICON    = 67,  // Machoke (#067)
        SURFACE_MACHAMP_ICON    = 68,  // Machamp (#068)
        SURFACE_BELLSPROUT_ICON = 69,  // Bellsprout (#069)
        SURFACE_WEEPINBELL_ICON = 70,  // Weepinbell (#070)
        SURFACE_VICTREEBEL_ICON = 71,  // Victreebel (#071)
        SURFACE_TENTACOOL_ICON  = 72,  // Tentacool (#072)
        SURFACE_TENTACRUEL_ICON = 73,  // Tentacruel (#073)
        SURFACE_GEODUDE_ICON    = 74,  // Geodude (#074)
        SURFACE_GRAVELER_ICON   = 75,  // Graveler (#075)
        SURFACE_GOLEM_ICON      = 76,  // Golem (#076)
        SURFACE_PONYTA_ICON     = 77,  // Ponyta (#077)
        SURFACE_RAPIDASH_ICON   = 78,  // Rapidash (#078)
        SURFACE_SLOWPOKE_ICON   = 79,  // Slowpoke (#079)
        SURFACE_SLOWBRO_ICON    = 80,  // Slowbro (#080)
        SURFACE_MAGNEMITE_ICON  = 81,  // Magnemite (#081)
        SURFACE_MAGNETON_ICON   = 82,  // Magneton (#082)
        SURFACE_FARFETCHD_ICON  = 83,  // Farfetch'd (#083)
        SURFACE_DODUO_ICON      = 84,  // Doduo (#084)
        SURFACE_DODRIO_ICON     = 85,  // Dodrio (#085)
        SURFACE_SEEL_ICON       = 86,  // Seel (#086)
        SURFACE_DEWGONG_ICON    = 87,  // Dewgong (#087)
        SURFACE_GRIMER_ICON     = 88,  // Grimer (#088)
        SURFACE_MUK_ICON        = 89,  // Muk (#089)
        SURFACE_SHELLDER_ICON   = 90,  // Shellder (#090)
        SURFACE_CLOYSTER_ICON   = 91,  // Cloyster (#091)
        SURFACE_GASTLY_ICON     = 92,  // Gastly (#092)
        SURFACE_HAUNTER_ICON    = 93,  // Haunter (#093)
        SURFACE_GENGAR_ICON     = 94,  // Gengar (#094)
        SURFACE_ONIX_ICON       = 95,  // Onix (#095)
        SURFACE_DROWZEE_ICON    = 96,  // Drowzee (#096)
        SURFACE_HYPNO_ICON      = 97,  // Hypno (#097)
        SURFACE_KRABBY_ICON     = 98,  // Krabby (#098)
        SURFACE_KINGLER_ICON    = 99,  // Kingler (#099)
        SURFACE_VOLTORB_ICON    = 100, // Voltorb (#100)
        SURFACE_ELECTRODE_ICON  = 101, // Electrode (#101)
        SURFACE_EXEGGCUTE_ICON  = 102, // Exeggcute (#102)
        SURFACE_EXEGGUTOR_ICON  = 103, // Exeggutor (#103)
        SURFACE_CUBONE_ICON     = 104, // Cubone (#104)
        SURFACE_MAROWAK_ICON    = 105, // Marowak (#105)
        SURFACE_HITMONLEE_ICON  = 106, // Hitmonlee (#106)
        SURFACE_HITMONCHAN_ICON = 107, // Hitmonchan (#107)
        SURFACE_LICKITUNG_ICON  = 108, // Lickitung (#108)
        SURFACE_KOFFING_ICON    = 109, // Koffing (#109)
        SURFACE_WEEZING_ICON    = 110, // Weezing (#110)
        SURFACE_RHYHORN_ICON    = 111, // Rhyhorn (#111)
        SURFACE_RHYDON_ICON     = 112, // Rhydon (#112)
        SURFACE_CHANSEY_ICON    = 113, // Chansey (#113)
        SURFACE_TANGELA_ICON    = 114, // Tangela (#114)
        SURFACE_KANGASKHAN_ICON = 115, // Kangaskhan (#115)
        SURFACE_HORSEA_ICON     = 116, // Horsea (#116)
        SURFACE_SEADRA_ICON     = 117, // Seadra (#117)
        SURFACE_GOLDEEN_ICON    = 118, // Goldeen (#118)
        SURFACE_SEAKING_ICON    = 119, // Seaking (#119)
        SURFACE_STARYU_ICON     = 120, // Staryu (#120)
        SURFACE_STARMIE_ICON    = 121, // Starmie (#121)
        SURFACE_MRMIME_ICON     = 122, // Mr. Mime (#122)
        SURFACE_SCYTHER_ICON    = 123, // Scyther (#123)
        SURFACE_JYNX_ICON       = 124, // Jynx (#124)
        SURFACE_ELECTABUZZ_ICON = 125, // Electabuzz (#125)
        SURFACE_MAGMAR_ICON     = 126, // Magmar (#126)
        SURFACE_PINSIR_ICON     = 127, // Pinsir (#127)
        SURFACE_TAUROS_ICON     = 128, // Tauros (#128)
        SURFACE_MAGIKARP_ICON   = 129, // Magikarp (#129)
        SURFACE_GYARADOS_ICON   = 130, // Gyarados (#130)
        SURFACE_LAPRAS_ICON     = 131, // Lapras (#131)
        SURFACE_DITTO_ICON      = 132, // Ditto (#132)
        SURFACE_EEVEE_ICON      = 133, // Eevee (#133)
        SURFACE_VAPOREON_ICON   = 134, // Vaporeon (#134)
        SURFACE_JOLTEON_ICON    = 135, // Jolteon (#135)
        SURFACE_FLAREON_ICON    = 136, // Flareon (#136)
        SURFACE_PORYGON_ICON    = 137, // Porygon (#137)
        SURFACE_OMANYTE_ICON    = 138, // Omanyte (#138)
        SURFACE_OMASTAR_ICON    = 139, // Omastar (#139)
        SURFACE_KABUTO_ICON     = 140, // Kabuto (#140)
        SURFACE_KABUTOPS_ICON   = 141, // Kabutops (#141)
        SURFACE_AERODACTYL_ICON = 142, // Aerodactyl (#142)
        SURFACE_SNORLAX_ICON    = 143, // Snorlax (#143)
        SURFACE_ARTICUNO_ICON   = 144, // Articuno (#144)
        SURFACE_ZAPDOS_ICON     = 145, // Zapdos (#145)
        SURFACE_MOLTRES_ICON    = 146, // Moltres (#146)
        SURFACE_DRATINI_ICON    = 147, // Dratini (#147)
        SURFACE_DRAGONAIR_ICON  = 148, // Dragonair (#148)
        SURFACE_DRAGONITE_ICON  = 149, // Dragonite (#149)
        SURFACE_MEWTWO_ICON     = 150, // Mewtwo (#150)
        SURFACE_MEW_ICON        = 151, // Mew (#151)

        // Johto Pokémon (Generation 2) - National Dex IDs 152–251
        // =========================================================
        SURFACE_CHIKORITA_ICON  = 152, // Chikorita
        SURFACE_BAYLEEF_ICON    = 153, // Bayleef
        SURFACE_MEGANIUM_ICON   = 154, // Meganium
        SURFACE_CYNDAQUIL_ICON  = 155, // Cyndaquil
        SURFACE_QUILAVA_ICON    = 156, // Quilava
        SURFACE_TYPHLOSION_ICON = 157, // Typhlosion
        SURFACE_TOTODILE_ICON   = 158, // Totodile
        SURFACE_CROCONAW_ICON   = 159, // Croconaw
        SURFACE_FERALIGATR_ICON = 160, // Feraligatr
        SURFACE_SENTRET_ICON    = 161, // Sentret
        SURFACE_FURRET_ICON     = 162, // Furret
        SURFACE_HOOTHOOT_ICON   = 163, // Hoothoot
        SURFACE_NOCTOWL_ICON    = 164, // Noctowl
        SURFACE_LEDYBA_ICON     = 165, // Ledyba
        SURFACE_LEDIAN_ICON     = 166, // Ledian
        SURFACE_SPINARAK_ICON   = 167, // Spinarak
        SURFACE_ARIADOS_ICON    = 168, // Ariados
        SURFACE_CROBAT_ICON     = 169, // Crobat
        SURFACE_CHINCHOU_ICON   = 170, // Chinchou
        SURFACE_LANTURN_ICON    = 171, // Lanturn
        SURFACE_PICHU_ICON      = 172, // Pichu
        SURFACE_CLEFFA_ICON     = 173, // Cleffa
        SURFACE_IGGLYBUFF_ICON  = 174, // Igglybuff
        SURFACE_TOGEPI_ICON     = 175, // Togepi
        SURFACE_TOGETIC_ICON    = 176, // Togetic
        SURFACE_NATU_ICON       = 177, // Natu
        SURFACE_XATU_ICON       = 178, // Xatu
        SURFACE_MAREEP_ICON     = 179, // Mareep
        SURFACE_FLAAFFY_ICON    = 180, // Flaaffy
        SURFACE_AMPHAROS_ICON   = 181, // Ampharos
        SURFACE_BELLOSSOM_ICON  = 182, // Bellossom
        SURFACE_MARILL_ICON     = 183, // Marill
        SURFACE_AZUMARILL_ICON  = 184, // Azumarill
        SURFACE_SUDOWOODO_ICON  = 185, // Sudowoodo
        SURFACE_POLITOED_ICON   = 186, // Politoed
        SURFACE_HOPPIP_ICON     = 187, // Hoppip
        SURFACE_SKIPLOOM_ICON   = 188, // Skiploom
        SURFACE_JUMPLUFF_ICON   = 189, // Jumpluff
        SURFACE_AIPOM_ICON      = 190, // Aipom
        SURFACE_SUNKERN_ICON    = 191, // Sunkern
        SURFACE_SUNFLORA_ICON   = 192, // Sunflora
        SURFACE_YANMA_ICON      = 193, // Yanma
        SURFACE_WOOPER_ICON     = 194, // Wooper
        SURFACE_QUAGSIRE_ICON   = 195, // Quagsire
        SURFACE_ESPEON_ICON     = 196, // Espeon
        SURFACE_UMBREON_ICON    = 197, // Umbreon
        SURFACE_MURKROW_ICON    = 198, // Murkrow
        SURFACE_SLOWKING_ICON   = 199, // Slowking
        SURFACE_MISDREAVUS_ICON = 200, // Misdreavus
        SURFACE_UNOWN_ICON      = 201, // Unown
        SURFACE_WOBBUFFET_ICON  = 202, // Wobbuffet
        SURFACE_GIRAFARIG_ICON  = 203, // Girafarig
        SURFACE_PINECO_ICON     = 204, // Pineco
        SURFACE_FORRETRESS_ICON = 205, // Forretress
        SURFACE_DUNSPARCE_ICON  = 206, // Dunsparce
        SURFACE_GLIGAR_ICON     = 207, // Gligar
        SURFACE_STEELIX_ICON    = 208, // Steelix
        SURFACE_SNUBBULL_ICON   = 209, // Snubbull
        SURFACE_GRANBULL_ICON   = 210, // Granbull
        SURFACE_QWILFISH_ICON   = 211, // Qwilfish
        SURFACE_SCIZOR_ICON     = 212, // Scizor
        SURFACE_SHUCKLE_ICON    = 213, // Shuckle
        SURFACE_HERACROSS_ICON  = 214, // Heracross
        SURFACE_SNEASEL_ICON    = 215, // Sneasel
        SURFACE_TEDDIURSA_ICON  = 216, // Teddiursa
        SURFACE_URSARING_ICON   = 217, // Ursaring
        SURFACE_SLUGMA_ICON     = 218, // Slugma
        SURFACE_MAGCARGO_ICON   = 219, // Magcargo
        SURFACE_SWINUB_ICON     = 220, // Swinub
        SURFACE_PILOSWINE_ICON  = 221, // Piloswine
        SURFACE_CORSOLA_ICON    = 222, // Corsola
        SURFACE_REMORAID_ICON   = 223, // Remoraid
        SURFACE_OCTILLERY_ICON  = 224, // Octillery
        SURFACE_DELIBIRD_ICON   = 225, // Delibird
        SURFACE_MANTINE_ICON    = 226, // Mantine
        SURFACE_SKARMORY_ICON   = 227, // Skarmory
        SURFACE_HOUNDOUR_ICON   = 228, // Houndour
        SURFACE_HOUNDOOM_ICON   = 229, // Houndoom
        SURFACE_KINGDRA_ICON    = 230, // Kingdra
        SURFACE_PHANPY_ICON     = 231, // Phanpy
        SURFACE_DONPHAN_ICON    = 232, // Donphan
        SURFACE_PORYGON2_ICON   = 233, // Porygon2
        SURFACE_STANTLER_ICON   = 234, // Stantler
        SURFACE_SMEARGLE_ICON   = 235, // Smeargle
        SURFACE_TYROGUE_ICON    = 236, // Tyrogue
        SURFACE_HITMONTOP_ICON  = 237, // Hitmontop
        SURFACE_SMOOCHUM_ICON   = 238, // Smoochum
        SURFACE_ELEKID_ICON     = 239, // Elekid
        SURFACE_MAGBY_ICON      = 240, // Magby
        SURFACE_MILTANK_ICON    = 241, // Miltank
        SURFACE_BLISSEY_ICON    = 242, // Blissey
        SURFACE_RAIKOU_ICON     = 243, // Raikou
        SURFACE_ENTEI_ICON      = 244, // Entei
        SURFACE_SUICUNE_ICON    = 245, // Suicune
        SURFACE_LARVITAR_ICON   = 246, // Larvitar
        SURFACE_PUPITAR_ICON    = 247, // Pupitar
        SURFACE_TYRANITAR_ICON  = 248, // Tyranitar
        SURFACE_LUGIA_ICON      = 249, // Lugia
        SURFACE_HOOH_ICON       = 250, // Ho-Oh
        SURFACE_CELEBI_ICON     = 251, // Celebi

        // Hoenn Pokémon (Generation 3) - National Dex IDs 252–386
        // =========================================================
        SURFACE_TREECKO_ICON      = 252, // Treecko
        SURFACE_GROVYLE_ICON      = 253, // Grovyle
        SURFACE_SCEPTILE_ICON     = 254, // Sceptile
        SURFACE_TORCHIC_ICON      = 255, // Torchic
        SURFACE_COMBUSKEN_ICON    = 256, // Combusken
        SURFACE_BLAZIKEN_ICON     = 257, // Blaziken
        SURFACE_MUDKIP_ICON       = 258, // Mudkip
        SURFACE_MARSHTOMP_ICON    = 259, // Marshtomp
        SURFACE_SWAMPERT_ICON     = 260, // Swampert
        SURFACE_POOCHYENA_ICON    = 261, // Poochyena
        SURFACE_MIGHTYENA_ICON    = 262, // Mightyena
        SURFACE_ZIGZAGOON_ICON    = 263, // Zigzagoon
        SURFACE_LINOONE_ICON      = 264, // Linoone
        SURFACE_WURMPLE_ICON      = 265, // Wurmple
        SURFACE_SILCOON_ICON      = 266, // Silcoon
        SURFACE_BEAUTIFLY_ICON    = 267, // Beautifly
        SURFACE_CASCOON_ICON      = 268, // Cascoon
        SURFACE_DUSTOX_ICON       = 269, // Dustox
        SURFACE_LOTAD_ICON        = 270, // Lotad
        SURFACE_LOMBRE_ICON       = 271, // Lombre
        SURFACE_LUDICOLO_ICON     = 272, // Ludicolo
        SURFACE_SEEDOT_ICON       = 273, // Seedot
        SURFACE_NUZLEAF_ICON      = 274, // Nuzleaf
        SURFACE_SHIFTRY_ICON      = 275, // Shiftry
        SURFACE_TAILLOW_ICON      = 276, // Taillow
        SURFACE_SWELLOW_ICON      = 277, // Swellow
        SURFACE_WINGULL_ICON      = 278, // Wingull
        SURFACE_PELIPPER_ICON     = 279, // Pelipper
        SURFACE_RALTS_ICON        = 280, // Ralts
        SURFACE_KIRLIA_ICON       = 281, // Kirlia
        SURFACE_GARDEVOIR_ICON    = 282, // Gardevoir
        SURFACE_SURSKIT_ICON      = 283, // Surskit
        SURFACE_MASQUERAIN_ICON   = 284, // Masquerain
        SURFACE_SHROOMISH_ICON    = 285, // Shroomish
        SURFACE_BRELOOM_ICON      = 286, // Breloom
        SURFACE_SLAKOTH_ICON      = 287, // Slakoth
        SURFACE_VIGOROTH_ICON     = 288, // Vigoroth
        SURFACE_SLAKING_ICON      = 289, // Slaking
        SURFACE_NINCADA_ICON      = 290, // Nincada
        SURFACE_NINJASK_ICON      = 291, // Ninjask
        SURFACE_SHEDINJA_ICON     = 292, // Shedinja
        SURFACE_WHISMUR_ICON      = 293, // Whismur
        SURFACE_LOUDRED_ICON      = 294, // Loudred
        SURFACE_EXPLOUD_ICON      = 295, // Exploud
        SURFACE_MAKUHITA_ICON     = 296, // Makuhita
        SURFACE_HARIYAMA_ICON     = 297, // Hariyama
        SURFACE_AZURILL_ICON      = 298, // Azurill
        SURFACE_NOSEPASS_ICON     = 299, // Nosepass
        SURFACE_SKITTY_ICON       = 300, // Skitty
        SURFACE_DELCATTY_ICON     = 301, // Delcatty
        SURFACE_SABLEYE_ICON      = 302, // Sableye
        SURFACE_MAWILE_ICON       = 303, // Mawile
        SURFACE_ARON_ICON         = 304, // Aron
        SURFACE_LAIRON_ICON       = 305, // Lairon
        SURFACE_AGGRON_ICON       = 306, // Aggron
        SURFACE_MEDITITE_ICON     = 307, // Meditite
        SURFACE_MEDICHAM_ICON     = 308, // Medicham
        SURFACE_ELECTRIKE_ICON    = 309, // Electrike
        SURFACE_MANECTRIC_ICON    = 310, // Manectric
        SURFACE_PLUSLE_ICON       = 311, // Plusle
        SURFACE_MINUN_ICON        = 312, // Minun
        SURFACE_VOLBEAT_ICON      = 313, // Volbeat
        SURFACE_ILLUMISE_ICON     = 314, // Illumise
        SURFACE_ROSELIA_ICON      = 315, // Roselia
        SURFACE_GULPIN_ICON       = 316, // Gulpin
        SURFACE_SWALOT_ICON       = 317, // Swalot
        SURFACE_CARVANHA_ICON     = 318, // Carvanha
        SURFACE_SHARPEDO_ICON     = 319, // Sharpedo
        SURFACE_WAILMER_ICON      = 320, // Wailmer
        SURFACE_WAILORD_ICON      = 321, // Wailord
        SURFACE_NUMEL_ICON        = 322, // Numel
        SURFACE_CAMERUPT_ICON     = 323, // Camerupt
        SURFACE_TORKOAL_ICON      = 324, // Torkal
        SURFACE_SPOINK_ICON       = 325, // Spoink
        SURFACE_GRUMPIG_ICON      = 326, // Grumpig
        SURFACE_SPINDA_ICON       = 327, // Spinda
        SURFACE_TRAPINCH_ICON     = 328, // Trapinch
        SURFACE_VIBRAVA_ICON      = 329, // Vibrava
        SURFACE_FLYGON_ICON       = 330, // Flygon
        SURFACE_CACNEA_ICON       = 331, // Cacnea
        SURFACE_CACTURNE_ICON     = 332, // Cacturne
        SURFACE_SWABLU_ICON       = 333, // Swablu
        SURFACE_ALTARIA_ICON      = 334, // Altaria
        SURFACE_ZANGOOSE_ICON     = 335, // Zangoose
        SURFACE_SEVIPER_ICON      = 336, // Seviper
        SURFACE_LUNATONE_ICON     = 337, // Lunatone
        SURFACE_SOLROCK_ICON      = 338, // Solrock
        SURFACE_BARBOACH_ICON     = 339, // Barboach
        SURFACE_WHISCASH_ICON     = 340, // Whiscash
        SURFACE_CORPHISH_ICON     = 341, // Corphish
        SURFACE_CRAWDAUNT_ICON    = 342, // Crawdaunt
        SURFACE_BALTOY_ICON       = 343, // Baltoy
        SURFACE_CLAYDOL_ICON      = 344, // Claydol
        SURFACE_LILEEP_ICON       = 345, // Lileep
        SURFACE_CRADILY_ICON      = 346, // Cradily
        SURFACE_ANORITH_ICON      = 347, // Anorith
        SURFACE_ARMALDO_ICON      = 348, // Armaldo
        SURFACE_FEEBAS_ICON       = 349, // Feebas
        SURFACE_MILOTIC_ICON      = 350, // Milotic
        SURFACE_CASTFORM_ICON     = 351, // Castform
        SURFACE_KECLEON_ICON      = 352, // Kecleon
        SURFACE_SHUPPET_ICON      = 353, // Shuppet
        SURFACE_BANETTE_ICON      = 354, // Banette
        SURFACE_DUSKULL_ICON      = 355, // Duskull
        SURFACE_DUSCLOPS_ICON     = 356, // Dusclops
        SURFACE_TROPIUS_ICON      = 357, // Tropius
        SURFACE_CHIMECHO_ICON     = 358, // Chimecho
        SURFACE_ABSOL_ICON        = 359, // Absol
        SURFACE_WYNAUT_ICON       = 360, // Wynaut
        SURFACE_SNORUNT_ICON      = 361, // Snorunt
        SURFACE_GLALIE_ICON       = 362, // Glalie
        SURFACE_SPHEAL_ICON       = 363, // Spheal
        SURFACE_SEALEO_ICON       = 364, // Sealeo
        SURFACE_WALREIN_ICON      = 365, // Walrein
        SURFACE_CLAMPERL_ICON     = 366, // Clamperl
        SURFACE_HUNTAIL_ICON      = 367, // Huntail
        SURFACE_GOREBYSS_ICON     = 368, // Gorebyss
        SURFACE_RELICANTH_ICON    = 369, // Relicanth
        SURFACE_LUVDISC_ICON      = 370, // Luvdisc
        SURFACE_BAGON_ICON        = 371, // Bagon
        SURFACE_SHELGON_ICON      = 372, // Shelgon
        SURFACE_SALAMENCE_ICON    = 373, // Salamence
        SURFACE_BELDUM_ICON       = 374, // Beldum
        SURFACE_METANG_ICON       = 375, // Metang
        SURFACE_METAGROSS_ICON    = 376, // Metagross
        SURFACE_REGIROCK_ICON     = 377, // Regirock
        SURFACE_REGICE_ICON       = 378, // Regice
        SURFACE_REGISTEEL_ICON    = 379, // Registeel
        SURFACE_LATIAS_ICON       = 380, // Latias
        SURFACE_LATIOS_ICON       = 381, // Latios
        SURFACE_KYOGRE_ICON       = 382, // Kyogre
        SURFACE_GROUDON_ICON      = 383, // Groudon
        SURFACE_RAYQUAZA_ICON     = 384, // Rayquaza
        SURFACE_JIRACHI_ICON      = 385, // Jirachi
        SURFACE_DEOXYSNORMAL_ICON = 386, // Deoxys

        // Sinnoh Pokémon (Generation 4) - National Dex IDs 387–493
        // =========================================================
        SURFACE_TURTWIG_ICON        = 387, // Turtwig
        SURFACE_GROTLE_ICON         = 388, // Grotle
        SURFACE_TORTERRA_ICON       = 389, // Torterra
        SURFACE_CHIMCHAR_ICON       = 390, // Chimchar
        SURFACE_MONFERNO_ICON       = 391, // Monferno
        SURFACE_INFERNAPE_ICON      = 392, // Infernape
        SURFACE_PIPLUP_ICON         = 393, // Piplup
        SURFACE_PRINPLUP_ICON       = 394, // Prinplup
        SURFACE_EMPOLEON_ICON       = 395, // Empoleon
        SURFACE_STARLY_ICON         = 396, // Starly
        SURFACE_STARAVIA_ICON       = 397, // Staravia
        SURFACE_STARAPTOR_ICON      = 398, // Staraptor
        SURFACE_BIDOOF_ICON         = 399, // Bidoof
        SURFACE_BIBAREL_ICON        = 400, // Bibarel
        SURFACE_KRICKETOT_ICON      = 401, // Kricketot
        SURFACE_KRICKETUNE_ICON     = 402, // Kricketune
        SURFACE_SHINX_ICON          = 403, // Shinx
        SURFACE_LUXIO_ICON          = 404, // Luxio
        SURFACE_LUXRAY_ICON         = 405, // Luxray
        SURFACE_BUDEW_ICON          = 406, // Budew
        SURFACE_ROSERADE_ICON       = 407, // Roserade
        SURFACE_CRANIDOS_ICON       = 408, // Cranidos
        SURFACE_RAMPARDOS_ICON      = 409, // Rampardos
        SURFACE_SHIELDON_ICON       = 410, // Shieldon
        SURFACE_BASTIODON_ICON      = 411, // Bastiodon
        SURFACE_BURMY_ICON          = 412, // Burmy
        SURFACE_WORMADAM_PLANT_ICON = 413, // Wormadam
        SURFACE_MOTHIM_ICON         = 414, // Mothim
        SURFACE_COMBEE_ICON         = 415, // Combee
        SURFACE_VESPIQUEN_ICON      = 416, // Vespiquen
        SURFACE_PACHIRISU_ICON      = 417, // Pachirisu
        SURFACE_BUIZEL_ICON         = 418, // Buizel
        SURFACE_FLOATZEL_ICON       = 419, // Floatzel
        SURFACE_CHERUBI_ICON        = 420, // Cherubi
        SURFACE_CHERRIM_ICON        = 421, // Cherrim
        SURFACE_SHELLOS_ICON        = 422, // Shellos
        SURFACE_GASTRODON_ICON      = 423, // Gastrodon
        SURFACE_AMBIPOM_ICON        = 424, // Ambipom
        SURFACE_DRIFLOON_ICON       = 425, // Drifloon
        SURFACE_DRIFBLIM_ICON       = 426, // Drifblim
        SURFACE_BUNEARY_ICON        = 427, // Buneary
        SURFACE_LOPUNNY_ICON        = 428, // Lopunny
        SURFACE_MISMAGIUS_ICON      = 429, // Mismagius
        SURFACE_HONCHKROW_ICON      = 430, // Honchkrow
        SURFACE_GLAMEOW_ICON        = 431, // Glameow
        SURFACE_PURUGLY_ICON        = 432, // Purugly
        SURFACE_CHINGLING_ICON      = 433, // Chingling
        SURFACE_STUNKY_ICON         = 434, // Stunky
        SURFACE_SKUNTANK_ICON       = 435, // Skuntank
        SURFACE_BRONZOR_ICON        = 436, // Bronzor
        SURFACE_BRONZONG_ICON       = 437, // Bronzong
        SURFACE_BONSLY_ICON         = 438, // Bonsly
        SURFACE_MIMEJR_ICON         = 439, // Mime Jr.
        SURFACE_HAPPINY_ICON        = 440, // Happiny
        SURFACE_CHATOT_ICON         = 441, // Chatot
        SURFACE_SPIRITOMB_ICON      = 442, // Spiritomb
        SURFACE_GIBLE_ICON          = 443, // Gible
        SURFACE_GABITE_ICON         = 444, // Gabite
        SURFACE_GARCHOMP_ICON       = 445, // Garchomp
        SURFACE_MUNCHLAX_ICON       = 446, // Munchlax
        SURFACE_RIOLU_ICON          = 447, // Riolu
        SURFACE_LUCARIO_ICON        = 448, // Lucario
        SURFACE_HIPPOPOTAS_ICON     = 449, // Hippopotas
        SURFACE_HIPPOWDON_ICON      = 450, // Hippowdon
        SURFACE_SKORUPI_ICON        = 451, // Skorupi
        SURFACE_DRAPION_ICON        = 452, // Drapion
        SURFACE_CROAGUNK_ICON       = 453, // Croagunk
        SURFACE_TOXICROAK_ICON      = 454, // Toxicroak
        SURFACE_CARNIVINE_ICON      = 455, // Carnivine
        SURFACE_FINNEON_ICON        = 456, // Finneon
        SURFACE_LUMINEON_ICON       = 457, // Lumineon
        SURFACE_MANTYKE_ICON        = 458, // Mantyke
        SURFACE_SNOVER_ICON         = 459, // Snover
        SURFACE_ABOMASNOW_ICON      = 460, // Abomasnow
        SURFACE_WEAVILE_ICON        = 461, // Weavile
        SURFACE_MAGNEZONE_ICON      = 462, // Magnezone
        SURFACE_LICKILICKY_ICON     = 463, // Lickilicky
        SURFACE_RHYPERIOR_ICON      = 464, // Rhyperior
        SURFACE_TANGROWTH_ICON      = 465, // Tangrowth
        SURFACE_ELECTIVIRE_ICON     = 466, // Electivire
        SURFACE_MAGMORTAR_ICON      = 467, // Magmortar
        SURFACE_TOGEKISS_ICON       = 468, // Togekiss
        SURFACE_YANMEGA_ICON        = 469, // Yanmega
        SURFACE_LEAFEON_ICON        = 470, // Leafeon
        SURFACE_GLACEON_ICON        = 471, // Glaceon
        SURFACE_GLISCOR_ICON        = 472, // Gliscor
        SURFACE_MAMOSWINE_ICON      = 473, // Mamoswine
        SURFACE_PORYGONZ_ICON       = 474, // Porygon-Z
        SURFACE_GALLADE_ICON        = 475, // Gallade
        SURFACE_PROBOPASS_ICON      = 476, // Probopass
        SURFACE_DUSKNOIR_ICON       = 477, // Dusknoir
        SURFACE_FROSLASS_ICON       = 478, // Froslass
        SURFACE_ROTOM_ICON          = 479, // Rotom
        SURFACE_UXIE_ICON           = 480, // Uxie
        SURFACE_MESPRIT_ICON        = 481, // Mesprit
        SURFACE_AZELF_ICON          = 482, // Azelf
        SURFACE_DIALGA_ICON         = 483, // Dialga
        SURFACE_PALKIA_ICON         = 484, // Palkia
        SURFACE_HEATRAN_ICON        = 485, // Heatran
        SURFACE_REGIGIGAS_ICON      = 486, // Regigigas
        SURFACE_GIRATINA_ICON       = 487, // Giratina
        SURFACE_CRESSELIA_ICON      = 488, // Cresselia
        SURFACE_PHIONE_ICON         = 489, // Phione
        SURFACE_MANAPHY_ICON        = 490, // Manaphy
        SURFACE_DARKRAI_ICON        = 491, // Darkrai
        SURFACE_SHAYMIN_LAND_ICON   = 492, // Shaymin
        SURFACE_ARCEUS_ICON         = 493, // Arceus

        // Unova Pokémon (Generation 5) - National Dex IDs 494–649
        // =========================================================
        SURFACE_VICTINI_ICON              = 494, // Victini
        SURFACE_SNIVY_ICON                = 495, // Snivy
        SURFACE_SERVINE_ICON              = 496, // Servine
        SURFACE_SERPERIOR_ICON            = 497, // Serperior
        SURFACE_TEPIG_ICON                = 498, // Tepig
        SURFACE_PIGNITE_ICON              = 499, // Pignite
        SURFACE_EMBOAR_ICON               = 500, // Emboar
        SURFACE_OSHAWOTT_ICON             = 501, // Oshawott
        SURFACE_DEWOTT_ICON               = 502, // Dewott
        SURFACE_SAMUROTT_ICON             = 503, // Samurott
        SURFACE_PATRAT_ICON               = 504, // Patrat
        SURFACE_WATCHOG_ICON              = 505, // Watchog
        SURFACE_LILLIPUP_ICON             = 506, // Lillipup
        SURFACE_HERDIER_ICON              = 507, // Herdier
        SURFACE_STOUTLAND_ICON            = 508, // Stoutland
        SURFACE_PURRLOIN_ICON             = 509, // Purrloin
        SURFACE_LIEPARD_ICON              = 510, // Liepard
        SURFACE_PANSAGE_ICON              = 511, // Pansage
        SURFACE_SIMISAGE_ICON             = 512, // Simisage
        SURFACE_PANSEAR_ICON              = 513, // Pansear
        SURFACE_SIMISEAR_ICON             = 514, // Simisear
        SURFACE_PANPOUR_ICON              = 515, // Panpour
        SURFACE_SIMIPOUR_ICON             = 516, // Simipour
        SURFACE_MUNNA_ICON                = 517, // Munna
        SURFACE_MUSHARNA_ICON             = 518, // Musharna
        SURFACE_PIDOVE_ICON               = 519, // Pidove
        SURFACE_TRANQUILL_ICON            = 520, // Tranquill
        SURFACE_UNFEZANT_ICON             = 521, // Unfezant
        SURFACE_BLITZLE_ICON              = 522, // Blitzle
        SURFACE_ZEBSTRIKA_ICON            = 523, // Zebstrika
        SURFACE_ROGGENROLA_ICON           = 524, // Roggenrola
        SURFACE_BOLDORE_ICON              = 525, // Boldore
        SURFACE_GIGALITH_ICON             = 526, // Gigalith
        SURFACE_WOOBAT_ICON               = 527, // Woobat
        SURFACE_SWOOBAT_ICON              = 528, // Swoobat
        SURFACE_DRILBUR_ICON              = 529, // Drilbur
        SURFACE_EXCADRILL_ICON            = 530, // Excadrill
        SURFACE_AUDINO_ICON               = 531, // Audino
        SURFACE_TIMBURR_ICON              = 532, // Timburr
        SURFACE_GURDURR_ICON              = 533, // Gurdurr
        SURFACE_CONKELDURR_ICON           = 534, // Conkeldurr
        SURFACE_TYMPOLE_ICON              = 535, // Tympole
        SURFACE_PALPITOAD_ICON            = 536, // Palpitoad
        SURFACE_SEISMITOAD_ICON           = 537, // Seismitoad
        SURFACE_THROH_ICON                = 538, // Throh
        SURFACE_SAWK_ICON                 = 539, // Sawk
        SURFACE_SEWADDLE_ICON             = 540, // Sewaddle
        SURFACE_SWADLOON_ICON             = 541, // Swadloon
        SURFACE_LEAVANNY_ICON             = 542, // Leavanny
        SURFACE_VENIPEDE_ICON             = 543, // Venipede
        SURFACE_WHIRLIPEDE_ICON           = 544, // Whirlipede
        SURFACE_SCOLIPEDE_ICON            = 545, // Scolipede
        SURFACE_COTTONEE_ICON             = 546, // Cottonee
        SURFACE_WHIMSICOTT_ICON           = 547, // Whimsicott
        SURFACE_PETILIL_ICON              = 548, // Petilil
        SURFACE_LILLIGANT_ICON            = 549, // Lilligant
        SURFACE_BASCULIN_RED_STRIPED_ICON = 550, // Basculin
        SURFACE_SANDILE_ICON              = 551, // Sandile
        SURFACE_KROKOROK_ICON             = 552, // Krokorok
        SURFACE_KROOKODILE_ICON           = 553, // Krookodile
        SURFACE_DARUMAKA_ICON             = 554, // Darumaka
        SURFACE_DARMANITAN_STANDARD_ICON  = 555, // Darmanitan
        SURFACE_MARACTUS_ICON             = 556, // Maractus
        SURFACE_DWEBBLE_ICON              = 557, // Dwebble
        SURFACE_CRUSTLE_ICON              = 558, // Crustle
        SURFACE_SCRAGGY_ICON              = 559, // Scraggy
        SURFACE_SCRAFTY_ICON              = 560, // Scrafty
        SURFACE_SIGILYPH_ICON             = 561, // Sigilyph
        SURFACE_YAMASK_ICON               = 562, // Yamask
        SURFACE_COFAGRIGUS_ICON           = 563, // Cofagrigus
        SURFACE_TIRTOUGA_ICON             = 564, // Tirtouga
        SURFACE_CARRACOSTA_ICON           = 565, // Carracosta
        SURFACE_ARCHEN_ICON               = 566, // Archen
        SURFACE_ARCHEOPS_ICON             = 567, // Archeops
        SURFACE_TRUBBISH_ICON             = 568, // Trubbish
        SURFACE_GARBODOR_ICON             = 569, // Garbodor
        SURFACE_ZORUA_ICON                = 570, // Zorua
        SURFACE_ZOROARK_ICON              = 571, // Zoroark
        SURFACE_MINCCINO_ICON             = 572, // Minccino
        SURFACE_CINCCINO_ICON             = 573, // Cinccino
        SURFACE_GOTHITA_ICON              = 574, // Gothita
        SURFACE_GOTHORITA_ICON            = 575, // Gothorita
        SURFACE_GOTHITELLE_ICON           = 576, // Gothitelle
        SURFACE_SOLOSIS_ICON              = 577, // Solosis
        SURFACE_DUOSION_ICON              = 578, // Duosion
        SURFACE_REUNICLUS_ICON            = 579, // Reuniclus
        SURFACE_DUCKLETT_ICON             = 580, // Ducklett
        SURFACE_SWANNA_ICON               = 581, // Swanna
        SURFACE_VANILLITE_ICON            = 582, // Vanillite
        SURFACE_VANILLISH_ICON            = 583, // Vanillish
        SURFACE_VANILLUXE_ICON            = 584, // Vanilluxe
        SURFACE_DEERLING_ICON             = 585, // Deerling
        SURFACE_SAWSBUCK_ICON             = 586, // Sawsbuck
        SURFACE_EMOLGA_ICON               = 587, // Emolga
        SURFACE_KARRABLAST_ICON           = 588, // Karrablast
        SURFACE_ESCAVALIER_ICON           = 589, // Escavalier
        SURFACE_FOONGUS_ICON              = 590, // Foongus
        SURFACE_AMOONGUSS_ICON            = 591, // Amoonguss
        SURFACE_FRILLISH_ICON             = 592, // Frillish
        SURFACE_JELLICENT_ICON            = 593, // Jellicent
        SURFACE_ALOMOMOLA_ICON            = 594, // Alomomola
        SURFACE_JOLTIK_ICON               = 595, // Joltik
        SURFACE_GALVANTULA_ICON           = 596, // Galvantula
        SURFACE_FERROSEED_ICON            = 597, // Ferroseed
        SURFACE_FERROTHORN_ICON           = 598, // Ferrothorn
        SURFACE_KLINK_ICON                = 599, // Klink
        SURFACE_KLANG_ICON                = 600, // Klang
        SURFACE_KLINKLANG_ICON            = 601, // Klinklang
        SURFACE_TYNAMO_ICON               = 602, // Tynamo
        SURFACE_EELEKTRIK_ICON            = 603, // Eelektrik
        SURFACE_EELEKTROSS_ICON           = 604, // Eelektross
        SURFACE_ELGYEM_ICON               = 605, // Elgyem
        SURFACE_BEHEEYEM_ICON             = 606, // Beheeyem
        SURFACE_LITWICK_ICON              = 607, // Litwick
        SURFACE_LAMPENT_ICON              = 608, // Lampent
        SURFACE_CHANDELURE_ICON           = 609, // Chandelure
        SURFACE_AXEW_ICON                 = 610, // Axew
        SURFACE_FRAXURE_ICON              = 611, // Fraxure
        SURFACE_HAXORUS_ICON              = 612, // Haxorus
        SURFACE_CUBCHOO_ICON              = 613, // Cubchoo
        SURFACE_BEARTIC_ICON              = 614, // Beartic
        SURFACE_CRYOGONAL_ICON            = 615, // Cryogonal
        SURFACE_SHELMET_ICON              = 616, // Shelmet
        SURFACE_ACCELGOR_ICON             = 617, // Accelgor
        SURFACE_STUNFISK_ICON             = 618, // Stunfisk
        SURFACE_MIENFOO_ICON              = 619, // Mienfoo
        SURFACE_MIENSHAO_ICON             = 620, // Mienshao
        SURFACE_DRUDDIGON_ICON            = 621, // Druddigon
        SURFACE_GOLETT_ICON               = 622, // Golett
        SURFACE_GOLURK_ICON               = 623, // Golurk
        SURFACE_PAWNIARD_ICON             = 624, // Pawniard
        SURFACE_BISHARP_ICON              = 625, // Bisharp
        SURFACE_BOUFFALANT_ICON           = 626, // Bouffalant
        SURFACE_RUFFLET_ICON              = 627, // Rufflet
        SURFACE_BRAVIARY_ICON             = 628, // Braviary
        SURFACE_VULLABY_ICON              = 629, // Vullaby
        SURFACE_MANDIBUZZ_ICON            = 630, // Mandibuzz
        SURFACE_HEATMOR_ICON              = 631, // Heatmor
        SURFACE_DURANT_ICON               = 632, // Durant
        SURFACE_DEINO_ICON                = 633, // Deino
        SURFACE_ZWEILOUS_ICON             = 634, // Zweilous
        SURFACE_HYDREIGON_ICON            = 635, // Hydreigon
        SURFACE_LARVESTA_ICON             = 636, // Larvesta
        SURFACE_VOLCARONA_ICON            = 637, // Volcarona
        SURFACE_COBALION_ICON             = 638, // Cobalion
        SURFACE_TERRAKION_ICON            = 639, // Terrakion
        SURFACE_VIRIZION_ICON             = 640, // Virizion
        SURFACE_TORNADUS_INCARNATE_ICON   = 641, // Tornadus
        SURFACE_THUNDURUS_INCARNATE_ICON  = 642, // Thundurus
        SURFACE_RESHIRAM_ICON             = 643, // Reshiram
        SURFACE_ZEKROM_ICON               = 644, // Zekrom
        SURFACE_LANDORUSINCARNATE_ICON    = 645, // Landorus
        SURFACE_KYUREM_ICON               = 646, // Kyurem
        SURFACE_KELDEOORDINARY_ICON       = 647, // Keldeo
        SURFACE_MELOETTAARIA_ICON         = 648, // Meloetta
        SURFACE_GENESECT_ICON             = 649, // Genesect

        //=================================================POKEMON_CRY================================================================

        /* // POKEMON CRY */
        /* AUDIO_POKEMON_001_CRY  = 1, */
        /* AUDIO_POKEMON_002_CRY  = 2, */
        /* AUDIO_POKEMON_003_CRY  = 3, */
        /* AUDIO_POKEMON_003M_CRY = 3 + 2000, */
        /* AUDIO_POKEMON_004_CRY  = 4, */
        /* AUDIO_POKEMON_005_CRY  = 5, */
        /* AUDIO_POKEMON_006_CRY  = 6, */
        /* AUDIO_POKEMON_006X_CRY = 6 + 2000, */
        /* AUDIO_POKEMON_006Y_CRY = 6 - 2000, */
        /* AUDIO_POKEMON_007_CRY  = 7, */
        /* AUDIO_POKEMON_008_CRY  = 8, */
        /* AUDIO_POKEMON_009_CRY  = 9, */
        /* AUDIO_POKEMON_009M_CRY = 9 + 2000, */
        /* AUDIO_POKEMON_010_CRY  = 10, */
        /* AUDIO_POKEMON_011_CRY  = 11, */
        /* AUDIO_POKEMON_012_CRY  = 12, */
        /* AUDIO_POKEMON_013_CRY  = 13, */
        /* AUDIO_POKEMON_014_CRY  = 14, */
        /* AUDIO_POKEMON_015_CRY  = 15, */
        /* AUDIO_POKEMON_015M_CRY = 15 + 2000, */
        /* AUDIO_POKEMON_016_CRY  = 16, */
        /* AUDIO_POKEMON_017_CRY  = 17, */
        /* AUDIO_POKEMON_018_CRY  = 18, */
        /* AUDIO_POKEMON_018M_CRY = 18 + 2000, */
        /* AUDIO_POKEMON_019_CRY  = 19, */
        /* AUDIO_POKEMON_020_CRY  = 20, */
        /* AUDIO_POKEMON_021_CRY  = 21, */
        /* AUDIO_POKEMON_022_CRY  = 22, */
        /* AUDIO_POKEMON_023_CRY  = 23, */
        /* AUDIO_POKEMON_024_CRY  = 24, */
        /* AUDIO_POKEMON_025_CRY  = 25, */
        /* AUDIO_POKEMON_026_CRY  = 26, */
        /* AUDIO_POKEMON_027_CRY  = 27, */
        /* AUDIO_POKEMON_028_CRY  = 28, */
        /* AUDIO_POKEMON_029_CRY  = 29, */
        /* AUDIO_POKEMON_030_CRY  = 30, */
        /* AUDIO_POKEMON_031_CRY  = 31, */
        /* AUDIO_POKEMON_032_CRY  = 32, */
        /* AUDIO_POKEMON_033_CRY  = 33, */
        /* AUDIO_POKEMON_034_CRY  = 34, */
        /* AUDIO_POKEMON_035_CRY  = 35, */
        /* AUDIO_POKEMON_036_CRY  = 36, */
        /* AUDIO_POKEMON_037_CRY  = 37, */
        /* AUDIO_POKEMON_038_CRY  = 38, */
        /* AUDIO_POKEMON_039_CRY  = 39, */
        /* AUDIO_POKEMON_040_CRY  = 40, */
        /* AUDIO_POKEMON_041_CRY  = 41, */
        /* AUDIO_POKEMON_042_CRY  = 42, */
        /* AUDIO_POKEMON_043_CRY  = 43, */
        /* AUDIO_POKEMON_044_CRY  = 44, */
        /* AUDIO_POKEMON_045_CRY  = 45, */
        /* AUDIO_POKEMON_046_CRY  = 46, */
        /* AUDIO_POKEMON_047_CRY  = 47, */
        /* AUDIO_POKEMON_048_CRY  = 48, */
        /* AUDIO_POKEMON_049_CRY  = 49, */
        /* AUDIO_POKEMON_050_CRY  = 50, */
        /* AUDIO_POKEMON_051_CRY  = 51, */
        /* AUDIO_POKEMON_052_CRY  = 52, */
        /* AUDIO_POKEMON_053_CRY  = 53, */
        /* AUDIO_POKEMON_054_CRY  = 54, */
        /* AUDIO_POKEMON_055_CRY  = 55, */
        /* AUDIO_POKEMON_056_CRY  = 56, */
        /* AUDIO_POKEMON_057_CRY  = 57, */
        /* AUDIO_POKEMON_058_CRY  = 58, */
        /* AUDIO_POKEMON_059_CRY  = 59, */
        /* AUDIO_POKEMON_060_CRY  = 60, */
        /* AUDIO_POKEMON_061_CRY  = 61, */
        /* AUDIO_POKEMON_062_CRY  = 62, */
        /* AUDIO_POKEMON_063_CRY  = 63, */
        /* AUDIO_POKEMON_064_CRY  = 64, */
        /* AUDIO_POKEMON_065_CRY  = 65, */
        /* AUDIO_POKEMON_065M_CRY = 65 + 2000, */
        /* AUDIO_POKEMON_066_CRY  = 66, */
        /* AUDIO_POKEMON_067_CRY  = 67, */
        /* AUDIO_POKEMON_068_CRY  = 68, */
        /* AUDIO_POKEMON_069_CRY  = 69, */
        /* AUDIO_POKEMON_070_CRY  = 70, */
        /* AUDIO_POKEMON_071_CRY  = 71, */
        /* AUDIO_POKEMON_072_CRY  = 72, */
        /* AUDIO_POKEMON_073_CRY  = 73, */
        /* AUDIO_POKEMON_074_CRY  = 74, */
        /* AUDIO_POKEMON_075_CRY  = 75, */
        /* AUDIO_POKEMON_076_CRY  = 76, */
        /* AUDIO_POKEMON_077_CRY  = 77, */
        /* AUDIO_POKEMON_078_CRY  = 78, */
        /* AUDIO_POKEMON_079_CRY  = 79, */
        /* AUDIO_POKEMON_080_CRY  = 80, */
        /* AUDIO_POKEMON_080M_CRY = 80 + 2000, */
        /* AUDIO_POKEMON_081_CRY  = 81, */
        /* AUDIO_POKEMON_082_CRY  = 82, */
        /* AUDIO_POKEMON_083_CRY  = 83, */
        /* AUDIO_POKEMON_084_CRY  = 84, */
        /* AUDIO_POKEMON_085_CRY  = 85, */
        /* AUDIO_POKEMON_086_CRY  = 86, */
        /* AUDIO_POKEMON_087_CRY  = 87, */
        /* AUDIO_POKEMON_088_CRY  = 88, */
        /* AUDIO_POKEMON_089_CRY  = 89, */
        /* AUDIO_POKEMON_090_CRY  = 90, */
        /* AUDIO_POKEMON_091_CRY  = 91, */
        /* AUDIO_POKEMON_092_CRY  = 92, */
        /* AUDIO_POKEMON_093_CRY  = 93, */
        /* AUDIO_POKEMON_094_CRY  = 94, */
        /* AUDIO_POKEMON_094M_CRY = 94 + 2000, */
        /* AUDIO_POKEMON_095_CRY  = 95, */
        /* AUDIO_POKEMON_096_CRY  = 96, */
        /* AUDIO_POKEMON_097_CRY  = 97, */
        /* AUDIO_POKEMON_098_CRY  = 98, */
        /* AUDIO_POKEMON_099_CRY  = 99, */
        /* AUDIO_POKEMON_100_CRY  = 100, */
        /* AUDIO_POKEMON_101_CRY  = 101, */
        /* AUDIO_POKEMON_102_CRY  = 102, */
        /* AUDIO_POKEMON_103_CRY  = 103, */
        /* AUDIO_POKEMON_104_CRY  = 104, */
        /* AUDIO_POKEMON_105_CRY  = 105, */
        /* AUDIO_POKEMON_106_CRY  = 106, */
        /* AUDIO_POKEMON_107_CRY  = 107, */
        /* AUDIO_POKEMON_108_CRY  = 108, */
        /* AUDIO_POKEMON_109_CRY  = 109, */
        /* AUDIO_POKEMON_110_CRY  = 110, */
        /* AUDIO_POKEMON_111_CRY  = 111, */
        /* AUDIO_POKEMON_112_CRY  = 112, */
        /* AUDIO_POKEMON_113_CRY  = 113, */
        /* AUDIO_POKEMON_114_CRY  = 114, */
        /* AUDIO_POKEMON_115_CRY  = 115, */
        /* AUDIO_POKEMON_115M_CRY = 115 + 2000, */
        /* AUDIO_POKEMON_116_CRY  = 116, */
        /* AUDIO_POKEMON_117_CRY  = 117, */
        /* AUDIO_POKEMON_118_CRY  = 118, */
        /* AUDIO_POKEMON_119_CRY  = 119, */
        /* AUDIO_POKEMON_120_CRY  = 120, */
        /* AUDIO_POKEMON_121_CRY  = 121, */
        /* AUDIO_POKEMON_122_CRY  = 122, */
        /* AUDIO_POKEMON_123_CRY  = 123, */
        /* AUDIO_POKEMON_124_CRY  = 124, */
        /* AUDIO_POKEMON_125_CRY  = 125, */
        /* AUDIO_POKEMON_126_CRY  = 126, */
        /* AUDIO_POKEMON_127_CRY  = 127, */
        /* AUDIO_POKEMON_127M_CRY = 127 + 2000, */
        /* AUDIO_POKEMON_128_CRY  = 128, */
        /* AUDIO_POKEMON_129_CRY  = 129, */
        /* AUDIO_POKEMON_130_CRY  = 130, */
        /* AUDIO_POKEMON_130M_CRY = 130 + 2000, */
        /* AUDIO_POKEMON_131_CRY  = 131, */
        /* AUDIO_POKEMON_132_CRY  = 132, */
        /* AUDIO_POKEMON_133_CRY  = 133, */
        /* AUDIO_POKEMON_134_CRY  = 134, */
        /* AUDIO_POKEMON_135_CRY  = 135, */
        /* AUDIO_POKEMON_136_CRY  = 136, */
        /* AUDIO_POKEMON_137_CRY  = 137, */
        /* AUDIO_POKEMON_138_CRY  = 138, */
        /* AUDIO_POKEMON_139_CRY  = 139, */
        /* AUDIO_POKEMON_140_CRY  = 140, */
        /* AUDIO_POKEMON_141_CRY  = 141, */
        /* AUDIO_POKEMON_142_CRY  = 142, */
        /* AUDIO_POKEMON_142M_CRY = 142 + 2000, */
        /* AUDIO_POKEMON_143_CRY  = 143, */
        /* AUDIO_POKEMON_144_CRY  = 144, */
        /* AUDIO_POKEMON_145_CRY  = 145, */
        /* AUDIO_POKEMON_146_CRY  = 146, */
        /* AUDIO_POKEMON_147_CRY  = 147, */
        /* AUDIO_POKEMON_148_CRY  = 148, */
        /* AUDIO_POKEMON_149_CRY  = 149, */
        /* AUDIO_POKEMON_150_CRY  = 150, */
        /* AUDIO_POKEMON_150X_CRY = 150 + 2000, */
        /* AUDIO_POKEMON_150Y_CRY = 150 - 2000, */
        /* AUDIO_POKEMON_151_CRY  = 151, */
        /* AUDIO_POKEMON_152_CRY  = 152, */
        /* AUDIO_POKEMON_153_CRY  = 153, */
        /* AUDIO_POKEMON_154_CRY  = 154, */
        /* AUDIO_POKEMON_155_CRY  = 155, */
        /* AUDIO_POKEMON_156_CRY  = 156, */
        /* AUDIO_POKEMON_157_CRY  = 157, */
        /* AUDIO_POKEMON_158_CRY  = 158, */
        /* AUDIO_POKEMON_159_CRY  = 159, */
        /* AUDIO_POKEMON_160_CRY  = 160, */
        /* AUDIO_POKEMON_161_CRY  = 161, */
        /* AUDIO_POKEMON_162_CRY  = 162, */
        /* AUDIO_POKEMON_163_CRY  = 163, */
        /* AUDIO_POKEMON_164_CRY  = 164, */
        /* AUDIO_POKEMON_165_CRY  = 165, */
        /* AUDIO_POKEMON_166_CRY  = 166, */
        /* AUDIO_POKEMON_167_CRY  = 167, */
        /* AUDIO_POKEMON_168_CRY  = 168, */
        /* AUDIO_POKEMON_169_CRY  = 169, */
        /* AUDIO_POKEMON_170_CRY  = 170, */
        /* AUDIO_POKEMON_171_CRY  = 171, */
        /* AUDIO_POKEMON_172_CRY  = 172, */
        /* AUDIO_POKEMON_173_CRY  = 173, */
        /* AUDIO_POKEMON_174_CRY  = 174, */
        /* AUDIO_POKEMON_175_CRY  = 175, */
        /* AUDIO_POKEMON_176_CRY  = 176, */
        /* AUDIO_POKEMON_177_CRY  = 177, */
        /* AUDIO_POKEMON_178_CRY  = 178, */
        /* AUDIO_POKEMON_179_CRY  = 179, */
        /* AUDIO_POKEMON_180_CRY  = 180, */
        /* AUDIO_POKEMON_181_CRY  = 181, */
        /* AUDIO_POKEMON_181M_CRY = 181 + 2000, */
        /* AUDIO_POKEMON_182_CRY  = 182, */
        /* AUDIO_POKEMON_183_CRY  = 183, */
        /* AUDIO_POKEMON_184_CRY  = 184, */
        /* AUDIO_POKEMON_185_CRY  = 185, */
        /* AUDIO_POKEMON_186_CRY  = 186, */
        /* AUDIO_POKEMON_187_CRY  = 187, */
        /* AUDIO_POKEMON_188_CRY  = 188, */
        /* AUDIO_POKEMON_189_CRY  = 189, */
        /* AUDIO_POKEMON_190_CRY  = 190, */
        /* AUDIO_POKEMON_191_CRY  = 191, */
        /* AUDIO_POKEMON_192_CRY  = 192, */
        /* AUDIO_POKEMON_193_CRY  = 193, */
        /* AUDIO_POKEMON_194_CRY  = 194, */
        /* AUDIO_POKEMON_195_CRY  = 195, */
        /* AUDIO_POKEMON_196_CRY  = 196, */
        /* AUDIO_POKEMON_197_CRY  = 197, */
        /* AUDIO_POKEMON_198_CRY  = 198, */
        /* AUDIO_POKEMON_199_CRY  = 199, */
        /* AUDIO_POKEMON_200_CRY  = 200, */
        /* AUDIO_POKEMON_201_CRY  = 201, */
        /* AUDIO_POKEMON_202_CRY  = 202, */
        /* AUDIO_POKEMON_203_CRY  = 203, */
        /* AUDIO_POKEMON_204_CRY  = 204, */
        /* AUDIO_POKEMON_205_CRY  = 205, */
        /* AUDIO_POKEMON_206_CRY  = 206, */
        /* AUDIO_POKEMON_207_CRY  = 207, */
        /* AUDIO_POKEMON_208_CRY  = 208, */
        /* AUDIO_POKEMON_208M_CRY = 208 + 2000, */
        /* AUDIO_POKEMON_209_CRY  = 209, */
        /* AUDIO_POKEMON_210_CRY  = 210, */
        /* AUDIO_POKEMON_211_CRY  = 211, */
        /* AUDIO_POKEMON_212_CRY  = 212, */
        /* AUDIO_POKEMON_212M_CRY = 212 + 2000, */
        /* AUDIO_POKEMON_213_CRY  = 213, */
        /* AUDIO_POKEMON_214_CRY  = 214, */
        /* AUDIO_POKEMON_214M_CRY = 214 + 2000, */
        /* AUDIO_POKEMON_215_CRY  = 215, */
        /* AUDIO_POKEMON_216_CRY  = 216, */
        /* AUDIO_POKEMON_217_CRY  = 217, */
        /* AUDIO_POKEMON_218_CRY  = 218, */
        /* AUDIO_POKEMON_219_CRY  = 219, */
        /* AUDIO_POKEMON_220_CRY  = 220, */
        /* AUDIO_POKEMON_221_CRY  = 221, */
        /* AUDIO_POKEMON_222_CRY  = 222, */
        /* AUDIO_POKEMON_223_CRY  = 223, */
        /* AUDIO_POKEMON_224_CRY  = 224, */
        /* AUDIO_POKEMON_225_CRY  = 225, */
        /* AUDIO_POKEMON_226_CRY  = 226, */
        /* AUDIO_POKEMON_227_CRY  = 227, */
        /* AUDIO_POKEMON_228_CRY  = 228, */
        /* AUDIO_POKEMON_229_CRY  = 229, */
        /* AUDIO_POKEMON_229M_CRY = 229 + 2000, */
        /* AUDIO_POKEMON_230_CRY  = 230, */
        /* AUDIO_POKEMON_231_CRY  = 231, */
        /* AUDIO_POKEMON_232_CRY  = 232, */
        /* AUDIO_POKEMON_233_CRY  = 233, */
        /* AUDIO_POKEMON_234_CRY  = 234, */
        /* AUDIO_POKEMON_235_CRY  = 235, */
        /* AUDIO_POKEMON_236_CRY  = 236, */
        /* AUDIO_POKEMON_237_CRY  = 237, */
        /* AUDIO_POKEMON_238_CRY  = 238, */
        /* AUDIO_POKEMON_239_CRY  = 239, */
        /* AUDIO_POKEMON_240_CRY  = 240, */
        /* AUDIO_POKEMON_241_CRY  = 241, */
        /* AUDIO_POKEMON_242_CRY  = 242, */
        /* AUDIO_POKEMON_243_CRY  = 243, */
        /* AUDIO_POKEMON_244_CRY  = 244, */
        /* AUDIO_POKEMON_245_CRY  = 245, */
        /* AUDIO_POKEMON_246_CRY  = 246, */
        /* AUDIO_POKEMON_247_CRY  = 247, */
        /* AUDIO_POKEMON_248_CRY  = 248, */
        /* AUDIO_POKEMON_248M_CRY = 248 + 2000, */
        /* AUDIO_POKEMON_249_CRY  = 249, */
        /* AUDIO_POKEMON_250_CRY  = 250, */
        /* AUDIO_POKEMON_251_CRY  = 251, */
        /* AUDIO_POKEMON_252_CRY  = 252, */
        /* AUDIO_POKEMON_253_CRY  = 253, */
        /* AUDIO_POKEMON_254_CRY  = 254, */
        /* AUDIO_POKEMON_254M_CRY = 254 + 2000, */
        /* AUDIO_POKEMON_255_CRY  = 255, */
        /* AUDIO_POKEMON_256_CRY  = 256, */
        /* AUDIO_POKEMON_257_CRY  = 257, */
        /* AUDIO_POKEMON_257M_CRY = 257 + 2000, */
        /* AUDIO_POKEMON_258_CRY  = 258, */
        /* AUDIO_POKEMON_259_CRY  = 259, */
        /* AUDIO_POKEMON_260_CRY  = 260, */
        /* AUDIO_POKEMON_260M_CRY = 260 + 2000, */
        /* AUDIO_POKEMON_261_CRY  = 261, */
        /* AUDIO_POKEMON_262_CRY  = 262, */
        /* AUDIO_POKEMON_263_CRY  = 263, */
        /* AUDIO_POKEMON_264_CRY  = 264, */
        /* AUDIO_POKEMON_265_CRY  = 265, */
        /* AUDIO_POKEMON_266_CRY  = 266, */
        /* AUDIO_POKEMON_267_CRY  = 267, */
        /* AUDIO_POKEMON_268_CRY  = 268, */
        /* AUDIO_POKEMON_269_CRY  = 269, */
        /* AUDIO_POKEMON_270_CRY  = 270, */
        /* AUDIO_POKEMON_271_CRY  = 271, */
        /* AUDIO_POKEMON_272_CRY  = 272, */
        /* AUDIO_POKEMON_273_CRY  = 273, */
        /* AUDIO_POKEMON_274_CRY  = 274, */
        /* AUDIO_POKEMON_275_CRY  = 275, */
        /* AUDIO_POKEMON_276_CRY  = 276, */
        /* AUDIO_POKEMON_277_CRY  = 277, */
        /* AUDIO_POKEMON_278_CRY  = 278, */
        /* AUDIO_POKEMON_279_CRY  = 279, */
        /* AUDIO_POKEMON_280_CRY  = 280, */
        /* AUDIO_POKEMON_281_CRY  = 281, */
        /* AUDIO_POKEMON_282_CRY  = 282, */
        /* AUDIO_POKEMON_282M_CRY = 282 + 2000, */
        /* AUDIO_POKEMON_283_CRY  = 283, */
        /* AUDIO_POKEMON_284_CRY  = 284, */
        /* AUDIO_POKEMON_285_CRY  = 285, */
        /* AUDIO_POKEMON_286_CRY  = 286, */
        /* AUDIO_POKEMON_287_CRY  = 287, */
        /* AUDIO_POKEMON_288_CRY  = 288, */
        /* AUDIO_POKEMON_289_CRY  = 289, */
        /* AUDIO_POKEMON_290_CRY  = 290, */
        /* AUDIO_POKEMON_291_CRY  = 291, */
        /* AUDIO_POKEMON_292_CRY  = 292, */
        /* AUDIO_POKEMON_293_CRY  = 293, */
        /* AUDIO_POKEMON_294_CRY  = 294, */
        /* AUDIO_POKEMON_295_CRY  = 295, */
        /* AUDIO_POKEMON_296_CRY  = 296, */
        /* AUDIO_POKEMON_297_CRY  = 297, */
        /* AUDIO_POKEMON_298_CRY  = 298, */
        /* AUDIO_POKEMON_299_CRY  = 299, */
        /* AUDIO_POKEMON_300_CRY  = 300, */
        /* AUDIO_POKEMON_301_CRY  = 301, */
        /* AUDIO_POKEMON_302_CRY  = 302, */
        /* AUDIO_POKEMON_302M_CRY = 302 + 2000, */
        /* AUDIO_POKEMON_303_CRY  = 303, */
        /* AUDIO_POKEMON_303M_CRY = 303 + 2000, */
        /* AUDIO_POKEMON_304_CRY  = 304, */
        /* AUDIO_POKEMON_305_CRY  = 305, */
        /* AUDIO_POKEMON_306_CRY  = 306, */
        /* AUDIO_POKEMON_306M_CRY = 306 + 2000, */
        /* AUDIO_POKEMON_307_CRY  = 307, */
        /* AUDIO_POKEMON_308_CRY  = 308, */
        /* AUDIO_POKEMON_308M_CRY = 308 + 2000, */
        /* AUDIO_POKEMON_309_CRY  = 309, */
        /* AUDIO_POKEMON_310_CRY  = 310, */
        /* AUDIO_POKEMON_310M_CRY = 310 + 2000, */
        /* AUDIO_POKEMON_311_CRY  = 311, */
        /* AUDIO_POKEMON_312_CRY  = 312, */
        /* AUDIO_POKEMON_313_CRY  = 313, */
        /* AUDIO_POKEMON_314_CRY  = 314, */
        /* AUDIO_POKEMON_315_CRY  = 315, */
        /* AUDIO_POKEMON_316_CRY  = 316, */
        /* AUDIO_POKEMON_317_CRY  = 317, */
        /* AUDIO_POKEMON_318_CRY  = 318, */
        /* AUDIO_POKEMON_319_CRY  = 319, */
        /* AUDIO_POKEMON_319M_CRY = 319 + 2000, */
        /* AUDIO_POKEMON_320_CRY  = 320, */
        /* AUDIO_POKEMON_321_CRY  = 321, */
        /* AUDIO_POKEMON_322_CRY  = 322, */
        /* AUDIO_POKEMON_323_CRY  = 323, */
        /* AUDIO_POKEMON_323M_CRY = 323 + 2000, */
        /* AUDIO_POKEMON_324_CRY  = 324, */
        /* AUDIO_POKEMON_325_CRY  = 325, */
        /* AUDIO_POKEMON_326_CRY  = 326, */
        /* AUDIO_POKEMON_327_CRY  = 327, */
        /* AUDIO_POKEMON_328_CRY  = 328, */
        /* AUDIO_POKEMON_329_CRY  = 329, */
        /* AUDIO_POKEMON_330_CRY  = 330, */
        /* AUDIO_POKEMON_331_CRY  = 331, */
        /* AUDIO_POKEMON_332_CRY  = 332, */
        /* AUDIO_POKEMON_333_CRY  = 333, */
        /* AUDIO_POKEMON_334_CRY  = 334, */
        /* AUDIO_POKEMON_334M_CRY = 334 + 2000, */
        /* AUDIO_POKEMON_335_CRY  = 335, */
        /* AUDIO_POKEMON_336_CRY  = 336, */
        /* AUDIO_POKEMON_337_CRY  = 337, */
        /* AUDIO_POKEMON_338_CRY  = 338, */
        /* AUDIO_POKEMON_339_CRY  = 339, */
        /* AUDIO_POKEMON_340_CRY  = 340, */
        /* AUDIO_POKEMON_341_CRY  = 341, */
        /* AUDIO_POKEMON_342_CRY  = 342, */
        /* AUDIO_POKEMON_343_CRY  = 343, */
        /* AUDIO_POKEMON_344_CRY  = 344, */
        /* AUDIO_POKEMON_345_CRY  = 345, */
        /* AUDIO_POKEMON_346_CRY  = 346, */
        /* AUDIO_POKEMON_347_CRY  = 347, */
        /* AUDIO_POKEMON_348_CRY  = 348, */
        /* AUDIO_POKEMON_349_CRY  = 349, */
        /* AUDIO_POKEMON_350_CRY  = 350, */
        /* AUDIO_POKEMON_351_CRY  = 351, */
        /* AUDIO_POKEMON_352_CRY  = 352, */
        /* AUDIO_POKEMON_353_CRY  = 353, */
        /* AUDIO_POKEMON_354_CRY  = 354, */
        /* AUDIO_POKEMON_354M_CRY = 354 + 2000, */
        /* AUDIO_POKEMON_355_CRY  = 355, */
        /* AUDIO_POKEMON_356_CRY  = 356, */
        /* AUDIO_POKEMON_357_CRY  = 357, */
        /* AUDIO_POKEMON_358_CRY  = 358, */
        /* AUDIO_POKEMON_359_CRY  = 359, */
        /* AUDIO_POKEMON_359M_CRY = 359 + 2000, */
        /* AUDIO_POKEMON_360_CRY  = 360, */
        /* AUDIO_POKEMON_361_CRY  = 361, */
        /* AUDIO_POKEMON_362_CRY  = 362, */
        /* AUDIO_POKEMON_362M_CRY = 362 + 2000, */
        /* AUDIO_POKEMON_363_CRY  = 363, */
        /* AUDIO_POKEMON_364_CRY  = 364, */
        /* AUDIO_POKEMON_365_CRY  = 365, */
        /* AUDIO_POKEMON_366_CRY  = 366, */
        /* AUDIO_POKEMON_367_CRY  = 367, */
        /* AUDIO_POKEMON_368_CRY  = 368, */
        /* AUDIO_POKEMON_369_CRY  = 369, */
        /* AUDIO_POKEMON_370_CRY  = 370, */
        /* AUDIO_POKEMON_371_CRY  = 371, */
        /* AUDIO_POKEMON_372_CRY  = 372, */
        /* AUDIO_POKEMON_373_CRY  = 373, */
        /* AUDIO_POKEMON_373M_CRY = 373 + 2000, */
        /* AUDIO_POKEMON_374_CRY  = 374, */
        /* AUDIO_POKEMON_375_CRY  = 375, */
        /* AUDIO_POKEMON_376_CRY  = 376, */
        /* AUDIO_POKEMON_376M_CRY = 376 + 2000, */
        /* AUDIO_POKEMON_377_CRY  = 377, */
        /* AUDIO_POKEMON_378_CRY  = 378, */
        /* AUDIO_POKEMON_379_CRY  = 379, */
        /* AUDIO_POKEMON_380_CRY  = 380, */
        /* AUDIO_POKEMON_380M_CRY = 380 + 2000, */
        /* AUDIO_POKEMON_381_CRY  = 381, */
        /* AUDIO_POKEMON_381M_CRY = 381 + 2000, */
        /* AUDIO_POKEMON_382_CRY  = 382, */
        /* AUDIO_POKEMON_382P_CRY = 382 + 2000, */
        /* AUDIO_POKEMON_383_CRY  = 383, */
        /* AUDIO_POKEMON_383P_CRY = 383 + 2000, */
        /* AUDIO_POKEMON_384_CRY  = 384, */
        /* AUDIO_POKEMON_384M_CRY = 384 + 2000, */
        /* AUDIO_POKEMON_385_CRY  = 385, */
        /* AUDIO_POKEMON_386_CRY  = 386, */
        /* AUDIO_POKEMON_387_CRY  = 387, */
        /* AUDIO_POKEMON_388_CRY  = 388, */
        /* AUDIO_POKEMON_389_CRY  = 389, */
        /* AUDIO_POKEMON_390_CRY  = 390, */
        /* AUDIO_POKEMON_391_CRY  = 391, */
        /* AUDIO_POKEMON_392_CRY  = 392, */
        /* AUDIO_POKEMON_393_CRY  = 393, */
        /* AUDIO_POKEMON_394_CRY  = 394, */
        /* AUDIO_POKEMON_395_CRY  = 395, */
        /* AUDIO_POKEMON_396_CRY  = 396, */
        /* AUDIO_POKEMON_397_CRY  = 397, */
        /* AUDIO_POKEMON_398_CRY  = 398, */
        /* AUDIO_POKEMON_399_CRY  = 399, */
        /* AUDIO_POKEMON_400_CRY  = 400, */
        /* AUDIO_POKEMON_401_CRY  = 401, */
        /* AUDIO_POKEMON_402_CRY  = 402, */
        /* AUDIO_POKEMON_403_CRY  = 403, */
        /* AUDIO_POKEMON_404_CRY  = 404, */
        /* AUDIO_POKEMON_405_CRY  = 405, */
        /* AUDIO_POKEMON_406_CRY  = 406, */
        /* AUDIO_POKEMON_407_CRY  = 407, */
        /* AUDIO_POKEMON_408_CRY  = 408, */
        /* AUDIO_POKEMON_409_CRY  = 409, */
        /* AUDIO_POKEMON_410_CRY  = 410, */
        /* AUDIO_POKEMON_411_CRY  = 411, */
        /* AUDIO_POKEMON_412_CRY  = 412, */
        /* AUDIO_POKEMON_413_CRY  = 413, */
        /* AUDIO_POKEMON_414_CRY  = 414, */
        /* AUDIO_POKEMON_415_CRY  = 415, */
        /* AUDIO_POKEMON_416_CRY  = 416, */
        /* AUDIO_POKEMON_417_CRY  = 417, */
        /* AUDIO_POKEMON_418_CRY  = 418, */
        /* AUDIO_POKEMON_419_CRY  = 419, */
        /* AUDIO_POKEMON_420_CRY  = 420, */
        /* AUDIO_POKEMON_421_CRY  = 421, */
        /* AUDIO_POKEMON_422_CRY  = 422, */
        /* AUDIO_POKEMON_423_CRY  = 423, */
        /* AUDIO_POKEMON_424_CRY  = 424, */
        /* AUDIO_POKEMON_425_CRY  = 425, */
        /* AUDIO_POKEMON_426_CRY  = 426, */
        /* AUDIO_POKEMON_427_CRY  = 427, */
        /* AUDIO_POKEMON_428_CRY  = 428, */
        /* AUDIO_POKEMON_428M_CRY = 428 + 2000, */
        /* AUDIO_POKEMON_429_CRY  = 429, */
        /* AUDIO_POKEMON_430_CRY  = 430, */
        /* AUDIO_POKEMON_431_CRY  = 431, */
        /* AUDIO_POKEMON_432_CRY  = 432, */
        /* AUDIO_POKEMON_433_CRY  = 433, */
        /* AUDIO_POKEMON_434_CRY  = 434, */
        /* AUDIO_POKEMON_435_CRY  = 435, */
        /* AUDIO_POKEMON_436_CRY  = 436, */
        /* AUDIO_POKEMON_437_CRY  = 437, */
        /* AUDIO_POKEMON_438_CRY  = 438, */
        /* AUDIO_POKEMON_439_CRY  = 439, */
        /* AUDIO_POKEMON_440_CRY  = 440, */
        /* AUDIO_POKEMON_441_CRY  = 441, */
        /* AUDIO_POKEMON_442_CRY  = 442, */
        /* AUDIO_POKEMON_443_CRY  = 443, */
        /* AUDIO_POKEMON_444_CRY  = 444, */
        /* AUDIO_POKEMON_445_CRY  = 445, */
        /* AUDIO_POKEMON_445M_CRY = 445 + 2000, */
        /* AUDIO_POKEMON_446_CRY  = 446, */
        /* AUDIO_POKEMON_447_CRY  = 447, */
        /* AUDIO_POKEMON_448_CRY  = 448, */
        /* AUDIO_POKEMON_448M_CRY = 448 + 2000, */
        /* AUDIO_POKEMON_449_CRY  = 449, */
        /* AUDIO_POKEMON_450_CRY  = 450, */
        /* AUDIO_POKEMON_451_CRY  = 451, */
        /* AUDIO_POKEMON_452_CRY  = 452, */
        /* AUDIO_POKEMON_453_CRY  = 453, */
        /* AUDIO_POKEMON_454_CRY  = 454, */
        /* AUDIO_POKEMON_455_CRY  = 455, */
        /* AUDIO_POKEMON_456_CRY  = 456, */
        /* AUDIO_POKEMON_457_CRY  = 457, */
        /* AUDIO_POKEMON_458_CRY  = 458, */
        /* AUDIO_POKEMON_459_CRY  = 459, */
        /* AUDIO_POKEMON_460_CRY  = 460, */
        /* AUDIO_POKEMON_460M_CRY = 460 + 2000, */
        /* AUDIO_POKEMON_461_CRY  = 461, */
        /* AUDIO_POKEMON_462_CRY  = 462, */
        /* AUDIO_POKEMON_463_CRY  = 463, */
        /* AUDIO_POKEMON_464_CRY  = 464, */
        /* AUDIO_POKEMON_465_CRY  = 465, */
        /* AUDIO_POKEMON_466_CRY  = 466, */
        /* AUDIO_POKEMON_467_CRY  = 467, */
        /* AUDIO_POKEMON_468_CRY  = 468, */
        /* AUDIO_POKEMON_469_CRY  = 469, */
        /* AUDIO_POKEMON_470_CRY  = 470, */
        /* AUDIO_POKEMON_471_CRY  = 471, */
        /* AUDIO_POKEMON_472_CRY  = 472, */
        /* AUDIO_POKEMON_473_CRY  = 473, */
        /* AUDIO_POKEMON_474_CRY  = 474, */
        /* AUDIO_POKEMON_475_CRY  = 475, */
        /* AUDIO_POKEMON_475M_CRY = 475 + 2000, */
        /* AUDIO_POKEMON_476_CRY  = 476, */
        /* AUDIO_POKEMON_477_CRY  = 477, */
        /* AUDIO_POKEMON_478_CRY  = 478, */
        /* AUDIO_POKEMON_479_CRY  = 479, */
        /* AUDIO_POKEMON_480_CRY  = 480, */
        /* AUDIO_POKEMON_481_CRY  = 481, */
        /* AUDIO_POKEMON_482_CRY  = 482, */
        /* AUDIO_POKEMON_483_CRY  = 483, */
        /* AUDIO_POKEMON_484_CRY  = 484, */
        /* AUDIO_POKEMON_485_CRY  = 485, */
        /* AUDIO_POKEMON_486_CRY  = 486, */
        /* AUDIO_POKEMON_487_CRY  = 487, */
        /* AUDIO_POKEMON_488_CRY  = 488, */
        /* AUDIO_POKEMON_489_CRY  = 489, */
        /* AUDIO_POKEMON_490_CRY  = 490, */
        /* AUDIO_POKEMON_491_CRY  = 491, */
        /* AUDIO_POKEMON_492_CRY  = 492, */
        /* AUDIO_POKEMON_492S_CRY = 492 + 2000, */
        /* AUDIO_POKEMON_493_CRY  = 493, */
        /* AUDIO_POKEMON_494_CRY  = 494, */
        /* AUDIO_POKEMON_495_CRY  = 495, */
        /* AUDIO_POKEMON_496_CRY  = 496, */
        /* AUDIO_POKEMON_497_CRY  = 497, */
        /* AUDIO_POKEMON_498_CRY  = 498, */
        /* AUDIO_POKEMON_499_CRY  = 499, */
        /* AUDIO_POKEMON_500_CRY  = 500, */
        /* AUDIO_POKEMON_501_CRY  = 501, */
        /* AUDIO_POKEMON_502_CRY  = 502, */
        /* AUDIO_POKEMON_503_CRY  = 503, */
        /* AUDIO_POKEMON_504_CRY  = 504, */
        /* AUDIO_POKEMON_505_CRY  = 505, */
        /* AUDIO_POKEMON_506_CRY  = 506, */
        /* AUDIO_POKEMON_507_CRY  = 507, */
        /* AUDIO_POKEMON_508_CRY  = 508, */
        /* AUDIO_POKEMON_509_CRY  = 509, */
        /* AUDIO_POKEMON_510_CRY  = 510, */
        /* AUDIO_POKEMON_511_CRY  = 511, */
        /* AUDIO_POKEMON_512_CRY  = 512, */
        /* AUDIO_POKEMON_513_CRY  = 513, */
        /* AUDIO_POKEMON_514_CRY  = 514, */
        /* AUDIO_POKEMON_515_CRY  = 515, */
        /* AUDIO_POKEMON_516_CRY  = 516, */
        /* AUDIO_POKEMON_517_CRY  = 517, */
        /* AUDIO_POKEMON_518_CRY  = 518, */
        /* AUDIO_POKEMON_519_CRY  = 519, */
        /* AUDIO_POKEMON_520_CRY  = 520, */
        /* AUDIO_POKEMON_521_CRY  = 521, */
        /* AUDIO_POKEMON_522_CRY  = 522, */
        /* AUDIO_POKEMON_523_CRY  = 523, */
        /* AUDIO_POKEMON_524_CRY  = 524, */
        /* AUDIO_POKEMON_525_CRY  = 525, */
        /* AUDIO_POKEMON_526_CRY  = 526, */
        /* AUDIO_POKEMON_527_CRY  = 527, */
        /* AUDIO_POKEMON_528_CRY  = 528, */
        /* AUDIO_POKEMON_529_CRY  = 529, */
        /* AUDIO_POKEMON_530_CRY  = 530, */
        /* AUDIO_POKEMON_531_CRY  = 531, */
        /* AUDIO_POKEMON_531M_CRY = 531 + 2000, */
        /* AUDIO_POKEMON_532_CRY  = 532, */
        /* AUDIO_POKEMON_533_CRY  = 533, */
        /* AUDIO_POKEMON_534_CRY  = 534, */
        /* AUDIO_POKEMON_535_CRY  = 535, */
        /* AUDIO_POKEMON_536_CRY  = 536, */
        /* AUDIO_POKEMON_537_CRY  = 537, */
        /* AUDIO_POKEMON_538_CRY  = 538, */
        /* AUDIO_POKEMON_539_CRY  = 539, */
        /* AUDIO_POKEMON_540_CRY  = 540, */
        /* AUDIO_POKEMON_541_CRY  = 541, */
        /* AUDIO_POKEMON_542_CRY  = 542, */
        /* AUDIO_POKEMON_543_CRY  = 543, */
        /* AUDIO_POKEMON_544_CRY  = 544, */
        /* AUDIO_POKEMON_545_CRY  = 545, */
        /* AUDIO_POKEMON_546_CRY  = 546, */
        /* AUDIO_POKEMON_547_CRY  = 547, */
        /* AUDIO_POKEMON_548_CRY  = 548, */
        /* AUDIO_POKEMON_549_CRY  = 549, */
        /* AUDIO_POKEMON_550_CRY  = 550, */
        /* AUDIO_POKEMON_551_CRY  = 551, */
        /* AUDIO_POKEMON_552_CRY  = 552, */
        /* AUDIO_POKEMON_553_CRY  = 553, */
        /* AUDIO_POKEMON_554_CRY  = 554, */
        /* AUDIO_POKEMON_555_CRY  = 555, */
        /* AUDIO_POKEMON_556_CRY  = 556, */
        /* AUDIO_POKEMON_557_CRY  = 557, */
        /* AUDIO_POKEMON_558_CRY  = 558, */
        /* AUDIO_POKEMON_559_CRY  = 559, */
        /* AUDIO_POKEMON_560_CRY  = 560, */
        /* AUDIO_POKEMON_561_CRY  = 561, */
        /* AUDIO_POKEMON_562_CRY  = 562, */
        /* AUDIO_POKEMON_563_CRY  = 563, */
        /* AUDIO_POKEMON_564_CRY  = 564, */
        /* AUDIO_POKEMON_565_CRY  = 565, */
        /* AUDIO_POKEMON_566_CRY  = 566, */
        /* AUDIO_POKEMON_567_CRY  = 567, */
        /* AUDIO_POKEMON_568_CRY  = 568, */
        /* AUDIO_POKEMON_569_CRY  = 569, */
        /* AUDIO_POKEMON_570_CRY  = 570, */
        /* AUDIO_POKEMON_571_CRY  = 571, */
        /* AUDIO_POKEMON_572_CRY  = 572, */
        /* AUDIO_POKEMON_573_CRY  = 573, */
        /* AUDIO_POKEMON_574_CRY  = 574, */
        /* AUDIO_POKEMON_575_CRY  = 575, */
        /* AUDIO_POKEMON_576_CRY  = 576, */
        /* AUDIO_POKEMON_577_CRY  = 577, */
        /* AUDIO_POKEMON_578_CRY  = 578, */
        /* AUDIO_POKEMON_579_CRY  = 579, */
        /* AUDIO_POKEMON_580_CRY  = 580, */
        /* AUDIO_POKEMON_581_CRY  = 581, */
        /* AUDIO_POKEMON_582_CRY  = 582, */
        /* AUDIO_POKEMON_583_CRY  = 583, */
        /* AUDIO_POKEMON_584_CRY  = 584, */
        /* AUDIO_POKEMON_585_CRY  = 585, */
        /* AUDIO_POKEMON_586_CRY  = 586, */
        /* AUDIO_POKEMON_587_CRY  = 587, */
        /* AUDIO_POKEMON_588_CRY  = 588, */
        /* AUDIO_POKEMON_589_CRY  = 589, */
        /* AUDIO_POKEMON_590_CRY  = 590, */
        /* AUDIO_POKEMON_591_CRY  = 591, */
        /* AUDIO_POKEMON_592_CRY  = 592, */
        /* AUDIO_POKEMON_593_CRY  = 593, */
        /* AUDIO_POKEMON_594_CRY  = 594, */
        /* AUDIO_POKEMON_595_CRY  = 595, */
        /* AUDIO_POKEMON_596_CRY  = 596, */
        /* AUDIO_POKEMON_597_CRY  = 597, */
        /* AUDIO_POKEMON_598_CRY  = 598, */
        /* AUDIO_POKEMON_599_CRY  = 599, */
        /* AUDIO_POKEMON_600_CRY  = 600, */
        /* AUDIO_POKEMON_601_CRY  = 601, */
        /* AUDIO_POKEMON_602_CRY  = 602, */
        /* AUDIO_POKEMON_603_CRY  = 603, */
        /* AUDIO_POKEMON_604_CRY  = 604, */
        /* AUDIO_POKEMON_605_CRY  = 605, */
        /* AUDIO_POKEMON_606_CRY  = 606, */
        /* AUDIO_POKEMON_607_CRY  = 607, */
        /* AUDIO_POKEMON_608_CRY  = 608, */
        /* AUDIO_POKEMON_609_CRY  = 609, */
        /* AUDIO_POKEMON_610_CRY  = 610, */
        /* AUDIO_POKEMON_611_CRY  = 611, */
        /* AUDIO_POKEMON_612_CRY  = 612, */
        /* AUDIO_POKEMON_613_CRY  = 613, */
        /* AUDIO_POKEMON_614_CRY  = 614, */
        /* AUDIO_POKEMON_615_CRY  = 615, */
        /* AUDIO_POKEMON_616_CRY  = 616, */
        /* AUDIO_POKEMON_617_CRY  = 617, */
        /* AUDIO_POKEMON_618_CRY  = 618, */
        /* AUDIO_POKEMON_619_CRY  = 619, */
        /* AUDIO_POKEMON_620_CRY  = 620, */
        /* AUDIO_POKEMON_621_CRY  = 621, */
        /* AUDIO_POKEMON_622_CRY  = 622, */
        /* AUDIO_POKEMON_623_CRY  = 623, */
        /* AUDIO_POKEMON_624_CRY  = 624, */
        /* AUDIO_POKEMON_625_CRY  = 625, */
        /* AUDIO_POKEMON_626_CRY  = 626, */
        /* AUDIO_POKEMON_627_CRY  = 627, */
        /* AUDIO_POKEMON_628_CRY  = 628, */
        /* AUDIO_POKEMON_629_CRY  = 629, */
        /* AUDIO_POKEMON_630_CRY  = 630, */
        /* AUDIO_POKEMON_631_CRY  = 631, */
        /* AUDIO_POKEMON_632_CRY  = 632, */
        /* AUDIO_POKEMON_633_CRY  = 633, */
        /* AUDIO_POKEMON_634_CRY  = 634, */
        /* AUDIO_POKEMON_635_CRY  = 635, */
        /* AUDIO_POKEMON_636_CRY  = 636, */
        /* AUDIO_POKEMON_637_CRY  = 637, */
        /* AUDIO_POKEMON_638_CRY  = 638, */
        /* AUDIO_POKEMON_639_CRY  = 639, */
        /* AUDIO_POKEMON_640_CRY  = 640, */
        /* AUDIO_POKEMON_641_CRY  = 641, */
        /* AUDIO_POKEMON_641T_CRY = 641 + 2000, */
        /* AUDIO_POKEMON_642_CRY  = 642, */
        /* AUDIO_POKEMON_642T_CRY = 642 + 2000, */
        /* AUDIO_POKEMON_643_CRY  = 643, */
        /* AUDIO_POKEMON_644_CRY  = 644, */
        /* AUDIO_POKEMON_645_CRY  = 645, */
        /* AUDIO_POKEMON_645T_CRY = 645 + 2000, */
        /* AUDIO_POKEMON_646_CRY  = 646, */
        /* AUDIO_POKEMON_646B_CRY = 646 + 2000, */
        /* AUDIO_POKEMON_646W_CRY = 646 - 2000, */
        /* AUDIO_POKEMON_647_CRY  = 647, */
        /* AUDIO_POKEMON_648_CRY  = 648, */
        /* AUDIO_POKEMON_649_CRY  = 649, */

        //=================================================ITEMS================================================================

        /* // ITEMS */
        SURFACE_ABILITY_URGE_ICON,
        SURFACE_ABSORB_BULB_ICON,
        SURFACE_ACRO_BIKE_ICON,
        SURFACE_ADAMANT_ORB_ICON,
        SURFACE_AGUAV_BERRY_ICON,
        SURFACE_AIR_BALLOON_ICON,
        SURFACE_AIR_MAIL_ICON,
        SURFACE_AMULET_COIN_ICON,
        SURFACE_ANTIDOTE_ICON,
        SURFACE_APICOT_BERRY_ICON,
        SURFACE_APRICORN_BOX_ICON,
        SURFACE_ARMOR_FOSSIL_ICON,
        SURFACE_ASPEAR_BERRY_ICON,
        SURFACE_AURORATICKET_ICON,
        SURFACE_AWAKENING_ICON,
        SURFACE_AZURE_FLUTE_ICON,
        SURFACE_BABIRI_BERRY_ICON,
        SURFACE_BALM_MUSHROOM_ICON,
        SURFACE_BASEMENT_KEY_ICON,
        SURFACE_BEAD_MAIL_ICON,
        SURFACE_BELUE_BERRY_ICON,
        SURFACE_BERRY_JUICE_ICON,
        SURFACE_BERRY_POTS_ICON,
        SURFACE_BERRY_POUCH_ICON,
        SURFACE_BICYCLE_ICON,
        SURFACE_BIG_MUSHROOM_ICON,
        SURFACE_BIG_NUGGET_ICON,
        SURFACE_BIG_PEARL_ICON,
        SURFACE_BIG_ROOT_ICON,
        SURFACE_BIKE_VOUCHER_ICON,
        SURFACE_BINDING_BAND_ICON,
        SURFACE_BLACK_APRICORN_ICON,
        SURFACE_BLACK_BELT_ICON,
        SURFACE_BLACK_FLUTE_ICON,
        SURFACE_BLACK_GLASSES_ICON,
        SURFACE_BLACK_SLUDGE_ICON,
        SURFACE_BLOOM_MAIL_ICON,
        SURFACE_BLUE_APRICORN_ICON,
        SURFACE_BLUE_CARD_ICON,
        SURFACE_BLUE_FLUTE_ICON,
        SURFACE_BLUE_ORB_ICON,
        SURFACE_BLUE_SCARF_ICON,
        SURFACE_BLUE_SHARD_ICON,
        SURFACE_BLUK_BERRY_ICON,
        SURFACE_BRICK_MAIL_ICON,
        SURFACE_BRIDGE_MAIL_D_ICON,
        SURFACE_BRIDGE_MAIL_M_ICON,
        SURFACE_BRIDGE_MAIL_S_ICON,
        SURFACE_BRIDGE_MAIL_T_ICON,
        SURFACE_BRIDGE_MAIL_V_ICON,
        SURFACE_BRIGHT_POWDER_ICON,
        SURFACE_BUBBLE_MAIL_ICON,
        SURFACE_BUG_GEM_ICON,
        SURFACE_BURN_DRIVE_ICON,
        SURFACE_BURN_HEAL_ICON,
        SURFACE_CALCIUM_ICON,
        SURFACE_CARBOS_ICON,
        SURFACE_CARD_KEY_ICON,
        SURFACE_CASTELIACONE_ICON,
        SURFACE_CELL_BATTERY_ICON,
        SURFACE_CHARCOAL_ICON,
        SURFACE_CHARTI_BERRY_ICON,
        SURFACE_CHERI_BERRY_ICON,
        SURFACE_CHERISH_BALL_ICON,
        SURFACE_CHESTO_BERRY_ICON,
        SURFACE_CHILAN_BERRY_ICON,
        SURFACE_CHILL_DRIVE_ICON,
        SURFACE_CHIPPED_POT_ICON,
        SURFACE_CHOICE_BAND_ICON,
        SURFACE_CHOICE_SCARF_ICON,
        SURFACE_CHOICE_SPECS_ICON,
        SURFACE_CHOPLE_BERRY_ICON,
        SURFACE_CLAW_FOSSIL_ICON,
        SURFACE_CLEANSE_TAG_ICON,
        SURFACE_CLEAR_BELL_ICON,
        SURFACE_CLEVER_WING_ICON,
        SURFACE_CLOVER_SWEET_ICON,
        SURFACE_COBA_BERRY_ICON,
        SURFACE_COIN_CASE_ICON,
        SURFACE_COLBUR_BERRY_ICON,
        SURFACE_COLRESS_MACHINE_ICON,
        SURFACE_COMET_SHARD_ICON,
        SURFACE_CONTEST_PASS_ICON,
        SURFACE_CORNN_BERRY_ICON,
        SURFACE_COUPON_1_ICON,
        SURFACE_COUPON_2_ICON,
        SURFACE_COUPON_3_ICON,
        SURFACE_COVER_FOSSIL_ICON,
        SURFACE_CRACKED_POT_ICON,
        SURFACE_CUSTAP_BERRY_ICON,
        SURFACE_DAMP_MULCH_ICON,
        SURFACE_DAMP_ROCK_ICON,
        SURFACE_DARK_GEM_ICON,
        SURFACE_DARK_STONE_ICON,
        SURFACE_DATA_CARD_ICON,
        SURFACE_DAWN_STONE_ICON,
        SURFACE_DEEP_SEA_SCALE_ICON,
        SURFACE_DEEP_SEA_TOOTH_ICON,
        SURFACE_DESTINY_KNOT_ICON,
        SURFACE_DEVON_GOODS_ICON,
        SURFACE_DEVON_SCOPE_ICON,
        SURFACE_DIRE_HIT_2_ICON,
        SURFACE_DIRE_HIT_3_ICON,
        SURFACE_DIRE_HIT_ICON,
        SURFACE_DIVE_BALL_ICON,
        SURFACE_DNA_SPLICERS_ICON,
        SURFACE_DOME_FOSSIL_ICON,
        SURFACE_DOUSE_DRIVE_ICON,
        SURFACE_DOWSING_MACHINE_ICON,
        SURFACE_DRACO_PLATE_ICON,
        SURFACE_DRAGON_FANG_ICON,
        SURFACE_DRAGON_GEM_ICON,
        SURFACE_DRAGON_SCALE_ICON,
        SURFACE_DRAGON_SKULL_ICON,
        SURFACE_DREAD_PLATE_ICON,
        SURFACE_DREAM_BALL_ICON,
        SURFACE_DREAM_MAIL_ICON,
        SURFACE_DROPPED_ITEM_ICON,
        SURFACE_DUBIOUS_DISC_ICON,
        SURFACE_DURIN_BERRY_ICON,
        SURFACE_DUSK_BALL_ICON,
        SURFACE_DUSK_STONE_ICON,
        SURFACE_EARTH_PLATE_ICON,
        SURFACE_EJECT_BUTTON_ICON,
        SURFACE_ELECTIRIZER_ICON,
        SURFACE_ELECTRIC_GEM_ICON,
        SURFACE_ELIXIR_ICON,
        SURFACE_ENERGY_POWDER_ICON,
        SURFACE_ENERGY_ROOT_ICON,
        SURFACE_ENIGMA_BERRY_ICON,
        SURFACE_ENIGMA_STONE_ICON,
        SURFACE_EON_TICKET_ICON,
        SURFACE_ESCAPE_ROPE_ICON,
        SURFACE_ETHER_ICON,
        SURFACE_EVERSTONE_ICON,
        SURFACE_EVIOLITE_ICON,
        SURFACE_EXP_SHARE_ICON,
        SURFACE_EXPERT_BELT_ICON,
        SURFACE_EXPLORER_KIT_ICON,
        SURFACE_FAB_MAIL_ICON,
        SURFACE_FAME_CHECKER_ICON,
        SURFACE_FASHION_CASE_ICON,
        SURFACE_FAST_BALL_ICON,
        SURFACE_FAVORED_MAIL_ICON,
        SURFACE_FIGHTING_GEM_ICON,
        SURFACE_FIGY_BERRY_ICON,
        SURFACE_FIRE_GEM_ICON,
        SURFACE_FIRE_STONE_ICON,
        SURFACE_FIST_PLATE_ICON,
        SURFACE_FLAME_MAIL_ICON,
        SURFACE_FLAME_ORB_ICON,
        SURFACE_FLAME_PLATE_ICON,
        SURFACE_FLOAT_STONE_ICON,
        SURFACE_FLOWER_SWEET_ICON,
        SURFACE_FLUFFY_TAIL_ICON,
        SURFACE_FLYING_GEM_ICON,
        SURFACE_FOCUS_BAND_ICON,
        SURFACE_FOCUS_SASH_ICON,
        SURFACE_FRESH_WATER_ICON,
        SURFACE_FRIEND_BALL_ICON,
        SURFACE_FULL_HEAL_ICON,
        SURFACE_FULL_INCENSE_ICON,
        SURFACE_FULL_RESTORE_ICON,
        SURFACE_GALACTIC_KEY_ICON,
        SURFACE_GANLON_BERRY_ICON,
        SURFACE_GB_SOUNDS_ICON,
        SURFACE_GENIUS_WING_ICON,
        SURFACE_GHOST_GEM_ICON,
        SURFACE_GLITTER_MAIL_ICON,
        SURFACE_GO_GOGGLES_ICON,
        SURFACE_GOD_STONE_ICON,
        SURFACE_GOLD_TEETH_ICON,
        // SURFACE_GOOD_ROD_ICON,
        SURFACE_GOOEY_MULCH_ICON,
        SURFACE_GRACIDEA_ICON,
        SURFACE_GRAM_1_ICON,
        SURFACE_GRAM_2_ICON,
        SURFACE_GRAM_3_ICON,
        SURFACE_GRASS_GEM_ICON,
        SURFACE_GRASS_MAIL_ICON,
        SURFACE_GREAT_BALL_ICON,
        SURFACE_GREEN_APRICORN_ICON,
        SURFACE_GREEN_SCARF_ICON,
        SURFACE_GREEN_SHARD_ICON,
        SURFACE_GREET_MAIL_ICON,
        SURFACE_GREPA_BERRY_ICON,
        SURFACE_GRIP_CLAW_ICON,
        SURFACE_GRISEOUS_ORB_ICON,
        SURFACE_GROUND_GEM_ICON,
        SURFACE_GROWTH_MULCH_ICON,
        SURFACE_GRUBBY_HANKY_ICON,
        SURFACE_GUARD_SPEC_ICON,
        SURFACE_HABAN_BERRY_ICON,
        SURFACE_HARBOR_MAIL_ICON,
        SURFACE_HARD_STONE_ICON,
        SURFACE_HEAL_BALL_ICON,
        SURFACE_HEAL_POWDER_ICON,
        SURFACE_HEALTH_WING_ICON,
        SURFACE_HEART_MAIL_ICON,
        SURFACE_HEART_SCALE_ICON,
        SURFACE_HEAT_ROCK_ICON,
        SURFACE_HEAVY_BALL_ICON,
        SURFACE_HELIX_FOSSIL_ICON,
        SURFACE_HM_BUG_ICON,
        SURFACE_HM_DARK_ICON,
        SURFACE_HM_DRAGON_ICON,
        SURFACE_HM_ELECTRIC_ICON,
        SURFACE_HM_FIGHTING_ICON,
        SURFACE_HM_FIRE_ICON,
        SURFACE_HM_FLYING_ICON,
        SURFACE_HM_GHOST_ICON,
        SURFACE_HM_GRASS_ICON,
        SURFACE_HM_GROUND_ICON,
        SURFACE_HM_ICE_ICON,
        SURFACE_HM_NORMAL_ICON,
        SURFACE_HM_POISON_ICON,
        SURFACE_HM_PSYCHIC_ICON,
        SURFACE_HM_ROCK_ICON,
        SURFACE_HM_STEEL_ICON,
        SURFACE_HM_WATER_ICON,
        SURFACE_HONDEW_BERRY_ICON,
        SURFACE_HONEY_ICON,
        SURFACE_HP_UP_ICON,
        SURFACE_HYPER_POTION_ICON,
        SURFACE_IAPAPA_BERRY_ICON,
        SURFACE_ICE_GEM_ICON,
        SURFACE_ICE_HEAL_ICON,
        SURFACE_ICE_STONE_ICON,
        SURFACE_ICICLE_PLATE_ICON,
        SURFACE_ICY_ROCK_ICON,
        SURFACE_INQUIRY_MAIL_ICON,
        SURFACE_INSECT_PLATE_ICON,
        SURFACE_IRON_BALL_ICON,
        SURFACE_IRON_PLATE_ICON,
        SURFACE_IRON_ICON,
        SURFACE_ITEM_DROP_ICON,
        SURFACE_ITEM_URGE_ICON,
        SURFACE_JABOCA_BERRY_ICON,
        SURFACE_JADE_ORB_ICON,
        SURFACE_JOURNAL_ICON,
        SURFACE_KASIB_BERRY_ICON,
        SURFACE_KEBIA_BERRY_ICON,
        SURFACE_KELPSY_BERRY_ICON,
        SURFACE_KINGS_ROCK_ICON,
        SURFACE_LAGGING_TAIL_ICON,
        SURFACE_LANSAT_BERRY_ICON,
        SURFACE_LAVA_COOKIE_ICON,
        SURFACE_LAX_INCENSE_ICON,
        SURFACE_LEAF_STONE_ICON,
        SURFACE_LEFTOVERS_ICON,
        SURFACE_LEMONADE_ICON,
        SURFACE_LEPPA_BERRY_ICON,
        SURFACE_LETTER_ICON,
        SURFACE_LEVEL_BALL_ICON,
        SURFACE_LIBERTY_PASS_ICON,
        SURFACE_LIECHI_BERRY_ICON,
        SURFACE_LIFE_ORB_ICON,
        SURFACE_LIFT_KEY_ICON,
        SURFACE_LIGHT_BALL_ICON,
        SURFACE_LIGHT_CLAY_ICON,
        SURFACE_LIGHT_STONE_ICON,
        SURFACE_LIKE_MAIL_ICON,
        SURFACE_LOCK_CAPSULE_ICON,
        SURFACE_LOOT_SACK_ICON,
        SURFACE_LOST_ITEM_ICON,
        SURFACE_LOVE_BALL_ICON,
        SURFACE_LOVE_SWEET_ICON,
        SURFACE_LUCK_INCENSE_ICON,
        SURFACE_LUCKY_EGG_ICON,
        SURFACE_LUCKY_PUNCH_ICON,
        SURFACE_LUM_BERRY_ICON,
        SURFACE_LUNAR_WING_ICON,
        SURFACE_LURE_BALL_ICON,
        SURFACE_LUSTROUS_ORB_ICON,
        SURFACE_LUXURY_BALL_ICON,
        SURFACE_MACH_BIKE_ICON,
        SURFACE_MACHINE_PART_ICON,
        SURFACE_MACHO_BRACE_ICON,
        SURFACE_MAGMA_EMBLEM_ICON,
        SURFACE_MAGMA_STONE_ICON,
        SURFACE_MAGMARIZER_ICON,
        SURFACE_MAGNET_ICON,
        SURFACE_MAGO_BERRY_ICON,
        SURFACE_MAGOST_BERRY_ICON,
        SURFACE_MASTER_BALL_ICON,
        SURFACE_MAX_ELIXIR_ICON,
        SURFACE_MAX_ETHER_ICON,
        SURFACE_MAX_POTION_ICON,
        SURFACE_MAX_REPEL_ICON,
        SURFACE_MAX_REVIVE_ICON,
        SURFACE_MEADOW_PLATE_ICON,
        SURFACE_MECH_MAIL_ICON,
        SURFACE_MEDAL_BOX_ICON,
        SURFACE_MEMBER_CARD_ICON,
        SURFACE_MENTAL_HERB_ICON,
        SURFACE_METAL_COAT_ICON,
        SURFACE_METAL_POWDER_ICON,
        SURFACE_METEORITE_ICON,
        SURFACE_METRONOME_ICON,
        SURFACE_MICLE_BERRY_ICON,
        SURFACE_MIND_PLATE_ICON,
        SURFACE_MIRACLE_SEED_ICON,
        SURFACE_MOOMOO_MILK_ICON,
        SURFACE_MOON_BALL_ICON,
        SURFACE_MOON_STONE_ICON,
        SURFACE_MOSAIC_MAIL_ICON,
        SURFACE_MUSCLE_BAND_ICON,
        SURFACE_MUSCLE_WING_ICON,
        SURFACE_MYSTERY_EGG_ICON,
        SURFACE_MYSTIC_WATER_ICON,
        SURFACE_MYSTICTICKET_ICON,
        SURFACE_NANAB_BERRY_ICON,
        SURFACE_NEST_BALL_ICON,
        SURFACE_NET_BALL_ICON,
        SURFACE_NEVER_MELT_ICE_ICON,
        SURFACE_NOMEL_BERRY_ICON,
        SURFACE_NORMAL_GEM_ICON,
        SURFACE_NUGGET_ICON,
        SURFACE_OAKS_LETTER_ICON,
        SURFACE_OAKS_PARCEL_ICON,
        SURFACE_OCCA_BERRY_ICON,
        SURFACE_ODD_INCENSE_ICON,
        SURFACE_ODD_KEYSTONE_ICON,
        SURFACE_OLD_AMBER_ICON,
        SURFACE_OLD_CHARM_ICON,
        SURFACE_OLD_GATEAU_ICON,
        // SURFACE_OLD_ROD_ICON,
        SURFACE_OLD_SEA_MAP_ICON,
        SURFACE_ORAN_BERRY_ICON,
        SURFACE_ORANGE_MAIL_ICON,
        SURFACE_OVAL_CHARM_ICON,
        SURFACE_OVAL_STONE_ICON,
        SURFACE_PAL_PAD_ICON,
        SURFACE_PAMTRE_BERRY_ICON,
        SURFACE_PARALYZE_HEAL_ICON,
        SURFACE_PARCEL_ICON,
        SURFACE_PARK_BALL_ICON,
        SURFACE_PASS_ORB_ICON,
        SURFACE_PASS_ICON,
        SURFACE_PASSHO_BERRY_ICON,
        SURFACE_PAYAPA_BERRY_ICON,
        SURFACE_PEARL_STRING_ICON,
        SURFACE_PEARL_ICON,
        SURFACE_PECHA_BERRY_ICON,
        SURFACE_PERMIT_ICON,
        SURFACE_PERSIM_BERRY_ICON,
        SURFACE_PETAYA_BERRY_ICON,
        SURFACE_PHOTO_ALBUM_ICON,
        SURFACE_PINAP_BERRY_ICON,
        SURFACE_PINK_APRICORN_ICON,
        SURFACE_PINK_SCARF_ICON,
        SURFACE_PLASMA_CARD_ICON,
        SURFACE_PLUME_FOSSIL_ICON,
        SURFACE_POFFIN_CASE_ICON,
        SURFACE_POINT_CARD_ICON,
        SURFACE_POISON_BARB_ICON,
        SURFACE_POISON_GEM_ICON,
        SURFACE_POKE_BALL_ICON,
        SURFACE_POKE_DOLL_ICON,
        SURFACE_POKE_FLUTE_ICON,
        SURFACE_POKE_RADAR_ICON,
        SURFACE_POKE_TOY_ICON,
        SURFACE_POKEBLOCK_CASE_ICON,
        SURFACE_POMEG_BERRY_ICON,
        SURFACE_POTION_ICON,
        SURFACE_POWDER_JAR_ICON,
        SURFACE_POWER_ANKLET_ICON,
        SURFACE_POWER_BAND_ICON,
        SURFACE_POWER_BELT_ICON,
        SURFACE_POWER_BRACER_ICON,
        SURFACE_POWER_HERB_ICON,
        SURFACE_POWER_LENS_ICON,
        SURFACE_POWER_WEIGHT_ICON,
        SURFACE_PP_MAX_ICON,
        SURFACE_PP_UP_ICON,
        SURFACE_PREMIER_BALL_ICON,
        SURFACE_PRETTY_WING_ICON,
        SURFACE_PRISM_SCALE_ICON,
        SURFACE_PROP_CASE_ICON,
        SURFACE_PROTECTOR_ICON,
        SURFACE_PROTEIN_ICON,
        SURFACE_PSYCHIC_GEM_ICON,
        SURFACE_PURE_INCENSE_ICON,
        SURFACE_QUALOT_BERRY_ICON,
        SURFACE_QUICK_BALL_ICON,
        SURFACE_QUICK_CLAW_ICON,
        SURFACE_QUICK_POWDER_ICON,
        SURFACE_RABUTA_BERRY_ICON,
        SURFACE_RAGE_CANDY_BAR_ICON,
        SURFACE_RAINBOW_PASS_ICON,
        SURFACE_RAINBOW_WING_ICON,
        SURFACE_RARE_BONE_ICON,
        SURFACE_RARE_CANDY_ICON,
        SURFACE_RAWST_BERRY_ICON,
        SURFACE_RAZOR_CLAW_ICON,
        SURFACE_RAZOR_FANG_ICON,
        SURFACE_RAZZ_BERRY_ICON,
        SURFACE_REAPER_CLOTH_ICON,
        SURFACE_RED_APRICORN_ICON,
        SURFACE_RED_CARD_ICON,
        SURFACE_RED_CHAIN_ICON,
        SURFACE_RED_FLUTE_ICON,
        SURFACE_RED_ORB_ICON,
        SURFACE_RED_SCALE_ICON,
        SURFACE_RED_SCARF_ICON,
        SURFACE_RED_SHARD_ICON,
        SURFACE_RELIC_BAND_ICON,
        SURFACE_RELIC_COPPER_ICON,
        SURFACE_RELIC_CROWN_ICON,
        SURFACE_RELIC_GOLD_ICON,
        SURFACE_RELIC_SILVER_ICON,
        SURFACE_RELIC_STATUE_ICON,
        SURFACE_RELIC_VASE_ICON,
        SURFACE_REPEAT_BALL_ICON,
        SURFACE_REPEL_ICON,
        SURFACE_REPLY_MAIL_ICON,
        SURFACE_RESET_URGE_ICON,
        SURFACE_RESIST_WING_ICON,
        SURFACE_RETRO_MAIL_ICON,
        SURFACE_REVEAL_GLASS_ICON,
        SURFACE_REVIVAL_HERB_ICON,
        SURFACE_REVIVE_ICON,
        SURFACE_RIBBON_SWEET_ICON,
        SURFACE_RINDO_BERRY_ICON,
        SURFACE_RING_TARGET_ICON,
        SURFACE_RM_1_KEY_ICON,
        SURFACE_RM_2_KEY_ICON,
        SURFACE_RM_4_KEY_ICON,
        SURFACE_RM_6_KEY_ICON,
        SURFACE_ROCK_GEM_ICON,
        SURFACE_ROCK_INCENSE_ICON,
        SURFACE_ROCKY_HELMET_ICON,
        SURFACE_ROOT_FOSSIL_ICON,
        SURFACE_ROSE_INCENSE_ICON,
        SURFACE_ROWAP_BERRY_ICON,
        SURFACE_RSVP_MAIL_ICON,
        SURFACE_RUBY_ICON,
        SURFACE_RULE_BOOK_ICON,
        SURFACE_SACHET_ICON,
        SURFACE_SACRED_ASH_ICON,
        SURFACE_SAFARI_BALL_ICON,
        SURFACE_SALAC_BERRY_ICON,
        SURFACE_SAPPHIRE_ICON,
        SURFACE_SCANNER_ICON,
        SURFACE_SCOPE_LENS_ICON,
        SURFACE_SEA_INCENSE_ICON,
        SURFACE_SEAL_BAG_ICON,
        SURFACE_SEAL_CASE_ICON,
        SURFACE_SECRET_KEY_ICON,
        SURFACE_SECRET_POTION_ICON,
        SURFACE_SHADOW_MAIL_ICON,
        SURFACE_SHARP_BEAK_ICON,
        SURFACE_SHED_SHELL_ICON,
        SURFACE_SHELL_BELL_ICON,
        SURFACE_SHINY_CHARM_ICON,
        SURFACE_SHINY_STONE_ICON,
        SURFACE_SHOAL_SALT_ICON,
        SURFACE_SHOAL_SHELL_ICON,
        SURFACE_SHOCK_DRIVE_ICON,
        SURFACE_SHUCA_BERRY_ICON,
        SURFACE_SILK_SCARF_ICON,
        SURFACE_SILPH_SCOPE_ICON,
        SURFACE_SILVER_POWDER_ICON,
        SURFACE_SILVER_WING_ICON,
        SURFACE_SITRUS_BERRY_ICON,
        SURFACE_SKULL_FOSSIL_ICON,
        SURFACE_SKY_PLATE_ICON,
        SURFACE_SLOWPOKE_TAIL_ICON,
        SURFACE_SMOKE_BALL_ICON,
        SURFACE_SMOOTH_ROCK_ICON,
        SURFACE_SNOW_MAIL_ICON,
        SURFACE_SODA_POP_ICON,
        SURFACE_SOFT_SAND_ICON,
        SURFACE_SOOT_SACK_ICON,
        SURFACE_SOOTHE_BELL_ICON,
        SURFACE_SOUL_DEW_ICON,
        SURFACE_SPACE_MAIL_ICON,
        SURFACE_SPELL_TAG_ICON,
        SURFACE_SPELON_BERRY_ICON,
        SURFACE_SPLASH_PLATE_ICON,
        SURFACE_SPOOKY_PLATE_ICON,
        SURFACE_SPORT_BALL_ICON,
        SURFACE_SPRAYDUCK_ICON,
        SURFACE_SQUIRT_BOTTLE_ICON,
        SURFACE_SS_TICKET_ICON,
        SURFACE_STABLE_MULCH_ICON,
        SURFACE_STAR_PIECE_ICON,
        SURFACE_STAR_SWEET_ICON,
        SURFACE_STARDUST_ICON,
        SURFACE_STARF_BERRY_ICON,
        SURFACE_STEEL_GEM_ICON,
        SURFACE_STEEL_MAIL_ICON,
        SURFACE_STICK_ICON,
        SURFACE_STICKY_BARB_ICON,
        SURFACE_STONE_PLATE_ICON,
        SURFACE_STORAGE_KEY_ICON,
        SURFACE_STRAWBERRY_SWEET_ICON,
        SURFACE_SUITE_KEY_ICON,
        SURFACE_SUN_STONE_ICON,
        SURFACE_SUPER_POTION_ICON,
        SURFACE_SUPER_REPEL_ICON,
        // SURFACE_SUPER_ROD_ICON,
        SURFACE_SWEET_APPLE_ICON,
        SURFACE_SWEET_HEART_ICON,
        SURFACE_SWIFT_WING_ICON,
        SURFACE_TAMATO_BERRY_ICON,
        SURFACE_TANGA_BERRY_ICON,
        SURFACE_TART_APPLE_ICON,
        SURFACE_TEA_ICON,
        SURFACE_TEACHY_TV_ICON,
        SURFACE_THANKS_MAIL_ICON,
        SURFACE_THICK_CLUB_ICON,
        SURFACE_THUNDER_STONE_ICON,
        SURFACE_TIDAL_BELL_ICON,
        SURFACE_TIMER_BALL_ICON,
        SURFACE_TINY_MUSHROOM_ICON,
        SURFACE_TM_BUG_ICON,
        SURFACE_TM_CASE_ICON,
        SURFACE_TM_DARK_ICON,
        SURFACE_TM_DRAGON_ICON,
        SURFACE_TM_ELECTRIC_ICON,
        SURFACE_TM_FIGHTING_ICON,
        SURFACE_TM_FIRE_ICON,
        SURFACE_TM_FLYING_ICON,
        SURFACE_TM_GHOST_ICON,
        SURFACE_TM_GRASS_ICON,
        SURFACE_TM_GROUND_ICON,
        SURFACE_TM_ICE_ICON,
        SURFACE_TM_NORMAL_ICON,
        SURFACE_TM_POISON_ICON,
        SURFACE_TM_PSYCHIC_ICON,
        SURFACE_TM_ROCK_ICON,
        SURFACE_TM_STEEL_ICON,
        SURFACE_TM_WATER_ICON,
        SURFACE_TOWN_MAP_ICON,
        SURFACE_TOXIC_ORB_ICON,
        SURFACE_TOXIC_PLATE_ICON,
        SURFACE_TRI_PASS_ICON,
        SURFACE_TROPIC_MAIL_ICON,
        SURFACE_TUNNEL_MAIL_ICON,
        SURFACE_TWISTED_SPOON_ICON,
        SURFACE_ULTRA_BALL_ICON,
        SURFACE_UNOWN_REPORT_ICON,
        SURFACE_UP_GRADE_ICON,
        SURFACE_VOLT_TACKLE_PICHU_ICON,
        SURFACE_VS_RECORDER_ICON,
        SURFACE_VS_SEEKER_ICON,
        SURFACE_WACAN_BERRY_ICON,
        SURFACE_WAILMER_PAIL_ICON,
        SURFACE_WATER_GEM_ICON,
        SURFACE_WATER_STONE_ICON,
        SURFACE_WATMEL_BERRY_ICON,
        SURFACE_WAVE_INCENSE_ICON,
        SURFACE_WAVE_MAIL_ICON,
        SURFACE_WEPEAR_BERRY_ICON,
        SURFACE_WHIPPED_DREAM_ICON,
        SURFACE_WHITE_APRICORN_ICON,
        SURFACE_WHITE_FLUTE_ICON,
        SURFACE_WHITE_HERB_ICON,
        SURFACE_WIDE_LENS_ICON,
        SURFACE_WIKI_BERRY_ICON,
        SURFACE_WISE_GLASSES_ICON,
        SURFACE_WOOD_MAIL_ICON,
        SURFACE_WORKS_KEY_ICON,
        SURFACE_X_ACCURACY_2_ICON,
        SURFACE_X_ACCURACY_3_ICON,
        SURFACE_X_ACCURACY_6_ICON,
        SURFACE_X_ACCURACY_ICON,
        SURFACE_X_ATTACK_2_ICON,
        SURFACE_X_ATTACK_3_ICON,
        SURFACE_X_ATTACK_6_ICON,
        SURFACE_X_ATTACK_ICON,
        SURFACE_X_DEFENSE_2_ICON,
        SURFACE_X_DEFENSE_3_ICON,
        SURFACE_X_DEFENSE_6_ICON,
        SURFACE_X_DEFENSE_ICON,
        SURFACE_X_SP_ATK_2_ICON,
        SURFACE_X_SP_ATK_3_ICON,
        SURFACE_X_SP_ATK_6_ICON,
        SURFACE_X_SP_ATK_ICON,
        SURFACE_X_SP_DEF_2_ICON,
        SURFACE_X_SP_DEF_3_ICON,
        SURFACE_X_SP_DEF_6_ICON,
        SURFACE_X_SP_DEF_ICON,
        SURFACE_X_SPEED_2_ICON,
        SURFACE_X_SPEED_3_ICON,
        SURFACE_X_SPEED_6_ICON,
        SURFACE_X_SPEED_ICON,
        SURFACE_XTRANSCEIVER_ICON,
        SURFACE_YACHE_BERRY_ICON,
        SURFACE_YELLOW_APRICORN_ICON,
        SURFACE_YELLOW_FLUTE_ICON,
        SURFACE_YELLOW_SCARF_ICON,
        SURFACE_YELLOW_SHARD_ICON,
        SURFACE_ZAP_PLATE_ICON,
        SURFACE_ZINC_ICON,
        SURFACE_ZOOM_LENS_ICON,

        ASSET_COUNT,
    } t_assetID;

  public:
    PokedexAssets();
};

#endif
