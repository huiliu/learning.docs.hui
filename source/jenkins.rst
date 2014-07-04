赤兔项目持续集成系统－Jenkins CI部署
*************************************
当前(2013-10-16)赤兔项目主要针对：\ **android**, **iOS**\ 和\ **Web浏览器**\ 三个平\
台发布客户端。本文主要介绍客户端的持续集成。

项目文件结构说明
=================
`http://devhome/repos/rhsrc/trunk/src/Client2`\ 下的代码目录结构为：

.. code-block:: text

    Client2/
            Assets/
                Editor/
            Bundles/
            Library/
            ProjectSettings/
            StreamingAssets/
                Temp_Data/
                Tex/
            StreamingIOS/
            Tools/
                bin/
                etc/
                ...
            WebPlayer/

其中：

*   “\ ``Assets``\ ”目录存放着所有源代码和原始资源
*   “\ ``Assets/Editor``\ ”目录存放着命令命令编译脚本\ “\ ``CommandCompiler.cs``\ ”
*   “\ ``Bundles``\ ”用于存放Web版客户端编译后资源 （可以在编译代码中调整）
*   “\ ``StreamingAssets``\ ”用于存放Android版客户端编译后资源（可以编译代码中修改调整）
*   “\ ``StreamingIOS``\ ”用于存放iOS客户端编译后资源（可以在编译代码中修改调整）
*   "\ ``Tools``\ 存放命令行编译工具
*   “\ ``WebPlayer``\ ”基本废弃


资源的链接
---------------
当前项目的设定中，将编译后的资源根据不同的平台分别存放在\ ``Bundles, StreamingAssets, StreamingIOS``\ 这三个目录。
而根据\ ``Unity3D``\ 的特殊目录要求，资源应该保存在\ ``Assets/StreamingAssets``\ 中，所以在编译输出二进制包和\ `xcode`\ 项目文件时，需要将对应编译后的资源文件连接至此。如：当编译iOS的xcode项目文件时，需要将\ ``Assets/StreamingIOS``\ 目录链接到\ ``Assets/StreamingAssets``\ ；编译Android客户端时，需要将\ ``StreamingAssets``\ 链接到\ ``Assets/StreamingAssets``\ 等。

部分贴图资源仅在目录\ ``StreamingAssets/Tex``\ 下维护，所以当编译Web客户端和iOS客户端时需要将此两个目录拷贝到\ ``Bundles``\ 或\ ``StreamingIOS``\

由于\ ``Bundles``\ ，\ ``StreamingAssets``\ 和\ ``StreamingIOS``\ 每次SVN更新可能有新的资源加入，也可能有资源被移除，所以在每次编译前需要将子目录都清空，进行SVN更新时将这些目录排除。


软件需求
=========

开发及编译工具:

1.  服务端使用\ ``gcc``\ 和\ ``automake``\ 进行编译
2.  Unity3D (>=4.1.2f)
3.  Android SDK (API Level >= 10)
4.  Xcode (>=4.6)
5.  Subversion客户端
6.  Unity3D CacheServer

Jenkins CI系统

1.  jenkins (>=1.534)
2.  jenkins插件：\ *Jenkins Unity3d plugin*, *Jenkins SSH Slaves plugin*,\
    *Log Parser Plugin*, *SSH Credentials Plugin*, *Token Macro Plugin*,\
    *Xcode integration*, *IPMessager plugin*\ 等。
3.  jre(Java Runtime Enviroment. >=1.7)

上述开发工具和软件在内网服务器上均有保存。（\ ``\\server\SOFTHOME\开发相关\CI``\ ）所有运行\ **jenkins**\ 的系统（包括slave）都必需安装java运行时环境(jre)，其它软件根据不同平台角色需要安装即可。注意\ `jenkins`\ 和相应插件必要时应进行升级更新（出现莫名其妙的问题，请检查\ `jenkins`\ 官方网站的更新升级日志）

配置Master-Slave
===================
由于不同的版本需要在不同平台上编译，所以\ **jenkins**\ 需要采用\ **Master-Slave**\ 形式将编译工作分配到不同的平台执行。当前使用Windows平台作为Master，Mac OS和Linux作为Slave。（当前使用过程中发现，使用\ ``JNLP``\ 方法连接slave时，随着时间的推进，java会战胜大量内存，而且经常断开。可能原因是编译是产生了大量日志>100M）

