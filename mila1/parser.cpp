/* parser.cpp */

#include "parser.h"
#include "lexan.h"
#include "tabsym.h"
#include "zaspoc.h"
#include <stdio.h>
#include <string.h>

void Program();
void Dekl();
void DeklKonst();
void ZbDeklKonst();
void DeklProm();
void ZbDeklProm();
void SlozPrikaz();
void ZbPrikazu();
void Prikaz();
void CastElse(int);
void Podminka();
Operator RelOp();
void Vyraz();
void ZbVyrazu();
void Term();
void ZbTermu();
void Faktor();

void Srovnani(LexSymbol s)
{
   if (Symb == s)
      CtiSymb();
   else
      ChybaSrovnani(s);
}

void Srovnani_IDENT(char *id)
{
   if (Symb == IDENT) {
      strcpy(id, Ident);
      CtiSymb();
   } else
      ChybaSrovnani(IDENT);
}

void Srovnani_NUMB(int *h)
{
   if (Symb == NUMB) {
      *h = Cislo;
      CtiSymb();
   } else 
      ChybaSrovnani(NUMB);
}

void Program()
{
   Dekl();
   SlozPrikaz();
   Gener(STOP);
}

void Dekl()
{
   switch (Symb) {
   case kwVAR:
      DeklProm();
      Dekl();
      break;
   case kwCONST:
      DeklKonst();
      Dekl();
      break;
   default:
      ;
   }
}

void DeklKonst()
{
   char id[MaxLenIdent];
   int hod;
   CtiSymb();
   Srovnani_IDENT(id);
   Srovnani(EQ);
   Srovnani_NUMB(&hod);
   deklKonst(id, hod);
   ZbDeklKonst();
   Srovnani(SEMICOLON);
}

void ZbDeklKonst()
{
   if (Symb == COMMA) {
      char id[MaxLenIdent];
      int hod;
      CtiSymb();
      Srovnani_IDENT(id);
      Srovnani(EQ);
      Srovnani_NUMB(&hod);
      deklKonst(id, hod);
      ZbDeklKonst();
   }
}
   
void DeklProm()
{
   char id[MaxLenIdent];
   CtiSymb();
   Srovnani_IDENT(id);
   deklProm(id);
   ZbDeklProm();
   Srovnani(SEMICOLON);
}

void ZbDeklProm()
{
   if (Symb == COMMA) {
      char id[MaxLenIdent];
      CtiSymb();
      Srovnani_IDENT(id);
      deklProm(id);
      ZbDeklProm();
   }
}

void SlozPrikaz()
{
   Srovnani(kwBEGIN);
   Prikaz();
   ZbPrikazu();
   Srovnani(kwEND);
}

void ZbPrikazu()
{
   if (Symb == SEMICOLON) {
      CtiSymb();
      Prikaz();
      ZbPrikazu();
   }
}

void Prikaz()
{
   switch (Symb) {
   case IDENT: {
      Gener(TA, adrProm(Ident));
      CtiSymb();
      Srovnani(ASSGN);
      Vyraz();
      Gener(ST);
      break;
   }
   case kwWRITE:
      CtiSymb();
      Vyraz();
      Gener(WRT);
      break;
   case kwIF: {
      CtiSymb();
      Podminka();
      int adrIFJ = Gener(IFJ);
      Srovnani(kwTHEN);
      Prikaz();
      CastElse(adrIFJ);
      break;
   }
   case kwWHILE: {
      int a1 = GetIC();
      CtiSymb();
      Podminka();
      int aIFJ = Gener(IFJ);
      Srovnani(kwDO);
      Prikaz();
      Gener(JU, a1);
      PutIC(aIFJ);
      break;
   }
   case kwBEGIN:
      SlozPrikaz();
      break;
   }
}

void CastElse(int adrIFJ)
{
   if (Symb == kwELSE) {
      CtiSymb();
      int adrJU = Gener(JU);
      PutIC(adrIFJ);
      Prikaz();
      PutIC(adrJU);
   }
}

void Podminka()
{
   Vyraz();
   Operator op = RelOp();
   Vyraz();
   Gener(BOP, op);
}

Operator RelOp()
{
   switch (Symb) {
   case EQ:
      CtiSymb();
      return Eq;
   case NEQ:
      CtiSymb();
      return NotEq;
   case LT:
      CtiSymb();
      return Less;
   case GT:
      CtiSymb();
      return Greater;
   case LTE:
      CtiSymb();
      return LessOrEq;
   case GTE:
      CtiSymb();
      return GreaterOrEq;
   default:
      Chyba("neocekavany symbol");
   }
}

void Vyraz()
{
   if (Symb == MINUS) {
      CtiSymb();
      Term();
      Gener(UNM);
      ZbVyrazu();
   } else {
      Term();
      ZbVyrazu();
   }
}

void ZbVyrazu()
{
   switch (Symb) {
   case PLUS:
      CtiSymb();
      Term();
      Gener(BOP, Plus);
      ZbVyrazu();
      break;
   case MINUS:
      CtiSymb();
      Term();
      Gener(BOP, Minus);
      ZbVyrazu();
      break;
   }
}

void Term()
{
   Faktor();
   ZbTermu();
}

void ZbTermu()
{
   switch (Symb) {
   case TIMES:
      CtiSymb();
      Faktor();
      Gener(BOP, Times);
      ZbTermu();
      break;
   case DIVIDE:
      CtiSymb();
      Faktor();
      Gener(BOP, Divide);
      ZbTermu();
      break;
   }
}

void Faktor()
{
   switch (Symb) {
   case IDENT:
      char id[MaxLenIdent];
      Srovnani_IDENT(id);
      GenTR(id);
      break;
   case NUMB:
      int hodn;
      Srovnani_NUMB(&hodn);
      Gener(TC, hodn);
      break;
   case LPAR: {
      CtiSymb();
      Vyraz();
      Srovnani(RPAR);
      break;
   }
   default:
      Chyba("Neocekavany symbol");
   }
}
