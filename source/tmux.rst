tmux
*****

`tmux`_\ 是\ **screen**\ 的替代工具，功能比screen更为强大。

.. _tmux:   http://tmux.sourceforge.net/

从源码编译安装
==============
**tmux**\ 依赖\ `libevent`_\ 和\ `ncurses`_\ 这两个库，所以从源码安装时，也得确认这两个库文件\
已经安装。

.. _libevent:   http://libevent.org/
.. _ncurses:    http://invisible-island.net/ncurses/

假定所有软件的安装目录为："**~/local**"

* 安装\ **libevent**\ 和\ **ncurses**\ 时只需要设定好安装目录即可： ::

    ./configure --prefix=~/local

* 安装\ **tmux**\ 时需要设定好\ *libevent*\ 库的链接，如果系统上安装了"**pkgcon\fig**"，那么只需设定一个环境变量： ::
    
    export PKG_CONFIG_PATH=~/local/lib/pkgconfig

就可以；如果没有安装此软件，将环境变量\ *LIBEVENT_CFLAGS*\ 设定为："**-L~/loc\
al/lib -levent**"，另外一个环境变量是"*LIBEVENT_LIBS*"。然后运行\ *configure*\ 即可。::
    
    ./configure --prefix=~/local


