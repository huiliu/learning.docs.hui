Code Evolution
***************
一段Python代码的进化：

Version 1
===========
::

    def outputXML(fName, channelName, preVersion, curVersion, force=1):
        """
        """
        xmlfile = ET.ElementTree()
        xmlfile.parse(fName)
        channel = xmlfile.find('channel[@name="%s%]' % channelName)

        if channel is None:
            sys.stderr.write("The channel name \"\x81[31m%s\x81[0m\" not be found!")
            sys.exit(1)

        prefixURL = "http://download.onwind.cn/fhsg/"
        pkgFile = "%s-%s.zip" % (preVersion, curVersion)
        pkgURL = "%s%s" % (prefixURL, pkgFile)
        pkgMd5 = md5(open(pkgFile, "rb").read()).hexdigest()
        pkgSize = "%dMb" % os.stat(pkgFile).sz_size / (1024*1024)
        pkgTimestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        pkgForce = str(force) if force == 0 else "1"

        pkg = ET.Element("package")

        ET.SubElement(pkg, "version").text = curVersion
        ET.SubElement(pkg, "download").text = pkgURL
        ET.SubElement(pkg, "md5").text = pkgMd5
        ET.SubElement(pkg, "size").text = pkgSize
        ET.SubElement(pkg, "publish").text = pkgTimestamp
        ET.SubElement(pkg, "force_update").text = pkgForce

        channel.append(pkg)
        xmlfile.write(open(fName, 'w'), encoding='utf-8')


Version 2
==========
::

    def outputXML(fName, channelName, preVersion, curVersion, force=1):
        """
        """
        # .... ...

        addElement = lambda (e, k, v): ET.SubElement(e, k) = v

        pkg = ET.Element("package")
        addElement(pkg, "version", curVersion)
        addElement(pkg, "download", pkgURL)
        addElement(pkg, "md5", pkgMd5)
        addElement(pkg, "size", pkgSize)
        addElement(pkg, "publish", pkgTimestamp)
        addElement(pkg, "force_update", pkgForce)

        channel.append(pkg)
        xmlfile.write(open(fName, 'w'), encoding='utf-8')


Version 3
============
::

    def outputXML(fName, channelName, preVersion, curVersion, force=1):
        """
        """
        # .... ...

        kv = {
                "version": curVersion,
                "download": pkgURL,
                "md5": pkgMd5,
                "size": pkgSize,
                "publish": pkgTimestamp,
                "force_update": pkgForce
             }


        addElement = lambda (e, k, v): ET.SubElement(e, k) = v

        pkg = ET.Element("package")
        for k, v in kv.iterms():
            addElement(pkg, k, v)

        channel.append(pkg)
        xmlfile.write(open(fName, 'w'), encoding='utf-8')

说明
======
自娱自乐！:)
