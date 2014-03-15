抓包-libpcap
***************

简单实例
=========
.. sourcecode:: c

    #include <stdio.h>
    #include <stdlib.h>
    #include <pcap.h>

    void
    handle_packets(u_char *user, const struct pcap_pkthdr *h, const u_char *packet)
    {
        fprint("capture a packet.\n");
    }

    int
    main(int argc, char **argv)
    {
        char errbuff[PCAP_ERRBUF_SIZE];
        char *dev;

        if ((dev = pcap_lookupdev(errbuff)) == NULL)
        {
            fprintf(stderr, "Failed to find active network interface!");
            exit(EXIT_FAILURE);
        }

        pcap_t *p;

        if ((p = pcap_open_live(dev, 1518, -1, 1000, errbuff)) ==NULL)
        {
            fprintf(stderr, "Failed to open the interface: %s!", dev);
            exit(EXIT_FAILURE);
        }

        struct bpf_program bfp;
        char *fp = "ip";

        if (pcap_compile(p, &bfp, fp, 0, PCAP_NETMASK_UNKNOWN) != 0)
        {
            fprintf(stderr, "Failed to compile the bpf expression: %s!", pcap_geterr(p));
            exit(EXIT_FAILURE);
        }

        if (pcap_setfilter(p, &bfp) != 0)
        {
            fprintf(stderr, "Failed to set the bpf expression: %s!", pcap_geterr(p));
            exit(EXIT_FAILURE);
        }

        pcap_loop(p, 10, handle_packets, NULL);

        pcap_freecode(&fp);
        pcap_close(p);

        return 0;
    }


基本步骤
=========
利用\ **libpcap**\ 抓包包含下列基本步骤：

* 设置或查找网络接口设备，调用函数\ ``pcap_lookupdev``\ 查找当然活动的网络接口

.. sourcecode:: c

    #include <pcap/pcap.h>
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // errbuf   存储出错信息
    // 函数返回网络接口名，如eth0, em0等; 如果出错返回NULL
    char *pcap_lookupdev(char *errbuf);

    
* 调用函数\ ``pcap_open_live``\ 打开网络接口，进行侦听

.. sourcecode:: c

    #include <pcap/pcap.h>
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // 打开网络接口准备侦听
    // device       设备名，可以自己设定；也可以从第一步返回。如果设定为"any"或
    //              "NULL"，则会侦听所有接口
    // snaplen      捕捉包的最大长度
    // promisc      是否将网络设备设置为“混杂”模式。
    // to_ms        超时时间
    // errbuf       存储出错信息
    pcap_t *pcap_open_live(const char *device, int snaplen, int promisc,
                                                    int to_ms, char *errbuf);

* 设定过滤器。调用函数\ ``pcap_compile``\ 和\ ``pcap_setfilter``\ 来设定BFP过\
  滤器

.. sourcecode:: c

    #include <pcap/pcap.h>

    // p            是由函数pcap_open_live返回的指针
    // fp           用于存放编译后的BPF
    // str          bpf表达式
    // optimize     是否对bpf进行优化
    // netmask      一个IPV4的网络掩码，仅当bpf表达式中要过滤IP广播时有用，如果
    //              设定为PCAP_NETMASK_UNKNOWN，将忽略广播
    //
    //  成功将返回0,失败返回-1, 失败时可以通调用函数pcap_geterr(p),
    //  pcap_perror(p)获取详细的出错信息
    int pcap_compile(pcap_t *p, struct bpf_program *fp,
                          const char *str, int optimize, bpf_u_int32 netmask);

    // 设定过滤器
    // 成功返回0, 失败返回-1, 同样可以用函数pcap_geterr, pcap_perror获取出错信息
    int pcap_setfilter(pcap_t *p, struct bpf_program *fp);


* 开始抓包。调用函数\ ``pcap_loop``\ ，根据自己的需求编写处理数据包的回调函数。

.. sourcecode:: c

    #include <pcap/pcap.h>

    /* pcap_loop 启动对数据的处理，可以是来自一个已经将打开的网络接口，也可以是
     * 保存在磁盘上的数据文件，当抓取cnt个数据包，或者到外文件的结尾(从文件中读
     * 取数据时），或者调用了pcap_breakloop()，或者发生错误，pcap_loop会退出。当
     * 读取数据时超时，pcap_loop并不会返回；
     *
     * cnt          处理数据包的个数。设定为0或-1表示无限；注意对于一些低版本
     *              libpcap，或者在不同平台上，"0"可能是未定义的，只有"-1"才表
     *              示无穷多
     * callback     回调函数，当pcap收到数据后就会调用此函数对数据进行处理
     * user         将传递给回调函数的第一参数
     */
    int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user);

    /* pcap_dispatch 与pcap_loop作用类似,不过不太明白。测试时发现pcap_loop貌似应
     * 该是阻塞形式，直到处理了cnt个数据包才会返回，而相同程序pcap_dispatch可能一
     * 个包也没处理就返回了，正如文档中描述的最多处理cnt个数据包。
     *
     * pcap_dispatch() processes packets from a live capture or ``savefile''
     * until cnt packets are processed, the end  of  the current  bufferful of
     * packets is reached when doing a live capture, the end of the ``savefile''
     * is reached when reading from a ``savefile'', pcap_breakloop() is called,
     * or an error occurs.  Thus, when doing a live capture, cnt is the maximum
     * number  of packets to process before returning, but is not a minimum
     * number; when reading a live capture, only one bufferful of packets is
     * read at a time, so fewer than cnt packets may be processed. A value of -1
     * or 0 for  cnt  causes all  the  packets received in one buffer to be
     * processed when reading a live capture, and causes all the packets in the
     * file to be processed when reading a ``savefile''.
     *
     * 返回值为“0”说明正常返回，“-1”说明发生了错误，”-2“说明是调用pcap_breakloop
     * 中止的sniff,建议详细确认返回值。
     */
    int pcap_dispatch(pcap_t *p, int cnt, pcap_handler callback, u_char *user);

    /* 定义一个回调函数来处理每个被捕捉到的数据包
     * 第一个参数user由pcap_loop或pcap_dispatch第三个参数传递而来
     * 第二个参数h指向一个pcap_pkthdr结构体，其中包含了捕捉到的数据包的相关信息，
     *      如时间戳，大小等。此结构体不能被主动释放，也不保证回调函数返回后仍然
     *      可用，如果希望可以，请在返回之前拷贝一份。
     * 第三个参数用于指定捕捉一个数据包的最大bytes数
     */
    typedef void (*pcap_handler)(u_char *user, const struct pcap_pkthdr *h,
                                                        const u_char *bytes);

    struct pcap_pkthdr {
    	struct timeval ts;	/* time stamp */
    	bpf_u_int32 caplen;	/* length of portion present */
    	bpf_u_int32 len;	/* length this packet (off wire) */
    };

包处理
=======
关于数据包的处理，需要详细的了解TCP/IP协议簇，清楚的知道每层协议的头部结构，还需
要知道\ **网络字节序**\ 和\ **主机字节序**\ 等基本知识。至于数据如何处理，根据应
用需求，涉及的知识可能更多。

TCP, UDP等协议在IP头部中的标示定义在\ `/usr/include/netinet/in.h`\ 中


