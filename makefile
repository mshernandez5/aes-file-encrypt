TARGET = AES128Encrypt
CXX = g++

$(TARGET): AES128Encrypt.o AES.o KeyInput.o
	$(CXX) -o $(TARGET) AES128Encrypt.o AES.o KeyInput.o

.PHONY: clean
clean:
	$(RM) $(TARGET) AES128Encrypt.o AES.o KeyInput.o
