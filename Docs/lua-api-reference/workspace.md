## Workspace

### `Node`

#### method `type(): string`

Returns the type of the node in the form of a string.
See `Operation::keyWord` and `EOperatorType`, or `ETransformType`
in C++ code for possible values.

#### method `get_id(): number`

Returns the unique identifier of the node.

#### method `get_position(): Vec2`

Returns the position of the node within the Workspace.

#### method `set_position(Vec2 value)`

Sets the position of the node within the Workspace.

#### method `get_label(): string`

Returns the label of the node.

#### method `set_label(string value)`

Sets the label of the node.

#### method `get_number_of_decimals(): number`

Returns the number of decimals used for displaying the node's values.

#### method `set_number_of_decimals(number value)`

Sets the number of decimals used for displaying the node's values.

#### method `get_lod(): number`

Returns the level of detail of the node.

#### method `set_lod(number value)`

Sets the level of detail of the node.

#### method `as_operator(): Operator`
#### method `as_transform(): Transform`
#### method `as_sequence(): Sequence`
#### method `as_camera(): Camera`
#### method `as_cycle(): Cycle`
#### method `as_screen(): Screen`
#### method `as_model(): Model`

#### metamethod `__tostring(): string`

Returns a string representation of the node in the form `{type} #{ID}`.


### `Operator`

Inherits all `Node` methods.

#### constructor `new(string type)`

Creates a new operator of the given `type`.
See `EOperatorType` enum and `Operation::keyWord` in C++ code for possible values.

#### method `is_input_plugged(index: number): boolean`

Returns true if the input at the given `index` is plugged.

#### method `is_output_plugged(index: number): boolean`

Returns true if the output at the given `index` is plugged.

#### method `get_input_float(index: number): number`

Get the input value of the operator at the given `index`.
Returns nil if the input is not a `float`.

#### method `get_input_vec3(index: number): Vec3`

Get the input value of the operator at the given `index`.
Returns nil if the input is not a `Vec3`.

#### method `get_input_vec4(index: number): Vec4`

Get the input value of the operator at the given `index`.
Returns nil if the input is not a `Vec4`.

#### method `get_input_quat(index: number): Quat`

Get the input value of the operator at the given `index`.
Returns nil if the input is not a `Quat`.

#### method `get_input_mat4(index: number): Mat4`

Get the input value of the operator at the given `index`.
Returns nil if the input is not a `Mat4`.

#### method `get_float(index: number): number`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `float`.

#### method `get_vec3(index: number): Vec3`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Vec3`.

#### method `get_vec4(index: number): Vec4`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Vec4`.

#### method `get_quat(index: number): Quat`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Quat`.

#### method `get_mat4(index: number): Mat4`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Mat4`.

#### method `set_value(value: number): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.

#### method `set_value(value: Vec3): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.

#### method `set_value(value: Vec4): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.


### `Transform`

Inherits all `Node` methods.

#### constructor `new(string type)`

Creates a new transform of the given `type`.
See `ETransformType` enum and `Operation::keyWord` in C++ code for possible values.

#### method `get_value(coords: Vec2): number`

Returns the value of transform matrix at the given `coords`
where `coords.x` is the column and `coords.y` is the row.

#### method `set_value(value: number, coords: Vec2)`

Sets the `value` of transform matrix at the given `coords`
where `coords.x` is the column and `coords.y` is the row.

#### method `get_default_float(name: string): number`

Returns the value of the `float` property with the given `name`.
For example, `get_float("angle")` can be used to get the `angle` property
of a `AxisAngle` rotation transform.

#### method `get_default_vec3(name: string): Vec3`

Returns the value of the `Vec3` property with the given `name`.
For example, `get_vec3("scale")` can be used to get the `scale` property
of a `Scale` transform.

#### method `get_default_vec4(name: string): Vec4`

Returns the value of the `Vec4` property with the given `name`.
For example, `get_vec4("quat")` can be used to get the `quat` property
of a `Quat` (quaternion) rotation transform.

#### method `set_default_float(name: string, value: number)`

Sets the value of the `float` property with the given `name`.
See `get_float` for examples.

#### method `set_default_vec3(name: string, value: Vec3)`

Sets the value of the `Vec3` property with the given `name`.
See `get_vec3` for examples.

#### method `set_default_vec4(name: string, value: Vec4)`

Sets the value of the `Vec4` property with the given `name`.
See `get_vec4` for examples.

#### method `is_valid(): boolean`

Returns true if the transform is valid based on its properties.
For example a uniform scale transform is valid only if all scale 
properties are equal.

#### method `is_locked(): boolean`

Returns true if the transform is read-only.

#### method `is_in_sequence(): boolean`

Returns true if the transform is part of a sequence.

#### method `lock()`

Makes the transform read-only.

#### method `unlock()`

Makes the transform writable.

#### method `has_synergies(): boolean`

Returns true if the transform has synergies.
For example a `Scale` transform with enabled synergies will have
its `x`, `y`, and `z` properties locked together.
When one of the properties is changed, the other two will be updated
to match the changed one.

#### method `enable_synergies()`

Enables synergies for the transform.
See `has_synergies` for more information.

#### method `disable_synergies()`

Disables synergies for the transform.
See `has_synergies` for more information.


### `Sequence`

Inherits all `Node` methods.

#### constructor `new()`

#### method `get_mat4(index: number): Mat4`

Returns the transform matrix at the given `index`.

#### method `push(transform: Transform)`

Pushes the given `transform` to the back of the sequence.

#### method `push(transform: Transform, index: number)`

