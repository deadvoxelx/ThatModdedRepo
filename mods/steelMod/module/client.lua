function main()

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
