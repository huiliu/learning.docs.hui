VIM Tips
*********


TAB的问题
==========
众所周知\ `Python`\ 使用缩进来控制代码的块，在PEP8中建议使\
用4个空格来替换TAB，而\ **Makefile**\ 文件对缩进的要求是必\
须使用TAB，即“\ **\t**\ ”。So，在\ **~/.vimrc**\ 需要使用\
`autocmd` [#]_ [#]_\ 命令来达到这一目的。

.. code-block:: vim

    syntax on
    set expandtab
    set tabstop=4
    set shiftwidth=4

    autocmd FileType make setlocal noexpandtab


参考资料
=========
.. [#]  http://stackoverflow.com/questions/8999208/quickest-way-to-revert-spaces-to-tabs-in-vim
.. [#]  http://vimcdoc.sourceforge.net/doc/quickref.html
