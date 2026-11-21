function main()
    local registered = registerBiome
    (
        "haunted_forest",
        {
            name = "Haunted Forest",
            surface = "minecraft:grass_block",
            filler = "minecraft:dirt",
            depth = 0.1,
            scale = 1.2,
            temperature = 1.2,
            downfall = 1.2,
            snow = false,
            rain = true,
            grassColor = 0x6e6e6e,
            foliageColor = 0x404040,
            waterColor = 0x4040ff,
            skyColor = 0xd1b6b6,
			size = 128,
			frequency = 8,
			grassCount = 5,
			flowerCount = 0,
			deadBushCount = 8,
			mushroomCount = 8,
			cactusCount = 0,
			reedsCount = 0,
			waterlilyCount = 0,
			sandCount = 3,
			gravelCount = 1,
			clayCount = 1,
			mobSpawns = {
				{ mob = "zombie",   weight = 10, min = 4, max = 4 },
				{ mob = "skeleton", weight = 10, min = 4, max = 4 },
				{ mob = "cave_spider", weight = 10, min = 4, max = 4 },
				{ mob = "creeper", weight = 10, min = 4, max = 4 },
				{ mob = "enderman", weight = 2,  min = 1, max = 4 },
				{ mob = "witch", weight = 2,  min = 1, max = 2 },
				--{ mob = "sheep", category = "creature", weight = 12, min = 4, max = 4 },
				{ mob = "squid", category = "water",    weight = 10, min = 4, max = 4 },
				{ mob = "bat",   category = "ambient",  weight = 10, min = 8, max = 8 }
			}
        }
    )

    if registered then
        log("Haunted Forest: registered hauntedForest:haunted_forest")
    else
        log("Haunted Forest: biome registration failed, check ruby.log")
    end
	
	local hauntedSaplingId = registerSapling
        (
            "hauntedSapling", "Haunted Sapling", "res/hauntedSapling.png",
            {
                feature = "hauntedTree"
            }
        )
    log("Haunted Forest: registered hauntedSapling as id " .. tostring(hauntedSaplingId))
	
	local hauntedLeavesId = registerBlock
        (
            "hauntedLeaves", "Haunted Leaves", "res/hauntedLeaves.png",
            BlockDefinition.new
            (
                {
                    transparent = true,
					lightEmission = 0,			-- 0 - 15; 0 being no light, 15 being max light
					lightBlocking = 1,			-- 0 - 15; 0 being blocks no light, 15 being blocks all light
                    hardness = 0.2,
                    resistance = 0.0,
                    tool = EBlockTool.Hoe,
                    sound = "grass",
					drop = "hauntedForest:hauntedSapling",
                    dropMin = 0,
                    dropMax = 1,
                    dropChance = 0.2
                }
            )
        )
    log("Haunted Forest: registered hauntedLeaves as id " .. tostring(hauntedLeavesId))

    local hauntedLogId = registerBlock
        (
            "hauntedLog", "Haunted Log", "res/hauntedLog.png",
            BlockDefinition.new
            (
                {
					lightEmission = 0,
					lightBlocking = 15,
                    hardness = 2.0,
                    resistance = 5.0,
                    tool = EBlockTool.Hatchet,
                    sound = "wood",
					topTexture = "res/hauntedLogTop.png",
                    bottomTexture = "res/hauntedLogTop.png"
                }
            )
        )
    log("Haunted Forest: registered hauntedLog as id " .. tostring(hauntedLogId))
	
	local hauntedPlanksId = registerBlock
        (
            "hauntedPlanks", "Haunted Planks", "res/hauntedPlanks.png",
            BlockDefinition.new
            (
                {
					lightEmission = 0,
					lightBlocking = 15,
                    hardness = 2.0,
                    resistance = 5.0,
                    tool = EBlockTool.Hatchet,
                    sound = "wood",
					baseItem = "planks"
                }
            )
        )
    log("Haunted Forest: registered hauntedPlanks as id " .. tostring(hauntedPlanksId))
	
	local hauntedSlabId = registerBlock
		(
			"hauntedSlab", "Haunted Slab", "res/hauntedPlanks.png",
    		BlockDefinition.new
			(
				{
        			shape = "slab",
					lightEmission = 0,
					lightBlocking = 15,
					hardness = 2.0,
					resistance = 5.0,
        			tool = EBlockTool.Hatchet,
					sound = "wood"
    			}
			)
		)
	log("Haunted Forest: registered hauntedSlab as id " .. tostring(hauntedSlabId))
	
	local hauntedStairsId = registerBlock
		(
			"hauntedStairs", "Haunted Stairs", "res/hauntedPlanks.png",
    		BlockDefinition.new
			(
				{
        			shape = "stairs",
					lightEmission = 0,
					lightBlocking = 15,
					hardness = 2.0,
					resistance = 5.0,
        			tool = EBlockTool.Hatchet,
					sound = "wood"
    			}
			)
		)
	log("Haunted Forest: registered hauntedStairs as id " .. tostring(hauntedStairsId))
	
	local hauntedFenceId = registerBlock
		(
			"hauntedFence", "Haunted Fence", "res/hauntedPlanks.png",
    		BlockDefinition.new
			(
				{
        			shape = "fence",
					lightEmission = 0,
					lightBlocking = 0,
					hardness = 2.0,
					resistance = 5.0,
        			tool = EBlockTool.Hatchet,
					sound = "wood"
    			}
			)
		)
	log("Haunted Forest: registered hauntedFence as id " .. tostring(hauntedFenceId))
	
	local hauntedFenceGateId = registerBlock
		(
			"hauntedFenceGate", "Haunted Fence Gate", "res/hauntedPlanks.png",
    		BlockDefinition.new
			(
				{
        			shape = "fenceGate",
					lightEmission = 0,
					lightBlocking = 0,
					hardness = 2.0,
					resistance = 5.0,
        			tool = EBlockTool.Hatchet,
					sound = "wood"
    			}
			)
		)
	log("Haunted Forest: registered hauntedFenceGate as id " .. tostring(hauntedFenceGateId))
	
	registerShapelessRecipe
	(
		"hauntedPlanksRecipe",
		{ "hauntedForest:hauntedLog" },
		"hauntedForest:hauntedPlanks",
		{ count = 4, group = ERecipeGroup.Structure }
	)
	
	registerShapedRecipe
    (
        "craftingTableFromHauntedPlanks",
        {
            "SS",
            "SS"
        },
        { S = "hauntedForest:hauntedPlanks" },
        "minecraft:crafting_table",
        { count = 1, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "chestFromHauntedPlanks",
        {
            "SSS",
            "S S",
			"SSS"
        },
        { S = "hauntedForest:hauntedPlanks" },
        "minecraft:chest",
        { count = 1, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "sticksFromHauntedPlanks",
        {
            "S",
            "S"
        },
        { S = "hauntedForest:hauntedPlanks" },
        "minecraft:stick",
        { count = 8, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "hauntedSlabRecipe",
        {
            "SSS"
        },
        { S = "hauntedForest:hauntedPlanks" },
        "hauntedForest:hauntedSlab",
        { count = 6, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "hauntedStairsRecipe",
        {
            "  S",
			" SS",
			"SSS"
        },
        { S = "hauntedForest:hauntedPlanks" },
        "hauntedForest:hauntedStairs",
        { count = 6, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "hauntedFenceRecipe",
        {
            "STS",
			"STS"
        },
        { S = "hauntedForest:hauntedPlanks", T = "minecraft:stick" },
        "hauntedForest:hauntedFence",
        { count = 4, group = ERecipeGroup.Structure }
    )
	
	registerShapedRecipe
    (
        "hauntedFenceGateRecipe",
        {
            "TST",
			"TST"
        },
        { S = "hauntedForest:hauntedPlanks", T = "minecraft:stick" },
        "hauntedForest:hauntedFenceGate",
        { count = 1, group = ERecipeGroup.Structure }
    )
	
    registerSmeltingRecipe
    (
        "hauntedLogSmelt",
        "hauntedForest:hauntedLog",
        "minecraft:charcoal",
        { count = 1, xp = 0.3 }
    )
end
