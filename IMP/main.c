/* Header file with all the essential definitions for a given type of MCU */
#include "MK60DZ10.h"
#include <stdlib.h>
#include <string.h>

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		20
#define tdelay3 		70


#define R0 0x04000000
#define R1 0x01000000
#define R2 0x00000200
#define R3 0x02000000
#define R4 0x10000000
#define R5 0x00000080
#define R6 0x08000000
#define R7 0x20000000
#define DEL() PTA->PDOR &= GPIO_PDOR_PDO(0x00000000)

unsigned char A[] = {0b00000000,0b00111100,0b01100110,0b01100110,0b01111110,0b01100110,0b01100110,0b01100110};
unsigned char B[] = {0b01111000,0b01001000,0b01001000,0b01110000,0b01001000,0b01000100,0b01000100,0b01111100};
unsigned char C[] = {0b00000000,0b00011110,0b00100000,0b01000000,0b01000000,0b01000000,0b00100000,0b00011110};
unsigned char D[] = {0b00000000,0b00111000,0b00100100,0b00100010,0b00100010,0b00100100,0b00111000,0b00000000};
unsigned char E[] = {0b00000000,0b00111100,0b00100000,0b00111000,0b00100000,0b00100000,0b00111100,0b00000000};
unsigned char F[] = {0b00000000,0b00111100,0b00100000,0b00111000,0b00100000,0b00100000,0b00100000,0b00000000};
unsigned char G[] = {0b00000000,0b00111110,0b00100000,0b00100000,0b00101110,0b00100010,0b00111110,0b00000000};
unsigned char H[] = {0b00000000,0b00100100,0b00100100,0b00111100,0b00100100,0b00100100,0b00100100,0b00000000};
unsigned char I[] = {0b00000000,0b00111000,0b00010000,0b00010000,0b00010000,0b00010000,0b00111000,0b00000000};
unsigned char J[] = {0b00000000,0b00011100,0b00001000,0b00001000,0b00001000,0b00101000,0b00111000,0b00000000};
unsigned char K[] = {0b00000000,0b00100100,0b00101000,0b00110000,0b00101000,0b00100100,0b00100100,0b00000000};
unsigned char L[] = {0b00000000,0b00100000,0b00100000,0b00100000,0b00100000,0b00100000,0b00111100,0b00000000};
unsigned char M[] = {0b00000000,0b00000000,0b01000100,0b10101010,0b10010010,0b10000010,0b10000010,0b00000000};
unsigned char N[] = {0b00000000,0b00100010,0b00110010,0b00101010,0b00100110,0b00100010,0b00000000,0b00000000};
unsigned char O[] = {0b00000000,0b00111100,0b01000010,0b01000010,0b01000010,0b01000010,0b00111100,0b00000000};
unsigned char P[] = {0b00000000,0b00111000,0b00100100,0b00100100,0b00111000,0b00100000,0b00100000,0b00000000};
unsigned char Q[] = {0b00000000,0b00111100,0b01000010,0b01000010,0b01000010,0b01000110,0b00111110,0b00000001};
unsigned char R[] = {0b00000000,0b00111000,0b00100100,0b00100100,0b00111000,0b00100100,0b00100100,0b00000000};
unsigned char S[] = {0b00000000,0b00111100,0b00100000,0b00111100,0b00000100,0b00000100,0b00111100,0b00000000};
unsigned char T[] = {0b00000000,0b01111100,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000};
unsigned char U[] = {0b00000000,0b01000010,0b01000010,0b01000010,0b01000010,0b00100100,0b00011000,0b00000000};
unsigned char V[] = {0b00000000,0b00100010,0b00100010,0b00100010,0b00010100,0b00010100,0b00001000,0b00000000};
unsigned char W[] = {0b00000000,0b10000010,0b10010010,0b01010100,0b01010100,0b00101000,0b00000000,0b00000000};
unsigned char X[] = {0b00000000,0b01000010,0b00100100,0b00011000,0b00011000,0b00100100,0b01000010,0b00000000};
unsigned char Y[] = {0b00000000,0b01000100,0b00101000,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000};
unsigned char Z[] = {0b00000000,0b00111100,0b00000100,0b00001000,0b00010000,0b00100000,0b00111100,0b00000000};



