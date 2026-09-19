#include "IDs.h"
#include "Item.h"
#include "Tile.h"

IDMapping* IDMapping::get() {
    static IDMapping instance;
    return &instance;
}

void IDMapping::add(std::string ns, std::string name, bool isBlock, int id) {
    std::string n = ns+":"+name;
    stringToID[n] = { id, isBlock, 0 };
    buildReverseMap();
}

IDMapping::MappedItem IDMapping::getID(std::string k) {
    auto it = stringToID.find(k);
    if (it != stringToID.end()) {
        return it->second;
    }
    return { 0, false, 0 };
}

void IDMapping::buildReverseMap() {
    idToString.clear();
    for (const auto& pair : stringToID) {
        uint32_t key = makeKey(pair.second.id, pair.second.aux);
        idToString.insert({key, pair.first});
    }
}

std::string IDMapping::getByID(int id, int aux) {
    uint32_t key = makeKey(id, aux);
    auto stringId = idToString.find(key);

    if (stringId != idToString.end()) {return stringId->second;}

    if (aux != 0) {
        key = makeKey(id, 0);
        stringId = idToString.find(key);
        if (stringId != idToString.end()) {return stringId->second;}
    }

    return "minecraft:air";
}

std::string IDMapping::getByStringedID(const std::string& numIdStr) {
    if (numIdStr.empty()) return "minecraft:air";

    try {
        size_t colon = numIdStr.find(':');
        if (colon != std::string::npos) {
            int id = std::stoi(numIdStr.substr(0, colon));
            int aux = std::stoi(numIdStr.substr(colon + 1));
            return getByID(id, aux);
        }else {
            int id = std::stoi(numIdStr);
            return getByID(id);
        }
    } catch (const std::invalid_argument&) {
        return "minecraft:air";
    }
}

/* Boilerplate HELL reduced with macros (you're welcome) */

