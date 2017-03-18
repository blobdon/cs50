import crypt, sys
from itertools import product


def main():
    
    # check correct usage
    if len(sys.argv) != 2:
        print("Usage: crack.py hash")
        exit(1)
    
    hash = sys.argv[1]
    letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

    for n in range(1,5):
        for pppp in product(letters, repeat=n): #stackoverflow discovery
            pwd = "".join(pppp)
            if crypt.crypt(pwd, hash) == hash:
                print(pwd)
                exit()
    
if __name__ == "__main__":
    main()