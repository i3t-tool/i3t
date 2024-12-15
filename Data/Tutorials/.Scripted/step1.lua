local initial_nodes_count = #get_all_nodes()

local step = I3T.get_tutorial():get_current_step()
step.completed = false

local timer
timer = set_timer(0.5, function()
    local new_nodes = get_all_nodes()

    if #new_nodes > 0 then
        local found_node

        for i = 1 + initial_nodes_count, #new_nodes do
            local node = new_nodes[i]
            if node:type() == "Scale" then
                found_node = node
                break
            end
        end

        if found_node then
            print("Found a new Scale node! Task complete!")
            step.completed = true
            clear_timer(timer)
        end
    end
end)
