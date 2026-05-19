local tutorial = I3T.get_tutorial() 
if not tutorial then return end

local step = tutorial:get_current_step() 
step.completed = false 

local timer
timer = I3T.set_timer(0.5, function() 
    if step.completed then 
        I3T.clear_timer(timer) 
        return 
    end

    local all_nodes = I3T.get_all_nodes()
    if not all_nodes then return end

    for _, node in ipairs(all_nodes) do
        local label = node:get_label()
        
        if node:as_sequence() and (label == "sequence" or label == "Sequence") then
            local seq_node = node:as_sequence()
            local target_transform = seq_node:get_transform("MyRotation")
            
            if target_transform then
                print("SUCCESS: Found 'MyRotation' inside the Sequence")
                step.completed = true
                I3T.clear_timer(timer)
            end
            break 
        end
    end
end)