## Workspace

### `Node`

#### method `type(): string`

Returns the type of the node in the form of a string.
See `Operation::keyWord` and `EOperatorType`, or `ETransformType`
in C++ code for possible values.

#### method `get_position(): Vec2`

Returns the position of the node within the Workspace.

#### method `set_position(Vec2 value)`

Sets the position of the node within the Workspace.

#### metamethod `__tostring(): string`

Returns a string representation of the node in the form `{type} #{ID}`.


### `Operator`

Inherits all `Node` methods.

#### constructor `new(string type)`

Creates a new operator of the given `type`.
See `EOperatorType` enum and `Operation::keyWord` in C++ code for possible values.

#### method `get_float(index: number): number`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `float`.

#### method `get_vec3(index: number): Vec3`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Vec3`.

#### method `get_vec4(index: number): Vec4`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Vec4`.

#### method `get_mat4(index: number): Mat4`

Get the output value of the operator at the given `index`.
Returns nil if the output is not a `Mat4`.

#### method `set_float(value: number): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.

#### method `set_vec3(value: Vec3): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.

#### method `set_vec4(value: Vec4): boolean`

Set the value of the operator to the given `value`.
Returns true if the operation was successful.

#### method `plug(from_index: number, to: Node, to_index: number): boolean`

Returns false if cannot plug the output of the operator at `from_index`
to the input of the `to` node at `to_index`.

#### method `unplug_input(index: number)`

Returns false if cannot unplug the input of the operator at `index`.

#### method `unplug_output(index: number)`

Returns false if cannot unplug the output of the operator at `index`.


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

#### method `get_float(name: string): number`

Returns the value of the `float` property with the given `name`.
For example, `get_float("angle")` can be used to get the `angle` property
of a `AxisAngle` rotation transform.

#### method `get_vec3(name: string): Vec3`

Returns the value of the `Vec3` property with the given `name`.
For example, `get_vec3("scale")` can be used to get the `scale` property
of a `Scale` transform.

#### method `get_vec4(name: string): Vec4`

Returns the value of the `Vec4` property with the given `name`.
For example, `get_vec4("quat")` can be used to get the `quat` property
of a `Quat` (quaternion) rotation transform.

#### method `set_float(name: string, value: number)`

Sets the value of the `float` property with the given `name`.
See `get_float` for examples.

#### method `set_vec3(name: string, value: Vec3)`

Sets the value of the `Vec3` property with the given `name`.
See `get_vec3` for examples.

#### method `set_vec4(name: string, value: Vec4)`

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

#### method `push(transform: Transform)`

Pushes the given `transform` to the back of the sequence.

#### method `push(transform: Transform, index: number)`

Inserts the given `transform` at the given `index` in the sequence.

#### method `pop(): Transform`

Removes and returns the last transform in the sequence.


### Workspace API functions

#### function `I3T.print_operator_types()`

Prints all available operator types to the console.

#### function `I3T.print_transform_types()`

Prints all available transform types to the console.

#### function `I3T.get_node(id: number): Node`

Returns nil if the operator with the given `id` does not exist.

#### function `I3T.get_transform(id: number): Transform`

Returns nil if the transform with the given `id` does not exist.

#### function `I3T.delete_node(node: Node)`

Deletes the given `node` from the workspace.
