## Tutorial 

### `TutorialElement`

#### attribute `content: string`

Text content of the element, common to all types of elements.

#### function `as_explanation(): Explanation`
#### function `as_headline(): Headline`
#### function `as_task(): Task`
#### function `as_hint(): Hint`
#### function `as_choice_task(): ChoiceTask`
#### function `as_multi_choice_task(): MultiChoiceTask`
#### function `as_input_task(): InputTask`


### `Explanation`

Inherits all `TutorialElement` content.

#### constructor `new(content: string)`


### `Headline`

Inherits all `TutorialElement` content.

#### constructor `new(content: string)`


### `Task`

Inherits all `TutorialElement` content.

#### constructor `new(content: string)`

#### attribute `completed: boolean`

Whether the task is completed. When set to `true` tutorial renders a checkmark before task content.


### `Hint`

Represents a hint button that can be clicked and expanded by the user.
Inherits all `TutorialElement` content.

#### constructor `new(content: string)`

#### attribute `expanded: boolean`

When set to `true` hint content is visible.


### `ChoiceTask`

Question with single choice answers (radio buttons).
Inherits all `TutorialElement` content.

#### constructor `new(question: string, choices: string[], correct_choice: number)`

`correct_index` is the index (0-based) of the correct answer in the `choices` array.

#### attribute `choices: string[]`

#### attribute `correct_choice: number`


### `MultiChoiceTask`

Question with multiple choice answers (checkboxes).

#### constructor `new(question: string, choices: string[], correct_choices: number[])`

`correct_choices` is an array of indexes (0-based) of the correct answers in the `choices` array.

#### attribute `choices: string[]`

#### attribute `correct_choices: number[]`


### `InputTask`

Question with a text input field.

#### constructor `new(question: string, correct_answers: string[])`

#### attribute `correct_answers: string[]`

<hr>

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

#### attribute `content: TutorialElement[]`

List of elements that make up the step content.


### `Tutorial`

#### attribute `header: TutorialHeader`

#### attribute `steps: TutorialStep[]`

#### method `get_current_step(): TutorialStep`

#### method `set_step(index: number)`

Set the current step of the tutorial to the given `index` (1-based)
only if the index is valid.

#### method `push_element(element: TutorialElement): TutorialElement`

Add a new element to the tutorial steps and return it to the caller.

#### method `push_element_at(element: TutorialElement, index: number): TutorialElement`

Add a new element to the tutorial steps at the given `index` (1-based) 
and return it to the caller.

#### method `pop_element(index: number)`
Remove the element at the given `index` (1-based) from the tutorial steps.


### Tutorial API functions

#### function `I3T.get_tutorial()`

Returns the current tutorial object. If no tutorial is active, returns `nil`.
