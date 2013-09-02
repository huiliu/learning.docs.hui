Gentoo中文输入法
******************
Gentoo的Gnome桌面环境默认输入法使用的是\ `ibus`\ 。由于一次升级ibus中发现ibus的\
的主程序\ `ibus`\ 与其输入法Table不同步，而且很长时间都没有同步上，让我在输入问\
题上浪费了大量的时间，所以将\ `gnome-base/gnome-settings-daemon, gnome-base/\
gnome-control-center, gnome-base/gnome-shell`\ 的默认USE中的"**i18n**"给去掉了。\
这样安装Gnome时就不会默认安装上ibus了。

安装fcitx
===========
**fcitx**\ 只需要安装一个包\ `app-i18n/fcitx`\ 中可以使用拼音，五笔输入了，感觉\
比ibus方便。安装“fcitx”之前需要进行一些设定：

* 将系统环境变量“\ `LC_CTYPE`\ 设定为： "**zh_CN.UTF-8**"。通过命令"`locale`"来\
  检查确认。
* 设定环境变量：

.. sourcecode:: bash

    #! /bin/bash

    # /etc/X11/xinit/xinit.c/99-input

    export XMODIFIERS="@im=fcitx"
    export XIM=fcitx
    export XIM_PROGRAM=fcitx
    export GTK_IM_MODULE=fcitx
    export QT_IM_MODULE=fcitx

* 安装完成后重新启动X Server： "`pkill X`"

如果仍然不能使用中文使用法，可能是版本的问题。请使用当前稳定版（即emerge -pv时\
版本号为绿色的）

.. sourcecode:: bash

   emerge -qv =app-i18n/fcitx-4.2.7


参考资料
==========
http://hi.baidu.com/yioopayczwgnsye/item/044ae5a2b165f69e151073a1
