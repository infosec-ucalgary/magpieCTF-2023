#!usr/bin/python3
import string
import re

#message depending on the storyline
m="ucflag{FLAG}"
# Can be other key based on the storyline
key="TIME"

ext=string.ascii_letters+string.digits+"{}"
# Taking out the characters in key
for chr in key:
    ext=ext.replace(chr,"")

# key table is compost of key, lowercase and uppercase alphabets, digits from 0 to 9 and {}
key+=ext

print("key table")
# Print out the key table
count=1
print("-"*17)
for c in key:
    if(count%8==1):
        print("|",end="")
    print(c+"|",end="")
    if(count%8==0):
        print("\n"+"-"*17)
    count+=1

# break the key down into groups of 8    
key=re.findall('.'*8,key)


def encrypt(key,m):
    #Adding X if two adjacent letters are the same
    count=0
    while(count<len(m)-1):
        if(m[count]==m[count+1]):
            m=m[:count+1]+"X"+m[count+1:]
            count+=1
        count+=1

    #Adding X at the end if the message does not have even amount of letters
    if(len(m)%2!=0):
        m+="X"

    #Breaking the message down into pairs
    m=re.findall('.'*2,m)
    
    #Creating cipher
    c=""
    for p in m:
        p0Index=(-1,-1)
        p1Index=(-1,-1)
        for x in range(len(key)):
            for y in range(len(key[0])):
                if key[x][y]==p[0]:
                    p0Index=(x,y)
                if key[x][y]==p[1]:
                    p1Index=(x,y)
        
        # if they are on the same row, add the character on its right to cipher
        if p0Index[0]==p1Index[0]:
            if(p0Index[1]!=len(key[0])-1):
                c+=key[p0Index[0]][p0Index[1]+1]
            else:
                c+=key[p0Index[0]][0]

            if(p1Index[1]!=len(key[0])-1):
                c+=key[p1Index[0]][p1Index[1]+1]
            else:
                c+=key[p1Index[0]][0]

        # if they are on the same column, add the character below to cipher
        elif p0Index[1]==p1Index[1]:
            if(p0Index[0]!=len(key)-1):
                c+=key[p0Index[0]+1][p0Index[1]]
            else:
                c+=key[0][p0Index[1]]

            if(p1Index[0]!=len(key)-1):
                c+=key[p1Index[0]+1][p1Index[1]]
            else:
                c+=key[0][p1Index[1]]

        # if they are not on the same row or column, make a rectangle and add the character on the opposite side
        else:
            c+=key[p0Index[0]][p1Index[1]]+key[p1Index[0]][p0Index[1]]

    return(c)

print()
print("cipher: "+encrypt(key,m))
