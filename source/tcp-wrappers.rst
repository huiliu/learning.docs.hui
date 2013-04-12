简易防火墙TCP-Wrapper
#####################

控制文件的结构
**************

Each access control file consists of zero or more lines of text. These lines
are processed in order of appearance. The search terminates when a match is
found.

每个存取控制文件（/etc/hosts.allow, /etc/hosts.deny）由多行组成，并且是按顺序进\
行匹配处理的，并匹配到某一条后就会执行相应的操作，后面的规则将不再进行匹配（与\
iptables匹配模式一样）。

* A newline character is ignored when it is preceded by a backslash 
  character. This permits you to break up long lines so that they are easier to
  edit.
* 反斜杆“\"为续行符；

* Blank lines or lines that begin with a **#** character are ignored. This
  permits you to insert comments and whitespace so that the tables are easier to
  read.
* "#"开头的行为注释，空行将被忽略；
 
* All other lines should satisfy the following format, things between [] being
  optional:
* 所有其它行为匹配规则，应该满足下面的格式：（其中\ **[]**\ 表示其中内容为可选）

.. sourcecode:: bash

    daemon_list : client_list [ : shell_command ]

其中
    * daemon_list is a list of one or more daemon process names (argv[0] values)
      or server port numbers or wildcards (see below).
    * client_list is a list of one or more host names, host addresses, patterns 
      or wildcards (see below) that will be matched against the
      client host name or address.

    * **daemon_list**\ 是由一个或多个进程名，服务端口号、通配符组成的列表。
    * **client_list**\ 是由一个或多个主机名，主机地址，匹配模式或通配符构成的列表。

    The more complex forms daemon@host and user@host are explained in the
    sections
    on server endpoint patterns and on client username lookups, respectively.
    
    还有一些更复杂的形式，如daemon@host, user@host等

* List elements should be separated by blanks and/or commas.
* 列表元素用空格或者冒号”:“分隔

* With the exception of NIS (YP) netgroup lookups, all access control checks are
  case insensitive.
* NIS(YP)的匹配大小敏感，其它匹配规则都是大小写不敏感。
 

存取控制语法
************
The access control language implements the following patterns:

存取控制语法有以下规则：

* A string that begins with a "**.**" character. A host name is matched if the
  last components of its name match the specified pattern. For example, the
  pattern "*.tue.nl*" matches the host name "*wzv.win.tue.nl*".
* 以"**.**"开头的字符串，将进行域匹配。即：如果匹配规则为"*.tue.nl*"，它可以匹\
  配主机名"*wzv.win.tue.nl*"

* A string that ends with a "**.**" character. A host address is matched if its
  first numeric fields match the given string. For example, the pattern
  "*131.155.*" matches the address of (almost) every host on the Eindhoven
  University network (131.155.x.x).
* 以"."结尾的字符串，将对主机IP地址进行匹配。如果匹配规则为”\ *131.155.*\ “，它\
  将匹配”\ *131.155.0.0/16*\ “这个网段。

* A string that begins with an "**@**" character is treated as an NIS (formerly
  YP) netgroup name. A host name is matched if it is a host member of the
  specified netgroup. Netgroup matches are not supported for daemon process
  names or for client user names.
* 以“\ **@**\ “开头的字符串，被作用NIS(YP)网络组名处理。属于指定网络组的主机将\
  被匹配。网络组匹配规则不支持daemon进程名和客户用户名。

* An expression of the form **n.n.n.n/m.m.m.m** is interpreted as a **net/mask**
  pair. An IPv4 host address is matched if **net** is equal to the bitwise AND
  of the address and the **mask**. For example, the net/mask pattern **131.155.7
  2.0/255.255.254.0** matches every address in the range **131.155.72.0**
  through **131.155.73.255**. **255.255.255.255** is not a valid mask value, so
  a single host can be matched just by its IP.

* An expression of the form **n.n.n.n/mm** is interpreted as a
  **net/masklength** pair, where **mm** is the number of consecutive **1** bits
  in the netmask applied to the **n.n.n.n** address.

* An expression of the form **[n:n:n:n:n:n:n:n]/m** is interpreted as a
  **[net]/prefixlen** pair. An IPv6 host address is matched if **prefixlen**
  bits of **net** is equal to the **prefixlen** bits of the address. For example
  , the [net]/prefixlen pattern **[3ffe:505:2:1::]/64** matches every address in
  the range **3ffe:505:2:1::** through **3ffe:505:2:1:ffff:ffff:ffff:ffff**.

