配置Zabbix
***************

Log File Monitoring时ZBX_NOTSUPPORT错误
=======================================
配置日志文件监控时一直出错，经过在Zabbix Forums上的提醒\ [#r1]_\ ，仔细查看了agent\
的调试日志，在其中发现：

.. sourcecode:: text

    # ACTIVE CHECK 主机上没有查询时，返回的data为空。


    2526:20130418:020747.866 refresh_active_checks('lab.liuhui.xmu',10051)
    2525:20130418:020747.867 agent #1 started [listener]
    2526:20130418:020747.867 Sending [{
      "host":"node01.liuhui.xmu",
      "ip":"192.168.122.11"}]
    2526:20130418:020747.868 Before read
    2524:20130418:020747.868 agent #0 started [collector]
    2524:20130418:020747.868 In init_cpu_collector()
    2524:20130418:020747.868 End of init_cpu_collector():SUCCEED
    2524:20130418:020747.868 In update_cpustats()
    2524:20130418:020747.868 End of update_cpustats()
    2526:20130418:020747.869 Got [{
      "response":"success",
      "data":[]}]
    2526:20130418:020747.869 In parse_list_of_checks()
    2526:20130418:020747.869 In disable_all_metrics()
    2526:20130418:020747.869 In process_active_checks('lab.liuhui.xmu',10051)
    2526:20130418:020747.869 End of process_active_checks()

    # 这是另外一个ACTIVE CHECK日志，此时我定义了一个日志监视的item
    # 可以发现agent已经开始处理日志文件，但是没有相应的权限，最终出错，返回
    # ZBX_NOTSUPPORT

    4320:20130418:165640.307 refresh_active_checks('lab.liuhui.xmu',10051)
    4320:20130418:165640.308 Sending [{
        "request":"active checks",
        "host":"node01.liuhui.xmu",
        "ip":"192.168.122.11"}]
    4320:20130418:165640.309 Before read
    4320:20130418:165640.310 Got [{
        "response":"success",
        "data":[
                {
                    "key":"log[\/var\/log\/messages,,,,]",
                    "delay":30,
                    "lastlogsize":0,
                    "mtime":0}]}]
    4320:20130418:165640.310 In parse_list_of_checks()
    4320:20130418:165640.310 In disable_all_metrics()
    4320:20130418:165640.310 In add_check() key:'log[/var/log/messages,,,,]' refresh:30 lastlogsize:0 mtime:0
    4320:20130418:165640.310 End of add_check()
    4320:20130418:165640.310 In process_active_checks('lab.liuhui.xmu',10051)
    4320:20130418:165640.310 In process_log() filename:'/var/log/messages' lastlogsize:0
    4320:20130418:165640.310 cannot open '/var/log/messages': [13] Permission denied
    4320:20130418:165640.310 Active check [log[/var/log/messages,,,,]] is not supported. Disabled.
    4320:20130418:165640.310 In process_value() key:'node01.liuhui.xmu:log[/var/log/messages,,,,]' value:'ZBX_NOTSUPPORTED'

    # 再看看下面的日志，这是agent输出的一个成功的日志监控的调试日志
    # 与上面的主要差别在于，agent进程有权限访问server要求监控的日志文件

    3904:20130419:020817.682 refresh_active_checks('lab.liuhui.xmu',10051)        
    3904:20130419:020817.683 Sending [{                                           
      "request":"active checks",                                                  
      "host":"node01.liuhui.xmu",                                                 
      "ip":"192.168.122.11"}]                                                     
    3904:20130419:020817.683 Before read                                          
    3904:20130419:020817.684 Got [{                                               
      "response":"success",                                                       
      "data":[                                                                    
        {"key":"log[\/var\/log\/zabbix\/zabbix_agentd.log]",
         "delay":30,
         "lastlogsize":0,
         "mtime":0}]}]
    3904:20130419:020817.684 In parse_list_of_checks()
    3904:20130419:020817.684 In disable_all_metrics()
    3904:20130419:020817.684 In add_check() key:'log[/var/log/zabbix/zabbix_agentd.log]' refresh:30 lastlogsize:0 mtime:0
    3904:20130419:020817.684 End of add_check()     
    
    3904:20130419:020817.685 In process_active_checks('lab.liuhui.xmu',10051)
    3904:20130419:020817.685 In process_log() filename:'/var/log/zabbix/zabbix_agentd.log' last
    gsize:0
    3904:20130419:020817.685 In process_value() key:'node01.liuhui.xmu:log[/var/log/zabbix/zabb_agentd.log]' value:'  2058:20130417:235114.399 Starting Zabbix Agent [node01.liuhui.xmu].
    bbix 2.0.5 (revision 33558).'
    3904:20130419:020817.685 In send_buffer() host:'lab.liuhui.xmu' port:10051 values:0/100
    3904:20130419:020817.685 End of send_buffer():SUCCEED
    3904:20130419:020817.685 buffer: new element 0
    3904:20130419:020817.685 End of process_value():SUCCEED
    3904:20130419:020817.685 In process_log() filename:'/var/log/zabbix/zabbix_agentd.log' lastlogsize:101
    3904:20130419:020817.685 In process_value() key:'node01.liuhui.xmu:log[/var/log/zabbix/zabb_agentd.log]' value:'  2062:20130417:235114.420 agent #3 started [listener]'
    3904:20130419:020817.685 In send_buffer() host:'lab.liuhui.xmu' port:10051 values:1/100
    3904:20130419:020817.685 JSON before sending [{
      "request":"agent data",
      "data":[
          {
              "host":"node01.liuhui.xmu",
              "key":"log[\/var\/log\/zabbix\/zabbix_agentd.log]",
              "value":"  2058:20130417:235114.399 Starting Zabbix Agent [node01.liuhui.xmu]. Zabbix 2.0.5 (revision 33558).",
                "lastlogsize":101,
                "clock":1366308497,
                "ns":685378025}],
        "clock":1366308497,
        "ns":685485907}]
    3904:20130419:020817.686 JSON back [{
      "response":"success",
      "info":"Processed 1 Failed 0 Total 1 Seconds spent 0.000075"}]
    3904:20130419:020817.686 In check_response() response:'{
    ……

通过上面的日志分析，可以发现，server监视agent上的日志文件，需要agent进程可以读\
取相应的文件，否则会出错。关于监控日志的详细文档见Zabbix Manual [#r2]_\ [#r3]_

参考资料
=========
.. [#r1] https://www.zabbix.com/forum/showthread.php?t=23033
.. [#r2] https://www.zabbix.com/documentation/2.0/manual/config/items/itemtypes/log_items
.. [#r3] https://www.zabbix.com/documentation/2.0/manual/config/items/itemtypes/zabbix_agent#supported_item_keys


TODO List
=========
* 如何在agent上可以查询自身数据, 命令\ ``zabbix_agentd``\ 可以打印zabbix agent的\
  数据

.. sourcecode:: bash

    zabbix_agentd -p

* 使用IP/域名配置Server，agent
