#include "bitstream.h"
#include "util.h"

#include <stdio.h>
#include <string.h>

bitstream* bs_init()
{
	bitstream* bs = malloc(sizeof(bitstream));
	bs->size = 0;
	bs->buffersize = 4;
	bs->buffer = calloc(sizeof(byte), bs->buffersize);
	return bs;
}		

void bs_destroy(bitstream* bs) 
{
	free(bs->buffer);
	free(bs);
}

void bs_add_b(bitstream* bs, byte data, size_t numBits)
{
	
	data = reverse_bit_b(data);
	data >>= (sizeof(data)*8 - numBits);
	bs_add_internal(bs, data, numBits);
}

void bs_add_bs(bitstream* bs, byte* data, size_t dataSize, size_t numBits)
{
	if (!data) return;

	while(numBits > 8 && dataSize) {
		bs_add_b(bs, *data, 8);
		data++;
		numBits -=8;
		dataSize--;
	}
	if (dataSize) bs_add_b(bs, *data, numBits);

}

void bs_add_i(bitstream* bs, unsigned int data, size_t numBits) {
	data = reverse_bit_i(data);
	data >>= (sizeof(data)*8 - numBits);
	while (numBits > 8) {
		bs_add_internal(bs, (byte)data, 8);
		data >>= 8;
		numBits -= 8;
	}
	bs_add_internal(bs, (byte)data, numBits);
}

void bs_add_internal(bitstream* bs, byte data, size_t numBits) 
{
	int i;

	if (bs->buffersize * 8 < bs->size + numBits) {
		bs->buffersize++;
		bs->buffer = realloc(bs->buffer, bs->buffersize);
		bs->buffer[bs->buffersize-1] &= 0;
		
	}

	int numBytes = bs->size / 8;
	int trailingbits = bs->size % 8;
	byte mask = 0;
	
	for (i = 0; i < numBits; i++) mask = (mask << 1) | 1;

	if (trailingbits) {
	   	bs->buffer[numBytes] |= ((mask & data) << trailingbits); 
		if (numBits+trailingbits >= 8) bs->buffer[numBytes] = reverse_bit_b(bs->buffer[numBytes]);
		numBytes++;
   	}
	if (numBits - trailingbits > 0) {
		bs->buffer[numBytes] |= (byte)((mask & data) >> (trailingbits ? 8 - trailingbits : 0));
		if (numBits - trailingbits == 8) bs->buffer[numBytes] = reverse_bit_b(bs->buffer[numBytes]);
	}

	bs->size += numBits;

}

byte bs_get(bitstream* bs, size_t index)
{
	if (index * 8 >= bs->size) return 0;
	return bs->buffer[index];
}

void  bs_geta(bitstream* bs, byte* dest, size_t index, size_t numBytes)
{
	if (!dest || !bs || index*8 >= bs->size) return;	

	if (bs->size < (numBytes+index)*8) {numBytes = bs->size/8-index;}
	memcpy(dest, bs->buffer+index, numBytes);
}


void bs_print_full(bitstream* bs) {
	printf("size: %u \t buffersize: %u\t\t", (unsigned int)bs->size, (unsigned int)bs->buffersize);
	int i, j;
	for (i = 0; i < bs->buffersize; i++) {
		printf (" ");
		for (j = 7; j >= 0; j--) {
			if (8*i+7-j == bs->size) printf("(");
			printf("%d", (bs->buffer[i] >> j) & 1);
		}
	}
	if (bs->size%8) printf(")");
	printf("\n");
}
