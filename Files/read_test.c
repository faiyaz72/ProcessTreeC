#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>


const char *PROC_ROOT = "tests";
const unsigned int MAX_PATH_LENGTH = 1024;




int main(int argc, char *argv[]) {
	char childrenpath[MAX_PATH_LENGTH + 1];
	FILE *childrenfile;
	char children[MAX_PATH_LENGTH + 1];
	//char child[MAX_PATH_LENGTH+1];
	pid_t pid = 1123;

	sprintf(childrenpath, "%s/%d/task/%d/children", PROC_ROOT, pid, pid);


	childrenfile = fopen(childrenpath, "r");

	int first;
	fgets(children, 100, childrenfile);
	fscanf(childrenfile, "%d", &first);
	printf("%d\n", first);


	while(fscanf(childrenfile, "%d", &first) != EOF) {
		printf("%d\n", first);
	}

	printf("%s\n",children);
}