import subprocess
from os import devnull
from time import sleep

#second part of the flag
flag = "p0w32_70_54v3_7h3_w021d}"
textToSend = [
    "Hello there!", "If you are reading this", "it means you have already broken", "into my server. Omniflags got to me",
    "before I could finish the implementation.", "Thus why you could probably break into it.", "I suddenly remembered this website", "I was working on",
    "and thought it would", "be a good way to send a message.", "Who knows maybe",
    "someone actually finds", "a way to break into my server.",
    "I'm sending this message", "as a secret just in case they",
    "find out what I have done.", "The flag is the only way", "to get me out of here", "so I really hope you find it.", "It's hidden in these messages",
    "but I can't tell you where", "in case they read these messages.", "You're close! Keep looking!",
    "Best of luck!",
]


def main():
    while (True):
        i = 0
        for c in flag:
            subprocess.call(["nping --icmp --icmp-id " + str(ord(c))+" --data-string \"" + textToSend[i]+"\" -c 1 172.16.238.10"],
                            shell=True, stdout=None)
            i += 1
        sleep(20)


if __name__ == '__main__':
    main()
