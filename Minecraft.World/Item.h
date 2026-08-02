#pragma once

using namespace std;

#include "Container.h"
#include "UseAnim.h"
#include "Rarity.h"

class MapItem;
class Mob;
class Player;
class Random;
class Level;
class ShearsItem;
class PotionItem;
class HitResult;
class IconRegister;
class Icon;
class ArmorItem;
class BowItem;
class FishingRodItem;
class EnchantedBookItem;
class EmptyMapItem;
class DartShooterGoldItem;
//class DartShooterPoisonItem;
class DartShooterEnchantedItem;
class HealingStoneItem;
class CloudParachuteItem;

#define ITEM_ICON_COLUMNS 16


class Item : public enable_shared_from_this<Item>
{
protected:
	//static const UUID BASE_ATTACK_DAMAGE_UUID;

public:
	static const int ITEM_NUM_COUNT = 32000;

	static void staticCtor();
	static void staticInit();

	// 4J-PB - added for new crafting menu
	enum
	{
		eMaterial_undefined=0,
		eMaterial_wood,
		eMaterial_stone,
		eMaterial_iron,
		eMaterial_gold,
		eMaterial_diamond,
		eMaterial_cloth,
		eMaterial_chain, // 4J Stu - It's available in creative in 1.8
		eMaterial_detector,
		eMaterial_lapis,
		eMaterial_music,
		eMaterial_dye,
		eMaterial_sand,
		eMaterial_brick,
		eMaterial_clay,
		eMaterial_snow,
		eMaterial_bow,
		eMaterial_arrow,
		eMaterial_compass,
		eMaterial_clock,
		eMaterial_map,
		eMaterial_pumpkin,
		eMaterial_glowstone,
		eMaterial_water,
		eMaterial_trap,
		eMaterial_flintandsteel,
		eMaterial_shears,
		eMaterial_piston,
		eMaterial_stickypiston,
		eMaterial_gate,
		eMaterial_stoneSmooth,
		eMaterial_netherbrick,
		eMaterial_ender,
		eMaterial_glass,
		eMaterial_blaze,
		eMaterial_magic,
		eMaterial_melon,
		eMaterial_setfire,
		eMaterial_sprucewood,
		eMaterial_birchwood,
		eMaterial_junglewood,
		eMaterial_emerald,
		eMaterial_quartz,
		eMaterial_apple,
		eMaterial_carrot,
		eMaterial_redstone,
		eMaterial_coal,
		eMaterial_paper,
		eMaterial_book,
		eMaterial_bookshelf,
		eMaterial_wheat,
		eMaterial_nethanium,
		eMaterial_bread,
		eMaterial_endorium,
		eMaterial_ice,
		eMaterial_stick,
		eMaterial_tnt,
		eMaterial_endStone,
		eMaterial_ambrosium,
		eMaterial_zanite,
		eMaterial_gravitite,
		eMaterial_cloud,

	}
	eMaterial;

	enum
	{
		eBaseItemType_undefined=0,
		eBaseItemType_sword,
		eBaseItemType_shovel,
		eBaseItemType_pickaxe,
		eBaseItemType_hatchet,
		eBaseItemType_hoe,
		eBaseItemType_door,
		eBaseItemType_helmet,
		eBaseItemType_chestplate,
		eBaseItemType_leggings,
		eBaseItemType_boots,
		eBaseItemType_ingot,
		eBaseItemType_rail,
		eBaseItemType_block,
		eBaseItemType_pressureplate,
		eBaseItemType_stairs,
		eBaseItemType_cloth,
		eBaseItemType_dyepowder,
		eBaseItemType_structwoodstuff,
		eBaseItemType_structblock,
		eBaseItemType_slab,
		eBaseItemType_halfslab,
		eBaseItemType_torch,
		eBaseItemType_bow,
		eBaseItemType_pockettool,
		eBaseItemType_utensil,
		eBaseItemType_piston,
		eBaseItemType_devicetool,
		eBaseItemType_fence,
		eBaseItemType_device,
		eBaseItemType_treasure,
		eBaseItemType_seed,
		eBaseItemType_HangingItem,
		eBaseItemType_button,
		eBaseItemType_chest,
		eBaseItemType_rod,
		eBaseItemType_giltFruit,
		eBaseItemType_carpet,
		eBaseItemType_clay,
		eBaseItemType_glass,
		eBaseItemType_redstoneContainer,
		eBaseItemType_fireworks,
		eBaseItemType_lever,
		eBaseItemType_paper,
		eBaseItemType_bread,
		eBaseItemType_stick,
		eBaseItemType_tnt,
		eBaseItemType_wall,
		eBaseItemType_MAXTYPES,
	}
	eBaseItemType;

protected:
	static const int ICON_COLUMNS = ITEM_ICON_COLUMNS;
	static wstring ICON_DESCRIPTION_PREFIX; // 4J Stu - Was const but we have to static initialise it outside of this class

public:

