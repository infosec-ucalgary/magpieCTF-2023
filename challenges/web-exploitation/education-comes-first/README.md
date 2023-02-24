# Education Comes First
### Category: Web Exploitation
### Dificulty: Easy
### Author: Auric Adubofour-Poku (AJay)

## Description

**URGENT**  W-Team,

An OmniFlags educational website has been hacked by one of the captives in the space prison, who we believe has left a message for us. Can you find it?

Good luck, \
HQ

## Hints
1. Someone had to add code to cause the flash on the screen. I wish we could stop this code from executing.

## Solution
When the website is first loaded, it can be seen that there is a flag that quickly changes to the omni flags logo after less than a second. Reloading this page will show the flash of the flag again but it is not fully readable. In order to keep the flag on the screen to make it readable, you need to:

1. Inspect the page
2. Navigate to the sources tab and open the javascript file
3. Create a breakpoint in this javascript file
4. Reload the page and the flag will stay on the screen.

## Flag
magpie{W3_R_S71LL_H3R3}

