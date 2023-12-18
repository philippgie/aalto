package com.example.anotherone

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log

class FlagReceiver : BroadcastReceiver() {

    private val TAG = "MOBISEC"

    override fun onReceive(context: Context, intent: Intent) {
        val action = intent.action
        Log.v(TAG, "action=$action")
        if (action != null && action == "com.mobisec.intent.action.FLAG_ANNOUNCEMENT") {
            val flag = intent.getStringExtra("flag");
            if (flag != null) {
                Log.v(TAG, flag)
            }

            // Do something when the custom action is received
            // You can access data from the intent if needed
        }
        }
    }