<p align="center">
<img src=https://gridshl.files.wordpress.com/2022/06/img_3487.png>
</p>  

----------------

# What you will need ?
This tutorial assumes that you have an ESP32 device ready to join the GridShell and that you have Arduino application installed.
Arduino needs to be able to upload sketches to your ESP32, therefore ensure that this is all working before moving forward.

# Step 1 - standalone miner code
You will upload a stand alone miner code, the sketch will contribute to the GridShell, making your device an official Miner on the network.

- Create a folder on your desktop called **ClientESP32**

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut1.png)


- [Download](https://github.com/invpe/gridshell/tree/main/Miners/ESP32) all files from the latest ESP32 library sources 
and put them to the **ClientESP32** folder. This is the strucutre you should be having after completing this step.


![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut2.png)


- Open the sketch by doubleclicking on ClientESP32.ino file


![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut3.png)


- Arduino will launch and you will be presented with the Miner software.

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut4.png)

Step 1 completed, good job ! 🆒

------------------

# Step 2 - provide your WiFi credentials

The sketch requires few items from you to be provided, first let's update your WiFi credentials.
Miner needs to be able to connect to the internet, thus the WiFi credentials are required.

Put these details here, line 15 and line 16 of the code replacing XX's and YY's with your wifi details.


![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut5.png)

Step 2 completed, great ! 🆒


--------------------------

# Step 3 - provide GridShell user id

The final update is putting your GridShell user id to the sketch.
This will be your id of the user on the grid, so keep it safe and never share it.
Also, since the protocol is plain text, rather try to connect your miners from trusted network
so that it cannot be picked up by somebody.

Here, create your user id: [Click](https://explorer.gridshell.net/api/getuser/)

And you will be presented with two values

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut6.png)

 

❗❗UserHash (Private)  never share it  
 

Copy the _UserHash (Private)_ to your sketch at line 21, like so:

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut7.png)

This one _Username (Public)_ is what will be your public user id, anyone can see it, you can share it and explore it in a moment!

Ok, this step is done ! 🍪

---------------------------------

# Step 4 - upload the code to ESP32

Now that we have sketch and it's code updated, account created, the final thing is uploading it to the device.
Upload the sketch with arduino to your ESP32 device. 

* Click Sketch -> Upload in the Arduino

The uploading should start, arduino will let you know it's uploading :

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut8.png)

Once the upload is done, your miner is connecting to the GridShell servers.

All steps are completed now ! Congratulations.
Go and check your miner and user by opening the GridShell user explorer.

-------------------------------------

# Let's look at your new user and newly connected miner

[Open up GridShell explorer](https://explorer.gridshell.net:3000/d/MAgVXZj7k/user-explorer) page 

Provide your newly created **public** userid, for me it was _VSVTSPPSUSVSP1T1V11T11S1111P1UV11TR1U1ST_ 

![GridShell](https://github.com/invpe/gridshell/blob/main/Resources/Tut9.png)



