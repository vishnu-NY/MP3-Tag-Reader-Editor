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



    char input_mp3_fileraw[50];  strcpy(input_mp3_fileraw,(strtok(argv[4],"."))); 
    char input_mp3_extention[5]; input_mp3_extention[0] = '.' ;input_mp3_extention[1] = '\0';  strcat(input_mp3_extention,(strtok(NULL,"."))) ;
    char input_mp3_filename[55]; strcpy(input_mp3_filename,(strcat(input_mp3_fileraw,input_mp3_extention)));
    if(strcmp(input_mp3_extention,".mp3")==0)
    {
        printf("INFO : Input file Validation Successful\n");
        strcpy(Tag->input_mp3_filename,input_mp3_filename);
        printf("INFO : Proceeding with file - %s\n",Tag->input_mp3_filename);

    }
    else
    {
        printf("ERROR : Input file extention is not .mp3 it is %s\n",input_mp3_extention);
        printf("Wrong file name %s\n",argv[4]);
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
    /*-----------------------------------------------------------W A R N I N G -------------------------------------------------------*/
    /*------------------------------------POINTERS-------------------------------------------------------------------------------------*/
    char ch ='\0';
    char mp3_signature_buffer[7];
    int i=0;
    while(i!=3)
    {
        ch = getc(Tag->input_mp3_fptr);
         mp3_signature_buffer[i] = ch;
        i++;
    }
    mp3_signature_buffer[i]='\0';
    if((strcmp(mp3_signature_buffer,"ID3")==0))
    {
        printf("INFO : Input file signature verified as %s\n",mp3_signature_buffer);
    }


}

