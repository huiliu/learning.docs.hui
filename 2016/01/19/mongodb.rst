MongoDB
***********


.. author:: default
.. categories:: database
.. tags:: python, database, mongodb
.. comments::
.. more::



Python SDK
===========
``MongoDB``\ 的官方网站有提供利用各种语言进行MongoDB开发的SDK，Python当然不能少。
关于SDK包\ ``pymongo``\ 的详细手册，可以参考\ `MongoDB`_\ 。在此进行超简介绍：

.. _MongoDB:    http://api.mongodb.org/python/current/tutorial.html

连接MongoDB服务器
------------------
``MongoDB``\ 服务器默认是侦听TCP 27017这个端口，等待用户连接：\ ::

    from pymongo import MongoClient

    client = MongoClient("localhost", 27017)

建立一个数据库
---------------------------
通过\ ``MongoClient``\ 对象的属性的形式来获取或建立一个数据库，如：\ ::

    db = client.dbName
    # or
    db = client[dbName]

建立一个集合（Collection, 类似于表）
------------------------------------
``Collection``\ 的获取和建立的方法与数据库一样：\ ::

    collection = db.dbName
    # OR
    collection = db[dbName]

插入一条记录（Document）
-------------------------
``MongoDB``\ 的数据是以json格式存储，而这刚好与Python中的字典的形式一致，所以\
处理起来非常的方便。\ ::

    data = {"author": "Mike",
            "text": "My first blog post!",
            "tags": ["mongodb", "python", "pymongo"],
            "date": datetime.datetime.utcnow()}
    collection = db.dbName
    collection.insert(data)

参考资料
==========
