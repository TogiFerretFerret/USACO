# cfsubmit.nvim

A simple Neovim plugin to submit Codeforces solutions using a local python bridge.

## Prerequisites

1.  Python 3 installed.
2.  Selenium and Flask installed (`pip install selenium flask beautifulsoup4`).
3.  Chromedriver installed and compatible with your Chrome/Chromium version.
4.  The `submitter.py` script must be running.

## Installation

Using [packer.nvim](https://github.com/wbthomason/packer.nvim):

```lua
use {
  '~/USACO/cfsubmit.nvim', -- Adjust path if necessary
  requires = { 'nvim-lua/plenary.nvim' } -- Optional, but good to have
}
```

Using [lazy.nvim](https://github.com/folke/lazy.nvim):

```lua
{
  dir = "~/USACO/cfsubmit.nvim",
}
```

## Usage

1.  Start the python submitter server:
    ```bash
    python3 cf_submit/submitter.py
    ```
    (Ensure you are logged into Codeforces in the browser instance that opens).

2.  Open a `.cpp` file with the required metadata header:
    ```cpp
    // Problem: Problem Name
    // Judge: Codeforces
    // URL: https://codeforces.com/contest/1234/problem/A
    ```

3.  Run `:CFSubmit` to submit the current file.

4.  Run `:CFStatus` to check the status of the last submission.