	class Tier
	{
	public:
		static const Tier *WOOD; //
		static const Tier *STONE; //
		static const Tier *IRON; //
		static const Tier *DIAMOND; //
		static const Tier *GOLD;
		static const Tier *NETHANIUM; //
		static const Tier *ENDORIUM; //
		static const Tier *ZANITE; //
		static const Tier *GRAVITITE; //
		static const Tier *VAMPIRE; //
		static const Tier *APHALAF; //

	private:
		const int level;
		const int uses;
		const float speed;
		const float damage;
		const int enchantmentValue;

		// 4J Stu - Had to make this public but was protected
		// We shouldn't be creating these except the static initialisation
	public:
		Tier(int level, int uses, float speed, float damage, int enchantmentValue);

	public:
		int getUses() const;
		float getSpeed() const;
		float getAttackDamageBonus() const;
		int getLevel() const;
		int getEnchantmentValue() const;
		int getTierItemId() const;
	};

protected:
	static Random *random;

private:
	static const int MAX_STACK_SIZE = Container::LARGE_MAX_STACK_SIZE;

public:
	static ItemArray items;

	static Item *shovel_iron;
	static Item *pickAxe_iron;
	static Item *hatchet_iron;
	static Item *flintAndSteel;
	static Item *apple;
	static BowItem *bow;
	static Item *arrow;
	static Item *coal;
	static Item *diamond;
	static Item *ironIngot;
	static Item *goldIngot;
	static Item *sword_iron;

	static Item *sword_wood;
	static Item *shovel_wood;
	static Item *pickAxe_wood;
	static Item *hatchet_wood;

	static Item *sword_stone;
	static Item *shovel_stone;
	static Item *pickAxe_stone;
	static Item *hatchet_stone;

	static Item *sword_diamond;
	static Item *shovel_diamond;
	static Item *pickAxe_diamond;
	static Item *hatchet_diamond;

	static Item *stick;
	static Item *bowl;
	static Item *mushroomStew;

	static Item *sword_gold;
	static Item *shovel_gold;
	static Item *pickAxe_gold;
	static Item *hatchet_gold;

	static Item *string;
	static Item *feather;
	static Item *gunpowder;

	static Item *hoe_wood;
	static Item *hoe_stone;
	static Item *hoe_iron;
	static Item *hoe_diamond;
	static Item *hoe_gold;

	static Item *seeds_wheat;
	static Item *wheat;
	static Item *bread;

	static ArmorItem *helmet_leather;
	static ArmorItem *chestplate_leather;
	static ArmorItem *leggings_leather;
	static ArmorItem *boots_leather;

	static ArmorItem *helmet_chain;
	static ArmorItem *chestplate_chain;
	static ArmorItem *leggings_chain;
	static ArmorItem *boots_chain;

	static ArmorItem *helmet_iron;
	static ArmorItem *chestplate_iron;
	static ArmorItem *leggings_iron;
	static ArmorItem *boots_iron;

	static ArmorItem *helmet_diamond;
	static ArmorItem *chestplate_diamond;
	static ArmorItem *leggings_diamond;
	static ArmorItem *boots_diamond;

