package com.example.whereareyou

import android.Manifest
import android.app.Service
import android.content.Intent
import android.content.pm.PackageManager
import android.location.Location
import android.os.IBinder
import android.util.Log
import androidx.core.app.ActivityCompat
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import com.google.android.gms.location.Priority

class LocationService : Service() {
    private lateinit var fusedLocationClient: FusedLocationProviderClient

    var TAG = "MOBISEC"

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {

        fusedLocationClient = LocationServices.getFusedLocationProviderClient(this)
        if (ActivityCompat.checkSelfPermission(
                this, Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(
                this, Manifest.permission.ACCESS_COARSE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            Log.v(TAG, "missing permissions")
            return super.onStartCommand(intent, flags, startId)

        }
        fusedLocationClient.getCurrentLocation(Priority.PRIORITY_HIGH_ACCURACY, null)
            .addOnSuccessListener { location: Location? ->
                if (location == null) {
                    Log.v(TAG, "location is null")
                }
                var i = Intent()
                i.action = "com.mobisec.intent.action.LOCATION_ANNOUNCEMENT"
                i.putExtra("location", location)
                sendBroadcast(i)
            }
        return super.onStartCommand(intent, flags, startId)
    }

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

}