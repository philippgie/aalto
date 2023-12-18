package com.example.serialintent

import android.content.ComponentName
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import dalvik.system.DexClassLoader


class MainActivity : AppCompatActivity() {

    //inner class

    var TAG = "MOBISEC"

    fun ByteArray.toHex(): String =
        joinToString(separator = "") { eachByte -> "%02x".format(eachByte) }


    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        Log.v(TAG, "requestCode: " + requestCode)
        Log.v(TAG, "resultCode: " + resultCode)


        val classname = "com.mobisec.serialintent.FlagContainer"
        try {
            val path = packageManager.getPackageInfo("com.mobisec.serialintent", PackageManager.GET_META_DATA).applicationInfo.sourceDir
            val classloader = DexClassLoader(
                path,
                null,
                null,
                ClassLoader.getSystemClassLoader()
            )
            val cls = classloader.loadClass("com.mobisec.serialintent.FlagContainer")
            val m = cls.getDeclaredMethod("getFlag")
            m.isAccessible = true

            val extras = data?.extras
            if (extras != null) {
                extras.classLoader = cls.classLoader
                val fc = extras?.get("flag")
                val flag = m.invoke(fc) as String
                Log.e(TAG, flag)
            }

        } catch (e: Exception) {
            Log.e(TAG, e.stackTraceToString())
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val intent = Intent("android.intent.action.MAIN")
        intent.component =
            ComponentName("com.mobisec.serialintent", "com.mobisec.serialintent.SerialActivity")
        startActivityForResult(intent, 0)
    }
}