//   která čte jedno slovo ze souboru f do zadaného pole znaků
//   a vrátí délku slova (z delších slov načte prvních max-1 znaků,
//   a zbytek přeskočí). Funkce vrací EOF, pokud je konec souboru.
//   Umístěte ji do zvláštního modulu "io.c" (nepatří do knihovny).
//   Poznámka: Slovo je souvislá posloupnost znaků oddělená isspace znaky.

// Omezení: řešení v C bude tisknout jinak uspořádaný výstup
//   a je povoleno použít implementační limit na maximální
//   délku slova (např. 127 znaků), delší slova se ZKRÁTÍ a program
//   při prvním delším slovu vytiskne varování na stderr (max 1 varování).

// Poznámka: Vhodný soubor pro testování je například seznam slov
//           v souboru /usr/share/dict/words
//           nebo texty z http://www.gutenberg.org/
//           případně výsledek příkazu:  "seq 1000000 2000000|shuf"

#include "io.h"

int get_word(char *s, int max, FILE *f)
{
    if(s == NULL || f == NULL || max ==0)
        return EOF;
	int c;
	int count = 0;
    //while(isspace(c = fgetc(f)) || c != EOF) ;
	while((c = fgetc(f)) != EOF)
	{    
        
        if(count >= max)
        {
            s[count] = '\0';
            //while(!isspace(c = fgetc(f)) || c != EOF) ;
            return max;
        }
        if(isspace(c))
        {
            if(count)
            {
                s[count] = '\0';
                return count;
            }
        }
        else 
        {
            s[count++] = c;	
        }
		
        
        //count++;
	}
    return EOF;
}