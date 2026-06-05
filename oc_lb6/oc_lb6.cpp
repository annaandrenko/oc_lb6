#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <limits>
using namespace std;

const int PASSWORD_SIZE = 100;

// Простий хеш, щоб не підключати MD5
unsigned int SimpleHash(const char* password)
{
    unsigned int hash = 0;

    for (int i = 0; password[i] != '\0'; i++)
    {
        hash = hash * 31 + password[i];
    }

    return hash;
}

// Очищення пам'яті
void ClearMemory(char* memory, int size)
{
    if (memory != nullptr)
    {
        SecureZeroMemory(memory, size);
    }
}

// Перевірка хорошого пароля
bool IsGoodPassword(const char* password)
{
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;

    int length = 0;

    for (int i = 0; password[i] != '\0'; i++)
    {
        length++;

        if (password[i] >= 'A' && password[i] <= 'Z')
            hasUpper = true;

        if (password[i] >= 'a' && password[i] <= 'z')
            hasLower = true;

        if (password[i] >= '0' && password[i] <= '9')
            hasDigit = true;
    }

    return length >= 8 && hasUpper && hasLower && hasDigit;
}

// Встановлення пароля
unsigned int SetPassword()
{
    char* password1 = (char*)VirtualAlloc(
        NULL,
        PASSWORD_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    char* password2 = (char*)VirtualAlloc(
        NULL,
        PASSWORD_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (password1 == NULL || password2 == NULL)
    {
        cout << "Memory allocation error!" << endl;
        return 0;
    }

    VirtualLock(password1, PASSWORD_SIZE);
    VirtualLock(password2, PASSWORD_SIZE);

    cout << "Enter password: ";
    cin.getline(password1, PASSWORD_SIZE);

    cout << "Repeat password: ";
    cin.getline(password2, PASSWORD_SIZE);

    if (strcmp(password1, password2) != 0)
    {
        cout << "Passwords do not match!" << endl;

        ClearMemory(password1, PASSWORD_SIZE);
        ClearMemory(password2, PASSWORD_SIZE);

        VirtualUnlock(password1, PASSWORD_SIZE);
        VirtualUnlock(password2, PASSWORD_SIZE);

        VirtualFree(password1, 0, MEM_RELEASE);
        VirtualFree(password2, 0, MEM_RELEASE);

        return 0;
    }

    if (!IsGoodPassword(password1))
    {
        cout << "Password is not strong enough!" << endl;

        ClearMemory(password1, PASSWORD_SIZE);
        ClearMemory(password2, PASSWORD_SIZE);

        VirtualUnlock(password1, PASSWORD_SIZE);
        VirtualUnlock(password2, PASSWORD_SIZE);

        VirtualFree(password1, 0, MEM_RELEASE);
        VirtualFree(password2, 0, MEM_RELEASE);

        return 0;
    }

    unsigned int hash = SimpleHash(password1);

    ClearMemory(password1, PASSWORD_SIZE);
    ClearMemory(password2, PASSWORD_SIZE);

    VirtualUnlock(password1, PASSWORD_SIZE);
    VirtualUnlock(password2, PASSWORD_SIZE);

    VirtualFree(password1, 0, MEM_RELEASE);
    VirtualFree(password2, 0, MEM_RELEASE);

    cout << "Password has been set successfully." << endl;

    return hash;
}

// Перевірка пароля
bool CheckPassword(unsigned int savedHash)
{
    char* password = (char*)VirtualAlloc(
        NULL,
        PASSWORD_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (password == NULL)
    {
        cout << "Memory allocation error!" << endl;
        return false;
    }

    VirtualLock(password, PASSWORD_SIZE);

    cout << "Enter password for checking: ";
    cin.getline(password, PASSWORD_SIZE);
    unsigned int currentHash = SimpleHash(password);

    ClearMemory(password, PASSWORD_SIZE);

    VirtualUnlock(password, PASSWORD_SIZE);
    VirtualFree(password, 0, MEM_RELEASE);

    return currentHash == savedHash;
}

int main()
{
    unsigned int passwordHash = SetPassword();

    if (passwordHash == 0)
    {
        cout << "Password was not saved." << endl;
        return 0;
    }

    if (CheckPassword(passwordHash))
    {
        cout << "Password is correct." << endl;
    }
    else
    {
        cout << "Password is incorrect." << endl;
    }

    return 0;
}