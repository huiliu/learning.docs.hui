android应用权限
**********************



.. author:: default
.. categories:: android
.. tags:: none
.. comments::


基本流程
======================
* 向清单中添加权限
  * 按API级别声明权限
* 检查是否获得授权
  * 执行相关逻辑
* 请求权限
  * 处理权限请求响应

向清单中添加权限
------------------------
向AndroidManifest.xml中添加\ `uses-permission`\ [u]_
.. sourcecode:: xml

   <manifest xmlns:android="http://schemas.android.com/apk/res/android"
      package="com.example.snazzyapp">

      <uses-permission android:name="android.permission.INTERNET"/>
      <!-- other permissions go here -->

      <application ...>
         <!-- ... -->
      </application>
   </manifest>

Android权限被分为三个级别\ [pl]_
* Normal
* Signature
* Dangerous

检查权限
-------------
调用方法\ `ContextCompat.checkSelfPermission()`\ 检查是否获得授权。

.. sourcecode:: java

   if (ContextCompat.checkSelfPermission(thisActivity, Manifest.permission.WRITE_CALENDAR)
         != PackageManager.PERMISSION_GRANTED) {
      // Permission is not granted
   }


`ContextCompat.checkSelfPermission()`\ 返回：
* `PERMISSION_GRANTED` 说明已经获得授权。
* `PERMISSION_DENIED` 说明没有权限，需要请求用户授权

示例\ [a]_
====================
.. sourcecode:: java

   /*
   * Copyright 2015 The Android Open Source Project
   *
   * Licensed under the Apache License, Version 2.0 (the "License");
   * you may not use this file except in compliance with the License.
   * You may obtain a copy of the License at
   *
   *     http://www.apache.org/licenses/LICENSE-2.0
   *
   * Unless required by applicable law or agreed to in writing, software
   * distributed under the License is distributed on an "AS IS" BASIS,
   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   * See the License for the specific language governing permissions and
   * limitations under the License.
   */

   package com.example.android.basicpermissions;

   import android.Manifest;
   import android.app.Activity;
   import android.content.Context;
   import android.content.Intent;
   import android.content.pm.PackageManager;
   import android.os.Bundle;
   import android.support.annotation.NonNull;
   import android.support.design.widget.Snackbar;
   import android.support.v4.app.ActivityCompat;
   import android.support.v7.app.AppCompatActivity;
   import android.view.View;
   import android.widget.Button;

   import com.example.android.basicpermissions.camera.CameraPreviewActivity;

   /**
    * Launcher Activity that demonstrates the use of runtime permissions for Android M.
    * This Activity requests permissions to access the camera
    * ({@link android.Manifest.permission#CAMERA})
    * when the 'Show Camera Preview' button is clicked to start  {@link CameraPreviewActivity} once
    * the permission has been granted.
    * <p>
    * First, the status of the Camera permission is checked using {@link
    * ActivityCompat#checkSelfPermission(Context, String)}
    * If it has not been granted ({@link PackageManager#PERMISSION_GRANTED}), it is requested by
    * calling
    * {@link ActivityCompat#requestPermissions(Activity, String[], int)}. The result of the request is
    * returned to the
    * {@link android.support.v4.app.ActivityCompat.OnRequestPermissionsResultCallback}, which starts
    * {@link
    * CameraPreviewActivity} if the permission has been granted.
    * <p>
    * Note that there is no need to check the API level, the support library
    * already takes care of this. Similar helper methods for permissions are also available in
    * ({@link ActivityCompat},
    * {@link android.support.v4.content.ContextCompat} and {@link android.support.v4.app.Fragment}).
    */
   public class MainActivity extends AppCompatActivity
           implements ActivityCompat.OnRequestPermissionsResultCallback {

       private static final int PERMISSION_REQUEST_CAMERA = 0;

       private View mLayout;

       @Override
       protected void onCreate(Bundle savedInstanceState) {
           super.onCreate(savedInstanceState);
           setContentView(R.layout.activity_main);
           mLayout = findViewById(R.id.main_layout);

           // Register a listener for the 'Show Camera Preview' button.
           findViewById(R.id.button_open_camera).setOnClickListener(new View.OnClickListener() {
               @Override
               public void onClick(View view) {
                   showCameraPreview();
               }
           });
       }

       @Override
       public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
               @NonNull int[] grantResults) {
           // BEGIN_INCLUDE(onRequestPermissionsResult)
           if (requestCode == PERMISSION_REQUEST_CAMERA) {
               // Request for camera permission.
               if (grantResults.length == 1 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                   // Permission has been granted. Start camera preview Activity.
                   Snackbar.make(mLayout, R.string.camera_permission_granted,
                           Snackbar.LENGTH_SHORT)
                           .show();
                   startCamera();
               } else {
                   // Permission request was denied.
                   Snackbar.make(mLayout, R.string.camera_permission_denied,
                           Snackbar.LENGTH_SHORT)
                           .show();
               }
           }
           // END_INCLUDE(onRequestPermissionsResult)
       }

       private void showCameraPreview() {
           // BEGIN_INCLUDE(startCamera)
           // Check if the Camera permission has been granted
           if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                   == PackageManager.PERMISSION_GRANTED) {
               // Permission is already available, start camera preview
               Snackbar.make(mLayout,
                       R.string.camera_permission_available,
                       Snackbar.LENGTH_SHORT).show();
               startCamera();
           } else {
               // Permission is missing and must be requested.
               requestCameraPermission();
           }
           // END_INCLUDE(startCamera)
       }

       /**
        * Requests the {@link android.Manifest.permission#CAMERA} permission.
        * If an additional rationale should be displayed, the user has to launch the request from
        * a SnackBar that includes additional information.
        */
       private void requestCameraPermission() {
           // Permission has not been granted and must be requested.
           if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                   Manifest.permission.CAMERA)) {
               // Provide an additional rationale to the user if the permission was not granted
               // and the user would benefit from additional context for the use of the permission.
               // Display a SnackBar with cda button to request the missing permission.
               Snackbar.make(mLayout, R.string.camera_access_required,
                       Snackbar.LENGTH_INDEFINITE).setAction(R.string.ok, new View.OnClickListener() {
                   @Override
                   public void onClick(View view) {
                       // Request the permission
                       ActivityCompat.requestPermissions(MainActivity.this,
                               new String[]{Manifest.permission.CAMERA},
                               PERMISSION_REQUEST_CAMERA);
                   }
               }).show();

           } else {
               Snackbar.make(mLayout, R.string.camera_unavailable, Snackbar.LENGTH_SHORT).show();
               // Request the permission. The result will be received in onRequestPermissionResult().
               ActivityCompat.requestPermissions(this,
                       new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CAMERA);
           }
       }

       private void startCamera() {
           Intent intent = new Intent(this, CameraPreviewActivity.class);
           startActivity(intent);
       }

   }



参考资料
===========
.. [1]   `请求应用权限 <https://developer.android.com/training/permissions/requesting>`_
.. [a]   `permissions samples <https://github.com/android/permissions-samples.git>`_
.. [g]   `permission group <https://developer.android.com/reference/android/Manifest.permission_group>`_
.. [p]   `permission <https://developer.android.com/reference/android/Manifest.permission>`_
.. [m]   `Android 权限的 Material Design 准则 <https://material.io/design/platform-guidance/android-permissions.html#request-types>`_
.. [u]   `uses-permission <https://developer.android.com/guide/topics/manifest/uses-permission-element>`_
.. [pl]  `Permission Protect Level <https://developer.android.com/guide/topics/permissions/overview#normal-dangerous>`_
