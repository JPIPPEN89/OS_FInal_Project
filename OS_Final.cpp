// OS_Final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>

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
    Node* headNode = currNode;


    while (currNode != nullptr) {
        for (int i = 0; i < memorySize; i++) {
            if (currNode->memory[i] == currNode->reference) {
                pageFaults++;
                break;
            }

            else if ()
        }
    }

    

}

int main()
{
    std::cout << "Hello World!\n";


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
