local node = Operator.new("FloatToFloat")
node:set_position(Vec2.new(0, 0))

local makeRotation = Operator.new("MakeEulerX")
makeRotation:set_position(Vec2.new(0, 150))

local sequence = Sequence.new()
sequence:set_position(Vec2.new(0, 300))

local model = Model.new()
model:set_position(Vec2.new(0, 450))

I3T.plug(node:get_id(), 0, makeRotation:get_id(), 0)
I3T.plug(makeRotation:get_id(), 0, sequence:get_id(), 1)
I3T.plug(sequence:get_id(), 0, model:get_id(), 0)

I3T.set_timer(0.25, function()
    local value = math.sin(os.clock() / 10)
    node:set_label("sin(" .. tostring(value) .. ")")
    node:set_value(value)
end)
