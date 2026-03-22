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
    uint8_t posicao = 0x00;

    //setup inicial
    mem[posicao+0] = 0x05; mem[posicao+1] = 0x00; mem[posicao+2] = 0x01;  // MOV R0, 1 (incremento) 
    mem[posicao+3] = 0x05; mem[posicao+4] = 0x01; mem[posicao+5] = 0x10;  // MOV R1, 16 (início: 0x10)    
    mem[posicao+6] = 0x05; mem[posicao+7] = 0x02; mem[posicao+8] = 0x18;  // MOV R2, 24 (fim: 0x18)    
    mem[posicao+9] = 0x05; mem[posicao+10] = 0x03; mem[posicao+11] = 56;    // MOV R3, 56 (valor para preencher)    

    //Loop de preenchimento
    mem[posicao+12] = 0x07; mem[posicao+13] = 0x18; mem[posicao+14] = 0x00; // JMP 0x18    

    mem[posicao+24] = 0x02; mem[posicao+25] = 0x01; mem[posicao+26] = 0x1D; // STORE R1, 0x1D    
    
    mem[posicao+27] = 0x02; mem[posicao+28] = 0x03; mem[posicao+29] = 0x00; // STORE R3    
    mem[posicao+30] = 0x03; mem[posicao+31] = 0x01; mem[posicao+32] = 0x00; // ADD R1, R0    

    //Verifica se terminou
    mem[posicao+33] = 0x06; mem[posicao+34] = 0x01; mem[posicao+35] = 0x02; // CMP R1, R2    
    mem[posicao+36] = 0x09; mem[posicao+37] = 0x18; mem[posicao+38] = 0x00; // JNZ 0x18 (Volta se não for 24)    


    
    mem[posicao+39] = 0x0A; // HALT após preencher


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
