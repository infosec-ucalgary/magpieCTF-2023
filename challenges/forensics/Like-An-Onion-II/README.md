# Like an Onion II

### Category: Forensics
### Author: Greg Vance (Expergefied)

## Description
One of the prisoners sent us this picture. See what you can find.

## Hints
1. ext4 filesystem
2. Try a different order of steps

## Solution
1. Start by using binwalk -e 5303.jpg to get the zip file out of the image. The zip filesystem
   is password encrypted.
2. Use steghide extract -sf 5303.jpg to extract the .wav file
3. The intercept.wav file is morse code, when decoded it contains the password "etacarinae".
   This can be used to unzip the file extracted using binwalk.
4. Once you have unzipped the file you have a filesystem img file. Mount this and explore.
5. Hidden in jwebb folder are several images. One of the images cosmiccliffs.png is the same image
   as the 5303.jpg image. Using binwalk -e cosmiccliffs.png you will get another password encrypted
   zip file.
6. Referenced and hinted at in the filesystem there is a missing message or deleted file from this
   filesystem.
7. Unmount the station5303.img file
8. use extundelete station5303.img --restore-all (this may fail, in which case you need to delete
   station5303.img and re-extract it using binwalk and the obtained password and then perform this
   step again) this will give the deleted message which contains the password "QnVyZWF1Y3JhY3lDbHViRlRX"
   to unzip the file extracted from cosmiccliffs.png. When this is unzipped using the password,
   you will obtain the flag.

## Flag
magpie{0gr35_h4v3_l4y3r5_t0o}