	static ArmorItem *helmet_gold;
	static ArmorItem *chestplate_gold;
	static ArmorItem *leggings_gold;
	static ArmorItem *boots_gold;

	static Item *flint;
	static Item *porkChop_raw;
	static Item *porkChop_cooked;
	static Item *painting;

	static Item *apple_gold;

	static Item *sign;
	static Item *door_wood;

	static Item *bucket_empty;
	static Item *bucket_water;
	static Item *bucket_lava;

	static Item *minecart;
	static Item *saddle;
	static Item *door_iron;
	static Item *redStone;
	static Item *snowBall;

	static Item *boat;

	static Item *leather;
	static Item *bucket_milk;
	static Item *brick;
	static Item *clay;
	static Item *reeds;
	static Item *paper;
	static Item *book;
	static Item *slimeBall;
	static Item *minecart_chest;
	static Item *minecart_furnace;
	static Item *egg;
	static Item *compass;
	static FishingRodItem *fishingRod;
	static Item *clock;
	static Item *yellowDust;
	static Item *fish_raw;
	static Item *fish_cooked;

	static Item *dye_powder;
	static Item *bone;
	static Item *sugar;
	static Item *cake;

	static Item *bed;

	static Item *repeater;
	static Item *cookie;

	static MapItem *map;

	static ShearsItem *shears;

	static Item *melon;

	static Item *seeds_pumpkin;
	static Item *seeds_melon;

	static Item *beef_raw;
	static Item *beef_cooked;
	static Item *chicken_raw;
	static Item *chicken_cooked;
	static Item *rotten_flesh;

	static Item *enderPearl;

	static Item *blazeRod;
	static Item *ghastTear;
	static Item *goldNugget;

	static Item *netherwart_seeds;

	static PotionItem *potion;
	static Item *glassBottle;

	static Item *spiderEye;
	static Item *fermentedSpiderEye;

	static Item *blazePowder;
	static Item *magmaCream;

	static Item *brewingStand;
	static Item *cauldron;
	static Item *eyeOfEnder;
	static Item *speckledMelon;

	static Item *spawnEgg;

	static Item *expBottle;

	static Item *skull;

	static Item *record_01;
	static Item *record_02;
	static Item *record_03;
	static Item *record_04;
	static Item *record_05;
	static Item *record_06;
	static Item *record_07;
	static Item *record_08;
	static Item *record_09;
	static Item *record_10;
	static Item *record_11;
	static Item *record_12;

	// TU9
	static Item *fireball;
	static Item *frame;

	// TU14
	//static Item writingBook;
	//static Item writtenBook;

	static Item *emerald;

	static Item *flowerPot;

	static Item *carrots;
	static Item *potato;
	static Item *potatoBaked;
	static Item *potatoPoisonous;

	static EmptyMapItem *emptyMap;

	static Item *carrotGolden;

	static Item *carrotOnAStick;
	static Item *netherStar;
	static Item *pumpkinPie;

	static Item *fireworks;
	static Item *fireworksCharge;
	static Item *netherQuartz;

	static Item *comparator;
	static Item *netherbrick;
	static EnchantedBookItem *enchantedBook;
	static Item *minecart_tnt;
	static Item *minecart_hopper;

