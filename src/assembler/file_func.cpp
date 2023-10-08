// #include "assembler.h"
// #include "commands.h"
// #include "verror.h"
// #include <stdlib.h>

// ssize_t getline(char **line, size_t *n, FILE *stream) // gets line from the file and returns the number of characters
// {
//     if(*line == NULL)
//     {
//         *line = (char *)calloc(*n, sizeof(char));
//     }

//     ssize_t i = 0;
//     char c = fgetc(stream);
//     const int multiplayer = 2;
//     while(c != EOF)
//     {
//         (*line)[i] = c;
//         if(c == '\n')
//         {
//             break;
//         }
//         i++;

//         if(i > *n)
//         {
//             *n *= multiplayer;
//             *line = (char *)realloc(*line, *n);
//         }
//         c = fgetc(stream);
//     }
//     if(c == EOF)
//     {
//         return -1;
//     }

//     return i;
// }

// int file_into_lines(const char *file_name, const char *bc_file_name, struct codes *all_codes, size_t n_lines, char **lines)
// {
//     size_t max_fline_len = 30;

//     FILE *file = fopen(file_name, "rt");  // open file to conver it to binary-code file
//     if(file == NULL)
//     {
//         VERROR_FOPEN(file_name);
//         return 1;
//     }

//     FILE *bc_file = fopen(bc_file_name, "w");
//     if(bc_file == NULL)
//     {
//         VERROR_FOPEN(bc_file_name);
//         close_file(file, file_name);
//         return 1;
//     }

//     char **line = (char **)calloc(sizeof(char*), max_fline_len); // free!
//     if(line == NULL)
//     {
//         VERROR("memory allocation problem");
//         return 1;
//     }

//     size_t cur_line = 0;

//     while(getline(line, &max_fline_len, file) != -1)
//     {
//         *(lines + cur_line) = *line;
//         cur_line++;
//     }

//     close_file(file, file_name);
//     close_file(bc_file, bc_file_name);
//     free(line);

//     return 0;
// }

#include "verror.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

size_t split_line(char *data, char **ptr_data)
{
    assert(data != NULL);
    assert(ptr_data != NULL);

    size_t str_count = 0;
    size_t i = 0;
    size_t data_size = strlen(data);
    size_t strlen = 0; // the lenght of each string

    for(; i < data_size; i++)
    {
        strlen++;
        if(data[i] == '\n')
        {
            data[i] = '\0';

            if(strlen > 1) // if it is not an empty line
            {
                data[i] = '\0';
                *(ptr_data + str_count) = data + i - strlen + 1;
                str_count++;
            }
            strlen = 0;
        }
    }

    return str_count;
}

char **make_ptr_array(char *data, size_t *str_count) // returns the number of strings in data
{
    assert(data != NULL);
    assert(str_count != NULL);

    const size_t max_str_count = 200;

    char **ptr_data = (char **)calloc(sizeof(char **), max_str_count);
    *str_count = split_line(data, ptr_data);
    ptr_data = (char **)realloc(ptr_data, ((*str_count) + 1) * sizeof(char*));

    return ptr_data;
} 

char *get_data_from_file(const char *filename, size_t *data_size) // collect data from file to the array data
{
    assert(filename != NULL);
    assert(data_size != NULL);

    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        VERROR_FOPEN(filename);
        return NULL;
    }

    struct stat buf;
    if(!stat(filename, &buf))
    {
        *data_size = buf.st_size;
        char *data = (char *)calloc(sizeof(char), *data_size + 1); // free data!
        if(data == NULL)
        {
            fclose(file);
            VERROR("memory allocation failure");
            return NULL;
        }

        if(!fread(data, sizeof(char), *data_size, file))
        {
            fclose(file);
            VERROR("can not read the file");
            return NULL;
        }

        fclose(file);
        return data;
    }

    fclose(file);
    VERROR("unable to stat");
    return NULL;
}

