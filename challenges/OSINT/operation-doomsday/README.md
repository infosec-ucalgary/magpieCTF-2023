# Operation DOOMSDAY
### Category: OSINT
### Author: Ejaaz (dmsday101)

## Description (Part 1)

Incoming transmission (Identity: unknown)...

OmniFlags has a new CEO: Trenton Blackwell. He is twice as nefarious as all their other CEO's combined and he has a terrible plan.
Somewhere deep in space he has set up a DOOMSDAY device on a mysterious planet. The ones that disappeared were getting close to finding its location but they went missing before they could make it.
What they were able to figure out is that Trenton has hidden the planets X and Y coords out there, your job: FIND THEM. 

You've got this, \
V

### Hints Part 1

1. Trenton Blackwell has an instagram: trentonblackwell37


## Description (Part 2)

Incoming transmission (Identity: unknown)...

Great job finding the X coordinate, only one coordinate left to find. You have access to Trentons blog, however he has covered his tracks and left hundereds of possibilities for the last flag and the y coordinate. 
Unfortunately we only have a limited number of tries to lock onto the DOOMSDAY device. 5 failed attempts will trigger the self destruct. 
I wonder if the post we want is a date that is important to him?

Keep at it, \
V

## Hints

## Solution (Part 1)

1. Trenton Blackwell has a twitter and instagram

2. Twitter has 3 tweets 

3. Clicking view hidden replies on second tweet we see one from Andre Ahman telling us to look where the code is hidden - look for Andre github

4. WE find a github for Andre

5. Trenton beat us here.

6. Look at commit history

7. WE find 2 commits that are helpful

    1. A link to the blogspot with structure for password: Blog password will be: pets_name + mothers_maiden_name + favourite_food + favourite_place_in_world + birth_year (all lower case underscore for spaces)

    2. The flag will be first post on blog

8. We find trentons instagram trentonblackwell37

9. His first post has his favourite food in the description: shrimp

10. One person he follows lucky_carps68 has his pets name in a post: Oliver

11. Another follower has his a birthday post which includes his current age. Putting his birthyear as 1967

12. Go to lazerblackwell instagram, look at his followers to find ctblackwell38, her bio confirms this is Trentons mom and her maiden name is Olson

13. Finally a post Trenton has been tagged in j_robles is about vacation, Trenton commented saying the vacation was his favourite place in world which is described by the 3 words: modern older lure

14. Go to what 3 words and use those 3 words to find the location lisbon.

15. We have all pieces of the password which is: oliver_olson_shrimp_lisbon_1967

16. enter the blog and scroll down to find the first post and the flag

## Solution (Part 2)

1. We want to find a date important to Trenton. We see the blog has ~100 posts all with a QR code. Each QR code gives us a sort of flag but we cant really tell which is the right one. The correct one would be behind the date we find. 

2. Going back to his instagram we find his most recent post with the caption: "Roughly 34 years later, coming back to where i had the best day of my life. The day i met my wife. "The first symtom of true love in a man is timidity, in a young woman, boldness"

3. There is a clue in the picture if we zoom in a bit, we see a sign reading "Being Human 29 April - 22 May.

4. Googlin that we get a rough location of Melbourne

5. Doing math 34 years ago put us in 1989

6. Googling the quote we find this is from Les Misérables.

7. Doing a google search for Melbourne + Les Misérables + 1989 + day we find December 9th 1989. 

8. We go to the blog post on that day, scan the QR code and that is the flag we want. 

## Flag (Part 1)

magpie{3arLy_m@6p13_6e75_th3_W0rM}

## Flag (Part 2)

magpie{pL@n3t_d00M_F0UnD_022}


