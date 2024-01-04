import azure.functions as func
import logging
import math

app = func.FunctionApp(http_auth_level=func.AuthLevel.ANONYMOUS)

@app.route(route="ni")
def numerical_integral(req: func.HttpRequest) -> func.HttpResponse:

    lower = req.params.get('lower', None)
    upper = req.params.get('upper', None)
    if(not lower or not upper):
        return func.HttpResponse("Missing arguments lower and/or upper. Please pass parameters like so:<br/><br/> <i>/ni?lower=0&upper=3.1459</i>", status_code=200)
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
    return func.HttpResponse(str(integral), status_code=200)