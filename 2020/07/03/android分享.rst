android分享
******************
01234567890123456789012345678901234567890123456789012345678901234567890123456789


.. author:: default
.. categories:: none
.. tags:: none
.. comments::

分享可以分为两种情况\ [s]_

* 向其它应用分享数据。如向微信朋友圈分享图片。
* 接收其它应该的分享。如微信可以接收其它应用分享的图片

向其它应用分享数据
======================================
分享文本数据
----------------

.. sourcecode:: java

   private void shareText(String content){
      Intent sendIntent = new Intent();
      sendIntent.setAction(Intent.ACTION_SEND);
      sendIntent.putExtra(Intent.EXTRA_TEXT, content);
      sendIntent.setType("text/plain");

      Intent shareIntent = Intent.createChooser(sendIntent, null);
      startActivity(shareIntent);
   } 

可以添加 extra 以包含更多信息，如电子邮件收件人（EXTRA_EMAIL、EXTRA_CC 和
EXTRA_BCC）、电子邮件主题 (EXTRA_SUBJECT)，等等。

分享二进制内容
-----------------------

.. sourcecode:: java

   static void Share(String content, String imagePath, String url) {
      setAction Intent shareIntent = new Intent();
      // 给予接收应用访问Uri的权限 
      shareIntent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
      shareIntent.setAction(Intent.ACTION_SEND);
      shareIntent.setType("*/*");
      shareIntent.putExtra(Intent.EXTRA_TEXT, Html.fromHtml("<a href='" + url + "'>" + content + "</a>"));

      File imageFile = new File(imagePath);
      Uri uri = FileProvider.getUriForFile(activity.getApplicationContext(),
      activity.getPackageName() + ".fileprovider", imageFile);
      shareIntent.putExtra(Intent.EXTRA_STREAM, uri);

      activity.startActivity(Intent.createChooser(shareIntent, "Share"));
   }


分享多份内容
--------------------

.. sourcecode:: java

    ArrayList<Uri> imageUris = new ArrayList<Uri>();
    imageUris.add(imageUri1); // Add your image URIs here
    imageUris.add(imageUri2);

    Intent shareIntent = new Intent();
    shareIntent.setAction(Intent.ACTION_SEND_MULTIPLE);
    shareIntent.putParcelableArrayListExtra(Intent.EXTRA_STREAM, imageUris);
    shareIntent.setType("image/*");
    startActivity(Intent.createChooser(shareIntent, "Share images to.."));


分享文件
------------------


接收其它应用的分享
======================================

参考资料
==============
.. [s]   `分享简单的数据 <https://developer.android.com/training/sharing?hl=zh_cn>`_
