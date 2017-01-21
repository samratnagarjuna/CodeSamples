
Player = 
{
	name = "Reaper",
	class = "Monster",
	serializable = "true",
	controller = "MonsterController",
	initial_position = { 400.0, 0.0, 0.0 },
	initial_velocity = { 0.0, 0.0, 0.0 },
	
	bounding_box = { 
			offset	= { 0.0, 0.0, 0.0 },
			size	= { 96.0, 96.0, 20.0 }
	},

	physics_settings = 
	{
		mass = 1.0,
		drag = 0.01
	},

	render_settings = 
	{
		sprite = {
			path	= "Resources\\Sprites\\Player_04.dds",
			offset	= { 0.0, 0.0 },
					-- left, top, right, bottom
			extents	= { -1.0, -1.0, 1.0, 1.0 }
		},
		sprite_color = { 255, 255, 255, 255 }
	}
}