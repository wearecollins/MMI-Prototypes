set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab
set colorcolumn=80
highlight colorcolumn ctermbg=darkgray
let &path.="../Shared/ofxLibwebsockets/libs/ofxLibwebsockets/include/ofxLibwebsockets,../../addons/ofxGui/src"
set nocompatible
filetype off
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'rdnetto/YCM-Generator'
call vundle#end()
filetype plugin indent on
nnoremap <F4> :make!<cr>
nnoremap <F5> :!make run<cr>
