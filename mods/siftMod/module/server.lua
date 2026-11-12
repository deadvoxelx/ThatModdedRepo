function main()

    local siftTree = registerFeature
    (
        "siftTree",
        {
            biomes = { "siftMod:siftlands" },			-- The biomes to generate it in
            dimensions = { "siftMod:sift" },			-- Extra dimensions param "just in case"
            count = 2,									-- Average count per chunk
            chance = 0.4,								-- Placement chance
            placement =
            {
                mode = "surface",						-- surface, range, fixed
				--min = 0,								-- Use both of these for range, only min is required for fixed
				--max = 0,
                ground = { "siftMod:siftGrass", "siftMod:siftSoil" }	-- Valid placement blocks
            },
            place = function(ctx)
                local logTile = ctx:block("siftMod:siftLog")
                local leafTile = ctx:block("siftMod:siftLeaves")
                local soilTile = ctx:block("siftMod:siftSoil")
                if logTile <= 0 or leafTile <= 0 or soilTile <= 0 then return false end

                local height = ctx:randomInt(7, 12)

                for dy = 1, height - 1 do
                    for dx = 0, 1 do
                        for dz = 0, 1 do
                            if not ctx:isAir(dx, dy, dz) then return false end
                        end
                    end
                end

                for dy = 0, height - 1 do
                    for dx = 0, 1 do
                        for dz = 0, 1 do
                            ctx:set(dx, dy, dz, logTile)
                        end
                    end
                end

                for dy = -1, -1 do
                    for dx = 0, 1 do
                        for dz = 0, 1 do
                            ctx:set(dx, dy, dz, soilTile)
                        end
                    end
                end

                local top = height - 1
                for dy = 1, 2 do
                    for dx = -2, 3 do
                        for dz = -2, 3 do
                            ctx:replace(dx, top + dy, dz, leafTile)
                        end
                    end
                end

                local function wall(cellX, cellZ, alongX)
                    local depth = 3 + ctx:randomInt(0, 1)
                    for i = -2, 3 do
                        for d = 0, depth - 1 do
                            if alongX then ctx:replace(i, top - d, cellZ, leafTile)
                            else ctx:replace(cellX, top - d, i, leafTile) end
                        end
                        if ctx:randomFloat() < 0.3 then
                            if alongX then ctx:replace(i, top - depth, cellZ, leafTile)
                            else ctx:replace(cellX, top - depth, i, leafTile) end
                        end
                    end
                end

                wall(0, 4, true)
                wall(0, -3, true)
                wall(4, 0, false)
                wall(-3, 0, false)
            end
        }
    )

    if log then
        if siftTree then
            log("Sift Mod: registered the siftTree feature")
        else
            log("Sift Mod: siftTree feature registration failed, check ruby.log")
        end
    end

    
	local siftSpike = registerFeature
    (
        "siftSpike",
        {
            biomes = { "siftMod:siftlands" },
            dimensions = { "siftMod:sift" },
            count = 1,
            chance = 0.1,
            placement =
            {
                mode = "surface",
                ground = { "siftMod:siftGrass", "siftMod:siftSoil" }
            },
            place = function(ctx)
                local soilTile = ctx:block("siftMod:siftSoil")
                local grassTile = ctx:block("siftMod:siftGrass")
                if soilTile <= 0 or grassTile <= 0 then return false end

                local base = ctx:randomInt(5, 9)

                local function isCut(dx, dz)
                    return ((dx * 37 + dz * 91) % 100) < 35
                end
                local height = ctx:randomInt(24, 40)
                local radius = base
                local ledgeLeft = ctx:randomInt(2, 4)
                for dy = 0, height - 1 do
                    if ledgeLeft <= 0 then
                        local roll = ctx:randomFloat()
                        local step = 0
                        if roll < 0.35 then step = 2
                        elseif roll < 0.75 then step = -1 end
                        radius = radius + step

                        if radius > base + 1 then radius = base + 1 end
                        if radius < base - 1 then radius = base - 1 end
                        ledgeLeft = ctx:randomInt(2, 4)
                    end

                    local rx = math.floor(radius + 0.5)
                    local rz = rx - 1
                    if rz < 2 then rz = 2 end

                    for dx = -rx, rx do
                        for dz = -rz, rz do
                            local rim = (dx * dx) / (rx * rx) + (dz * dz) / (rz * rz)
                            if rim <= 1.0 and not (rim > 0.62 and isCut(dx, dz)) then
                                ctx:set(dx, dy, dz, soilTile)
                            end
                        end
                    end

                    ledgeLeft = ledgeLeft - 1
                end

                local px = base + ctx:randomInt(2, 3)
                local pz = math.max(base + 1, px - ctx:randomInt(1, 2))

                for layer = 0, 2 do
                    local scale = (layer == 0) and 0.7 or ((layer == 1) and 1.0 or 0.9)
                    local rx = math.max(2, math.floor(px * scale + 0.5))
                    local rz = math.max(2, math.floor(pz * scale + 0.5))

                    if layer == 0 then
                        rx = math.max(rx, base + 2)
                        rz = math.max(rz, base + 1)
                    end

                    for dx = -rx, rx do
                        for dz = -rz, rz do
                            local rim = (dx * dx) / (rx * rx) + (dz * dz) / (rz * rz)
                            if rim <= 1.0 then
                                if layer == 2 then ctx:set(dx, height + 2, dz, grassTile)
                                else ctx:set(dx, height + layer, dz, soilTile) end
                            end
                        end
                    end
                end
            end
        }
    )

    if log then
        if siftSpike then
            log("Sift Mod: registered the siftSpike feature")
        else
            log("Sift Mod: siftSpike feature registration failed, check ruby.log")
        end
    end


    local siftPortalSite = registerFeature
    (
        "siftPortalFeature",
        {
            dimensions = { "minecraft:overworld" },
            count = 1,
            chance = 0.1,
            placement =
            {
                mode = "surface",
                ground = { "minecraft:grass_block", "minecraft:dirt", "minecraft:stone", "minecraft:sand", "minecraft:sandstone", "minecraft:gravel", "minecraft:snow" }
            },
            place = function(ctx)
                local frameTile = ctx:block("siftMod:reinforcedDeepslate")
                local waterTile = ctx:block("minecraft:water")
                local lavaTile = ctx:block("minecraft:lava")
                if frameTile <= 0 then return false end

                local alongX = ctx:randomInt(0, 1) == 0
                local function tileAt(u, v, w)
                    if alongX then return ctx:get(u, v, w) end
                    return ctx:get(w, v, u)
                end
                local function isClear(u, v, w)
                    if alongX then return ctx:isAir(u, v, w) end
                    return ctx:isAir(w, v, u)
                end
                local function put(u, v, w, tile)
                    if alongX then ctx:set(u, v, w, tile)
                    else ctx:set(w, v, u, tile) end
                end

                for u = -1, 2 do
                    local under = tileAt(u, -1, 0)
                    if under == 0 or under == waterTile or under == lavaTile then return false end

                    for v = 0, 3 do
                        if not isClear(u, v, 0) then return false end
                    end
                end

                for u = 0, 1 do
                    for v = 0, 2 do
                        if not isClear(u, v, -1) or not isClear(u, v, 1) then return false end
                    end
                end

                for u = -1, 2 do
                    for v = -1, 3 do
                        if u == -1 or u == 2 or v == -1 or v == 3 then
                            put(u, v, 0, frameTile)
                        end
                    end
                end
            end
        }
    )

    if log then
        if siftPortalSite then
            log("Sift Mod: registered the siftPortalFeature feature")
        else
            log("Sift Mod: siftPortalFeature feature registration failed, check ruby.log")
        end
    end
end
