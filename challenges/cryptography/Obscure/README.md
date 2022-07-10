# Obscure
### Category: Cryptography
### Author: Daniel Sabourov (Sabo)

## Description
Hello C-Team,

You have been tasked with decrypting what we believe to be an encrypted file that may contain important data that may help our efforts in extracting the prisoners. The aformentioned file is attached to this message. Let us know if you manage to find anything of value.

Best of luck,
HQ

P.S. Any flags you find should be **entirely lowercase**.

## Hints
1. The file is password protected?? Where do we get the password from...?
2. Some parts of this comment stick out. They seem a little out of place...

## Solution
The name of the file is encoded in Base64, and decrypts to "b3st_cryp70gr4pHeR!", which is the password to access the file. Once they're in, they'll see a comment above an encrypted string. The string is the flag encrypted using the Myszkowski Transposition cipher. The key is the capitalized word in the comment, "SORTING", and using that key, the string will decrypt to the flag.

1. Base64 decode the file name
2. Use the decoded file name to access the contents of the file
3. Decrypt the ciphertext using Myszkowski Transposition Cipher using the word "ATTACK"
    * [This site](https://crypto.interactive-maths.com/myszkowski-transposition-cipher.html) may be helpful.
4. Profit


## Flag
magpie{7h1s_guy'5_g3tt1ng_f1r3d}