赤兔项目持续集成系统－Jenkins CI部署
**************************************
当前(2013-10-16)赤兔项目主要针对：\ **android**, **iOS**\ 和\ **Web浏览器**\ 三个平台发布客户端。

项目文件结构说明
==================
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
            WebPlayer/

其中：

*   “\ **Assets**\ ”目录存放着所有源代码和原始资源
*   “\ **Assets/Editor**\ ”目录存放着命令命令编译脚本\ “\ **CommandCompiler.cs**\ ”
*   “\ **Bundles**\ ”用于存放Web版客户端编译后资源
*   “\ **StreamingAssets**\ ”用于存放Android版客户端编译后资源
*   “\ **StreamingIOS**\ ”用于存放iOS客户端编译后资源
*   “\ **WebPlayer**\ ”基本废弃

资源的链接
---------------

另外有一个非常重要的文件夹\ **Assets/StreamingAssets**\ ，在编译客户端时需要将将相应的编译后资源链接至此目录。如：当编译Web客户端时，需要将\ **Bundles**\ 目录链接到\ **Assets/StreamingAssets**\ ；编译Android客户端时，需要将\ **StreamingAssets**\ 链接到\ **Assets/StreamingAssets**\ 等。

部分贴图资源仅在目录\ **StreamingAssets/Tex**\ 和\ **StreamingAssets/Temp_Data**\下维护，所以当编译Web客户端和iOS客户端时需要将此两个目录拷贝到\ **Bundles**\ 或\ **StreamingIOS**\

由于\ **Bundles**\ ，\ **StreamingAssets**\ 和\ **StreamingIOS**\ 下面的目录主要用于存放编译后的资源，每次SVN更新可能有新的资源加入，也可能有资源被移除，所以在每次编译前需要将子目录都清空，进行SVN更新时将这些目录排除。



软件需求
=========

开发及编译工具:

1.  服务端使用\ **gcc**\ 和\ **automake**\ 进行编译
2.  Unity3D (>=4.1.2f)
3.  Android SDK (API Level >= 10)
4.  Xcode (>=4.6)
5.  Subversion客户端
6.  Unity3D CacheServer

Jenkins CI系统

1.  jenkins (>=1.534)
2.  jenkins插件：\ *Jenins Unity3d plugin*, *Jenkins SSH Slaves plugin*,\
    *Log Parser Plugin*, *SSH Credentials Plugin*, *Token Macro Plugin*,\
    *Xcode integration*, *IPMessager plugin*\ 等。
3.  jre(Java Runtime Enviroment. >=1.7)

上述开发工具和软件在内网服务器上均有保存。所有运行\ **jenkins**\ 的系统都必需安装java运行时环境(jre)，其它软件根据不同平台需要安装即可。

配置Master-Slave
===================
由于不同的版本需要在不同平台上编译，所以\ **jenkins**\ 需要采用\ **Master-Slave**\ 形式将编译工作分配到不同的平台执行。当前使用Windows平台作为Master，Mac OS和Linux作为Slave。

