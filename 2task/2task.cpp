#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

using namespace std;

class Stack
{
private:
    struct Node
    {
        int data;
        Node* next;
    };

    HANDLE heap; // дескриптор власної купи
    Node* top;   // вершина стеку

public:
    Stack()
    {
        heap = HeapCreate(0, 0, 0); // створення нестандартної купи
        top = nullptr;

        if (heap == NULL)
        {
            cout << "Heap creation error!" << endl;
        }
    }

    ~Stack()
    {
        while (!IsEmpty())
        {
            Pop();
        }

        if (heap != NULL)
        {
            HeapDestroy(heap); // знищення купи
        }
    }

    bool IsEmpty()
    {
        return top == nullptr;
    }

    void Push(int value)
    {
        if (heap == NULL)
            return;

        Node* newNode = (Node*)HeapAlloc(heap, 0, sizeof(Node));

        if (newNode == NULL)
        {
            cout << "Memory allocation error!" << endl;
            return;
        }

        newNode->data = value;
        newNode->next = top;
        top = newNode;

        cout << "Added to stack: " << value << endl;
    }

    bool Pop()
    {
        if (IsEmpty())
        {
            cout << "Stack is empty!" << endl;
            return false;
        }

        Node* temp = top;
        cout << "Removed from stack: " << temp->data << endl;

        top = top->next;

        HeapFree(heap, 0, temp);

        return true;
    }

    void Print()
    {
        if (IsEmpty())
        {
            cout << "Stack is empty!" << endl;
            return;
        }

        Node* current = top;

        cout << "Stack: ";

        while (current != nullptr)
        {
            cout << current->data << " ";
            current = current->next;
        }

        cout << endl;
    }
};

int main()
{
    Stack stack;

    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    stack.Print();

    stack.Pop();
    stack.Print();

    stack.Push(40);
    stack.Print();

    stack.Pop();
    stack.Pop();
    stack.Pop();
    stack.Pop();

    return 0;
}