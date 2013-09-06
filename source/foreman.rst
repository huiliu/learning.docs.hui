Nginx+Foreman+Passenger
************************

准备工作
================
系统为\ **CentOS 6.4(x64) mininal Installation**\ 。需要下载的软件包：

* Nginx_
* Passenger_
* Foreman_

.. _Nginx: http://nginx.org/download/nginx-1.4.2.tar.gz>
.. _Passenger: http://s3.amazonaws.com/phusion-passenger/releases/passenger-4.0.16.tar.gz>
.. _Foreman: https://github.com/theforeman/foreman/archive/1.2-stable.zip>

由于系统为最小安装的CentOS，所以基本的编译环境都没有，需要安装\ **gcc, gcc-c++,
make, automake**\ 等基本开发工具。

.. code-block:: bash

    yum install gcc gcc-c++ boost make automake

安装Nginx, Passenger和Foreman时还需要安装一些软件包：

.. code-block:: bash

    yum install libcurl-devel zlib-devel ruby-devel pcre-devel openssl-devel libatomic_ops-devel
    gem install rake
    gem install bundle

    # 请参考http://theforeman.org/manuals/1.1/index.html#3.4InstallFromSource
    yum install libxml-devel libxslt-devel libvirt-devel mysql-devel postgresql-devel sqlite-devel

安装
=========

解压下载的源代码：\ `tar xf source.tar.gz`

安装Nginx和Passenger
----------------------
*   使用的Nginx编译选项为:

    .. code-block:: bash
    
        # 需要建立用户nginx
        useradd -r nginx
    
        # 需要特别注意的是--add-module选项，需要指向passenger源码包中的Nginx扩展
        ./configure --user=nginx --group=nginx --with-pcre --add-module=/root/passenger-4.0.14/ext/nginx --with-file-aio --with-http_ssl_module --with-http_gunzip_module --with-http_gzip_static_module --with-libatomic
    
        # 编译正常完成后，安装Nginx。安装目录默认为：/usr/local/nginx
        # Nginx的所有相关信息都存放在此目录中
        make && make install

*   验证安装：

    -   安装完成后，在\ */usr/local/nginx/html*\ 下创建一个简单的例子，测试一下\
        安装是否正确。\ [#ref1]_
    
        .. code-block:: bash
        
            cd /usr/local/nginx/html
            mkdir -p webapp/{public,tmp}
        
            # 创建一个文件passenger_wsgi.py，并写入：
            cat passenger_wsgi.py
            def application(environ, start_response):
              start_response('200 OK', [('Content-Type', 'text/plain')])
                return [b"hello world!\n"]

    -   编辑Nginx的配置文件

        .. code-block:: nginx

            passenger_root  /usr/lib/ruby/gems/1.8/gems/passenger-4.0.14;
            passenger_ruby  /usr/bin/ruby;

            index  index.html index.htm index.php;
            server {
                listen       80;
                server_name  localhost;

                charset utf-8;

                location / {
                    root   html/webapp/public;
                    passenger_enabled on;
                }

                error_page  404              /404.html;

                # redirect server error pages to the static page /50x.html
                #
                error_page   500 502 503 504  /50x.html;
                location = /50x.html {
                    root   html;
                }
            }


    -   直接使用命令\ `nginx`\ 启动Nginx(需要将\ */usr/local/nginx/bin*\ 加入到\
        环境变量PATH)。用浏览器访问\ *http://localhost/webapp*\ 。如果显示正常\
        ，说明passenger已经正常编译至Nginx。如果发生错误，请查看访问日志排除问题。

安装Foreman
-------------
上一步Nginx和Foreman成功安装以后，Foreman的安装相对简单，按照文档说明就可以完成
[#ref2]_ \ 。只需要注意将依赖的软件包安装好，如果出错仔细出错说明，再解决。

.. code-block:: bash

    cd foreman-1.2-stable
    bundle install --without mysql mysql2  sqlite test --path vendor # or postgresql
    cp config/settings.yaml.example config/settings.yaml
    cp config/database.yml.example config/database.yml
    RAILS_ENV=production bundle exec rake db:migrate # (to set up database schema)
    
    # 顺利完成后就可以启动rail了。如果出错一般是因为依赖安装不完整。
    ./script/rails s -e production

然后就可以在浏览器中打开\ `http://localhost:3000`\ 了。当然也可能出错，比如：
**ActionView::Template::Error (couldn't find file 'jquery.ui.autocomplete'**\ 。
google到此问题的解决方法：\ [#ref3]_

.. code-block:: bash

    # 请耐心等待此命令执行完，虽然不太明白其意思
    RAILS_ENV=production bundle exec rake assets:precompile
    

Nginx+Passenger+Foreman
============================
Nginx, Passenger和Foreman成功安装运行后，只需在给Nginx添加一小段配置就可以通过\
Nginx来访问Foreman了。

.. code-block:: nginx

    # 已经设定好passenger_root和passenger_ruby等
    server {
        listen 3000;
        root /opt/foreman/public;

        passenger_enabled on;

        access_log logs/foreman_access.log;
        error_log logs/foreman_access.log;
    }

另外还有官方文档说明\ [#ref4]_\ 和\ GitHub_\ 上关于Nginx和foreman的配置：\
[#ref5]_

.. _GitHub: https://github.com

.. code-block:: nginx

    ## Puppet Foreman
    server {
        listen 443 ssl;
        ssl_certificate ssl/foreman.xxx.com.crt;
        ssl_certificate_key ssl/foreman.xxx.com.key;
        ssl_session_timeout 10m;
        add_header Strict-Transport-Security max-age=15768000;
         
        server_name foreman.xxx.com;
         
        access_log /var/log/nginx/foreman.access.log;
        error_log /var/log/nginx/foreman.error.log;
        root /opt/foreman/public;
         
        location / {
            passenger_enabled on;
            auth_basic "Restricted!!!";
            auth_basic_user_file .htpasswd;
        }
    }
     
    server {
        listen 127.0.0.1:3000;
        root /opt/foreman/public;
        passenger_enabled on;
        access_log off;
        allow 127.0.0.1;
        deny all;
    }

关于Nginx，Passenger和Foreman 各自详细的配置请查看各自文档。\ [#ref6]_

参考资料
==========
.. [#ref1] http://www.modrails.com/documentation/Users%20guide%20Nginx.html#_deploying_a_wsgi_python_application
.. [#ref2]  http://theforeman.org/manuals/1.1/index.html#3.4InstallFromSource
.. [#ref3]  http://projects.theforeman.org/issues/2683
.. [#ref4]  http://projects.theforeman.org/projects/1/wiki/Setting_up_Nginx_+_Passenger_
.. [#ref5]  https://gist.github.com/vladgh/1598698
.. [#ref6]  http://nginx.org/en/docs/, \
            http://www.modrails.com/documentation/Users%20guide%20Nginx.html#_configuring_phusion_passenger, \
            http://theforeman.org/manuals/1.1/index.html#3.5Configuration
