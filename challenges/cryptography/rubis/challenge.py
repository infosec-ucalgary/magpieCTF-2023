
import random, time
from Crypto.Util.number import bytes_to_long, long_to_bytes, inverse, GCD, getRandomNBitInteger
from sympy import nextprime, prevprime

WELCOME_SCREEN = """
************************************************************
*                                                          *
*                                                          * 
*                     WELCOME TO RUBIS                     *
*                                                          * 
*                                                          *
************************************************************

ATTENTION ALL EMPLOYEES OF OMNI, Rubis is no longer in use,
and thus it should not be used to store important information. 
"""
FLAG = b''
PASSWORD = b''


class RSA():
    def __init__(self, key_length):
        self.e = 0x10001
        phi = 0
        prime_length = key_length // 2
        while GCD(self.e, phi) != 1:
            x = getRandomNBitInteger(prime_length)
            self.p = nextprime(x)
            self.q = prevprime(x)
            phi = (self.p - 1) * (self.q - 1)
            self.n = self.p * self.q
        self.d = inverse(self.e, phi)

    def encrypt(self, message):
        message = bytes_to_long(message)
        return pow(message, self.e, self.n)

    def decrypt(self, encrypted_message):
        message = pow(encrypted_message, self.d, self.n)
        return long_to_bytes(message)


class Ship():
    def __init__(self, password, data):
        self.password = password
        self.data = data


class Cluster():
    def __init__(self, rsa, password, flag):
        self.rsa = rsa
        self.ships = {}
        for i in range(1000000):
            self.ships[i] = Ship(b"pa$$w0rd", b"Nothing to see here :( Better luck next time!")
        self.prevShips = []
        self.currentShip = 0
        random.seed(round(time.time()))
        self.masterPassword = password
        self.setCurrentShip()
        self.populateSpecialShip(password, flag)

    def setCurrentShip(self):
        for i in range(5):
            self.prevShips.append(random.randrange(1000000))
        self.currentShip = random.randrange(1000000)

    def populateSpecialShip(self, password, flag):
        self.ships[self.currentShip].password = password
        self.ships[self.currentShip].data = flag

    def getPrevShips(self):
        return self.prevShips

    def getPasswordHint(self):
        return hex(self.rsa.encrypt(self.masterPassword))

    def unlockShip(self, ship, password):
        if (ship < 0 or ship > 1000000):
            print(f"No ship exists with id: {ship}")
            exit(1)
        elif (self.ships[ship].password == password):
            print(
                f"You've managed to unlock ship {ship}! Here's what's inside: \n{self.ships[ship].data.decode()}")
        else:
            print("Err... couldn't open the ship! Maybe next time...\n")
            exit(1)


if __name__ == "__main__":
    rsa = RSA(2048)
    cluster = Cluster(rsa, PASSWORD, FLAG)
    print(WELCOME_SCREEN)
    while True:
        try:
            print("What would you like to do?\n\t1. Get previous ships\n\t2. Get public certificate\n\t3. Get password hint\n\t4. Unlock a ship")
            option = int(input("input: \n"))
            if option == 1:
                for ship in cluster.getPrevShips():
                    print(ship)
            elif option == 2:
                print(f"n={cluster.rsa.n}\ne={cluster.rsa.e}")
            elif option == 3:
                print(cluster.getPasswordHint())
            elif option == 4:
                ship = int(input("Which ship would you like to unlock? \n"))
                password = input(f"Enter password for ship {ship}: \n")
                cluster.unlockShip(ship, password.encode())
            else:
                print("Quitting...")
                exit(1)
        except Exception as e:
            print(f"An error occurred while processing data: {e}")
            exit(1)
