# Layered
### Category: Forensics
### Author: Christina He

## Description
1+1=0

## Hint
<<<<<<< HEAD
1. I wonder if I can unzip the files automatically.
2. What's written in the metadata of "challenge.png"?
3. You need to find the exactly correct tool to get the flag.

## Solution
There are three steps in this challenge.
1. Unzip files using a script of your choice. See https://www.codegrepper.com/code-examples/shell/unzip+recursively+linux for example
2. Install stegsolve from https://github.com/zardus/ctf-tools/blob/master/stegsolve/install
3. According to the instruction, open stegsolve and combine two images given with xor to get the flag
=======
1. What's written in the metadata of "challenge.png"?
2. You need to find the exactly correct tool to get the flag.

## Solution
There are two steps in this challenge.
1. Install stegsolve https://github.com/zardus/ctf-tools/blob/master/stegsolve/install
2. According to the instruction, open stegsolve and combine two images given with xor to get the flag
>>>>>>> 47ffa19 (removed zip part of the challenge; modified README.md file)

## Flag
magpie{H0p3fully_U_D1dnt_Brut3_f0rc3_1t}
