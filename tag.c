#include<stdio.h>
#include<string.h>
#include"header.h"

OperationType check_operation(char* argv)
{
    if(strcmp(argv,"-v")==0)
    {
        return e_view;
    }
    else if(strcmp(argv,"-m")==0)
    {
        return e_edit;
    }
    else
    {
        return e_unsupported ;
    }
}

Status validate_input(char**argv, tag* Tag)
{



    char input_mp3_filename[50];  strcpy(input_mp3_filename,argv[4]); 
    char input_mp3_extention[5]; input_mp3_extention[0] = '.' ;input_mp3_extention[1] = '\0';  strcat(input_mp3_extention,strtok(input_mp3_filename,".")) ;
    if(strcmp(input_mp3_extention,".mp3")==0)
    {
        printf("INFO : Input file Validation Successful\n");
        strcpy(Tag->input_mp3_filename,input_mp3_filename);
        printf("INFO : Proceeding with file - %s\n",Tag->input_mp3_filename);

    }
    else
    {
        printf("ERROR : Input file extention is not .mp3\n");
        return e_failure ; 
    }
    
    return e_success; 
}

Status open_files(tag* Tag)
{
    Tag->input_mp3_fptr = fopen(Tag->input_mp3_filename,"r");
    if(Tag->input_mp3_fptr == NULL)
    {
        printf("ERROR : Failed to open the file -%s \n",Tag->input_mp3_filename);
        return e_failure;
    }

    return e_success;

}

Status view_operation(tag* Tag)
{
    char ch ='\0';
    char mp3_signature_buffer[7];
    int i=0;
    while(i!=6)
    {
        ch = getc(Tag->input_mp3_fptr);
         mp3_signature_buffer[i] = ch;
        i++;
    }
    if((strcmp(mp3_signature_buffer,"ID323")==0) || (strcmp(mp3_signature_buffer,"ID324")==0))
    {
        printf("INFO : Input file signature verified as %s\n",mp3_signature_buffer);
    }


}