Inserts the given `transform` at the given `index` in the sequence.


### `Model`

Inherits all `Node` methods.

#### constructor `new()`

#### method `set_model(alias: string)`

Sets new model by its `alias`.

#### method `set_visible(value: boolean)`

Sets the visibility of the model.

#### method `show_axes(value: boolean)`

Sets the visibility of the model's axes.

#### method `set_opaque(value: boolean)`

Sets the model to be opaque or transparent.

#### method `set_opacity(value: number)`

Sets the opacity of the model in the range [0, 1].

#### method `set_tint(color: Vec3)`

Sets the tint color of the model in the RGB format in the range [0, 1].

#### method `set_tint_strength(value: number)`

Sets the strength of the tint color in the range [0, 1].


### `Cycle`

Inherits all `Node` methods.

#### constructor `new(): Cycle`

#### method `set_from(value: float)`
#### method `set_to(value: float)`
#### method `set_manual_step(value: float)`
#### method `set_step(value: float)`
#### method `set_step_duration(value: float)`
#### method `set_smooth(value: boolean)`
#### method `set_mode(value: number)`
#### method `play()`


### `Camera`

Inherits all `Node` methods.

#### constructor `new(): Camera`

#### method `get_projection(): Sequence`

Returns the projection sequence of the camera.

#### method `get_view(): Sequence`

Returns the view sequence of the camera.


### `Screen`

Inherits all `Node` methods.

#### constructor `new(): Screen`

#### method `set_acpect(value: Vec2)`

Sets the aspect.


### `ScriptingNode`

#### constructor `new()`

Creates a new scripting node.

#### method `get_script(): string`

Returns the Lua script of the node.

#### method `set_script(script: string): ScriptingNode`

Sets the Lua script of the node.
Note that on success the new node will be returned!


### enum `ValueType`

#### `ValueType.Float`

#### `ValueType.Vec3`

#### `ValueType.Vec4`

#### `ValueType.Matrix`

#### `ValueType.Quat`


### `Operation`

### constructor `new(inputs: table[string, ValueType], outputs: table[string, ValueType])`

Creates a new operation from the given `inputs` and `outputs`.
The `inputs` and `outputs` tables should have the form `{ name = ValueType }`.
If label name contains spaces, pass it like this: `{ ["label name"] = ValueType }`.

#### attribute `inputTypes: ValueType[]`

Types of the operation's inputs.

#### attribute `outputTypes: ValueType[]`

Types of the operation's outputs.

#### attribute `defaultInputNames: string[]`

Names of the operation's inputs, must be the same length as `inputTypes`.

#### attribute `defaultOutputNames: string[]`

Names of the operation's outputs, must be the same length as `outputTypes`.


### Scripting Node API functions

Scripting Node is a special node that can be modified using Lua scripts.
You can specify node properties by creating a new `Operation` and attaching
it to the node.
The API provides a set of functions - `on_init` and `on_update_values` - that can be
used in the script to interact with the node,
you can also access the node's properties by using the `self.node` object.
To save custom properties, you can use the `self` table, for example `self.my_property = 1`.

Example of a simple script:

```lua
self.operation = Operation.new(
	{ x = ValueType.Float, y = ValueType.Float },
	{ result = ValueType.Vec3 }
)
self.on_init = function() print("Node initialized!") end
self.on_update_values = function()
	local operator = self.node:as_operator()
	if (operator:is_input_plugged(1) and operator:is_input_plugged(2)) then
		local x = operator:get_input_float(1)
		local y = operator:get_input_float(2)
		operator:set_value(Vec3.new(x, y, x + y))
	end
end
```

#### attribute `self.operation: Operation`

The operation which is used to create the node.

#### attribute `self.node: Operator`

The node that the script is attached to.

#### function `self.on_init()`

Called when the node is created.
Function is protected, errors will be caught and printed to the console.

#### function `self.on_update_values()`

Called when the node's values are updated.
Function is protected, errors will be caught and printed to the console.


### Workspace API functions

#### function `I3T.print_operator_types()`

Prints all available operator types to the console.

#### function `I3T.print_transform_types()`

Prints all available transform types to the console.

#### function `I3T.print_workspace()`

Prints the workspace nodes to the console in the form of signatures.

#### function `I3T.get_node(id: number): Node`

Returns nil if the node with the given `id` does not exist.

#### function `I3T.delete_node(node: Node)`

Deletes the given `node` from the workspace.

#### function `I3T.to_script(): string`

Returns the Lua script representation of the workspace.

#### function `I3T.get_all_nodes(): Node[]`

Returns all nodes in the workspace.

#### function `I3T.get_node(id: number): Node`

Returns the node with the given `id`.

#### function `I3T.delete_node(node: Node)`

Deletes the given `node` from the workspace.

#### function `I3T.plug(from: number, from_index: number, to: number, to_index: number)`

Plugs the output of the node with the given `from` id at the given `from_index`
to the input of the node with the given `to` id at the given `to_index`.

#### function `I3T.unplug_input(node: number, index: number)`

Unplugs the input of the node with the given `node` id at the given `index`.

#### function `I3T.unplug_output(node: number, index: number)`

Unplugs the output of the node with the given `node` id at the given `index`.

#### function `I3T.workspace.clear()`

Deletes all nodes from the workspace.

#### function `I3T.workspace.set_zoom(value: number)`

Sets the zoom level of the workspace.

#### function `I3T.workspace.set_work_area(min: Vec2, max: Vec2)`

Sets the work area of the workspace, where `min` is the top-left corner
and `max` is the bottom-right corner.
