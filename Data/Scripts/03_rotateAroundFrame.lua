-- I3T.load_script_from("Data/Scripts/03_rotateAroundFrame.lua")

local node_1 = Operator.new("Inversion")
node_1:set_render(true)
node_1:set_number_of_decimals(1)
node_1:set_lod(0)
node_1:set_position(Vec2.new(660.333, 27.761864))
local node_4 = Operator.new("MakeEulerY")
node_4:set_render(true)
node_4:set_number_of_decimals(1)
node_4:set_lod(2)
node_4:set_position(Vec2.new(484.7777, 379.00024))
local node_7 = Sequence.new()
node_7:set_label("A^(-1)")
node_7:set_render(true)
node_7:set_number_of_decimals(1)
node_7:set_lod(0)
node_7:set_position(Vec2.new(894.3805, 162.71426))
local node_13 = Sequence.new()
node_13:set_label("A")
node_13:set_render(true)
node_13:set_number_of_decimals(1)
node_13:set_lod(0)
node_13:set_position(Vec2.new(282.76172, 159.66663))
local node_19 = Sequence.new()
node_19:set_label("Rotation with respect to the reference frame")
node_19:set_render(true)
node_19:set_number_of_decimals(1)
node_19:set_lod(0)
node_19:set_position(Vec2.new(500.333, 161.31422))
local node_25 = Transform.new("EulerY")
node_25:set_render(true)
node_25:set_number_of_decimals(1)
node_25:set_lod(0)
node_25:set_position(Vec2.new(602.1111, 191.44452))
node_25:set_value(Mat4.new(Vec4.new(0.6, 0, -0.8, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0.8, 0, 0.6, 0), Vec4.new(0, 0, 0, 1)))
node_25:enable_synergies()
node_25:lock()
node_25:set_default_value("angle", 0.927295)
node_19:push(node_25)
local node_26 = Sequence.new()
node_26:set_label("Reference coordinate frame transformation")
node_26:set_render(true)
node_26:set_number_of_decimals(1)
node_26:set_lod(0)
node_26:set_position(Vec2.new(10.238182, -66.285736))
local node_32 = Transform.new("Translation")
node_32:set_label("translate origin")
node_32:set_render(true)
node_32:set_number_of_decimals(1)
node_32:set_lod(0)
node_32:set_position(Vec2.new(29.844456, -35.82213))
node_32:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(-2.6, 1.3, 3.5, 1)))
node_32:disable_synergies()
node_32:lock()
node_32:set_default_value("translation", Vec3.new(-2.6, 1.3, 3.5))
node_26:push(node_32)
local node_33 = Transform.new("EulerX")
node_33:set_render(true)
node_33:set_number_of_decimals(1)
node_33:set_lod(0)
node_33:set_position(Vec2.new(200.95557, -35.82213))
node_33:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 0.2, 0.9797959, 0), Vec4.new(0, -0.9797959, 0.2, 0), Vec4.new(0, 0, 0, 1)))
node_33:enable_synergies()
node_33:lock()
node_33:set_default_value("angle", 1.369438)
node_26:push(node_33)
local node_34 = Cycle.new()
node_34:set_render(true)
node_34:set_number_of_decimals(1)
node_34:set_lod(2)
node_34:set_position(Vec2.new(132.77791, 370.11127))
node_34:set_from(0)
node_34:set_to(6.28)
node_34:set_manual_step(0.1)
node_34:set_step(0.2)
node_34:set_step_duration(0.1)
node_34:set_smooth(true)
node_34:set_mode(1)
node_34:play()
local node_51 = Model.new()
node_51:set_render(true)
node_51:set_number_of_decimals(1)
node_51:set_lod(0)
node_51:set_position(Vec2.new(1119.9521, 157.71428))
node_51:set_model("default_cube")
node_51:set_visible(true)
node_51:show_axes(true)
node_51:set_opaque(true)
node_51:set_opacity(0.5)
node_51:set_tint(Vec3.new(1, 1, 1))
node_51:set_tint_strength(1)
local node_53 = Model.new()
node_53:set_label("Reference coorfinate frame")
node_53:set_render(true)
node_53:set_number_of_decimals(1)
node_53:set_lod(0)
node_53:set_position(Vec2.new(481.21136, -208.2287))
node_53:set_model("Axes")
node_53:set_visible(true)
node_53:show_axes(false)
node_53:set_opaque(true)
node_53:set_opacity(0.5)
node_53:set_tint(Vec3.new(1, 1, 1))
node_53:set_tint_strength(1)
I3T.plug(node_26:get_id(), 1, node_1:get_id(), 0)
I3T.plug(node_34:get_id(), 0, node_4:get_id(), 0)
I3T.plug(node_19:get_id(), 0, node_7:get_id(), 0)
I3T.plug(node_1:get_id(), 0, node_7:get_id(), 1)
I3T.plug(node_26:get_id(), 1, node_13:get_id(), 1)
I3T.plug(node_13:get_id(), 0, node_19:get_id(), 0)
I3T.plug(node_4:get_id(), 0, node_19:get_id(), 1)
I3T.plug(node_7:get_id(), 0, node_51:get_id(), 0)
I3T.plug(node_26:get_id(), 0, node_53:get_id(), 0)
