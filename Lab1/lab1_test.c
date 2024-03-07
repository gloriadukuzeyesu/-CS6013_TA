#include <stdio.h>

void bubblesort(unsigned char bytes[]) {
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 - 1 - i; j++) {
            if(bytes[j] > bytes[j + 1]) {
                unsigned char temp = bytes[j];
                bytes[j] = bytes[j + 1]; 
                bytes[j + 1] = temp; 
            }
        }
    }
}

int main() {
    unsigned long arg = 0x0403deadbeef0201; 
    unsigned char bytes[8]; 
    
    bytes[0] = (arg >> 56) & 0xFF;
    bytes[1] = (arg >> 48) & 0xFF;
    bytes[2] = (arg >> 40) & 0xFF;
    bytes[3] = (arg >> 32) & 0xFF;
    bytes[4] = (arg >> 24) & 0xFF;
    bytes[5] = (arg >> 16) & 0xFF;
    bytes[6] = (arg >> 8) & 0xFF;
    bytes[7] = arg & 0xFF;

       printf("Extracted bytes:\n");
    for (int i = 0; i < 8; i++) {
        printf("Byte %d: 0x%02x\n", i + 1, bytes[i]);
    }
    bubblesort(bytes); 
    printf("Sorted bytes:\n\n");
    for (int i = 0; i < 8; i++) {
        printf("Byte %d: 0x%02x\n", i + 1, bytes[i]);
    }

    unsigned long sorted = 0; 
    for (int i = 7; i >= 0; i --) {
        sorted = (sorted << 8) | bytes[i]; 
    }

    printf("sorted unsigned long integer:\n\n");
    printf("0x%02lx\n", sorted);
    //0xefdebead04030201

}

