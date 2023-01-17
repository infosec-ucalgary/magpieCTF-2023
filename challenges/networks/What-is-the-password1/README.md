# What is the password?
### Category: Network (Easy I or II)
### Author: Ned Liu

## Description
We managed to capture some web traffic from the OMNI flag company. This time I think we are onto something. It looks like they are transferring something but I cannot read it.

## Hints
1. If only there is a way to look at what is transfered.

## Solution
1. Open the packet in wireshark and you will see one ip is transferring a file to another ip using HTTP.
2. Follow the TCP stream in wireshark so you will get the content of the file.
3. Save the file as RAW value into a text file and make sure only the file only contains the PNG file.
4. Open the PNG file for the flag

## Flag
magpie{wh3r3_15_7h3_p455w0rd}
