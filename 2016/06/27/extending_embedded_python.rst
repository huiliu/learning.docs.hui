在应用程序中嵌入Python
==========================
在应用程序中嵌入Python或其它什么，关键就是如何让应用程序与Python脚本交互，即在
应用程序中解释执行Python代码。回想一下你是如何调用Python标准模块的？应用程序采
取相同的方式与Python交互：

1.  导入Python模块（加载脚本）
2.  获取模块中的函数/类/类的方法等
3.  构建参数，调用Python函数；访问属性等
4.  解析返回值

.. author:: default
.. categories:: c/c++
.. tags:: c/c++, program, python
.. comments::
.. more::


在应用程序中嵌入Python或其它什么，关键就是如何让应用程序与Python脚本交互，即在\
应用程序中解释执行Python代码。回想一下你是如何调用Python标准模块的？应用程序采\
取相同的方式与Python交互：

1.  导入Python模块（加载脚本）
2.  获取模块中的函数/类/类的方法等
3.  构建参数，调用Python函数；访问属性等
4.  解析返回值


一个简单的例子
===============
C程序：

.. sourcecode:: c

    // file: main.c
    #include <Python.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    
    #define CHECK_NULL_ASSERT(p) \
        if (NULL == (p)) {\
            PyErr_Print();\
            assert(0);\
        }
    
    int main(int argc, char *argv[])
    {
        if (argc < 3) {
            fprintf(stderr, "Usage: exe <python_source_file> <func_name>");
        }
    
        setenv("PYTHONPATH", "./", 1);
        Py_SetProgramName("main");
    
        Py_Initialize();
        PyObject* pModule = PyImport_ImportModule(argv[1]);
        CHECK_NULL_ASSERT(pModule);
        PyObject* pDict = PyModule_GetDict(pModule);
        CHECK_NULL_ASSERT(pDict);
        PyObject* pFunc = PyDict_GetItemString(pDict, argv[2]);
        CHECK_NULL_ASSERT(pFunc);
    
        if (PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, NULL);
        }
        else
        {
            PyErr_Print();
        }
    
        Py_DECREF(pModule);
    
        Py_Finalize();
        return 0;
    }

编译：\ `gcc main.c -o main \`pkg-config --libs --cflags python2\``

python文件：

.. sourcecode:: python

    # file: example.py
    def echo():
        print("Hello World!")


执行：\ `./main example echo`\ 。打印："hello world!"

上面C程序中的基本流程就是按照前面提到的步骤进行的。

Python库搜索路径
=================
应用程序加载python脚本模块，有一个模块的搜索路径问题，\ `python`\ 库的默认搜索\
路径与Python的安装路径有关，可以通过下面的代码查看：

.. sourcecode:: python

    import sys

    print sys.path

另外可以通过两个环境变量：\ `PYTHONHOME`\ 和\ `PYTHONPATH`\ 来调整。根据官方文
档中的说明：

*   PYTHONHOME      用于更改Python标准库的（搜索）位置，默认位置为：\
    `prefix/lib/pythonversion`\ 和\ `exec_prefix/lib/pythonversion`\。如果将\
    `PYTHONHOME`\ 设置为单个目录，则\ *prefix*\ 和\ *exec_prefix*\ 将取相同值\
    。也可以设置为不同的值：\ `prefix:exec_prefix`

*   PYTHONPATH      用于增加模块文件的默认搜索路径。默认搜索路径与安装相关，通
    过为：\ `prefox/lib/pythonversion`\ ，且它通常应该添加到变量\ `PYTHONPATH`\
    中。

调整库搜索路径
---------------
调整python解释器的搜索路径，其实就是修改\ `PYTHONHOME`\ 和\ `PYTHONPATH`\ 两个\
环境变量。方法有：

1.  使用\ `setenv`\ 来修改\ `PYTHONHOME`\ 和\ `PYTHONPATH`\ 。如上面的代码

    .. sourcecode:: c

        setenv("PYTHONPATH", "./", 1);

