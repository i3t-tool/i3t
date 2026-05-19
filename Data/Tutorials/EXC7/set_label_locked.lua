local tutorial = I3T.get_tutorial() 
if not tutorial then return end

local existing_nodes = I3T.get_all_nodes()
if existing_nodes then
    for _, node in ipairs(existing_nodes) do
        if not node:is_label_locked() then
            node:set_label_locked(true)
        end
    end
end