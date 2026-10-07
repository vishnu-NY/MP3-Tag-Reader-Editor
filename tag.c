#include<stdio.h>
#include<string.h>
#include"header.h"

char* tags[]={"TIT2","TPE1","TALB","TYER","TCOM","TCON","COMM",NULL};

/*----------------------------------------------H E L P E R     F U N C T I O N S-----------------------------------*/


void display_help_menu(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("              MP3 TAG VIEWER / EDITOR - HELP MENU           \n");
    printf("============================================================\n");

    printf("\nAVAILABLE OPERATIONS:\n");
    printf("------------------------------------------------------------\n");
    printf("1. VIEW TAG INFORMATION\n");
    printf("2. MODIFY TAG INFORMATION\n");

    printf("\n------------------------------------------------------------\n");
    printf("VIEW OPERATION\n");
    printf("------------------------------------------------------------\n");
    printf("Syntax:\n");
    printf("  ./a.out -v <song_name.mp3>\n");

    printf("\nExample:\n");
    printf("  ./a.out -v sunny_sunny.mp3\n");

    printf("\nDescription:\n");
    printf("  Displays the available tag information of the MP3 file.\n");

    printf("\n------------------------------------------------------------\n");
    printf("MODIFY OPERATION\n");
    printf("------------------------------------------------------------\n");
    printf("Syntax:\n");
    printf("  ./a.out -m <parameter> <new_data> <song_name.mp3>\n");

    printf("\nExample:\n");
    printf("  ./a.out -m Album \"New Album\" sunny_sunny.mp3\n");

    printf("\nSupported parameters:\n");
    printf("  Album\n");
    printf("  Artist\n");
    printf("  Year\n");
    printf("  Title\n");
    printf("  Content\n");
    printf("  Comments\n");

    printf("\nNOTE:\n");
    printf("  Parameter names are CASE INSENSITIVE.\n");
    printf("  Example: Album, ALBUM, album and aLbUm are all valid.\n");
    printf("  However, the spelling must be correct.\n");

    printf("\nExamples:\n");
    printf("  ./a.out -m Artist \"Honey Singh\" sunny_sunny.mp3\n");
    printf("  ./a.out -m Year \"2014\" sunny_sunny.mp3\n");
    printf("  ./a.out -m Title \"Sunny Sunny\" sunny_sunny.mp3\n");
    printf("  ./a.out -m Comments \"My favourite song\" sunny_sunny.mp3\n");

    printf("\nIMPORTANT:\n");
    printf("  If the new data contains spaces, enclose it in double quotes.\n");
    printf("  Example:\n");
    printf("  ./a.out -m Album \"Best Party Songs\" sunny_sunny.mp3\n");

    printf("\n============================================================\n");
    printf("                    END OF HELP MENU                        \n");
    printf("============================================================\n\n");

}


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

int synchsafe_to_int(char *buffer)
{
    int size;

    size = ((buffer[0] & 0x7F) << 21) |
           ((buffer[1] & 0x7F) << 14) |
           ((buffer[2] & 0x7F) << 7)  |
           (buffer[3] & 0x7F);

    return size;
}


/* Convert int into 4 synchsafe bytes */
void int_to_synchsafe(int size, char *buffer)
{
    buffer[0] = (size >> 21) & 0x7F;
    buffer[1] = (size >> 14) & 0x7F;
    buffer[2] = (size >> 7)  & 0x7F;
    buffer[3] = size & 0x7F;
}

