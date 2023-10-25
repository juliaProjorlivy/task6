#include "commands.h"
#include "file_func.h"
#include "assembler.h"
#include "verror.h"
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

static size_t split_line(char *data, char **ptr_data)
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

    char **ptr_data = (char **)calloc(sizeof(char *), max_str_count);
    *str_count = split_line(data, ptr_data);
    ptr_data = (char **)realloc(ptr_data, ((*str_count) + 1) * sizeof(char*));

    return ptr_data;
} 

char *get_data_from_file(const char *filename, size_t *data_size) // collect data from file to the array data
{
    assert(filename != NULL);
    assert(data_size != NULL);

    FILE *file = fopen(filename, "rt");
    if(file == NULL)
    {
        VERROR_FOPEN(filename);
        return NULL;
    }

    struct stat buf;
    if(!stat(filename, &buf))
    {
        *data_size = (size_t)buf.st_size;
        char *data = (char *)calloc(sizeof(char), *data_size + 1); // free data!
        if(data == NULL)
        {
            close_file(file, filename);
            VERROR("memory allocation failure");
            return NULL;
        }

        if(fread(data, sizeof(char), *data_size, file) <= 0)
        {
            close_file(file, filename);
            VERROR("can not read the file");
            return NULL;
        }

        close_file(file, filename);
        return data;
    }

    close_file(file, filename);

    return NULL;
}

int write_file(char *buf, const char *file_name, size_t n_com, size_t i_buf)
{
    FILE *file = fopen(file_name, "wb");
    if(file == NULL)
    {
        VERROR_FOPEN(file_name);
        return 1;
    }

    if(fwrite(&n_com, sizeof(size_t), 1, file) < 1)
    {
        VERROR_FWRITE(file_name);
        return 1;
    }
    
    if(fwrite(buf, sizeof(char), i_buf, file)  < i_buf)
    {
        VERROR_FWRITE(file_name);
        return 1;
    }

    // if(fwrite(all_codes, sizeof(codes), n_com, file)  < n_com)
    // {
    //     VERROR_FWRITE(file_name);
    //     return 1;
    // }

    close_file(file, file_name);
    return 0;
}
