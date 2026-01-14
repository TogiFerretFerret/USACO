local M = {}

---@class CsesConfig
---@field popup_ui { total_width: number, total_height: number, layout: table }
---@field floating_border string
---@field floating_border_highlight string

local default_config = {
	popup_ui = {
		total_width = 0.8,
		total_height = 0.8,
		layout = {
			{ 0.35, "info" },
			{ 0.65, "tests" },
		},
	},
	floating_border = "rounded",
	floating_border_highlight = "FloatBorder",
}

local function get_ui_size()
	return vim.o.columns, vim.o.lines
end

local function compute_layout(config)
	local sizes = { info = {}, tests = {} }
	local positions = { info = {}, tests = {} }

	local function rec_compute_layout(layout, vertical, width, height, col, row)
		if type(layout) == "string" then
			if sizes[layout] then
				sizes[layout].width = width - 2
				sizes[layout].height = height - 2
				positions[layout].col = col
				positions[layout].row = row
			end
			return
		end

		local layout_width = 0
		for _, l in ipairs(layout) do
			layout_width = layout_width + l[1]
		end

		local current_size = 0
		local dimension = vertical and height or width
		for i, l in ipairs(layout) do
			local popup_size = math.floor(dimension * l[1] / layout_width + 0.5)
			if i == #layout then
				popup_size = dimension - current_size
			end

			if vertical then
				rec_compute_layout(l[2], not vertical, width, popup_size, col, row + current_size)
			else
				rec_compute_layout(l[2], not vertical, popup_size, height, col + current_size, row)
			end
			current_size = current_size + popup_size
		end
	end

	local vim_width, vim_height = get_ui_size()
	local total_width = math.floor(vim_width * config.popup_ui.total_width + 0.5)
	local total_height = math.floor(vim_height * config.popup_ui.total_height + 0.5)
	local initial_col = math.floor((vim_width - total_width) / 2 + 0.5)
	local initial_row = math.floor((vim_height - total_height) / 2 + 0.5)

	-- Start with vertical=true to split top/bottom
	rec_compute_layout(config.popup_ui.layout, true, total_width, total_height, initial_col, initial_row)
	return sizes, positions
end

function M.show_status(info_lines, test_lines)
	local has_nui, Popup = pcall(require, "nui.popup")
	if not has_nui then
		vim.notify("CSES Tools: nui.nvim is required for the status UI.", vim.log.levels.ERROR)
		-- Fallback to notify
		vim.notify(table.concat(info_lines, "\n") .. "\n" .. table.concat(test_lines, "\n"), vim.log.levels.INFO)
		return
	end

	local config = default_config
	local sizes, positions = compute_layout(config)

	local popup_options = {
		enter = true,
		focusable = true,
		zindex = 50,
		relative = "editor",
		border = {
			style = config.floating_border,
			highlight = config.floating_border_highlight,
			text = { top_align = "center" },
		},
		buf_options = {
			modifiable = true,
			readonly = false,
		},
		win_options = {
			winhighlight = "Normal:NormalFloat,FloatBorder:FloatBorder",
			winblend = 0,
		},
	}

	local windows = {}

	-- Info Window
	local info_opts = vim.deepcopy(popup_options)
	info_opts.border.text.top = " Submission Details "
	info_opts.position = positions.info
	info_opts.size = sizes.info
	windows.info = Popup(info_opts)

	-- Tests Window
	local tests_opts = vim.deepcopy(popup_options)
	tests_opts.border.text.top = " Test Results "
	tests_opts.position = positions.tests
	tests_opts.size = sizes.tests
	windows.tests = Popup(tests_opts)

	-- Mount
	windows.info:mount()
	windows.tests:mount()

	-- Set content
	vim.api.nvim_buf_set_lines(windows.info.bufnr, 0, -1, false, info_lines)
	vim.api.nvim_buf_set_lines(windows.tests.bufnr, 0, -1, false, test_lines)

	-- Highlights
	vim.cmd([[
		highlight default CsesAccepted guifg=#98c379 gui=bold
		highlight default CsesError guifg=#e06c75 gui=bold
		highlight default CsesLabel guifg=#61afef gui=bold
		highlight default CsesInfo guifg=#e5c07b
	]])

	local function highlight_buf(bufnr)
		local lines = vim.api.nvim_buf_get_lines(bufnr, 0, -1, false)
		for i, line in ipairs(lines) do
			-- Labels
			local label_match = line:match("^([^:]+):")
			if label_match then
				vim.api.nvim_buf_add_highlight(bufnr, -1, "CsesLabel", i - 1, 0, #label_match + 1)
			end

			-- Verdicts
			if line:find("ACCEPTED") then
				local s, e = line:find("ACCEPTED")
				vim.api.nvim_buf_add_highlight(bufnr, -1, "CsesAccepted", i - 1, s - 1, e)
			elseif line:find("WRONG ANSWER") or line:find("TIME LIMIT EXCEEDED") or line:find("MEMORY LIMIT EXCEEDED") or line:find("RUNTIME ERROR") or line:find("COMPILE ERROR") then
				for _, pattern in ipairs({"WRONG ANSWER", "TIME LIMIT EXCEEDED", "MEMORY LIMIT EXCEEDED", "RUNTIME ERROR", "COMPILE ERROR"}) do
					local s, e = line:find(pattern)
					if s then
						vim.api.nvim_buf_add_highlight(bufnr, -1, "CsesError", i - 1, s - 1, e)
						break
					end
				end
			end
		end
	end

	highlight_buf(windows.info.bufnr)
	highlight_buf(windows.tests.bufnr)

	-- Make buffers readonly and unmodifiable after writing
	vim.api.nvim_buf_set_option(windows.info.bufnr, "modifiable", false)
	vim.api.nvim_buf_set_option(windows.info.bufnr, "readonly", true)
	vim.api.nvim_buf_set_option(windows.tests.bufnr, "modifiable", false)
	vim.api.nvim_buf_set_option(windows.tests.bufnr, "readonly", true)

	-- Keymaps to close
	local function map_quit(bufnr)
		vim.keymap.set("n", "q", function()
			for _, win in pairs(windows) do
				win:unmount()
			end
		end, { buffer = bufnr })
		vim.keymap.set("n", "<Esc>", function()
			for _, win in pairs(windows) do
				win:unmount()
			end
		end, { buffer = bufnr })
	end

	map_quit(windows.info.bufnr)
	map_quit(windows.tests.bufnr)
	
	-- Focus the info window
	vim.api.nvim_set_current_win(windows.info.winid)
end

return M
