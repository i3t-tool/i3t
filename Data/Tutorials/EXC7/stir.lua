local tutorial = I3T.get_tutorial() 
if not tutorial then return end

local step = tutorial:get_current_step() 
step.completed = false 

if I3T.workspace and I3T.workspace.stir then
    I3T.workspace.stir()
end

local timer
timer = I3T.set_timer(0.5, function() 
    if step.completed then 
        I3T.clear_timer(timer) 
        return 
    end

    local all_nodes = I3T.get_all_nodes()
    if not all_nodes then return end

    local translate_node = nil
    local sequence_node = nil

    for _, node in ipairs(all_nodes) do
        local keyword = node:get_keyword()
        
        if keyword == "Translation" then
            translate_node = node
        elseif keyword == "Sequence" then
            sequence_node = node
        end
    end
    
    step.completed = true
    I3T.clear_timer(timer)
     
end)