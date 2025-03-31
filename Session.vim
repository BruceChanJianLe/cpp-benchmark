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
badd +428 src/memory/pmr_deep_dive.cpp
badd +1 fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//
badd +41 benchmark/pmr_performance.cpp
badd +146 benchmark/pmr_multi_threads.cpp
badd +7 benchmark/CMakeLists.txt
badd +0 ~/reference/cpp-benchmark.git/memory
badd +1 ~/reference/cpp-benchmark.git/worktrees/memory/COMMIT_EDITMSG
argglobal
%argdel
$argadd ~/reference/cpp-benchmark.git/memory
edit src/memory/pmr_deep_dive.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
2wincmd k
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
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
exe 'vert 1resize ' . ((&columns * 95 + 95) / 191)
exe '2resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 2resize ' . ((&columns * 95 + 95) / 191)
exe '3resize ' . ((&lines * 11 + 25) / 50)
exe '4resize ' . ((&lines * 11 + 25) / 50)
argglobal
balt benchmark/CMakeLists.txt
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
let s:l = 428 - ((15 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 428
normal! 061|
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
argglobal
if bufexists(fnamemodify("~/reference/cpp-benchmark.git/memory/benchmark/pmr_performance.cpp", ":p")) | buffer ~/reference/cpp-benchmark.git/memory/benchmark/pmr_performance.cpp | else | edit ~/reference/cpp-benchmark.git/memory/benchmark/pmr_performance.cpp | endif
if &buftype ==# 'terminal'
  silent file ~/reference/cpp-benchmark.git/memory/benchmark/pmr_performance.cpp
endif
balt ~/reference/cpp-benchmark.git/memory/benchmark/pmr_multi_threads.cpp
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
let s:l = 69 - ((8 * winheight(0) + 11) / 23)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 69
normal! 016|
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
argglobal
if bufexists(fnamemodify("fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//", ":p")) | buffer fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory// | else | edit fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory// | endif
if &buftype ==# 'terminal'
  silent file fugitive:///home/developer/reference/cpp-benchmark.git/worktrees/memory//
endif
balt ~/reference/cpp-benchmark.git/memory/benchmark/CMakeLists.txt
setlocal fdm=manual
setlocal fde=0
setlocal fmr=<<<<<<<<,>>>>>>>>
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 9 - ((8 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 9
normal! 0
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
argglobal
if bufexists(fnamemodify("~/reference/cpp-benchmark.git/worktrees/memory/COMMIT_EDITMSG", ":p")) | buffer ~/reference/cpp-benchmark.git/worktrees/memory/COMMIT_EDITMSG | else | edit ~/reference/cpp-benchmark.git/worktrees/memory/COMMIT_EDITMSG | endif
if &buftype ==# 'terminal'
  silent file ~/reference/cpp-benchmark.git/worktrees/memory/COMMIT_EDITMSG
endif
balt ~/reference/cpp-benchmark.git/memory/benchmark/CMakeLists.txt
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
let s:l = 1 - ((0 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 015|
lcd ~/reference/cpp-benchmark.git/memory
wincmd w
4wincmd w
exe '1resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 1resize ' . ((&columns * 95 + 95) / 191)
exe '2resize ' . ((&lines * 23 + 25) / 50)
exe 'vert 2resize ' . ((&columns * 95 + 95) / 191)
exe '3resize ' . ((&lines * 11 + 25) / 50)
exe '4resize ' . ((&lines * 11 + 25) / 50)
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
