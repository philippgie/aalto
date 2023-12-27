#!/usr/bin/python3
import sys,os
import time
import math


def integrate_numerically(lower: int, upper: int) -> int:
    integral =  []
    for i, n in enumerate((10, 100, 1000, 10000, 100000, 1000000)):
        dx = (upper-lower)/n 

        acc = 0
        for j in range(0, n):
            xip12 = dx*(j+0.5)
            dI = abs(math.sin(xip12))*dx
            
            acc += dI
        integral.append(acc)
    return integral

if __name__ == '__main__':
    i = integrate_numerically(0, 3.14159)
    print(i)

