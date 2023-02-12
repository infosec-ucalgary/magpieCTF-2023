# missing-flag
### Category: Reverse Engineering
### Author: Christina
## Description
We found a word document left off on one of the Omniflag's computer. Does it contains any information useful to us? Not really sure, but it looks like it was in the middle of execution of something. What could it be?

## Hints
1. Macros give you surprises in word.
2. Resource abundance

## Solution
1. After opening the word document, you would see a long number 1090970103011201050101012301190104012109501000490100095049095048011205101100950116010404901150950109052099011404809508408708401250
2. Using the description and hints, you guess the flag is probably hidden in macro. Open the macro, you would see the obfuscated vbscript.
3. After some renaming, you would find out some variables declared are never used. Getting rid of those first.
4. Cleaning up a bit, you would see only vusirht and iruthvb are used.
5. Knowing vusirht, you can speculate that iruthvb is acted as an index. Searching on Google tells you that Asc() turns an ascii character to an integer.
6. Putting all the information together, you would notice, this program takes an array of characters, turn them into ascii value, multiply by 10 and concatenate them together. Using your knowledge about ascii, you know the flag is between ascii value 32 and 127. With some patience, separate the number and then convert them to ascii characters, you will get the flag.

## Flag
magpie{why_d1d_1_0p3n_th1s_m4cr0_TWT}
