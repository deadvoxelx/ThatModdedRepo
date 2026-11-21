local ticks = 0

local function dynamiteOnHit(projectile, target, level)
    if target ~= nil then
        local living = target:castToLivingEntity()
        if living ~= nil then
            living:setOnFire(5)
        end
        log("Dynamite Mod: dynamite hit an entity")
    else
        log("Dynamite Mod: dynamite hit the world")
    end
end

local function poisonOnHit(projectile, target, level)
    if target ~= nil then
        local living = target:castToLivingEntity()
        if living ~= nil then
            living:addEffect(Effect.Poison, 200, 0)
        end
        log("Dynamite Mod: poison dynamite hit an entity")
    else
        log("Dynamite Mod: poison dynamite hit the world")
    end
end

local function thunderOnHit(projectile, target, level)
    local pos = projectile.pos
    if level ~= nil then
        level:strikeLightning(pos.x, pos.y, pos.z)
    end
    if target ~= nil then
        log("Dynamite Mod: thunder dynamite hit an entity")
    else
        log("Dynamite Mod: thunder dynamite hit the world")
    end
end

function main()

    local dynamiteId = registerItem
        (
            "dynamite", "Dynamite", "res/dynamiteRed.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Throwable,			-- This is what makes em throwable
					maxStackSize = 16,
					projectile =						-- The projectile stuff
					{
						damage = 1,
						explosionPower = 2.0,			-- 0 to disable explosions
						throwPower = 1.2,				-- Default is 1.5
						particle = "flame",				-- Hit particle; defaults to "snowballpoof"
						trail = "flame",				-- Flight particle; defaults to "snowballpoof"
-- particle names: bubble, smoke, note, explode, flame, lava, footstep, splash, largesmoke, reddust, snowballpoof, snowshovel, slime, heart, crit, hugeexplosion, largeexplode, townaura, spell, witchMagic, mobSpell, magicCrit, dripWater, dripLava, enchantmenttable, dragonbreath, ender, angryVillager, happyVillager, fireworksspark
						onHit = dynamiteOnHit			-- For executing a function upon impact
						--onTick = functionName			-- For executing a function during the projectile's flight
					},
					fireImmune = false,
					blastImmune = false
                }
            )
        )
    log("Dynamite Mod: registered dynamite as id " .. tostring(dynamiteId))
	
	local poisonDynamiteId = registerItem
        (
            "poisonDynamite", "Poison Dynamite", "res/dynamiteGreen.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Throwable,
					maxStackSize = 16,
					projectile =
					{
						damage = 1,
						explosionPower = 2.0,
						throwPower = 1.2,
						particle = "flame",
						trail = "flame",
						onHit = poisonOnHit
					},
					fireImmune = false,
					blastImmune = false
                }
            )
        )
    log("Dynamite Mod: registered poisonDynamite as id " .. tostring(poisonDynamiteId))
	
	local thunderDynamiteId = registerItem
        (
            "thunderDynamite", "Thunder Dynamite", "res/dynamiteBlue.png",
            ItemDefinition.new
            (
                {
                    base = EBaseItem.Throwable,
					maxStackSize = 16,
					projectile =
					{
						damage = 1,
						explosionPower = 2.0,
						throwPower = 1.2,
						particle = "flame",
						trail = "flame",
						onHit = thunderOnHit
					},
					fireImmune = false,
					blastImmune = false
                }
            )
        )
    log("Dynamite Mod: registered thunderDynamite as id " .. tostring(thunderDynamiteId))
	
	registerShapelessRecipe
    (
        "dynamiteRecipe",
        { "minecraft:tnt" },
        "dynamiteMod:dynamite",
        { count = 9, group = ERecipeGroup.Tool }
    )
	
	registerShapedRecipe
    (
        "poisonDynamiteRecipe",
        {
            "S",
            "D"
        },
        { S = "minecraft:spider_eye", D = "dynamiteMod:dynamite" },
        "dynamiteMod:poisonDynamite",
        { count = 1, group = ERecipeGroup.Tool }
    )
	
	registerShapedRecipe
    (
        "thunderDynamiteRecipe",
        {
            " S ",
            "SDS",
			" S "
        },
        { S = "minecraft:redstone", D = "dynamiteMod:dynamite" },
        "dynamiteMod:thunderDynamite",
        { count = 1, group = ERecipeGroup.Tool }
    )

end
