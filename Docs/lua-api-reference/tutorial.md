## Tutorial 

### `TutorialHeader`

#### attribute `title: string`

Title of the tutorial.

#### attribute `description: string`

Description of the tutorial.


### `TutorialStep`

#### attribute `completed: boolean`

Whether the step is completed. 
Each step is considered completed by default.
When set to `false` user cannot proceed to the next step.


### `Tutorial`

#### attribute `header: TutorialHeader`

#### method `set_step(index: number)`

Set the current step of the tutorial to the given `index` (1-based)
only if the index is valid.

#### method `show_hint(message: string): number`

Show a hint button with the given `message` that can be clicked by the user.
Returns the index (1-based) of the hint element within the tutorial steps.

#### method `pop_element(index: number)`
Remove the element at the given `index` (1-based) from the tutorial steps.