	static Item *horseArmorMetal;
	static Item *horseArmorGold;
	static Item *horseArmorDiamond;
	static Item *lead;
	static Item *nameTag;
	static Item *nethanium;
	static Item *nethaniumSword;
	static Item *nethaniumAxe;
	static Item *nethaniumPickaxe;
	static Item *nethaniumShovel;
	static Item *nethaniumHoe;
	static Item *hellSphere;
	static Item *nethaniumHelmet;
	static Item *nethaniumChestplate;
	static Item *nethaniumLeggings;
	static Item *nethaniumBoots;
	static Item *relicMallet;
	static Item *netherBread;
	static Item *goldBread;
	static Item *nethaniumBread;
	static Item *endorium;
	static Item *veloettBerry;
	static Item *endoriumSword;
	static Item *endoriumAxe;
	static Item *endoriumPickaxe;
	static Item *endoriumShovel;
	static Item *endoriumHoe;
	static Item *evupulWing;
	static Item *evupulWingGold;
	static Item *isogalRib;
	static Item *endoriumHelmet;
	static Item *endoriumChestplate;
	static Item *endoriumLeggings;
	static Item *endoriumBoots;
	static Item *aphalafTooth;
	static Item *aphalafSword;
	static Item *goldenAmber;
	static Item *ambrosiumShard;
	static Item *zaniteGemstone;
	static Item *aechorPetal;
	static Item *gravititeSword;
	static Item *gravititePickaxe;
	static Item *gravititeAxe;
	static Item *gravititeShovel;
	static Item *gravititeHoe;
	static Item *zaniteSword;
	static Item *zanitePickaxe;
	static Item *zaniteAxe;
	static Item *zaniteShovel;
	static Item *zaniteHoe;
	static Item *zaniteHelmet;
	static Item *zaniteChestplate;
	static Item *zaniteLeggings;
	static Item *zaniteBoots;
	static Item *gravititeHelmet;
	static Item *gravititeChestplate;
	static Item *gravititeLeggings;
	static Item *gravititeBoots;
	static DartShooterGoldItem *dartShooterGold;
	//static DartShooterPoisonItem *dartShooterPoison;
	static DartShooterEnchantedItem *dartShooterEnchanted;
	static Item *dartGold;
	static Item *dartPoison;
	static Item *dartEnchanted;
	static Item *blueBerry;
	static Item *blueBerryEnchanted;
	static Item *swetBall;
	static HealingStoneItem *healingStone;
	static Item *holystoneSword;
	static Item *holystonePickaxe;
	static Item *holystoneAxe;
	static Item *holystoneShovel;
	static Item *holystoneHoe;
	static Item *gummySwetBlue;
	static Item *gummySwetGold;
	static HealingStoneItem *regenerationStone;
	static HealingStoneItem *lifeShard;
	static Item *skyrootBucket_empty;
	static Item *skyrootBucket_water;
	static Item *skyrootBucket_milk;
	static Item *skyrootBucket_poison;
	static Item *moaEggBlue;
	static Item *moaEggWhite;
	static Item *moaEggBlack;
	static Item *keyBronze;
	static Item *keySilver;
	static Item *keyGold;
	static CloudParachuteItem *cloudParachute;
	static Item *skyrootSword;
	static Item *skyrootPickaxe;
	static Item *skyrootAxe;
	static Item *skyrootShovel;
	static Item *skyrootHoe;
	static Item *vampireBlade;
	static Item *staffShard;
	static Item *aphalafShard;
	static Item *aphalafRod;
	static Item *nusaShard;
	static Item *endothaniumIngot;


	static const int shovel_iron_Id			= 512;
	static const int pickAxe_iron_Id		= 513;
	static const int hatchet_iron_Id		= 514;
	static const int flintAndSteel_Id		= 515;
	static const int apple_Id				= 516;
	static const int bow_Id					= 517;
	static const int arrow_Id				= 518;
	static const int coal_Id				= 519;
	static const int diamond_Id				= 520;
	static const int ironIngot_Id			= 521;
	static const int goldIngot_Id			= 522;
	static const int sword_iron_Id			= 523;
	static const int sword_wood_Id			= 524;
	static const int shovel_wood_Id			= 525;
	static const int pickAxe_wood_Id		= 526;
	static const int hatchet_wood_Id		= 527;
	static const int sword_stone_Id			= 528;
	static const int shovel_stone_Id		= 529;
	static const int pickAxe_stone_Id		= 530;
	static const int hatchet_stone_Id		= 531;
	static const int sword_diamond_Id		= 532;
	static const int shovel_diamond_Id		= 533;
	static const int pickAxe_diamond_Id		= 534;
	static const int hatchet_diamond_Id		= 535;
	static const int stick_Id				= 536;
	static const int bowl_Id				= 537;
	static const int mushroomStew_Id		= 538;
	static const int sword_gold_Id			= 539;
	static const int shovel_gold_Id			= 540;
	static const int pickAxe_gold_Id		= 541;
	static const int hatchet_gold_Id		= 542;
	static const int string_Id				= 543;
	static const int feather_Id				= 544;
	static const int gunpowder_Id			= 545;
	static const int hoe_wood_Id			= 546;
	static const int hoe_stone_Id			= 547;
	static const int hoe_iron_Id			= 548;
	static const int hoe_diamond_Id			= 549;
	static const int hoe_gold_Id			= 550;
	static const int seeds_wheat_Id			= 551;
	static const int wheat_Id				= 552;
	static const int bread_Id				= 553;