Master和Slave都需要有足够的磁盘空间存放赤兔客户端源代码，编译中间文件和输出文件。\ [#]_

安装及配置Master
------------------

安装jenkins Master

1.  在作为Master的Windows主机上安装jre, Unity3D和Android SDK。
2.  从SVN服务器上取下赤兔客户端代码
3.  用Unity3D打开客户端项目，指定Android SDK位置。\
    （Editor -> Preference -> External Tools -> Android SDK Location） 
4.  如果有配置CacheServer，同样在Unity3D中指定CacheServer\
    （Editor -> Preference -> Cache Server）
5.  设置环境变量\ `JENKINS_HOME`\ 。项目编译大小和本机执行的job数目，将此环境变量指向一个磁盘空间充足的分区。
6.  从命令行启动jenkins [#]_

    .. code-block:: batch

        java -jar Your_Path/jenkins.war --httpPort=80

7.  从浏览器打开\ http://localhost\ 就可以看到jenkins的界面\ [#]_

jenkins master安装完成后，可以进一步配置：

1.  从浏览器打开jenksin后，进入插件管理\
    （\ *系统管理 -> 管理插件 -> 高级 -> 上传插件*\ ）。\
    安装必备的一些插件：\ *Jenins Unity3d plugin*, *Jenkins SSH Slaves plugin*,\
    *Log Parser Plugin*, *SSH Credentials Plugin*, *Token Macro Plugin*,\
    *Xcode integration*, *IPMessager plugin*\ 等。
2.  指定Unity3D安装位置（\ *系统管理 -> 系统设置 -> Unity3D -> Unity3D安装*\ ）。
3.  指定Xcode安装信息（\ *系统管理 -> 系统设置 -> Xcode Builder*\ ）
4.  指定jenkins使用的Subversion客户端版本（\ *系统管理 -> 系统设置 -> Subversion -> Subversion Workspace Version*\ ）
5.  指定Unity3D日志分析工具的规则集（\ *系统管理 -> 系统设置 -> Console Output Parsing*\ ）
6.  添加SSH登陆凭证（\ *系统管理 -> Manage Credentials*\ ）。在这里添加需要通过SSH登陆的主机的用户名和密码。

至此Master配置完成，可以建立工作在Master上执行编译

配置Slave
-----------
Master配置完成后，可以新建编译工作进行执行。根据不同平台和版本的需求，需要将不同的工作分发给Slave来执行。如：iPhone/iPad版客户端只能在MacOS上进行编译。

以配置MacOS Slave为例:

1.  在主机上安装jre
2.  在浏览器打开Master界面，进行\ *系统管理 -> 管理节点 -> 新建节点*\ 。
        *   设定\ **节点名**\ ，
        *   类型选择\ **Dumb Slave**\ 。
    然后进一步设定节点信息。其中重点关注的是：
        *   可同时执行文件数“\ **# of excutors**\ ”
        *   Slave上的工作目录“\ **Remote FS root**\ ”。此目录所在分区必须有足够磁盘空间\ [#]_
        *   设定一个标签，便于工作分类。“\ **Labels**\ ”
        *   连接Slave的方法“\ **Launch method**\ ”。对于MacOS（类Unix系统）使用SSH连接是最为便利的；对于Windows则使用\ *Java Web Start*\ 比较方便，当然也可以使用cygwin
3.  回到\ *系统管理 -> 管理节点*\ 就可以看到节点连接状态。如果连接失败，点击相应的节点名，查看日志，修正问题

节点连接正常后就可以向其分发任务。

创建工作任务
==============
当Master配置好后，如果满足编译环境要求即可进行编译；当连接上Slave后，即可将不同的工作分配进行分配。

编译Andorid客户端
------------------
1.  “\ *新Job -> 设定任务名称 -> 类型选择自由风格或拷贝已存在任务*\ ”，进入任务详细设定
2.  限制任务的运行节点（\ *Restrict where this project can be run*\ ），指定一个标签（创建Slave时设定的）,这样就可以将不同的任务分发到不同的主机了。如iOS客户端分发到MacOS编译，而Android客户端分发给Windows编译。
3.  \ *源码管理*\ 可以选择Subversion，但由于当前代码冲突的问题没有很好解决，所以使用的是\ **None**\ 。通过命令来更新SVN
4.  增加\ *构建步骤*\ 执行自动构建和发布客户端。

    *   更新SVN：\ `svn up --force --accept tf`
    *   启用Unity3D命令行编译，命令行编译参数为：\ `-batchmode -projectPath "F:\jenkins\workspache\android.branch.rh.onwind.cn -executeMethod CommandCompile.BuildAPK -quit`
5.  发布客户端

    .. code-block:: batch

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
6.  增加构建后动作－进行日志分析和通知相关人员构建结果

    *   *Log Parser Plugin*\ 可以添加规则来分析构建日志。
    *   *IPMessager Plugin*\ 可以通过IPMessage（飞秋）即时通知
    *   还有一些其它通知插件

编译iOS客户端
---------------
iOS平台相对比较复杂，需要导入prov, 证书，证书需要解密，需要编译为IPA文件，安装需要使用plist文件等。

在配置MacOS Slave之前，需要做些额外的准备工作：

1.  新建一个本地用户（xcode），所有操作都通过此用户完成
2.  导入\ **Apple证书**\ 并确认证书有效
3.  导入\ **mobileprovision**\ 文件，并确认有效\ [#]_

在使用jenkins自动集成前，最好手动Checkout一份代码；然后用Unity3D打开；Build一份Xcode代码；用Xcode打开，并编译一个APP，确认整个流程正常。然后，按下面步骤即可建立一个MacOS Slave

1.  确认MacOS Slave已经连接上Master
2.  类似Android任务创建一个新的任务
3.  通过“\ *restrict where this project can be run*\  参数将此任务限制在MacOS上运行
4.  源码管理同样选用“\ *None*\ ”，通过命令行来控制源码的更新
5.  添加构建步骤：

    *   更新源码

        .. code-block:: bash

            cd ${WORKSPACE}

            svn up --force --accept theirs-full
            # TODO 因为使用选项--set-depth exclude将StreamingAssets目录排除了
            # 后来更新时StreamingAssets目录貌似不会被up,需要修正此问题
            svn up --force --accept theirs-full StreamingAssets
            
            rm -rf StreamingIOS/Temp_Data
            rm -rf StreamingIOS/Tex
            
            rsync -av --exclude='.svn' StreamingAssets/Temp_Data StreamingIOS/
            rsync -av --exclude '.svn' StreamingAssets/Tex StreamingIOS/
            
            cp -f StreamingAssets/gameconfig.cfg StreamingIOS/

    *   Unity3D命令行编译输出Xcode项目代码，命令行编译参数为：\ `-batchmode -projectPath "$HOME/jenkins_slave/workspache/ios.branch.rh.onwind.cn -executeMethod CommandCompile.BuildIPA -quit`
    *   Xcode编译iOS APP程序，需要注意下面的参数设定：

        *   **Clean before build**
        *   **Xcode Project Directory**
        *   **Build output directory**
        *   **Build IPA?**
        *   **Unlock Keychain?**
        *   **Keychain path** (${HOME}/Library/Keychains/login.keychain)
        *   **Keychain password** (帐号登陆密码)


建议步骤
---------
由于目录结构的原因，当前编译生成的资源目录也在SVN的管理下，所以可能存在最终资源的混乱，建议在正式自动集成前执行下面操作：

1.  此步骤为必需步骤：将“\ **Bundles, StreamingAssets, StreamingIOS**\ ”根据不同平台需求，建立一个符号链接至“\ **Assets/StreamingAssets**\ ”
2.  对于不同的平台，SVN更新时将其它平台的资源文件排除不更新，以减少更新时间。如Windows平台输出Web版本客户端时，第一次可以先运行：（必须在jenkins的构建步骤中添加）

.. code-block:: batch

    svn up --force --accept theirs-full --set-depth exclude StreamingAssets\Data --set-depth exclude StreamingAssets\Fx --set-depth exclude StreamingAssets\hud --set-depth exclude StreamingAssets\Model --set-depth exclude StreamingAssets\Music --set-depth exclude StreamingAssets\Scenes --set-depth exclude StreamingAssets\Sound

    对于其它平台每第一次运行时，执行类似的命令排除无关目录


Unity3D的命令行编译
=====================
Unity3D支持命令行编译，常用命令行参数选项有：

    *   -batchmode      启用命令模式
    *   -projectPath    指定项目路径。Unix环境可以使用$HOME
    *   -executeMethod  指定执行编译的类与其方法
    *   -quit           完成自动退出。没有此选项，即使编译完成也不会返回

一个标准的命令行编译命令如：（与平台无关）

.. code-block:: bash

    unity3d -batchmode -projectPath $HOME/jenkins/workspace/android.trunk.rh.onwind.cn

编译代码
----------



常见问题
=========
1.  资源，特效，贴图丢失

    导致这些问题的原因大多是因为资源的meta文件丢失或混乱所造成的。

        *   gameconfig.cfg文件是否更新正常
        *   在Unity3D中运行游戏，运行到故障场景时，查看相应的资源加载情况，找到丢失了什么资源，然后去检查相应的meta文件是否存在，与prefeb目录中一致。找到不一致的原因。
        *   也可能是某次更新时，资源的meta文件丢失，编译时Unity3D会自动为没有meta文件的资源创建一个新的meta文件；而后来丢失的meta文件被补充至SVN服务器，当再次更新时，SVN服务器上的meta文件将不会被下载，就会导致meta文件混乱而找不到资源。

2.  项目属性的设定

对于不同的版本的客户端，其输出参数不尽相同。在代码中可以通过Unity3D中的\ `PlayerSettings`\ 类进行设定；在图形界面可以通过菜单\ **File -> Build Settings -> Player Settings**\ 打开选项卡进行设定。当前已通过代码的方式指定（“\ *Assets/Editor/CommandCompiler.cs*\ ”）。


3.  SVN更新时冲突的解决


说明
======
.. [#]  Master的临时文件夹所在分区也应该有足够磁盘空间，否则master将不能执行job并离线。
.. [#]  如果主机上运行着其它服务占用了80, 8080等端口，可以通过命令行参数调整jenkins侦听的端口。
.. [#]  jenkins界面语言与你的浏览器默认语言一致。即浏览器默认英文则为英文界面，默认为中文则为中文界面。
.. [#]  相关阀值由\ *系统管理 -> 管理节点 -> 设置*\ 处指定
.. [#]  证书可以通过查看\ **钥匙串**\ 确认是否有效；mobileprovision需要打开Xcode查看
