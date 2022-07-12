#!usr/bin/python3
import string
import re

# Can be other cipher depending on the flag
c="ATgeMi6KKBL7"
# Can be other key based on the storyline
key="TIME"

ext=string.ascii_letters+string.digits+"{}"

# Taking out the characters in key
for chr in key:
    ext=ext.replace(chr,"")

# key table is compost of key, alphabets, digits from 0 to 9 and {}
key+=ext

# Print out the key table
count=1
print("-"*17)
for chr in key:
    if(count%8==1):
        print("|",end="")
    print(chr+"|",end="")
    if(count%8==0):
        print("\n"+"-"*17)
    count+=1

# break the key down into groups of 6    
key=re.findall('.'*8,key)

#solution
def decrypt(key,c):
    m=""
    # break the cipher down to groups of 2
    c=re.findall('.'*2,c)

    for p in c:
        p0Index=(-1,-1)
        p1Index=(-1,-1)
        for x in range(len(key)):
            for y in range(len(key[0])):
                if key[x][y]==p[0]:
                    p0Index=(x,y)
                elif key[x][y]==p[1]:
                    p1Index=[x,y]
        # if they are on the same row, add the character on its left to message
        if p0Index[0]==p1Index[0]:
            if(p0Index[1]!=0):
                m+=key[p0Index[0]][p0Index[1]-1]
            else:
                m+=key[p0Index[0]][-1]
            if(p1Index[1]!=0):
                m+=key[p1Index[0]][p1Index[1]-1]
            else:
                m+=key[p1Index[0]][-1]

        # if they are on the same column, add the character above to message
        elif p0Index[1]==p1Index[1]:
            if(p0Index[0]!=0):
                m+=key[p0Index[0]-1][p0Index[1]]
            else:
                m+=key[-1][p0Index[1]]

            if(p1Index[0]!=0):
                m+=key[p1Index[0]-1][p1Index[1]]
            else:
                m+=key[-1][p1Index[1]]

        # if they are not on the same row or column, make a rectangle and add the character on the opposite side
        else:
            m+=key[p0Index[0]][p1Index[1]]+key[p1Index[0]][p0Index[1]]
    
    m=m.replace("X","")
    return m

print("message: "+decrypt(key,c))
