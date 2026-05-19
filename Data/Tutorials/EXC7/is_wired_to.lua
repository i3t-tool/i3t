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

    local seq_node = nil
    local model_node = nil

    local all_nodes = I3T.get_all_nodes()
    if not all_nodes then return end

    for _, node in ipairs(all_nodes) do
        local label = node:get_label()
        
        if node:as_sequence() and (label == "sequence" or label == "Sequence") then
            seq_node = node:as_sequence()
        end
        
        if node:as_model() and (label == "model" or label == "Model") then
            model_node = node:as_model()
        end
    end

    if seq_node and model_node then
        if seq_node:is_wired_to(model_node) then
            print("SUCCESS: Sequence is wired to the Model")
            step.completed = true
            I3T.clear_timer(timer)
        end
    end
end)