#include <stdint.h>
#include <stdio.h>

uint8_t mem[256] = {0};
uint8_t rg[4] = {0};
uint8_t flag = 0,pc = 0, running = 1,ciclo = 0;

#define LOAD 0x01
#define STORE 0x02
#define ADD 0x03
#define SUB 0x04
#define MOV 0x05
#define CMP 0x06
#define JMP 0x07
#define JZ 0x08
#define JNZ 0x09
#define HALT 0x0A

#define R0 0x00
#define R1 0x01   
#define R2 0x02
#define R3 0x03

void fetch(uint8_t *op,uint8_t *a,uint8_t *b){
	*op = mem[pc]; *a = mem[pc+1];*b = mem[pc+2];
	pc+=3;
}

void decode_execute(uint8_t op,uint8_t a,uint8_t b){
	switch (op) {
		case 0x01: rg[a] = mem[b]; break;
		case 0x02: mem[b] = rg[a]; break;
		case 0x03: rg[a] = rg[a] + rg[b]; break;
		case 0x04: rg[a] = rg[a] - rg[b]; break;
		case 0x05: rg[a] = b; break;
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

    uint8_t posicao = 0x00;

    //setup1
    mem[posicao + 0]  = MOV; mem[posicao + 1]  = R0; mem[posicao + 2]  = 0x01;
    mem[posicao + 3]  = MOV; mem[posicao + 4]  = R1; mem[posicao + 5]  = 0x10;
    mem[posicao + 6]  = MOV; mem[posicao + 7]  = R2; mem[posicao + 8]  = 0x17;
    mem[posicao + 9]  = MOV; mem[posicao + 10] = R3; mem[posicao + 11] = 0x1A;

    /* salto para loop */
    mem[posicao + 12] = JMP; mem[posicao + 13] = 0x18; mem[posicao + 14] = 0x00;

    //loop1
    mem[posicao + 24] = STORE; mem[posicao + 25] = R1; mem[posicao + 26] = 0x1D;
    mem[posicao + 27] = STORE; mem[posicao + 28] = R3; mem[posicao + 29] = 0x00;

    mem[posicao + 30] = ADD; mem[posicao + 31] = R3; mem[posicao + 32] = R0;
    mem[posicao + 33] = ADD; mem[posicao + 34] = R1; mem[posicao + 35] = R0;

    mem[posicao + 36] = CMP; mem[posicao + 37] = R1; mem[posicao + 38] = R2;
    mem[posicao + 39] = JNZ; mem[posicao + 40] = 0x18; mem[posicao + 41] = 0x00;

    //setup2
    mem[posicao + 42] = MOV; mem[posicao + 43] = R0; mem[posicao + 44] = 0x00;
    mem[posicao + 45] = MOV; mem[posicao + 46] = R1; mem[posicao + 47] = 0x10;
    mem[posicao + 48] = MOV; mem[posicao + 49] = R3; mem[posicao + 50] = 0x38;

    mem[posicao + 51] = STORE; mem[posicao + 52] = R3; mem[posicao + 53] = 0x08;
    mem[posicao + 54] = STORE; mem[posicao + 55] = R3; mem[posicao + 56] = 0x17;
    mem[posicao + 57] = STORE; mem[posicao + 58] = R1; mem[posicao + 59] = 0x09;

    //loop2
    mem[posicao + 60] = ADD;   mem[posicao + 61] = R1; mem[posicao + 62] = R0;
    mem[posicao + 63] = STORE; mem[posicao + 64] = R1; mem[posicao + 65] = 0x44;

    mem[posicao + 66] = LOAD;  mem[posicao + 67] = R1; mem[posicao + 68] = 0x00;

    mem[posicao + 69] = CMP; mem[posicao + 70] = R1; mem[posicao + 71] = R3;
    mem[posicao + 72] = JZ;  mem[posicao + 73] = 0x6C; mem[posicao + 74] = 0x00;

    mem[posicao + 75] = LOAD; mem[posicao + 76] = R1; mem[posicao + 77] = 0x09;
    mem[posicao + 78] = ADD;  mem[posicao + 79] = R1; mem[posicao + 80] = R0;

    mem[posicao + 81] = MOV; mem[posicao + 82] = R3; mem[posicao + 83] = 0x01;
    mem[posicao + 84] = ADD; mem[posicao + 85] = R0; mem[posicao + 86] = R3;

    mem[posicao + 87] = LOAD; mem[posicao + 88] = R3; mem[posicao + 89] = 0x08;

    mem[posicao + 90] = CMP; mem[posicao + 91] = R1; mem[posicao + 92] = R2;

    mem[posicao + 93] = LOAD; mem[posicao + 94] = R1; mem[posicao + 95] = 0x09;
    mem[posicao + 96] = JNZ;  mem[posicao + 97] = 0x3C; mem[posicao + 98] = 0x00;

    //final
    mem[posicao + 99]  = MOV;   mem[posicao + 100] = R2; mem[posicao + 101] = 0x02;
    mem[posicao + 102] = STORE; mem[posicao + 103] = R2; mem[posicao + 104] = 0xFF;
    mem[posicao + 105] = HALT;  mem[posicao + 106] = 0x00; mem[posicao + 107] = 0x00;

    mem[posicao + 108] = STORE; mem[posicao + 109] = R0; mem[posicao + 110] = 0x20;
    mem[posicao + 111] = HALT;  mem[posicao + 112] = 0x00; mem[posicao + 113] = 0x00;
}



int main(int argc,char *argv[]){
	linearSearch();

	while (running && pc < 255) {
		uint8_t op,a,b;
		ciclo++;
		fetch(&op,&a,&b);
		decode_execute(op,a,b);
		trace(op,a,b);
	}

	printf("%d\n",mem[0x20]);
	printf("%d\n",mem[0xFF]);

};