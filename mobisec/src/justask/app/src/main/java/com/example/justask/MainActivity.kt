package com.example.justask

import android.content.ComponentName
import android.content.Intent
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    val TAG = "MOBISEC"

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        Log.v(TAG, "requestCode: " + requestCode)
        Log.v(TAG, "resultCode: " + resultCode)

        if (data != null) {
            Log.v(TAG, data.toUri(Intent.URI_INTENT_SCHEME))
        }

        val bundle = data?.extras
        if (bundle != null) {
            for (key in bundle.keySet()) {
                Log.v(TAG, key + " : " + if (bundle[key] != null) bundle[key] else "NULL")
                if(key == "follow") {
                    val follow = data.getBundleExtra("follow")
                    if (follow != null) {
                        extractbundle(follow)
                    }
                }
            }
        }
    }

    fun extractbundle(bundle : Bundle) {
        for(key in bundle.keySet()) {
            val obj : Bundle? = bundle.getBundle(key)
            if (obj == null) {
                Log.v(TAG, key + ": " + bundle.getString(key) ?: "")
            }
            else {
                Log.v(TAG, key)
                extractbundle(obj)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val oneIntent = Intent()
        oneIntent.component = ComponentName("com.mobisec.justask", "com.mobisec.justask.PartOne")
        startActivityForResult(oneIntent, 1);

        val twoIntent = Intent("com.mobisec.intent.action.JUSTASK")
        twoIntent.component = ComponentName("com.mobisec.justask", "com.mobisec.justask.PartTwo")
        startActivityForResult(twoIntent, 2);

        val threeIntent = Intent()
        threeIntent.component = ComponentName("com.mobisec.justask", "com.mobisec.justask.PartThree")
        startActivityForResult(threeIntent  , 3);

        val fourIntent = Intent("com.mobisec.intent.action.JUSTASKBUTNOTSOSIMPLE")
        fourIntent.component = ComponentName("com.mobisec.justask", "com.mobisec.justask.PartFour")
        startActivityForResult(fourIntent, 4);

    }
}