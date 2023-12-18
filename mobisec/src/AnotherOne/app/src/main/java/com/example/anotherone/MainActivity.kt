package com.example.anotherone

import android.content.BroadcastReceiver
import android.content.Context
import android.content.IntentFilter
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {
    val br: BroadcastReceiver = FlagReceiver()
    val FLAG_ACTION = "com.mobisec.intent.action.FLAG_ANNOUNCEMENT"
    private val filter = IntentFilter(FLAG_ACTION)
    private val receiverFlags = Context.RECEIVER_EXPORTED



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        this.registerReceiver(br, filter, receiverFlags)

    }
}