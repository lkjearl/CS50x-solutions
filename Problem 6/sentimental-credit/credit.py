from cs50 import get_int
# TODOchecksum(card) == True and
def main():
    while True:
        card = get_int("Number: ")

        if amex(card) == True and checksum(card) == True:
            print("AMEX\n")
        elif master(card) == True and checksum(card) == True:
            print("MASTERCARD\n")
        elif visa(card) == True and checksum(card) == True:
            print("VISA\n")
        else:
            print("INVALID\n")
        break

def checksum(card):
    sum = 0
    # temp store number that need to *2
    temp = 0
    for i in range(len(str(card))):
            if i % 2 == 0:
                sum += card % 10
            else:
                temp = 2*(card % 10)
                sum += (temp//10) + (temp%10)
            card //= 10

    if (sum % 10) == 0:
        return True

def amex(card):
    if (len(str(card)) == 15) and (int(str(card)[:2]) == 34 or int(str(card)[:2]) == 37):
        return True

def master(card):
    if (len(str(card)) == 16) and (int(str(card)[:2]) >=51 and int(str(card)[:2]) <=55):
        return True

def visa(card):
    if (len(str(card)) == 13 or len(str(card)) == 16) and (int(str(card)[:1]) == 4):
        return True


main()