Master和Slave都需要有足够的磁盘空间存放赤兔客户端源代码，编译中间文件和输出文件。\ [#]_

安装及配置Master
------------------
安装jenkins Master
^^^^^^^^^^^^^^^^^^^
1.  在作为Master的主机上安装\ ``jre``
2.  设置环境变量\ `JENKINS_HOME`\ ，\ `JENKINS_HOME`\ 所指向的目录应该有足够的空间（每个job >= 1GB）
3.  从命令行启动jenkins [#]_

    .. code-block:: bash

        java -jar Your_Path/jenkins.war --httpPort=80
        # 如果是在linux上运行，可以使用daemon模式
        # Windows可以在进行主界面后安装成系统服务
        java -jar Your_Path/jenkins.war --daemon --httpPort=80
        

4.  从浏览器打开\ http://ip\ 就可以看到jenkins的界面\ [#]_

当\ ``jenkins`` 界面后可以对系统进行第一步配置：

1.  从浏览器打开jenksin后，进入插件管理\
    （\ *系统管理 -> 管理插件 -> 高级 -> 上传插件*\ ）。\
    安装必备的一些插件：\ *Jenins Unity3d plugin*, *Jenkins SSH Slaves plugin*,\
    *Log Parser Plugin*, *SSH Credentials Plugin*, *Token Macro Plugin*,\
    *Xcode integration*, *IPMessager plugin*\ 等。
2.  指定Unity3D安装位置（\ *系统管理 -> 系统设置 -> Unity3D -> Unity3D安装*\ ）。即使不在master执行编译任务，也需要设定一个名字，因为向其它节点添加工具路径时需要此值。
3.  指定Xcode安装信息（\ *系统管理 -> 系统设置 -> Xcode Builder*\ ）
4.  指定jenkins使用的Subversion客户端版本（\ *系统管理 -> 系统设置 -> Subversion ->\
    Subversion Workspace Version*\ ）
5.  指定Unity3D日志分析工具的规则集（\ *系统管理 -> 系统设置 -> Console Output\
    Parsing*\ ）
6.  添加SSH登陆凭证（\ *系统管理 -> Manage Credentials*\ ）。在这里添加需要通过SSH登\
    陆的主机的用户名和密码。
7.  添加“标签”


安装插件
^^^^^^^^^
安装一些jenkins插件，可以更好的进行持续集成：

1.  **Environment Injector Plugin** 可以输出预定义变量以供构建时使用，可以大大方便对\
    job参数的修改。如当前Unity3D项目，Unity3D命令行编译时需要ProjectPath,将编译资源和\
    输出二进制包分开，就需要输入两次，当需要进行修改时，极易出现遗漏，导致构建失败。
2.  **Build Pipeline Plugin**
3.  **IPMessage** 便于通知用户，不足之处是中文显示错误。


配置Slave
---------
根据不同平台和版本的需求，需要将不同的工作分发给Slave来执行。如：iPhone/iPad
版客户端只能在MacOS上进行编译。

以配置MacOS Slave为例:

1.  在主机上安装\ `jre`\ ,\ `Unity3D`\ 等必需软件
2.  在浏览器打开Master界面，进行\ *系统管理 -> 管理节点 -> 新建节点*\ 。
        *   设定\ **节点名**\ ，
        *   类型选择\ **Dumb Slave**\ 。
    然后进一步设定节点信息。其中重点关注的是：
        *   可同时执行文件数“\ **# of excutors**\ ”
        *   Slave上的工作目录“\ **Remote FS root**\ ”。此目录所在分区必须有足够磁盘\
            空间\ [#]_
        *   设定一个标签，便于工作分类。“\ **Labels**\ ”
        *   连接Slave的方法“\ ``Launch method``\ ”。对于MacOS（类Unix系统）使用SSH\
            连接是最为便利的；对于Windows则使用\ *Java Web Start*\ 比较方便，当然也可\
            以使用cygwin或其它ssh工具连接Windows。接着指定slave的IP，选择已经保存的凭证（Credentials）
        *   设定要使用的工具路径（Unity3D）
        *   设定slave上的工作目录，并确认此目录存在且ssh用户具有读写权限。
3.  回到\ *系统管理 -> 管理节点*\ 就可以看到节点连接状态。如果连接失败，点击相应的节点名，查看日志，修正问题
4.  用于编译\ ``android``\ 客户端的主机，还需要安装\ `Android SDK`\ ，并且需要打开Unity3D指定路径
    1.  用Unity3D打开客户端项目，指定Android SDK位置。（Editor -> Preference -> External Tools -> Android SDK Location） 
    2.  如果有配置CacheServer，同样在Unity3D中指定CacheServer（Editor -> Preference -> Cache Server）
5.  编译iPhone/iPad客户端，只能在Mac OS平台上进行，步骤也相对复杂，需要导入prov, 证书，证书需要解密，需要编译为IPA文件等
    1.  新建一个本地用户（xcode），所有CI相关操作都通过此用户完成
    2.  导入\ **Apple开发者证书**\ 并确认证书有效
    3.  导入\ **mobileprovision**\ 文件，并确认有效\ [#]_

在使用master/slave进行构建前，最好手动Checkout一份代码；然后用Unity3D打开，手动完成一次完整的编译，并确保成功。对于iPhone/iPad客户端，需要先由\ ``Unity3D``\ 输出一个Xcode项目代码；用Xcode打开，并编译一个APP，确认整个流程正常。

slave节点连接正常后就可以创建任务并向其分发。

创建工作任务
============
当Master配置好后，如果满足编译环境要求即可进行编译；当连接上Slave后，即可将的工作分发给slave执行，对于不同的任务，可以通过标签来加以限定。
手动确认\ `slave`\ 上的编译工作可以正常完成后，可以打开\ ``jenkins``\ 的界面创建新的工作。
为了便于进行持续集成，所以通常使用Unity3D的命令行编译来完成编译。[#]_

Android
-------
1.  确认作为slave的主机是否已经连上master。
2.  确认\ ``jenkins``\ 已经安装了插件\ ``Jenkins Unity3d plugin, Xcode integration``
3.  “\ *新Job -> 设定任务名称 -> 类型选择自由风格或拷贝已存在任务*\ ”，进入任务详细设定 
4.  为任务指定一个标签（创建Slave时设定的），以限制任务的运行节点（\ *Restrict where this project can be run*\ ），这样就可以将不同的任务分发到不同的主机了。如iOS客户端分发到MacOS编译，而Android客户端分发给Windows编译。
5.  \ *源码管理*\ 可以选择Subversion，但由于当前代码冲突的问题没有很好解决，所以使用的是\ **None**\ 。在\ *构建步骤*\ 中增加构建步骤通过命令来更新SVN。
6.  增加\ *构建步骤*\ 执行自动构建和发布客户端。

    *   更新SVN：\ `svn up --force --accept tf`
    *   使用Unity3D命令行编译，命令行编译参数为：\

        .. sourcecode:: bash

            # 编译资源
            -batchmode -projectPath $UNITY3D_PROJECT_DIR -executeMethod CommandCompiler.CompileResource -quit
            # 编译xcode项目
            -batchmode -quit -projectPath $UNITY3D_PROJECT_DIR -executeMethod CommandCompiler.PerformBuild "" "onwind" "yourpath" "iPhone"
7.  客户端编译完成后，可以通过sftp, ftp等方法将客户端发布：

    .. code-block:: bat

        ::echo off

        set GAME_HOST=10.1.0.190
        set GAME_PATH=/var/www/html/install/branch/android
        set GAME_VERSION=0.00.02
        set SCP=D:\CI\soft\PSCP.EXE
        set SSH_USER=root
        set SSH_PASSWD=setupthepassword
        
        set iFILE=E:\output\android.apk
        :: BUILD_NUMBER and BUILD_ID come from jenkins CI System
        set oFILE=E:\output\fhsgCommon_%GAME_VERSION%_%BUILD_NUMBER%_%BUILD_ID%.apk
        
        move %iFILE% %oFILE%
        
        %SCP% -batch -pw %SSH_PASSWD% %oFILE% %SSH_USER%@%GAME_HOST%:%GAME_PATH%

    另外可以使用Public-over-ssh/ftp/samba等插件来进行发布客户端。
8.  添加构建后动作－进行日志分析和通知相关人员构建结果

    *   *Log Parser Plugin*\ 可以添加规则来分析构建日志。
    *   *IPMessager Plugin*\ 可以通过IPMessage（飞秋）即时通知
    *   还有一些其它通知插件

iOS/iPhone/iPad
---------------

1.  确认MacOS Slave已经连接上Master，确认\ ``jenkins``\ 已经安装了插件\ ``Jenkins Unity3d plugin, Xcode integration``
2.  类似Android任务创建一个新的任务
3.  通过“\ *restrict where this project can be run*\  参数将此任务限制在MacOS上运行
4.  源码管理同样选用“\ *None*\ ”，通过命令行来控制源码的更新
5.  添加构建步骤：

    *   更新源码

        .. code-block:: bash

            cd ${WORKSPACE}
            svn up --force --accept theirs-full Assets ProjectSettings StreamingAssets\Tex Tools StreamingAssets\hud.cfg StreamingAssets\gameconfig.cfg 
            
    *   Unity3D命令行编译输出Xcode项目代码，命令行编译参数为：\

        .. sourcecode:: bash

            # 编译资源
            -batchmode -projectPath $UNITY3D_PROJECT_DIR -executeMethod CommandCompiler.CompileResource -quit
            # 编译xcode项目
            -batchmode -quit -projectPath $UNITY3D_PROJECT_DIR -executeMethod CommandCompiler.PerformBuild "" "onwind" "yourpath" "iPhone"
    *   Xcode编译iOS APP程序，需要注意下面的参数设定：

        *   **Clean before build**
        *   **Xcode Project Directory**
        *   **Build output directory**
        *   **Build IPA?**
        *   **Unlock Keychain?**
        *   **Keychain path** (${HOME}/Library/Keychains/login.keychain)
        *   **Keychain password** (帐号登陆密码)

Nightly Build
---------------
针对每一个平台（客户端）建立一个\ `每日构建`\ 的任务，以保证每天提交的代码是可以通过编译，\
`每日构建`\ 任务与上面任务的建立方法几乎完全一样，唯一的差别在于源码控制。\ `每日构建`\
任务需要每次使用完全干净的代码（与SVN服务器上的代码一致）。所以建议使用\ `jenkins`\ 内\
置的版本控制工具来管理源码，每次执行构建之前都从SVN服务器上\ `checkout`\ 一份新的源码，\
或者更新源码之前对当前代码进行\ `svn revert`\ 。基本步骤：

1.  按照通用方法建立任务。
2.  在任务配置页中，\ `Source Code Management`\ 部分选择使用\ `subversion`\ 。填写好\
    相关选项，如：\ *Repository URL, Credentials*\ ，最重要的是：\ `Check-out Strategy`
    项，默认为："*Use 'svn update' as much as possible*"，需要更改为选项：\
    "*Use 'svn update' as much as possible, with 'svn revert' before update*"\ 以保证更新代码前revert所有本地更改。
3.  构建步骤按相应平台（客户端）设置即可。

`每日构建`\ 的关键是：\ **每次执行构建时使用一份全新，干净的源码**\ 。然后自动部署到测试\
环境中，由测试人员进行测试。而日常由于各种情况需要不定期进行编译，此时执行其它非\
`每日构建`\ 的任务完成构建。

建议步骤
---------
由于目录结构的原因，当前编译生成的资源目录也在SVN的管理下，所以可能存在最终资源的混乱，建\
议在正式自动集成前执行下面操作：

1.  此步骤为必需步骤：将“\ **Bundles, StreamingAssets, StreamingIOS**\ ”根据不同平\
    台需求，建立一个符号链接至“\ **Assets/StreamingAssets**\ ”。（可使用脚本\
    `SetupAPK.bat`\ 和\ `SetupIOS.sh`\ 完成此操作）
2.  对于不同的平台，SVN更新时将其它平台的资源文件排除不更新，以减少更新时间。如Windows平\
    台输出Web版本客户端时，使用\ `TortoiseSVN`\ 从SVN选择性的\ `checkout`\ 与当前平台\
    相关的目录；更新时只更新必需目录。

    .. code-block:: bat

        :: 必须在jenkins的构建步骤中添加
        svn up --force --accept theirs-full Assets ProjectSettings Tools StreamingAssets/Tex StreamingAssets/gameconfig.cfg StreamingAssets/hud.cfg

    对于其它平台每第一次运行时，执行类似的命令排除无关目录
3.  利用插件\ **Environment Injector Plugin**\ 对环境（变量）进行统一管理
4.  针对每个平台建立一个由\ `jenkins`\ 内置版本控制工具（SVN）管理源码的工作，实现\ ``每日构建``\ 确保每天提交的代码可以正常工作。

构建步骤
=========
``Unity3D``\ 的编译过程可以通过代码自定制，相当比较灵活。当前编译程序位于\ ``Assets/Editor``\ 目录下，\ ``BuildBundle.cs``\ 是用于GUI界面编译的菜单选项；\ ``CommandCompiler.cs``\ 是命令编译代码。资源和程序的编译功能由程序维护。\ ``CommandCompiler.cs``\ 中需要根据不同平台，不同渠道分别进行一些设定。对于\ ``iPhone/iPad``\ 可能来需要向生成的xcode项目添加一些额外的SDK文件，此时需要利用Unity3D提供的\ `Post Process BuildPlayer`\ 功能。\ [#]_

Unity3D的命令行编译
---------------------
Unity3D支持命令行编译，常用命令行参数选项有：\ [#cmd_param]_

*   \-batchmode      启用命令模式
*   \-projectPath    指定项目路径。Unix环境可以使用环境变量$HOME
*   \-executeMethod  指定执行编译的类与其方法
*   \-quit           完成自动退出。没有此选项，即使编译完成也不会返回
*   \-buildTarget   激活相应的平台。由于SVN上只保存一份代码（即只针对一个平台），所有在编译时需要将工程切换到指定平台。

一个标准的命令行编译命令如：（与平台无关）

.. code-block:: bash

    unity3d -batchmode -projectPath $HOME/jenkins/workspace/android.trunk.rh.onwind.cn

PostprocessBuildPlayer
-----------------------
`Unity3D`\程序编译Player完成后会执行\ `Editor`\ 目录下\
``PostprocessBuildPlayer``\ 程序（任意可执行代码）进行相关操作。\ [#]_\
在当前项目中由于不同渠道提供的SDK千奇百怪，有时需要使用此功能向xcode项目中添加\
文件。当前项目中的\ ``PostprocessBuildPlayer``\ 是使用python所写，用于向xcode项\
目添加91的SDK。

``Unity``\ 调用\ ``PostprocessBuildPlayer``\ 时会向其传递7个参数：

.. sourcecode:: perl

    #!/usr/bin/perl
    
    my $installPath = $ARGV[0];
    
    # The type of player built:
    # "dashboard", "standaloneWin32", "standaloneOSXIntel", "standaloneOSXPPC", "standaloneOSXUniversal", "webplayer"
    my $target = $ARGV[1];
    
    # What optimizations are applied. At the moment either "" or "strip" when Strip debug symbols is selected.
    my $optimization = $ARGV[2];
    
    # The name of the company set in the project settings
    my $companyName = $ARGV[3];
    
    # The name of the product set in the project settings
    my $productName = $ARGV[4];
    
    # The default screen width of the player.
    my $width = $ARGV[5];
    
    # The default screen height of the player 
    my $height = $ARGV[6];
    
    print ("\n*** Building at '$installPath' with target: $target \n");

在三国中我们使用python来实现\ ``PostprocessBuildPlayer``\ 向Xcode项目中添加文件：

.. sourcecode:: python

    #!/usr/bin/env python
    # -*- coding: utf-8 -*-
    
    """Unity3D PostprocessBuildPlayer
    用于向Unity3D导出的xcode项目中添加额外的文件。
    
    由于unity3D会输出各种不同的客户端，并接入不同的平台（使用不同的SDK），由此知道\
    会添加各种不同的文件，而Unity3D再编译完成后均会调用\ ``PostprocessBuildPlayer``\
    。为了可以正确的添加相应的SDK信息，需要在\ ``PostprocessBuildPlayer``\ 中根据\
    Unity3D传递的参数来进行判断。Unity3D会向\ ``PostprocessBuildPlayer``\ 传递一些\
    参数，其中\ ``sys.argv[1]``\ 为安装路径（即导入xcode的路径）；\ ``sys.argv[2]``\
    为BuildTarget（即：Andriod, iPhone ...）。
    
    **在编译时将不同版本，不同运营商的客户端输出到不同的路径，上面两个参数就可以用\
    于确定相应添加什么文件。**
    
    Author: Liu Hui
    Date: Sat Feb  8 14:07:41 CST 2014
    """
    
    import sys
    from mod_pbxproj import XcodeProject
    
    # 请根据实际情况修改\ ``xcode_path, agent``\ 的值
    # ``agent``\ 的值与命令行编译脚本\ ``CommandCompiler.cs``\ 中的output相对应
    xcode_path = '/Users/xcode/xcode_project'
    agent = 'dj91'
    xcode_project_path = '%s/%s' % (xcode_path, agent)
    
    if sys.argv[2].lower() != 'iphone':
        sys.exit(0)
    elif sys.argv[2].lower() == 'iphone' and sys.argv[1] == xcode_project_path:
        pass
    
    
    # 请根据实际情况修改或增加SDK路径：\ ``DJ_PATH``\ 的值
    DJ_PATH = '/Users/xcode/GameSDK/DJGameSDK1'
    system_framework_path = '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk/System/Library/Frameworks'
    pbxproj = '%s/Unity-iPhone.xcodeproj/project.pbxproj' % xcode_project_path
    
    # 添加第三方SDK信息
    dj_resource = '%s/Resources' % DJ_PATH
    dj_frm = '%s/DJGame.framework' % DJ_PATH
    dj_unity3d_frm = '%s/DJGameForUnity3D.framework' % DJ_PATH
    
    # 添加依赖系统框架
    messageui = '%s/MessageUI.framework' % system_framework_path
    coretext = '%s/CoreText.framework' % system_framework_path
    
    project = XcodeProject.Load(pbxproj)
    
    project.add_folder(dj_resource)
    project.add_folder(dj_frm)
    project.add_folder(dj_unity3d_frm)
    project.add_folder(messageui)
    project.add_folder(coretext)
    
    if project.modified:
        project.backup()
        project.save()



基本构建步骤
-------------

当前由于需要在资源编译完成后生成\ `gameconfig.cfg`\ 和资源的版本信息文件\ `version.cfg`\ ，所以当前将整个编译过程分为四部分

1.  从SVN上更新完代码后，清理上次编译生成的资源
2.  调用\ ``Tools/bin/datamaker.py``\ 生成数据文件，并检查当前工程的资源读取方式是否正确
3.  编译资源
4.  调用\ ``Tools/bin/fileversion.py``\ 生成\ `gameconfig.cfg`\ 和\ `version.cfg`
5.  编译二进制文件（\ `Android`\ 系统为apk包，\ `iPhone/iPad`\ 为xcode项目文件）
6.  进一步编译使用\ `xcodebuild`\ 和\ `xcrun`\ 编译xcode项目，生成ipa文件

常见问题
=========
1.  资源，特效，贴图丢失
    
    导致这些问题的原因大多是因为资源的meta文件丢失或混乱所造成的。

    *   `gameconfig.cfg`\ 文件是否更新正常
    *   在Unity3D中运行游戏，运行到故障场景时，查看相应的资源加载情况，找到丢失了什么资\
        源，然后去检查相应的meta文件是否存在，与prefeb目录中一致。找到不一致的原因。
    *   也可能是某次更新时，资源的meta文件丢失，编译时Unity3D会自动为没有meta文件的资源\
        创建一个新的meta文件；而后来丢失的meta文件被补充至SVN服务器，当再次更新时，SVN\
        服务器上的meta文件将不会被下载，就会导致meta文件混乱而找不到资源。

2.  项目属性的设定
    
    对于不同的版本的客户端，其输出参数不尽相同。在代码中可以通过Unity3D中的\
    `PlayerSettings`\ 类进行设定；在图形界面可以通过菜单\ **File -> Build Settings
    -> Player Settings**\ 打开选项卡进行设定。当前已通过代码的方式指定（“\
    *Assets/Editor/CommandCompiler.cs*\ ”）。

3.  SVN更新时冲突的解决


说明
======
.. [#]  Master的临时文件夹所在分区也应该有足够磁盘空间，否则master将不能执行job并离线。
.. [#]  如果主机上运行着其它服务占用了80, 8080等端口，可以通过命令行参数调整jenkins侦听\
        的端口。
.. [#]  jenkins界面语言与你的浏览器默认语言一致。即浏览器默认英文则为英文界面，默认为中\
        文则为中文界面。
.. [#]  相关阀值由\ *系统管理 -> 管理节点 -> 设置*\ 处指定
.. [#]  证书可以通过查看\ **钥匙串**\ 确认是否有效；mobileprovision需要打开Xcode查看
.. [#]  file:///C:/Program%20Files/Unity/Editor/Data/Documentation/Documentation/Manual/CommandLineArguments.html
.. [#]  file:///C:/Program%20Files/Unity/Editor/Data/Documentation/Documentation/Manual/BuildPlayerPipeline.html
.. [#]  file:///C:/Program%20Files/Unity/Editor/Data/Documentation/Documentation/Manual/BuildPlayerPipeline.html
.. [#cmd_param] http://www.unity3d.com
