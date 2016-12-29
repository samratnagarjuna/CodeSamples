--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	-- The top-level table is a dictionary whose keys
	-- correspond to the asset types (inherited from cbAssetTypeInfo)
	-- that you have defined in AssetBuildSystem.lua

	meshes =
	{
		-- The actual assets can be defined simply as relative path strings (the common case)
		"Meshes/Floor.hlml",
		"Meshes/Player.hlml",
		"Meshes/Enemy.hlml",
		"Meshes/Ship.hlml",
	},
	materials = 
	{
		"Materials/standard.material",
		"Materials/oscillate.material",
		"Materials/floor.material",
		"Materials/ship.material",
		"Materials/copyright.material",
	},
	textures = 
	{
		"Images/default.tga",
	},
}
