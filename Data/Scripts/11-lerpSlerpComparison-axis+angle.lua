-- I3T.load_script_from("Data/Scripts/11-lerpSlerpComparison-axis+angle.lua")

local node_1 = Operator.new("QuatToMatrix")
node_1:set_render(true)
node_1:set_number_of_decimals(1)
node_1:set_lod(0)
node_1:set_position(Vec2.new(933, 25))
local node_4 = Operator.new("QuatToMatrix")
node_4:set_render(true)
node_4:set_number_of_decimals(1)
node_4:set_lod(0)
node_4:set_position(Vec2.new(924.42865, 897.57184))
local node_7 = Operator.new("QuatToMatrix")
node_7:set_render(true)
node_7:set_number_of_decimals(1)
node_7:set_lod(0)
node_7:set_position(Vec2.new(925.4289, 714.57184))
local node_10 = Operator.new("Vector3ToVector3")
node_10:set_label("start xyz axis")
node_10:set_render(true)
node_10:set_number_of_decimals(1)
node_10:set_lod(0)
node_10:set_position(Vec2.new(-43.42856, -15.714281))
node_10:set_value(Vec3.new(-0.8, -0.9, -0.3))
local node_13 = Operator.new("QuatSlerp")
node_13:set_render(true)
node_13:set_number_of_decimals(1)
node_13:set_lod(0)
node_13:set_position(Vec2.new(625.8573, -28.14289))
local node_18 = Operator.new("Vector3ToVector3")
node_18:set_label("end xyz axis")
node_18:set_render(true)
node_18:set_number_of_decimals(1)
node_18:set_lod(0)
node_18:set_position(Vec2.new(-55.42856, 196.2858))
node_18:set_value(Vec3.new(0.6, 2.6, -1.5))
local node_21 = Operator.new("FloatToFloat")
node_21:set_label("end w")
node_21:set_render(true)
node_21:set_number_of_decimals(1)
node_21:set_lod(0)
node_21:set_position(Vec2.new(-51.428555, 131.28568))
node_21:set_value(4.600000)
local node_24 = Operator.new("QuatToMatrix")
node_24:set_render(true)
node_24:set_number_of_decimals(1)
node_24:set_lod(0)
node_24:set_position(Vec2.new(925.972, 421.62863))
local node_27 = Operator.new("QuatToMatrix")
node_27:set_render(true)
node_27:set_number_of_decimals(1)
node_27:set_lod(0)
node_27:set_position(Vec2.new(926.9715, 218.28577))
local node_30 = Operator.new("NormalizeQuat")
node_30:set_render(true)
node_30:set_number_of_decimals(1)
node_30:set_lod(0)
node_30:set_position(Vec2.new(693.9436, 140.91444))
local node_33 = Operator.new("AngleAxisToQuat")
node_33:set_render(true)
node_33:set_number_of_decimals(4)
node_33:set_lod(0)
node_33:set_position(Vec2.new(157.29031, -80.99947))
local node_38 = Operator.new("QuatLength")
node_38:set_render(true)
node_38:set_number_of_decimals(4)
node_38:set_lod(0)
node_38:set_position(Vec2.new(312.55505, 225.84851))
local node_41 = Operator.new("QuatLength")
node_41:set_render(true)
node_41:set_number_of_decimals(4)
node_41:set_lod(0)
node_41:set_position(Vec2.new(712.71356, 481.17532))
local node_44 = Operator.new("QuatLength")
node_44:set_render(true)
node_44:set_number_of_decimals(4)
node_44:set_lod(0)
node_44:set_position(Vec2.new(700.40796, 50.14396))
local node_47 = Operator.new("AngleAxisToQuat")
node_47:set_render(true)
node_47:set_number_of_decimals(4)
node_47:set_lod(0)
node_47:set_position(Vec2.new(143.85663, 124.426254))
local node_52 = Operator.new("QuatLerp")
node_52:set_render(true)
node_52:set_number_of_decimals(1)
node_52:set_lod(0)
node_52:set_position(Vec2.new(617.7121, 194.1132))
local node_57 = Operator.new("QuatLongWaySlerp")
node_57:set_render(true)
node_57:set_number_of_decimals(1)
node_57:set_lod(0)
node_57:set_position(Vec2.new(619.027, 397.0383))
local node_62 = Operator.new("QuatLength")
node_62:set_render(true)
node_62:set_number_of_decimals(4)
node_62:set_lod(0)
node_62:set_position(Vec2.new(722.5398, 281.5439))
local node_65 = Operator.new("FloatToFloat")
node_65:set_label("start w")
node_65:set_render(true)
node_65:set_number_of_decimals(1)
node_65:set_lod(0)
node_65:set_position(Vec2.new(-44.42856, -78.71431))
node_65:set_value(1.700000)
local node_68 = Sequence.new()
node_68:set_render(true)
node_68:set_number_of_decimals(1)
node_68:set_lod(0)
node_68:set_position(Vec2.new(1151.835, 185.58682))
local node_74 = Transform.new("Translation")
node_74:set_render(true)
node_74:set_number_of_decimals(1)
node_74:set_lod(0)
node_74:set_position(Vec2.new(1171.754, 215.47813))
node_74:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0, 0, 0, 1)))
node_74:disable_synergies()
node_74:lock()
node_74:set_default_value("translation", Vec3.new(0, 0, 0))
node_68:push(node_74)
local node_75 = Sequence.new()
node_75:set_render(true)
node_75:set_number_of_decimals(1)
node_75:set_lod(0)
node_75:set_position(Vec2.new(1149.2922, 864.8728))
local node_81 = Transform.new("Translation")
node_81:set_render(true)
node_81:set_number_of_decimals(1)
node_81:set_lod(0)
node_81:set_position(Vec2.new(1169.0951, 894.8507))
node_81:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(3.7198045, -0.81059116, -1.7836914, 1)))
node_81:disable_synergies()
node_81:lock()
node_81:set_default_value("translation", Vec3.new(3.7198045, -0.81059116, -1.7836914))
node_75:push(node_81)
local node_82 = Sequence.new()
node_82:set_render(true)
node_82:set_number_of_decimals(1)
node_82:set_lod(0)
node_82:set_position(Vec2.new(1150.2917, 681.8728))
local node_88 = Transform.new("Translation")
node_88:set_render(true)
node_88:set_number_of_decimals(1)
node_88:set_lod(0)
node_88:set_position(Vec2.new(1169.0951, 712.7097))
node_88:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(-3.8867314, -0.7929491, -1.6783599, 1)))
node_88:disable_synergies()
node_88:lock()
node_88:set_default_value("translation", Vec3.new(-3.8867314, -0.7929491, -1.6783599))
node_82:push(node_88)
local node_89 = Sequence.new()
node_89:set_render(true)
node_89:set_number_of_decimals(1)
node_89:set_lod(0)
node_89:set_position(Vec2.new(1150.8342, 388.92966))
local node_95 = Transform.new("Translation")
node_95:set_render(true)
node_95:set_number_of_decimals(1)
node_95:set_lod(0)
node_95:set_position(Vec2.new(1170.4246, 418.89105))
node_95:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(-0.44580233, -2.895607, 1.6717682, 1)))
node_95:disable_synergies()
node_95:lock()
node_95:set_default_value("translation", Vec3.new(-0.44580233, -2.895607, 1.6717682))
node_89:push(node_95)
local node_96 = Sequence.new()
node_96:set_render(true)
node_96:set_number_of_decimals(1)
node_96:set_lod(0)
node_96:set_position(Vec2.new(1157.8633, -7.6989613))
local node_102 = Transform.new("Translation")
node_102:set_render(true)
node_102:set_number_of_decimals(1)
node_102:set_lod(0)
node_102:set_position(Vec2.new(1177.072, 22.701218))
node_102:set_value(Mat4.new(Vec4.new(1, 0, 0, 0), Vec4.new(0, 1, 0, 0), Vec4.new(0, 0, 1, 0), Vec4.new(0.06870228, 2.7765503, -1.603073, 1)))
node_102:disable_synergies()
node_102:lock()
node_102:set_default_value("translation", Vec3.new(0.06870228, 2.7765503, -1.603073))
node_96:push(node_102)
local node_103 = Sequence.new()
node_103:set_render(true)
node_103:set_number_of_decimals(1)
node_103:set_lod(0)
node_103:set_position(Vec2.new(1390.5576, 213.86003))
local node_109 = Sequence.new()
node_109:set_render(true)
node_109:set_number_of_decimals(1)
node_109:set_lod(0)
node_109:set_position(Vec2.new(1389.5569, 417.20288))
local node_115 = Sequence.new()
node_115:set_render(true)
node_115:set_number_of_decimals(1)
node_115:set_lod(0)
node_115:set_position(Vec2.new(1389.0144, 710.1462))
local node_121 = Sequence.new()
node_121:set_render(true)
node_121:set_number_of_decimals(1)
node_121:set_lod(0)
node_121:set_position(Vec2.new(1388.0149, 893.1462))
local node_133 = Cycle.new()
node_133:set_render(true)
node_133:set_number_of_decimals(4)
node_133:set_lod(0)
node_133:set_position(Vec2.new(163.14276, 640.4282))
node_133:set_from(0)
node_133:set_to(1)
node_133:set_manual_step(0.1)
node_133:set_step(0.07)
node_133:set_step_duration(0.31)
node_133:set_smooth(true)
node_133:set_mode(2)
node_133:play()
local node_150 = Model.new()
node_150:set_render(true)
node_150:set_number_of_decimals(1)
node_150:set_lod(0)
node_150:set_position(Vec2.new(1610.3009, 880.28906))
node_150:set_model("default_cube")
node_150:set_visible(true)
node_150:show_axes(true)
node_150:set_opaque(true)
node_150:set_opacity(0.5)
node_150:set_tint(Vec3.new(0.9599999, 0.64000005, 0.96))
node_150:set_tint_strength(1)
local node_152 = Model.new()
node_152:set_render(true)
node_152:set_number_of_decimals(1)
node_152:set_lod(0)
node_152:set_position(Vec2.new(1618.872, 7.7171497))
node_152:set_model("default_cube")
node_152:set_visible(true)
node_152:show_axes(true)
node_152:set_opaque(true)
node_152:set_opacity(0.5)
node_152:set_tint(Vec3.new(0.96, 0.64000005, 0.64000016))
node_152:set_tint_strength(1)
local node_154 = Model.new()
node_154:set_render(true)
node_154:set_number_of_decimals(1)
node_154:set_lod(0)
node_154:set_position(Vec2.new(1612.8436, 201.00291))
node_154:set_model("default_cube")
node_154:set_visible(true)
node_154:show_axes(true)
node_154:set_opaque(true)
node_154:set_opacity(0.5)
node_154:set_tint(Vec3.new(0.64000005, 0.96, 0.64000005))
node_154:set_tint_strength(1)
local node_156 = Model.new()
node_156:set_render(true)
node_156:set_number_of_decimals(1)
node_156:set_lod(0)
node_156:set_position(Vec2.new(1611.8429, 404.34576))
node_156:set_model("default_cube")
node_156:set_visible(true)
node_156:show_axes(true)
node_156:set_opaque(true)
node_156:set_opacity(0.5)
node_156:set_tint(Vec3.new(0.64000005, 0.64000005, 0.96))
node_156:set_tint_strength(1)
local node_158 = Model.new()
node_158:set_render(true)
node_158:set_number_of_decimals(1)
node_158:set_lod(0)
node_158:set_position(Vec2.new(1611.3004, 697.28906))
node_158:set_model("default_cube")
node_158:set_visible(true)
node_158:show_axes(true)
node_158:set_opaque(true)
node_158:set_opacity(0.5)
node_158:set_tint(Vec3.new(0.96000004, 0.96, 0.64000005))
node_158:set_tint_strength(1)
local node_127 = Sequence.new()
node_127:set_render(true)
node_127:set_number_of_decimals(1)
node_127:set_lod(0)
node_127:set_position(Vec2.new(1396.5859, 20.574266))

