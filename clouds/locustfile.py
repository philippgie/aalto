#!/bin/usr/python3

import time
from locust import HttpUser, task ,between

class QuickstartUser(HttpUser):
    @task
    def hello_world(self):
        self.client.get("/ni?lower=0&upper=3.1459")
