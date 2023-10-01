import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Incorrect number of command-line arguments.")

    # TODO: Read database file into a variable
    data = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for i in reader:
            data.append(i)

    # TODO: Read DNA sequence file into a variable
    dna = open(sys.argv[2], "r")
    sequence = dna.read()
    dna.close()

    # TODO: Find longest match of each STR in DNA sequence
    #name of header for str
    STRs = list(data[0].keys())[1:]
    #to count the result from matches
    strcount = {}
    for j in STRs:
        strcount[j] = (longest_match(sequence, j))

    # TODO: Check database for matching profiles
    # matches is counter for match, and if matches == amount of str, means it fits the bill
    for k in data:
        matches = 0
        for l in STRs:
            if int(k[l]) == strcount[l]:
                matches += 1
            if matches == len(strcount):
                print(k["name"])
                return
    else:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
