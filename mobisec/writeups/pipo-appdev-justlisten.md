# Solution

## Description of the problem

The justlisten challenge requires to develop an app that implements a broadcast receiver that waits to recevie an implicit broadcast with the action *com.mobisec.intent.action.FLAG_ANNOUNCEMENT*.

## Solution

I've solved the challenge by developing an app implementing a `BroadcastReceiver` called `FlagReceiver`. The `BroadcastReceiver` is implemented in the code because manifest declared receivers are deprecated:

> Note: If your app targets API level 26 or higher, you cannot use the manifest to declare a receiver for implicit broadcasts (broadcasts that do not target your app specifically), except for a few implicit broadcasts that are exempted from that restriction. In most cases, you can use scheduled jobs instead.

Knowing, this it is straightforward to implement the receiver and printing the receveid intent's extra `flag` as indiciated in the challenge description.

