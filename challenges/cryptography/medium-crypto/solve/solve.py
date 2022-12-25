
from Crypto.Util.number import long_to_bytes, inverse
from math import isqrt
import random, time
from pwn import *

FLAG = b'magpie{17_w45n\'7_72u1y_24nd0m_4f732_411}\n'
HOST = "127.0.0.1"
PORT = 20000

def fermatfactor(N):
    if N <= 0:
        return (N, 1)
    if N % 2 == 0:
        return (2, N/2)
    a = isqrt(N) + 1
    while isqrt(a**2-N) ** 2 != (a**2-N):
        a = a + 1
    b = isqrt(a**2-N)
    return (a - b, a + b)


def decrypt(n, e, c):
    p, q = fermatfactor(n)
    phi = (p-1) * (q-1)
    d = inverse(e, phi)
    message = pow(c, d, n)
    return long_to_bytes(message)

def findCurrentShip(prevShips):
    seed = round(time.time())
    ship = 0
    while True:
        random.seed(seed)
        randNums = []
        for i in range(5):
            randNums.append(random.randrange(1000000))
        # print(prevShips)
        # print(randNums)
        if set(randNums) == set(prevShips):
            ship = random.randrange(1000000)
            break
        seed = seed -1 
    return ship
def solve():
    try: 
        conn = remote(HOST,PORT)

        ## welcome message and options
        for i in range(12):
            conn.recvline()    

        # get previous ships 
        conn.sendline(b'1')
        prevShips = []
        for i in range(5):
            prevShips.append(int(conn.recvline().decode()))
        prevShips

        ## options
        for i in range(6):
            conn.recvline()

        # get public certificate
        conn.sendline(b'2')
        n = int(conn.recvline().decode().split("=")[1])
        e = int(conn.recvline().decode().split("=")[1])

        ## options
        for i in range(6):
            conn.recvline()

        # get encrypted password 
        conn.sendline(b'3')
        encryptedPass = conn.recvline()

        # decrpt password 
        password = decrypt(n,e,int(encryptedPass,0))
        # find current ship
        ship = findCurrentShip(prevShips)

        ## options
        for i in range(6):
            conn.recvline()
        
        # unlock ship
        conn.sendline(b'4')
        conn.recvline()
        conn.sendline(str(ship).encode())
        conn.recvline()
        conn.sendline(password)
        conn.recvline()
        flag = conn.recvline()
        conn.close()
        return FLAG == flag
    except:
        print(Exception)
        exit(1)
if __name__ == "__main__":
    print(solve())