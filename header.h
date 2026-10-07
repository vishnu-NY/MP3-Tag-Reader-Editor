#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_view,
    e_edit,
    e_unsupported
} OperationType;

typedef enum
{
    p_Artist,
    p_Title,
    p_Album,
    p_Year,
    p_Content,
    p_Composer
}parameter;

#endif


#ifndef MP3_H
#define MP3H



typedef struct{

    int operationflag;  

    char input_mp3_filename[50];
    FILE* input_mp3_fptr;
    char input_mp3_fileext[5];
    char mp3_file_version[6];
    int mp3_file_size;

    /*------EDIT PARAMETERS-------*/

    char edit_parameter[4];
    char newdata[100];

} tag;

OperationType check_operation(char* argv);

parameter check_parameter(char* argv);

Status validate_input(char**argv, tag * Tag);

Status open_files(tag* Tag);

Status view_operation(tag* Tag);

int big_to_little_endian(char* buffer);
int little_to_big_endian(int val);




#endif
