package com.example.reachingout

import android.os.Bundle
import android.os.NetworkOnMainThreadException
import android.os.StrictMode
import android.os.StrictMode.ThreadPolicy
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import java.io.BufferedInputStream
import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.IOException
import java.io.InputStream
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL
import java.nio.charset.StandardCharsets


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        req("http://10.0.2.2:31337/flag")
        //req("https://httpbin.org/post")
    }

    @Throws(IOException::class)
    private fun readStream(`is`: InputStream): String {
        val sb = StringBuilder()
        val r = BufferedReader(InputStreamReader(`is`), 1000)
        var line = r.readLine()
        while (line != null) {
            sb.append(line)
            line = r.readLine()
        }
        `is`.close()
        return sb.toString()
    }

    fun req(url: String) {

        val policy = ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)

        val TAG = "MOBISEC"
        val url = URL(url)
        val urlConnection = url.openConnection() as HttpURLConnection
        urlConnection.setRequestMethod("POST");
        urlConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        urlConnection.setDoInput(true);
        urlConnection.setDoOutput(true);
        val postData = "answer=9&val1=3&oper=%2B&val2=6".toByteArray(StandardCharsets.UTF_8)
        urlConnection.setRequestProperty("Content-Length", postData.size.toString())

        try {
            val outputStream = DataOutputStream(urlConnection.outputStream)
            outputStream.write(postData)
            outputStream.flush()
            outputStream.close()

            // Get the response from the server
            val responseCode = urlConnection.responseCode

            val `in`: InputStream = BufferedInputStream(urlConnection.inputStream)
            val r = readStream(`in`)
            if (r != null) {
                Log.v(TAG, r)
            }

        } catch (exc: NetworkOnMainThreadException) {
            Log.v(TAG, "exc")
            Log.v(TAG, Log.getStackTraceString(exc))

        } finally {
            Log.v(TAG, "disconnect")
            urlConnection.disconnect()
        }
    }
}
