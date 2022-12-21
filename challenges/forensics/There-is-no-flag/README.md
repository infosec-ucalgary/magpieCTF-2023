# There is no flag
### Category: Forensics (Medium)
### Author: Ned Liu
## Description

There is no flag. Even if there was a flag here, it is GONE now. Have a wonderful day!

Your starting point: https://github.com/lybned/NoFlag

## Hint
1. Hmmm, I wonder what happened to here before I arrived.
2. The program might be more useful than you thought. Are there anything else in the program?
3. What type of file is it?

## Solution

1. When you arrive at the github repo, you can see that this is NOT the first commit to the github. If you go to the github repo history, you can see there is a file named "program".
2. If we binwalk the program we can notice that the file contains another zip file.
3. If we extract the zip file, we will notice that there is a file named "Flag.PNG" in it. But we cannot open it.
4. When looking at the file in binary, we will see that the first byte of the file magic is wrong. We fix the file magic and then we get the flag.

## Flag
 magpie{m15510n_c0mpl373_w17h_r35p3c7}
