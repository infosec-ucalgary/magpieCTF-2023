#!/usr/bin/python3

import requests
from pwn import *
import random
import sys

# flag prefix
PREFIX = "magpie{"

# valid fake flag characters
VALID_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456790_!@#"

# actual flag (ONE THAT CAN BE SUBMITTED)
REAL_FLAG = "magpie{chOc0LatE_Ch1p_c0Ok1e5}"

# spams post requests <num_of_requests> number of times, and delays each request by <delay_in_ms> milliseconds
def post_spammer(target_url, num_of_requests, position_of_flag, delay_in_ms, min_length, max_length):

    # spams <num_of_requests> requests
    for i in range(num_of_requests):
        # if the specified real flag position has been reached, send the real flag
        if i == position_of_flag - 1:
            data= REAL_FLAG
        
        # otherwise, generate a fake flag
        else:
            random_flag = ''.join(random.choice(VALID_CHARACTERS) for i in range(round(random.random() * (max_length - min_length) + min_length)))
            data= PREFIX + random_flag + "}"

        # print data being sent
        print(data)
        # loading bar
        percentage = round(i / num_of_requests * 100, 2)
        print(f"{percentage}% complete ({i} / {num_of_requests})\t[{'-' * round(percentage / 2)}{' ' * round(50 - percentage / 2)}]", end="\r")

        # send data
        requests.post(target_url, data=data)
        
        # wait <delay_in_ms> amount of milliseconds before the next request
        sleep(delay_in_ms / 1000)

def main(args):
    if len(args) < 7:
        print(f"usage: {args[0]} <target_url> <num_of_requests> <position_of_flag> <delay_in_ms> <min_fake_length> <max_fake_length>")
    else:
        post_spammer(args[1], int(args[2]), int(args[3]), int(args[4]), int(args[5]), int(args[6]))

main(sys.argv)