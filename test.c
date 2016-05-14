#include "extlib.vect.more.h"
#include "extlib.sll.more.h"
#include "extlib.dll.h"

Dll directoriesStack;

void printDirectoriesStack() {
	DllNode node = dllGetFront(directoriesStack);
	while(node!=NULL) {
		printf("%s ",*((char **)dllGetData(node)));
		node = dllGetNext(node);
	}
	printf("\n");
}

int main() {
    char *pwd = "voici le pwd!";

    directoriesStack = dllNew(sizeof(char*));
	dllPushFront(directoriesStack,&pwd);

	printf("%s\n",pwd);

	printDirectoriesStack();

    return EXIT_SUCCESS;
}
