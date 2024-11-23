-- I3T.load_script_from("Data/Scripts/01_modelTransformation.lua")

I3T.workspace.set_work_area(Vec2.new(160.0, 80.0), Vec2.new(1300.0, 890.0))
I3T.workspace.set_zoom(1.25)

-- TODO
-- "numberOfDecimals": 1,
-- "LOD": "Full",

local sequence = Sequence.new()
sequence:set_position(Vec2.new(250.0, 300.0))

local translation = Transform.new("Translation")
translation:set_default_value("translation", Vec3.new(3.0, 0.0, 0.0))
translation:disable_synergies()
translation:lock()

local rotationY = Transform.new("EulerY")
rotationY:set_default_value("angle", -0.9272952675819397)
translation:enable_synergies()
translation:lock()

local scale = Transform.new("Scale")
scale:set_default_value("scale", Vec3.new(1.9, 1.9, 1.9))
translation:enable_synergies()
translation:lock()

sequence:push(translation)
sequence:push(rotationY)
sequence:push(scale)

local model = Model.new("default_cube")
model:set_position(Vec2.new(860.0, 300.0))
model:set_opaque(true)
model:set_opacity(0.5)
model:set_visible(true)
model:show_axes(true)
model:set_tint_strength(1.0)

I3T.plug(sequence:get_id(), 0, model:get_id(), 0)