	static const int helmet_leather_Id		= 554;
	static const int chestplate_leather_Id	= 555;
	static const int leggings_leather_Id	= 556;
	static const int boots_leather_Id		= 557;

	static const int helmet_chain_Id		= 558;
	static const int chestplate_chain_Id	= 559;
	static const int leggings_chain_Id		= 560;
	static const int boots_chain_Id			= 561;

	static const int helmet_iron_Id			= 562;
	static const int chestplate_iron_Id		= 563;
	static const int leggings_iron_Id		= 564;
	static const int boots_iron_Id			= 565;

	static const int helmet_diamond_Id		= 566;
	static const int chestplate_diamond_Id	= 567;
	static const int leggings_diamond_Id	= 568;
	static const int boots_diamond_Id		= 569;

	static const int helmet_gold_Id			= 570;
	static const int chestplate_gold_Id		= 571;
	static const int leggings_gold_Id		= 572;
	static const int boots_gold_Id			= 573;

	static const int flint_Id				= 574;
	static const int porkChop_raw_Id		= 575;
	static const int porkChop_cooked_Id		= 576;
	static const int painting_Id			= 577;
	static const int apple_gold_Id			= 578;
	static const int sign_Id				= 579;
	static const int door_wood_Id			= 580;
	static const int bucket_empty_Id		= 581;
	static const int bucket_water_Id		= 582;
	static const int bucket_lava_Id			= 583;
	static const int minecart_Id			= 584;
	static const int saddle_Id				= 585;
	static const int door_iron_Id			= 586;
	static const int redStone_Id			= 587;
	static const int snowBall_Id			= 588;
	static const int boat_Id				= 589;
	static const int leather_Id				= 590;
	static const int bucket_milk_Id			= 591;
	static const int brick_Id				= 592;
	static const int clay_Id				= 593;
	static const int reeds_Id				= 594;
	static const int paper_Id				= 595;
	static const int book_Id				= 596;
	static const int slimeBall_Id			= 597;
	static const int minecart_chest_Id		= 598;
	static const int minecart_furnace_Id	= 599;
	static const int egg_Id					= 600;
	static const int compass_Id				= 601;
	static const int fishingRod_Id			= 602;
	static const int clock_Id				= 603;
	static const int yellowDust_Id			= 604;
	static const int fish_raw_Id			= 605;
	static const int fish_cooked_Id			= 606;
	static const int dye_powder_Id			= 607;
	static const int bone_Id				= 608;
	static const int sugar_Id				= 609;
	static const int cake_Id				= 610;
	static const int bed_Id					= 611;
	static const int repeater_Id			= 612;
	static const int cookie_Id				= 613;
	static const int map_Id					= 614;

	// 1.7.3
	static const int shears_Id				= 615;

	// 1.8.2
	static const int melon_Id				= 616;
	static const int seeds_pumpkin_Id		= 617;
	static const int seeds_melon_Id			= 618;
	static const int beef_raw_Id			= 619;
	static const int beef_cooked_Id			= 620;
	static const int chicken_raw_Id			= 621;
	static const int chicken_cooked_Id		= 622;
	static const int rotten_flesh_Id		= 623;
	static const int enderPearl_Id			= 624;

