-- I3T.load_script_from("Data/Scripts/03_rotateAroundPoint.lua")

local node_142 = Operator.new("Inversion")
node_142:set_render(true)
node_142:set_number_of_decimals(1)
node_142:set_lod(0)
node_142:set_position(Vec2.new(660.333, 27.761864))
local node_145 = Sequence.new()
node_145:set_label("A^(-1)")
node_145:set_render(true)
node_145:set_number_of_decimals(1)
node_145:set_lod(0)
node_145:set_position(Vec2.new(894.3805, 162.71426))
local node_151 = Sequence.new()
node_151:set_label("Rotation with respect to reference frame")
node_151:set_render(true)
node_151:set_number_of_decimals(1)
node_151:set_lod(0)
node_151:set_position(Vec2.new(500.333, 161.31422))
local node_157 = Transform.new("EulerY")
node_157:set_render(true)
node_157:set_number_of_decimals(1)
node_157:set_lod(0)
node_157:set_position(Vec2.new(564.6745, 191.84134))
node_157:set_value(Mat4.new(Vec4.new(0.9, 0, -0.43588996, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0.43588996, 0, 0.9, 0), Vec4.new(0, 0, 0, 1)))
node_157:enable_synergies()
node_157:lock()
node_157:set_default_value("angle", 0.451027)
node_151:push(node_157)
local node_158 = Sequence.new()
node_158:set_label("A")
node_158:set_render(true)
node_158:set_number_of_decimals(1)
node_158:set_lod(0)
node_158:set_position(Vec2.new(282.76172, 159.66663))
local node_164 = Sequence.new()
node_164:set_label("Reference point")
node_164:set_render(true)
node_164:set_number_of_decimals(1)
node_164:set_lod(0)
node_164:set_position(Vec2.new(10.238182, -66.285736))
local node_170 = Transform.new("Translation")
node_170:set_render(true)
node_170:set_number_of_decimals(1)
node_170:set_lod(0)
node_170:set_position(Vec2.new(30.17456, -36.158646))
node_170:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(-3.1, 0, 3.5, 1)))
node_170:disable_synergies()
node_170:lock()
node_170:set_default_value("translation", Vec3.new(-3.1, 0, 3.5))
node_164:push(node_170)
local node_171 = Model.new()
node_171:set_render(true)
node_171:set_number_of_decimals(1)
node_171:set_lod(0)
node_171:set_position(Vec2.new(1119.9521, 157.71428))
node_171:set_model("default_cube")
node_171:set_visible(true)
node_171:show_axes(true)
node_171:set_opaque(true)
node_171:set_opacity(0.5)
node_171:set_tint(Vec3.new(1, 1, 1))
node_171:set_tint_strength(1)
local node_173 = Model.new()
node_173:set_label("Reference coorfinate frame")
node_173:set_render(true)
node_173:set_number_of_decimals(1)
node_173:set_lod(0)
node_173:set_position(Vec2.new(293.65594, -186.89542))
node_173:set_model("Axes")
node_173:set_visible(true)
node_173:show_axes(false)
node_173:set_opaque(true)
node_173:set_opacity(0.5)
node_173:set_tint(Vec3.new(1, 1, 1))
node_173:set_tint_strength(1)
I3T.plug(node_164:get_id(), 1, node_142:get_id(), 0)
I3T.plug(node_151:get_id(), 0, node_145:get_id(), 0)
I3T.plug(node_142:get_id(), 0, node_145:get_id(), 1)
I3T.plug(node_158:get_id(), 0, node_151:get_id(), 0)
I3T.plug(node_164:get_id(), 1, node_158:get_id(), 1)
I3T.plug(node_145:get_id(), 0, node_171:get_id(), 0)
I3T.plug(node_164:get_id(), 0, node_173:get_id(), 0)