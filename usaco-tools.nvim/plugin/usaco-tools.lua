if vim.g.loaded_usaco_tools then
    return
end
vim.g.loaded_usaco_tools = 1

require("usaco").setup()

vim.api.nvim_create_user_command("UsacoRestart", function()
    require("usaco").restart()
end, {})
