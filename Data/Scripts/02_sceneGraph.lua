-- I3T.load_script_from("Data/Scripts/02_sceneGraph.lua")

local node_1 = Sequence.new()
node_1:set_render(true)
node_1:set_number_of_decimals(1)
node_1:set_lod(0)
node_1:set_position(Vec2.new(61.714283, -50.14285))
local node_7 = Transform.new("Translation")
node_7:set_render(true)
node_7:set_number_of_decimals(1)
node_7:set_lod(0)
node_7:set_position(Vec2.new(81, -20))
node_7:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0, -0.2, 0, 1)))
node_7:disable_synergies()
node_7:lock()
node_7:set_default_value("translation", Vec3.new(0, -0.2, 0))
node_1:push(node_7)
local node_8 = Sequence.new()
node_8:set_render(true)
node_8:set_number_of_decimals(1)
node_8:set_lod(0)
node_8:set_position(Vec2.new(428.71436, -168.14285))
local node_14 = Transform.new("Scale")
node_14:set_render(true)
node_14:set_number_of_decimals(1)
node_14:set_lod(0)
node_14:set_position(Vec2.new(448, -138))
node_14:set_value(Mat4.new(Vec4.new(4, 0, 0, 0), Vec4.new(0, 0.2, 0, 0), Vec4.new(0, 0, 4, 0), Vec4.new(0, 0, 0, 1)))
node_14:disable_synergies()
node_14:lock()
node_14:set_default_value("scale", Vec3.new(4, 0.2, 4))
node_8:push(node_14)
local node_24 = Sequence.new()
node_24:set_render(true)
node_24:set_number_of_decimals(1)
node_24:set_lod(0)
node_24:set_position(Vec2.new(326.71432, 29.857147))
local node_30 = Transform.new("Translation")
node_30:set_render(true)
node_30:set_number_of_decimals(1)
node_30:set_lod(0)
node_30:set_position(Vec2.new(346, 60))
node_30:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0, 0.2, 0, 1)))
node_30:disable_synergies()
node_30:lock()
node_30:set_default_value("translation", Vec3.new(0, 0.2, 0))
node_24:push(node_30)
local node_31 = Transform.new("Scale")
node_31:set_render(true)
node_31:set_number_of_decimals(1)
node_31:set_lod(0)
node_31:set_position(Vec2.new(518, 60))
node_31:set_value(Mat4.new(Vec4.new(2.5, 0, 0, 0), Vec4.new(0, 2.5, 0, 0), Vec4.new(0, 0, 2.5, 0), Vec4.new(0, 0, 0, 1)))
node_31:enable_synergies()
node_31:lock()
node_31:set_default_value("scale", Vec3.new(2.5, 2.5, 2.5))
node_24:push(node_31)
local node_34 = Model.new()
node_34:set_render(true)
node_34:set_number_of_decimals(1)
node_34:set_lod(0)
node_34:set_position(Vec2.new(1210.7142, 367.85715))
node_34:set_model("Teapot")
node_34:set_visible(true)
node_34:show_axes(false)
node_34:set_opaque(true)
node_34:set_opacity(0.5)
node_34:set_tint(Vec3.new(1, 1, 1))
node_34:set_tint_strength(1)
local node_36 = Model.new()
node_36:set_render(true)
node_36:set_number_of_decimals(1)
node_36:set_lod(0)
node_36:set_position(Vec2.new(747.71423, -167.14285))
node_36:set_model("default_cube")
node_36:set_visible(true)
node_36:show_axes(false)
node_36:set_opaque(true)
node_36:set_opacity(0.5)
node_36:set_tint(Vec3.new(1, 1, 1))
node_36:set_tint_strength(1)
local node_32 = Model.new()
node_32:set_render(true)
node_32:set_number_of_decimals(1)
node_32:set_lod(0)
node_32:set_position(Vec2.new(755.71423, 33.857117))
node_32:set_model("Duck")
node_32:set_visible(true)
node_32:show_axes(false)
node_32:set_opaque(true)
node_32:set_opacity(0.5)
node_32:set_tint(Vec3.new(1, 1, 1))
node_32:set_tint_strength(1)
local node_15 = Sequence.new()
node_15:set_render(true)
node_15:set_number_of_decimals(1)
node_15:set_lod(0)
node_15:set_position(Vec2.new(749.71423, 183.85715))
local node_21 = Transform.new("Translation")
node_21:set_render(true)
node_21:set_number_of_decimals(1)
node_21:set_lod(0)
node_21:set_position(Vec2.new(769, 214))
node_21:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0.30439836, 1.815, -0.031425856, 1)))
node_21:disable_synergies()
node_21:lock()
node_21:set_default_value("translation", Vec3.new(0.30439836, 1.815, -0.031425856))
node_15:push(node_21)
local node_22 = Transform.new("EulerY")
node_22:set_render(true)
node_22:set_number_of_decimals(1)
node_22:set_lod(0)
node_22:set_position(Vec2.new(941, 214))
node_22:set_value(Mat4.new(Vec4.new(-0.50000006, 0, -0.8660254, 0), Vec4.new(0, 0.99999994, 0, 0), Vec4.new(0.8660254, 0, -0.50000006, 0), Vec4.new(0, 0, 0, 1)))
node_22:enable_synergies()
node_22:lock()
node_22:set_default_value("angle", 2.094395)
node_15:push(node_22)
local node_23 = Transform.new("Scale")
node_23:set_render(true)
node_23:set_number_of_decimals(1)
node_23:set_lod(0)
node_23:set_position(Vec2.new(1113, 214))
node_23:set_value(Mat4.new(Vec4.new(0.3, 0, 0, 0), Vec4.new(0, 0.3, 0, 0), Vec4.new(0, 0, 0.3, 0), Vec4.new(0, 0, 0, 1)))
node_23:enable_synergies()
node_23:lock()
node_23:set_default_value("scale", Vec3.new(0.3, 0.3, 0.3))
node_15:push(node_23)
I3T.plug(node_1:get_id(), 0, node_8:get_id(), 0)
I3T.plug(node_1:get_id(), 0, node_24:get_id(), 0)
I3T.plug(node_15:get_id(), 0, node_34:get_id(), 0)
I3T.plug(node_8:get_id(), 0, node_36:get_id(), 0)
I3T.plug(node_24:get_id(), 0, node_32:get_id(), 0)
I3T.plug(node_24:get_id(), 0, node_15:get_id(), 0)