2.  在命令行修改环境变量
3.  Python API函数\ `PySys_SetPath(char*)`

    .. sourcecode:: c

        // ...... 
        Py_Initialize();
        PySys_SetPath("./");
        // ...... 
        Py_Finalize();

加载Python模块
===============
加载Python模块（脚本文件）

1.  PyObject\* `PyImport_ImportModule` (const char \*name)
    相当于：\ `import name`\ 。\ *name*\ 可以是内置模块名，也可以是开发的新模\
    模块名（文件名）


2.  PyObject\* PyImport_Import(PyObject \*name)
    参数\ *name*\ 应该是一个\ `PyObject`\ 对象，如：

    .. sourcecode:: c

        PyObject* pFileName = PyString_FromString(argv[1]);
        PyObject* pModule = PyImport_Import(pFileName);

取得函数的引用
===============
如果要执行脚本中的某个特定的函数，首先要取得这个函数：

1.  int PyObject_HasAttrString(PyObject* o, const char* attr_name);
    PyObject* PyObject_GetAttrString(PyObject* o, const char* attr_name);
    int PyObject_SetAttrString(PyObject* o, const char* attr_name, PyObject* v);
    int PyObject_DelAttrString(PyObject* o, const char* attr_name);

    相当于: obj.name [= val]

    通过这种方法不仅是取得属性，也可以是函数


构造参数调用函数
================
C调用Python函数，向Python函数传递参数，则需要将C的数据类型转换为Python的数据类\
型，然后再调用Python函数。

将C类型转换为Python类型
------------------------

1.  PyObject* Py_BuildValue(const char* format, ...);    可以将各种C数据类型，\
    转换为python数据类型。

    如：

    .. sourcecode:: text

        Py_BuildValue("")                               None
        Py_BuildValue("i", 37)                          37
        Py_BuildValue("ids", 37, 3.4, "hello")          (37, 3.4, "hello")元组
        Py_BuildValue("[ii]", 37, 3)                    [37, 3]
        Py_BuildValue("{s:i,s:i}", "x", 3, "y", 2)      {"x":1, "y":2}


2.  int PyArg_ParseTuple(PyObject \*args, const char \*format, ...) 

    int PyArg_ParseTupleAndKeywords(PyObject \*args, PyObject \*kw, const char
    \*format, char \*keywords[], ...) 

    int PyArg_Parse(PyObject \*args, const char \*format, ...)

调用python中的函数
--------------------

1.  PyCallable_Check(PyObject*)     检查对象是否可以被调用执行
2.  PyObject_CallObject(PyObject* callable_obj, PyObject* arg)  传递参数\
    *arg*\ 调用对象\ *callable_obj*\ 。等同于python中：
    ``apply(callable_object, args)``\ 。注意\ `args`\ 是元组。

3.  PyObject* PyObject_CallFunction(PyObject* callable, char* format, ...)
    直接以C类型数据调用\ *callable*\ 对象， 格式串\ *format*\ 与\
    `Py_BuildValue`\ 一样。

4.  PyObject* PyObject_CallFunctionObjArgs(PyObject* callable, ..., NULL) 


4.  PyObject* PyObject_CallMethod(PyObject* o, char* method, char* format, ...) 
    等同于：\ o.method(args)

5.  PyObject* PyObject_CallMethodObjArgs(PyObject* o, PyObject* name, ..., NULL) 



处理返回值
============
处理返回值，即将Python的数据类型转换为C的类型

1.  long PyInt_AsLong(PyObject*);
2.  long PyLong_AsLong(PyObject*);
3.  double PyFloat_AsDouble(PyObject*)
4.  char* PyString_AsString(PyObject*)


扩展嵌入的Python解释器
======================
通过Python C API可以将标准的Python解释器嵌入到应用程序中，但是如何扩展这个解释
器呢？即在解释器中内置一些模块。

首先看一下，Python文档中的例子：

