# What is the password?
### Category: Network (Easy I or II)
### Author: Ned Liu

Users should have access to the Packet.pcapng file. Nothing else.

## Description

N-Team,

Recon managed to capture some web traffic from OmniFlags and think they are onto something. It looks like they are transferring something but we cannot read it.

HQ

## Hints
1. Is that a PNG? If it is then how can I download it?

## Solution
1. Open the packet in wireshark and you will see one ip is transferring a file to another ip using HTTP.
2. Follow the TCP stream in wireshark so you will get the content of the file.
3. Save the file as RAW value into a text file and make sure only the file only contains the PNG file.
4. Open the PNG file for the flag

## Flag
magpie{wh3r3_15_7h3_p455w0rd}
