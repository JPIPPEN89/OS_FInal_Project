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

        for (int i= 0; i < memorySize; i++) {
            memory[i] = -1;
        }
    }
};



void FIFO(int memorySize, Node* currNode) {
    int pageFaults = 0;
    int firstIn = 0;

    Node* prevNode = nullptr;

    // Loop through every reference
    while (currNode != nullptr) {
        bool found = false;

        // Copy memory from the previous node
        if (prevNode != nullptr) {
            for (int i = 0; i < memorySize; i++) {
                currNode->memory[i] = prevNode->memory[i];
            }
        }

        // Check if the reference is already in memory
        for (int i = 0; i < memorySize; i++) {
            if (currNode->memory[i] == currNode->reference) {
                found = true;
                break;
            }
        }

        // If the reference is not found, a page fault occurs
        if (!found) {
            pageFaults++;
            currNode->pageFault = true;

            // Insert the reference into the selected frame
            currNode->memory[firstIn] = currNode->reference;

            // Cycle to the next memory frame
            firstIn = (firstIn + 1) % memorySize;
        }

        // Save the current node
        prevNode = currNode;

        // Move to the next node
        currNode = currNode->next;
    }

    // Display total page faults
    std::cout << "Total Page Faults: " << pageFaults << "\n\n";
}

void displayMemory(int memorySize, Node* headNode) {
    Node* currNode = headNode;

    // Display the reference string
    while (currNode != nullptr) {
        std::cout << std::setw(4) << currNode->reference;
        currNode = currNode->next;
    }

    std::cout << "\n";

    // Reset to the beginning of the linked list
    currNode = headNode;

    // Display the separator
    while (currNode != nullptr) {
        std::cout << "----";
        currNode = currNode->next;
    }

    std::cout << "\n";

    // Loop through each memory frame
    for (int i = 0; i < memorySize; i++) {

        // Start at the beginning of the linked list
        currNode = headNode;

        // Loop through every reference
        while (currNode != nullptr) {

            // Display memory if a page fault occurred
            if (currNode->pageFault &&
                currNode->memory[i] != -1) {

                std::cout << std::setw(4)
                    << currNode->memory[i];
            }

            // Leave the column blank if no page fault occurred
            else {
                std::cout << std::setw(4) << " ";
            }

            // Move to the next node
            currNode = currNode->next;
        }

        std::cout << "\n";
    }
}




int main()

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
{
    std::string fileName;
    std::string value;

    char algorithm;
    int memorySize;

    Node* headNode = nullptr;
    Node* tailNode = nullptr;

    // Ask the user for the file name
    std::cout << "Enter file name: ";
    std::getline(std::cin, fileName);

    // Open the text file
    std::ifstream inputFile(fileName);

    // Check if the file opened successfully
    if (!inputFile.is_open()) {
        std::cout << "Error: Unable to open file.\n";
        return 1;
    }

    // Read the algorithm character
    std::getline(inputFile, value, ',');
    algorithm = value[0];

    // Read the number of memory frames
    std::getline(inputFile, value, ',');
    memorySize = std::stoi(value);

    // Check if the number of frames is valid
    if (memorySize <= 0) {
        std::cout << "Invalid memory size.\n";
        return 1;
    }

    // Read the remaining references
    while (std::getline(inputFile, value, ',')) {

        // Convert the reference to an integer
        int reference = std::stoi(value);

        // Create a new node for the reference
        Node* newNode = new Node(reference, memorySize);

        // If the linked list is empty
        if (headNode == nullptr) {
            headNode = newNode;
            tailNode = newNode;
        }

        // Otherwise, add the node to the end
        else {
            tailNode->next = newNode;
            tailNode = newNode;
        }
    }

    // Close the input file
    inputFile.close();

    // Display the number of memory frames
    std::cout << "\nMemory Frames: " << memorySize << "\n";

    // Run FIFO if the algorithm character is F
    if (algorithm == 'F') {
        std::cout << "Algorithm: FIFO\n\n";

        FIFO(memorySize, headNode);
        displayMemory(memorySize, headNode);
    }

    // Display an error for unsupported algorithms
    else {
        std::cout << "Unsupported algorithm.\n";
    }

    // Start at the beginning of the linked list
    Node* currNode = headNode;

    // Delete dynamically allocated memory
    while (currNode != nullptr) {

        // Save the address of the next node
        Node* nextNode = currNode->next;

        // Delete the memory array
        delete[] currNode->memory;

        // Delete the current node
        delete currNode;

        // Move to the next node
        currNode = nextNode;
    }

    return 0;
}
