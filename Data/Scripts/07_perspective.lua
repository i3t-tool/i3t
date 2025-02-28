-- I3T.load_script_from("Data/Scripts/07_perspective.lua")

local node_143 = Sequence.new()
node_143:set_render(true)
node_143:set_number_of_decimals(1)
node_143:set_lod(0)
node_143:set_position(Vec2.new(778.8001, 279.00006))
local node_149 = Transform.new("Translation")
node_149:set_render(true)
node_149:set_number_of_decimals(1)
node_149:set_lod(0)
node_149:set_position(Vec2.new(798.66064, 309.45743))
node_149:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0, 0, -3, 1)))
node_149:disable_synergies()
node_149:lock()
node_149:set_default_value("translation", Vec3.new(0, 0, -3))
node_143:push(node_149)
local node_150 = Camera.new()
node_150:set_render(true)
node_150:set_number_of_decimals(1)
node_150:set_lod(0)
node_150:set_position(Vec2.new(219.60002, 75.2))
local node_154 = node_150:get_projection()
local node_167 = Transform.new("Perspective")
node_167:set_render(true)
node_167:set_number_of_decimals(1)
node_167:set_lod(1)
node_167:set_position(Vec2.new(246.66063, 136.88599))
node_167:set_value(Mat4.new(Vec4.new(1.3763065, 0, 0, 0), Vec4.new(0, 1.8304877, 0, 0), Vec4.new(0, 0, -1.4, -1), Vec4.new(0, 0, -2.4, 0)))
node_167:disable_synergies()
node_167:lock()
node_167:set_default_value("fovy", 1.000000)
node_167:set_default_value("aspect", 1.330000)
node_167:set_default_value("near", 1.000000)
node_167:set_default_value("far", 6.000000)
node_154:push(node_167)
local node_160 = node_150:get_view()
local node_166 = Transform.new("LookAt")
node_166:set_render(true)
node_166:set_number_of_decimals(1)
node_166:set_lod(1)
node_166:set_position(Vec2.new(411.23206, 136.88599))
node_166:set_value(Mat4.new(Vec4.new(1, 0, -0, 0), Vec4.new(-0, 1, -0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(-0, -0, -1, 1)))
node_166:disable_synergies()
node_166:lock()
node_166:set_default_value("eye", Vec3.new(0, 0, 1))
node_166:set_default_value("center", Vec3.new(0, 0, 0))
node_166:set_default_value("up", Vec3.new(0, 1, 0))
node_160:push(node_166)
local node_168 = Screen.new()
node_168:set_render(true)
node_168:set_number_of_decimals(1)
node_168:set_lod(0)
node_168:set_position(Vec2.new(776.8001, -9.000001))
node_168:set_aspect(Vec2.new(282, 226))
local node_172 = Model.new()
node_172:set_render(true)
node_172:set_number_of_decimals(1)
node_172:set_lod(0)
node_172:set_position(Vec2.new(995.6003, 280.40024))
node_172:set_model("default_cube")
node_172:set_visible(true)
node_172:show_axes(true)
node_172:set_opaque(true)
node_172:set_opacity(0.5)
node_172:set_tint(Vec3.new(1, 1, 1))
node_172:set_tint_strength(1)
I3T.plug(node_150:get_id(), 0, node_168:get_id(), 0)
I3T.plug(node_143:get_id(), 0, node_172:get_id(), 0)
