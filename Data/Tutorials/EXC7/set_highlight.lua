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
        
        if node:as_model() and (label == "model" or label == "Model") then
            local model_node = node:as_model()
            
            print("SUCCESS: Highlighting the model node")
            model_node:set_highlight(true)
            
            step.completed = true
            I3T.clear_timer(timer)
            break 
        end
    end
end)