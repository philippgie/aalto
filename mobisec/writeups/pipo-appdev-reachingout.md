# Solution

## Description of the problem

The reachingout challenge requires to develop an app that retrieves the flag from a server that is only reachable from the analysis system.

## Solution

I've solved the challenge by developing an app doing an initial http request to **10.0.2.2:31337**. I implemented the functionality using *HttpURLConnection*. In the manifest we have to declare `android:usesCleartextTraffic="true"` to allow the usage of http. Inside the code, it is necessary to set 

```

val policy = ThreadPolicy.Builder().permitAll().build()
StrictMode.setThreadPolicy(policy)

```
or use tasks otherwise.

After connecting, we are redirectedd as the requested webpages body suggests:

> You can get the flag \<a href="http://10.0.2.2:31337/flag"\>here\</a\>.

Requesting that page returns:

> How much is 3 + 6?        <form action="/flag" method="POST">          <label for="answer">Insert Answer</label>          <input id="answer" name="answer" required type="text" value="">          <input id="val1" name="val1" type="hidden" value="3">          <input id="oper" name="oper" type="hidden" value="+">          <input id="val2" name="val2" type="hidden" value="6">          <input type="submit" value="Get Flag">        </form>       

Now, we have to send a post message.
We set the necessary header information such as *Content-Type* -> *application/x-www-form-urlencoded* and the correct *Content-Length*, as well as setting the request body to `answer=9&val1=3&oper=%2B&val2=6`. Using this, we are able to obtain the flag.

## Optional Feedback

This challenge was annoying because the application supplied very little debugging information in the last step.

