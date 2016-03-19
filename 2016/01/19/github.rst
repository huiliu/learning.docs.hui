使用GitHub
***********
记录使用\ `github`_\ 的一些经历。

.. author:: default
.. categories:: tips
.. tags:: tips, github
.. comments::
.. more::



GitHub Pages
=============
`github pages`_\ 可以方便的用来建立一个BLOG，默认，也是最常用的是\ `jekyll`_\
引擎，\ **jekyll**\ 的文件使用\ `Markdown`_\ 方法，\ **Markdown**\ 是一种非常简\
方便的文本撰写格式，不过Python文档使用\ `sphinx`_\ 工具对我更有吸引力，因此转用\
**sphinx**\ 来写文档，生成HTML文件然后上传至GitHub Pages就可以了。

由于\ *github pages*\ 默认使用\ *jekyll*\ 引擎，使用\ *sphinx*\ 生成的HTML文件\
会显示不正常，主要是因为无法找到CSS和javascript文件，此时需要告诉\ *github
pages*\ ：“\ *我不使用jekyll*\ ”，只需要在repo中加入一个文件\ ``.nojekyll``\ 就\
可以了\ [#ref1]_\ 。

.. _github: https://github.com
.. _github pages: http://pages.github.com/
.. _jekyll: http://jekyllbootstrap.com/
.. _Markdown:   http://daringfireball.net/projects/markdown/syntax
.. _sphinx:     http://sphinx-doc.org


.gitignore
===========
在repo的根目录下添加文件\ **.gitignore**\ ，然后将当前目录下你不希望加入仓库的\
文件名，目录名加入其中，这样能减少不必要的烦恼。\ *.gitignore*\ 的内容如：

.. sourcecode:: text

    _site/*
    _theme_packages/*

    Thumbs.db
    .DS_Store
    
    !.gitkeep
    
    .rbenv-version
    .rvmrc


参考资料
=========
.. [#ref1] https://github.com/blog/572-bypassing-jekyll-on-github-pages
