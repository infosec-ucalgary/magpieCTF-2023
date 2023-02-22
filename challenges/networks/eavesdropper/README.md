# eavesdropper
### Category: Networks
### Author: Marianna Ferreira (MariannaFerr)

## Description

Hello N-Team,

According to recon, one of the captives has been sending flags discretely to our endpoints, and the key flag is going to have something to do with her favourite food. Unfortunately, you just can't remember what it was...

She's got a sweet tooth, so research has determined that it has to do with some kind of dessert, like **cookies** or **cakes**. What was her favourite flavour? **vanilla**? **oreo**? **chocolate chip**? Or maybe **pistachio**?

Good luck, \
HQ

## Hints
1. The flag is in l33t5p34k, containing numbers and underscores, and will be obvious when you see it!
2. There's probably some REGULAR way to filter for her flag...

## Solution
This would be the solution to the challenge!

1. Open .pcapng file
    * Preferably using Wireshark
2. navigate to search bar
3. Filter for flag using regular expression
    * some solutions include:
        * **frame matches "magpie{[cholateipkis_0-9]+}"**
        * **frame matches "magpie{[acehiklopst_0-9]+}"**

## Flag
magpie{chOc0LatE_Ch1p_c0Ok1e5}
