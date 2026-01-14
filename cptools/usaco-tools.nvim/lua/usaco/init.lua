local M = {}
local loop = vim.loop or vim.uv

local server_pid = nil

function M.setup()
    -- Configuration or other setup if needed
end

function M.start_server()
    if server_pid then
        print("USACO Server is already running")
        return
    end

    -- Path to the python script
    local script_path = vim.fn.stdpath('data') .. '/lazy/usaco-tools.nvim/server/main.py'
    -- Note: The above path assumes typical lazy.nvim install. 
    -- For local development (current directory), we might need to adjust.
    
    -- Let's try to find it relative to this file if possible, or assume current working dir if that's how user runs it.
    -- Given the user is in /home/river/USACO and the plugin is a subdir:
    local cwd = vim.fn.getcwd()
    local potential_script = cwd .. '/usaco-tools.nvim/server/main.py'
    
    if vim.fn.filereadable(potential_script) == 1 then
        script_path = potential_script
    else
        -- Fallback to plugin directory detection
        local str = debug.getinfo(1, "S").source:sub(2)
        local root = str:match("(.*/)")
        -- root is .../usaco-tools.nvim/lua/usaco/
        script_path = root .. "../../server/main.py"
    end

    -- Start Server
    -- We use python3
    local stdout = loop.new_pipe(false)
    local stderr = loop.new_pipe(false)

    local handle, pid
    
    handle, pid = loop.spawn("python3", {
        args = {script_path},
        cwd = cwd, -- Run in project root so relative paths work
        stdio = {nil, stdout, stderr}
    }, function(code, signal)
        print("USACO Server exited", code, signal)
        server_pid = nil
    end)

    if handle then
        server_pid = pid
        print("USACO Server started with PID", pid)
        
        loop.read_start(stdout, vim.schedule_wrap(function(err, data)
            if data then
                -- Optional: Log server output
                -- print("Server:", data)
            end
        end))
        
        loop.read_start(stderr, vim.schedule_wrap(function(err, data)
            if data then
                print("Server Error:", data)
            end
        end))
        
        -- Cleanup on exit
        vim.api.nvim_create_autocmd("VimLeavePre", {
            callback = function()
                if server_pid then
                    loop.kill(server_pid, 15) -- SIGTERM
                end
            end
        })
    else
        print("Failed to start USACO Server")
    end
end

function M.stop_server()
    if server_pid then
        loop.kill(server_pid, 15)
        server_pid = nil
        print("USACO Server stopped")
    else
        print("USACO Server is not running")
    end
end


function M.restart()
    M.stop_server()
    vim.defer_fn(M.start_server, 1000)
end

return M
