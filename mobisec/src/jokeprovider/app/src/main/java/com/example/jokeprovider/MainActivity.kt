package com.example.jokeprovider

import android.annotation.SuppressLint
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    var TAG = "MOBISEC"

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var PROVIDER_NAME = "com.mobisec.provider.Joke";
        var URL : Uri? = Uri.parse("content://$PROVIDER_NAME/jokes")

// A "projection" defines the columns that will be returned for each row
        val mProjection: Array<String> = arrayOf(
            "id",    // Contract class constant for the _ID column name
            "author",   // Contract class constant for the word column name
            "joke"  // Contract class constant for the locale column name
        )

// Defines a string to contain the selection clause
        var selectionClause: String? = "author=?"

// Declares an array to contain selection arguments
        var selectionArgs: Array<String>  = arrayOf("reyammer")


// Does a query against the table and returns a Cursor object
        var mCursor = URL?.let {
            contentResolver.query(
                it,
                mProjection,
                selectionClause,
                selectionArgs,
                "id"
            )
        }

// Some providers return null if an error occurs, others throw an exception
        when (mCursor?.count) {
            null -> {
                Log.v(TAG, "mCursor.count == null")
                /*
                 * Insert code here to handle the error. Be sure not to use the cursor!
                 * You may want to call android.util.Log.e() to log this error.
                 *
                 */
            }
            0 -> {
                Log.v(TAG, "mCursor.count == 0")
                /*
                 * Insert code here to notify the user that the search was unsuccessful. This isn't
                 * necessarily an error. You may want to offer the user the option to insert a new
                 * row, or re-type the search term.
                 */
            }
            else -> {
                Log.v(TAG, "mCursor.count > 0")
                if(mCursor.moveToFirst()) {
                    var strBuild= StringBuilder();
                    while (!mCursor.isAfterLast()) {
                        strBuild.append("\n"+mCursor.getString(mCursor.getColumnIndex("joke")));
                        mCursor.moveToNext();
                    }
                    Log.v(TAG, strBuild.toString())
                }
                else {
                    Log.v(TAG, "No Records Found");
                }
            }
        }
    }
}