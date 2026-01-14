local M = {}

local function extract_metadata()
    local lines = vim.api.nvim_buf_get_lines(0, 0, 20, false)
    for _, line in ipairs(lines) do
        local match = line:match("//%s*URL:%s*(https://cses.fi/[^%s]+)")
        if match then
            local course_id, task_id = match:match("cses.fi/([^/]+)/task/([^/]+)")
            if course_id and task_id then
                -- Clean task_id
                task_id = task_id:gsub("[/?].*$", "")
                return course_id, task_id
            end
        end
    end
    return nil, nil
end

local function run_job(cmd, title)
    vim.fn.jobstart(cmd, {
        stdout_buffered = true,
        stderr_buffered = true,
        on_stdout = function(_, data)
            if data then
				vim.notify(title .. ": " .. table.concat(data,"\n"), vim.log.levels.INFO)
            end
        end,
        on_stderr = function(_, data)
            if data then
                for _, line in ipairs(data) do
                    if line ~= "" then
                        vim.notify(title .. " Error: " .. line, vim.log.levels.ERROR)
                    end
                end
            end
        end,
        on_exit = function(_, exit_code)
            if exit_code == 0 then
				-- do nothing
            else
                vim.notify(title .. " failed (exit code " .. exit_code .. ")", vim.log.levels.ERROR)
            end
        end
    })
end

local function run_job_capture(cmd, title, on_success)
    local stdout_lines = {}
    local stderr_lines = {}
    vim.notify(title .. ": Running...", vim.log.levels.INFO)
    
    vim.fn.jobstart(cmd, {
        stdout_buffered = true,
        stderr_buffered = true,
        on_stdout = function(_, data)
            if data then
                for _, line in ipairs(data) do
                    table.insert(stdout_lines, line)
                end
            end
        end,
        on_stderr = function(_, data)
            if data then
                for _, line in ipairs(data) do
                    if line ~= "" then
                        table.insert(stderr_lines, line)
                    end
                end
            end
        end,
        on_exit = function(_, exit_code)
            if exit_code == 0 then
                if on_success then
                    on_success(stdout_lines)
                end
            else
                vim.notify(title .. " failed (exit code " .. exit_code .. ")", vim.log.levels.ERROR)
                if #stderr_lines > 0 then
                    vim.notify(table.concat(stderr_lines, "\n"), vim.log.levels.ERROR)
                end
            end
        end
    })
end

local function run_job_silent(cmd, title)
    vim.notify(title .. ": Running...", vim.log.levels.INFO)
    vim.fn.jobstart(cmd, {
        stdout_buffered = true,
        stderr_buffered = true,
        on_stdout = function(_, data)
            -- do nothing
        end,
        on_stderr = function(_, data)
            if data then
                for _, line in ipairs(data) do
                    if line ~= "" then
                        vim.notify(title .. " Error: " .. line, vim.log.levels.ERROR)
                    end
                end
            end
        end,
        on_exit = function(_, exit_code)
            if exit_code == 0 then
                -- do nothing
            else
                vim.notify(title .. " failed (exit code " .. exit_code .. ")", vim.log.levels.ERROR)
            end
        end
    })
end

function M.submit()
    local file_path = vim.fn.expand("%:p")
    local course_id, task_id = extract_metadata()

    if not course_id or not task_id then
        vim.notify("CSES: Could not find/parse CSES URL in the first 20 lines.", vim.log.levels.ERROR)
        return
    end

    local cmd = string.format("cses-cli submit %s -c %s -t %s", vim.fn.shellescape(file_path), vim.fn.shellescape(course_id), vim.fn.shellescape(task_id))
    run_job_silent(cmd, "CSES Submit")
end

function M.status()
    local course_id, task_id = extract_metadata()
    local cmd
    if course_id and task_id then
        cmd = string.format("cses-cli submission -c %s -t %s", vim.fn.shellescape(course_id), vim.fn.shellescape(task_id))
    else
        cmd = "cses-cli submission"
    end
    
    run_job_capture(cmd, "CSES Status", function(lines)
        local info = {}
        local tests = {}
        local in_tests = false
        
        for _, line in ipairs(lines) do
            -- Check for the start of the test results section
            if line:match("^Test results") then
                in_tests = true
            elseif in_tests then
                table.insert(tests, line)
            else
                table.insert(info, line)
            end
        end
        
        -- Clean up trailing empty lines
        while #info > 0 and info[#info] == "" do
            table.remove(info)
        end
        while #tests > 0 and tests[#tests] == "" do
            table.remove(tests)
        end
        -- Remove leading empty lines from tests if any
        while #tests > 0 and tests[1] == "" do
            table.remove(tests, 1)
        end

        require("cses.ui").show_status(info, tests)
    end)
end

return M
