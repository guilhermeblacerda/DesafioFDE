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
		case 0x01: rg[a] = mem[b]; break;                 // LOAD  
        case 0x02: mem[b] = rg[a]; break;                 // STORE 
        case 0x03: rg[a] = rg[a] + rg[b]; break;          // ADD 
        case 0x04: rg[a] = rg[a] - rg[b]; break;          // SUB  
        case 0x05: rg[a] = b; break;                      // MOV 
        case 0x06: flag = (rg[a] == rg[b]) ? 1 : 0; break;// CMP 
        case 0x07: pc = a; break;                         // JMP  
        case 0x08: if(flag) pc = a; break;                // JZ   
        case 0x09: if(!flag) pc = a; break;               // JNZ  
        case 0x0A: running = 0; break;                    // HALT
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
	mem[0x08] = 56;                        
    mem[0x10] = 12; mem[0x11] = 34; mem[0x12] = 56; mem[0x13] = 78;
	mem[0x14] = 90; mem[0x15] = 11; mem[0x16] = 22; mem[0x17] = 33;

	uint8_t *memoria = mem;
    uint8_t posicao = 0x00;

	// MOV R1, 0xFF  (resultado padrão = não encontrado)
    mem[posicao]=0x05; mem[posicao+1]=0x01; mem[posicao+2]=0xFF; posicao+=3;

    // LOAD R0, 0x08  (carrega alvo)
    mem[posicao]=0x01; mem[posicao+1]=0x00; mem[posicao+2]=0x08; posicao+=3;

	//posicao = 6
	

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
