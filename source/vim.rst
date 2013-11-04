VIM Tips
*********


TAB的问题
==========
众所周知\ `Python`\ 使用缩进来控制代码的块，在PEP8中建议使用4个空格来替换TAB，\
须使用TAB，即“\ **\t**\ ”。So，在\ **~/.vimrc**\ 需要使用\ `autocmd` [#]_ [#]_\
命令来达到这一目的。

.. code-block:: vim

    syntax on
    set expandtab
    set tabstop=4
    set shiftwidth=4

    autocmd FileType make setlocal noexpandtab

以十六进制形式查看文件
========================
对于一些二进制文件，可能希望以十六进制的形式来查看文件。用\ ``vim``\ 打开文件后\
在命令模式下输入：\ **:%!xxd**\ 就可以看到文件的十六进制码了。

参考资料
=========
.. [#]  http://stackoverflow.com/questions/8999208/quickest-way-to-revert-spaces-to-tabs-in-vim
.. [#]  http://vimcdoc.sourceforge.net/doc/quickref.html
