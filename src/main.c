#include <stdio.h>
#include <stdint.h>

uint8_t mem[256] = {0};
uint8_t rg[4] = {0};
uint8_t flag = 0,pc = 0;


void decode_execute(uint8_t op,uint8_t a,uint8_t b){
	switch (op) {
		case 0x01: rg[a] = mem[b]; break;
		case 0x02: mem[b]= rg[a]; break;
		case 0x03: rg[a] = rg[a] + rg[b]; break;
		case 0x04: rg[a] = rg[a] - rg[b]; break;
		case 0x05: rg[a] = mem[b]; break;
		case 0x06: flag = (rg[a] == rg[b]) ? 1 : 0; break;
		case 0x07: break;
		case 0x08: break;
		case 0x09: break;
		case 0x0A: break;
	}




};

int main(int argc,char *argv[]){

};
