#include <stdint.h>
#include <stdio.h>

uint8_t mem[255] = {0};
uint8_t rg[4] = {0};
uint8_t flag = 0,pc = 0, running = 1,ciclo = 0;


void fetch(uint8_t *op,uint8_t *a,uint8_t *b){
	*op = mem[pc]; *a = mem[pc+1];*b = mem[pc+2];
	pc+=3;
}

void decode_execute(uint8_t op,uint8_t a,uint8_t b){
	switch (op) {
		case 0x01: rg[a] = mem[b]; break;
		case 0x02: mem[b]= rg[a]; break;
		case 0x03: rg[a] = rg[a] + rg[b]; break;
		case 0x04: rg[a] = rg[a] - rg[b]; break;
		case 0x05: rg[a] = mem[b]; break;
		case 0x06: flag = (rg[a] == rg[b]) ? 1 : 0; break;
		case 0x07: pc = a; break;
		case 0x08: if(flag) pc = a; break;
		case 0x09: if(!flag) pc = a; break;
		case 0x0A: running = 0; break;
	}
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
	const char *nomes[] = {"","LOAD","STORE","ADD",
		"SUB","MOV","CMP","JMP","JZ","JNZ","HALT"};

	printf("Ciclo %d: %-5s %d,%d | R0=%3d R1=%3d"
	" R2=%3d R3=%3d | PC=%3d ZF=%d\n",
	ciclo, nomes[op], a, b,rg[0], rg[1], rg[2], rg[3], pc, flag);

}

void linearSearch(void){
	mem[0x00] = 0x05; 



}

int main(int argc,char *argv[]){

	while (running && pc < 255) {
		uint8_t op,a,b;
		ciclo++;
		fetch(&op,&a,&b);
		decode_execute(op,a,b);
		trace(op,a,b);
	}


};