	// 1.0.1
	static const int blazeRod_Id			= 625;
	static const int ghastTear_Id			= 626;
	static const int goldNugget_Id			= 627;
	static const int netherwart_seeds_Id	= 628;
	static const int potion_Id				= 629;
	static const int glassBottle_Id			= 630;
	static const int spiderEye_Id			= 631;
	static const int fermentedSpiderEye_Id	= 632;
	static const int blazePowder_Id			= 633;
	static const int magmaCream_Id			= 634;
	static const int brewingStand_Id		= 635;
	static const int cauldron_Id			= 636;
	static const int eyeOfEnder_Id			= 637;
	static const int speckledMelon_Id		= 638;
	static const int spawnEgg_Id			= 639;
	static const int expBottle_Id			= 640;

	static const int record_01_Id			= 2512;
	static const int record_02_Id			= 2513;
	static const int record_03_Id			= 2514;
	static const int record_04_Id			= 2515;
	static const int record_05_Id			= 2516;
	static const int record_06_Id			= 2517;
	static const int record_07_Id			= 2518;
	static const int record_09_Id			= 2519;
	static const int record_10_Id		    = 2520;
	static const int record_11_Id		    = 2521;
	static const int record_12_Id			= 2522;

	// 4J-PB - this one isn't playable in the PC game, but is fine in ours
	static const int record_08_Id			= 2523;

	static const int fireball_Id			 = 641;
	//static const int writingBook_Id			 = 642;
	//static const int writtenBook_Id			 = 643;
	static const int emerald_Id				 = 644;
	static const int itemFrame_Id			 = 645;
	static const int flowerPot_Id = 646;
	static const int carrots_Id = 647;
	static const int potato_Id = 648;
	static const int potatoBaked_Id = 649;
	static const int potatoPoisonous_Id = 650;
	static const int emptyMap_Id = 651;
	static const int carrotGolden_Id = 652;
	static const int skull_Id = 653;
	static const int carrotOnAStick_Id = 654;
	static const int netherStar_Id = 655;
	static const int pumpkinPie_Id = 656;
	static const int fireworks_Id = 657;
	static const int fireworksCharge_Id = 658;
	static const int enchantedBook_Id = 659;
	static const int comparator_Id = 660;
	static const int netherbrick_Id = 661;
	static const int netherQuartz_Id = 662;
	static const int minecart_tnt_Id = 663;
	static const int minecart_hopper_Id = 664;
	static const int horseArmorMetal_Id = 673;
	static const int horseArmorGold_Id = 674;
	static const int horseArmorDiamond_Id = 675;
	static const int lead_Id = 676;
	static const int nameTag_Id = 677;

