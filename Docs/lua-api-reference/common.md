## Types

### `Vec2`

Mapped `ImVec2` type to Lua.

#### constructor `new(x: number, y: number)`

#### attribute `x: number`

#### attribute `y: number`

#### metamethod `__tostring(): string`

Returns a string representation of the vector in the form `[x, y]`.

### `Vec3`

Mapped `glm::vec3` type to Lua.

#### constructor `new(x: number, y: number, z: number)`

#### attribute `x: number`

#### attribute `y: number`

#### attribute `z: number`

#### metamethod `__tostring(): string`

Returns a string representation of the vector in the form `[x, y, z]`.


### `Vec4`

Mapped `glm::vec4` type to Lua.

#### constructor `new(x: number, y: number, z: number, w: number)`

#### attribute `x: number`

#### attribute `y: number`

#### attribute `z: number`

#### attribute `w: number`

#### metamethod `__tostring(): string`

Returns a string representation of the vector in the form `[x, y, z, w]`.


### `Mat4`

Mapped `glm::mat4` type to Lua.

#### constructor `new(col1: Vec4, col2: Vec4, col3: Vec4, col4: Vec4)`

#### attribute `x: Vec4`

#### attribute `y: Vec4`

#### attribute `z: Vec4`

#### attribute `w: Vec4`

#### metamethod `__tostring(): string`

Returns a string representation of the vector in the form 
`[[col1], [col2], [col3], [col4]]`.


### `Timer`

Utility class to schedule functions to be called after a delay.
Can be created with `I3T.set_timer` function.
See `Timer` class in C++ code for more details.


### I3T API functions

#### function `I3T.load_script(script: string): boolean`

Executes the given `script` string as Lua code.

#### function `I3T.load_script_from(path: string): boolean`

Executes the Lua code from the file at the given `path`.

<hr>

#### function `I3T.set_timer(intervalSeconds: number, callback: function): Timer`

Creates a new timer that will call the given `callback` function after
the given `intervalSeconds` delay.

#### function `I3T.clear_timer(timer: Timer)`

Removes the given `timer` from the scheduler.
