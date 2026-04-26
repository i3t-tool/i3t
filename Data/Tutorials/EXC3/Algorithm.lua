local tutorial = I3T.get_tutorial() 
if not tutorial then
    print("ERR: No active tutorial found.")
    return
end

local step = tutorial:get_current_step() 
step.completed = false 

local seq_node = nil

for _, node in ipairs(I3T.get_all_nodes()) do
    if node:as_sequence() and node:get_label() == "Algorithm" then
        seq_node = node:as_sequence()  
        break 
    end
end

if not seq_node then
    print("ERR: Could not find Sequence node labeled 'Algorithm'")
    return
end

local task = nil
for i = #step.content, 1, -1 do 
    local task_attempt = step.content[i]:as_task() 
    if task_attempt then
        task = task_attempt
        break
    end
end

if not task then
    print("ERR: No task found in this step.")
    return
end
task.completed = false 

local right_seq = {
    "START", 
    "1 step", 
    "turn right", 
    "2 steps", 
    "turn left", 
    "1 step",
    "turn right",
    "1 step", 
    "turn right", 
    "1 step", 
    "turn left",
    "END"
}

local timer
timer = I3T.set_timer(0.5, function() 
    
    if step.completed then 
        I3T.clear_timer(timer) 
        return
    end

    local trrrrr = sequence_node:get_transforms()
    local is_seq_compl = (#trrrrr == #right_seq)

    for i, transform_node in ipairs(trrrrr) do
        local exp_label = right_seq[i]
        local label = transform_node:get_label()

        if not exp_label or label ~= exp_label then
            sequence_node:pop(i - 1) 
            
            local seq_pos = sequence_node:get_position()
            local rand_x = math.random(-150, 150)
            local rand_y = math.random(150, 300)

            transform_node:set_position(Vec2.new(seq_pos.x + rand_x, seq_pos.y + rand_y))
            
            print("Wrong node at index", i - 1, "Expected:", exp_label, "Got:", label)
            is_seq_compl = false
        
            break
        end
    end

    if is_seq_compl then
        print("Task complete!")
        task.completed = true
        step.completed = true
        I3T.clear_timer(timer)
    end
    
end)

