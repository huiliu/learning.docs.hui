epoll(7)
********



.. author:: default
.. categories:: program, c/c++
.. tags:: c/c++
.. comments::
.. more::



Questions and answers
======================

       Q0  What  is  the  key used to distinguish the file descriptors registered in an epoll
           set?

       A0  The key is the combination of  the  file  descriptor  number  and  the  open  file
           description  (also known as an "open file handle", the kernel's internal represen‐
           tation of an open file).

Q: 如何判断注册在epoll中的文件描述符？（区分注册在epoll中的文件描述符的关键是什么？）

A: 关键是结合文件描述符和打开文件描述符（进行对比）。

       Q1  What happens if you register the same file descriptor on an epoll instance twice?

       A1  You will probably get EEXIST.  However, it is possible to add a duplicate (dup(2),
           dup2(2),  fcntl(2)  F_DUPFD) descriptor to the same epoll instance.  This can be a
           useful technique for filtering events, if the duplicate file descriptors are  reg‐
           istered with different events masks.

Q: 如果你在一个epoll实例中将同一个文件描述符注册再次会发生什么？

A: 你可能会得到EEXIST。然后可以在同一个epoll实例中添加一个文件描述符的副本（通过调用dup, dup2, fcntl等获得的），
对文件描述符的副本注册不同的事件，是进行事件过滤的一种有效方法。

       Q2  Can  two  epoll  instances  wait  for the same file descriptor?  If so, are events
           reported to both epoll file descriptors?

       A2  Yes, and events would be reported to both.  However, careful  programming  may  be
           needed to do this correctly.

Q:一个文件描述符可以同时注册到两个epoll实例中吗？如果可以，所有的事件都会被两个epoll实例报告吗？

A: 是的，所有的事件都会被不同epoll报告。然后，程序中可能需要小心，正确的处理这种情况。

       Q3  Is the epoll file descriptor itself poll/epoll/selectable?

       A3  Yes.   If  an  epoll  file  descriptor has events waiting then it will indicate as
           being readable.

Q: epoll实例的文件描述符可以用于poll/epoll/select吗？

A: 是的。如果一个epoll文件描述符有事件等待（处理），它将会被标示为可读。

       Q4  What happens if one attempts to put an epoll file descriptor  into  its  own  file
           descriptor set?

       A4  The  epoll_ctl(2)  call  will  fail  (EINVAL).  However, you can add an epoll file
           descriptor inside another epoll file descriptor set.

Q: 如果将一个epoll实例的描述符加入到自身侦听队列中会发生什么？

A: epoll_ctl将会返回错误（EINVAL）。但是你可以将一个epoll的描述述加入到另一个epoll侦听队列中。

       Q5  Can I send an epoll file descriptor over a UNIX domain socket to another process?

       A5  Yes, but it does not make sense to do this, since the receiving process would  not
           have copies of the file descriptors in the epoll set.

Q: 能否通过Unix域socket将一个epoll描述符传递给另外的进程？

A: 当然可以，但是这样做没有什么意义，因为接收它的进程没有一份epoll侦听文件描述符的列表。

       Q6  Will closing a file descriptor cause it to be removed from all epoll sets automat‐
           ically?

       A6  Yes, but be aware of the following point.  A file descriptor is a reference to  an
           open  file  description  (see  open(2)).   Whenever a descriptor is duplicated via
           dup(2), dup2(2), fcntl(2) F_DUPFD, or fork(2), a new file descriptor referring  to
           the  same open file description is created.  An open file description continues to
           exist until all file descriptors  referring  to  it  have  been  closed.   A  file
           descriptor is removed from an epoll set only after all the file descriptors refer‐
           ring to the underlying open file description have been closed (or  before  if  the
           descriptor  is  explicitly  removed using epoll_ctl(2) EPOLL_CTL_DEL).  This means
           that even after a file descriptor that is part of an epoll set  has  been  closed,
           events  may  be reported for that file descriptor if other file descriptors refer‐
           ring to the same underlying file description remain open.

Q: 关闭一个文件描述符是否会自动将其从所有epoll中移除？

A: 是的，但是应该注意以下情况。文件描述符是对一个打开文件的引用，无论什么时候，通过dup, dup2, fcntl, F_DUPFD, fork等复制得到的文件描述符，仍然是对原文件的引用，文件描述将一直可用（存在）直到所有指向该文件文件描述符均关闭。只有当所有指向某个文件的文件描述符全部关闭，位于epoll中该文件的文件描述符才会（自动）移除（或者显式的调用epoll_ctl EPOLL_CTL_DEL移除文件描述符）。这意味着即使一个epoll中的文件描述符关闭了，如果其它引用相同文件的描述符一直打开着，那么该文件的事件仍然会被epoll报告。

       Q7  If more than one event occurs between epoll_wait(2) calls, are  they  combined  or
           reported separately?

       A7  They will be combined.

Q:  如果在调用epoll_wait时发生了多个事件，它们是联合报告还是分开独立报告？

A:  联合在一起报告。

       Q8  Does  an  operation  on a file descriptor affect the already collected but not yet
           reported events?

       A8  You can do two operations on an existing file descriptor.  Remove would  be  mean‐
           ingless for this case.  Modify will reread available I/O.

Q:  对一个文件描述符的操作是否会影响到已经收集但是尚未报告的事件？
A:  


       Q9  Do I need to continuously read/write a file descriptor until EAGAIN when using the
           EPOLLET flag (edge-triggered behavior) ?

       A9  Receiving an event from  epoll_wait(2)  should  suggest  to  you  that  such  file
           descriptor  is  ready for the requested I/O operation.  You must consider it ready
           until the next (nonblocking) read/write yields EAGAIN.  When and how you will  use
           the file descriptor is entirely up to you.

           For  packet/token-oriented  files  (e.g.,  datagram  socket, terminal in canonical
           mode), the only way to detect the end of the read/write I/O space is  to  continue
           to read/write until EAGAIN.

           For  stream-oriented  files  (e.g., pipe, FIFO, stream socket), the condition that
           the read/write I/O space is exhausted can also be detected by checking the  amount
           of  data  read  from / written to the target file descriptor.  For example, if you
           call read(2) by asking to read a certain amount of  data  and  read(2)  returns  a
           lower  number of bytes, you can be sure of having exhausted the read I/O space for
           the file descriptor.  The same is true when writing using write(2).   (Avoid  this
           latter technique if you cannot guarantee that the monitored file descriptor always
           refers to a stream-oriented file.)

Q:  在边际触发模式下，是否必须不断的读/写一个文件描述符直到EAGAIN？

A:  当从epoll_wait收到事件，epoll仅仅是通知你某个文件描述符已经准备好I/O操作，You must consider it ready until the next (nonblocking) read/write yields EAGAIN. 什么时候、怎样使用这个文件描述符完全取决于你。 
对于面向数据包/令牌的文件（如UDP socket，终端），检测读/写I/O空间结尾的唯一办法是不断读/写直到EAGAIN。

对于面向流的文件（如管理，FIFO，TCP socket），通过计算读写目标文件描述符的数据量可以检测到相应I/O空间耗尽的状况。例如可以调用read读取一定量的数据，read返回一个较小的值，就可以确定当前文件描述符的读IO空间已经空了。写时调用write，前面的例子同样适用。（如果不能保证监控的文件描述符不是一直指向一个面向流的文件，应该避免后面一种技术。）