	static const int nethanium_Id = 678;
	static const int nethaniumSword_Id = 679;
	static const int nethaniumAxe_Id = 680;
	static const int nethaniumPickaxe_Id = 681;
	static const int nethaniumShovel_Id = 682;
	static const int nethaniumHoe_Id = 683;
	static const int hellSphere_Id = 684;
	static const int nethaniumHelmet_Id = 685;
	static const int nethaniumChestplate_Id = 686;
	static const int nethaniumLeggings_Id = 687;
	static const int nethaniumBoots_Id = 688;
	static const int relicMallet_Id = 689;
	static const int netherBread_Id = 690;
	static const int goldBread_Id = 691;
	static const int nethaniumBread_Id = 692;
	static const int endorium_Id = 693;
	static const int veloettBerry_Id = 694;
	static const int endoriumSword_Id = 695;
	static const int endoriumAxe_Id = 696;
	static const int endoriumPickaxe_Id = 697;
	static const int endoriumShovel_Id = 698;
	static const int endoriumHoe_Id = 699;
	static const int evupulWing_Id = 700;
	static const int evupulWingGold_Id = 701;
	static const int isogalRib_Id = 702;
	static const int endoriumHelmet_Id = 703;
	static const int endoriumChestplate_Id = 704;
	static const int endoriumLeggings_Id = 705;
	static const int endoriumBoots_Id = 706;
	static const int aphalafTooth_Id = 707;
	static const int aphalafSword_Id = 708;
	static const int goldenAmber_Id = 709;
	static const int ambrosiumShard_Id = 710;
	static const int zaniteGemstone_Id = 711;
	static const int aechorPetal_Id = 712;
	static const int gravititeSword_Id = 713;
	static const int gravititePickaxe_Id = 714;
	static const int gravititeAxe_Id = 715;
	static const int gravititeShovel_Id = 716;
	static const int gravititeHoe_Id = 717;
	static const int zaniteSword_Id = 718;
	static const int zanitePickaxe_Id = 719;
	static const int zaniteAxe_Id = 720;
	static const int zaniteShovel_Id = 721;
	static const int zaniteHoe_Id = 722;
	static const int zaniteHelmet_Id = 723;
	static const int zaniteChestplate_Id = 724;
	static const int zaniteLeggings_Id = 725;
	static const int zaniteBoots_Id = 726;
	static const int gravititeHelmet_Id = 727;
	static const int gravititeChestplate_Id = 728;
	static const int gravititeLeggings_Id = 729;
	static const int gravititeBoots_Id = 730;
	static const int dartShooterGold_Id = 731;
	//static const int dartShooterPoison_Id = 732;
	static const int dartShooterEnchanted_Id = 733;
	static const int dartGold_Id = 734;
	static const int dartPoison_Id = 735;
	static const int dartEnchanted_Id = 736;
	static const int blueBerry_Id = 737;
	static const int blueBerryEnchanted_Id = 738;
	static const int swetBall_Id = 739;
	static const int healingStone_Id = 740;
	static const int holystoneSword_Id = 741;
	static const int holystonePickaxe_Id = 742;
	static const int holystoneAxe_Id = 743;
	static const int holystoneShovel_Id = 744;
	static const int holystoneHoe_Id = 745;
	static const int gummySwetBlue_Id = 746;
	static const int gummySwetGold_Id = 747;
	static const int regenerationStone_Id = 748;
	static const int lifeShard_Id = 749;
	static const int skyrootBucket_empty_Id = 750;
	static const int skyrootBucket_water_Id = 751;
	static const int skyrootBucket_milk_Id = 752;
	static const int skyrootBucket_poison_Id = 753;
	static const int moaEggBlue_Id = 754;
	static const int moaEggWhite_Id = 755;
	static const int moaEggBlack_Id = 756;
	static const int keyBronze_Id = 757;
	static const int keySilver_Id = 758;
	static const int keyGold_Id = 759;
	static const int cloudParachute_Id = 760;
	static const int skyrootSword_Id = 761;
	static const int skyrootPickaxe_Id = 762;
	static const int skyrootAxe_Id = 763;
	static const int skyrootShovel_Id = 764;
	static const int skyrootHoe_Id = 765;
	static const int vampireBlade_Id = 766;
	static const int staffShard_Id = 767;
	static const int aphalafShard_Id = 768;
	static const int aphalafRod_Id = 769;
	static const int nusaShard_Id = 770;
	static const int endothaniumIngot_Id = 771;

public:
	const int id;

protected:
	int maxStackSize;

private:
	int maxDamage;

protected:
	Icon *icon;
	// 4J-PB - added for new crafting menu
	int m_iBaseItemType;
	int m_iMaterial;
	bool m_handEquipped;
	bool m_isStackedByData;

private:
	Item *craftingRemainingItem;
	wstring potionBrewingFormula;

	// 4J Stu - A value from strings.h, that is the name of the item
	unsigned int descriptionId;

	// 4J Stu - A value from strings.h that says what this does
	unsigned int useDescriptionId;

	wstring m_textureName;

protected:
	Item(int id);

public:
	// 4J Using per-item textures now
	Item *setIconName(const wstring &name);
	wstring getIconName();
	Item *setMaxStackSize(int max);
	Item *setBaseItemTypeAndMaterial(int iType,int iMaterial);
	int getBaseItemType();
	int getMaterial();

	virtual int getIconType();
	virtual Icon *getIcon(int auxValue);
	Icon *getIcon(shared_ptr<ItemInstance> itemInstance);

