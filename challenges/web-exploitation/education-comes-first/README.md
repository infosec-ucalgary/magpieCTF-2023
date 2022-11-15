# OmniFlags Education
### Category: Web Exploitation
### Dificulty: Easy
### Author: Auric Adubofour-Poku

## Description
A Omni Flags educational website has been hacked by a captive executive who has left a message for you. Can you find it?
  
## Solution
When the website is first loaded, it can be seen that there is a flag that quickly changes to the omni flags logo after less than a second. Reloading this page will show the flash of the flag again but it is not fully readable. In order to keep the flag on the screen to make it readable, you need to:

1. Inspect the page
2. Navigate to the sources tab and open the javascript file
3. Create a breakpoint in this javascript file
4. Reload the page and the flag will stay on the screen.

## Flag
ucsec{WE_ARE_STILL_HERE}

### Languages Used:
- HTML
- CSS
- JavaScript
