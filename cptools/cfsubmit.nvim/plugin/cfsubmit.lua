if vim.g.loaded_cfsubmit then
  return
end
vim.g.loaded_cfsubmit = 1

local cfsubmit = require("cfsubmit")

vim.api.nvim_create_user_command("CFSubmit", function()
  cfsubmit.submit()
end, {})

vim.api.nvim_create_user_command("CFStatus", function()
  cfsubmit.status()
end, {})
