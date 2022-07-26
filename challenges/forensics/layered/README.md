# Layered
### Category: Forensics
### Author: Christina He

## Description
I got a zip file and not sure what to do with it. Hmmm...



## Hint
1. Do not unzip file manually, it's gonna take too long.
2. What's written in the metadata of "challenge.png"?
3. You need to find the exactly correct tool to get the flag.

## Solution
There are three steps in this challenge.
1. Unzip files using a script of your choice. See sol.sh for example
2. Install stegsolve https://github.com/zardus/ctf-tools/blob/master/stegsolve/install
3. According to the instruction, open stegsolve and combine two images given with xor to get the flag

## Flag
magpie{H0p3fully_U_D1dnt_Brut3_f0rc3_1t}