# Solution

## Description of the problem

The *serialintent* challenge requires to develop an app that requesta a service via intent.

## Solution

The main obstacle in this challenge is that the service returns an extra as a Parcel which is not available to us at build time such that it needs to be deserialized at run time using dynamic code loading and reflection. We setup the intent request like in the *justask* challenge, however, `onActivityResult` needs major modifications. In order to deserialize the parcel we load the `com.mobisec.serialintent` package. We use `packageManager.getPackageInfo` and pass the returned location to `DexClassLoader` to finally load `com.mobisec.serialintent.FlagContainer` using `loadClass()`.  
Whereafter we can acquire the `getFlag()` method using reflection, overwriting the `isAccessible` attribute in the process since the method is initially private.
Finally, we set the appropiate classloader and load the extra, on which we can onvoke the previously obtained method and log the result.
