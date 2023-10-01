from cs50 import get_int

# TODO
while True:
    height = get_int("Height: ")
    if (height >= 1 and height <= 8):
        break
space = " "
hashs = "#"
for i in range(1, height+1, 1):
    print((space*(height - i)) + (hashs*i) + (space*2) +  (hashs*i))