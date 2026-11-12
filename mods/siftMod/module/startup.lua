function main()

    local siftStoneId = registerBlock
        (
            "siftStone", "Soulshale", "res/siftStone.png",
            BlockDefinition.new
            (
                {
                    hardness = 3.0,
                    resistance = 10.0,
                    tool = EBlockTool.Pickaxe,
                    sound = "stone"
                }
            )
        )
    log("Sift Mod: registered siftStone as id " .. tostring(siftStoneId))

    local siftSoilId = registerBlock
        (
            "siftSoil", "Sift Soil", "res/siftSoil.png",
            BlockDefinition.new
            (
                {
                    hardness = 1.0,
                    resistance = 0.0,
                    tool = EBlockTool.Shovel,
                    sound = "gravel"
                }
            )
        )
    log("Sift Mod: registered siftSoil as id " .. tostring(siftSoilId))

    local siftGrassId = registerBlock
        (
            "siftGrass", "Sift Grass", "res/siftGrassSide.png",
            BlockDefinition.new
            (
                {
                    hardness = 1.0,
                    resistance = 0.0,
                    tool = EBlockTool.Shovel,
                    sound = "grass",
                    topTexture = "res/siftGrassTop.png",
                    bottomTexture = "res/siftSoil.png"
                }
            )
        )
    log("Sift Mod: registered siftGrass as id " .. tostring(siftGrassId))

    local siftLeavesId = registerBlock
        (
            "siftLeaves", "Wellspring Leaves", "res/siftLeaves.png",
            BlockDefinition.new
            (
                {
                    hardness = 0.5,
                    resistance = 0.0,
                    tool = EBlockTool.Hoe,
                    sound = "grass"
                }
            )
        )
    log("Sift Mod: registered siftLeaves as id " .. tostring(siftLeavesId))

    local siftLogId = registerBlock
        (
            "siftLog", "Wellspring Log", "res/siftLog.png",
            BlockDefinition.new
            (
                {
                    hardness = 2.0,
                    resistance = 5.0,
                    tool = EBlockTool.Hatchet,
                    sound = "wood"
                }
            )
        )
    log("Sift Mod: registered siftLog as id " .. tostring(siftLogId))
	
	local siftPlanksId = registerBlock
        (
            "siftPlanks", "Wellspring Planks", "res/siftPlanks.png",
            BlockDefinition.new
            (
                {
                    hardness = 2.0,
                    resistance = 5.0,
                    tool = EBlockTool.Hatchet,
                    sound = "wood",
					baseItem = "planks"		-- a recipe group override; currently only planks is valid
                }
            )
        )
    log("Sift Mod: registered siftPlanks as id " .. tostring(siftPlanksId))
	
	local reinforcedDeepslateId = registerBlock
        (
            "reinforcedDeepslate", "Reinforced Deepslate", "res/reinforcedDeepslateSide.png",
            BlockDefinition.new
            (
                {
                    hardness = 50.0,
                    resistance = 6000000.0,
                    tool = EBlockTool.Pickaxe,
                    sound = "stone",
                    topTexture = "res/reinforcedDeepslateTop.png",
                    bottomTexture = "res/reinforcedDeepslateTop.png"
                }
            )
        )
    log("Sift Mod: registered reinforcedDeepslate as id " .. tostring(reinforcedDeepslateId))
	
    -- Plant
    local siftTallgrassId = registerPlant
    (
        "siftTallgrass", "Sift Tallgrass", "res/siftTallGrass.png",
        {
            soil = { "siftMod:siftGrass", "siftMod:siftSoil" }		-- List of valid place-on blocks
        }
    )
    log("Sift Mod: registered siftTallgrass as id " .. tostring(siftTallgrassId))
	
	registerShapelessRecipe
	(
		"siftPlanksRecipe",
		{ "siftMod:siftLog" },
		"siftMod:siftPlanks",
		{ count = 4, group = ERecipeGroup.Structure }
	)
	
	registerShapedRecipe
    (
        "craftingTableFromSiftPlanks",
        {
            "SS",
            "SS"
        },
        { S = "siftMod:siftPlanks" },
        "minecraft:crafting_table",
        { count = 1, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "chestFromSiftPlanks",
        {
            "SSS",
            "S S",
			"SSS"
        },
        { S = "siftMod:siftPlanks" },
        "minecraft:chest",
        { count = 1, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "furnaceFromSiftStone",
        {
            "SSS",
            "S S",
			"SSS"
        },
        { S = "siftMod:siftStone" },
        "minecraft:furnace",
        { count = 1, group = ERecipeGroup.Structure }
    )


	-- Biome
	local registered = registerBiome
    (
        "siftlands",
        {
            name = "Siftlands",
            surface = "siftMod:siftGrass",
            filler = "siftMod:siftSoil",
            depth = 0.25,
            scale = 2.0,
            temperature = 0.2,
            downfall = 0.42,
            snow = false,
            rain = false,
            grassColor = 0x60d5c7,
            foliageColor = 0x60d5c7,
            waterColor = 0x4040ff,
            skyColor = 0x60d5c7,
			size = 256,
			frequency = 12,
			grassCount = 4,
			tallGrass = "siftMod:siftTallgrass",
			flowerCount = 0,
			isOverworldBiome = false	-- Set to false for custom dimension biomes; defaults to true
        }
    )

    if registered then
        log("Sift Mod: registered siftMod:siftlands")
    else
        log("Sift Mod: biome registration failed, check ruby.log")
    end

    -- Dimension
    local dimension = registerDimension
    (
        "sift",
        {
            slot = 5,							-- Dimension ID slot (-1 to 4 are built-in dimensions)
            xzSize = 54,						-- Size in chunks
            seaLevel = 0,
            biome = "siftMod:siftlands",		-- The biome to generate
            generator = "surface",				-- The generator; surface, underground, floatingIslands
            natural = false,					-- true = stars, sun/moon, etc
            mayRespawn = false,					-- false = beds go boom
            spawn = { 0, 72, 0 },				-- Default spawn point (every dimension has this, gets overridden by the portal essentially)
            terrain =
            {
                stone = "siftMod:siftStone",	-- Base block
                surface = "siftMod:siftGrass",	-- Grass layer
                filler = "siftMod:siftSoil",	-- Dirt layer
                baseHeight = 48,
                heightVariation = 24,
                relief = 1.0,					-- Terrain roughness; 0.5 = relatively flat for example
                soilDepth = 4,					-- Dirt layer height
                lakes = true
            }
        }
    )

    -- Portal Block
    local siftPortalBlockId = registerBlock
    (
        "siftPortalBlock", "Sift Portal", "res/siftPortal.png",
        BlockDefinition.new
        (
            {
                hardness = -1.0,
                resistance = 6000000.0,
                tool = EBlockTool.None,
                sound = "stone",
                transparent = true,
                dropNothing = true
            }
        )
    )
    log("Sift Mod: registered siftPortalBlock as id " .. tostring(siftPortalBlockId))
	
	-- Portal
    local portal = registerPortal
    (
        "siftPortal",
        {
            block = "siftMod:siftPortalBlock",		-- The block to be the portal
            frame = "siftMod:reinforcedDeepslate",	-- The frame block that fire opens
            from = "minecraft:overworld",			-- Works in all dimensions if absent
            to = "siftMod:sift",
            bidirectional = true,					-- true = traversal both ways, false = enter only
            cooldown = 100							-- Cooldown in ticks; 20 ticks per second
        }
    )

    if portal then
        log("Sift Mod: linked the overworld and siftMod:sift through siftMod:siftPortalBlock")
    else
        log("Sift Mod: portal registration failed, check ruby.log")
    end

    if dimension then
        log("Sift Mod: registered siftMod:sift for dimension slot 5")
    else
        log("Sift Mod: dimension registration failed, check ruby.log")
    end

end
