# Do it again
### Category: OSINT
### Author: Ned Liu

## Description

Hello O-Team,

One of our recon squads has managed to sabotage the headquarters and recover a flag under a Twitter account that is made up of a name and a number. 
The person made a full report on the process of discovering the flag and sent it to us. 
However, the communication is disrupted and we do not know how much of the report got delivered and we also do not know where the message ended up at. 
Fortunately, it looks like one of their employees received part of the message and posted it on Twitter. Let's see if we can do something about it.

Report back with your findings.

HQ

## Hints


1. Oh no, it seems like the last part is gone. Only if I can go WAY BACK into the past with a time MACHINE.
2. The screenshot looks familiar, I am sure you can find the EXACT same text in the website.



## Solution
1. Going to the Twitter post, we can see (4/4) is gone. It could be deleted by the original poster. So we use a way back machine to see what it is.
2. Lucky, there is a record in the way back machine, it seems like the deleted post contains a number and a screenshot of another website.
3. There are key phrases like "Missions insignia", "Mercury-Redstone 3" and "Apollo 14" in the screenshot that we can search by. By putting these words in the google search engine with double quotes.
4. This leads us to a wiki page for Alan Shepard and you will find the exact same image as the screenshot.
5. The description mentioned that the flag is made up of a name and a number so we look for a Twitter account with Alan Shepard and the number we had earlier. ("AlanShepard1248") This will lead us to the flag.

## Start link
https://twitter.com/JohnDoe20418691/status/1617755100549902339


## Flag
magpie{7h3_m023_y0u_kn0w}
