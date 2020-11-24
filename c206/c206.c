
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/


    //seznam nemá žádné prvky, proto se všechny poitry na prvky nastaví na NULL.
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
    
	

}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    //dočasný ukazatel
    tDLElemPtr tmp = L->First;

    //smyčka pojede od prvního prvku seznamu do posledního a postupně tyto prvky uvolňuje
    while(tmp->rptr != NULL){
        tmp = tmp->rptr;
        free(tmp->lptr);
    }
    //nakonec uvolní i poslední
    free(tmp);

    //jakmile je seznam prázdný, opět se všechny pointry na prvky nastaví na NULL, protože namají na co ukazovat.
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;

}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokuje se paměť pro prvek
    tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
    if(new_element == NULL){
        DLError();
        return;
    }

    //nastaví se data a pointery na sousedící prvky... prvek umisťujeme na začátek, proto bude levý pointer prázdný a pravý bude ukazovat na současný první prvek.
    new_element->data = val;
    new_element->lptr = NULL;
    new_element->rptr = L->First;

    //pokud má seznam první prvek, vloží se před něj
    if(L->First != NULL)
        L->First->lptr = new_element;
    //pokud nemá, tak se nastaví jako poslední, aby se seznam propojil.
    else
        L->Last = new_element;
    //nakonec se v seznamu nastaví jako první prvek seznamu.
    L->First = new_element;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokuje se paměť pro prvek
    tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
    if(new_element == NULL){
        DLError();
        return;
    }

    //nastaví se data a pointery na sousedící prvky... prvek umisťujeme na konec, proto bude pravý pointer prázdný a levý bude ukazovat na současný poslední prvek.
    new_element->data = val;
    new_element->rptr = NULL;
    new_element->lptr = L->Last;

    //pokud má seznam poslední prvek, vloží se za něj
    if(L->Last != NULL)
        L->Last->rptr = new_element;
    //pokud nemá, tak se nastaví jako první, aby se seznam propojil
    else
        L->First = new_element;
    //nakonec se v seznamu nastaví jako poslední prvek seznamu.
    L->Last = new_element;

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->Last;

}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL){
	    DLError();
	    return;
	}

	*val = L->First->data;

}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if(L->First == NULL){
        DLError();
        return;
    }

    *val = L->Last->data;

}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if(L->First == NULL)
        return;

    if(L->Act == L->First)
        L->Act = NULL;

    //jako první prvek seznamu se nastaví druhý prvek
    L->First = L->First->rptr;
    //uvolní se paměť prvního prvku
    free(L->First->lptr);
    //levý pointer nově prvního prvku se nastaví na NULL, protože nikam neukazuje.
    L->First->lptr = NULL;
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if(L->First == NULL)
        return;

    if(L->Act == L->Last)
        L->Act = NULL;

    //jako poslední prvek seznamu se nastaví předposlední prvek
    L->Last = L->Last->lptr;
    //uvolní se paměť posledního prvku
    free(L->Last->rptr);
    //pravý pointer nově posledního prvku se nastaví na NULL, protože nikam neukazuje.
    L->Last->rptr = NULL;
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

    if((L->Act == NULL) || (L->Act == L->Last))
        return;

    //vytvoří se dočasný pointer
    tDLElemPtr tmp = L->Act->rptr;

    //prvek za prvkem, který se má vymazat se nastaví jako prvek za aktuálním prvkem
    L->Act->rptr = tmp->rptr;

    //jestliže je prvek na smazání zároveň posledním prvkem, jako poslední prvek seznamu se nastaví aktivní prvek.
    if(tmp == L->Last)
        L->Last = L->Act;
    //jinak se pointer prvku následujicího prvek na smazání nastaví na aktivní prvek
    else
        tmp->rptr->lptr = L->Act;

    //a uvolní se paměť prvku
    free(tmp);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

    if((L->Act == NULL) || (L->Act == L->First))
        return;

    tDLElemPtr tmp = L->Act->lptr;

    //jako levý pointer aktivního prvku se nastaví prvek před prvkem pro smazání.
    L->Act->lptr = tmp->lptr;

    //jestliže je prvek na smazání zároveň prvním prvkem, jako první prvek seznamu se nastaví aktivní prvek.
    if(tmp == L->First)
        L->First = L->Act;
    //jinak se pointer prvku předcházejicího prvek na smazání nastaví na aktivní prvek
    else
        tmp->lptr->rptr = L->Act;

    //a uvolní se paměť prvku
    free(tmp);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if(L->Act == NULL)
        return;

    //alokace a kontrola paměti
    tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
    if(new_element == NULL){
        DLError();
        return;
    }

    //nastaví se data a pointry na sousedící prvky.
    new_element->data = val;
    new_element->lptr = L->Act;
    new_element->rptr = L->Act->rptr;

    //pointery sousedících prvku se nastaví na nový prvek
    L->Act->rptr->lptr = new_element;
    L->Act->rptr = new_element;

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    if(L->Act == NULL)
        return;

    ////alokace a kontrola paměti
    tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
    if(new_element == NULL){
        DLError();
        return;
    }

    //nastaví se data a pointry na sousedící prvky.
    new_element->data = val;
    new_element->rptr = L->Act;
    new_element->lptr = L->Act->lptr;

    //pointery sousedících prvku se nastaví na nový prvek
    L->Act->lptr = new_element;
    L->Act->lptr->rptr = new_element;

}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

    if(L->Act == NULL){
        DLError();
        return;
    }

    *val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

    if(L->Act == NULL)
        return;

    L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

    if(L->Act == NULL)
        return;

    L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

    if(L->Act == NULL)
        return;

    L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

    return (L->Act != NULL);
}

/* Konec c206.c*/
