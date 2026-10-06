#include<stdio.h>
#include<string.h>
#include"header.h"

int main (int argc, char* argv[])
{
    tag Tag;
    
    if(argc<2)
    {
        printf("Insufficient arguments to run the program\n");
        printf("\tH E L P     M E N U\n");
        return 0;
    }
    if(check_operation(argv[1])==e_view)
    {
        printf("View operation being performed \n");

        Tag.operationflag = e_view;

        Status validate_input(argv, &Tag);

        Status open_files(&Tag);

        Status view_operation(&Tag);
    }
    else if(check_operation(argv[1])==e_edit)
    {
        if(argc<2)
        {
        printf("Insufficient arguments to run the program\n");
        printf("\tH E L P     M E N U\n");
        return 0;
        }
        Tag.operationflag = e_edit;

        
        printf("Performing modifying operation\n");
    }
    else
    {
        printf("Unsupported operation\n");
        printf("\tH E L P     M E N U\n");
        return 0;
    }

}
