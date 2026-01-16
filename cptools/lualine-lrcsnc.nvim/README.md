# lualine-lrcsnc.nvim

A Neovim plugin to display lyrics from `lrcsnc` in your statusline via [lualine.nvim](https://github.com/nvim-lualine/lualine.nvim).

## Requirements

- `lrcsnc` installed and in your PATH.
- `lualine.nvim`

## Installation

### Using [lazy.nvim](https://github.com/folke/lazy.nvim)

```lua
{
  "dir": "~/USACO/cptools/lualine-lrcsnc.nvim", -- Or wherever you store local plugins
  dependencies = { "nvim-lualine/lualine.nvim" },
  config = function()
    require("lrcsnc").setup({
      cmd = { "lrcsnc", "waybar" }, -- Default
      max_length = 60,
      icon = "♪"
    })
  end
}
```

## Usage

Add the component to your lualine config:

```lua
require('lualine').setup {
  sections = {
    lualine_x = {
      require('lrcsnc').component,
      -- other components...
    }
  }
}
```
