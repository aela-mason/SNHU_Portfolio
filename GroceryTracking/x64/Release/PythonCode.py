import re
import string

# global dictionary to allow us to read in as a separate function
grocery_items = {}

# Function to read in the file
def ReadFile():   
    global grocery_items

    # Open the file we need to read in
    f = open("CS210_Project_Three_Input_File.txt")

    # If it failed to open
    if f.closed:
        # Print that and return
        print('Couldn\'t open file')
        return

    # For each line in the file
    for line in f:
        # Remove the newline
        item = line[:-1]

        # If it's already in our dictionary
        if item in grocery_items:
            # Just increment the number sold
            grocery_items[item] += 1
        else:
            # Otherwise add it to the dictionary with one sold
            grocery_items[item] = 1

    # Close the file
    f.close()


# Function to count the frequencies of all items sold
def CountAll():
    global grocery_items

    # Get the content of the file into grocery_items
    ReadFile()

    # For each item in the dictionary
    for item in grocery_items:
        # Print it, as well as the number of times it was counted
        print('{}: {}'.format(item, grocery_items[item]))

# Function to count the frequencies of one specific item sold
def CountOne(item):
    global grocery_items

    # Get the content of the file into grocery_items
    ReadFile()
    # Loop through the items in the dictionary
    for grocery in grocery_items:
        # If we find our item, print it and its frequency
        if item.lower() == grocery.lower():
            # Deprecated to align with documentation
            #print('{}: {}'.format(grocery, grocery_items[grocery]))
            # Since we found it, we're done
            return grocery_items[grocery]
    else:
        # Otherwise, print that it wasn't sold
        print('Item not sold')
        return -1;

# Function to create a histogram of all sales
def MakeHistogram():
    global grocery_items

    # Get the content of the file into grocery_items
    ReadFile()

    # Variable to count the longest item name
    longest = 0
    # Loop through the dictionary and find the longest item name
    for item in grocery_items:
        if len(item) > longest:
            longest = len(item)

    # Make a file to write frequencies to
    f = open('frequency.dat', 'w')
    # If the file isn't open
    if f.closed:
        # Print that we failed to open it and return
        print('File failed to open')
        return

    # Write the total number of items and the length of the longest name to the file
    # This helps immensely with file input and formatting back in c++
    f.write('{} {}\n'.format(len(grocery_items), longest))
    # Write all of our items and their frequencies to the file
    for item in grocery_items:
        f.write('{} {}\n'.format(item, grocery_items[item]))

    # Close the file
    f.close()

    # Code below was to have Python print the histogram
    # Deprecated to better align with documentation

    # For each item in the dictionary
    #for item in grocery_items:
        # Print the item name, right justified and set to line up with the longest name
    #    print("{num:>{width}}:".format(num=item, width=longest), end=' ')

        # Print one asterisk for each time the item was counted
    #    for i in range(width):
    #        print('*', end='')
    #    print()
