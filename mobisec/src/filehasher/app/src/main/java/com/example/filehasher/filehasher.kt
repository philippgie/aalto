package com.example.filehasher

import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import java.io.File
import java.security.MessageDigest


class HashFile : Activity() {
    val TAG = "MOBISEC"
    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        val data: Uri? = intent?.data

        val hash: String? = data?.path?.let { hash(it) }

        // return the hash in a "result" intent

        val resultIntent = Intent();
        resultIntent.putExtra("hash", hash);
        setResult(Activity.RESULT_OK, resultIntent);
        finish()
    }

    fun hash(path : String): String {
        val bytes = File(path).readBytes()

        val md = MessageDigest.getInstance("SHA-256")
        val digest = bytes.let { md.digest(it) }
        return digest.fold("", { str, it -> str + "%02x".format(it) })


    }
}