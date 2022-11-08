# So meta
### Category: Forensics
### Author: Christina He
## Description
1 1 0
1 0 1
0 1 1
0 0 0
## Hint
1. You need the correct tool to get the flag.
2. Maybe take a look into the metadata

## Solution
1. Using exiftool, you would find the the tool, stegsolve that you need for this challenge
2. Download stegsolve from the github page https://github.com/zardus/ctf-tools/blob/master/stegsolve/install and open the program with 'java -jar stegsolve.jar'
3. xor challenge.png with combine.png with stegsolve to get the flag (file->open->challenge.png, analysis->image combiner->combine.png)

## Flag
 magpie{H0p3fully_U_D1dnt_Brut3_f0rc3_1t}
