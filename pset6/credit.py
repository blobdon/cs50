
# luhn conducts Luhn's Algorithm, returns True for valid numbers.
def luhn(cc):
    
    l = 0;
    for n in range(-1, -(len(cc)+1), -1): # loop backwards
        if n % 2 == 0:
            dd = str(2 * int(cc[n]))
            l += sum(int(d) for d in dd)
        else:
            l += int(cc[n])
        
    return True if l % 10 == 0 else False
    

def main():
    
    print("Number: ", end="")
    cc = input()
    # keep asking until non-negative number given
    while not cc.isdigit():
        print("Retry: ", end="")
        cc = input()
    
    carrier = "INVALID"
    if  (len(cc) == 13 or len(cc) == 16) and cc[:1] == "4"      and luhn(cc):
        carrier = "VISA"
    elif len(cc) == 15 and cc[:2] in ("34", "37")               and luhn(cc):
        carrier = "AMEX"
    elif len(cc) == 16 and cc[:2] in ("51","52","53","54","55") and luhn(cc):
        carrier = "MASTERCARD"
        
    print(carrier)
    

if __name__ == "__main__":
    main()