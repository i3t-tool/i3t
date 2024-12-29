local initial_nodes_count = #I3T.get_all_nodes()

local step = I3T.get_tutorial():get_current_step()
step.completed = false

local timer
timer = I3T.set_timer(0.5, function()
    local new_nodes = I3T.get_all_nodes()

    if #new_nodes > 0 then
        local found_node = I3T.find_node(initial_nodes_count, #new_nodes, function (node)
            print("???")
            return node:get_keyword() == "Scale"
        end)

        if found_node then
            print("Found a new Scale node! Task complete!")
            step.content[#step.content]:as_task().completed = true
            step.completed = true
            I3T.clear_timer(timer)

            I3T.get_tutorial():push_element(Explanation.new("Pomocí skriptovacího rozhraní lze i vytvářet nové elementy v aktivním tutoriálu. To si předvedeme v dalším kroku."))
        end
    end
end)
