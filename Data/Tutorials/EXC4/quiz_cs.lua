local tut = I3T.get_tutorial()
local step = tut:get_current_step()

local tasks = {
    ChoiceTask.new(
        "Podle tutoriálu, jaká je klíčová vlastnost reálné ortogonální matice Q?",
        {
            "Její inverze se rovná její transpozici",
            "Rovná se své transpozici",
            "Její čtverec se rovná jí samotné"
        },
        0
    ),
    ChoiceTask.new(
        "Tutoriál ukazuje, že jednoduchá scale matice není ortogonální. Proč?",
        {
            "Protože její sloupcové vektory nejsou kolmé",
            "Protože její sloupcové vektory nejsou normalizované",
            "Protože matici nelze invertovat"
        },
        1
    ),
    ChoiceTask.new(
        "Jak se jmenuje proces, který převádí množinu bázových vektorů na ortogonální množinu bázových vektorů?",
        {
            "Gram-Schmidt",
            "Ortonormalizace",
            "Parsevalova rovnost"
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