#!/usr/bin/python3

import math

from flask import Flask 
from flask import request


app = Flask(__name__)

@app.route("/ni", methods=['GET'])
def integrate_numerically() -> int:
    lower = request.args.get('lower', None)
    upper = request.args.get('upper', None)
    if(not lower or not upper):
        return "Missing arguments lower and/or upper. Please pass parameters like so:<br/><br/> <i>/ni?lower=0&upper=3.1459</i>"
    integral =  []
    lower = float(lower)
    upper = float(upper)
    for i, n in enumerate((10, 100, 1000, 10000, 100000, 1000000)):
        dx = (upper-lower)/n 

        acc = 0
        for j in range(0, n):
            xip12 = dx*(j+0.5)
            dI = abs(math.sin(xip12))*dx
            
            acc += dI
        integral.append(acc)
    return integral
