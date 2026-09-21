-- Server scripts run when a world is hosted; this is where event listeners are registered
-- Worldgen stuff is also done here
function main()
    local joins = 0
    registerEvent
    ( -- Fires on player join
        "PlayerJoinEvent", function(event)
            local player = event.player
            player:sendMessage("Steel Mod active")
            log("Steel Mod: " .. player.name .. " joined")

            joins = player:getData("joins") or 0
            player:setData("joins", joins + 1)
            player:sendMessage("Steel Mod: you have joined " .. player:getData("joins") .. " time(s)")

            runCommand(player, "steel", "welcome", player.name)
        end
    )

    log.info("Steel Mod: server info message")
    log.warn("Steel Mod: server warning message")
    log.error("Steel Mod: server error message")

    local steelTimer = scheduleEvery(100, function()
        log("Steel Mod: server scheduler fired")
    end)
    schedule(450, function()
        cancelTask(steelTimer)
        log("Steel Mod: cancelled server scheduler")
    end)

    registerEvent("ServerTickEvent", function(event)
        if event.tickCount % 1200 == 0 then
            log.info("Steel Mod: server tick " .. event.tickCount)
        end
    end)

    local boots = mod:getData("bootCount") or 0
    mod:setData("bootCount", boots + 1)
    log("Steel Mod: this save has hosted " .. (mod:getData("bootCount") or 0) .. " time(s)")
    log("Steel Mod: env version = " .. mod.version .. ", folder = " .. mod.folder)

    function serverHello()
        return "hello from steelMod server (v" .. mod.version .. ")"
    end

    log("Steel Mod: callMod -> " .. tostring(callMod("steelMod", "serverHello")))
    log("Steel Mod: mods table -> " .. tostring(mods.steelMod:call("serverHello")))
    net.listen("steelChat", function(event)
        local sender = event.player ~= nil and event.player.name or "?"
        log("Steel Mod: server received '" .. event.data .. "' from " .. sender)
        net.send(event.player, "steelChat", "echo: " .. event.data)
    end)

    registerCommand("steel", 0, function(source, ...)
        local who = source ~= nil and source.name or "console"
        local args = { ... }
        log("Steel Mod: /steel run by " .. who .. " with " .. #args .. " arg(s)")
    end)

    registerEvent
    ( -- Fires on block destroyed
        "PlayerBlockBreakEvent", function(event)
            log("Steel Mod: " .. event.block.oid .. " broken at " .. tostring(event.block.pos))
        end
    )

    registerEvent
    ( -- Fires on item use completion
        "ItemCompleteUseEvent", function(event)
            if event.itemId ~= "steelMod:steelApple" then return end
            event.player:addEffect(Effect.Resistance, 2400, 0)
            event.player:addEffect(Effect.Regeneration, 100, 2)
        end
    )

    -- Ore Features
    registerOreFeature
    (
        "steelOreFeature",              -- Feature ID
        "steelMod:steelOre",            -- Placed block
        {
            size = 11,                  -- Blocks per vein
            target = "minecraft:stone", -- Replaced block
            dimension = 0,              -- Dimension (-999 for all)
            yMin = 0,
            yMax = 64,                  -- Min + max y
            count = 8,                  -- Average veins per chunk
        }
    )

    -- Tree Features
    registerTreeFeature
    (
        "steelTreeFeature",                  -- Feature ID
        "oak",                               -- Tree shape (oak, spruce, birch, hugeJungle, swamp)
        {
            trunk = "steelMod:steelLog",     -- Log
            leaves = "steelMod:steelLeaves", -- Leaves
            biome = "minecraft:forest",      -- Biome
            count = 1,                       -- Average trees per chunk
            height = 7,                      -- Base height
        }
    )
end