	virtual bool useOn(shared_ptr<ItemInstance> itemInstance, shared_ptr<Player> player, Level *level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, bool bTestUseOnOnly=false);
	virtual float getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile);
	virtual bool TestUse(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player);
	virtual shared_ptr<ItemInstance> useTimeDepleted(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player);
	virtual int getMaxStackSize() const;
	virtual int getLevelDataForAuxValue(int auxValue);
	bool isStackedByData();

protected:
	Item *setStackedByData(bool isStackedByData);

public:
	int getMaxDamage();

protected:
	Item *setMaxDamage(int maxDamage);

public:
	bool canBeDepleted();

	/**
	* Returns true when the item was used to deal more than default damage
	*
	* @param itemInstance
	* @param mob
	* @param attacker
	* @return
	*/
	virtual bool hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker);

	/**
	* Returns true when the item was used to mine more efficiently
	*
	* @param itemInstance
	* @param tile
	* @param x
	* @param yf
	* @param z
	* @param owner
	* @return
	*/
	virtual bool mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner);
	virtual int getAttackDamage(shared_ptr<Entity> entity);
	virtual bool canDestroySpecial(Tile *tile);
	virtual bool interactEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<Player> player, shared_ptr<LivingEntity> mob);
	Item *handEquipped();
	virtual bool isHandEquipped();
	virtual bool isMirroredArt();
	Item *setDescriptionId(unsigned int id);
	LPCWSTR getDescription();
	LPCWSTR getDescription(shared_ptr<ItemInstance> instance);
	virtual unsigned int getDescriptionId(int iData = -1);
	virtual unsigned int getDescriptionId(shared_ptr<ItemInstance> instance);
	Item *setUseDescriptionId(unsigned int id);
	virtual unsigned int getUseDescriptionId();
	virtual unsigned int getUseDescriptionId(shared_ptr<ItemInstance> instance);
	Item *setCraftingRemainingItem(Item *craftingRemainingItem);
	virtual bool shouldMoveCraftingResultToInventory(shared_ptr<ItemInstance> instance);
	virtual bool shouldOverrideMultiplayerNBT();
	Item *getCraftingRemainingItem();
	bool hasCraftingRemainingItem();
	std::wstring getName();
	virtual int getColor(shared_ptr<ItemInstance> item, int spriteLayer);
	virtual void inventoryTick(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Entity> owner, int slot, bool selected);
	virtual void onCraftedBy(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player);
	virtual bool isComplex();

	virtual UseAnim getUseAnimation(shared_ptr<ItemInstance> itemInstance);
	virtual int getUseDuration(shared_ptr<ItemInstance> itemInstance);
	virtual void releaseUsing(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player, int durationLeft);

protected:
	virtual Item *setPotionBrewingFormula(const wstring &potionBrewingFormula);

public:
	virtual wstring getPotionBrewingFormula();
	virtual bool hasPotionBrewingFormula();
	virtual void appendHoverText(shared_ptr<ItemInstance> itemInstance, shared_ptr<Player> player, vector<HtmlString> *lines, bool advanced);
	virtual wstring getHoverName(shared_ptr<ItemInstance> itemInstance);
	virtual bool isFoil(shared_ptr<ItemInstance> itemInstance);
	virtual const Rarity *getRarity(shared_ptr<ItemInstance> itemInstance);
	virtual bool isEnchantable(shared_ptr<ItemInstance> itemInstance);

protected:
	HitResult *getPlayerPOVHitResult(Level *level, shared_ptr<Player> player, bool alsoPickLiquid);

public:
	virtual int getEnchantmentValue();
	virtual bool hasMultipleSpriteLayers();
	virtual Icon *getLayerIcon(int auxValue, int spriteLayer);
	virtual bool mayBePlacedInAdventureMode();
	virtual bool isValidRepairItem(shared_ptr<ItemInstance> source, shared_ptr<ItemInstance> repairItem);
	virtual void registerIcons(IconRegister *iconRegister);
	virtual attrAttrModMap *getDefaultAttributeModifiers();
};