typedef struct node Node;
struct node {
	Node *next;
	uint32_t data;
};
typedef struct circularBuffer CircularBuffer;
struct circularBuffer {
	Node *first;
	uint32_t size;
};

void addNode(CircularBuffer *cb, Node *n)
{
	Node *first = cb->first;
	if (first == NULL)
	{
		cb->first = n;
		cb->first->next = cb->first;
		return;
	}

	Node *last = first->next;
	while(last->next != first) last = last->next;
	last->next = n;
	n->next = first;

	cb->size++;
}

void freeBuffer(CircularBuffer *cb)
{
	Node * first = cb->first;
	Node * next = first->next;
	while(next != first)
	{
		Node * tmp = next;
		next = next->next;
		free(tmp);
	}
	free(first);
	free(cb);
}


uint32_t* ParseLetter(unsigned char letter[])
{
	uint32_t *result = malloc(8*32);
	uint32_t pos = 0;
	for(int i = 0; i < 8 ; i++)
		result[i] = 0;

	for (int i = 0; i < 8; i++)
	{
		if (i == 0) 	 pos = R0;
		else if (i == 1) pos = R1;
		else if (i == 2) pos = R2;
		else if (i == 3) pos = R3;
		else if (i == 4) pos = R4;
		else if (i == 5) pos = R5;
		else if (i == 6) pos = R6;
		else if (i == 7) pos = R7;


		if (letter[i] & 0b00000001)
		{
			result[7] |= pos;
		}
		if (letter[i] & 0b00000010)
		{
			result[6] |= pos;
		}
		if (letter[i] & 0b00000100)
		{
			result[5] |= pos;
		}
		if (letter[i] & 0b00001000)
		{
			result[4] |= pos;
		}
		if (letter[i] & 0b00010000)
		{
			result[3] |= pos;
		}
		if (letter[i] & 0b00100000)
		{
			result[2] |= pos;
		}
		if (letter[i] & 0b01000000)
		{
			result[1] |= pos;
		}
		if (letter[i] & 0b10000000)
		{
			result[0] |= pos;
		}
	}
	return result;
}

/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );
}


/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}

/* Function delay loop */
void delayFunction( void (*fun)(Node * ), Node * param)
{

	int i, j;

	for(i=0; i<5; i++)
	{
		for(j=0; j<5; j++)
		{
			fun(param);
		}
	}

}


/* Conversion of requested column number into the 4-to-16 decoder control.  */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i =0; i<4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
		    case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

void writeColumn(Node* node)
{

	for (int k = 0; k < 16; k++)
	{
		DEL();
		column_select(k);
		PTA->PDOR |= GPIO_PDOR_PDO(node->data);
		PTE->PDDR &= ~GPIO_PDDR_PDD( GPIO_PIN(28) );
		delay(tdelay3, tdelay3);
		PTE->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(28));

		node = node->next;
	}
}


int main(void)
{
	SystemConfig();

	delay(tdelay1, tdelay2);

	uint32_t *columns = ParseLetter(K);
	uint32_t *columns2 = ParseLetter(O);
	uint32_t *columns3 = ParseLetter(T);

	/*uint32_t* total = malloc(5*8*32); // array to hold the result

	memset(total, 0, 32*8*5);
	memcpy(total , columns, 8 * 32);
	memcpy(total + 8, columns2, 8 * 32);
	memcpy(total + 16, columns3, 8 * 32);*/

	uint32_t* total = malloc(5*8*32); // array to hold the result

	memset(total, 0, 32*8*3);
	memcpy(total + 16, columns , 8 * 32);
	memcpy(total + 24, columns2, 8 * 32);
	memcpy(total + 32, columns3, 8 * 32);

	CircularBuffer *cb = malloc(sizeof(struct circularBuffer));
	cb->first = NULL;
	for(int i = 0; i < 40; i++)
	{
		Node *n = malloc(sizeof(struct node));
		n->data = total[i];
		addNode(cb, n);
	}

	Node *start = cb->first;
	Node *next = start;
	Node *tmp = start;
	for (;;)
	{
		start = start->next;
		next = start;
		delay(tdelay1, tdelay2);
		delayFunction(writeColumn,tmp);
		tmp = next;
	}
	free(columns);
	free(columns2);
	free(columns3);
	free(total);
	freeBuffer(cb);
    /* Never leave main */
    return 0;
}
