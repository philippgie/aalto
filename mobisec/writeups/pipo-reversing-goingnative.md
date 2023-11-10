# Solution

## Description of the problem

The `goingnative` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. First, I examined the seeming intersting `FlagChecker` class. This class checks whether an input starts with "MOBISEC{" and ends with "}". The string is split based on the delimiter "-", whereafter it is checked that the second part consists of 6 digits before being passed to a native library function along with the first part. The native library can be reverse engineered using tools such as IDA or ghidra. Using the IDa decompiler we can see that the first part must be "native_is_so" and the second part must be *31337*. Since the java code verified that the length is 6 digits, the complete value of the second part must be *031337*.

## Optional Feedback

N/A
