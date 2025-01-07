local tut = I3T.get_tutorial()
local step = tut:get_current_step()

local tasks = {
    ChoiceTask.new(
        "Kolik je 1 + 1?",
        {
            "1",
            "2",
            "3",
            "4"
        },
        1
    ),
    MultiChoiceTask.new(
        "Která z následujících čísel jsou prvočísla?",
        {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"
        },
        {1, 2, 4, 6}
    ),
    InputTask.new(
        "Jak se jmenuje hlavní město České republiky?",
        {"Praha"}
    )
}

step.completed = false

local taskIndex = 1
tut:push_element(tasks[taskIndex])

I3T.set_timer(0.5, function()
    local task = tasks[taskIndex]
    if task == nil then
        -- At the end of the task list, all tasks are completed.
        step.completed = true
        return
    end

    -- Set next task on completion.
    if task.completed and taskIndex <= #tasks then
        taskIndex = taskIndex + 1
        tut:push_element(tasks[taskIndex])
    end
end)
