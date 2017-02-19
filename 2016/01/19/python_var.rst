Pyhton中变量的作用域
*********************



.. author:: default
.. categories:: python, program
.. tags:: python, program, learn
.. comments::
.. more::



下面的程序：\ ::

    def test():
        if a == 100:
            b = 10
        print(b)

    if __name__ == '__main__':
        a = 100
        b = 11
        test()
        print(b)

    # 程序输出结果为
    # 10
    # 11
