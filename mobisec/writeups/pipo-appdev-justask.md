## Description of the problem


The justask challenge requires to develop an app that requests four activities, each replying with a port of the final flag.

## Solution

I've solved the challenge by developing an app doing four requests. The different intents to be recevied are `com.mobisec.justask.PartOne`, `com.mobisec.justask.PartTwo`, `com.mobisec.justask.PartThree`, `com.mobisec.justask.PartFour`. See here for an example how to setup the Activity to be launcehd for the first part

```

    val oneIntent = Intent()
    oneIntent.component = ComponentName("com.mobisec.justask", "com.mobisec.justask.PartOne")
    startActivityForResult(oneIntent, 1);

```

To obtain the intent, for the second and fourth part we need to explicitly define the action as they have an intent-filter.
Interstingly, the three parts are easily parsed. The intents return a bundle, where I iterate over the elements with thhe `keySet` function. However, the fourth part ins encapsulated in multiple levels of bundles. I wrote a recursive function to extract the flag.


## Optional Feedback

