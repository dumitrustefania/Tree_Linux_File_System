// Copyright 2022 Dumitru Bianca Stefania & Nioata Alexandra 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "linked_list.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

FileTree createFileTree(char *rootFolderName)
{
    FileTree tree;
    //  allocate memory for the root node
    TreeNode *root = malloc(sizeof(TreeNode));

    //  becuse root is a folder, its content will be a list of folders/files
    //  so we allocate memory for the list
    root->content = ll_create();

    //  associate the root node with its properties
    root->name = rootFolderName;
    root->parent = NULL;
    root->type = FOLDER_NODE;
    tree.root = root;

    return tree;
}

void freeTree(FileTree fileTree)
{
    //  start from the root node, eliminating files recursively
    List *folder_list = (List *)fileTree.root->content;
    ListNode *curr = folder_list->head;

    while (curr)
    {
        //  use rmrec function for eliminating each folder/file
        rmrec(fileTree.root, curr->info->name);
        curr = curr->next;
    }

    //  free memory for the root (list elements, list placeholder, name)
    ll_free(folder_list);
    free(fileTree.root->content);
    free(fileTree.root->name);
    free(fileTree.root);
}

int find_path(TreeNode *currentNode, char *path_, TreeNode **path_node)
{
    //  path node will return the last document in the path
    *path_node = currentNode;
    //  tokenize the path
    char *path = strtok(path_, "/");
    while (path)
    {   //  if argument is "..", move to the parent of the current document
        if (!strcmp(path, PARENT_DIR))
            *path_node = (*path_node)->parent;
        else
        {    //  search argument in the list of documents of the current folder
            List *folder_list = (List *)(*path_node)->content;
            int pos = find_in_list(folder_list, path, path_node);
            if (pos == -1)  // if document doesn't exist in the list
                return 0;  // not found
        }
        path = strtok(NULL, "/");
    }

    return 1;  // found
}

void ls(TreeNode *currentNode, char *arg)
{   //  if argument was given from input, move
    //  to the document where we want to use the ls command
    if (strcmp(arg, NO_ARG)) {
        if (find_path(currentNode, arg, &currentNode) == 0)
        {   //  id document doesn't exist
            printf("ls: cannot access '%s': No such file or directory\n", arg);
            return;
        }
    }

    //  if document is a folder, list all the documents inside of it
    if (currentNode->type == FOLDER_NODE)
    {
        List *folder_list = (List *)currentNode->content;
        ListNode *curr = folder_list->head;
        while (curr)
        {   //  print the name for each document in the list
            printf("%s\n", curr->info->name);
            curr = curr->next;
        }
    }
    else
    {   //  if document is a file, print its content
        printf("%s: %s\n", arg, (char *)currentNode->content);
    }
}

void pwd(TreeNode *treeNode)
{
    if (!treeNode->parent)
    {
        //  if current node is root, we print "root"
        //  and return back in the recursion
        printf("%s", treeNode->name);
        return;
    }
    //  go up, traversing the file system tree towards the root
    pwd(treeNode->parent);
    //  print "/" and the name of the current document
    printf("/%s", treeNode->name);
}

TreeNode *cd(TreeNode *currentNode, char *path)
{
    TreeNode *curr = currentNode;

    //  traverse the given path until reaching the final node
    //  or until finding a non-existent document in the path
    if (find_path(currentNode, path, &curr) == 0)
    {   //  if document doesn't exist
        printf("cd: no such file or directory: %s\n", path);
        return currentNode;
    }
    //  return node we moved to or the initial node,
    //  if the path was not a valid one
    return curr;
}

void __tree(TreeNode *currentNode, int *dir, int *files, int depth)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;
    ListNode *curr = folder_list->head;

    //  traverse the list
    while (curr)
    {   //  print depth*4 spaces to show the level of the
        //  document in the hierarchy
        for (int i = 0; i < depth; i++)
            printf("    ");

        printf("%s\n", curr->info->name);

        if (curr->info->type == FOLDER_NODE)
        {
            (*dir)++;
            //  if current document is a folder, call the tree function
            //  recursively to print all the documents inside of it
            __tree(curr->info, dir, files, depth + 1);
        }
        else
            (*files)++;

        curr = curr->next;
    }
}

