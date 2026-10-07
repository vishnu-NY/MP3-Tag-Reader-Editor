#include<stdio.h>
#include<string.h>
#include"header.h"

int main(int argc, char *argv[])
{
    tag Tag;
    Status ret;

   
    if (argc < 2)
    {
        printf("ERROR : Insufficient arguments to run the program\n");
        display_help_menu();
        return 0;
    }

    OperationType operation = check_operation(argv[1]);


    if (operation == e_view)
    {
        /* ./a.out -v song.mp3 */
        if (argc != 3)
        {
            printf("ERROR : Invalid number of arguments for VIEW operation\n");
            display_help_menu();
            return 0;
        }

        printf("INFO : View operation being performed\n");

        Tag.operationflag = e_view;

        ret = validate_input(argv, &Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Input validation failed\n");
            display_help_menu();
            return 0;
        }

        ret = open_files(&Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Failed to open required files\n");
            return 0;
        }

        ret = view_operation(&Tag);

        if (ret == e_failure)
        {
            printf("ERROR : View operation failed\n");
            return 0;
        }

        printf("INFO : View operation completed successfully\n");
    }

    /*--------------- MODIFY OPERATION ---------------------- */

    else if (operation == e_edit)
    {
        /* ./a.out -m Album "New Album" song.mp3 */
        if (argc != 5)
        {
            printf("ERROR : Invalid number of arguments for MODIFY operation\n");
            display_help_menu();
            return 0;
        }

        printf("INFO : Performing modify operation\n");

        Tag.operationflag = e_edit;

        ret = validate_input(argv, &Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Input validation failed\n");
            display_help_menu();
            return 0;
        }

        ret = open_files(&Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Failed to open required files\n");
            return 0;
        }

        ret = edit_operation(&Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Modify operation failed\n");
            return 0;
        }

        ret = rename_and_delete(&Tag);

        if (ret == e_failure)
        {
            printf("ERROR : Failed while replacing the original file\n");
            return 0;
        }

        printf("INFO : Modify operation completed successfully\n");
    }

    /* -------------------------------------------------- */
    /*              UNSUPPORTED OPERATION                 */
    /* -------------------------------------------------- */

    else
    {
        printf("ERROR : Unsupported operation '%s'\n", argv[1]);
        display_help_menu();
        return 0;
    }

    return 0;
}
