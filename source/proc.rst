文件系统/proc
*************

``man proc``\ 可以查看\ ``/proc``\ 下文件的介绍。部分详细的信息在\ ``Document/``
下的文档。


``/proc/stat``
==============
内容如下：

.. sourcecode:: text

    cpu  2303587 581 429836 17770999 23613 6 3208 0 0 0
    cpu0 295930 14 54832 2211637 3243 0 460 0 0 0
    cpu1 290937 7 53366 2219764 2343 0 339 0 0 0
    cpu2 286404 398 54243 2222236 3039 0 400 0 0 0
    cpu3 289395 56 53009 2221468 2625 0 355 0 0 0
    cpu4 287492 65 54035 2221863 2796 0 389 0 0 0
    cpu5 283755 12 53378 2226847 2756 0 352 0 0 0
    cpu6 284510 12 54328 2223049 3263 0 390 0 0 0
    cpu7 285162 13 52642 2224133 3546 1 521 0 0 0
    intr 90553713 131 3 0 0 0 0 0 1 1 0 0 0 3 0 0 0 2263 3 172382 258726 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 69 2048202 1465341 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    ctxt 114753645
    btime 1395311332
    processes 527044
    procs_running 2
    procs_blocked 0
    softirq 53396993 13 36920330 4298 2049696 258348 0 182581 1762075 72888 12146764



``/proc/uptime``
================
内容如下：

.. sourcecode:: text

    25864.56 179317.80

*   第一个字段：系统启动时间
*   第二个字段：CPU空闲时间，所有CPU的空闲时间之和

``/proc/loadavg``
=================
内容如下：

.. sourcecode:: text

    0.17 0.45 1.85 3/431 13306

前三个字段分别表示系统在1, 5, 15分钟内的负载，负载是根据进程队列和磁盘IO等待计\
算得到。其值与\ ``uptime``\ 输出的平均负载值。

第四个字段，“\ ``/``\ ”前面的数字表示当前正在运行的进程，线程数；后面的数字表示\
当前内核调度队列中的进程数。

最后一个字段为最近创建进程的PID。


``/proc/diskstats``
===================
内容如下：

.. sourcecode:: text

      1       0 ram0 0 0 0 0 0 0 0 0 0 0 0
      1       1 ram1 0 0 0 0 0 0 0 0 0 0 0
      1       2 ram2 0 0 0 0 0 0 0 0 0 0 0
      1       3 ram3 0 0 0 0 0 0 0 0 0 0 0
      1       4 ram4 0 0 0 0 0 0 0 0 0 0 0
      1       5 ram5 0 0 0 0 0 0 0 0 0 0 0
      1       6 ram6 0 0 0 0 0 0 0 0 0 0 0
      1       7 ram7 0 0 0 0 0 0 0 0 0 0 0
      1       8 ram8 0 0 0 0 0 0 0 0 0 0 0
      1       9 ram9 0 0 0 0 0 0 0 0 0 0 0
      1      10 ram10 0 0 0 0 0 0 0 0 0 0 0
      1      11 ram11 0 0 0 0 0 0 0 0 0 0 0
      1      12 ram12 0 0 0 0 0 0 0 0 0 0 0
      1      13 ram13 0 0 0 0 0 0 0 0 0 0 0
      1      14 ram14 0 0 0 0 0 0 0 0 0 0 0
      1      15 ram15 0 0 0 0 0 0 0 0 0 0 0
      7       0 loop0 0 0 0 0 0 0 0 0 0 0 0
      7       1 loop1 0 0 0 0 0 0 0 0 0 0 0
      7       2 loop2 0 0 0 0 0 0 0 0 0 0 0
      7       3 loop3 0 0 0 0 0 0 0 0 0 0 0
      7       4 loop4 0 0 0 0 0 0 0 0 0 0 0
      7       5 loop5 0 0 0 0 0 0 0 0 0 0 0
      7       6 loop6 0 0 0 0 0 0 0 0 0 0 0
      7       7 loop7 0 0 0 0 0 0 0 0 0 0 0
      8       0 sda 164471 79897 3068268 487297 97761 384460 18959662 6259738 0 300411 6760070
      8       1 sda1 126 110 1888 746 0 0 0 0 0 719 743
      8       2 sda2 119 110 1840 836 0 0 0 0 0 729 835
      8       3 sda3 231 6955 9560 2234 0 0 0 0 0 1692 2234
      8       4 sda4 2 0 20 199 0 0 0 0 0 199 199
      8       5 sda5 232 6969 9560 1532 0 0 0 0 0 1386 1531
      8       6 sda6 240 6984 9632 1228 0 0 0 0 0 1143 1228
      8       7 sda7 224 505 2942 1614 23 42 142 171 0 1586 1784
      8       8 sda8 81930 54409 1278120 121542 78503 380665 18713064 4852751 0 135538 4987423
      8       9 sda9 81100 3425 1750898 355434 17764 3753 246456 1378816 0 159504 1734162
      8      10 sda10 152 320 2008 919 0 0 0 0 0 881 918
      8      16 sdb 380 336 5728 494 0 0 0 0 0 433 493
      8      17 sdb1 198 216 3312 168 0 0 0 0 0 163 168
      8      18 sdb2 136 110 1968 199 0 0 0 0 0 195 199
    253       0 dm-0 130 0 1040 588 0 0 0 0 0 530 588
    253       1 dm-1 130 0 1040 849 0 0 0 0 0 720 849
    253       2 dm-2 163 0 1304 1532 0 0 0 0 0 469 1532
    253       3 dm-3 442 0 3888 1336 10 0 64 907 0 1999 2243
    253       4 dm-4 135282 0 1269104 227359 474569 0 18713000 63707752 0 157087 63945996
    253       5 dm-5 130 0 1040 1613 0 0 0 0 0 308 1613


