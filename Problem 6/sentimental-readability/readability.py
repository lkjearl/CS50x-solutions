from cs50 import get_string
# TODO
def main():
    text = get_string("Text: ")
    grade = index(text)
    if (grade < 1):
        print("Before Grade 1")
    elif (grade >= 16):
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def letters(text):
    letter = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letter += 1
    return letter

def words(text):
    letter = text.split()
    word = len(letter)
    return word

def sentences(text):
    sentence = 0
    for i in range(len(text)):
        if text[i] == "." or text[i] == "?" or text[i] == "!":
            sentence += 1
    return sentence

def index(text):
    grade = round(0.0588 * (letters(text)/words(text))*100 - 0.296 * (sentences(text)/words(text))*100 - 15.8)
    return grade

main()