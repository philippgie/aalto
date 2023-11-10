# Solution

## Description of the problem

The `goingnative` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. First, I examined the seeming intersting `FlagChecker` class. The given method takes a 6 digit Pin, hashes it *10000* times, and checks it against a hash. Trying all possible 6 digit PINS can be done on commodity hardware. I used the following script:


import hashlib
import itertools
import multiprocessing

```python

    #iterates over all 6 digit combinations
    def brute_force(r : int):
        MAX_PIN = 1000000
        THREADS = 10
        offset = MAX_PIN//THREADS
        for x in range(r*offset, (r+1)*offset):
            pin = str(x).encode()
            for i in range(0, 25 * 400):
                m = hashlib.md5()
                m.update(pin)
                pin = m.digest()
        
            if pin.hex() == "d04988522ddfed3133cc24fb6924eae9":
                print(x)
                break;
    
    if __name__ == '__main__':
        THREADS = 10
        pool = multiprocessing.Pool(THREADS)
        out1, out2, out3 = zip(*pool.map(brute_force, range(THREADS)))

```



## Optional Feedback

N/A
