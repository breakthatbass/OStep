### Chapter 45
# Data Integrity

1. Write a short C program (called `check-xor.c`) that computes and XOR-based checksum over an input file, and prints the checksum as output. Use a 8-bit unsigned char to store the (one byte) checksum. Make some test files to make sure it works as expected.

    - [`check-xor.c`](https://github.com/breakthatbass/OStep/blob/main/chap45/check-xor.c)

2. Now write a short C program (called `check-fletcher.c`) that computes the Fletcher checksum over an input file. Once again, test your program to see if it works.

    - [`check-fletcher.c`](https://github.com/breakthatbass/OStep/blob/main/chap45/check-fletcher.c)

3. Now compare the performance of both: is one faster than the other? How does performance change as the size of the file changes? Use internal calls to `gettimeofday()` to time the programs. Which should you use if you care about performance? About checking ability?