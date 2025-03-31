let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/reference/cpp-benchmark.git/memory
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +60 include/memory/pmr_deep_dive.hpp
badd +441 src/memory/pmr_deep_dive.cpp
badd +31 CMakeLists.txt
badd +1 fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//
argglobal
%argdel
$argadd ./
edit src/memory/pmr_deep_dive.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 1resize ' . ((&columns * 119 + 95) / 191)
exe '2resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 2resize ' . ((&columns * 71 + 95) / 191)
exe '3resize ' . ((&lines * 23 + 25) / 50)
argglobal
balt include/memory/pmr_deep_dive.hpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 440 - ((11 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 440
normal! 022|
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
argglobal
if bufexists(fnamemodify("~/reference/cpp-benchmark.git/memory/include/memory/pmr_deep_dive.hpp", ":p")) | buffer ~/reference/cpp-benchmark.git/memory/include/memory/pmr_deep_dive.hpp | else | edit ~/reference/cpp-benchmark.git/memory/include/memory/pmr_deep_dive.hpp | endif
if &buftype ==# 'terminal'
  silent file ~/reference/cpp-benchmark.git/memory/include/memory/pmr_deep_dive.hpp
endif
balt ~/reference/cpp-benchmark.git/memory/src/memory/pmr_deep_dive.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 32 - ((0 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 32
normal! 0
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
argglobal
if bufexists(fnamemodify("fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//", ":p")) | buffer fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory// | else | edit fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory// | endif
if &buftype ==# 'terminal'
  silent file fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//
endif
balt ~/reference/cpp-benchmark.git/memory/include/memory/pmr_deep_dive.hpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr=<<<<<<<<,>>>>>>>>
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 6 - ((5 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 0
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
3wincmd w
exe '1resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 1resize ' . ((&columns * 119 + 95) / 191)
exe '2resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 2resize ' . ((&columns * 71 + 95) / 191)
exe '3resize ' . ((&lines * 23 + 25) / 50)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
