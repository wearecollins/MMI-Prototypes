set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab
set colorcolumn=80
highlight colorcolumn ctermbg=darkgray
set nocompatible
filetype off
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'rdnetto/YCM-Generator'
call vundle#end()
filetype plugin indent on
