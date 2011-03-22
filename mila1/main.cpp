/* main.c */
/* syntakticky analyzator */

#include "lexan.h"
#include "parser.h"
#include "zaspoc.h"
#include <stdio.h>


void main(int argc, char *argv[])
{
   char *jmeno;
   printf("Interpret jazyka MILA\n");
   if (argc == 1) {
      printf("Vstup z klavesnice, zadejte zdrojovy program\n");
      jmeno = NULL;
   } else {
      jmeno = argv[1];
      printf("Vstupni soubor %s\n", jmeno);
   }
   InitLexan(jmeno);
   CtiSymb();
   Program();
   Print();
   Run();
   printf("Konec\n");
}