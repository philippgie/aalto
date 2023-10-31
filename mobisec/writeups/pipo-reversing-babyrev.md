# Solution

## Description of the problem

The `babyrev` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. I replaced the function calls to `get\*` by their concrete values. Continuing, I iteratively reverse engineered the conditions inside the first if-condition in `checkFlag()`, slowly obtaining different parts of the flag. The interesting parts were related to `bam()` and `getR()`. `bam()` takes an input and increases its ascii value by 0xD if it is between a/A and m/M or decreases it by 0xD if outside. Hence, to reverse `ERNYYL` I did the opposite resulting in `REALLY`. Finally, the if-condition did a lot of uppercase conversation, such that our preliminary solution is not valid, which is where the `getR()` comes into play. This function returns a regex basically expressing that every character should alternate between lowercase and uppercase or be an undersocre, starting with an uppercase letter. Applying this condition to our preliminary solution leaves us with the flag.

## Optional Feedback

N/A
