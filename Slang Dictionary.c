#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    struct Node *children[26];
    bool endOfWord;
    char description[100];
};

struct Node* createNode() {
    struct Node *curr = (struct Node*)malloc(sizeof(struct Node));
    if (!curr) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    curr->endOfWord = false;
    strcpy(curr->description, "");
    for (int i = 0; i < 26; i++)
        curr->children[i] = NULL;
    return curr;
}

char toLowerCase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

struct Node* insert(struct Node* root, char word[], char description[]) {
    if (!root) root = createNode();
    struct Node* curr = root;
    for (int i = 0; word[i]; i++) {
        char c = toLowerCase(word[i]);
        int index = c - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createNode();
        }
        curr = curr->children[index];
    }
    curr->endOfWord = true;
    strncpy(curr->description, description, 99);
    curr->description[99] = '\0';
    return root;
}

struct Node* searchExactWord(struct Node* root, char word[]) {
    if (!root) return NULL;
    struct Node* curr = root;
    for (int i = 0; word[i]; i++) {
        char c = toLowerCase(word[i]);
        int index = c - 'a';
        if (index < 0 || index >= 26) return NULL;
        if (!curr->children[index]) return NULL;
        curr = curr->children[index];
    }
    return (curr && curr->endOfWord) ? curr : NULL;
}

struct Node* searchPrefixNode(struct Node* root, char prefix[]) {
    if (!root) return NULL;
    struct Node* curr = root;
    for (int i = 0; prefix[i]; i++) {
        char c = toLowerCase(prefix[i]);
        int index = c - 'a';
        if (index < 0 || index >= 26) return NULL;
        if (!curr->children[index]) return NULL;
        curr = curr->children[index];
    }
    return curr;
}

void printAllWordsFromNodeWithNumber(struct Node* curr, char buffer[], int depth, int* count) {
    if (curr->endOfWord) {
        buffer[depth] = '\0';
        (*count)++;
        printf("%d. %s\n", *count, buffer);
    }
    for (int i = 0; i < 26; i++) {
        if (curr->children[i]) {
            buffer[depth] = 'a' + i;
            printAllWordsFromNodeWithNumber(curr->children[i], buffer, depth + 1, count);
        }
    }
}

void printAllWordsWithPrefix(struct Node* root, char prefix[]) {
    struct Node* prefixNode = searchPrefixNode(root, prefix);
    if (!prefixNode) {
        printf("There is no prefix '%s' in the dictionary.\n", prefix);
        return;
    }
    printf("Words starts with '%s':\n", prefix);
    char buffer[100];
    strcpy(buffer, prefix);
    int count = 0;
    printAllWordsFromNodeWithNumber(prefixNode, buffer, strlen(prefix), &count);
}

void printAllWords(struct Node* root) {
    if (!root) {
        printf("There is no slang word yet in the dictionary.\n");
        return;
    }
    printf("List of all slang words in the dictionary:\n");
    char buffer[100];
    int count = 0;
    printAllWordsFromNodeWithNumber(root, buffer, 0, &count);
}


void menu() {
    printf("Welcome to Boogle Application\n");
    printf("1. Release a new slang word\n");
    printf("2. Search a slang word\n");
    printf("3. View all slang words starting with a certain prefix word\n");
    printf("4. View all slang words\n");
    printf("5. Exit\n");
}

int main() {
	
    struct Node* root = NULL;
    int choice;
    char slang[100], description[100];

    do {
        menu();
        printf("Choose your selection: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
   
            continue;
        }



        if (choice < 1 || choice > 5) {
            printf("Please input a valid choice\n");
            continue;
        }

        if (choice == 1) {
            int len, isspace;
            do {
                len = 0;
                isspace = 0;
                printf("Input a slang word to be searched [Must be more than 1 character and contains no space]: ");
                scanf(" %[^\n]", slang);
                getchar();
                len = strlen(slang);
                for (int i = 0; i < len; i++) {
                    char c = slang[i];
                    if (c == ' ') {
                        isspace = 1;
                        break;
                    }
                }
            } while (len <= 1 || isspace);

            int word_count, desc_len, desc_space;
            do {
                desc_len = 0;
                desc_space = 0;
                printf("Input a new slang word description [Must be more than 2 words]: ");
                scanf(" %[^\n]", description);
                getchar();
                desc_len = strlen(description);
                for (int i = 0; i < desc_len; i++) {
                    if (description[i] == ' ') {
                        desc_space++;
                    }
                }
                char temp[100];
                strcpy(temp, description);
                char *token = strtok(temp, " ");
                word_count = 0;
                while (token) {
                    word_count++;
                    token = strtok(NULL, " ");
                }
            } while (word_count <= 2);

            struct Node* search = searchExactWord(root, slang);
            if (!search) {
                root = insert(root, slang, description);
                printf("Successfully released new slang word.\n");
            } else {
                strncpy(search->description, description, 99);
                search->description[99] = '\0';
                printf("Successfully updated a slang word.\n");
            }

            printf("Press enter to continue...");
            getchar();
        } else if (choice == 2) {
            int len, isspace;
            do {
                len = 0;
                isspace = 0;
                printf("Input a slang word to search [Must be more than 1 character and contains no space]: ");
                scanf(" %[^\n]", slang);
                getchar();
                len = strlen(slang);
        
                for (int i = 0; i < len; i++) {
                    char c = slang[i];
                    if (c == ' ') {
                        isspace = 1;
                        break;
                    }
                }
            } while (len <= 1 || isspace == 1);
            struct Node* result = searchExactWord(root, slang);
            if (result) {
                printf("Slang word: %s\nDescription: %s\n", slang, result->description);
            } else {
                printf("There is no word '%s' in the dictionary.\n", slang);
            }
            printf("Press enter to continue...");
            getchar();
        } else if (choice == 3) {
            int len, isspace;
            do {
                len = 0;
                isspace = 0;
                printf("Input a prefix to be searched: ");
                scanf(" %[^\n]", slang);
                getchar();
                len = strlen(slang);
                for (int i = 0; i < len; i++) {
                    char c = slang[i];
                    if (c == ' ') {
                        isspace = 1;
                        break;
                    }
                }
            } while (len < 1 || isspace == 1); 
            printAllWordsWithPrefix(root, slang);
            printf("Press enter to continue...");
            getchar();
        } else if (choice == 4) {
            printAllWords(root);
            printf("Press enter to continue...");
            getchar();
        } else if (choice == 5) {
            printf("Thank you... Have a nice day :)\n");
        }
    } while (choice != 5);

    
    return 0;
}
