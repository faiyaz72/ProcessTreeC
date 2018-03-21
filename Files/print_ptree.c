#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptree.h"


int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    // printf("Usage:\n\tptree [-d N] PID\n");

    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!


    if (argc == 2) {
		struct TreeNode *root = NULL;
        int process_id = strtol(argv[1], NULL, 10);
		int flag = generate_ptree(&root, process_id);
		print_ptree(root, 0);
        if (flag == 1) {
            return 2;
        }
		return 0;
    } else if (argc == 4) {
    	// checking whether -d is provided
    	char* compare = "-d";
    	if (strcmp(compare, argv[1]) == 0) {
    		struct TreeNode *root = NULL;
			int flag = generate_ptree(&root, strtol(argv[3], NULL, 10));
            print_ptree(root, strtol(argv[2], NULL, 10));
            if (flag == 1) {
                return 2;
            }
			return 0;
    	} else {
    		fprintf(stderr, "Usage:\n\tptree [-d N] PID\n");
    		return 1;
    	}

    } else {
    	fprintf(stderr, "Usage:\n\tptree [-d N] PID\n");
    	return 1;
    }
}