void tree(TreeNode *currentNode, char *arg)
{
    //  if argument was given from input, we have to move
    //  to the document where we want to use the tree command
    if (strcmp(arg, NO_ARG)) {
        if (find_path(currentNode, arg, &currentNode) == 0 ||
            currentNode->type == FILE_NODE)
        {
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", arg);
            return;
        }
    }

    //  we call the tree command from the current node.
    //  we are also counting the no. of files anf folders
    //  in the hierarchy
    int dir = 0, files = 0;
    __tree(currentNode, &dir, &files, 0);
    printf("%d directories, %d files\n", dir, files);
}

void mkdir(TreeNode *currentNode, char *folderName)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;
    TreeNode *node = NULL;

    //  check if directory with given name already exists
    if (find_in_list(folder_list, folderName, &node) != -1)
    {
        printf("mkdir: cannot create directory '%s': File exists\n",
               folderName);
        return;
    }

    //  create new folder node
    TreeNode *new_folder = malloc(sizeof(TreeNode));
    new_folder->content = ll_create();
    new_folder->name = folderName;
    new_folder->parent = currentNode;
    new_folder->type = FOLDER_NODE;

    //  add the new folder into the list of its parent folder
    ll_add_node(folder_list, new_folder);
}

void rmrec(TreeNode *currentNode, char *resourceName)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;

    TreeNode *node = NULL;
    int pos = find_in_list(folder_list, resourceName, &node);
    //  check if document with given name exists
    if (pos == -1)
    {
        printf("rmrec: failed to remove '%s': No such file or directory\n",
               resourceName);
        return;
    }

    //  if document is a file, use the rm command for it
    if (node->type == FILE_NODE) {
        rm(currentNode, resourceName);
    } else {
        //  if document is a folder, recursively remove all
        //  the documents inside of it, using rmrec function
        List *folder_list = (List *)node->content;
        ListNode *curr = folder_list->head;
        while (curr)
        {
            rmrec(node, curr->info->name);
            curr = curr->next;
        }

        //  free memory for the list of the current node
        ll_free(folder_list);

        //  use rmdir function to eliminate the folder from the hierarchy
        rmdir(currentNode, resourceName);
    }
}

void rm(TreeNode *currentNode, char *fileName)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;
    TreeNode *node = NULL;
    int pos = find_in_list(folder_list, fileName, &node);
    //  check if document with given name exists
    if (pos == -1)
    {
        printf("rm: failed to remove '%s': No such file or directory\n",
               fileName);
        return;
    }

    //  if document is a folder, return
    if (node->type == FOLDER_NODE)
    {
        printf("rm: cannot remove '%s': Is a directory\n", fileName);
        return;
    }

    //  eliminate file from its parent's list
    ll_remove_nth_node(folder_list, pos);

    // free memory for the file
    free(node->name);
    free(node);
}

void rmdir(TreeNode *currentNode, char *folderName)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;

    TreeNode *node = NULL;
    int pos = find_in_list(folder_list, folderName, &node);
    //  check if document with given name exists
    if (pos == -1)
    {
        printf("rmdir: failed to remove '%s': No such file or directory\n",
               folderName);
        return;
    }

    //  if document is a file, return
    if (node->type == FILE_NODE)
    {
        printf("rmdir: failed to remove '%s': Not a directory\n", folderName);
        return;
    }
    //  check if the folder is empty. if not, return
    List *inside_folder_list = (List *)node->content;
    if (inside_folder_list->head)
    {
        printf("rmdir: failed to remove '%s': Directory not empty", folderName);
        return;
    }

    //  if the folder is empty, remove it from its parent's list
    ll_remove_nth_node(folder_list, pos);

    //  free memory for the folder (list placeholder, name)
    free(node->content);
    free(node->name);
    free(node);
}