I3T.plug(node_13:get_id(), 0, node_1:get_id(), 0)
I3T.plug(node_47:get_id(), 0, node_4:get_id(), 0)
I3T.plug(node_33:get_id(), 0, node_7:get_id(), 0)
I3T.plug(node_33:get_id(), 0, node_13:get_id(), 0)
I3T.plug(node_47:get_id(), 0, node_13:get_id(), 1)
I3T.plug(node_133:get_id(), 0, node_13:get_id(), 2)
I3T.plug(node_57:get_id(), 0, node_24:get_id(), 0)
I3T.plug(node_30:get_id(), 0, node_27:get_id(), 0)
I3T.plug(node_52:get_id(), 0, node_30:get_id(), 0)
I3T.plug(node_65:get_id(), 0, node_33:get_id(), 0)
I3T.plug(node_10:get_id(), 0, node_33:get_id(), 2)
I3T.plug(node_47:get_id(), 0, node_38:get_id(), 0)
I3T.plug(node_57:get_id(), 0, node_41:get_id(), 0)
I3T.plug(node_13:get_id(), 0, node_44:get_id(), 0)
I3T.plug(node_21:get_id(), 0, node_47:get_id(), 0)
I3T.plug(node_18:get_id(), 0, node_47:get_id(), 2)
I3T.plug(node_33:get_id(), 0, node_52:get_id(), 0)
I3T.plug(node_47:get_id(), 0, node_52:get_id(), 1)
I3T.plug(node_133:get_id(), 0, node_52:get_id(), 2)
I3T.plug(node_33:get_id(), 0, node_57:get_id(), 0)
I3T.plug(node_47:get_id(), 0, node_57:get_id(), 1)
I3T.plug(node_133:get_id(), 0, node_57:get_id(), 2)
I3T.plug(node_52:get_id(), 0, node_62:get_id(), 0)
I3T.plug(node_68:get_id(), 0, node_103:get_id(), 0)
I3T.plug(node_27:get_id(), 0, node_103:get_id(), 1)
I3T.plug(node_89:get_id(), 0, node_109:get_id(), 0)
I3T.plug(node_24:get_id(), 0, node_109:get_id(), 1)
I3T.plug(node_82:get_id(), 0, node_115:get_id(), 0)
I3T.plug(node_7:get_id(), 0, node_115:get_id(), 1)
I3T.plug(node_75:get_id(), 0, node_121:get_id(), 0)
I3T.plug(node_4:get_id(), 0, node_121:get_id(), 1)
I3T.plug(node_121:get_id(), 0, node_150:get_id(), 0)
I3T.plug(node_127:get_id(), 0, node_152:get_id(), 0)
I3T.plug(node_103:get_id(), 0, node_154:get_id(), 0)
I3T.plug(node_109:get_id(), 0, node_156:get_id(), 0)
I3T.plug(node_115:get_id(), 0, node_158:get_id(), 0)
I3T.plug(node_96:get_id(), 0, node_127:get_id(), 0)
I3T.plug(node_1:get_id(), 0, node_127:get_id(), 1)