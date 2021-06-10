/*-------------------------------------------------------
# Student's Name: Orest Dushkevich, Axel Nguyen, and Omar Mohamed

*------------------------------------------------------*/

#include "peepEditor.h"


/*
A3M2 main function code, we used for testing
int main(void)
//use as template, you might need to modify this or add more code
{

    char *peep = malloc(PEEPLEN); //create a peep
    strcpy(peep, "hello");
    int num = editor(peep, modBuffer);
    free(peep);
    printf("number of transactions: %d\n", num);
    return 0;
}
*/

unsigned int editor(char *peep, Transaction *modBuffer)
{
    int choice, num_transactions = 0;
    for (;;)
    {
        choice = menu_options();
        if (choice == 1)
            view_characters(peep);
        else if (choice == 2)
            num_transactions = insert_character(peep, modBuffer, num_transactions);
        else if (choice == 3)
            num_transactions = delete_character(peep, modBuffer, num_transactions);
        else if (choice == 4)
            {
                save_exit(modBuffer, num_transactions);
                break;
            }
        if (num_transactions == 64)
            return num_transactions;
    }
    return num_transactions;
}

// Print out the menu then return the user choice of the menu selection
int menu_options(void)
{
    for (;;)
    {
        int choice;
        printf("PEEP EDITOR MENU\n");
        printf("Choose an Option\n");
        printf("1. View Characters\n");
        printf("2. Insert\n");
        printf("3. Delete\n");
        printf("4. Save and Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        if (choice > 0 && choice <= 4)
            return choice;
        else
            continue;
    }
}

void view_characters(char *peep)
{
    // iterates though peep untill either PEEPLEN is hit or '/0'

    int i = 0, j = 1;

    while (i < PEEPLEN && *(peep + i) != '\0')
    {

        if ((int)*(peep + i) >= 32 && (int)*(peep + i) <= 127)
        {
            //checks if characters are within ASCI valid 32-127
            printf("%d:%c    ", j, *(peep + i));
            i++;
            j++;
        }
        if (i % 10 == 0)
            printf("\n");
    }
    printf("\n");
}

int insert_character(char *peep, Transaction *modBuffer, int num_transactions)
{
    // insert a new charicter into peep, add to transaction list, num_transactions++
    //calls view_characters

    //***error checking***//
    if (strlen(peep) == PEEPLEN ) 
    {
        printf("peep is full, can't add more characters\n");
        return num_transactions;

    }
    unsigned short position;
    printf("Position to insert: ");
    scanf(" %hu", &position);
    if (position <= 0 || position > PEEPLEN)
    {
        printf("%hu is an invalid position\n", position);
        return num_transactions;
    }

    char character;
    printf("Character to insert: ");
    scanf(" %c", &character);

    if ((int)character < 32 || (int)character > 127)
    {
        printf("%c character is not valid\n", character);
        return num_transactions;
    }

    /********insertion**********/
    if(strlen(peep) == 0){
        peep[0] = character;
        peep[1] = '\0';
        printf("Peep becomes\n");
        view_characters(peep);
        modBuffer[num_transactions] = create_transaction(1, character, 0);
        return num_transactions+1;
    }
    // modBuffer[num_transactions] = create_transaction(position, character, 0); // can't be here
    int nullTermPos = 0;
    //stores position of the null terminator
    while (nullTermPos < PEEPLEN && *(peep + nullTermPos) != '\0')
    {
        nullTermPos++;
    }

    //if position is after null terminator, need to adjust position to be im array
    if (nullTermPos < position - 1)
    {
        printf("invalid position entered\n");
        return num_transactions;
    }
    
    else if ( nullTermPos == position-1){
       *(peep + position-1)=character;
       *(peep + nullTermPos + 1) = '\0';
    }
    //if position is an empty string fill the position with a character
    /* old way 
    else if (*(peep + position - 1) == ' ')
    {
        *(peep + position - 1) = character;
    }
    */
    //else the desired position is before the null terminator.
    //This means we must shift the array position.
    else
    {
        //shifts every element after the desired position 1 to the right
        memcpy(&peep[position], &peep[position - 1], nullTermPos - position + 1);
        *(peep + position - 1) = character;
        *(peep + nullTermPos + 1) = '\0';
    }
    modBuffer[num_transactions] = create_transaction(position, character, 0); //moved here
    printf("Peep becomes\n");
    view_characters(peep);
    return num_transactions + 1;
}

int delete_character(char *peep, Transaction *modBuffer, int num_transactions)
{
    if (strlen(peep) == 0)
    {
        printf("There is nothing to delete\n");
        return num_transactions;
    }
    unsigned short pos; //position of the to-be-deleted char
    //print current char:
    view_characters(peep);

    //Prompting:
    printf("Position to delete: ");
    scanf("%hu", &pos);

    //If pos is valid
    //Delete a  character from peep, add to transaction list
    if (pos >= 1 && pos <= strlen(peep))
    {
        modBuffer[num_transactions] = create_transaction(pos, peep[pos - 1], 1);
        memmove(&peep[pos - 1], &peep[pos], strlen(peep) - pos + 1);
        //calls view_characters
        printf("Peep becomes\n");
        view_characters(peep);
        //num_transactions++
        return num_transactions + 1;
    }
    else
        return num_transactions;
}

void save_exit(Transaction *modBuffer, int num_transactions)
{
    Time_stamp invalid_time;
    invalid_time.first = 0xFFFF;
    invalid_time.mid = 0xFFFF;
    invalid_time.last = 0xFFFF;
    
    Modification invalid_mod;
    invalid_mod.position=0;
    invalid_mod.character=0;
    invalid_mod.event=0;

    for (int i = num_transactions; i < MXTXNUM; i++)
    {
        modBuffer[i].time = invalid_time;
        modBuffer[i].mod = invalid_mod;
    }
}

Transaction create_transaction(unsigned short position, char character, unsigned short event_type)
{

    Transaction new_transaction;
    new_transaction.time = create_Time_stamp(); // create a timestamp

    Modification mod;
    mod.position = position;
    mod.character = character;
    mod.event = event_type;

    new_transaction.mod = mod;
    return new_transaction;
}
