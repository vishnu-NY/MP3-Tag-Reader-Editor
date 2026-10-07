#include<stdio.h>
#include<string.h>
#include"header.h"

char* tags[]={"TIT2","TPE1","TALB","TYER","TCOM","TCON","COMM",NULL};

/*----------------------------------------------H E L P E R     F U N C T I O N S-----------------------------------*/

   int big_to_little_endian(char* buffer)
{
    // Combine the 4 bytes explicitly from Big Endian layout
    unsigned int res = ((unsigned char)buffer[0] << 24) |
                       ((unsigned char)buffer[1] << 16) |
                       ((unsigned char)buffer[2] << 8)  |
                       ((unsigned char)buffer[3]);
                       
    return (int)res;
}


int little_to_big_endian(int val)
{
  return ((val & 0x000000FF) << 24) |
           ((val & 0x0000FF00) << 8)  |
           ((val & 0x00FF0000) >> 8)  |
           ((val & 0xFF000000) >> 24);  
}



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
    if(strcmp(argv[1],"-v")==0) /*This is view operation and only 2 parameters are passed here*/
    {
            char input_mp3_fileraw[50];  strcpy(input_mp3_fileraw,(strtok(argv[2],"."))); 
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


    }
    else if(strcmp(argv[1],"-m")==0)
    {
        char parameter_to_edit[6]; strcpy(parameter_to_edit,(get_parameter_to_edit(argv[2]))

    

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
    fseek(Tag->input_mp3_fptr,6,SEEK_SET);
    char mp3_filesize_buffer[5];
    i=0;
    while(i!=4)
    {
        ch = getc(Tag->input_mp3_fptr);
       mp3_filesize_buffer[i]=ch; 
       i++;
    }
    int size = 0;
    size = big_to_little_endian(mp3_filesize_buffer);
    Tag->mp3_file_size = size;
    printf("INFO : File size is taken -  %d\n",Tag->mp3_file_size);

    fseek(Tag->input_mp3_fptr,10,SEEK_SET);

    /*----------------------------------------------------- P O I N T E R S --------------------------------------------------*/
    int count =0; 
    int tagflag=0;
    while(count!=6)  // to find all 6 tags
    {
        char tag_buffer[5];
        int j;
        for(j=0;j<4;j++)
        {
            char ch = getc(Tag->input_mp3_fptr);
            tag_buffer[j]=ch;
        }
        tag_buffer[j] = '\0';
        int tagflag=0;
        printf("INFO : Tag found is %s\n",tag_buffer); //TESTING
        for(int k=0;tags[k]!=NULL;k++)
        {
            if(strcmp(tag_buffer,tags[k])==0)
            {
                tagflag=1;
                break;
            }
        }
        if(tagflag)
        {
            int tag_size;
            char tag_size_buffer[4];
            for(int i=0;i<4;i++)
            {
                char ch = getc(Tag->input_mp3_fptr);
                tag_size_buffer[i] = ch;
            }
            tag_size = big_to_little_endian(tag_size_buffer);
            char tag_data[tag_size + 1];
            fseek(Tag->input_mp3_fptr,3,SEEK_CUR);
            for(int i = 0;i<tag_size;i++)
            {
                ch = getc(Tag->input_mp3_fptr);
                tag_data[i] = ch;
            }
            ungetc(ch,Tag->input_mp3_fptr);
            tag_data[tag_size-1] = '\0';
            printf("TAG %d is %s\n",count+1,tag_data);
        }
        else
        {
            int tag_size;
            char tag_size_buffer[4];
            for(int i=0;i<4;i++)
            {
                char ch = getc(Tag->input_mp3_fptr);
                tag_size_buffer[i] = ch;
            }
            tag_size = big_to_little_endian(tag_size_buffer);
            fseek(Tag->input_mp3_fptr,(tag_size+2),SEEK_CUR);

            //continue;
            
        }
        count++;
        tagflag=0;
    }


}

Status edit_operation(tag* Tag)
{


}
