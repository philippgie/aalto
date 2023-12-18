package com.example.unbindable

import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.os.Bundle
import android.os.Handler
import android.os.IBinder
import android.os.Message
import android.os.Messenger
import android.os.RemoteException
import android.util.Log
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {
    /** Messenger for communicating with the service.  */
    private var mService: Messenger? = null
    private var mMessenger: Messenger? = null

    /** Flag indicating whether we have called bind on the service.  */
    private var bound: Boolean = false

    var TAG = "MOBISEC"

    class IncomingHandler : Handler() {
        override fun handleMessage(msg: Message) {
            var b = msg.obj as Bundle
                when(msg.what) {
                    4 -> Log.e("MOBISEC", "Following message was received in case 1: ${b.getString("flag")}")
                    1 -> Log.e("MOBISEC", "Following message was received in case 1: ${b.getString("flag")}")
                    else -> Log.e("MOBISEC", "got else")
                }
        }
    }

    /**
     * Class for interacting with the main interface of the service.
     */
    private val mConnection = object : ServiceConnection {

        override fun onServiceConnected(className: ComponentName, service: IBinder) {
            // This is called when the connection with the service has been
            // established, giving us the object we can use to
            // interact with the service.  We are communicating with the
            // service using a Messenger, so here we get a client-side
            // representation of that from the raw IBinder object.
            mService = Messenger(service)
            bound = true

            val msg: Message = Message.obtain(
                null,
                1
            )
            msg.replyTo = mMessenger
            mService!!.send(msg)
            sayHello()

        }

        override fun onServiceDisconnected(className: ComponentName) {
            // This is called when the connection with the service has been
            // unexpectedly disconnected&mdash;that is, its process crashed.

            mService = null
            bound = false
        }
    }

    fun sayHello() {
        if (!bound) return
        // Create and send a message to the service, using a supported 'what' value.
        val msg: Message = Message.obtain(null, 4)
        msg.replyTo = mMessenger
        try {
            mService?.send(msg)
        } catch (e: RemoteException) {
            e.printStackTrace()
        }

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }


    override fun onStart() {
        super.onStart()
        mMessenger = Messenger(IncomingHandler())

        val intent = Intent()
        intent.component = ComponentName("com.mobisec.unbindable", "com.mobisec.unbindable.UnbindableService")
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE)
        }

    override fun onStop() {
        super.onStop()
        // Unbind from the service.
        if (bound) {
            unbindService(mConnection)
            bound = false
        }
    }

}