#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "ptree.h"

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif

/*
 * Simple Helper function that counts number of children 
 * in children file
 */
int helper_count(FILE* children) {
    int dummy;
    int result = 0;
    while(fscanf(children, "%d", &dummy) != EOF) {
        result+=1;
    }
    fclose(children);
    return result;
}
/*
 * Creates a PTree rooted at the process pid.
 * The function returns 0 if the tree was created successfully 
 * and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */
int generate_ptree(struct TreeNode **root, pid_t pid) {

    char procfile[MAX_PATH_LENGTH + 1];
    char cmdName[MAX_PATH_LENGTH + 1];
    char childrenpath[MAX_PATH_LENGTH + 1];

    FILE *namefile;
    FILE *childrenfile;

    char name[MAX_PATH_LENGTH + 1];
    // Static variable for return value
    int static result = 0;
    if (sprintf(procfile, "%s/%d/exe", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }
    sprintf(cmdName, "%s/%d/cmdline", PROC_ROOT, pid);
    sprintf(childrenpath, "%s/%d/task/%d/children", PROC_ROOT, pid, pid);

    namefile = fopen(cmdName, "r");
    if (namefile == NULL) {
        return 1;
    }
    childrenfile = fopen(childrenpath, "r");
    if (childrenfile == NULL) {
        return 1;
    }
    //Initializing struct for lstat
    struct stat stat_buf;
    // Will only include process which are valid
    if (lstat(procfile, &stat_buf)) {
        result = 1;
        return result;
    } else if(stat_buf.st_nlink > 0) {


    fscanf(namefile, "%s", name);
    fclose(namefile);

    int number = helper_count(childrenfile);
    int children_list[number];
    int i = 0;

    childrenfile = fopen(childrenpath, "r");
     if (childrenfile == NULL) {
        result = 1;
        return result;
    }
    int process_id;

    // Putting children process ID to children_list
    while(fscanf(childrenfile, "%d", &process_id) != EOF) {
        children_list[i] = process_id;
        i+=1;
    }

    
    // Declaring a temporary variable to keep track of 
    struct TreeNode **temp = NULL; 

    *root = malloc(sizeof(struct TreeNode));
    (*root)->pid = pid; 
    (*root)->sibling = NULL;
    (*root)->child = NULL;
    (*root)->name = malloc(sizeof(char) * (strlen(name)+1));
    strncpy((*root)->name, name,strlen(name)+1);
    generate_ptree((&(*root)->child), children_list[0]);
        temp = (&((*root)->child));

    // Recursing for every child in the list
    for (int k =1; k < number; k++) {
        generate_ptree(&((*temp)->sibling), children_list[k]);
        if (&((*temp)->sibling) != NULL) {
        temp = &((*temp)->sibling);
        }
    }
    fclose(childrenfile);
    return result;
} else {
return result;
}
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a way to keep track of the depth (in the tree) you're at
    // and print 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    if (root == NULL || (max_depth != 0 && depth > max_depth)) {
        return;
    } 
    else if (depth < max_depth || max_depth == 0) {
        printf("%*s", depth * 2, "");
        if (root->name != NULL) {
            printf("%d: %s\n", root->pid, root->name);
        } else {
            printf("%d\n", root->pid);
        }
        depth+=1;        
        print_ptree(root->child, max_depth);
        depth -=1;
        print_ptree(root->sibling, max_depth); 
    }
    return;
}
