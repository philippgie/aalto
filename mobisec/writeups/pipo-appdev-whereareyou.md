# Solution

## Description of the problem

The whereareyou challenge requires to develop an app that implements a service that returns the current location via broadcast intent.

## Solution

I've solved the challenge by developing an app that implements a service `LocationService`. We leverage `FusedLocationProviderClient` to obtain the current location as suggested in https://developer.android.com/training/location/retrieve-current. Using `getCurrentLocation` we can request an updated location and deliver it in the broadcast intent, utilizing the template code from the challenge description.

Additionally, the correct permissions must be set in the manifest.

```
<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
```
