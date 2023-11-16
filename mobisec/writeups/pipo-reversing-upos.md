# Solution

## Description of the problem

The `upos` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. In the main activity I saw some print statements for detecting various analysis techniques, such as frida or a debugger. Upon executing the application in the emulator I was denied with an error message indicating that I modified the apk, even though I didn't(?). Through backtracing the variables indiciating whether an analysis technique is used, I found the `initActivity` method that checks them. I attempted to hook this method with frida, however, it wasn't sucessful since the method is executed before frida can work. I went back to static analysis. There is only one line which allows to return *true* from the `checkFlag` method. I attempted to understand when that would ocurr, which is the case if in the preceeding loop, the 30 values in the `fs` boolean array are true. This condition is satisfied by providing a flag input, that is presumably hashed or similar by a seemingly one-way function and compared with some values from a file called `lotto.dat`. I attempted to debug the application in order to find the values at the time of comparison. While some progress was made by patching out the intructions which for calling `initActivity`. However, there were further checks inside the checkFlag method. I attempted to remove those as well but this seemed to corrupt the apk somehow. Instead, I realized that due to the one way function this approach would not help anyway. I extraced the relvant code to brute the flag two bytes at a time. However, the app uses an lfsr, where it is unknown how many steps are executed. I did a frequency count for "step" in the FlagChecker class, where it was 20 times. Since I did not know how many times it is acutally executed I brute-forced for all possible steps between 0 and 20.


## Optional Feedback

N/A
