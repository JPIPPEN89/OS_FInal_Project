// OS_Final.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iomanip>

// Doing OPT and FIFO, make sure OPT.txt and FIFO.txt are both in the project folder.
// 
//Doing linked list so that we can run through references and make the vertical output easier
struct Node {
    int reference; // reference from txt file
    int* memory;     //array that will hold the nodes references stored in memory
    bool pageFault; // this will tell us whether the row should remain blank when outputting
    Node* next;     // points to the next node in the linked list

    Node(int ref, int memorySize) {
        reference = ref;
        memory = new int[memorySize];
        pageFault = false;
        next = nullptr;

        for (int i = 0; i < memorySize; i++) {
            memory[i] = -1;
        }
    }
};




// -FIFO-
// FIFO replaces the page that has been in memory the longest.
// firstIn keeps track of the next frame to replace, allowing
// us to implement FIFO without using a queue.

void FIFO(int memorySize, Node* currNode) {
    int pageFaults = 0;
    int firstIn = 0;

    Node* prevNode = nullptr;

    while (currNode != nullptr) {
        bool found = false;

        // Carry over the memory contents from the previous reference
        // so that memory does not reset with each new node.
        if (prevNode != nullptr) {
            for (int i = 0; i < memorySize; i++) {
                currNode->memory[i] = prevNode->memory[i];
            }
        }

        // Search memory to determine if the requested page
        // is already loaded.
        for (int i = 0; i < memorySize; i++) {
            if (currNode->memory[i] == currNode->reference) {
                found = true;
                break;
            }
        }

        // If the page is not found, count a page fault and
        // insert the requested page into the next FIFO frame.
        if (!found) {
            pageFaults++;
            currNode->pageFault = true;

            currNode->memory[firstIn] = currNode->reference;

            // Modulo returns firstIn to 0 after reaching
            // the end of the memory array.
            firstIn = (firstIn + 1) % memorySize;
        }

        // Save the current memory state and move to the next reference.
        prevNode = currNode;
        currNode = currNode->next;
    }

    std::cout << "Total Page Faults: " << pageFaults << "\n\n";
}


//  -DISPLAY MEMORY-
// Displays the memory states stored in the linked list.
// References are displayed horizontally and memory frames vertically.
// If no page fault occurred, the corresponding column is left blank.

void displayMemory(int memorySize, Node* headNode) {
    Node* currNode = headNode;

    // Display the reference string using setw to align the columns.
    while (currNode != nullptr) {
        std::cout << std::setw(4) << currNode->reference;
        currNode = currNode->next;
    }

    std::cout << "\n";

    // Return to the beginning of the list to display the separator.
    currNode = headNode;

    while (currNode != nullptr) {
        std::cout << "----";
        currNode = currNode->next;
    }

    std::cout << "\n";

    // The outer loop handles each memory frame, while the inner
    // loop displays that frame's contents across all references.
    for (int i = 0; i < memorySize; i++) {

        currNode = headNode;

        while (currNode != nullptr) {

            // Only display occupied frames when a page fault occurs.
            if (currNode->pageFault &&
                currNode->memory[i] != -1) {

                std::cout << std::setw(4)
                    << currNode->memory[i];
            }

            else {
                std::cout << std::setw(4) << " ";
            }

            currNode = currNode->next;
        }

        std::cout << "\n";
    }
}


//  -MAIN-
// Reads the input file, builds the linked list, and determines
// which page replacement algorithm to run. After displaying
// the results, all dynamically allocated memory is released.

int main()
{
    std::string fileName;
    std::string value;

    char algorithm;
    int memorySize;

    // headNode keeps track of the beginning of the list,
    // while tailNode makes adding new nodes easier.
    Node* headNode = nullptr;
    Node* tailNode = nullptr;

    std::cout << "Enter file name: ";
    std::getline(std::cin, fileName);

    std::ifstream inputFile(fileName);

    // Make sure the file can be opened before reading it.
    if (!inputFile.is_open()) {
        std::cout << "Error: Unable to open file.\n";
        return 1;
    }

    // Read the comma-separated values from the file.
    // The first value is the algorithm and the second is the frame count.
    std::getline(inputFile, value, ',');
    algorithm = value[0];

    std::getline(inputFile, value, ',');
    memorySize = std::stoi(value);

    if (memorySize <= 0) {
        std::cout << "Invalid memory size.\n";
        return 1;
    }

    // Create a node for each remaining reference in the file.
    // Nodes are added to the end of the list to preserve their order.
    while (std::getline(inputFile, value, ',')) {

        int reference = std::stoi(value);

        Node* newNode = new Node(reference, memorySize);

        // If this is the first node, it becomes both the head and tail.
        if (headNode == nullptr) {
            headNode = newNode;
            tailNode = newNode;
        }

        else {
            tailNode->next = newNode;
            tailNode = newNode;
        }
    }

    inputFile.close();

    std::cout << "\nMemory Frames: " << memorySize << "\n";

    // Run the algorithm specified by the first character in the file.
    if (algorithm == 'F') {
        std::cout << "Algorithm: FIFO\n\n";

        FIFO(memorySize, headNode);
        displayMemory(memorySize, headNode);
    }

    // OPT will be added here later.

    else {
        std::cout << "Unsupported algorithm.\n";
    }

    // Delete each node and its memory array to prevent memory leaks.
    Node* currNode = headNode;

    while (currNode != nullptr) {

        // Save the next node before deleting the current one
        // so that we do not lose access to the rest of the list.
        Node* nextNode = currNode->next;

        delete[] currNode->memory;
        delete currNode;

        currNode = nextNode;
    }

    return 0;
}
