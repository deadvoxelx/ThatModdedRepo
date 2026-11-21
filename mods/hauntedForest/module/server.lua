function main()

    local hauntedTree = registerFeature
    (
        "hauntedTree",
        {
            biomes = { "hauntedForest:haunted_forest" },
            dimensions = { "minecraft:overworld" },
            count = 7,
            placement =
            {
                mode = "surface",
                ground = { "minecraft:grass_block", "minecraft:dirt" }
            },
            place = function(ctx)
                local logTile = ctx:block("hauntedForest:hauntedLog")
                local leafTile = ctx:block("hauntedForest:hauntedLeaves")
                local grassTile = ctx:block("minecraft:grass")
                local dirtTile = ctx:block("minecraft:dirt")
                if logTile <= 0 or leafTile <= 0 or dirtTile <= 0 then return false end

                local webTile = ctx:block("minecraft:web")
                local webChance = 0.05
                local leaves = {}
                local leafBlocks = {}

                for dy = 5, 11 do
                    for dx = -2, 2 do
                        for dz = -2, 2 do
                            local distance = dx * dx + (dy - 8) * (dy - 8) + dz * dz
                            if distance < 12 + ctx:randomInt(0, 6) then
                                ctx:set(dx, dy, dz, leafTile)
                                leaves[dx .. ":" .. dy .. ":" .. dz] = true
                                leafBlocks[#leafBlocks + 1] = { dx, dy, dz }
                            end
                        end
                    end
                end

                if webTile > 0 then
                    for _, leaf in ipairs(leafBlocks) do
                        local dx, dy, dz = leaf[1], leaf[2], leaf[3]
                        if not leaves[dx .. ":" .. (dy - 1) .. ":" .. dz]
                            and ctx:get(dx, dy - 1, dz) == 0
                            and ctx:randomFloat() < webChance then
                            ctx:set(dx, dy - 1, dz, webTile)
                        end
                    end
                end

                ctx:set(0, -1, 0, dirtTile)

                local height = 9 - ctx:randomInt(0, 2)
                for dy = 0, height - 1 do
                    local slant = math.floor(dy / 4) - 1
                    local dirX = ctx:randomInt(-1, 1)
                    local dirZ = ctx:randomInt(-1, 1)
                    local outX, outZ, backX, backZ = 0, 0, 0, 0
                    local branchY = dy
                    for _ = 1, ctx:randomInt(0, 1) do
                        outX, outZ = outX + dirX, outZ + dirZ
                        backX, backZ = backX - dirX, backZ - dirZ
                        branchY = branchY + slant
                        if ctx:get(outX, branchY, outZ) == leafTile then
                            ctx:set(outX, branchY, outZ, logTile)
                            ctx:set(backX, branchY, backZ, logTile)
                        end
                    end

                    local current = ctx:get(0, dy, 0)
                    if current == 0 or current == leafTile or (grassTile > 0 and current == grassTile) then
                        ctx:set(0, dy, 0, logTile)
                    end
                end

                local function exposedRoot(dx, dz)
                    if ctx:get(dx, 0, dz) ~= 0 then return end
                    if ctx:get(dx, -1, dz) == 0 then return end
                    ctx:set(dx, 0, dz, logTile)
                    ctx:replace(dx, -1, dz, logTile)
                end

                for dx = -1, 1 do
                    for dz = -1, 1 do
                        if dx ~= 0 or dz ~= 0 then
                            exposedRoot(dx, dz)
                            if (dx == 0 or dz == 0) and ctx:randomFloat() < 0.5 then
                                if ctx:get(dx, 1, dz) == 0 then ctx:set(dx, 1, dz, logTile) end
                            end
                        end
                    end
                end

                for _ = 1, ctx:randomInt(2, 4) do
                    local stepX = ctx:randomInt(0, 1) == 0 and -1 or 1
                    local stepZ = ctx:randomInt(0, 1) == 0 and -1 or 1
                    local length = ctx:randomInt(2, 3)
                    for step = 1, length do
                        local dx = stepX * step
                        local dz = stepZ * step
                        if step < length then
                            exposedRoot(dx, dz)
                        else
                            ctx:replace(dx, -1, dz, logTile)
                            if ctx:randomFloat() < 0.5 then ctx:replace(dx, -2, dz, logTile) end
                        end
                    end
                end

                return true
            end
        }
    )

    if hauntedTree then
        log("Haunted Forest: registered hauntedForest:hauntedTree")
    else
        log("Haunted Forest: hauntedTree feature registration failed, check ruby.log")
    end
end
