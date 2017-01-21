
Player = 
{
	name = "Bullet",
	class = "Weapon",
	controller = "BulletController",
	initial_position = { 0.0, 0.0, 0.0 },
	initial_velocity = { 2.0, 0.0, 0.0 },
	bounding_box = { 
			offset	= { 0.0, 0.0, 0.0 },
			size	= { 30.0, 30.0, 20.0 }
	},

	physics_settings = 
	{
		mass = 1.0,
		drag = 0.001
	},

	render_settings = 
	{
		sprite = {
			path	= "Resources\\Sprites\\BulletR.dds",
			offset	= { 0.0, 0.0 },
					-- left, top, right, bottom
			extents	= { -1.0, -1.0, 1.0, 1.0 }
		},
		sprite_color = { 255, 255, 255, 255 }
	}
}