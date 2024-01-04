import json
import numpy as np

def parse_locust(filename: str) -> [int]:
    with open(filename, "r") as f:
        x = json.load(f)
        reqs_per_sec = x['num_reqs_per_sec']
        fail_per_sec = x['num_fail_per_sec']
        start = int(min(reqs_per_sec.keys()))
        end = int(max(reqs_per_sec.keys()))
        success_per_sec = [reqs_per_sec.get(str(i), 0) - fail_per_sec.get(str(i), 0) for i in range(start, end+1)]
        return success_per_sec

scaleset = parse_locust("locust_scaleset_3m_100u_20240104T000758.json")
appserv = parse_locust("locust_appserv_3m_100u_20240104T000206.json")
local = parse_locust("locust_local_3m_10000u_20240103T133502.json")
function = parse_locust("locust_function_3m_5000u_20240103T135452.json")

import matplotlib.pyplot as plt 
plt.xticks(range(0, 180 + 1, 20))

plt.plot(range(len(scaleset)), scaleset, color ='blue', label='scaleset')
plt.plot(range(len(appserv)), appserv, color ='red', label='appserv')
plt.plot(range(len(local)), local, color ='yellow', label='local')

plt.title("Scaleset, Appserv, and Local Statistics")
plt.xlabel("t in seconds")
plt.ylabel("successful requests per second")
plt.legend(loc="upper left")
plt.savefig('scaleset_appserv_local.png')
plt.close()

plt.xticks(range(0, 180 + 1, 20))
plt.title("Combined Statistics")
plt.xlabel("t in seconds")
plt.ylabel("successful requests per second")
plt.plot(range(len(scaleset)), scaleset, color ='blue', label='scaleset')
plt.plot(range(len(appserv)), appserv, color ='red', label='appserv')
plt.plot(range(len(local)), local, color ='yellow', label='local')
plt.plot(range(len(function)), function, color ='green', label='function')
plt.savefig('combined.png')
plt.close()

plt.xticks(range(0, 180 + 1, 20))
plt.title("Function Statistics")
plt.xlabel("t in seconds")
plt.ylabel("successful requests per second")
plt.plot(range(len(function)), function, color ='green', label='function')
plt.savefig('function.png')
plt.close()
