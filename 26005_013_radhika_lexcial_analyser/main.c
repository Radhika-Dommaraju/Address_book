#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN 256

int check_keyword(const char *str)
{
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "inline", "int", "long",
        "register", "restrict", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union", "unsigned",
        "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary"
    };

    size_t count = sizeof(keywords) / sizeof(keywords[0]);
    size_t i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

// Check the first character of an identifier 
int is_identifier_start(int ch)
{
    return isalpha((unsigned char)ch) || ch == '_';
}

// Check the remaining characters of an identifier 
int is_identifier_part(int ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

// Check whether a character can be part of an operator
int is_operator_char(int ch)
{
    return strchr("+-*/%=<>!&|^~?:", ch) != NULL;
}

//Check brackets, braces, commas, semicolons and similar symbols 
int is_punctuator(int ch)
{
    return strchr("(){}[];,.", ch) != NULL;
}

// Display one token and its category 
void print_token(const char *lexeme, const char *type)
{
    printf("%-24s : %s\n", lexeme, type);
}


void read_file(FILE *fp)
{
    int ch;
    char token[MAX_TOKEN];
    size_t i;

    while ((ch = fgetc(fp)) != EOF)
    {
        /* Ignore whitespace */
        if (isspace((unsigned char)ch))
            continue;

        /* Handle comments and the division operator */
        if (ch == '/')
        {
            int next = fgetc(fp);

            if (next == '/')
            {
                while ((ch = fgetc(fp)) != EOF && ch != '\n')
                {
                    // single-line comment 
                }
                continue;
            }

            if (next == '*')
            {
                int previous = 0;
                int closed = 0;

                while ((ch = fgetc(fp)) != EOF)
                {
                    if (previous == '*' && ch == '/')
                    {
                        closed = 1;
                        break;
                    }
                    previous = ch;
                }

                if (!closed)
                    print_token("/*", "Lexical error: unterminated comment");

                continue;
            }

            if (next != EOF)
                ungetc(next, fp);

            print_token("/", "Arithmetic operator");
            continue;
        }

        //keyword and identifier
        if (is_identifier_start(ch))
        {
            i = 0;
            token[i++] = (char)ch;

            while ((ch = fgetc(fp)) != EOF && is_identifier_part(ch))
            {
                if (i < MAX_TOKEN - 1)
                    token[i++] = (char)ch;
            }

            if (ch != EOF)
                ungetc(ch, fp);

            token[i] = '\0';

            if (check_keyword(token))
                print_token(token, "Keyword");
            else
                print_token(token, "Identifier");

            continue;
        }

        //int decimal float char
        if (isdigit((unsigned char)ch))
        {
            int has_dot = 0;
            i = 0;
            token[i++] = (char)ch;

            while ((ch = fgetc(fp)) != EOF &&
                   (isdigit((unsigned char)ch) ||
                    (ch == '.' && !has_dot)))
            {
                if (ch == '.')
                    has_dot = 1;

                if (i < MAX_TOKEN - 1)
                    token[i++] = (char)ch;
            }

            if (ch != EOF)
                ungetc(ch, fp);

            token[i] = '\0';

            if (has_dot)
                print_token(token, "Floating-point constant");
            else
                print_token(token, "Integer constant");

            continue;
        }

        //Identify string literals 
        if (ch == '"')
        {
            int closed = 0;
            i = 0;
            token[i++] = '"';

            while ((ch = fgetc(fp)) != EOF)
            {
                if (i < MAX_TOKEN - 1)
                    token[i++] = (char)ch;

                if (ch == '\\')
                {
                    int escaped = fgetc(fp);

                    if (escaped == EOF)
                        break;

                    if (i < MAX_TOKEN - 1)
                        token[i++] = (char)escaped;
                }
                else if (ch == '"')
                {
                    closed = 1;
                    break;
                }
                else if (ch == '\n')
                {
                    break;
                }
            }

            token[i] = '\0';

            if (closed)
                print_token(token, "String literal");
            else
                print_token(token, "Lexical error: unterminated string");

            continue;
        }

        /* Identify character constants */
        if (ch == '\'')
        {
            int closed = 0;
            i = 0;
            token[i++] = '\'';

            while ((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                if (i < MAX_TOKEN - 1)
                    token[i++] = (char)ch;

                if (ch == '\\')
                {
                    int escaped = fgetc(fp);

                    if (escaped == EOF)
                        break;

                    if (i < MAX_TOKEN - 1)
                        token[i++] = (char)escaped;
                }
                else if (ch == '\'')
                {
                    closed = 1;
                    break;
                }
            }

            token[i] = '\0';

            if (closed)
                print_token(token, "Character constant");
            else
                print_token(token, "Lexical error: unterminated character constant");

            continue;
        }

        /* Identify single, double and selected triple-character operators */
        if (is_operator_char(ch))
        {
            int next;
            int pair = 0;

            i = 0;
            token[i++] = (char)ch;
            next = fgetc(fp);

            if (next != EOF)
            {
                if ((ch == '+' && (next == '+' || next == '=')) ||
                    (ch == '-' && (next == '-' || next == '=' || next == '>')) ||
                    ((ch == '=' || ch == '!' || ch == '<' || ch == '>') && next == '=') ||
                    (ch == '&' && next == '&') ||
                    (ch == '|' && next == '|') ||
                    ((ch == '*' || ch == '%' || ch == '^') && next == '=') ||
                    (ch == '<' && next == '<') ||
                    (ch == '>' && next == '>'))
                {
                    pair = 1;
                }

                if (pair)
                {
                    token[i++] = (char)next;

                    if ((ch == '<' && next == '<') ||
                        (ch == '>' && next == '>'))
                    {
                        int third = fgetc(fp);

                        if (third == '=')
                            token[i++] = (char)third;
                        else if (third != EOF)
                            ungetc(third, fp);
                    }
                }
                else
                {
                    ungetc(next, fp);
                }
            }

            token[i] = '\0';

            if (strcmp(token, "=") == 0)
                print_token(token, "Assignment operator");
            else
                print_token(token, "Operator");

            continue;
        }

        /* Identify punctuation and special characters */
        if (is_punctuator(ch))
        {
            char one[2] = {(char)ch, '\0'};
            print_token(one, "Special character");
            continue;
        }

        /* Report unrecognized characters */
        {
            char one[2] = {(char)ch, '\0'};
            print_token(one, "Lexical error: unrecognized character");
        }
    }
}

int main(void)
{
    FILE *fp = fopen("input.c", "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open input.c\n");
        printf("Make sure input.c is in the program's working directory.\n");
        return 1;
    }

    printf("LEXICAL ANALYSER OUTPUT\n");
    printf("-----------------------\n");

    read_file(fp);

    fclose(fp);

    return 0;
}