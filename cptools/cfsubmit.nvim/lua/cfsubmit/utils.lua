local M = {}

function M.extract_metadata()
    -- Read the first 5 lines of the current buffer
    local lines = vim.api.nvim_buf_get_lines(0, 0, 5, false)
    local url = nil
    
    for _, line in ipairs(lines) do
        -- Look for // URL: ...
        local match = line:match("//%s*URL:%s*(.+)")
        if match then
            url = match:gsub("^%s*(.-)%s*$", "%1") -- trim whitespace
            break
        end
    end

    if not url then
        return nil, "No URL found in the first 5 lines."
    end

    -- Parse URL to get Problem ID
    -- Support formats:
    -- https://codeforces.com/contest/1234/problem/A
    -- https://codeforces.com/problemset/problem/1234/A
    -- mirror.codeforces.com ...
    
    local contest_id, index = url:match("contest/(%d+)/problem/(%w+)")
    if not contest_id then
        contest_id, index = url:match("problemset/problem/(%d+)/(%w+)")
    end

    if contest_id and index then
        return contest_id .. index, nil
    end

    return nil, "Could not parse Problem ID from URL: " .. url
end

return M
