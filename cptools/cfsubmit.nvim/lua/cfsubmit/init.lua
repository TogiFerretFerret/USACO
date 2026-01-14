local M = {}
local utils = require('cfsubmit.utils')

local SERVER_URL = "http://localhost:5000"

function M.submit()
    local problem_id, err = utils.extract_metadata()
    if not problem_id then
        vim.notify("CFSubmit: " .. (err or "Unknown error"), vim.log.levels.ERROR)
        return
    end

    -- Get full buffer content
    local lines = vim.api.nvim_buf_get_lines(0, 0, -1, false)
    local code = table.concat(lines, "\n")

    vim.notify("CFSubmit: Submitting problem " .. problem_id .. "...", vim.log.levels.INFO)

    -- Prepare JSON payload
    local json_payload = vim.fn.json_encode({
        problem_id = problem_id,
        code = code
    })

    -- Write payload to a temporary file to safely pass to curl
    local tmp_file = vim.fn.tempname()
    local f = io.open(tmp_file, "w")
    if f then
        f:write(json_payload)
        f:close()
    else
        vim.notify("CFSubmit: Could not write temp file.", vim.log.levels.ERROR)
        return
    end

    -- Construct curl command
    local cmd = string.format(
        "curl -s -X POST -H 'Content-Type: application/json' -d @%s %s/submit",
        tmp_file, SERVER_URL
    )

    vim.fn.jobstart(cmd, {
        on_exit = function(job_id, exit_code, event) 
            os.remove(tmp_file)
            if exit_code ~= 0 then
                vim.schedule(function()
                    vim.notify("CFSubmit: Submission failed (curl exit code " .. exit_code .. ")", vim.log.levels.ERROR)
                end)
            end
        end,
        on_stdout = function(_, data, _) 
             if data then
                 local output = table.concat(data, "")
                 if output ~= "" then
                     vim.schedule(function()
                         -- Try to parse JSON to see if success
                         local ok, decoded = pcall(vim.fn.json_decode, output)
                         if ok then
                             if decoded.status == "error" then
                                 vim.notify("CFSubmit Error: " .. (decoded.message or "Unknown"), vim.log.levels.ERROR)
                             elseif decoded.status == "success" then
                                 vim.notify("CFSubmit: Submitted! ID: " .. (decoded.submission_id or "?"), vim.log.levels.INFO)
                             end
                         end
                     end)
                 end
             end
        end
    })
end

function M.status()
    local cmd = string.format("curl -s %s/status", SERVER_URL)
    
    vim.fn.jobstart(cmd, {
        on_stdout = function(_, data, _) 
            if data then
                 local output = table.concat(data, "")
                 if output ~= "" then
                     vim.schedule(function()
                         local ok, decoded = pcall(vim.fn.json_decode, output)
                         if ok then
                             if decoded.status == "success" then
                                 local msg = string.format(
                                     "Problem: %s\nVerdict: %s\nTime: %s\nMemory: %s",
                                     decoded.problem, decoded.verdict, decoded.time_used, decoded.memory_used
                                 )
                                 vim.notify(msg, vim.log.levels.INFO)
                             else
                                 vim.notify("CFStatus: " .. (decoded.message or "Unknown error"), vim.log.levels.WARN)
                             end
                         end
                     end)
                 end
            end
        end
    })
end

return M
