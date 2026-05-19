local tut = I3T.get_tutorial()
local step = tut:get_current_step()

local tasks = {
    ChoiceTask.new(
        "According to the tutorial, what is the key property of a real orthogonal matrix Q?",
        {
            "Its inverse is equal to its transpose",
            "It is equal to its transpose",
            "Its square is equal to itself"
        },
        0
    ),
    ChoiceTask.new(
        "The tutorial demonstrates that a simple scale matrix is not orthogonal. Why?",
        {
            "Because its column vectors are not perpendicular",
            "Because its column vectors are not normalized",
            "Because the matrix cannot be inverted"
            
        },
        1
    ),
    ChoiceTask.new(
        "What is the name of the process that converts a set of basis vectors into an orthogonal set of basis vectors?",
        {
            "Gram-Schmidt",
            "Orthonormalization",
            "Parseval's Equality"
            
        },
        0
    )
}

step.completed = false

local taskIndex = 1
tut:push_element(tasks[taskIndex])

I3T.set_timer(0.5, function()
    local task = tasks[taskIndex]
    if task == nil then
        step.completed = true
        return
    end
    
    if task.completed and taskIndex <= #tasks then
        taskIndex = taskIndex + 1
        tut:push_element(tasks[taskIndex])
    end
end)