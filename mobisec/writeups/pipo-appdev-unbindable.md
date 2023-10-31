# Solution

## Description of the problem

The *unbindable* challenge requires to develop an app that binds to a service, communicating with it and finally obtains the flag.

## Solution

I figured from the given *apk* that we must communicate with the service by using a `Messenger`. We requiere messenger for sending and receiving messages, one respectively.
I implemented the callback `handleMessage` that simply casts the `Message` to a bundle and logs the content.
We create an instance of `ServiceConnection` that initializes the sending `Messenger`, sending a message with the value `1` to register it and setting `replyTo` to the receiving `Messenger`.
Continuing, we do the same with value `4` to request the flag.

N.B. that we used

> intent.component = ComponentName("com.mobisec.unbindable", "com.mobisec.unbindable.UnbindableService")

to set the target of the intent as the Activity is not available at build time.
