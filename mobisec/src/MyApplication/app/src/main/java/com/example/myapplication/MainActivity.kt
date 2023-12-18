package com.example.myapplication

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
    import android.util.Log

class MainActivity : AppCompatActivity() {
    private val TAG = "MOBISEC"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.v(TAG, "hello-world-mobisec-edition")
    }
}