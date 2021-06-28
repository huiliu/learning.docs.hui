Android沉浸模式
**********************
01234567890123456789012345678901234567890123456789012345678901234567890123456789


.. author:: default
.. categories:: android
.. tags:: android, tutorial
.. comments::

全屏模式，即隐藏顶部的状态栏和底部的导航栏。\ [e]_\ Android 提供了三个用于将应用
设为全屏模式选项：

*  向后倾斜模式
*  沉浸模式
*  粘性沉浸模式

> 在所有三种方法中，系统栏都是隐藏的，您的 Activity 会持续收到所有轻触事件。
> 它们之间的区别在于\ **用户让系统栏重新显示出来的方式**\ 。

启用全屏模式
=====================

向后倾斜模式
------------------------
向后倾斜模式适用于\ **用户不会与屏幕进行大量互动的全屏体验**\ ，例如在观看视频时。

**当用户希望调出系统栏时，只需点按屏幕上的任意位置即可。**

.. sourcecode:: java

   activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);


沉浸模式
------------
沉浸模式适用于\ **用户将与屏幕进行大量互动的应用**\ 。

**当用户需要调出系统栏时，他们可从隐藏系统栏的任一边滑动。**\ 要求使用这种这种
意图更强的手势是为了确保用户与您应用的互动不会因意外轻触和滑动而中断。

.. sourcecode:: java

   int options = View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | SYSTEM_UI_FLAG_IMMERSIVE;
   activity.getWindow().getDecorView().setSystemUiVisibility(options);
   

粘性沉浸模式
------------------
在普通的沉浸模式中，只要用户从边缘滑动，系统就会负责显示系统栏，您的应用甚至不会知
道发生了该手势。因此，如果用户实际上可能是出于主要的应用体验而需要从屏幕边缘滑动，
例如在玩需要大量滑动的游戏或使用绘图应用时，您应改为启用“粘性”沉浸模式。

**在粘性沉浸模式下，如果用户从隐藏了系统栏的边缘滑动，系统栏会显示出来，但它们是
半透明的，并且轻触手势会传递给应用，因此应用也会响应该手势。**

.. sourcecode:: java

   int options = View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
   activity.getWindow().getDecorView().setSystemUiVisibility(options);
   
总结
---------
*  向后倾斜模式：用户点击屏幕任意位置即退出全屏模式。a
*  沉浸模式：只有当用户从被隐藏系统栏的任一边滑动才会退
   出全屏。
*  粘性沉浸模式：与\ *沉浸模式*\ 相同，只有当用户从边缘
   滑过时才会显示系统栏，但系统栏为半透明的。

最好包含其他系统界面标志（例如\ `SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION`\
和\ `SYSTEM_UI_FLAG_LAYOUT_STABLE`\ ），防止布局随着系统栏的隐藏\
和显示调整大小。您还应确保操作栏和其他界面控件同时处于隐藏状态。


代码
----------
.. sourcecode:: java

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    private void hideSystemUI() {
        // Enables regular immersive mode.
        // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
        // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE
                // Set the content to appear under the system bars so that the
                // content doesn't resize when the system bars hide and show.
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                // Hide the nav bar and status bar
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }

    // Shows the system bars by removing all the flags
    // except for the ones that make the content appear under the system bars.
    private void showSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

问题
======
* 对于一些游戏，将操作按键布局到屏幕边缘，如何避免误操作呢？
* cocos2dx中看到一段代码，使用反射的方法来避免全屏模式对\
  API Level的依赖。\ [c]_

参考资源
============

.. [d]   `全屏模式 <https://developer.android.com/training/system-ui/immersive>`_
.. [e]   `Android ImmersiveMode Sample <https://github.com/android/user-interface-samples/tree/master/ImmersiveMode>`_
.. [c]   `use java reflection to avoid API level dependent, use app-abi 10 #16371 <https://github.com/cocos2d/cocos2d-x/pull/16371>`_
