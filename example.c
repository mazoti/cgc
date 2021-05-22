/* That's all you need, include before everything in your program */
#include "garbage_collector.h"
/* -------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void){
	char* str;

	str = (char*)calloc(1,14);
	strcpy(str, "Hello World!\n");

	/* You can call gc_clean if malloc/calloc returns NULL */
	printf("%.*s", 14, str);

	return 0;
	/* The garbage will be collected when the program exits */
}
