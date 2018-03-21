#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char *PROC_ROOT = "tests";
const unsigned int MAX_PATH_LENGTH = 1024;
struct TreeNode {
    pid_t pid;
    char *name;
    struct TreeNode *child;     // A link to the first of the child processes
    struct TreeNode *sibling;   // A link to the next sibling process.
};

int helper_count(FILE* children) {
    char test[1000];
    fgets(test, 100, children);
    int result = 0;
    int k = strlen(test);
    for (int i =0; i < k; i++) {
        if (test[i] == ' ') {
            result+= 1;
        }
    }
    return result + 1;
    fclose(children);
}

void print_list(int *array, int k) {
    for (int i = 0; i < k; i++) {
        printf("%d \n", array[i]);
    }
}

void helper_sibling(struct TreeNode **root, int proc_id) {
    char cmdName[MAX_PATH_LENGTH + 1];
    FILE *namefile;
    sprintf(cmdName, "%s/%d/cmdline", PROC_ROOT, proc_id);
    namefile = fopen(cmdName, "r");
    if (namefile == NULL) {
        return;
    }
    char name[MAX_PATH_LENGTH+1];
    fscanf(namefile, "%s", name);
    *root = malloc(sizeof(struct TreeNode));
    (*root)->pid = proc_id;

    (*root)->name = malloc(sizeof(char*) * strlen(name));
    strcpy((*root)->name, name);
}

int generate_ptree(struct TreeNode **root, pid_t pid) {

    char procfile[MAX_PATH_LENGTH + 1];
    char cmdName[MAX_PATH_LENGTH + 1];
    char childrenpath[MAX_PATH_LENGTH + 1];

    FILE *namefile;
    FILE *childrenfile;

    char name[MAX_PATH_LENGTH+1];
    //char child[MAX_PATH_LENGTH+1];

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

    fscanf(namefile, "%s", name);
    //struct stat *buf;
    fclose(namefile);

    int number = helper_count(childrenfile);
    int children_list[number];
    int i = 0;
    //printf("%d\n", number);
    childrenfile = fopen(childrenpath, "r");
     if (childrenfile == NULL) {
        return 1;
    }
    int process_id;
    //fscanf(childrenfile, "%d", &process_id);

    while(fscanf(childrenfile, "%d", &process_id) != EOF) {
        //printf("%d\n", process_id);
        children_list[i] = process_id;
        i+=1;
    }
    //print_list(children_list, number);
    struct TreeNode **temp = NULL; 
    //struct TreeNode **temp2 = NULL;
    //*temp = malloc(sizeof(struct TreeNode));
    *root = malloc(sizeof(struct TreeNode));
    (*root)->pid = pid; 
    //(*root)->name = malloc(sizeof(char*) * strlen(name));
    //strcpy((*root)->name, name);
    (*root)->name = name;
    generate_ptree((&(*root)->child), children_list[0]);
    //if ((&(*root)->child) != NULL) {
    temp = (&(*root)->child);
    //}
    for (int k =1; k < number; k++) {
        generate_ptree(&(*temp)->sibling, children_list[k]);
        temp = &(*temp)->sibling;
    }
    fclose(childrenfile);

    return 0;
}














void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a way to keep track of the depth (in the tree) you're at
    // and print 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    if (root == NULL) {
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

int main(int argc, char *argv[]) {
    // Creates a ptree to test printing
    // Notice that in this tree the names are string literals. This is fine for
    // testing but is not what the assignment asks you to do in generate_ptree.
    // Read the handout carefully. 
    struct TreeNode root, child_one, child_two, child_three, grandchild, grandchild2;
    root.pid = 4511;
    root.name = "root process";
    root.child = &child_one;
    root.sibling = NULL;

    child_one.pid = 4523;
    child_one.name = "first child";
    child_one.child = NULL;
    child_one.sibling = &child_two;

    child_two.pid = 4524; 
    child_two.name = "second child";
    child_two.child = &grandchild;
    child_two.sibling = &child_three;

    child_three.pid = 5673; 
    child_three.name = "third child";
    child_three.child = &grandchild2;
    child_three.sibling = NULL;

    grandchild.pid = 4609;
    grandchild.name = "grandchild of second";
    grandchild.child = NULL;
    grandchild.sibling = NULL;

    grandchild2.pid = 4656;
    grandchild2.name = "grandchild of third";
    grandchild2.child = NULL;
    grandchild2.sibling = NULL;

    struct TreeNode *test = NULL;
    generate_ptree(&test, 1123);
    print_ptree(test,0);
    print_ptree(&root, 0);
    printf("reached end\n");

    return 0;
}