# Solution

## Description of the problem

The `loadme` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. First, I set a breakpoint on the `m5ds()` function. Upon obfuscating some of the strings that way I realized that the application is downloading another apk, which is then loaded. Before running that file is deleted. I set a breakpoiint just before the file is delted and analyzed it in jadx. Quickly I relaized that this file is loading yet another file. I did not manage to run the file itself to repeat the procedure I used for the first stage. Hence, I used frida to gather the file name of the next stage, *logo.dex*. On the phone I ran an infinite loop trying to create a copy of *logo.dex*. After a few tries the file got succesfully copied before being deleted. Opening the file in jadx gives me the flag.

## Optional Feedback

N/A