void IDMapping::init() {
    #define ADD_BLOCK(key, tid, av) stringToID["minecraft:" key] = { tid, true, av };
    #define ADD_ITEM(key, iid, av) stringToID["minecraft:" key] = { iid, false, av };

    /* Blocks */
    ADD_BLOCK("stone", Tile::stone_Id, 0);
    ADD_BLOCK("grass_block", Tile::grass_Id, 0);
    ADD_BLOCK("dirt", Tile::dirt_Id, 0);
    ADD_BLOCK("cobblestone", Tile::cobblestone_Id, 0);
    ADD_BLOCK("oak_planks", Tile::wood_Id, 0);
    ADD_BLOCK("spruce_planks", Tile::wood_Id, 1);
    ADD_BLOCK("birch_planks", Tile::wood_Id, 2);
    ADD_BLOCK("jungle_planks", Tile::wood_Id, 3);
    ADD_BLOCK("oak_sapling", Tile::sapling_Id, 0);
    ADD_BLOCK("spruce_sapling", Tile::sapling_Id, 1);
    ADD_BLOCK("birch_sapling", Tile::sapling_Id, 2);
    ADD_BLOCK("jungle_sapling", Tile::sapling_Id, 3);
    ADD_BLOCK("bedrock", Tile::unbreakable_Id, 0);
    ADD_BLOCK("flowing_water", Tile::water_Id, 0);
    ADD_BLOCK("water", Tile::calmWater_Id, 0);
    ADD_BLOCK("flowing_lava", Tile::lava_Id, 0);
    ADD_BLOCK("lava", Tile::calmLava_Id, 0);
    ADD_BLOCK("sand", Tile::sand_Id, 0);
    ADD_BLOCK("gravel", Tile::gravel_Id, 0);
    ADD_BLOCK("gold_ore", Tile::goldOre_Id, 0);
    ADD_BLOCK("iron_ore", Tile::ironOre_Id, 0);
    ADD_BLOCK("coal_ore", Tile::coalOre_Id, 0);
    ADD_BLOCK("oak_log", Tile::treeTrunk_Id, 0);
    ADD_BLOCK("spruce_log", Tile::treeTrunk_Id, 1);
    ADD_BLOCK("birch_log", Tile::treeTrunk_Id, 2);
    ADD_BLOCK("jungle_log", Tile::treeTrunk_Id, 3);
    ADD_BLOCK("oak_leaves", Tile::leaves_Id, 0);
    ADD_BLOCK("spruce_leaves", Tile::leaves_Id, 1);
    ADD_BLOCK("birch_leaves", Tile::leaves_Id, 2);
    ADD_BLOCK("jungle_leaves", Tile::leaves_Id, 3);
    ADD_BLOCK("sponge", Tile::sponge_Id, 0);
    ADD_BLOCK("glass", Tile::glass_Id, 0);
    ADD_BLOCK("lapis_ore", Tile::lapisOre_Id, 0);
    ADD_BLOCK("lapis_block", Tile::lapisBlock_Id, 0);
    ADD_BLOCK("dispenser", Tile::dispenser_Id, 0);
    ADD_BLOCK("sandstone", Tile::sandStone_Id, 0);
    ADD_BLOCK("chiseled_sandstone", Tile::sandStone_Id, 1);
    ADD_BLOCK("cut_sandstone", Tile::sandStone_Id, 2);
    ADD_BLOCK("note_block", Tile::noteblock_Id, 0);
    ADD_BLOCK("bed", Tile::bed_Id, 0);
    ADD_BLOCK("golden_rail", Tile::goldenRail_Id, 0);
    ADD_BLOCK("detector_rail", Tile::detectorRail_Id, 0);
    ADD_BLOCK("sticky_piston", Tile::pistonStickyBase_Id, 0);
    ADD_BLOCK("web", Tile::web_Id, 0);
    ADD_BLOCK("dead_bush", Tile::tallgrass_Id, 0);
    ADD_BLOCK("grass", Tile::tallgrass_Id, 1);
    ADD_BLOCK("fern", Tile::tallgrass_Id, 2);
    ADD_BLOCK("dead_bush", Tile::deadBush_Id, 0);
    ADD_BLOCK("piston", Tile::pistonBase_Id, 0);
    ADD_BLOCK("piston_head", Tile::pistonExtensionPiece_Id, 0);
    ADD_BLOCK("white_wool", Tile::wool_Id, 0);
    ADD_BLOCK("orange_wool", Tile::wool_Id, 1);
    ADD_BLOCK("magenta_wool", Tile::wool_Id, 2);
    ADD_BLOCK("light_blue_wool", Tile::wool_Id, 3);
    ADD_BLOCK("yellow_wool", Tile::wool_Id, 4);
    ADD_BLOCK("lime_wool", Tile::wool_Id, 5);
    ADD_BLOCK("pink_wool", Tile::wool_Id, 6);
    ADD_BLOCK("gray_wool", Tile::wool_Id, 7);
    ADD_BLOCK("light_gray_wool", Tile::wool_Id, 8);
    ADD_BLOCK("cyan_wool", Tile::wool_Id, 9);
    ADD_BLOCK("purple_wool", Tile::wool_Id, 10);
    ADD_BLOCK("blue_wool", Tile::wool_Id, 11);
    ADD_BLOCK("brown_wool", Tile::wool_Id, 12);
    ADD_BLOCK("green_wool", Tile::wool_Id, 13);
    ADD_BLOCK("red_wool", Tile::wool_Id, 14);
    ADD_BLOCK("black_wool", Tile::wool_Id, 15);
    // ADD_BLOCK("piston_moving_piece", Tile::pistonMovingPiece_Id, 0);
    ADD_BLOCK("yellow_flower", Tile::flower_Id, 0);
    ADD_BLOCK("red_flower", Tile::rose_Id, 0);
    ADD_BLOCK("brown_mushroom", Tile::mushroom_brown_Id, 0);
    ADD_BLOCK("red_mushroom", Tile::mushroom_red_Id, 0);
    ADD_BLOCK("gold_block", Tile::goldBlock_Id, 0);
    ADD_BLOCK("iron_block", Tile::ironBlock_Id, 0);
    ADD_BLOCK("double_stone_slab", Tile::stoneSlab_Id, 0);
    ADD_BLOCK("double_sandstone_slab", Tile::stoneSlab_Id, 1);
    ADD_BLOCK("double_petrified_oak_slab", Tile::stoneSlab_Id, 2);
    ADD_BLOCK("double_cobblestone_slab", Tile::stoneSlab_Id, 3);
    ADD_BLOCK("double_brick_slab", Tile::stoneSlab_Id, 4);
    ADD_BLOCK("double_stone_brick_slab", Tile::stoneSlab_Id, 5);
    ADD_BLOCK("double_nether_brick_slab", Tile::stoneSlab_Id, 6);
    ADD_BLOCK("double_quartz_slab", Tile::stoneSlab_Id, 7);
    ADD_BLOCK("stone_slab", Tile::stoneSlabHalf_Id, 0);
    ADD_BLOCK("sandstone_slab", Tile::stoneSlabHalf_Id, 1);
    ADD_BLOCK("petrified_oak_slab", Tile::stoneSlabHalf_Id, 2);
    ADD_BLOCK("cobblestone_slab", Tile::stoneSlabHalf_Id, 3);
    ADD_BLOCK("brick_slab", Tile::stoneSlabHalf_Id, 4);
    ADD_BLOCK("stone_brick_slab", Tile::stoneSlabHalf_Id, 5);
    ADD_BLOCK("nether_brick_slab", Tile::stoneSlabHalf_Id, 6);
    ADD_BLOCK("quartz_slab", Tile::stoneSlabHalf_Id, 7);
    ADD_BLOCK("brick_block", Tile::redBrick_Id, 0);
    ADD_BLOCK("tnt", Tile::tnt_Id, 0);
    ADD_BLOCK("bookshelf", Tile::bookshelf_Id, 0);
    ADD_BLOCK("mossy_cobblestone", Tile::mossyCobblestone_Id, 0);
    ADD_BLOCK("obsidian", Tile::obsidian_Id, 0);
    ADD_BLOCK("torch", Tile::torch_Id, 0);
    ADD_BLOCK("fire", Tile::fire_Id, 0);
    ADD_BLOCK("mob_spawner", Tile::mobSpawner_Id, 0);
    ADD_BLOCK("oak_stairs", Tile::stairs_wood_Id, 0);
    ADD_BLOCK("chest", Tile::chest_Id, 0);
    ADD_BLOCK("redstone_wire", Tile::redStoneDust_Id, 0);
    ADD_BLOCK("diamond_ore", Tile::diamondOre_Id, 0);
    ADD_BLOCK("diamond_block", Tile::diamondBlock_Id, 0);
    ADD_BLOCK("crafting_table", Tile::workBench_Id, 0);
    ADD_BLOCK("wheat", Tile::wheat_Id, 0);
    ADD_BLOCK("farmland", Tile::farmland_Id, 0);
    ADD_BLOCK("furnace", Tile::furnace_Id, 0);
    ADD_BLOCK("lit_furnace", Tile::furnace_lit_Id, 0);
    ADD_BLOCK("standing_sign", Tile::sign_Id, 0);
    ADD_BLOCK("wooden_door", Tile::door_wood_Id, 0);
    ADD_BLOCK("ladder", Tile::ladder_Id, 0);
    ADD_BLOCK("rail", Tile::rail_Id, 0);
    ADD_BLOCK("stone_stairs", Tile::stairs_stone_Id, 0);
    ADD_BLOCK("wall_sign", Tile::wallSign_Id, 0);
    ADD_BLOCK("lever", Tile::lever_Id, 0);
    ADD_BLOCK("stone_pressure_plate", Tile::pressurePlate_stone_Id, 0);
    ADD_BLOCK("iron_door", Tile::door_iron_Id, 0);
    ADD_BLOCK("wooden_pressure_plate", Tile::pressurePlate_wood_Id, 0);
    ADD_BLOCK("redstone_ore", Tile::redStoneOre_Id, 0);
    ADD_BLOCK("lit_redstone_ore", Tile::redStoneOre_lit_Id, 0);
    ADD_BLOCK("unlit_redstone_torch", Tile::redstoneTorch_off_Id, 0);
    ADD_BLOCK("redstone_torch", Tile::redstoneTorch_on_Id, 0);
    ADD_BLOCK("stone_button", Tile::button_stone_Id, 0);
    ADD_BLOCK("snow_layer", Tile::topSnow_Id, 0);
    ADD_BLOCK("ice", Tile::ice_Id, 0);
    ADD_BLOCK("snow", Tile::snow_Id, 0);
    ADD_BLOCK("cactus", Tile::cactus_Id, 0);
    ADD_BLOCK("clay", Tile::clay_Id, 0);
    ADD_BLOCK("reeds", Tile::reeds_Id, 0);
    ADD_BLOCK("jukebox", Tile::jukebox_Id, 0);
    ADD_BLOCK("fence", Tile::fence_Id, 0);
    ADD_BLOCK("pumpkin", Tile::pumpkin_Id, 0);
    ADD_BLOCK("netherrack", Tile::netherRack_Id, 0);
    ADD_BLOCK("soul_sand", Tile::soulsand_Id, 0);
    ADD_BLOCK("glowstone", Tile::glowstone_Id, 0);
    ADD_BLOCK("portal", Tile::portalTile_Id, 0);
    ADD_BLOCK("lit_pumpkin", Tile::litPumpkin_Id, 0);
    ADD_BLOCK("cake", Tile::cake_Id, 0);
    ADD_BLOCK("unpowered_repeater", Tile::diode_off_Id, 0);
    ADD_BLOCK("powered_repeater", Tile::diode_on_Id, 0);
    ADD_BLOCK("stained_glass", Tile::stained_glass_Id, 0);
    ADD_BLOCK("trapdoor", Tile::trapdoor_Id, 0);
    ADD_BLOCK("monster_egg", Tile::monsterStoneEgg_Id, 0);
    ADD_BLOCK("stone_bricks", Tile::stoneBrick_Id, 0);
    ADD_BLOCK("mossy_stone_bricks", Tile::stoneBrick_Id, 1);
    ADD_BLOCK("cracked_stone_bricks", Tile::stoneBrick_Id, 2);
    ADD_BLOCK("chiseled_stone_bricks", Tile::stoneBrick_Id, 3);
    ADD_BLOCK("brown_mushroom_block", Tile::hugeMushroom_brown_Id, 0);
    ADD_BLOCK("red_mushroom_block", Tile::hugeMushroom_red_Id, 0);
    ADD_BLOCK("iron_bars", Tile::ironFence_Id, 0);
    ADD_BLOCK("glass_pane", Tile::thinGlass_Id, 0);
    ADD_BLOCK("melon_block", Tile::melon_Id, 0);
    ADD_BLOCK("pumpkin_stem", Tile::pumpkinStem_Id, 0);
    ADD_BLOCK("melon_stem", Tile::melonStem_Id, 0);
    ADD_BLOCK("vine", Tile::vine_Id, 0);
    ADD_BLOCK("fence_gate", Tile::fenceGate_Id, 0);
    ADD_BLOCK("brick_stairs", Tile::stairs_bricks_Id, 0);
    ADD_BLOCK("stone_brick_stairs", Tile::stairs_stoneBrick_Id, 0);
    ADD_BLOCK("mycelium", Tile::mycel_Id, 0);
    ADD_BLOCK("waterlily", Tile::waterLily_Id, 0);
    ADD_BLOCK("nether_brick", Tile::netherBrick_Id, 0);
    ADD_BLOCK("nether_brick_fence", Tile::netherFence_Id, 0);
    ADD_BLOCK("nether_brick_stairs", Tile::stairs_netherBricks_Id, 0);
    ADD_BLOCK("nether_wart", Tile::netherStalk_Id, 0);
    ADD_BLOCK("enchanting_table", Tile::enchantTable_Id, 0);
    ADD_BLOCK("brewing_stand", Tile::brewingStand_Id, 0);
    ADD_BLOCK("cauldron", Tile::cauldron_Id, 0);
    ADD_BLOCK("end_portal", Tile::endPortalTile_Id, 0);
    ADD_BLOCK("end_portal_frame", Tile::endPortalFrameTile_Id, 0);
    ADD_BLOCK("white_stone", Tile::endStone_Id, 0);
    ADD_BLOCK("dragon_egg", Tile::dragonEgg_Id, 0);
    ADD_BLOCK("redstone_lamp", Tile::redstoneLight_Id, 0);
    ADD_BLOCK("lit_redstone_lamp", Tile::redstoneLight_lit_Id, 0);
    ADD_BLOCK("double_oak_slab", Tile::woodSlab_Id, 0);
    ADD_BLOCK("double_spruce_slab", Tile::woodSlab_Id, 1);
    ADD_BLOCK("double_birch_slab", Tile::woodSlab_Id, 2);
    ADD_BLOCK("double_jungle_slab", Tile::woodSlab_Id, 3);
    ADD_BLOCK("oak_slab", Tile::woodSlabHalf_Id, 0);
    ADD_BLOCK("spruce_slab", Tile::woodSlabHalf_Id, 1);
    ADD_BLOCK("birch_slab", Tile::woodSlabHalf_Id, 2);
    ADD_BLOCK("jungle_slab", Tile::woodSlabHalf_Id, 3);
    ADD_BLOCK("cocoa", Tile::cocoa_Id, 0);
    ADD_BLOCK("sandstone_stairs", Tile::stairs_sandstone_Id, 0);
    ADD_BLOCK("spruce_stairs", Tile::stairs_sprucewood_Id, 0);
    ADD_BLOCK("birch_stairs", Tile::stairs_birchwood_Id, 0);
    ADD_BLOCK("jungle_stairs", Tile::stairs_junglewood_Id, 0);
    ADD_BLOCK("emerald_ore", Tile::emeraldOre_Id, 0);
    ADD_BLOCK("ender_chest", Tile::enderChest_Id, 0);
    ADD_BLOCK("tripwire_hook", Tile::tripWireSource_Id, 0);
    // ADD_BLOCK("tripwire_hook", Tile::tripWire_Id, 0);
    ADD_BLOCK("emerald_block", Tile::emeraldBlock_Id, 0);
    ADD_BLOCK("cobblestone_wall", Tile::cobbleWall_Id, 0);
    ADD_BLOCK("mossy_cobblestone_wall", Tile::cobbleWall_Id, 1);
    ADD_BLOCK("flower_pot", Tile::flowerPot_Id, 0);
    ADD_BLOCK("carrots", Tile::carrots_Id, 0);
    ADD_BLOCK("potatoes", Tile::potatoes_Id, 0);
    ADD_BLOCK("wooden_button", Tile::button_wood_Id, 0);
    // ADD_BLOCK("skull", Tile::skull_Id, 0);
    ADD_BLOCK("anvil", Tile::anvil_Id, 0);
    ADD_BLOCK("quartz_ore", Tile::netherQuartz_Id, 0);
    ADD_BLOCK("quartz_block", Tile::quartzBlock_Id, 0);
    ADD_BLOCK("chiseled_quartz_block", Tile::quartzBlock_Id, 1);
    ADD_BLOCK("quartz_pillar", Tile::quartzBlock_Id, 2);
    ADD_BLOCK("quartz_stairs", Tile::stairs_quartz_Id, 0);
    ADD_BLOCK("white_carpet", Tile::woolCarpet_Id, 0);
    ADD_BLOCK("orange_carpet", Tile::woolCarpet_Id, 1);
    ADD_BLOCK("magenta_carpet", Tile::woolCarpet_Id, 2);
    ADD_BLOCK("light_blue_carpet", Tile::woolCarpet_Id, 3);
    ADD_BLOCK("yellow_carpet", Tile::woolCarpet_Id, 4);
    ADD_BLOCK("lime_carpet", Tile::woolCarpet_Id, 5);
    ADD_BLOCK("pink_carpet", Tile::woolCarpet_Id, 6);
    ADD_BLOCK("gray_carpet", Tile::woolCarpet_Id, 7);
    ADD_BLOCK("light_gray_carpet", Tile::woolCarpet_Id, 8);
    ADD_BLOCK("cyan_carpet", Tile::woolCarpet_Id, 9);
    ADD_BLOCK("purple_carpet", Tile::woolCarpet_Id, 10);
    ADD_BLOCK("blue_carpet", Tile::woolCarpet_Id, 11);
    ADD_BLOCK("brown_carpet", Tile::woolCarpet_Id, 12);
    ADD_BLOCK("green_carpet", Tile::woolCarpet_Id, 13);
    ADD_BLOCK("red_carpet", Tile::woolCarpet_Id, 14);
    ADD_BLOCK("black_carpet", Tile::woolCarpet_Id, 15);
    ADD_BLOCK("command_block", Tile::commandBlock_Id, 0);
    ADD_BLOCK("beacon", Tile::beacon_Id, 0);
    ADD_BLOCK("trapped_chest", Tile::chest_trap_Id, 0);
    ADD_BLOCK("light_weighted_pressure_plate", Tile::weightedPlate_light_Id, 0);
    ADD_BLOCK("heavy_weighted_pressure_plate", Tile::weightedPlate_heavy_Id, 0);
    ADD_BLOCK("unpowered_comparator", Tile::comparator_off_Id, 0);
    ADD_BLOCK("powered_comparator", Tile::comparator_on_Id, 0);
    ADD_BLOCK("daylight_detector", Tile::daylightDetector_Id, 0);
    ADD_BLOCK("redstone_block", Tile::redstoneBlock_Id, 0);
    ADD_BLOCK("hopper", Tile::hopper_Id, 0);
    ADD_BLOCK("activator_rail", Tile::activatorRail_Id, 0);
    ADD_BLOCK("dropper", Tile::dropper_Id, 0);
    ADD_BLOCK("stained_hardened_clay", Tile::clayHardened_colored_Id, 0);
    ADD_BLOCK("stained_glass_pane", Tile::stained_glass_pane_Id, 0);
    ADD_BLOCK("hay_block", Tile::hayBlock_Id, 0);
    ADD_BLOCK("hardened_clay", Tile::clayHardened_Id, 0);
    ADD_BLOCK("coal_block", Tile::coalBlock_Id, 0);

    ADD_BLOCK("packed_ice", Tile::packedIce_Id, 0);
    ADD_BLOCK("blue_ice", Tile::blueIce_Id, 0);
    ADD_BLOCK("packed_glass", Tile::packedGlass_Id, 0);
    ADD_BLOCK("end_stone_button", Tile::endStoneButton_Id, 0);
    ADD_BLOCK("iron_trapdoor", Tile::ironTrapdoor_Id, 0);
    ADD_BLOCK("nether_diamond_ore", Tile::netherDiamond_Id, 0);
    ADD_BLOCK("nether_gold_ore", Tile::netherGold_Id, 0);
    ADD_BLOCK("goldenclin", Tile::goldenclin_Id, 0);
    ADD_BLOCK("aether_portal", Tile::aetherPortal_Id, 0);
    ADD_BLOCK("nether_soil", Tile::netherSoil_Id, 0);
    ADD_BLOCK("lava_rock", Tile::lavaRock_Id, 0);
    ADD_BLOCK("nether_leaves", Tile::netherLeaves_Id, 0);
    ADD_BLOCK("nether_wood", Tile::netherWood_Id, 0);
    ADD_BLOCK("nether_planks", Tile::netherPlanks_Id, 0);
    ADD_BLOCK("nether_vine", Tile::netherVine_Id, 0);
    ADD_BLOCK("mossy_nether_bricks", Tile::netherBricksMossy_Id, 0);
    ADD_BLOCK("nethanium_ore", Tile::nethaniumOre_Id, 0);
    ADD_BLOCK("nethanium_block", Tile::nethaniumBlock_Id, 0);
    ADD_BLOCK("nether_sapling", Tile::netherSapling_Id, 0);
    ADD_BLOCK("nether_furnace", Tile::nether_furnace_Id, 0);
    ADD_BLOCK("lit_nether_furnace", Tile::nether_furnace_lit_Id, 0);
    ADD_BLOCK("stained_sand", Tile::stainedSand_Id, 0);
    ADD_BLOCK("endothanium_block", Tile::endothaniumBlock_Id, 0);
    ADD_BLOCK("aether_grass", Tile::aetherGrass_Id, 0);
    ADD_BLOCK("veloett_grass", Tile::veloettGrass_Id, 0);
    ADD_BLOCK("end_sand", Tile::endSand_Id, 0);
    ADD_BLOCK("endorium_ore", Tile::endoriumOre_Id, 0);
    ADD_BLOCK("endorium_block", Tile::endoriumBlock_Id, 0);
    ADD_BLOCK("endorium_grate", Tile::endoriumGrate_Id, 0);
    ADD_BLOCK("endorium_lamp", Tile::endoriumLamp_Id, 0);
    ADD_BLOCK("end_gateway", Tile::endGateway_Id, 0);
    ADD_BLOCK("purul_veloett", Tile::purulVeloett_Id, 0);
    ADD_BLOCK("purul_log", Tile::purulLog_Id, 0);
    ADD_BLOCK("icestone", Tile::icestone_Id, 0);
    ADD_BLOCK("endactus", Tile::endactus_Id, 0);
    ADD_BLOCK("endesert_bush", Tile::endesertBush_Id, 0);
    ADD_BLOCK("veloett_shrub", Tile::veloettShrub_Id, 0);
    ADD_BLOCK("veloett_flower", Tile::veloettFlower_Id, 0);
    ADD_BLOCK("veloett_vine", Tile::veloettVine_Id, 0);
    ADD_BLOCK("faceless_pumpkin", Tile::pumpkinFaceless_Id, 0);
    ADD_BLOCK("double_stone_slab2", Tile::stoneSlab2_Id, 0);
    ADD_BLOCK("stone_slab2", Tile::stoneSlab2Half_Id, 0);
    ADD_BLOCK("skyroot_log", Tile::skyrootLog_Id, 0);
    ADD_BLOCK("golden_oak_log", Tile::goldenOakLog_Id, 0);
    ADD_BLOCK("skyroot_sapling", Tile::skyrootSapling_Id, 0);
    ADD_BLOCK("golden_oak_sapling", Tile::goldenOakSapling_Id, 0);
    ADD_BLOCK("ambrosium_ore", Tile::ambrosiumOre_Id, 0);
    ADD_BLOCK("zanite_ore", Tile::zaniteOre_Id, 0);
    ADD_BLOCK("gravitite_ore", Tile::gravititeOre_Id, 0);
    ADD_BLOCK("aercloud", Tile::aercloud_Id, 0);
    ADD_BLOCK("quicksoil", Tile::quicksoil_Id, 0);
    ADD_BLOCK("aether_dirt", Tile::aetherDirt_Id, 0);
    ADD_BLOCK("holystone", Tile::holystone_Id, 0);
    ADD_BLOCK("ambrosium_torch", Tile::ambrosiumTorch_Id, 0);
    ADD_BLOCK("enchanted_gravitite", Tile::enchantedGravitite_Id, 0);
    ADD_BLOCK("aerogel", Tile::aerogel_Id, 0);
    ADD_BLOCK("berry_bush_stem", Tile::berryBushStem_Id, 0);
    ADD_BLOCK("zanite_block", Tile::zaniteBlock_Id, 0);
    ADD_BLOCK("enchanter", Tile::enchanter_Id, 0);
    ADD_BLOCK("ambrosium_block", Tile::ambrosiumBlock_Id, 0);
    ADD_BLOCK("dungeon_stone", Tile::dungeonStone_Id, 0);
    ADD_BLOCK("dungeon_light", Tile::dungeonLight_Id, 0);
    ADD_BLOCK("treasure_chest", Tile::treasureChest_Id, 0);
    ADD_BLOCK("boss_stone", Tile::bossStone_Id, 0);
    ADD_BLOCK("nustone", Tile::nustone_Id, 0);
    ADD_BLOCK("nugrass", Tile::nugrass_Id, 0);
    ADD_BLOCK("glowing_nustone", Tile::glowingNustone_Id, 0);
    ADD_BLOCK("nusa_log", Tile::nusaLog_Id, 0);
    ADD_BLOCK("nusa_shrub", Tile::nusaShrub_Id, 0);
    ADD_BLOCK("nusa_vine", Tile::nusaVine_Id, 0);
    ADD_BLOCK("nusa_cube", Tile::nusaCube_Id, 0);
    ADD_BLOCK("nusa_core", Tile::nusaCore_Id, 0);
    ADD_BLOCK("nusa_portal", Tile::nusaPortal_Id, 0);
    ADD_BLOCK("fossil_ore", Tile::fossilOre_Id, 0);
    ADD_BLOCK("blackroot", Tile::blackroot_Id, 0);
    ADD_BLOCK("lava_reed", Tile::lavaReed_Id, 0);
    ADD_BLOCK("bone_vine", Tile::boneVine_Id, 0);
    ADD_BLOCK("nether_stairs", Tile::netherStairs_Id, 0);
    ADD_BLOCK("purul_stairs", Tile::purulStairs_Id, 0);
    ADD_BLOCK("skyroot_stairs", Tile::skyrootStairs_Id, 0);
    ADD_BLOCK("nusa_stairs", Tile::nusaStairs_Id, 0);
    ADD_BLOCK("goldenclin_stairs", Tile::goldenclinStairs_Id, 0);
    ADD_BLOCK("goldenclin_brick_stairs", Tile::goldenclinBrickStairs_Id, 0);
    ADD_BLOCK("nustone_stairs", Tile::nustoneStairs_Id, 0);
    ADD_BLOCK("nustone_brick_stairs", Tile::nustoneBrickStairs_Id, 0);
    ADD_BLOCK("dark_nustone_stairs", Tile::darkNustoneStairs_Id, 0);

    /* Items */
    ADD_ITEM("string", Item::string_Id, 0);

    ADD_ITEM("iron_shovel", Item::shovel_iron_Id, 0);
    ADD_ITEM("iron_pickaxe", Item::pickAxe_iron_Id, 0);
    ADD_ITEM("iron_axe", Item::hatchet_iron_Id, 0);
    ADD_ITEM("flint_and_steel", Item::flintAndSteel_Id, 0);
    ADD_ITEM("apple", Item::apple_Id, 0);
    ADD_ITEM("bow", Item::bow_Id, 0);
    ADD_ITEM("arrow", Item::arrow_Id, 0);
    ADD_ITEM("coal", Item::coal_Id, 0);
    ADD_ITEM("charcoal", Item::coal_Id, 1);
    ADD_ITEM("diamond", Item::diamond_Id, 0);
    ADD_ITEM("iron_ingot", Item::ironIngot_Id, 0);
    ADD_ITEM("gold_ingot", Item::goldIngot_Id, 0);

    ADD_ITEM("iron_sword", Item::sword_iron_Id, 0);
    ADD_ITEM("wooden_sword", Item::sword_wood_Id, 0);
    ADD_ITEM("stone_sword", Item::sword_stone_Id, 0);
    ADD_ITEM("diamond_sword", Item::sword_diamond_Id, 0);
    ADD_ITEM("golden_sword", Item::sword_gold_Id, 0);

    ADD_ITEM("wooden_shovel", Item::shovel_wood_Id, 0);
    ADD_ITEM("wooden_pickaxe", Item::pickAxe_wood_Id, 0);
    ADD_ITEM("wooden_axe", Item::hatchet_wood_Id, 0);

    ADD_ITEM("stone_shovel", Item::shovel_stone_Id, 0);
    ADD_ITEM("stone_pickaxe", Item::pickAxe_stone_Id, 0);
    ADD_ITEM("stone_axe", Item::hatchet_stone_Id, 0);

    ADD_ITEM("diamond_shovel", Item::shovel_diamond_Id, 0);
    ADD_ITEM("diamond_pickaxe", Item::pickAxe_diamond_Id, 0);
    ADD_ITEM("diamond_axe", Item::hatchet_diamond_Id, 0);

    ADD_ITEM("stick", Item::stick_Id, 0);
    ADD_ITEM("bowl", Item::bowl_Id, 0);
    ADD_ITEM("mushroom_stew", Item::mushroomStew_Id, 0);

    ADD_ITEM("feather", Item::feather_Id, 0);
    ADD_ITEM("gunpowder", Item::gunpowder_Id, 0);

    ADD_ITEM("wooden_hoe", Item::hoe_wood_Id, 0);
    ADD_ITEM("stone_hoe", Item::hoe_stone_Id, 0);
    ADD_ITEM("iron_hoe", Item::hoe_iron_Id, 0);
    ADD_ITEM("diamond_hoe", Item::hoe_diamond_Id, 0);
    ADD_ITEM("golden_hoe", Item::hoe_gold_Id, 0);

    ADD_ITEM("wheat_seeds", Item::seeds_wheat_Id, 0);
    ADD_ITEM("wheat", Item::wheat_Id, 0);
    ADD_ITEM("bread", Item::bread_Id, 0);

    ADD_ITEM("leather_helmet", Item::helmet_leather_Id, 0);
    ADD_ITEM("leather_chestplate", Item::chestplate_leather_Id, 0);
    ADD_ITEM("leather_leggings", Item::leggings_leather_Id, 0);
    ADD_ITEM("leather_boots", Item::boots_leather_Id, 0);

    ADD_ITEM("iron_helmet", Item::helmet_iron_Id, 0);
    ADD_ITEM("iron_chestplate", Item::chestplate_iron_Id, 0);
    ADD_ITEM("iron_leggings", Item::leggings_iron_Id, 0);
    ADD_ITEM("iron_boots", Item::boots_iron_Id, 0);

    ADD_ITEM("diamond_helmet", Item::helmet_diamond_Id, 0);
    ADD_ITEM("diamond_chestplate", Item::chestplate_diamond_Id, 0);
    ADD_ITEM("diamond_leggings", Item::leggings_diamond_Id, 0);
    ADD_ITEM("diamond_boots", Item::boots_diamond_Id, 0);

    ADD_ITEM("golden_helmet", Item::helmet_gold_Id, 0);
    ADD_ITEM("golden_chestplate", Item::chestplate_gold_Id, 0);
    ADD_ITEM("golden_leggings", Item::leggings_gold_Id, 0);
    ADD_ITEM("golden_boots", Item::boots_gold_Id, 0);

    ADD_ITEM("flint", Item::flint_Id, 0);
    ADD_ITEM("porkchop", Item::porkChop_raw_Id, 0);
    ADD_ITEM("cooked_porkchop", Item::porkChop_cooked_Id, 0);

    ADD_ITEM("painting", Item::painting_Id, 0);
    ADD_ITEM("golden_apple", Item::apple_gold_Id, 0);
    ADD_ITEM("sign", Item::sign_Id, 0);

    ADD_ITEM("bucket", Item::bucket_empty_Id, 0);
    ADD_ITEM("water_bucket", Item::bucket_water_Id, 0);
    ADD_ITEM("lava_bucket", Item::bucket_lava_Id, 0);

    ADD_ITEM("minecart", Item::minecart_Id, 0);
    ADD_ITEM("saddle", Item::saddle_Id, 0);

    ADD_ITEM("redstone", Item::redStone_Id, 0);
    ADD_ITEM("snowball", Item::snowBall_Id, 0);
    ADD_ITEM("boat", Item::boat_Id, 0);

    ADD_ITEM("leather", Item::leather_Id, 0);
    ADD_ITEM("milk_bucket", Item::bucket_milk_Id, 0);

    ADD_ITEM("brick", Item::brick_Id, 0);
    ADD_ITEM("clay_ball", Item::clay_Id, 0);

    ADD_ITEM("reeds", Item::reeds_Id, 0);
    ADD_ITEM("paper", Item::paper_Id, 0);
    ADD_ITEM("book", Item::book_Id, 0);

    ADD_ITEM("slime_ball", Item::slimeBall_Id, 0);

    ADD_ITEM("egg", Item::egg_Id, 0);
    ADD_ITEM("compass", Item::compass_Id, 0);
    ADD_ITEM("fishing_rod", Item::fishingRod_Id, 0);
    ADD_ITEM("clock", Item::clock_Id, 0);

    ADD_ITEM("bone", Item::bone_Id, 0);
    ADD_ITEM("sugar", Item::sugar_Id, 0);
    ADD_ITEM("cake", Item::cake_Id, 0);
    ADD_ITEM("empty_map", Item::emptyMap_Id, 0);

    ADD_ITEM("cookie", Item::cookie_Id, 0);
    ADD_ITEM("map", Item::map_Id, 0);

    ADD_ITEM("shears", Item::shears_Id, 0);

    ADD_ITEM("melon", Item::melon_Id, 0);
    ADD_ITEM("pumpkin_seeds", Item::seeds_pumpkin_Id, 0);
    ADD_ITEM("melon_seeds", Item::seeds_melon_Id, 0);

    ADD_ITEM("beef", Item::beef_raw_Id, 0);
    ADD_ITEM("cooked_beef", Item::beef_cooked_Id, 0);

    ADD_ITEM("chicken", Item::chicken_raw_Id, 0);
    ADD_ITEM("cooked_chicken", Item::chicken_cooked_Id, 0);

    ADD_ITEM("rotten_flesh", Item::rotten_flesh_Id, 0);
    ADD_ITEM("ender_pearl", Item::enderPearl_Id, 0);

    ADD_ITEM("blaze_rod", Item::blazeRod_Id, 0);
    ADD_ITEM("ghast_tear", Item::ghastTear_Id, 0);
    ADD_ITEM("gold_nugget", Item::goldNugget_Id, 0);

    ADD_ITEM("potion", Item::potion_Id, 0);
    ADD_ITEM("glass_bottle", Item::glassBottle_Id, 0);

    ADD_ITEM("spider_eye", Item::spiderEye_Id, 0);
    ADD_ITEM("blaze_powder", Item::blazePowder_Id, 0);
    ADD_ITEM("magma_cream", Item::magmaCream_Id, 0);

    ADD_ITEM("brewing_stand", Item::brewingStand_Id, 0);
    ADD_ITEM("cauldron", Item::cauldron_Id, 0);

    ADD_ITEM("ender_eye", Item::eyeOfEnder_Id, 0);

    ADD_ITEM("experience_bottle", Item::expBottle_Id, 0);

    ADD_ITEM("emerald", Item::emerald_Id, 0);

    ADD_ITEM("carrot", Item::carrots_Id, 0);
    ADD_ITEM("potato", Item::potato_Id, 0);
    ADD_ITEM("baked_potato", Item::potatoBaked_Id, 0);

    ADD_ITEM("golden_carrot", Item::carrotGolden_Id, 0);

    ADD_ITEM("carrot_on_a_stick", Item::carrotOnAStick_Id, 0);
    ADD_ITEM("pumpkin_pie", Item::pumpkinPie_Id, 0);

    ADD_ITEM("enchanted_book", Item::enchantedBook_Id, 0);
    ADD_ITEM("quartz", Item::netherQuartz_Id, 0);

    ADD_ITEM("chainmail_helmet", Item::helmet_chain_Id, 0);
    ADD_ITEM("chainmail_chestplate", Item::chestplate_chain_Id, 0);
    ADD_ITEM("chainmail_leggings", Item::leggings_chain_Id, 0);
    ADD_ITEM("chainmail_boots", Item::boots_chain_Id, 0);

    ADD_ITEM("wooden_door", Item::door_wood_Id, 0);
    ADD_ITEM("iron_door", Item::door_iron_Id, 0);
    ADD_ITEM("chest_minecart", Item::minecart_chest_Id, 0);
    ADD_ITEM("furnace_minecart", Item::minecart_furnace_Id, 0);
    ADD_ITEM("glowstone_dust", Item::yellowDust_Id, 0);
    ADD_ITEM("fish", Item::fish_raw_Id, 0);
    ADD_ITEM("cooked_fish", Item::fish_cooked_Id, 0);
    ADD_ITEM("ink_sac", Item::dye_powder_Id, 0);
    ADD_ITEM("rose_red", Item::dye_powder_Id, 1);
    ADD_ITEM("cactus_green", Item::dye_powder_Id, 2);
    ADD_ITEM("cocoa_beans", Item::dye_powder_Id, 3);
    ADD_ITEM("lapis_lazuli", Item::dye_powder_Id, 4);
    ADD_ITEM("purple_dye", Item::dye_powder_Id, 5);
    ADD_ITEM("cyan_dye", Item::dye_powder_Id, 6);
    ADD_ITEM("light_gray_dye", Item::dye_powder_Id, 7);
    ADD_ITEM("gray_dye", Item::dye_powder_Id, 8);
    ADD_ITEM("pink_dye", Item::dye_powder_Id, 9);
    ADD_ITEM("lime_dye", Item::dye_powder_Id, 10);
    ADD_ITEM("dandelion_yellow", Item::dye_powder_Id, 11);
    ADD_ITEM("light_blue_dye", Item::dye_powder_Id, 12);
    ADD_ITEM("magenta_dye", Item::dye_powder_Id, 13);
    ADD_ITEM("orange_dye", Item::dye_powder_Id, 14);
    ADD_ITEM("bone_meal", Item::dye_powder_Id, 15);
    ADD_ITEM("nether_wart", Item::netherwart_seeds_Id, 0);
    ADD_ITEM("fermented_spider_eye", Item::fermentedSpiderEye_Id, 0);
    ADD_ITEM("speckled_melon", Item::speckledMelon_Id, 0);
    ADD_ITEM("spawn_egg", Item::spawnEgg_Id, 0);
    ADD_ITEM("skeleton_skull", Item::skull_Id, 0);
    ADD_ITEM("wither_skeleton_skull", Item::skull_Id, 1);
    ADD_ITEM("zombie_head", Item::skull_Id, 2);
    ADD_ITEM("player_head", Item::skull_Id, 3);
    ADD_ITEM("creeper_head", Item::skull_Id, 4);
    ADD_ITEM("fire_charge", Item::fireball_Id, 0);
    ADD_ITEM("item_frame", Item::itemFrame_Id, 0);
    ADD_ITEM("netherbrick", Item::netherbrick_Id, 0);
    ADD_ITEM("flower_pot", Item::flowerPot_Id, 0);
    ADD_ITEM("poisonous_potato", Item::potatoPoisonous_Id, 0);

    ADD_ITEM("record_13", Item::record_01_Id, 0);
    ADD_ITEM("record_cat", Item::record_02_Id, 0);
    ADD_ITEM("record_blocks", Item::record_03_Id, 0);
    ADD_ITEM("record_chirp", Item::record_04_Id, 0);
    ADD_ITEM("record_far", Item::record_05_Id, 0);
    ADD_ITEM("record_mall", Item::record_06_Id, 0);
    ADD_ITEM("record_mellohi", Item::record_07_Id, 0);
    ADD_ITEM("record_stal", Item::record_08_Id, 0);
    ADD_ITEM("record_strad", Item::record_09_Id, 0);
    ADD_ITEM("record_ward", Item::record_10_Id, 0);
    ADD_ITEM("record_11", Item::record_11_Id, 0);
    ADD_ITEM("record_wait", Item::record_12_Id, 0);
    ADD_ITEM("golden_shovel", Item::shovel_gold_Id, 0);
    ADD_ITEM("golden_pickaxe", Item::pickAxe_gold_Id, 0);
    ADD_ITEM("golden_axe", Item::hatchet_gold_Id, 0);
    ADD_ITEM("repeater", Item::repeater_Id, 0);
    ADD_ITEM("nether_star", Item::netherStar_Id, 0);
    ADD_ITEM("fireworks", Item::fireworks_Id, 0);
    ADD_ITEM("firework_charge", Item::fireworksCharge_Id, 0);
    ADD_ITEM("comparator", Item::comparator_Id, 0);
    ADD_ITEM("tnt_minecart", Item::minecart_tnt_Id, 0);
    ADD_ITEM("hopper_minecart", Item::minecart_hopper_Id, 0);
    ADD_ITEM("iron_horse_armor", Item::horseArmorMetal_Id, 0);
    ADD_ITEM("golden_horse_armor", Item::horseArmorGold_Id, 0);
    ADD_ITEM("diamond_horse_armor", Item::horseArmorDiamond_Id, 0);
    ADD_ITEM("lead", Item::lead_Id, 0);
    ADD_ITEM("name_tag", Item::nameTag_Id, 0);

    ADD_ITEM("nethanium_ingot", Item::nethanium_Id, 0);
    ADD_ITEM("nethanium_sword", Item::nethaniumSword_Id, 0);
    ADD_ITEM("nethanium_axe", Item::nethaniumAxe_Id, 0);
    ADD_ITEM("nethanium_pickaxe", Item::nethaniumPickaxe_Id, 0);
    ADD_ITEM("nethanium_shovel", Item::nethaniumShovel_Id, 0);
    ADD_ITEM("nethanium_hoe", Item::nethaniumHoe_Id, 0);
    ADD_ITEM("nethanium_helmet", Item::nethaniumHelmet_Id, 0);
    ADD_ITEM("nethanium_chestplate", Item::nethaniumChestplate_Id, 0);
    ADD_ITEM("nethanium_leggings", Item::nethaniumLeggings_Id, 0);
    ADD_ITEM("nethanium_boots", Item::nethaniumBoots_Id, 0);
    ADD_ITEM("nethanium_bread", Item::nethaniumBread_Id, 0);
    ADD_ITEM("nethanium_pendant", Item::nethaniumPendant_Id, 0);
    ADD_ITEM("nethanium_ring", Item::nethaniumRing_Id, 0);
    ADD_ITEM("nether_bread", Item::netherBread_Id, 0);
    ADD_ITEM("gold_bread", Item::goldBread_Id, 0);
    ADD_ITEM("hell_sphere", Item::hellSphere_Id, 0);
    ADD_ITEM("relic_mallet", Item::relicMallet_Id, 0);
    ADD_ITEM("nether_flax", Item::netherFlax_Id, 0);
    ADD_ITEM("lava_reeds", Item::lavaReeds_Id, 0);
    ADD_ITEM("blackroot_berry", Item::blackrootBerry_Id, 0);
    ADD_ITEM("magmaroot_berry", Item::magmarootBerry_Id, 0);
    ADD_ITEM("ambrosium_shard", Item::ambrosiumShard_Id, 0);
    ADD_ITEM("zanite_gemstone", Item::zaniteGemstone_Id, 0);
    ADD_ITEM("golden_amber", Item::goldenAmber_Id, 0);
    ADD_ITEM("aechor_petal", Item::aechorPetal_Id, 0);
    ADD_ITEM("veloett_berry", Item::veloettBerry_Id, 0);
    ADD_ITEM("blue_berry", Item::blueBerry_Id, 0);
    ADD_ITEM("enchanted_blue_berry", Item::blueBerryEnchanted_Id, 0);
    ADD_ITEM("swet_ball", Item::swetBall_Id, 0);
    ADD_ITEM("blue_gummy_swet", Item::gummySwetBlue_Id, 0);
    ADD_ITEM("golden_gummy_swet", Item::gummySwetGold_Id, 0);
    ADD_ITEM("healing_stone", Item::healingStone_Id, 0);
    ADD_ITEM("regeneration_stone", Item::regenerationStone_Id, 0);
    ADD_ITEM("life_shard", Item::lifeShard_Id, 0);
    ADD_ITEM("iron_bubble", Item::ironBubble_Id, 0);
    ADD_ITEM("cloud_parachute", Item::cloudParachute_Id, 0);
    ADD_ITEM("evupul_wing", Item::evupulWing_Id, 0);
    ADD_ITEM("evupul_wing_gold", Item::evupulWingGold_Id, 0);
    ADD_ITEM("isogal_rib", Item::isogalRib_Id, 0);
    ADD_ITEM("staff_shard", Item::staffShard_Id, 0);
    ADD_ITEM("bronze_key", Item::keyBronze_Id, 0);
    ADD_ITEM("silver_key", Item::keySilver_Id, 0);
    ADD_ITEM("golden_key", Item::keyGold_Id, 0);
    ADD_ITEM("blue_moa_egg", Item::moaEggBlue_Id, 0);
    ADD_ITEM("white_moa_egg", Item::moaEggWhite_Id, 0);
    ADD_ITEM("black_moa_egg", Item::moaEggBlack_Id, 0);
    ADD_ITEM("skyroot_bucket", Item::skyrootBucket_empty_Id, 0);
    ADD_ITEM("skyroot_water_bucket", Item::skyrootBucket_water_Id, 0);
    ADD_ITEM("skyroot_milk_bucket", Item::skyrootBucket_milk_Id, 0);
    ADD_ITEM("skyroot_poison_bucket", Item::skyrootBucket_poison_Id, 0);
    ADD_ITEM("sun_fish", Item::sunFish_Id, 0);
    ADD_ITEM("cooked_sun_fish", Item::sunFishCooked_Id, 0);
    ADD_ITEM("moon_fish", Item::moonFish_Id, 0);
    ADD_ITEM("cooked_moon_fish", Item::moonFishCooked_Id, 0);
    ADD_ITEM("cloud_fish", Item::cloudFish_Id, 0);
    ADD_ITEM("cooked_cloud_fish", Item::cloudFishCooked_Id, 0);
    ADD_ITEM("holystone_sword", Item::holystoneSword_Id, 0);
    ADD_ITEM("holystone_pickaxe", Item::holystonePickaxe_Id, 0);
    ADD_ITEM("holystone_axe", Item::holystoneAxe_Id, 0);
    ADD_ITEM("holystone_shovel", Item::holystoneShovel_Id, 0);
    ADD_ITEM("holystone_hoe", Item::holystoneHoe_Id, 0);
    ADD_ITEM("skyroot_sword", Item::skyrootSword_Id, 0);
    ADD_ITEM("skyroot_pickaxe", Item::skyrootPickaxe_Id, 0);
    ADD_ITEM("skyroot_axe", Item::skyrootAxe_Id, 0);
    ADD_ITEM("skyroot_shovel", Item::skyrootShovel_Id, 0);
    ADD_ITEM("skyroot_hoe", Item::skyrootHoe_Id, 0);
    ADD_ITEM("zanite_sword", Item::zaniteSword_Id, 0);
    ADD_ITEM("zanite_pickaxe", Item::zanitePickaxe_Id, 0);
    ADD_ITEM("zanite_axe", Item::zaniteAxe_Id, 0);
    ADD_ITEM("zanite_shovel", Item::zaniteShovel_Id, 0);
    ADD_ITEM("zanite_hoe", Item::zaniteHoe_Id, 0);
    ADD_ITEM("zanite_helmet", Item::zaniteHelmet_Id, 0);
    ADD_ITEM("zanite_chestplate", Item::zaniteChestplate_Id, 0);
    ADD_ITEM("zanite_leggings", Item::zaniteLeggings_Id, 0);
    ADD_ITEM("zanite_boots", Item::zaniteBoots_Id, 0);
    ADD_ITEM("gravitite_sword", Item::gravititeSword_Id, 0);
    ADD_ITEM("gravitite_pickaxe", Item::gravititePickaxe_Id, 0);
    ADD_ITEM("gravitite_axe", Item::gravititeAxe_Id, 0);
    ADD_ITEM("gravitite_shovel", Item::gravititeShovel_Id, 0);
    ADD_ITEM("gravitite_hoe", Item::gravititeHoe_Id, 0);
    ADD_ITEM("gravitite_helmet", Item::gravititeHelmet_Id, 0);
    ADD_ITEM("gravitite_chestplate", Item::gravititeChestplate_Id, 0);
    ADD_ITEM("gravitite_leggings", Item::gravititeLeggings_Id, 0);
    ADD_ITEM("gravitite_boots", Item::gravititeBoots_Id, 0);
    ADD_ITEM("valkyrie_lance", Item::valkyrieLance_Id, 0);
    ADD_ITEM("valkyrie_axe", Item::valkyrieAxe_Id, 0);
    ADD_ITEM("valkyrie_pickaxe", Item::valkyriePickaxe_Id, 0);
    ADD_ITEM("valkyrie_shovel", Item::valkyrieShovel_Id, 0);
    ADD_ITEM("valkyrie_hoe", Item::valkyrieHoe_Id, 0);
    ADD_ITEM("vampire_blade", Item::vampireBlade_Id, 0);
    ADD_ITEM("aphalaf_sword", Item::aphalafSword_Id, 0);
    ADD_ITEM("aphalaf_tooth", Item::aphalafTooth_Id, 0);
    ADD_ITEM("aphalaf_shard", Item::aphalafShard_Id, 0);
    ADD_ITEM("aphalaf_rod", Item::aphalafRod_Id, 0);
    ADD_ITEM("aphalaf_gem", Item::aphalafGem_Id, 0);
    ADD_ITEM("iron_pendant", Item::ironPendant_Id, 0);
    ADD_ITEM("iron_ring", Item::ironRing_Id, 0);
    ADD_ITEM("gold_pendant", Item::goldPendant_Id, 0);
    ADD_ITEM("gold_ring", Item::goldRing_Id, 0);
    ADD_ITEM("zanite_pendant", Item::zanitePendant_Id, 0);
    ADD_ITEM("zanite_ring", Item::zaniteRing_Id, 0);
    ADD_ITEM("gravitite_pendant", Item::gravititePendant_Id, 0);
    ADD_ITEM("gravitite_ring", Item::gravititeRing_Id, 0);
    ADD_ITEM("agility_cape", Item::agilityCape_Id, 0);
    ADD_ITEM("invisibility_cape", Item::invisibilityCape_Id, 0);
    ADD_ITEM("swet_cape", Item::swetCape_Id, 0);
    ADD_ITEM("valkyrie_cape", Item::valkyrieCape_Id, 0);
    ADD_ITEM("endorium_ingot", Item::endorium_Id, 0);
    ADD_ITEM("endothanium_ingot", Item::endothaniumIngot_Id, 0);
    ADD_ITEM("endorium_sword", Item::endoriumSword_Id, 0);
    ADD_ITEM("endorium_pickaxe", Item::endoriumPickaxe_Id, 0);
    ADD_ITEM("endorium_axe", Item::endoriumAxe_Id, 0);
    ADD_ITEM("endorium_shovel", Item::endoriumShovel_Id, 0);
    ADD_ITEM("endorium_hoe", Item::endoriumHoe_Id, 0);
    ADD_ITEM("endorium_helmet", Item::endoriumHelmet_Id, 0);
    ADD_ITEM("endorium_chestplate", Item::endoriumChestplate_Id, 0);
    ADD_ITEM("endorium_leggings", Item::endoriumLeggings_Id, 0);
    ADD_ITEM("endorium_boots", Item::endoriumBoots_Id, 0);
    ADD_ITEM("endorium_pendant", Item::endoriumPendant_Id, 0);
    ADD_ITEM("endorium_ring", Item::endoriumRing_Id, 0);
    ADD_ITEM("shell_fossil", Item::shellFossil_Id, 0);
    ADD_ITEM("fire_fossil", Item::fireFossil_Id, 0);
    ADD_ITEM("air_fossil", Item::airFossil_Id, 0);
    ADD_ITEM("rib_fossil", Item::ribFossil_Id, 0);
    ADD_ITEM("nusa_shard", Item::nusaShard_Id, 0);
    ADD_ITEM("nusa_amulet", Item::nusaAmulet_Id, 0);
    ADD_ITEM("nusa_blade", Item::nusaBlade_Id, 0);
    ADD_ITEM("gold_dart_shooter", Item::dartShooterGold_Id, 0);
    ADD_ITEM("poison_dart_shooter", Item::dartShooterPoison_Id, 0);
    ADD_ITEM("enchanted_dart_shooter", Item::dartShooterEnchanted_Id, 0);
    ADD_ITEM("nethanium_dart_shooter", Item::dartShooterNethanium_Id, 0);
    ADD_ITEM("gold_dart", Item::dartGold_Id, 0);
    ADD_ITEM("poison_dart", Item::dartPoison_Id, 0);
    ADD_ITEM("enchanted_dart", Item::dartEnchanted_Id, 0);
    ADD_ITEM("nethanium_dart", Item::dartNethanium_Id, 0);
    
    #undef ADD_BLOCK
    #undef ADD_ITEM

    buildReverseMap();
}