* "**n.n.n.n/m.m.m.m**", "**n.n.n.n/mm**", "**[n:n:n:n:n:n:n:n]/m**"形式的字符\
  串均是匹配IP地址的规则。如：规则`\ **131.155.72.0/255.255.254.0**\ ´将匹配`\
  **131.155.72.0**\ ´到`\ **131.155.73.255**\ ´之间的所有IP。规则“\ **[3ffe:505:
  2:1::]/64**\ ”将匹配所有在“\ **3ffe:505:2:1::**\ ”到"\ **3ffe:505:2:1:ffff:\
  ffff:ffff:ffff**\ "之间的IP。

* A string that begins with a **/** character is treated as a file name. A host
  name or address is matched if it matches any host name or address pattern
  listed in the named file. The file format is zero or more lines with zero or
  more host name or address patterns separated by whitespace. A file name
  pattern can be used anywhere a host name or address pattern can be used.

* 以“\ **/**\ ”开头的字符串将作为一个文件处理。文件由零行或多行主机名、IP地址组\
  成，只要匹配文件中任意一条规则，当前规则即被匹配（类似于iptables中的自定义链）。

* Wildcards **\*** and **?** can be used to match hostnames or IP addresses.
  This method of matching cannot be used in conjunction with **net/mask**
  matching, hostname matching beginning with "**.**" or IP address matching
  ending with "**.**".
* 通配符”\ **\***\ ”和”\ **?**\ ”可以被用于匹配主机名或IP地址。不能与其它语法组\
  合使用，即不能用于以"."开头或结尾的字符串，不能用”网络/子网掩码“中。


通配符
------
WILDCARDS
The access control language supports explicit wildcards:

存取控制语法（语言）支持显式的通配符：
 
+----------+-------------------------------------------------------------------+
| 符号     | 意义                                                              |
+==========+===================================================================+
| ALL      | 通用匹配符，匹配一切。                                            |
+----------+-------------------------------------------------------------------+
| LOCAL    | 匹配所有主机名不包括“."的主机                                     |
+----------+-------------------------------------------------------------------+
| UNKNOWN  | 匹配用户名未知的用户；匹配主机名或网络地址未知的主机。这种模式应  |
|          | 该小心使用：主机名未知可能是因为DNS的原因，网络地址未知可能是软件 |
|          | 不知道软件的类型。                                                |
+----------+-------------------------------------------------------------------+
| KNOWN    | 匹配用户名已知的用户；匹配主机名或网络地址已知的主机。这种模式应  |
|          | 该小心使用：主机名未知可能是因为DNS的原因，网络地址未知可能是     |
+----------+-------------------------------------------------------------------+
| PARANOID | 匹配主机名与地址不匹配的主机。当tcpd包含-DPARANOID（默认情况）    |
|          | 时，将丢弃这类请求，即便在存取控制表中包含有处理规则；如果想手动  |
|          | 更加细致的控制，请不要添加-DPARANOID。                            |
+----------+-------------------------------------------------------------------+

 ALL The universal wildcard, always matches.

 LOCAL Matches any host whose name does not contain a dot character.

 UNKNOWN
 Matches any user whose name is unknown, and matches any host whose name or address are unknown. This pattern should be used
 with care: host names may be unavailable due to temporary name server problems. A network address will be unavailable when the
 software cannot figure out what type of network it is talking to.

 KNOWN Matches any user whose name is known, and matches any host whose name and address are known. This pattern should be used with
 care: host names may be unavailable due to temporary name server problems. A network address will be unavailable when the soft‐
 ware cannot figure out what type of network it is talking to.

 PARANOID
 Matches any host whose name does not match its address. When tcpd is built with -DPARANOID (default mode), it drops requests from such clients even before looking at the access control tables. Build without -DPARANOID when you want more control over such requests.


操作
-----
OPERATORS
* **EXCEPT** Intended use is of the form: *list_1 EXCEPT list_2*; this construct  matches anything that matches *list_1* unless it matches *list_2*. The **EXCEPT** operator can be used in *daemon_lists* and in *client_lists*. The **EXCEPT** operator can be nested: if the control language would permit the use of parentheses, *a EXCEPT b EXCEPT c* would parse as *(a EXCEPT (b EXCEPT c))*.

* **EXCEPT** 一般使用形式为：\ *List_1 EXCEPT List_2*\ 。它将匹配列表1中除列表2\
  外的所有项。\ **EXCEPT**\ 操作可以用于\ *daemon_lists*\ 和\ *client_lists*\ 。  也可以使用嵌套形式，如： *a EXCEPT b EXCEPT c*\ 被解释为\ *(a EXCEPT (b EXCEPT
  c))*\ 。


Shell命令
---------

SHELL COMMANDS
If the first-matched access control rule contains a shell command, that command is subjected to %<letter> substitutions (see next section). The result is executed by a /bin/sh child process with standard input, output and error connected to /dev/null. Specify an **&** at the end of the command if you do not want to wait until it has completed.

Shell commands should not rely on the PATH setting of the inetd. Instead, they should use absolute path names, or they should begin with an explicit PATH=whatever statement.
 
 The hosts_options(5) document describes an alternative language that uses the shell command field in a different and incompatible way.

 如果第一个匹配的存取控制规则包含一个shell命令，

 Shell命令不依赖于inetd中的PATH设定。应该直接使用绝对路径或者用PATH=显式的设定路径。


**%**\ 扩展表达式
^^^^^^^^^^^^^^^^^^^
Shell命令中可以使用下面的扩展表达：

+--------+---------------------------------------------------------------------+
| 扩展符 | 意义                                                                |
+========+=====================================================================+
| %a(%A) | 客户（服务器）地址                                                  |
+--------+---------------------------------------------------------------------+
| %c     | 客户信息：user@host, user@address。是主机名还是地址取决于那种信息可 |
|        | 以取得。                                                            |
+--------+---------------------------------------------------------------------+
| %d     | 守护进程名（argv[0]的值）                                           |
+--------+---------------------------------------------------------------------+
| %h(%H) | 客户（服务器）主机名，当主机名不可得的时候为地址                    |
+--------+---------------------------------------------------------------------+
| %n(%N) | 客户（服务器）的主机名（或为”unknown“，"paranoid"）                 |
+--------+---------------------------------------------------------------------+
| %r(%R) | 客户（服务器）的端口号（或为0）                                     |
+--------+---------------------------------------------------------------------+
| %p     | 守护进程的PID                                                       |
+--------+---------------------------------------------------------------------+
| %s     | 服务器信息：daemon@host, daemon@address或者只是一个守护进程名，取决 |
|        | 于什么信息可以取得                                                  |
+--------+---------------------------------------------------------------------+
| %u     | 客户用户名（或为"unknown"）                                         |
+--------+---------------------------------------------------------------------+
| %%     | 表示字符%                                                           |
+--------+---------------------------------------------------------------------+
 
Characters in % expansions that may confuse the shell are replaced by underscores.

如果%扩展中的字符如果在Shell会引起混淆，可以使用下划线代替。

服务器端匹配
------------
In order to distinguish clients by the network address that they connect to, use
patterns of the form:

为了区分来自不同网段的客户，可以使用下面的匹配模式：

.. sourcecode:: bash

    process_name@host_pattern : client_list ...

Patterns like these can be used when the machine has different internet
addresses with different internet hostnames. Service providers can use this
facility to offer FTP, GOPHER or WWW archives with internet names that may even 
belong to different organizations. See also the *twist* option in the
*hosts_options(5)* document. Some systems (Solaris, FreeBSD) can have more than
one internet address on one physical interface; with other systems you may have
to resort to SLIP or PPP pseudo interfaces that live in a dedicated network
address space.

上面的匹配模式可以用于匹配来自不同网段，不同主机的访问。服务提供者可以使用这个\
装置提供FTP, GOPHER, WWW等互联网服务名，这些服务未必是一个组织提供的。详细请查\
看文档hosts_options(5)的twist选项。如果系统中一个物接口上绑定了不止一个IP地址，\
你可能需要排虚拟接口的顺序。

The host_pattern obeys the same syntax rules as host names and addresses in
*client_list* context. Usually, server endpoint information is available only
with connection-oriented services.

**host_pattern**\ 使用主机名，IP地址的规则与\ **client_list**\ 中相同。只有是面\
向连接的服务才可以获得服务器端信息。


客户用户名查找
^^^^^^^^^^^^^^^
CLIENT USERNAME LOOKUP

When the client host supports the RFC 931 protocol or one of its descendants
(TAP, IDENT, RFC 1413) the wrapper programs can retrieve additional information
about the owner of a connection. Client username information, when available,
is logged together with the client host name, and can be used to match patterns
like:

.. sourcecode:: bash

    daemon_list : ... user_pattern@host_pattern ...


当客户端主机支持RFC931协议或其它衍生协议（TAP, IDENT, RFC1413），wrapper程序就\
可以取得关于连接所属用户等信息。当客户主机名，用户信息可以取得时，可以使用上面\
的匹配模式：


The daemon wrappers can be configured at compile time to perform rule-driven username lookups (default) or to always interrogate the client host. In the case of rule-driven username lookups, the above rule would cause username lookup only when both the daemon_list and the host_pattern match.

守护进程wrapper在编译时可以指定是否进行规则驱动查找（默认是进行）。在使用规则驱动用户名查找时，只有当同时匹配了守护进程名和主机匹配规则host_pattern时，才会进行用户查找。

A user pattern has the same syntax as a daemon process pattern, so the same wildcards apply (netgroup membership is not supported). One should not get carried away with username lookups, though.
用户匹配模式的语法与守护进程匹配模式的语法相同，如果使用通配符（网络组成员不可使用通配符匹配），将不会进行用户名查找。(翻译可能有问题)

* The client username information cannot be trusted when it is needed most, i.e. when the client system has been compromised. In general, ALL and (UN)KNOWN are the only user name patterns that make sense.

* Username lookups are possible only with TCP-based services, and only when the client host runs a suitable daemon; in all other cases the result is "unknown".

* A well-known UNIX kernel bug may cause loss of service when username lookups are blocked by a firewall. The wrapper README document describes a procedure to find out if your kernel has this bug.

* Username lookups may cause noticeable delays for non-UNIX users. The default timeout for username lookups is 10 seconds: too short to cope with slow networks, but long enough to irritate PC users.

* 当月客户机被入侵，客户用户名信息是不可信的。通常，ALL和(UN)KNOWN是唯一有意义的用户名匹配模式。
* 只有基于TCP的服务，同时客户端运行着合适的守护进程，才能进行用户名查找；其它情况查找结果都将是unknown。
* 一个有名的UNIX内核BUG可能在用户查找被防火墙阻挡时导致服务丢失，请仔细阅读相关文档确认是否包含此BUG

选择时的用户名查找可以减少这个问题的影响，例如，下面这样一条规则：

.. sourcecode:: bash

    daemon_list : @pcnetgroup ALL@ALL

将不进行用户查找直接匹配所有pcnetgroup上的用户，但是对所有其它系统会进行用户名查找。（意思是保证主机pcnetgroup上用户正常访问，其它系统则不保证，可能会因为内核BUG出现问题？）

Selective username lookups can alleviate the last problem. For example, a rule like:

.. sourcecode:: bash

    daemon_list : @pcnetgroup ALL@ALL

would match members of the pc netgroup without doing username lookups, but would perform username lookups with all other systems.
 

检测地址欺骗攻击
^^^^^^^^^^^^^^^^

DETECTING ADDRESS SPOOFING ATTACKS

A flaw in the sequence number generator of many TCP/IP implementations allows intruders to easily impersonate trusted hosts and to break in via, for example, the remote shell service. The IDENT (RFC931 etc.) service can be used to detect such and other host address spoofing attacks.

由于很多TCP/IP服务中的序号生成器存在缺陷，所以很容易被人伪装成可信主机通过远程shell等服务侵入系统。IDENT（RFC931等）服务可以用于检测这类和其它一些主机IP欺骗攻击。

* Before accepting a client request, the wrappers can use the IDENT service to find out that the client did not send the request at all. When the client host provides IDENT service, a negative IDENT lookup result (the client matches *UNKNOWN@host*) is strong evidence of a host spoofing attack.

* A positive IDENT lookup result (the client matches *KNOWN@host*) is less trustworthy. It is possible for an intruder to spoof both the client connection and the IDENT lookup, although doing so is much harder than spoofing just a client connection. It may also be that the client´s IDENT server is lying.

* Note: IDENT lookups don´t work with UDP services.

* 在接受一个客户的请求之前，wrapper会使用IDENT服务来查找客户是否已经发送过请求。当客户主机提供IDENT服务时，一个负的IDENT查找结果（客户匹配UNKNOWN@host）表明这是一个主机IP欺骗攻击。
* 一个正的IDENT查找结果（客户匹配KNOWN@host）也不一定可信。入侵者也可能同时伪造了客户连接和IDENT查找，虽然这做起来会比仅伪造一个客户连接要困难的多。也有可能是客户IDENT服务撒谎。
* 注意：IDENT查找不适用于UDP服务。


示例
=====
EXAMPLES
The language is flexible enough that different types of access control policy can be expressed with a minimum of fuss. Although the language uses two access control tables, the most common policies can be implemented with one of the tables being trivial or even empty.

When reading the examples below it is important to realize that the allow table is scanned before the deny table, that the search terminates when a match is found, and that access is granted when no match is found at all.

The examples use host and domain names. They can be improved by including address and/or network/netmask information, to reduce the impact of temporary name server lookup failures.

* 语法足够灵活，不同类型的控制策略能够被分割为更小来表述。虽然使用了两张存取控制表，大多数通用的策略可以声明在一张表中。
* 
* 阅读下面的示例时，需要谨记允许列表在阻止列表之前处理；当匹配某一条后，匹配将终止；如果没有匹配任何一条规则将会被允许。
* 
* 在示例中使用的是主机名和域名，实际中可以使用IP地址或网段的形式，这样可以避免DNS发生故障引起的问题。

默认阻止模式
------------
MOSTLY CLOSED

In this case, access is denied by default. Only explicitly authorized hosts are permitted access.

在此情况下，访问是被默认拒绝的，只有被显式授权的主机才被允许访问。

The default policy (no access) is implemented with a trivial deny file:

默认策略在/etc/hosts.deny中声明：

.. sourcecode:: bash

    /etc/hosts.deny:
    ALL: ALL

This denies all service to all hosts, unless they are permitted access by entries in the allow file.
* 拒绝所有的主机访问任何服务，除非在/etc/hosts.allow中它被显式的允许访问。
* 显式允许的主机写入在/etc/hosts.allow中，如：

The explicitly authorized hosts are listed in the allow file. For example:

.. sourcecode:: bash

    /etc/hosts.allow:
    ALL: LOCAL @some_netgroup
    ALL: .foobar.edu EXCEPT terminalserver.foobar.edu

The first rule permits access from hosts in the local domain (no *.* in the host name) and from members of the some_netgroup netgroup.

The second rule permits access from all hosts in the foobar.edu domain (notice the leading dot), with the exception of terminalserver.foobar.edu.

* 第一条规则允许来自本地域和网络组some_netgroup的用户访问
* 第二条规则允许来自域foobar.edu的所有主机访问，但是不允许terminalserver.foobar.edu访问

默认允许模式
--------------

MOSTLY OPEN

Here, access is granted by default; only explicitly specified hosts are refused service.
这种情况下，允许访问是默认的，只有被显式声明的才会被拒绝。

The default policy (access granted) makes the allow file redundant so that it can be omitted. The explicitly non-authorized hosts are listed in the deny file. For example:

/etc/hosts.allow为空时，即默认允许访问。显式拒绝规则写入在/etc/hosts.deny中，如：

.. sourcecode:: bash

    /etc/hosts.deny:
    ALL: some.host.name, .some.domain
    ALL EXCEPT in.fingerd: other.host.name, .other.domain

The first rule denies some hosts and domains all services; the second rule still permits finger requests from other hosts and domains.

第一条来自主机some.host.name和域.some.domain的所有访问。第二条允许来自other.host.name和.other.domain进行finger请求。

设置陷井
---------
BOOBY TRAPS

The next example permits tftp requests from hosts in the local domain (notice the leading dot). Requests from any other hosts are denied. Instead of the requested file, a finger probe is sent to the offending host. The result is mailed to the superuser.

接下来的例子中允许来自本地域的主机访问tftp服务，任何来自其它主机的请求都将被拒绝。Instead of the requested file, a finger probe is sent to the offending host. The result is mailed to the superuser.

.. sourcecode:: bash

    /etc/hosts.allow:
    in.tftpd: LOCAL, .my.domain
    
    /etc/hosts.deny:
    in.tftpd: ALL: (/usr/sbin/safe_finger -l @%h | \
    /usr/bin/mail -s %d-%h root) &

The **safe_finger** command comes with the tcpd wrapper and should be installed in a suitable place. It limits possible damage from data sent by the remote finger server. It gives better protection than the standard finger command.

 The expansion of the *%h* (client host) and *%d* (service name) sequences is described in the section on shell commands.

 Warning: do not booby-trap your finger daemon, unless you are prepared for infinite finger loops.

 On network firewall systems this trick can be carried even further. The typical network firewall only provides a limited set of services to the outer world. All other services can be "bugged" just like the above tftp example. The result is an excellent early-warning system.

* tcp_wrapper包提供的safe_finger命令可以限制由于远程finger服务发送的数据可能带来的破坏。它比起标准的finger命令更为安全。
* 扩展表达式%h和%d在前面已经说明，分别表示客户主机和守护进程名
* 警告：除非你准备不限制finger循环，否则不要为finger守护进程进行伪装。（不太明白）
