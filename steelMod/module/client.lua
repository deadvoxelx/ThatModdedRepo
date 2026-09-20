function main()
    -- Custom tool tiers
    local steelTier = registerItemTier
        (
            "steel",
            {
                level = 2,  -- Same as iron
                uses = 768, -- Durability
                speed = 7,  -- Dig speed
                damage = 2, -- Sword damage is 4 + damage, for Iron Swords this is 2, resulting in 6 damage
                ench = 12   -- Enchantability
            }
        )

    -- Custom armor tiers
    local steelArmorMaterial = registerArmorMaterial
        (
            "steel",
            {
                durabilityMultiplier = 22, -- Durability multiplier/protection level; its complicated
                -- For iron, its 15, for diamond, its 33; base off that logic
                head = 2,                  -- Helmet armor points
                torso = 6,                 -- Chestplate armor points
                legs = 5,                  -- Leggings armor points
                feet = 2,                  -- Boots armor points
                ench = 12                  -- Enchantability
            }
        )

    -- Blocks
    local steelBlockId = registerBlock
        ( -- ID --		-- Name --		-- Texture --
            "steelBlock", "Steel Block", "res/steel_block.png",
            BlockDefinition.new
            (
                {
                    hardness = 5.0,
                    resistance = 15.0,
                    tool = EBlockTool.Pickaxe,
                    sound = "metal"
                }
            )
        )
    log("Steel Mod: registered steelBlock as id " .. tostring(steelBlockId))

    local steelOreId = registerBlock
        (
            "steelOre", "Carbonated Iron Ore", "res/carbon_iron_ore.png",
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
    log("Steel Mod: registered steelOre as id " .. tostring(steelOreId))

    local steelLogId = registerBlock
        (
            "steelLog", "Steel Log", "res/steel_log.png",
            BlockDefinition.new
            (
                {
                    hardness = 2.0,
                    resistance = 5.0,
                    tool = EBlockTool.Hatchet,
                    sound = "wood",
                    topTexture = "res/steel_log_top.png",   -- Top texture
                    bottomTexture = "res/steel_log_top.png" -- Bottom texture
                }
            )
        )
    log("Steel Mod: registered steelLog as id " .. tostring(steelLogId))

    local steelLeavesId = registerBlock
        (
            "steelLeaves", "Steel Leaves", "res/steel_leaves.png",
            BlockDefinition.new
            (
                {
                    transparent = true, -- Transparency; if missing, defaults to false
                    hardness = 0.5,
                    resistance = 0.0,
                    tool = EBlockTool.Hoe,
                    sound = "grass",
                    drop = "steelMod:steelSapling", -- Drop
                    --dropNothing = true,  						-- Use this to make blocks drop nothing
                    dropMin = 0,
                    dropMax = 1,     -- Min + max
                    dropChance = 0.2 -- Chance
                }
            )
        )
    log("Steel Mod: registered steelLeaves as id " .. tostring(steelLeavesId))

    local steelSaplingId = registerSapling
        (
            "steelSapling", "Steel Sapling", "res/steel_sapling.png",
            {
                shape = "oak",                   -- Tree shape (oak, spruce, birch, hugeJungle, swamp)
                trunk = "steelMod:steelLog",     -- Log
                leaves = "steelMod:steelLeaves", -- Leaves
                height = 6                       -- Base height
            }
        )
    log("Steel Mod: registered steelSapling as id " .. tostring(steelSaplingId))

    local steelPlanksId = registerBlock
        (
            "steelPlanks", "Steel Planks", "res/steel_planks.png",
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
    log("Steel Mod: registered steelPlanks as id " .. tostring(steelPlanksId))

    -- Basic Items
    local steelIngotId = registerItem
        (
            "steelIngot", "Steel Ingot", "res/steel_ingot.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Ingot,       -- Defines the crafting category basically
                    material = EItemMaterial.Iron -- Ill explain this on the wiki
                }
            )
        )
    log("Steel Mod: registered steelIngot as id " .. tostring(steelIngotId))

    -- Foods
    -- saturationMod guideline: 0.1 poor, 0.3 low, 0.6 normal, 0.8 good, 1.0 max, 1.2 supernatural
    local steelAppleId = registerItem
        (
            "steelApple", "Steel Apple", "res/steel_apple.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Food_Fruit,
                    nutrition = 4,
                    saturationMod = 0.6,
                    isMeat = false,
                    canAlwaysEat = true
                    -- The food effects are handled in server.lua
                }
            )
        )
    log("Steel Mod: registered steelApple as id " .. tostring(steelAppleId))

    -- Tools
    local steelSwordId = registerItem
        (
            "steelSword", "Steel Sword", "res/steel_sword.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Weapon,
                    --tier = EItemTier.Iron						-- Use this for a vanilla tier
                    customTier = steelTier, -- This is for custom tiers
                    material = EItemMaterial.Iron
                }
            )
        )
    log("Steel Mod: registered steelSword as id " .. tostring(steelSwordId))

    local steelAxeId = registerItem
        (
            "steelAxe", "Steel Axe", "res/steel_axe.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Hatchet,
                    customTier = steelTier,
                    material = EItemMaterial.Iron
                }
            )
        )
    log("Steel Mod: registered steelAxe as id " .. tostring(steelAxeId))

    local steelPickaxeId = registerItem
        (
            "steelPickaxe", "Steel Pickaxe", "res/steel_pickaxe.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Pickaxe,
                    customTier = steelTier,
                    material = EItemMaterial.Iron
                }
            )
        )
    log("Steel Mod: registered steelPickaxe as id " .. tostring(steelPickaxeId))

    local steelShovelId = registerItem
        (
            "steelShovel", "Steel Shovel", "res/steel_shovel.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Shovel,
                    customTier = steelTier,
                    material = EItemMaterial.Iron
                }
            )
        )
    log("Steel Mod: registered steelShovel as id " .. tostring(steelShovelId))

    local steelHoeId = registerItem
        (
            "steelHoe", "Steel Hoe", "res/steel_hoe.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Hoe,
                    customTier = steelTier,
                    material = EItemMaterial.Iron
                }
            )
        )
    log("Steel Mod: registered steelHoe as id " .. tostring(steelHoeId))

    -- Armor
    local steelHelmetId = registerItem
        (
            "steelHelmet", "Steel Helmet", "res/steel_armor_helmet.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Helmet,
                    armorSet = "steel",
                    --armorMaterial = EArmorMaterial.Iron, 				-- Use this for vanilla armor tiers
                    customArmorMaterial = steelArmorMaterial -- This is for custom armor tiers
                }
            )
        )
    log("Steel Mod: registered steelHelmet as id " .. tostring(steelHelmetId))

    local steelChestplateId = registerItem
        (
            "steelChestplate", "Steel Chestplate", "res/steel_armor_chestplate.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Chestplate,
                    armorSet = "steel",
                    customArmorMaterial = steelArmorMaterial
                }
            )
        )
    log("Steel Mod: registered steelChestplate as id " .. tostring(steelChestplateId))

    local steelLeggingsId = registerItem
        (
            "steelLeggings", "Steel Leggings", "res/steel_armor_leggings.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Leggings,
                    armorSet = "steel",
                    customArmorMaterial = steelArmorMaterial
                }
            )
        )
    log("Steel Mod: registered steelLeggings as id " .. tostring(steelLeggingsId))

    local steelBootsId = registerItem
        (
            "steelBoots", "Steel Boots", "res/steel_armor_boots.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Boots,
                    armorSet = "steel",
                    customArmorMaterial = steelArmorMaterial
                }
            )
        )
    log("Steel Mod: registered steelBoots as id " .. tostring(steelBootsId))

    -- Crafting recipes
    registerShapelessRecipe
    (
        "steelIngotRecipe",                            -- Recipe ID
        { "steelMod:steelBlock" },                     -- Ingredient
        "steelMod:steelIngot",                         -- Result
        { count = 9, group = ERecipeGroup.Decoration } -- Count and group
    )

    registerShapelessRecipe
    (
        "steelPlanksRecipe",
        { "steelMod:steelLog" },
        "steelMod:steelPlanks",
        { count = 4, group = ERecipeGroup.Decoration }
    )

    registerShapedRecipe
    (
        "steelBlockRecipe", -- Recipe ID
        {                   -- Recipe pattern
            "SSS",
            "SSS",
            "SSS"
        },
        { S = "steelMod:steelIngot" },                -- Ingredient
        "steelMod:steelBlock",                        -- Result
        { count = 1, group = ERecipeGroup.Structure } -- Count and group
    )

    registerShapedRecipe
    (
        "steelAppleRecipe",
        {
            "SSS",
            "SAS",
            "SSS"
        },
        { S = "steelMod:steelIngot", A = "minecraft:apple" },
        "steelMod:steelApple",
        { count = 1, group = ERecipeGroup.Food }
    )

    registerShapedRecipe
    (
        "steelSwordRecipe",
        {
            " S ",
            " S ",
            " I "
        },
        { S = "steelMod:steelIngot", I = "minecraft:stick" },
        "steelMod:steelSword",
        { count = 1, group = ERecipeGroup.Tool }
    )

    registerShapedRecipe
    (
        "steelAxeRecipe",
        {
            "SS ",
            "SI ",
            " I "
        },
        { S = "steelMod:steelIngot", I = "minecraft:stick" },
        "steelMod:steelAxe",
        { count = 1, group = ERecipeGroup.Tool }
    )

    registerShapedRecipe
    (
        "steelPickaxeRecipe",
        {
            "SSS",
            " I ",
            " I "
        },
        { S = "steelMod:steelIngot", I = "minecraft:stick" },
        "steelMod:steelPickaxe",
        { count = 1, group = ERecipeGroup.Tool }
    )

    registerShapedRecipe
    (
        "steelShovelRecipe",
        {
            " S ",
            " I ",
            " I "
        },
        { S = "steelMod:steelIngot", I = "minecraft:stick" },
        "steelMod:steelShovel",
        { count = 1, group = ERecipeGroup.Tool }
    )

    registerShapedRecipe
    (
        "steelHoeRecipe",
        {
            "SS ",
            " I ",
            " I "
        },
        { S = "steelMod:steelIngot", I = "minecraft:stick" },
        "steelMod:steelHoe",
        { count = 1, group = ERecipeGroup.Tool }
    )

    registerShapedRecipe
    (
        "steelHelmetRecipe",
        {
            "SSS",
            "S S"
        },
        { S = "steelMod:steelIngot" },
        "steelMod:steelHelmet",
        { count = 1, group = ERecipeGroup.Armour }
    )

    registerShapedRecipe
    (
        "steelChestplateRecipe",
        {
            "S S",
            "SSS",
            "SSS"
        },
        { S = "steelMod:steelIngot" },
        "steelMod:steelChestplate",
        { count = 1, group = ERecipeGroup.Armour }
    )

    registerShapedRecipe
    (
        "steelLeggingsRecipe",
        {
            "SSS",
            "S S",
            "S S"
        },
        { S = "steelMod:steelIngot" },
        "steelMod:steelLeggings",
        { count = 1, group = ERecipeGroup.Armour }
    )

    registerShapedRecipe
    (
        "steelBootsRecipe",
        {
            "S S",
            "S S"
        },
        { S = "steelMod:steelIngot" },
        "steelMod:steelBoots",
        { count = 1, group = ERecipeGroup.Armour }
    )

    -- Smelting recipe
    registerSmeltingRecipe
    (
        "steelIngotSmelt",      -- Recipe ID
        "steelMod:steelOre",    -- Input
        "steelMod:steelIngot",  -- Output
        { count = 1, xp = 0.7 } -- Count and xp
    )

    log("Steel Mod: registered all recipes")
    log.info("Steel Mod: client info message")
    log.warn("Steel Mod: client warning message")
    log.error("Steel Mod: client error message")
    local clientTicker = scheduleEvery(50, function()
        log("Steel Mod: client scheduler fired (yay)")
    end)
    schedule(250, function()
        cancelTask(clientTicker)
        log("Steel Mod: cancelled client scheduler")
    end)

    local pingSent = false
    registerEvent("ClientTickEvent", function(event)
        if not pingSent and event.tickCount % 100 == 50 then
            pingSent = net.send("steelChat", "ping from client at tick " .. event.tickCount)
            if pingSent then
                log.info("Steel Mod: client tick " .. event.tickCount .. ", sent server ping")
            end
        end
    end)

    local boots = mod:getData("clientBootCount") or 0
    mod:setData("clientBootCount", boots + 1)
    log("Steel Mod: client has booted " .. (mod:getData("clientBootCount") or 0) .. " time(s)")
    function clientHello()
        return "hello from steelMod client (v" .. mod.version .. ")"
    end

    log("Steel Mod: callMod -> " .. tostring(callMod("steelMod", "clientHello")))
    log("Steel Mod: mods table -> " .. tostring(mods.steelMod:call("clientHello")))

    net.listen("steelChat", function(event)
        log("Steel Mod: client got '" .. event.data .. "' on channel '" .. event.channel .. "'")
    end)
end
