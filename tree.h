// Copyright 2022 Dumitru Bianca Stefania & Nioata Alexandra 312CA
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct ListNode ListNode;
typedef struct List List;

//  document can be file or folder
enum TreeNodeType {
    FILE_NODE,
    FOLDER_NODE
};

//  a file contains content in form of text
struct FileContent {
    char* text;
};

//  a folder contains multiple documents stored as a linked list
struct FolderContent {
    List* children;
};

//  each document has a parent, name, type and content
struct TreeNode {
    TreeNode* parent;
    char* name;
    enum TreeNodeType type;
    void* content;
};

//  the file tree is defined by its root
struct FileTree {
    TreeNode* root;
};

//  a node in the list contains the payload and a pointer
//  to the next node in the list
struct ListNode {
    TreeNode* info;
    ListNode* next;
};

//  a list is defined by its head
struct List {
    ListNode* head;
};

//  display all the files and directories in the directory
//  or the content of the file
void ls(TreeNode* currentNode, char* arg);

//  print the name of the current directory/file
void pwd(TreeNode* treeNode);

//  change the current directory to the one specified by path
TreeNode* cd(TreeNode* currentNode, char* path);

//  recursively list or display the content of a directory in a tree-like format
void tree(TreeNode* currentNode, char* arg);

//  create a new directory with specified name
void mkdir(TreeNode* currentNode, char* folderName);

//  remove a file form its parent directory by deleting it
void rm(TreeNode* currentNode, char* fileName);

//  remove an empty directory form its parent directory by deleting it
void rmdir(TreeNode* currentNode, char* folderName);

//  remove an non-empty directory form its parent directory by
//  recursively deleting it and all the documents inside of it
void rmrec(TreeNode* currentNode, char* resourceName);

//  create a new file with specified name and content
void touch(TreeNode* currentNode, char* fileName, char* fileContent);

//  copy the source document into the destination directory
void cp(TreeNode* currentNode, char* source, char* destination);

//  move the source document from its parent directory
//  to the destination directory/file
void mv(TreeNode* currentNode, char* source, char* destination);

//  helper function to be recusively called when executing the tree command
void __tree(TreeNode* currentNode, int *dir, int *files, int depth);

//  helper function that takes a path name separated by "/" and moves to
//  the final document in that path
int find_path(TreeNode *currentNode, char *path_, TreeNode **path_node);

//  create the file tree that stores all the iformation about the system
FileTree createFileTree();

//  free memory for the file tree and all the documents inside of it
void freeTree(FileTree fileTree);

char *strdup(char *src);
