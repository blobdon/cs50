
def main():
    
    h = -1
    while h < 0 or h > 23:
        print("Height: ", end="")
        try:
            h = int(input())
        except ValueError:
            h = -1
        
    for r in range(1, h + 1):
        print("{}{}  {}".format(" " * (h - r),"#" * r, "#" * r))

if __name__ == "__main__":
    main()
    