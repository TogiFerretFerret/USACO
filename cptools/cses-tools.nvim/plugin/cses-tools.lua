if vim.g.loaded_cses_tools then
    return
end
vim.g.loaded_cses_tools = 1

vim.api.nvim_create_user_command("CsesSubmit", function()
    require("cses").submit()
end, { desc = "Submit current file to CSES" })

vim.api.nvim_create_user_command("CsesStatus", function()
    require("cses").status()
end, { desc = "Check status of CSES submission" })