``/proc/meminfo``
=================
内容如下：

.. sourcecode:: text

    MemTotal:       12265380 kB
    MemFree:          991860 kB
    Buffers:          630796 kB
    Cached:          8175496 kB
    SwapCached:            0 kB
    Active:          6199424 kB
    Inactive:        3680280 kB
    Active(anon):    1245952 kB
    Inactive(anon):   127296 kB
    Active(file):    4953472 kB
    Inactive(file):  3552984 kB
    Unevictable:           0 kB
    Mlocked:               0 kB
    SwapTotal:       2047996 kB
    SwapFree:        2047996 kB
    Dirty:               240 kB
    Writeback:             0 kB
    AnonPages:       1073496 kB
    Mapped:           330912 kB
    Shmem:            299848 kB
    Slab:            1114644 kB
    SReclaimable:    1016448 kB
    SUnreclaim:        98196 kB
    KernelStack:        3504 kB
    PageTables:        20868 kB
    NFS_Unstable:          0 kB
    Bounce:                0 kB
    WritebackTmp:          0 kB
    CommitLimit:     8180684 kB
    Committed_AS:    3424504 kB
    VmallocTotal:   34359738367 kB
    VmallocUsed:      312904 kB
    VmallocChunk:   34359418620 kB
    HugePages_Total:       0
    HugePages_Free:        0
    HugePages_Rsvd:        0
    HugePages_Surp:        0
    Hugepagesize:       2048 kB
    DirectMap4k:      154760 kB
    DirectMap2M:     3995648 kB
    DirectMap1G:     8388608 kB



``/proc/partitions``
====================

.. sourcecode:: text

    major minor  #blocks  name
    
       8        0 1953514584 sda
       8        1     102400 sda1
       8        2   20377600 sda2
       8        3  102400000 sda3
       8        4          1 sda4
       8        5  102400000 sda5
       8        6  102400000 sda6
       8        7     102400 sda7
       8        8  307200000 sda8
       8        9   29294496 sda9
       8       10  200001186 sda10
       8       16  312571224 sdb
       8       17     131072 sdb1
       8       18  312438784 sdb2
     253        0    5120000 dm-0
     253        1   10240000 dm-1
     253        2    2048000 dm-2
     253        3   52428800 dm-3
     253        4   52428800 dm-4
     253        5   10485760 dm-5



``/proc/cpuinfo``
=================

.. sourcecode:: text

    processor	: 0
    vendor_id	: AuthenticAMD
    cpu family	: 21
    model		: 2
    model name	: AMD FX(tm)-8320 Eight-Core Processor
    stepping	: 0
    microcode	: 0x6000817
    cpu MHz		: 1400.000
    cache size	: 2048 KB
    physical id	: 0
    siblings	: 8
    core id		: 0
    cpu cores	: 4
    apicid		: 16
    initial apicid	: 0
    fpu		: yes
    fpu_exception	: yes
    cpuid level	: 13
    wp		: yes
    flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good nopl nonstop_tsc extd_apicid aperfmperf pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 popcnt aes xsave avx f16c lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs xop skinit wdt lwp fma4 tce nodeid_msr tbm topoext perfctr_core perfctr_nb arat cpb hw_pstate npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold bmi1
    bogomips	: 6984.54
    TLB size	: 1536 4K pages
    clflush size	: 64
    cache_alignment	: 64
    address sizes	: 48 bits physical, 48 bits virtual
    power management: ts ttp tm 100mhzsteps hwpstate cpb eff_freq_ro
    
    processor	: 1
    vendor_id	: AuthenticAMD
    cpu family	: 21
    model		: 2
    model name	: AMD FX(tm)-8320 Eight-Core Processor           
    stepping	: 0
    microcode	: 0x6000817
    cpu MHz		: 1400.000
    cache size	: 2048 KB
    physical id	: 0
    siblings	: 8
    core id		: 1
    cpu cores	: 4
    apicid		: 17
    initial apicid	: 1
    fpu		: yes
    fpu_exception	: yes
    cpuid level	: 13
    wp		: yes
    flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good nopl nonstop_tsc extd_apicid aperfmperf pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 popcnt aes xsave avx f16c lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs xop skinit wdt lwp fma4 tce nodeid_msr tbm topoext perfctr_core perfctr_nb arat cpb hw_pstate npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold bmi1
    bogomips	: 6984.54
    TLB size	: 1536 4K pages
    clflush size	: 64
    cache_alignment	: 64
    address sizes	: 48 bits physical, 48 bits virtual
    power management: ts ttp tm 100mhzsteps hwpstate cpb eff_freq_ro


``/proc/[pid]``
===============


``cmdline``
-----------



``cwd``
-------



``environ``
-----------


``exe``
-------

``io``
------


参考资源
========
