#define D3 2
#define D2 3
#define D1 4
#define D0 5

#define E 6
#define RS 7


#define DS 8

#define BUZZER 9

#define EN 10
#define CLK 11


#define DIR 12
#define ESQ 13

void kpInit();
int kpRead();
void kpDebounce();

static int valor = 0x0000;
static unsigned char tempo;

void serialInit();
void serialWrite(unsigned char data);

void ssdInit();
void ssdUpdate();
void ssdDigit(unsigned char pin,unsigned char data);

char disp[4];
char atual=0;

void lcdInit();
void lcdCmd(unsigned char cmd);
void lcdData(unsigned char data);

void kpInit(){
	pinMode(DIR, INPUT);
	pinMode(ESQ, INPUT);
}
int kpRead(){
	return valor;
}
void kpDebounce(){
	
	static int novo = 0x0000;
	static int antigo = 0x0000;
	for (unsigned char i = 0; i<5; i++){
		serialWrite(1<<(i+1));
		if (digitalRead(DIR)){
			novo |= (int)1<<i;
		}
      	else{
          novo &= ~(1<<i);
        }
		if (digitalRead(ESQ)){
			novo |= (int)1<<(i+5);
		}
      	else{
          novo &= ~(1<<(i+5));
      	}
	}
	if (novo == antigo){
		tempo--;
	}
	else{
		antigo = novo;
		tempo = 12;
	}
	if (!tempo){
		valor = novo;
	}
}
void serialInit(){
  pinMode(DS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(CLK, OUTPUT);
  
  digitalWrite(DS, LOW);
  digitalWrite(EN, LOW);
  digitalWrite(CLK, LOW);
}
void serialWrite(unsigned char data){
  digitalWrite(EN, LOW); //Desabilita a transferencia de dados para a saida
  
  for (char i = 0; i < 8; i++){
    digitalWrite(CLK, LOW);
    if (data & 0x80>>i){
      digitalWrite(DS, HIGH);
    }
    else{
      digitalWrite(DS, LOW);
    }
    digitalWrite(CLK, HIGH);
  }
  digitalWrite(EN, HIGH);
  digitalWrite(DS, LOW);
  digitalWrite(CLK, LOW);
  
}
void ssdInit(){
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
}
void ssdUpdate(){
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW); // Desliga todos os displays
  
  switch (atual){
    case 0:
    serialWrite(disp[0]);
    digitalWrite(D0, HIGH);
    atual = 1;
    	break;
    case 1:
    serialWrite(disp[1]);
    digitalWrite(D1, HIGH);
    atual = 2;
    	break;
    case 2:
    serialWrite(disp[2]);
    digitalWrite(D2, HIGH);
    atual = 3;
    	break;
    case 3:
    serialWrite(disp[3]);
    digitalWrite(D3, HIGH);
    atual = 0;
    	break;
    default:
    atual = 0;
    	break;
  }
}
void ssdDigit(unsigned char pin, unsigned char data){
  static const char valor [] = {0xFC , 0x60 , 0xDA , 0xF2 , 0x66 , 0xB6 , 0xBE , 0xE0 , 0xFE , 0xF6 , 0xEE , 0x3E , 0x9C , 0x7A ,0x9E , 0x8E };
  disp[pin] = valor[data];
}
void lcdInit(){
  	delay(10);
  
	pinMode(RS, OUTPUT);
	pinMode(E, OUTPUT);
	digitalWrite(RS, LOW);
	digitalWrite(E, LOW);
	digitalWrite(RS, LOW);
  
  	lcdCmd(0x02); //Chaveia para o modo 4 bits
	lcdCmd(0x28) ; //4 bits, 2 linhas, 5x8
	lcdCmd(0x06) ; //modo incremental
	lcdCmd(0x0F) ; // display, cursor e blinking ON
	lcdCmd(0x03) ; // zera contadores internos
	lcdCmd(0x01) ; // limpar display
	lcdCmd(0x80) ; // posição inicial
}
void lcdCmd(unsigned char cmd){
	digitalWrite(RS, LOW);
	
	serialWrite4bits(cmd);
	digitalWrite(E, HIGH);
	delay(3);
	digitalWrite(E, LOW);
	delay(3);
  
	serialWrite4bits(cmd<<4);
	digitalWrite(E, HIGH);
	delay(3);
	digitalWrite(E, LOW);
	delay(3);
}
void lcdData(unsigned char data){
	digitalWrite(RS, HIGH);
	
	serialWrite4bits(data);
	digitalWrite(E, HIGH);
	delay(3);
	digitalWrite(E, LOW);
	delay(3);
	
	serialWrite4bits(data<<4);
	digitalWrite(E, HIGH);
	delay(3);
	digitalWrite(E, LOW);
  	digitalWrite(RS, LOW);
	delay(3);
		
}