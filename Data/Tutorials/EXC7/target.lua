local tutorial = I3T.get_tutorial() 
if not tutorial then return end

local step = tutorial:get_current_step() 
step.completed = false 

local target_node = nil
local student_node = nil

local all_nodes = I3T.get_all_nodes()
for _, node in ipairs(all_nodes) do
    local label = node:get_label()
    
    if label == "Target" or label == "target" then
        target_node = node:as_sequence() or node:as_transform() or node:as_operator()
        node:set_render(false) 
        
    elseif label == "Student" or label == "student" then
        student_node = node:as_sequence() or node:as_transform() or node:as_operator()
    end
end

if not target_node then
    print("Error: Could not find a node labeled Target")
end
if not student_node then
    print("Error: Could not find a node labeled Student")
end
if not target_node or not student_node then
    return
end

local timer
timer = I3T.set_timer(0.2, function() 
    if step.completed then 
        I3T.clear_timer(timer) 
        return 
    end

    local target_mat = target_node:get_mat4(1)
    local student_mat = student_node:get_mat4(1)

    if target_mat and student_mat then
        if I3T.matrices_match(student_mat, target_mat) then
            print("SUCCESS: Target acquired")
            student_node:set_highlight(true)
            
            step.completed = true
            I3T.clear_timer(timer)
        end
    end
end)