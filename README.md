**Dumitru Bianca Stefania & Nioata Alexandra**
**id vmhecker Alexandra: alexandra.nioata**
**312CA**

## Homework #3 - Data Structures - Linux File System

## General description and notes:

- The program is meant to operate with a tree-like structure, that 
    stores documents in a file system hierarchically. All of the
    documents are represented in memory as tree nodes, making access
    to any element fast and easy.

- All the elements in the file system are dinamically alocated in memory,
using malloc(), realloc() and free() functions from the stdlib.h library.

- The root of the file system is created at the beginning of the program.

## Commands:
- There are 13 possible commands that can be recieved:

0) createFileTree
    - allocate memory for the root of the file tree and assign its values

1) touch <filename> [filecontent]
    - create a new tree node by allocating memory and assigning
        the specific file values to the node, including its name <filename>
    - if [filecontent] is given, also assign the content
    - insert the new file into its parent's list of documents

2) mkdir <dirname>
    - create a new tree node by allocating memory and assigning
        the specific folder values to the node, including its name <dirname>
    - insert the new folder into its parent's list of documents

3) ls [arg]
    - for a folder [arg] or no [arg], display all the documents
        present in the folder's list of documents, by traversing the list
    - for a file [arg], display the file's content

4) pwd
    - display the absolute path of the current working directory, by
        recursively going toward the root and printing the folders

5) cd <path>
    - change the pointer of the current directory to the directory
        given by path

6) tree [path]
    - use as start the directory given by [path] or the current
        directory if no path is given
    - from the starting directory, display all the documents stored 
        inside the folder and recursively, inside all the documents
        until reaching the end of every directory
    - display them in a tree like structure

7) rm <filename>
    - remove the file from its parent's list of documents
    - free memory for the file node and its content

8) rmdir <dirname>
    - remove the directory from its parent's list of documents
    - free memory for the directory node and its list placeholder

9) rmrec <resourcename>
    - use prevoiusly declared rm and rmdir functions to recursively
        remove every file or folder starting from the one given
        by <resourcename>

10) cp <source_path> <destination_path>
    - in the folder <destination_path>, add a new element in its list of
        documents, containing the information present in the file given
        by <source_path>

11) mv <source_path> <destination_path>
    - remove file given by <source_path> from its parent's list of documents
    - if <destination_path> is a folder, add a new element in its 
        list of documents, containing the information present in 
        the file given by <source_path>
    -  if <destination_path> is a file, change its content to the one
        of <source_path>

12) freeTree
    - from the file system root, recursively eliminate all the documents
        in the file system using rmrec function declared before
    - free memory for the root