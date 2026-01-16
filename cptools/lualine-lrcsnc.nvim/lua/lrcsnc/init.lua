local M = {}

local current_lyrics = ""
local job = nil
local stdout = nil

-- Default configuration
local config = {
    cmd = { "lrcsnc" }, -- Command to run
    max_length = 40,              -- Max length of lyrics to display
    icon = "♪",                   -- Icon to show before lyrics
}

local function update_lyrics(text)
    if text ~= current_lyrics then
        current_lyrics = text
        -- Refresh lualine to show the new lyrics immediately
        -- Use pcall to avoid errors if lualine isn't loaded yet
        pcall(function()
            require("lualine").refresh({
                scope = "window",
                place = { "statusline" },
            })
        end)
    end
end

local function on_stdout(err, data)
    if err then
        -- silently fail on error
        return
    end
    if data then
        -- Data might come in chunks, but usually waybar mode outputs one JSON object per line.
        -- For robustness, we process each line.
        for line in data:gmatch("[^\r\n]+") do
            if line ~= "" then
                local status, decoded = pcall(vim.json.decode, line)
                if status and decoded and decoded.text then
                    vim.schedule(function()
                        update_lyrics(decoded.text)
                    end)
                end
            end
        end
    end
end

function M.setup(opts)
    config = vim.tbl_deep_extend("force", config, opts or {})

    -- Clean up previous job if it exists
    if job then
        if not job:is_closing() then job:close() end
        job = nil
    end
    if stdout then
        if not stdout:is_closing() then stdout:close() end
        stdout = nil
    end

    stdout = vim.loop.new_pipe(false)
    
    local handle, pid
    handle, pid = vim.loop.spawn(config.cmd[1], {
        args = { unpack(config.cmd, 2) },
        stdio = { nil, stdout, nil },
    }, function(code, signal)
        -- Process exited
        stdout:close()
        job:close()
        job = nil
        stdout = nil
    end)

    job = handle

    if job then
        stdout:read_start(on_stdout)
    else
        vim.notify("lualine-lrcsnc: Failed to spawn command " .. config.cmd[1], vim.log.levels.ERROR)
    end
end

-- Function to be used in lualine
function M.component()
    if current_lyrics == "" then
        return ""
    end
    
    local text = current_lyrics
    if #text > config.max_length then
        text = text:sub(1, config.max_length - 3) .. "..."
    end
    
    if config.icon and config.icon ~= "" then
        return config.icon .. " " .. text
    end
    return text
end

return M
