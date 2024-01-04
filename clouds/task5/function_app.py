import azure.functions as func
import azure.durable_functions as df

import re
from collections import namedtuple

from azure.identity import DefaultAzureCredential
from azure.storage.blob import BlobServiceClient, BlobClient, ContainerClient

indexed_line = namedtuple('indexed_line', ['line_number', 'line'])
pair = namedtuple('pair', ['word', 'ocurrences'])


myApp = df.DFApp(http_auth_level=func.AuthLevel.ANONYMOUS)

# An HTTP-Triggered Function with a Durable Functions Client binding
@myApp.route(route="orchestrators/{functionName}")
@myApp.durable_client_input(client_name="client")
async def http_start(req: func.HttpRequest, client) -> func.HttpResponse:
    function_name = req.route_params.get('functionName')
    instance_id = await client.start_new(function_name)
    response = client.create_check_status_response(req, instance_id)
    return response


# Orchestrator
@myApp.orchestration_trigger(context_name="context")
def hello_orchestrator(context):
    tasks = []
    for i in range(1,5):
        tasks.append(context.call_activity("GetInputDataFn", f"mrinput-{i}.txt"))
    r = yield context.task_all(tasks)
    r = [x for xs in r for x in xs]

    tasks = []
    for pair in r:
        tasks.append(context.call_activity("mapper", pair))
    map_results = yield context.task_all(tasks)
    map_results = [x for xs in map_results for x in xs]

    shuffle_results = yield context.call_activity("shuffler", map_results)

    tasks = []
    for word in shuffle_results:
        tasks.append(context.call_activity("reducer", word))
    reducer_results = yield context.task_all(tasks)

    return reducer_results


# Activity
@myApp.activity_trigger(input_name="iline")
def mapper(iline: indexed_line):
    line_number, line = iline
    return [(word.lower(), 1) for word in re.compile('\w+').findall(line)]

@myApp.activity_trigger(input_name="pair")
def reducer(pair: pair):
    return (pair[0], len(pair[1]))

@myApp.activity_trigger(input_name="file")
def GetInputDataFn(file : str):

    account_url = "https://mapreduceassignment2.blob.core.windows.net/mrinput/"
    default_credential = DefaultAzureCredential()

    # Create the BlobServiceClient object
    blob_service_client = BlobServiceClient.from_connection_string("<redacted>")


    container_client = blob_service_client.get_container_client(container= "mrinput") 
    input = [indexed_line(line_number=i, line= line.decode()) for i, line in enumerate(container_client.download_blob(file).readall().splitlines())]
    return input

@myApp.activity_trigger(input_name="maps")
def shuffler(maps: list):
    to_be_reduced = {}
    for k, v in maps:
        if k in to_be_reduced:
            to_be_reduced[k].append(v)
        else:
            to_be_reduced[k] = [v]

    return list(to_be_reduced.items())