.. sourcecode:: c

    #include <Python.h>
    
    static int numargs=0;
    
    /* Return the number of arguments of the application command line */
    static PyObject*
    emb_numargs(PyObject *self, PyObject *args)
    {
        if(!PyArg_ParseTuple(args, ":numargs"))
            return NULL;
        return Py_BuildValue("i", numargs);
    }
    
    static PyMethodDef EmbMethods[] = {
        {"numargs", emb_numargs, METH_VARARGS,
         "Return the number of arguments received by the process."},
        {NULL, NULL, 0, NULL}
    };



    int
    main(int argc, char *argv[])
    {
        Py_SetProgramName(argv[0]);  /* optional but recommended */
        Py_Initialize();
        numargs = argc;
        // 关键之处
        Py_InitModule("emb", EmbMethods);
        PyObject* pModule = PyImport_ImportModule(argv[1]);
        PyObject* pFunc = PyObject_GetAttrString(pModule, argv[2]);
        if (PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, NULL);
        }
        Py_Finalize();
        return 0;
    }

上面的代码中，内置的Python解释中扩展了一个模块"`emb`"，\
即应用程序提供了一个API可供脚本使用，在下面的python脚本中导入模块，并执行其中\
的功能。python脚本为：

.. sourcecode:: python

    import emb

    def echo():
        print(print "Number of arguments", emb.numargs())

这样就实现了Python/应用程序的双向通讯。

再看看上面的代码，相当复杂麻烦，开发人员要专门写每一个API，并规划导出，而用\
C/C++来扩展Python有很多更方便的工具，如\ `swig, Boost.Python`\ 等。

如果将用这些工具导出的API扩展到嵌入在程序内的解释器呢？


swig与扩展嵌入的python解释器
-----------------------------

我们建立一个小工程：

1.  foo.h

    .. sourcecode:: c

    #ifndef __FOO_H__
    #define __FOO_H__

    int fib(int n);
    #endif

2. foo.c

   .. sourcecode:: c

        #include "foo.h"

        int fib(int n)
        {
            if (n <= 1)
            {
                return 1;
            }
            else
            {
                return n * fib(n - 1);
            }
        }

3.  foo.i

    .. sourcecode:: swig

        %module fool

        %{
        #include "foo.h"
        %}

        %include "foo.h"

4.  test.py

    .. sourcecode:: python

        import foo

        def calc(n):
            return foo.fib(n)

5.  main.c

    .. sourcecode:: c

        #include <Python.h>
        #include <stdio.h>
        #include <stdlib.h>
        
        // 由swig生成
        extern void init_foo();
        
        int main(int argc, char* argv[])
        {
        
            if (argc < 2) {
                fprintf(stderr, "Usage:\n\t%s <n>\n", argv[0]);
                exit(1);    
            }
            setenv("PYTHONPATH", "./", 1);
        
            Py_Initialize();
            // 关键代码，由swig生成，在文件foo_wrap.c中
            init_foo();
            PyObject* pModule = PyImport_ImportModule("test");
            PyObject* pFunc = PyObject_GetAttrString(pModule, "calc");
        
            PyObject* pValue = Py_BuildValue("i", atoi(argv[1]));
        
            if (PyCallable_Check(pFunc))
            {
                PyObject* pReturn = PyObject_CallFunctionObjArgs(pFunc, pValue, NULL);
                if (NULL == pReturn) {
                    PyErr_Print();
                }
                fprintf(stdout, "%ld\n", PyInt_AsLong(pReturn));
        
                Py_DECREF(pReturn);
            }
        
            Py_DECREF(pValue);
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
        
            Py_Finalize();
        
            return 0;
        }

6.  Makefile

    .. sourcecode:: makefile

        CFLAGS = -Wall -g `pkg-config --libs --cflags python2`
        
        all: main
        
        main: main.c foo.c foo_wrap.c
        
        
        foo_wrap.c: foo.h foo.i
        	swig -python foo.i
        
        
        clean:
        	rm -f main foo_wrap.c foo.py \*.pyc

运行make之前，\ `./main 5`


参数资料
=========
1.  Python Essential Reference(4th)
2.  `Python Document <http://docs.python.org>`_