void touch(TreeNode *currentNode, char *fileName, char *fileContent)
{
    //  create a new file by allocating memory for it and
    //  assigning the necessary values to it
    TreeNode *new_node = malloc(sizeof(TreeNode));
    new_node->name = fileName;
    new_node->parent = currentNode;
    new_node->type = FILE_NODE;
    new_node->content = NULL;
    if (fileContent)  //  if content was given from input
        new_node->content = fileContent;

    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;

    //  if there is no file in the current folder with the given filename
    if (find_in_list(folder_list, fileName, &currentNode) == -1)
        ll_add_node(folder_list, new_node);  //  add file to the folder list
}

void cp(TreeNode *currentNode, char *source, char *destination)
{
    //  extract the list of documents for current node
    List *folder_list = (List *)currentNode->content;
    TreeNode *source_node = currentNode;

    //  move to the document that we want to copy
    find_path(currentNode, source, &source_node);

    //  if document to copy is a folder, return
    if (source_node->type == FOLDER_NODE)
    {
        printf("cp: -r not specified; omitting directory '%s'\n", source);
        return;
    }
    //  save destination path for handling errors

    char *dest = malloc(strlen(destination) + 1);
    strcpy(dest, destination);

    //  move to the destination where we want to copy the file
    if (find_path(currentNode, destination, &currentNode) == 0)
    {
        printf("cp: failed to access '%s': Not a directory\n", dest);
        free(dest);
        return;
    }

    //  create a new node that has the same values as
    //  the one that has to be copied
    //  allocate memory for the copied node
    TreeNode *copied_node = malloc(sizeof(TreeNode));

    //  if we are copying into a folder, use the name of the source document
    if (currentNode->type == FOLDER_NODE)
    {
        copied_node->name = malloc(strlen(source_node->name) + 1);
        strcpy(copied_node->name, source_node->name);
    }
    //  if we are copying into a file, use the old name of the file
    else
    {
        copied_node->name = malloc(strlen(currentNode->name) + 1);
        strcpy(copied_node->name, currentNode->name);
        //  change current node, that will soon become
        //  the parent of the copied node
        currentNode = currentNode->parent;
    }
    //  associate parent and type
    copied_node->parent = currentNode;

    copied_node->type = FILE_NODE;

    //  copy content from the source node
    copied_node->content = malloc(strlen((char *)source_node->content) + 1);
    strcpy(copied_node->content, source_node->content);

    //  add the copied node to the list of documents of its parent
    folder_list = (List *)currentNode->content;
    ll_add_node(folder_list, copied_node);
}


void mv(TreeNode *currentNode, char *source, char *destination)
{   //  move to the file that we want to move
    TreeNode *source_node = currentNode;
    find_path(currentNode, source, &source_node);

    //  save destination path for handling errors
    char *dest = malloc(strlen(destination) + 1);
    strcpy(dest, destination);

    //  move to the file where we want to move the document
    TreeNode *dest_node = currentNode;
    if (find_path(currentNode, destination, &dest_node) == 0)
    {   //  if document doesn't exist
        printf("mv: failed to access '%s': Not a directory\n", dest);
        free(dest);
        return;
    }

    //  remove document from its old parent folder
    List *folder_list = (List *)source_node->parent->content;
    int pos = find_in_list(folder_list, source_node->name, &currentNode);
    ll_remove_nth_node(folder_list, pos);

    if (dest_node->type == FOLDER_NODE)
    {   //  add document to its new parent folder
        folder_list = (List *)dest_node->content;
        ll_add_node(folder_list, source_node);
    }
    else
    {   //  if we moved the file to another file, then
        //  we just have to change its content
        dest_node->content = realloc(dest_node->content,
                                     strlen((char *)source_node->content) + 1);
        strcpy(dest_node->content, source_node->content);
    }
}

// strdup implementation because it doesn't work in c99
char *strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = malloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}
