local step = I3T.get_tutorial():get_current_step()
step.completed = false

local required_keywords = {
    Scale = false
}

local function check_all_nodes()
    local all_nodes = I3T.get_all_nodes()
    for _, node in ipairs(all_nodes) do
        local keyword = node:get_keyword()
        if required_keywords[keyword] ~= nil then
            required_keywords[keyword] = true
        end
    end
end

local function check_completion()
    for _, found in pairs(required_keywords) do
        if not found then return false end
    end
    return true
end

-- First immediate scan
check_all_nodes()
if check_completion() then
    step.content[#step.content]:as_task().completed = true
    step.completed = true
    return
end

-- Continue polling if not all were found yet
local timer
timer = I3T.set_timer(0.5, function()
    check_all_nodes()

    if check_completion() then
        print("All required nodes found! Task complete!")
        step.content[#step.content]:as_task().completed = true
        step.completed = true
        I3T.clear_timer(timer)
    end
end)
