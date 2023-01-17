# There is no flag
### Category: Forensics (Medium)
### Author: Ned Liu
## Description

There is no flag. Even if there was a flag here, it is GONE now. Have a wonderful day!

Your starting point: https://github.com/Admin-is-here-GG/React

## Hint
1. Hmmm, I wonder what happened here before I arrived.
2. The program might be more useful than you thought. Is there anything else in the program?
3. What type of file is it?

## Solution

1. When you arrive at the GitHub repo, you can see that this is NOT the first commit to GitHub. If you go to the GitHub repo history, you can see there is a file named "program".
2. If we string the program we can see "Flag.PNG" is present in the result. This potentially means that there is something else in the executable.
3. We binwalk the program and we will see there is another zip file in the executable.
4. Inside the zip, there is a file named "Flag.PNG" in it. But we cannot open it.
5. We can also see that the computer cannot determine the file type of the file. This indicates that the file magic might be damaged. We can figure what is going on with a hex viewer.
6. When looking at the file in binary, we will see that the first byte of the file magic is wrong. We fix the file magic and then we get the flag.

## Flag
 magpie{m15510n_c0mpl373_w17h_r35p3c7}