char* get_parameter_to_edit(char*argv)
{
    if(strcasecmp(argv,"ALBUM")==0)
    {
        return "TALB" ;
    }
    else if(strcasecmp(argv,"Artist")==0)
    {
        return "TPE1";
    }
    else if(strcasecmp(argv,"Title")==0)
    {
        return "TIT2";
    }
    else if(strcasecmp(argv,"Year")==0)
    {
        return "TYER";
    }
    else if(strcasecmp(argv,"Content")==0)
    {
        return "TCON";
    }
    else if(strcasecmp(argv,"Comments")==0)   
    {
        return "COMM";
    }
    else
    {
        return NULL;
    }
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
        /*-----------------------------------Validating the tag to edit------------------------------------------*/
        char* ptr = get_parameter_to_edit(argv[2]);
        if(ptr==NULL)
        {
            printf("The Parameter choosen to Edit is Wrong / Unsupported\n");
            return e_failure;
        }
        char parameter_to_edit[6]; strcpy(parameter_to_edit,(get_parameter_to_edit(argv[2])));
        int parameterflag = 0;
        for(int k=0;tags[k]!=NULL;k++)
        {
            if(strcmp(parameter_to_edit,tags[k])==0)
            {
                parameterflag=1;
                break;
            }
        }
        if(parameterflag)
        {
            
            strcpy(Tag->edit_parameter,parameter_to_edit);
            printf("Parameter to edit is %s",Tag->edit_parameter);

        }
        else
        {
            printf("Parameter to edit - unsupported\n");
            return e_failure;
        }
        
        /*----------------------------------------------------------*/
        
        /*--------------------checking new data and storing its size in struct---------------------------*/

            Tag->size_of_newdata = strlen(argv[3]);
            strcpy(Tag->newdata,argv[3]); 

            printf("\nThe size of new data is %d and it is %s\n",Tag->size_of_newdata,Tag->newdata);

        /*--------------------validating input mp3 file for edit operation--------------------------------------------*/
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
    //printf("INFO : File size is taken -  %d\n",Tag->mp3_file_size);

    fseek(Tag->input_mp3_fptr,10,SEEK_SET);

    /*----------------------------------------------------- P O I N T E R S --------------------------------------------------*/
    int count =0; 
    int tagflag=0;
    printf("\n---------------------------------------------------------------\n");
    printf("Sl.No.\tTag\tTag Data\n");
    printf("---------------------------------------------------------------\n");
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
       // printf("INFO : Tag is %s\t",tag_buffer); //TESTING
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
            printf("%d\t%s\t%s\n",count+1,tag_buffer,tag_data);
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
    printf("---------------------------------------------------------------\n");


}
/*
Status edit_operation(tag* Tag)
{
    fseek(Tag->input_mp3_fptr,0,SEEK_SET);
    Tag->temp_mp3 = fopen("temp.mp3","wb");
    char ch;
    int i = 0;
    while(i!=10)
    {
        ch = getc(Tag->input_mp3_fptr);  //Copying Header AS IS
        putc(ch,Tag->temp_mp3);
        i++;
    }

    int che;

    int j=0;
    int count =0;
    while(count<6)
    { 
    char current_tag[6];
    i=0;
    while(i!=4)
    {
        ch = getc(Tag->input_mp3_fptr);
        current_tag[i] = ch;
        i++;
    }
    current_tag[i] = '\0';

    for(int k=0;tags[k]!=NULL;k++)
        {
            if(strcmp(current_tag,tags[k])==0)
            {
                count++;
                break;
            }
        }

    printf("Current tag is %s\n",current_tag); // Testing
        printf("Comparing %s with %s\n",current_tag,Tag->edit_parameter);
    if(strcmp(current_tag,(Tag->edit_parameter))==0)
    {
        i=0;
        while(i!=4)
        {
            ch=current_tag[i] ;
            putc(ch,Tag->temp_mp3);  //writing tag to new file.
            i++;
        }
        char old_size[4];
        fread(old_size,1,4,Tag->input_mp3_fptr);
        int old_size_int = big_to_little_endian(old_size);
        

        int size = little_to_big_endian(Tag->size_of_newdata);
        fwrite(&size,sizeof(int),1,Tag->temp_mp3);
        fseek(Tag->input_mp3_fptr,4,SEEK_CUR);

        i=0;
        while(i!=2)
        {
            ch = getc(Tag->input_mp3_fptr);      //copy flag as is
            putc(ch,Tag->temp_mp3);
            i++;  
        }
        putc('\0',Tag->temp_mp3);
        i=0;
        while(i<=(Tag->size_of_newdata))
        {
           ch = Tag->newdata[i];
           putc(ch,Tag->temp_mp3);
            i++;
        }
        fseek(Tag->input_mp3_fptr,old_size_int+1,SEEK_CUR);
        fseek(Tag->temp_mp3,1,SEEK_CUR);
    }
    else
    {
        i=0;
        while(i!=4)
        {
         ch = current_tag[i];
         putc(ch,Tag->temp_mp3);
         i++;
        }

        char size_of_unwanted_data[4];
        i=0;
            for(i=0;i<4;i++)
            {
                 ch = getc(Tag->input_mp3_fptr);
                size_of_unwanted_data[i] = ch;
                putc(ch,Tag->temp_mp3);
            }

            int tag_size = big_to_little_endian(size_of_unwanted_data);
            //fwrite(&tag_size,sizeof(int),1,Tag->temp_mp3);
            
            i=0;
            for(i=0;i<(tag_size+2);i++)
            {
                ch = getc(Tag->input_mp3_fptr);
                putc(ch,Tag->temp_mp3);

            }

    }
    }

}*/

