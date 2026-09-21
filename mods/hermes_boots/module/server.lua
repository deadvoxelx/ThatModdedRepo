function main()
	--slot ids for event.slot:
	--helmet - 39
	--chestplate - 38
	--leggings - 37
	--boots - 36
	--aether top - 43
	--aether mid - 42
	--aether bottom - 41
	--offhand - 40 
	registerEvent(
		"ItemTickEvent", function(event)
			if event.player.inventory:isInAccessory(event.item) and not event.player:hasEffect(Effect.Speed) and getIdFromString("hermes_boots:speedy_boots") == event.item.id then
				event.player:addEffect(Effect.Speed, 100, 0)
			end
    	end
	)
end
