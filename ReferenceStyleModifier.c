#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Structure to store the first name and last name of other authors
struct Other_Authors
{
    char *fn;
    char *ln;
};

// Structure to store the information of an MLA citation
struct MLA_Citation
{
    char author_fn[200], author_ln[200], others[200], title[200], journal[200];
    struct Other_Authors other_authors[10];
    int volume, issue, year, pages1, pages2;
    int other_n;
};

// Global variable to store the citation information
struct MLA_Citation citation;

// Function to remove a substring from a string
char *str_remove_sub(char *str, const char *sub)
{
    size_t len = strlen(sub);
    if (len > 0)
    {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL)
        {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

// Function to format the string containing other authors
void format_authors(char *t)
{
    char *token;
    int n = 0;

    // Remove "and" and "," from the string
    t = str_remove_sub(t, "and");
    t = str_remove_sub(t, ",");

    // Tokenize the string using " " as the delimiter
    
    token = strtok(t, " ");

    citation.other_n = 0;

    while (token != NULL)
    
    {
       
        // If n is even, this is the first name of an author
        if (n % 2 == 0)
        {
            citation.other_authors[citation.other_n].fn = token;
        }
        else
        { // Else this is the last name of an author
            citation.other_authors[citation.other_n].ln = token;
            citation.other_n++;
        }

        token = strtok(NULL, " ");
        n++;
    }
}
// Function to extract information from an MLA citation
void extract_info(char *in)
{
    // Parse the MLA citation using sscanf to extract the various pieces of information
    sscanf(in, "%[^,], %[^, ], %[^.]. \"%[^.\"] .\" %[^,], vol. %d, no. %d, %d, pp. %d-%d", citation.author_ln, citation.author_fn, citation.others, citation.title,
           citation.journal, &citation.volume, &citation.issue, &citation.year, &citation.pages1, &citation.pages2);
    // Format the string containing other authors
    format_authors(citation.others);
}

int main()
{
    char mla_reference[1000];

    char **lines = NULL;
    int n = 0;
    // Open MLA-in.txt for reading
    FILE *fp = fopen("MLA-in.txt", "r");

    // Open output files for writing
    FILE *fp2 = fopen("AMA-Output.txt", "w");
    FILE *fp3 = fopen("APA-Output.txt", "w");
    FILE *fp4 = fopen("Chicago-Output.txt", "w");
    FILE *fp5 = fopen("Vancouver-Output.txt", "w");
    FILE *fp6 = fopen("IEEE-Output.txt", "w");

    // Read the MLA citations from the input file, line by line
    while (fgets(mla_reference, sizeof(mla_reference), fp) != NULL)
    {
        if (strcmp(mla_reference, "\n") == 0 || strcmp(mla_reference, "\r\n") == 0)
        {
            continue;
        }
        // Allocate memory for each line of the input file
        lines = (char **)realloc(lines, (n + 1) * sizeof(char *));

        lines[n] = (char *)malloc((strlen(mla_reference) + 1) * sizeof(char));

        strcpy(lines[n], mla_reference);

        n++;
    }

    fclose(fp);

    fprintf(fp2, "AMA citation format:\n\n");

    // Iterate over each MLA citation
    for (int i = 0; i < n; i++)
    {
        
        // Extract information from the citation
        extract_info(lines[i]);
        char authors_part[100];

        for (int j = 0; j < citation.other_n; j++)
        {
            
            char authors_part2[100];
            if (strcmp(citation.other_authors[j].fn, "et") == 0)
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].fn, citation.other_authors[j].ln);
            }
            else
            {
                sprintf(authors_part, "%s %c", citation.other_authors[j].ln, citation.other_authors[j].fn[0]);
            }
            if (j != citation.other_n - 1)
            {
                strcat(authors_part, ",");
            }
            strcat(authors_part, authors_part2);
        }
    

        fprintf(fp2, "%s %c, %s. %s. %s. %d;%d(%d);%d-%d.\n", citation.author_ln, citation.author_fn[0], authors_part, citation.title,
                citation.journal, citation.year, citation.volume, citation.issue, citation.pages1, citation.pages2);
        fprintf(fp2, "\n");
    }

    fclose(fp2);

    fprintf(fp3, "APA citation format:\n\n");

    for (int i = 0; i < n; i++)
    {
        extract_info(lines[i]);
        char authors_part[25];

        for (int j = 0; j < citation.other_n; j++)
        {
            char authors_part2[25];
            if (strcmp(citation.other_authors[j].fn, "et") == 0)
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].fn, citation.other_authors[j].ln);
            }
            else
            {
                sprintf(authors_part, "%s %c", citation.other_authors[j].ln, citation.other_authors[j].fn[0]);
            }
            if (j < citation.other_n - 1)
            {
                strcat(authors_part, "&");
            }
            strcat(authors_part, authors_part2);
        }

        fprintf(fp3, "%s %c., & %s.(%d). %s. %s, %d(%d) %d-%d.\n", citation.author_ln, citation.author_fn[0], authors_part, citation.year,
                citation.title, citation.journal, citation.volume, citation.issue, citation.pages1, citation.pages2);
        fprintf(fp3, "\n");
    }

    fclose(fp3);

    fprintf(fp4, "Chicago citation format:\n\n");

    for (int i = 0; i < n; i++)
    {
        extract_info(lines[i]);
        char authors_part[200];

        for (int j = 0; j < citation.other_n; j++)
        {
            char authors_part2[200];

            printf("j %d,  n %d", j, citation.other_n);

            if (strcmp(citation.other_authors[j].fn, "et") == 0)
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].fn, citation.other_authors[j].ln);
            }
            else
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].ln, citation.other_authors[j].fn);
            }

            strcat(authors_part, authors_part2);
        }

        fprintf(fp4, "%s, %s, and %s.\"%s.\" %s %d, no. %d (%d) %d-%d.\n", citation.author_ln, citation.author_fn, authors_part, citation.title,
                citation.journal, citation.volume, citation.issue, citation.year, citation.pages1, citation.pages2);
        fprintf(fp4, "\n");
    }

    fclose(fp4);

    fprintf(fp5, "Vancouver citation format:\n\n");

    for (int i = 0; i < n; i++)
    {
        extract_info(lines[i]);
        char authors_part[200];

        for (int j = 0; j < citation.other_n; j++)
        {
            char authors_part2[200];
            if (strcmp(citation.other_authors[j].fn, "et") == 0)
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].fn, citation.other_authors[j].ln);
            }
            else
            {
                sprintf(authors_part, "%s %c", citation.other_authors[j].ln, citation.other_authors[j].fn[0]);
            }
            if (j < citation.other_n - 1)
            {
                strcat(authors_part, "&");
            }
            strcat(authors_part, authors_part2);
        }

        fprintf(fp5, "%d. %s %c, %s. %s. %s. %d;%d(%d);%d-%d.\n", i + 1, citation.author_ln, citation.author_fn[0], authors_part, citation.title,
                citation.journal, citation.year, citation.volume, citation.issue, citation.pages1, citation.pages2);
        fprintf(fp5, "\n");
    }

    fclose(fp5);

    fprintf(fp6, "IEEE citation format:\n\n");

    for (int i = 0; i < n; i++)
    {
        extract_info(lines[i]);
        char authors_part[200];

        for (int j = 0; j < citation.other_n; j++)
        {
            char authors_part2[200];
            if (strcmp(citation.other_authors[j].fn, "et") == 0)
            {
                sprintf(authors_part, "%s %s", citation.other_authors[j].fn, citation.other_authors[j].ln);
            }
            else
            {
                sprintf(authors_part, "%s %c", citation.other_authors[j].fn, citation.other_authors[j].ln[0]);
            }
            if (j < citation.other_n - 1)
            {
                strcat(authors_part, "&");
            }
            strcat(authors_part, authors_part2);
        }

        fprintf(fp6, "[%d] %s %c, and %s. \"%s.\", %s, vol %d, no. %d, pp. %d-%d, %d.\n", i + 1, citation.author_ln, citation.author_fn[0], authors_part, citation.title,
                citation.journal, citation.volume, citation.issue, citation.pages1, citation.pages2, citation.year);
        fprintf(fp6, "\n");
    }

    fclose(fp6);

    return 0;
}