Status edit_operation(tag* Tag)
{
    fseek(Tag->input_mp3_fptr, 0, SEEK_SET);

    Tag->temp_mp3 = fopen("temp.mp3", "wb");

    if(Tag->temp_mp3 == NULL)
    {
        printf("ERROR : Unable to create temp.mp3\n");
        return e_failure;
    }


    int ch;
    int i = 0;

    char header[10];


   /*first 10 bytes*/

    while(i != 10)
    {
        ch = getc(Tag->input_mp3_fptr);

        if(ch == EOF)
        {
            printf("ERROR : Invalid MP3 file\n");
            return e_failure;
        }

        header[i] = ch;

        putc(ch, Tag->temp_mp3);

        i++;
    }



    int old_tag_size = synchsafe_to_int(&header[6]); // only for size in heder

    int tag_end = 10 + old_tag_size;


    int difference = 0;
    int found = 0;


    while(ftell(Tag->input_mp3_fptr) < tag_end)
    {
        char current_tag[5];


        /* ------------------------------------------------
           READ 4 BYTE FRAME ID
           ------------------------------------------------ */

        i = 0;

        while(i != 4)
        {
            ch = getc(Tag->input_mp3_fptr);

            if(ch == EOF)
                break;

            current_tag[i] = ch;

            i++;
        }

        current_tag[4] = '\0';


        if(i != 4)
            break;


        /* ------------------------------------------------
           CHECK FOR PADDING

           Padding starts with 00 00 00 00...
           ------------------------------------------------ */

        if(current_tag[0] == '\0')
        {
            /*
               We already read 4 padding bytes.

               Write those 4 bytes.
            */

            for(i = 0; i < 4; i++)
            {
                putc(current_tag[i],
                     Tag->temp_mp3);
            }


            /*
               Copy remaining padding until original
               ID3 tag ends.
            */

            while(ftell(Tag->input_mp3_fptr) < tag_end)
            {
                ch = getc(Tag->input_mp3_fptr);

                if(ch == EOF)
                    break;

                putc(ch, Tag->temp_mp3);
            }


            break;
        }


        /* =================================================
           TARGET TAG FOUND
           ================================================= */

        if(strcmp(current_tag,
                  Tag->edit_parameter) == 0)
        {
            found = 1;


            /* ---------------------------------------------
               WRITE FRAME ID
               --------------------------------------------- */

            i = 0;

            while(i != 4)
            {
                ch = current_tag[i];

                putc(ch,
                     Tag->temp_mp3);

                i++;
            }


            /* ---------------------------------------------
               READ OLD FRAME SIZE
               --------------------------------------------- */

            char old_size[4];

            fread(old_size,
                  1,
                  4,
                  Tag->input_mp3_fptr);


            int old_size_int =
                big_to_little_endian(old_size);


            /* ---------------------------------------------
               READ THE 2 FLAG BYTES

               IMPORTANT:

               After reading old_size, input pointer is
               pointing at FLAGS.

               So read flags BEFORE skipping old data.
               --------------------------------------------- */

            char flag[2];

            flag[0] =
                getc(Tag->input_mp3_fptr);

            flag[1] =
                getc(Tag->input_mp3_fptr);


            /* ---------------------------------------------
               CALCULATE NEW FRAME SIZE

               Your new data contains:

               1 encoding byte
               +
               actual text

               Therefore +1.
               --------------------------------------------- */

            int new_size =
                Tag->size_of_newdata + 1;


            /*
               Convert new frame size into
               big endian.
            */

            int size =
                little_to_big_endian(new_size);


            /* Write new size */

            fwrite(&size,
                   sizeof(int),
                   1,
                   Tag->temp_mp3);


            /* ---------------------------------------------
               COPY FLAGS
               --------------------------------------------- */

            putc(flag[0],
                 Tag->temp_mp3);

            putc(flag[1],
                 Tag->temp_mp3);


            /* ---------------------------------------------
               WRITE ENCODING BYTE
               --------------------------------------------- */

            putc('\0',
                 Tag->temp_mp3);


            /* ---------------------------------------------
               WRITE NEW DATA
               --------------------------------------------- */

            i = 0;

            while(i < Tag->size_of_newdata)
            {
                ch = Tag->newdata[i];

                putc(ch,
                     Tag->temp_mp3);

                i++;
            }


            /* ---------------------------------------------
               SKIP OLD FRAME DATA

               Input pointer currently:

               ID
               SIZE
               FLAGS
                    ↓
                 OLD DATA

               So skip exactly old_size_int bytes.
               --------------------------------------------- */

            fseek(Tag->input_mp3_fptr,
                  old_size_int,
                  SEEK_CUR);


            /*
               Find change in overall ID3 size.
            */

            difference =
                new_size - old_size_int;


            printf("INFO : Tag %s edited\n",
                   current_tag);

            printf("Old size = %d\n",
                   old_size_int);

            printf("New size = %d\n",
                   new_size);
        }


        /* =================================================
           NOT THE TAG WE WANT TO EDIT
           ================================================= */

        else
        {
            /* ---------------------------------------------
               WRITE FRAME ID
               --------------------------------------------- */

            i = 0;

            while(i != 4)
            {
                ch = current_tag[i];

                putc(ch,
                     Tag->temp_mp3);

                i++;
            }


            /* ---------------------------------------------
               READ AND COPY ORIGINAL FRAME SIZE
               --------------------------------------------- */

            char size_of_unwanted_data[4];

            i = 0;

            while(i != 4)
            {
                ch =
                    getc(Tag->input_mp3_fptr);

                size_of_unwanted_data[i] =
                    ch;


                /*
                   IMPORTANT:

                   Write original size bytes directly.

                   Don't convert and fwrite again.
                */

                putc(ch,
                     Tag->temp_mp3);

                i++;
            }


            /*
               Convert only so that our program knows
               how many bytes to copy.
            */

            int tag_size =
                big_to_little_endian(
                    size_of_unwanted_data);


            /* ---------------------------------------------
               COPY:

               2 flag bytes
               +
               tag_size bytes of actual frame data
               --------------------------------------------- */

            i = 0;

            while(i < (tag_size + 2))
            {
                ch =
                    getc(Tag->input_mp3_fptr);

                if(ch == EOF)
                    break;


                putc(ch,
                     Tag->temp_mp3);

                i++;
            }
        }
    }


    /* ===================================================
       CHECK WHETHER TARGET WAS ACTUALLY FOUND
       =================================================== */

    if(found == 0)
    {
        printf("ERROR : Tag %s not found\n",
               Tag->edit_parameter);

        fclose(Tag->temp_mp3);

        remove("temp.mp3");

        return e_failure;
    }


    /* ===================================================
       COPY ACTUAL MP3 AUDIO

       We have completed ID3 frames/padding.

       Everything after tag_end is MP3 audio.
       =================================================== */

    fseek(Tag->input_mp3_fptr,
          tag_end,
          SEEK_SET);


    while((ch = getc(Tag->input_mp3_fptr)) != EOF)
    {
        putc(ch,
             Tag->temp_mp3);
    }


    /* ===================================================
       UPDATE MAIN ID3 HEADER SIZE
       =================================================== */

    int new_tag_size =
        old_tag_size + difference;


    char new_tag_size_array[4];


    /*
       Main ID3 header size is SYNCHSAFE.
    */

    int_to_synchsafe(new_tag_size,
                     new_tag_size_array);


    /*
       Bytes 6-9 contain tag size.
    */

    fseek(Tag->temp_mp3,
          6,
          SEEK_SET);


    fwrite(new_tag_size_array,
           1,
           4,
           Tag->temp_mp3);


    fflush(Tag->temp_mp3);


    printf("\nINFO : Editing completed successfully\n");

   

    return e_success;
}

Status rename_and_delete(tag* Tag)
{
    if(remove(Tag->input_mp3_filename))
    {
        return e_failure;
    }
    if(rename("temp.mp3",Tag->input_mp3_filename))
    {
        return e_failure;
    }

}