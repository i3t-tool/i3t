local initial_nodes_count = #get_all_nodes()

local timer
timer = set_timer(500, function()
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
            clear_timer(timer)
        end
    end
end)
