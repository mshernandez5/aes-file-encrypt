#include "OperationMode.h"

void OperationMode::padBuffer(uint8_t * buffer, const int & bytesRead, const int & blockSize)
{
    int paddingByte = blockSize - bytesRead;
    for (int byte = bytesRead; byte < blockSize; ++byte)
    {
        buffer[byte] = paddingByte;
    }
}