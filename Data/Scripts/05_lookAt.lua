-- I3T.load_script_from("Data/Scripts/05_lookAtScene.lua")

local node_1 = Camera.new()
node_1:set_render(true)
node_1:set_number_of_decimals(1)
node_1:set_lod(0)
node_1:set_position(Vec2.new(176, 76.99999))
local node_5 = node_1:get_projection()
local node_18 = Transform.new("Perspective")
node_18:set_render(true)
node_18:set_number_of_decimals(1)
node_18:set_lod(0)
node_18:set_position(Vec2.new(203.80322, 138.35876))
node_18:set_value(Mat4.new(Vec4.new(1.6, 0, 0, 0), Vec4.new(0, 2.1, 0, 0), Vec4.new(0, 0, -1.4, -1), Vec4.new(0, 0, -2.3999999, 0)))
node_18:disable_synergies()
node_18:lock()
node_18:set_default_value("fovy", 0.888838)
node_18:set_default_value("aspect", 1.312500)
node_18:set_default_value("near", 1.000000)
node_18:set_default_value("far", 6.000000)
node_5:push(node_18)
local node_11 = node_1:get_view()
local node_17 = Transform.new("LookAt")
node_17:set_render(true)
node_17:set_number_of_decimals(1)
node_17:set_lod(1)
node_17:set_position(Vec2.new(411.80322, 138.35876))
node_17:set_value(Mat4.new(Vec4.new(0.49909532, 0.09201144, -0.86164826, 0), Vec4.new(-0, 0.99434674, 0.10618169, 0), Vec4.new(0.8665471, -0.052994784, 0.4962738, 0), Vec4.new(-0, 1.4901161e-08, -4.6223993, 1)))
node_17:disable_synergies()
node_17:lock()
node_17:set_default_value("eye", Vec3.new(-3.9828827, 0.4908142, 2.2939758))
node_17:set_default_value("center", Vec3.new(0, 0, 0))
node_17:set_default_value("up", Vec3.new(0, 1, 0))
node_11:push(node_17)
local node_19 = Screen.new()
node_19:set_render(true)
node_19:set_number_of_decimals(1)
node_19:set_lod(0)
node_19:set_position(Vec2.new(723, -79))
node_19:set_aspect(Vec2.new(286, 244))
local node_23 = Model.new()
node_23:set_render(true)
node_23:set_number_of_decimals(1)
node_23:set_lod(0)
node_23:set_position(Vec2.new(747.8001, 232.20007))
node_23:set_model("default_cube")
node_23:set_visible(true)
node_23:show_axes(true)
node_23:set_opaque(true)
node_23:set_opacity(0.5)
node_23:set_tint(Vec3.new(1, 1, 1))
node_23:set_tint_strength(1)
I3T.plug(node_1:get_id(), 0, node_19:get_id(), 0)