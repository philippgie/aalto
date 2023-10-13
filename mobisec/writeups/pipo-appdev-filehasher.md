# Solution

## Description of the problem

The reachingout challenge requires to develop an app that reads a file from external storage and returns the SHA256 hash in a result intent.

## Solution

I've solved the challenge by developing an app doing that implements an activity hashfile. First, it reads the intent data and converts the *URI* to a path. Then, a function `hash` reads the file as a bytearray and creates the SHA256 hash of it. Thereafter, the returned value is set in the result intent.

Additionally, the correct permissions must be set in the manifest.

```
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```
