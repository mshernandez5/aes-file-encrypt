#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "AES.h"
#include "ECBMode.h"
#include "Utilities.h"
#include "BlockCipher.h"
#include "OperationMode.h"
#include "InvalidArgumentException.h"
#include "KeyInput.h"

int main(int argc, char * argv[])
{
    // Create std::string Objects For c_string Arguments
    std::vector<std::string> arguments(argv, argv + argc);

    // Program Options
    bool encryptMode;
    std::string inputFilename;
    std::string outputFilename;
    std::string textKey;
    std::string modeOfOperation = "ecb";
    int keyByteSize = 16;

    // Set Options Based On Arguments
    try
    {
        for (std::vector<std::string>::size_type i = 1; i < arguments.size(); ++i)
        {
            std::string & option = arguments[i];
            // Encrypt/Decrypt
            if (i == 1)
            {
                if (equalsIgnoreCase(option.substr(0, 3), "enc"))
                {
                    encryptMode = true;
                }
                else if (equalsIgnoreCase(option.substr(0, 3), "dec"))
                {
                    encryptMode = false;
                }
                else
                {
                    throw InvalidArgumentException(option);
                }
            }
            // ASCII Hex Key
            else if (equalsIgnoreCase(option, "-k"))
            {
                if (i + 1 >= arguments.size())
                {
                    throw InvalidArgumentException(option);
                }
                textKey = arguments[++i];
            }
            // Mode Of Operation (Currently Only ECB Supported)
            else if (equalsIgnoreCase(option, "-m"))
            {
                if (i + 1 >= arguments.size())
                {
                    throw InvalidArgumentException(option);
                }
                modeOfOperation = arguments[++i];
            }
            // Key Size (Currently Only 128 Supported)
            else if (equalsIgnoreCase(option, "-s"))
            {
                if (i + 1 >= arguments.size())
                {
                    throw InvalidArgumentException(option);
                }
                int keySize = std::stoi(arguments[++i]);
                if (keySize != 128 && keySize != 192 && keySize != 256)
                {
                    throw InvalidArgumentException(arguments[i]);
                }
                keyByteSize = keySize / 8;
            }
            // Input/Output Files
            else
            {
                if (inputFilename.empty())
                {
                    inputFilename = option;
                }
                else if (outputFilename.empty())
                {
                    outputFilename = option;
                }
                else
                {
                    throw InvalidArgumentException(option);
                }
            }
        }
        // Check For Required Options
        if (inputFilename.empty())
        {
            throw std::runtime_error("Input file not specified!");
        }
        if (outputFilename.empty())
        {
            throw std::runtime_error("Output file not specified!");
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    
    // Extract Key Bytes, Prompt For Key If Necessary
    uint8_t * keyBytes;
    KeyInput keyInput(keyByteSize);
    try
    {
        if (!textKey.empty())
        {
            keyBytes = keyInput.keyRead(textKey);
        }   
        else
        {
            keyBytes = keyInput.keyRead();
        }
    }
    catch(const std::exception & e)
    {
        textKey.clear();
        std::cerr << e.what() << '\n';
        exit(1);
    }
    
    // Get File Streams
    std::ifstream inputFileStream(inputFilename, std::ios::binary);
    std::ofstream outputFileStream(outputFilename, std::ios::binary);
    if (!inputFileStream)
    {
        std::cerr << "Failed To Open File: " << inputFilename << '\n';
        exit(1);
    }
    if (!outputFileStream)
    {
        std::cerr << "Failed To Open File: " << outputFilename << '\n';
        exit(1);
    }

    // Encrypt / Decrypt
    BlockCipher * algorithm = new AES(keyBytes, keyByteSize);
    OperationMode * mode = new ECBMode();
    if (encryptMode)
    {
        mode->encrypt(inputFileStream, outputFileStream, *algorithm);
    }
    else
    {
        mode->decrypt(inputFileStream, outputFileStream, *algorithm);
    }

    // Clean Up
    inputFileStream.close();
    outputFileStream.close();
}