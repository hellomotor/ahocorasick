#include <stdio.h>
#include <stdlib.h>
#include "aho-corasick.h"
#include <string.h>

int
keyword_search(aho_corasick_t* tree,
        unsigned char *queryString,
        int length,
        int startpos,
        aho_corasick_state_t *state,
        aho_corasick_state_t *last_state,
        size_t *start,
        size_t *end,
        ahocorasick_KeywordTree_search_helper_t helper)
{
    aho_corasick_state_t *initial_state = state;
    if (!initial_state) 
        initial_state = tree->zerostate;

    return (*helper)(initial_state, 
            queryString, length,
            (size_t) startpos,
            start, end, &last_state);
}


int main(int argc, char *argv[])
{
    int retcode = 0;
	aho_corasick_t* tree;

    tree = (aho_corasick_t *)malloc(sizeof(aho_corasick_t));
    if (!tree) {
        fprintf(stderr, "Out of memory\n");
        retcode = 1;
        goto END;
    }
    if (aho_corasick_init(tree) == -1) {
        fprintf(stderr, "aho_corasick_init error\n");
        retcode = 2;
        goto END;
    }
    {
        FILE *fs = fopen("keywords", "r");
        if (!fs) {
            fprintf(stderr, "open keywords file error\n");
            retcode = 3;
            goto END;
        }
        char string[100];
        while (!feof(fs)) {
            if (fgets(string, 100, fs)) {
                string[strlen(string) -1] = '\0';
                printf("%s\n", string);
                if (aho_corasick_addstring(tree, (unsigned char *)string, strlen(string)) == -1) {
                    break;
                }
            }
        }
        fclose(fs);
        if (aho_corasick_maketree(tree) != 0) {
            retcode = 4;
            goto END;
        }
        char queryString[1024] = { 0 };
        while (1) {
            printf("query=> ");
            if (fgets(queryString, 1024, stdin)) {
                int start, end;
                int startpos = 0;
                aho_corasick_state_t *init_state = NULL;
                aho_corasick_state_t *last_state = NULL;
                while (keyword_search(tree, queryString, strlen(queryString), 
                            startpos, init_state, last_state, &start, &end, 
                            ahocorasick_KeywordTree_search_helper)) {
                    printf("match: [%d - %d]", start, end);
                    init_state = last_state;
                    startpos = end;
                }
            }
        }
    }
END:
    if (tree) {
        free(tree);
    }

    return retcode;
}
