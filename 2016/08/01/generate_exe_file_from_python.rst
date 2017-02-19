Generate exe file from python
=============================

如何使用py2exe将python脚本转换为Windows下的exe文件。

.. author:: default
.. categories:: python
.. tags:: python
.. comments::
.. more::

根据\ `py2exe <http://www.py2exe.org>`_\ 官网介绍，\ `py2exe`\ 是将python库\
``distutils``\ 进行了扩展，使得python脚本可以转换为可直接运行于Windows上的exe\
文件。

使用py2exe将一个python脚本转换为exe文件的关键是"**setup.py**"文件的书写。

安装py2exe
============
到\ `py2exe <http://www.py2exe.org>`_\ 官网\
`下载 <https://sourceforge.net/projects/py2exe/files/py2exe/>`_\ 与Python版本\
匹配的安装包安装即可。

然后打开python的shell，执行\ ``import py2exe``\ ，没有出错即说明安装成功。

一个简单的例子
===============
先看一下例子的目录结构：

.. code-block:: text

    .
    ├── abc.xml
    ├── config.ini
    ├── README.rst
    ├── setup.py
    └── update_match_schedule.py

其中：

1.  `abc.xml`\ 为xml数据文件
2.  `config.ini`\ 为配置文件
3.  `README.rst`
4.  `setup.py`\ py2exe/distutils所需的配置文件，后面将详细介绍此文件。

    .. code-block:: python

        from distutils.core import setup
        import py2exe
        
        setup(
                options = {
                        'py2exe': {
                                'compressed': 1,
                                'optimize': 2,
                                'bundle_files': 1
                            }
                    },
                zipfile = None,
                console = ['update_match_schedule.py'],
                data_files = [
                        ("", ['config.ini']),
                        ("", ['README.rst']),
                    ]
            )

4.  `update_match_schedule.py`\ 是期望转换为Windows exe的python脚本文件，将xml\
    数据导入到指定的数据库中。

    .. code-block:: python


        import MySQLdb
        import xml.etree.ElementTree as ET
        import ConfigParser
        import codecs
        import os
        import sys
        
        DB_TBL_NAME = ""
        
        def load_config(cfgfile="config.ini"):
            """
            结节略去
            """
        
            return cfg_dict
        
        def generate_sql(xmlfile):
            """
            结节略去
            """
        
        if '__main__'== __name__:
        
            cfg = load_config()
        
            DB_TBL_NAME = cfg['db_tbl_name']
            conn = MySQLdb.connect(host=cfg['db_host'],
                    port=int(cfg['db_port']),
                    user=cfg['db_user'],
                    passwd=cfg['db_passwd'],
                    db=cfg['db_name'],
                    charset='utf8')
        
            assert conn
            cursor = conn.cursor()
        
            for item in generate_sql(sys.argv[1]):
                cursor.execute(item)
        
            conn.commit()
            conn.close()

在Windows CMD下运行命令："``setup.py py2exe``"。就会balabala的输出一堆，将执\
行脚本"**update_match_schedule.py**"所需的相关文件打包，并根据\ **setup.py**\
的设置来处理这些文件并输出exe文件"**update_match_schedule.exe**"。正确完成后会\
生成两个目录："*build*"和"*dist*"，"*build*"中是一些中间文件；"*dist*"为包含生\
成的exe文件和其它一些文件（依赖于配置设定）。

通常可以将命令"``setup.py py2exe``"写入到一个batch文件中，可以不用每次打开CMD\
输入命令。

.. code-block:: batch

    c:\Python27\python.exe setup.py py2exe
    pause

    move dist\* ..\
    pause

.. note::

    如果在Windows CMD执行命令："``setup.py py2exe``"，没有执行文件而是提示打开\
    文件，确认已经将Python可执行文件加入到PATH中。或者直接指定python.exe的路径\
    执行。

配置文件"**setup.py**
=======================
首先，\ `py2exe`\ 是对\ `distutils`\ 的扩展，所以\ `distutils`\ 的"**setup**"\
选项，在这里仍然可以使用。再看看上面的"**setup.py**"文件的含义，


.. code-block:: python

    from distutils.core import setup
    import py2exe
    
    setup(
            options = {
                    'py2exe': {
                            'compressed': 1,
                            'optimize': 2,
                            'bundle_files': 1
                        }
                },
            zipfile = None,
            console = ['update_match_schedule.py'],
            data_files = [
                    ("", ['config.ini']),
                    ("", ['README.rst']),
                ]
        )

1.  **options**\ 中的"**py2exe**"是py2exe对distutils的一些\ `扩展
    <http://www.py2exe.org/index.cgi/ListOfOptions>`_\ ，主要有：

    +----------------+-------------------------------------------------------+
    | Key            | Value                                                 |
    +----------------+-------------------------------------------------------+
    | compressed     | (boolean) 是否创建一个压缩过的zipfile                 |
    +----------------+-------------------------------------------------------+
    | optimize       | (int) 0=不优化(不生成.pyc);1=正常优化;2=额外优化      |
    +----------------+-------------------------------------------------------+
    | bundle_files   | 打包文件。1=打包所有,包括python解释器;2=打包除python解|
    |                | 释器外的外有;3=不打包（默认）                         |
    +----------------+-------------------------------------------------------+
    | includes       | 包含的模块名                                          |
    +----------------+-------------------------------------------------------+
    | packages       | list of packages to include with subpackages          |
    +----------------+-------------------------------------------------------+
    | excludes       | 被排除的模块名                                        |
    +----------------+-------------------------------------------------------+
    | dll_excludes   | 被排除的dll文件                                       |
    +----------------+-------------------------------------------------------+
    | ignores        | 如果未打到则忽略                                      |
    +----------------+-------------------------------------------------------+
    | dist_dir       | 存放最终输出文件的目录                                |
    +----------------+-------------------------------------------------------+
    | ascii          | (boolean) 是否自动包含encodings和codecs模块           |
    +----------------+-------------------------------------------------------+
    | skip_archive   | (boolean) do not place Python bytecode files in an    |
    |                | archive, put them directly in the file system         |
    +----------------+-------------------------------------------------------+
    | unbuffered     | 如果为true,则不缓存stdout和stderr                     |
    +----------------+-------------------------------------------------------+

2.  "**zipfile**"和"**console**"是重用distutils的参数，还包括一些其它的：


    +----------------+-------------------------------------------------------+
    | Keyword        | Description                                           |
    +----------------+-------------------------------------------------------+
    | console        | 将被转换为命令行执行的脚本名                          |
    +----------------+-------------------------------------------------------+
    | windows        | 将被转换为GUI程序的脚本名                             |
    +----------------+-------------------------------------------------------+
    | service        | 包含WIN32 服务类的模块名                              |
    +----------------+-------------------------------------------------------+
    | com_server     | 包含Win32服务类的模块名                               |
    +----------------+-------------------------------------------------------+
    | zipfile        | 生成的共享zip文件名，可以是一个子目录。默认library.zip|
    |                | 如果设定None，则相应的文件被打包的exe文件中           |
    +----------------+-------------------------------------------------------+
    | options        | 字典：{'py2exe': {'opt1': 'val1', 'opt2': 'val2'}}    |
    +----------------+-------------------------------------------------------+

3.  "**data_files**"用于指定需要发布的文件。\ [#data_files]_

参考资料
=========

..  [#data_files]    `Installing Additional Files <https://docs.python.org/2.7/distutils/setupscript.html#distutils-additional-files>`_
