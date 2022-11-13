# Take a wild guess
### Category: Reverse Engineering (Medium)
### Author: Ned Liu

## Description
We got a copy of the code to a top secret vault. Unfortunately we do not have the password. Could we try to unlock it and possibly get a flag on the way?

## Hints
1. There are some really long strings in the code. I wonder what they mean.
2. It looks like it is comparing something with a string in the program. I wonder what could that be?

## Solution
1. Looking at the reverse binary, we can see there is a long string being (split into 4 parts). If we look closer, it is all the possible characters for base 64 encoding.
2. Then we can see that the program passes each character from the user input into the a function that converts the char to binary in string.
3. After converting all the input chars to binary, it will check the length of the input. If it is NOT divisble by 6 then it append '0' to the end of the string. (Base64?)
4. Next, the program will run a while loop. This is the loop that will convert the binary numbers to a base 64 string.
5. Right after that, the program append '=' to the end of the base 64 string. 
6. The last part of the program will compare if the base64 encoded xor'ed with 0x28 (40) is equals to the one in the program.
7. If we get the get the string in the program and xor it with 0x28 then we get a base 64 encoded string. If we decode the string. It will give us the flag.


## Flag

magpie{b45364_3ncryp710n-rul35!}