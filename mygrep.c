#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


void grep(FILE* fp, char *pattern, int *flags)
{
    char buf[BUFSIZ];
    char *ret;

    // normal grep searches for the pattern in each line and prints the line
    // if the pattern is found
    while(fgets(buf, sizeof buf, fp) != NULL)
    {   
        char *temp = strdup(buf);

        if(flags[0])
        {
            // -i flag ignores case sensitivity when finding patterns
            int i;
            int buf_len = strlen(buf);
            int pattern_len = strlen(pattern);

            for(i = 0; temp[i]; i++)
                temp[i] = tolower(temp[i]);
        }

        if(flags[1])
        {   
            // -w flag only prints if the line only contains the pattern
            if(strlen(buf) - 1 != strlen(pattern))
                continue;
        }
        
        ret = strstr(temp, pattern);
        if(ret != NULL)
        {
            if(flags[2])
            {   
                // -o flag prints the only the pattern found
                int buf_len = strlen(buf);
                int pattern_len = strlen(pattern);
                int where = buf_len - strlen(ret);
                for(int i = 0; i <= pattern_len; i++)
                {
                    printf("%c", i == pattern_len ? '\n' : buf[where + i]);
                }
            }
            else
                printf("%s", buf);
        }
        
        // free the duplicated string
        free(temp);
    }
}

int main(int argc, char *argv[])
{
    int opt;
    int WhereisPattern = 1;
    char buf[BUFSIZ];
    char* pattern = argv[WhereisPattern];
    FILE *fp = stdin;


    int do_w = 0;
    int do_i = 0;
    int do_o = 0;

    // usage handling
    while((opt = getopt(argc, argv, "iwo")) != -1)
    {
        switch(opt)
        {
        case 'i': do_i = 1; break;
        case 'w': do_w = 1; break;
        case 'o': do_o = 1; break;

        default: exit(EXIT_FAILURE);
        }
    }

    // if there was is a flag make pattern the correct string
    if(do_i || do_w || do_o)
        pattern = argv[++WhereisPattern];

    // if input is not from stdin do open the file
    if(WhereisPattern + 1 != argc)
        fp = fopen(argv[WhereisPattern + 1], "r");


    if(do_i)
    {   
        // if not case sensitive lower case pattern for -i flag
        for(int i = 0; pattern[i]; i++)
            pattern[i] = tolower(pattern[i]);
    }

    int flags[] = {do_i, do_w, do_o};

    grep(fp, pattern, flags);
    // close the file after we are done using it.
    if(fp != stdin)
        fclose(fp);

    return 0;
}
