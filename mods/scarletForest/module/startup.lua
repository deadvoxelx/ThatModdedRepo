function main()
    local registered = registerBiome
    (
        "scarlet_forest",
        {
            name = "Scarlet Forest",
            surface = "minecraft:grass_block",
            filler = "minecraft:dirt",
            depth = 0.1,
            scale = 0.35,
            temperature = 0.7,
            downfall = 0.8,
            snow = false,
            rain = true,
            grassColor = 0xb01a00,
            foliageColor = 0x560319,
            waterColor = 0x4040ff,
            skyColor = 0xfc696D,
			size = 256,			-- higher values mean bigger biomes, lower values mean smaller biomes
			frequency = 12,     -- 0 - 100, 0 being nonexistent, 100 being the whole world
			grassCount = 5,
			flowerCount = 2
        }
    )

    if registered then
        log("Scarlet Forest: registered scarletForest:scarlet_forest")
    else
        log("Scarlet Forest: biome registration failed, check ruby.log")
    end
end
