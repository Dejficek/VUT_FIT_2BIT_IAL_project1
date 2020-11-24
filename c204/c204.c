
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {

    char top;

    //smyčka, která iteruje, dokud není zásobník prázdný
    while(!stackEmpty(s)){
        stackTop(s, &top);

        switch(top){
            //jestliže je na vrcholu zásobníku znak '=', vloží se na konec výstupního řetězce
            case '=':
                postExpr[(*postLen)++] = top;
                break;
            //jestliže je na vrcholu zásobníku znak levá závorka, odstraní se ze zásobníku a a funkce je ukončena, protože toto je účel této funkce
            case '(':
                stackPop(s);
                return;
            //jestliže je na vrcholu zásobníku jakýkoliv jiný znak, vloží se na konec výstupního řetězce a odstraní se ze zásobníku
            default:
                postExpr[(*postLen)++] = top;
                stackPop(s);
                break;
        }
    }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {

    //pomocná proměnná pro vrchol zásobníku
    char top;

    //pokud je zásobník prázdný, operátor se vloží na vrchol zásobníku
    if (stackEmpty(s)) {
        stackPush(s, c);
        return;
    }

    stackTop(s, &top);

    //pokud je na vrcholu zásobníku levá závorka, aktuálně načtený operátor se vloží na vrchol zásobníku
    if (top == '(') {
        stackPush(s, c);
        postLen++;
        return;
    }

    switch (c) {
        case '*':
        case '/':
            //pokud je aktuálně načtený znak násobení, a na vrcholu zásobníku je '+' nebo '-', znamená to, že na vrcholu zásobníku
            //se nachází operátor s nižší prioritou a proto se aktuální operátor vloží na vrchol zásobníku
            if ((top == '+') || (top == '-')) {
                stackPush(s, c);
                return;
            //pokud je na vrcholu zásobníku jiný operátor, znamená to, že má stejnou prioritu a tento oprátor z vrcholu zásobníku
            //se vloží na konec výstupního řetězce a odstraní se ze zásobníku. Poté se rekurzivně zavolá tato funkce a opakuje se stejný proces,
            //dokud nebude možné aktuálně načtený operátor vložit na vrchol zásobníku
            } else {
                stackPop(s);
                postExpr[(*postLen)++] = top;
                doOperation(s, c, postExpr, postLen);
            }
            break;
        // v tomto případě platí, že se načetl operátor '+' nebo '-'
        // v tomto případě se na vrcholu zásobníku na 100% nachází operátor s vyšší prioritou, proto se operátor z vrcholu zásobníku vloží na konec výstupního řetězce
        // a odstraní se ze zásobníku... dále se rekurzivně volá tato funkce a opakuje se stejný proces, dokud nebude možné aktuálně načtený operátor vložit na vrchol zásobníku
        default:
            postExpr[(*postLen)++] = top;
            stackPop(s);
            doOperation(s, c, postExpr, postLen);
            break;
    }
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

    //vytvoří a inicializuje se stack
    tStack* s;
    s = (tStack *)malloc(sizeof(tStack));
    stackInit(s);

    //indexy pro pole charů
    int inLen = 0;              //vstupní pole
    unsigned int outLen = 0;    //výstupní pole

    //pomocné proměnné
    char *top;          //vrchol zásobníku
    char c;             //aktuální znak, se kterým se pracuje

    //alokace pro pole znaků, které bude sloužit jako výstupní řetězec
    char *str = (char *)calloc(MAX_LEN, sizeof(char));

    //ošetření chybového vstupního stavu
    if(str == NULL)
        return NULL;

    //smyčka, která bude iterovat až do konce vstupního řetězce
    while((c = infExpr[inLen++]) != '\0') {

        //pokud je aktuálně načtený znak [a-zA-Z0-9], přidá se na konec výstupního řetězce
        if (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9'))){
            str[outLen++] = c;
        }

        //levá závorka se pushne na zásobník
        else if (c == '('){
            stackPush(s, c);
        }

        //pokud je aktuálně načtený znak [+-*/], zavolá se funkce, která rozhodne, co se má dít
        else if ((c == '+') || (c == '-') || (c == '*') || (c == '/')){
            doOperation(s, c, str, &outLen);
        }

        //pokud je aktuálně načtená pravá závorka, zavolá se funkce, která rozhodne, co se má dít
        else if (c == ')'){
            untilLeftPar(s, str, &outLen);
        }

        //pokud je aktuálně načtený znak '=', budou se na konec výstupního řetězce vkládat znaky z vrcholu zásobníku, dokud není zásobník prázdný.
        //nakonec se na konec výstupního řetězce přidá znak '='
        else if(c == '='){
            while (!stackEmpty(s)){
                stackTop(s, &top);
                str[outLen++] = top;
                stackPop(s);
            }
            str[outLen++] = '=';
            free(s);
            return str;
        }
    }

    //uvolnění paměti zásobníku
    free(s);

    //funkce vrací výstupní řetězec
    return str;
}

/* Konec c204.c */
