# Layered
### Category: Forensics
### Author: Christina He

## Description
1 1 0
1 0 1
0 1 1
0 0 0

## Hint
<<<<<<< HEAD
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
=======
1. What's added in the metadata of "challenge.png"?
2. You need to find out the exact tool to get the flag.

## Solution
There are three steps in this challenge.
1. Using exiftool on challenge.png, there is a comment in the metadata pointing to stegsolve as the right tool.
2. Install stegsolve https://github.com/zardus/ctf-tools/blob/master/stegsolve/install
3. Follow the instruction, open stegsolve and combine two images given with xor to get the flag.
>>>>>>> 231d8a2 (modified README.md; modified metadata of challenge.png)

## Flag
magpie{H0p3fully_U_D1dnt_Brut3_f0rc3_1t}
