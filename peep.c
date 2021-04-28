/*-------------------------------------------------------
# Student's Name: Orest Dushkevich, Axel Nguyen, and Omar Mohamed
# Assignment #3 Milestone 3
# Lab Section: X02L and X01l
# CMPUT 201
# Instructor's Name: Nesrine Abbas
*------------------------------------------------------*/

/*
Peep:
Main program which allows interactions with a peep. The program must be able to:
• Create a new peep and its ledger
• Edit an existing peep and append corresponding transactions to the ledger
• Verify the integrity of a peep ledger
• Show a peep at a given timestamp
• Display modification history
What mode it runs in is handled from the command line, this program does not take standard input. For sanity
and practice, consider using getopt for the command line manipulation.
The possible program invocations are ([ ] indicate a parameter):
 
peep -c [fileName] – to create (by invoking the editor) a new peep
peep -e [fileName] – to edit an existing peep
peep -v [fileName] – to verify the integrity of an existing peep
peep -h [fileName] – to print the transaction history of an existing peep
peep -p [fileName] -t [timestamp] –to reproduce a peep at a given timestamp
peep -p [fileName] –to reproduce the current peep 
*/

#include "peep.h"
#include "peepEditor.h"
#include "blockChain.h" 
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/err.h>
int main(int argc, char *argv[])
{
    int c, flag_input;
    int cflag = 0, eflag = 0, vflag = 0, hflag = 0, pflag = 0, tflag = 0; //each flag is set to 1 if respective option encountered

    char *pfile, *timestamp, *filename; //to save argument

    while ((c = getopt(argc, argv, ":c:e:v:h:p:t:")) != -1)
    {
        //colon after option indicates that option should be followed by argument
        //colon before first option to distinguish between missing argument and wrong option
        switch (c)
        {
        case 'c':
            flag_input++;
            cflag = 1;
            filename = optarg;
            break;
        case 'e':
            flag_input++;
            eflag = 1;
            filename = optarg;
            break;
        case 'v':
            flag_input++;
            vflag = 1;
            filename = optarg;
            break;
        case 'h':
            flag_input++;
            hflag = 1;
            filename = optarg;
            break;
        case 'p':
            flag_input++;
            pflag = 1;
            pfile = optarg;
            break;
        case 't':
            flag_input++;
            tflag = 1;
            timestamp = optarg;
            break;
        case ':': //missing argument
            fprintf(stderr, "%s: option '-%c' requires an argument\n", argv[0], optopt);
            printUsage();
            exit(ARG_ERROR);
            break;
        case '?': //incorrect option
            fprintf(stderr, "%s: option '-%c' is invalid: ignored\n", argv[0], optopt);
            printUsage();
            exit(ARG_ERROR);
            break;
        }
    }
    if (flag_input > 1 && pflag == 0)
    {
        printUsage();
        exit(ARG_ERROR);
    }
    if (pflag)
    {
        if (tflag)
            display_peep_at_time(atol(timestamp), pfile);
        else
            display_current_peep(pfile);
    }
    else if (cflag)
    {
        create_new_peep(filename);
    }
    else if (eflag)
    {
        edit_existing_peep(filename);
    }
    else if (vflag)
    {
        verify_integrity(filename);
    }
    else if (hflag)
    {
        print_transaction_history(filename);
    }
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    ERR_remove_state(0);
    EVP_cleanup();
}

void create_new_peep(char *filename)
{
    if(access(filename, F_OK) == 0){
        printf("File already exists. Please use -e <filename> option\n");
        exit(EXIT_FAILURE);
    }
    unsigned int num_transactions;
    // will create a new file and peep
    // make empty peep char * peep = mallco()
    char *peep=malloc(PEEPLEN);
    // pass empty peep into editor(char *peep, Transaction *modBuffer)
    num_transactions = editor(peep, modBuffer);
    
    
    // if a session needs to add more than 64 transactions will continue until a session is less then 64 transactions
    while (num_transactions == MXTXNUM)
    {
        writeBlockChain(filename, modBuffer, num_transactions);
        re_initilaize_modBuffer_to_zero();
        num_transactions = editor(peep, modBuffer);
    }
    // after session is finished
    // use writeBlockChain (char filename,  transactions *modBuffer)
    writeBlockChain(filename, modBuffer, num_transactions);
    free(peep);
}

void edit_existing_peep(char *filename)
{
    check_file_exist(filename);
    char *peep = malloc(PEEPLEN);
    unsigned long max_time = 281474976710654;
    getPeep(peep, filename, max_time);

    // pass empty peep into editor(char *peep, Transaction *modBuffer)
    unsigned int num_transactions = editor(peep, modBuffer);
    // after session is finished
    // use writeBlockChain (char filename,  transactions *modBuffer)
    while (num_transactions == MXTXNUM)
    {
        writeBlockChain(filename, modBuffer, num_transactions);
        re_initilaize_modBuffer_to_zero();
        num_transactions = editor(peep, modBuffer);
    }
    writeBlockChain(filename, modBuffer, num_transactions);
    free(peep);
    
    // if num_transactions < 64 ; break
    //
}

void verify_integrity(char *filename)
{
    // check to see if file exits 
    check_file_exist(filename);

    // calls function that verifies the entire blockchain
    verify_last_Block(filename);
    
}

void print_transaction_history(char *filename)
{
    // check to see if file exits 
    check_file_exist(filename);
    // calls on function that prints all the transactions in the blockchain
    printTransactions(filename);
    
}

void display_peep_at_time(unsigned long input_time, char *filename)
{
    // input verification to avoid any vurnabilities, this number is the maximum value a timestamp can be 
    if (input_time > 281474976710654) input_time = 281474976710654;
    // check to see if file exits 
    check_file_exist(filename);
    
    char *peep = malloc(PEEPLEN);    
    // recreates the peep from the blockchain file
    getPeep(peep, filename, input_time);
    
    // prints the recreated peep
    printf("the peep at Epoch time %lu is: %s\n", input_time, peep);    
    free(peep);
}

void display_current_peep(char *filename)
{
    // check to see if file exits 
    check_file_exist(filename);

    // this number is the maximum vaid value a timestamp can be so all transactions will print
    unsigned long input_time = 281474976710654; 
    char *peep = malloc(PEEPLEN);    
    // recreates the peep from the blockchain file 
    getPeep(peep, filename, input_time);
    printf("the current peep is: %s\n", peep);    
    free(peep);

    
}
void printUsage()
{
    printf("Usage: ./prog [options]\n \
  Options:\n \
peep -c [fileName] - to create (by invoking the editor) a new peep\n \
peep -e [fileName] - to edit an existing peep\n \
peep -v [fileName] - to verify the integrity of an existing peep\n \
peep -h [fileName] - to print the transaction history of an existing peep\n \
peep -p [fileName] -t [timestamp] -to reproduce a peep at a given timestamp\n \
peep -p [fileName] -to reproduce the current peep\n");
    return;
}

void check_file_exist(char * filename) {
    if (access(filename, F_OK) != 0)
    {
        printf("File does not exist. Please use -c <filename> option\n");
        exit(EXIT_FAILURE);
    }
}