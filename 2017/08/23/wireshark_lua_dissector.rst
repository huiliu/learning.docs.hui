Wireshark Lua Dissector
************************

记录一次使用Wireshark Lua Dissector的经历。

.. author:: default
.. categories:: none
.. tags:: Network
.. comments::

当前版本Wireshark支持lua吗？
===========================
从官方下载的版本都是支持Lua的(5.2.4)，如果是自己从源码编译，可以通过查看"帮助" -> "关于"\
出现的弹窗中，编译信息中是否包含Lua。

Dissector放在哪里
==================
查看"帮助" -> "关于" 窗口中的"文件夹"选项夹，可以看到当前使用的相关文件夹。

Wireshark启动时，会从两个目标加载插件(Lua Dissector)：(Windows系统)
*   用户插件目录：\ *APPDATA\Wireshark\plugins*\ (plugins目录可能不存在，手动创建即可)
*   全局插件目录：\ *WIRESHARK\Wireshark\plugins*\ (安装目录)

在全局插件目录下有一个特殊文件"*init.lua*"用于控制Lua相关功能：如果在"*init.lua*"中将\
"`disable_lua`"设置为"*true*"，Wireshark将不会继续加载lua脚本插件，并关闭lua引擎。

如果Wireshark以suexec的形式运行的，在加载任何其它脚本前，都会检查变量\
"*run_user_scripts_when_superuser*"是否为"*true*"。否则，加载个人插件目录中的\
"*init.lua*"，并且通过以命令行"``-X lua_script:xxx.lua``"形式按顺序运行。

一个简单的例子
==============

.. code-block:: lua

    -- trivial protocol example
    -- declare our protocol
    trivial_proto = Proto("trivial","Trivial Protocol")
    -- create a function to dissect it
    function trivial_proto.dissector(buffer,pinfo,tree)
        pinfo.cols.protocol = "TRIVIAL"
        local subtree = tree:add(trivial_proto,buffer(),"Trivial Protocol Data")
        subtree:add(buffer(0,2),"The first two bytes: " .. buffer(0,2):uint())
        subtree = subtree:add(buffer(2,2),"The next two bytes")
        subtree:add(buffer(2,1),"The 3rd byte: " .. buffer(2,1):uint())
        subtree:add(buffer(3,1),"The 4th byte: " .. buffer(3,1):uint())
    end
    -- load the udp.port table
    udp_table = DissectorTable.get("udp.port")
    -- register our protocol to handle udp port 7777
    udp_table:add(7777,trivial_proto)

Lua Dissector的结构
=====================
对于一个简单的Dissector，通常结构如下：
*   必须注册为一个处理其它协议负载的类型
*   为了注册，dissector必须分配一个"*Proto Object*"
*   当Dissector被Wireshark调用时，会向Dissector传递三个参数：

    1.  包含负载的TVB Buffer(Tvb Object)
    2.  网络包相关信息记录(Pinfo对象)
    3.  树的根(TreeItem Object)

*   Dissector只在网络包匹配到"*DissectorTable*"中的"Proto"时，或者用户强制使用"*Decode As*"\
    来解析时才能被调用

依照上面的例子，写了一个简单分析游戏协议的Dissector:

.. code-block:: lua

    local ares_proto = Proto("Ares", "Ares Proto")

    function ares_proto.dissector(buffer, pinfo, root)
        pinfo.cols.protocol = "Ares Proto"

        local subtree = root:add(ares_proto, buffer(), "Ares Proto Data")
        subtree:add(buffer(0,1), "SYNC CODE1: " .. buffer(0,1):uint())
        subtree:add(buffer(1,1), "SYNC CODE2: " .. buffer(1,1):uint())
        subtree:add(buffer(2,2), "Lenght: " .. buffer(2,2):le_int())
        subtree = subtree:add(buffer(4,0), "Message:")
        subtree:add(buffer(4,2), "Message Type: " .. getMessageName(buffer(4,2):le_int()))
    end

    function getMessageName(msgId)
        if msgId == 0x6001 then
            return "MsgLoginCharacterResult"
        elseif msgId == 0x6040 then
            return "MsgCharacterUID"
        elseif msgId == 0x6101 then
            return "MsgEnterDungeon"
        elseif msgId == 0x6102 then
            return "MsgOtherEnterDungeon"
        elseif msgId == 0x6103 then
            return "MsgOtherExitDungeon"
        elseif msgId == 0x6104 then
            return "MsgSelectHero"
        elseif msgId == 0x6106 then
            return "MsgCountDownOver"
        elseif msgId == 0x6107 then
            return "MsgEnterCombat"
        elseif msgId == 0x6108 then
            return "MsgOtherEnterCombat"
        elseif msgId == 0x6109 then
            return "MsgEnterCombatFailed"
        elseif msgId == 0x610a then
            return "MsgEndCombat"
        elseif msgId == 0x610b then
            return "MsgSetCountDown"
        end

        return "Unknow: " .. msgId
    end

    tcp_table = DissectorTable.get("tcp.port")
    tcp_table:add(8283, ares_proto)

其它事项
=========
*   Dissector分析包数据时，要考虑各种包不完整情形的处理。
*   读取BUFFER中数据时，注意字节序，注意有些转换Lua并不支持。

参考资料
========
*   `Tutorial scripts <https://wiki.wireshark.org/Lua/Examples>`_
*   `Dissectors <https://wiki.wireshark.org/Lua/Dissectors>`_
*   `Chapter 10. Lua Support in Wireshark <https://www.wireshark.org/docs/wsdg_html_chunked/wsluarm.html>`_
*   `Chapter 11. Wireshark’s Lua API Reference Manual <https://www.wireshark.org/docs/wsdg_html_chunked/wsluarm_modules.html>`_
*   `Lua in Wireshark <https://wiki.wireshark.org/Lua#Lua_in_Wireshark>`_
*   `Tvb <https://wiki.wireshark.org/LuaAPI/Tvb#Tvb>`_
