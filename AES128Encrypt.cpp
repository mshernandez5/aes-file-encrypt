// Markus Hernandez
// AES
// 3.22.2020

#include <iostream>
#include <string.h>
#include "AES.h"
#include "KeyInput.h"

/**
 * PKCS5 Padding - Pads the end of the buffer with bytes, each holding a value representing the total number of padding bytes used.
 */
void padBuffer(uint8_t * buffer, int bytesRead)
{
    int numPaddingBytes = 16 - bytesRead;
    for (int byte = bytesRead; byte < 16; byte++)
        buffer[byte] = numPaddingBytes;
}

const char * getCipherFileName(const char * inputFileName)
{
    int fileNameLength = strlen(inputFileName);
    int extensionIndex = -1;
    for (int i = fileNameLength - 1; i >= 0; i--)
    {
        if (inputFileName[i] == '.')
        {
            extensionIndex = i;
            break;
        }
    }
    char * cipherFileName;
    if (extensionIndex == -1)
    {
        cipherFileName = new char[fileNameLength + 5];
        strncpy(cipherFileName, inputFileName, fileNameLength);
        strncat(cipherFileName, ".enc", 5);
    }
    else
    {
        cipherFileName = new char[extensionIndex + 4];
        strncpy(cipherFileName, inputFileName, extensionIndex + 1);
        strncat(cipherFileName, "enc", 4);
    }
    return cipherFileName;
}

int main(int argc, char ** argv)
{
    // Check Arguments
    if (argc < 2)
    {
        std::cout << "No file parameter was specified!" << std::endl;
        return 0;
    }

    // Set Key Size (128)
    int keyByteSize = 16;
    
    // Get Key Bytes From Input
    KeyInput input = KeyInput(keyByteSize);
    uint8_t * keyBytes;
    bool readKey = false;
    while (!readKey)
    {
        try
        {
            keyBytes = input.keyRead();
            readKey = true;
        }
        catch(InvalidKeyException & e)
        {
            std::cout << e.what() << std::endl;
            char retry;
            do
            {
                std::cout << "Enter Again? (Y/N): ";
                retry = std::getchar();
                // Clear Buffer
                char c;
                do
                {
                    c = std::getchar();
                } while (c != '\n' && c != EOF);
                
            } while (retry != 'Y' && retry != 'y' && retry != 'N' && retry != 'n');
            if (retry == 'N' || retry == 'n')
                return 0;
        }
    }

    // Create Instance of AES Using Key Bytes
    AES aes = AES(keyBytes, keyByteSize);

    // Prepare Input/Output Files
    const char * cipherFileName = getCipherFileName(argv[1]);
    FILE * plainFile = fopen(argv[1], "rb");
    FILE * cipherFile = fopen(cipherFileName, "wb");
    delete cipherFileName;
    if (plainFile == NULL)
    {
        std::cout << "The given file could not be opened!" << std::endl;
        return 0;
    }
    if (cipherFile == NULL)
    {
        std::cout << "The cipher file could not be created!" << std::endl;
        return 0;
    }

    // Encrypt The File & Write Output Bytes
    int bytesRead;
    bool addedPadding = true;
    uint8_t fileBuffer[16];
    while ((bytesRead = fread(fileBuffer, sizeof (uint8_t), 16, plainFile)) > 0 || !addedPadding)
    {
        if (bytesRead < 16)
        {
            addedPadding = true;
            padBuffer(fileBuffer, bytesRead);
        }
        aes.encrypt(fileBuffer);
        fwrite(fileBuffer, sizeof (uint8_t), 16, cipherFile);
    }

    // Clean Up
    fclose(plainFile);
    fclose(cipherFile);
    return 0;
}