#include <stdint.h>
#include <stdio.h>

uint8_t mem[255] = {0};
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
    uint8_t posicao = 0x00;

    //setup inicial
    mem[posicao+0] = MOV; mem[posicao+1] = R0; mem[posicao+2] = 0x01;  // incremento de 1
    mem[posicao+3] = MOV; mem[posicao+4] = R1; mem[posicao+5] = 0x10;  // inicio array    
    mem[posicao+6] = MOV; mem[posicao+7] = R2; mem[posicao+8] = 0x18;  // fim array     
    mem[posicao+9] = MOV; mem[posicao+10] = R3; mem[posicao+11] = 0x37;    // valor para preencher    

    mem[posicao+12] = JMP; mem[posicao+13] = posicao+24; mem[posicao+14] = 0x00;     

    //Loop de preenchimento
    mem[posicao+24] = STORE; mem[posicao+25] = R3; mem[posicao+26] = R1; // salva o valores no array

    mem[posicao+27] = ADD; mem[posicao+28] = R1; mem[posicao+29] = R0; 
    mem[posicao+30] = ADD; mem[posicao+31] = R3; mem[posicao+32] = R0;  

    //Verifica se terminou
    mem[posicao+33] = CMP; mem[posicao+34] = R1; mem[posicao+35] = R2;     
    mem[posicao+36] = JNZ; mem[posicao+37] = posicao+24; mem[posicao+38] = 0x00; // volta pro loop    
    //Finaliza loop

    //valor para verificar
    mem[posicao+39] = MOV; mem[posicao+40] = R0; mem[posicao+41] = 0x38; 
    mem[posicao+42] = STORE; mem[posicao+43] = R0; mem[posicao+44] = 0x08;

    // loop principal de busca
    mem[posicao+45] = MOV;   mem[posicao+46] = R1; mem[posicao+47] = 0x10;
    mem[posicao+48] = LOAD;  mem[posicao+49] = R0; mem[posicao+50] = 0x08; 

    mem[posicao+51] = LOAD;  mem[posicao+52] = R3; mem[posicao+53] = R1;   
    mem[posicao+54] = CMP;   mem[posicao+55] = R3; mem[posicao+56] = R0;   
    mem[posicao+57] = JZ;    mem[posicao+58] = posicao+69; mem[posicao+59] = 0x00; 

    mem[posicao+60] = ADD;   mem[posicao+61] = R1; mem[posicao+62] = 0x01; 
    mem[posicao+63] = CMP;   mem[posicao+64] = R1; mem[posicao+65] = R2;   
    mem[posicao+66] = JNZ;   mem[posicao+67] = posicao+51; mem[posicao+68] = 0x00; 
    // fim do loop de busca 

    mem[posicao+69] = HALT;   mem[posicao+70] = 0x00; mem[posicao+71] = 0x00;

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
