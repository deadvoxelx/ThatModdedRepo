function main()
	--basic items
	local speedyBootsId = registerItem
	(
		"speedy_boots", "Hermes Boots", "res/speedy_boots.png", 
		ItemDefinition.new
		(
			{
				base = EBaseItem.Default,
				material = EItemMaterial.Iron,
				isAccessory = true,
				maxStackSize = 1
			}
		)
	)

	registerShapedRecipe
	(
		"speedyBootsRecipe",
		{
			"F F",
			"IDI",
			"I I"
		}, 
		{ D = "minecraft:diamond", I = "minecraft:iron_ingot", F = "minecraft:feather" },
		"hermes_boots:speedy_boots",
		{ count = 1, group = ERecipeGroup.Decoration }
	)
end
