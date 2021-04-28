/*-------------------------------------------------------
# Student's Name: Orest Dushkevich, Axel Nguyen, and Omar Mohamed
# Assignment #3 Milestone 3
# Lab Section: X02L and X01l
# CMPUT 201
# Instructor's Name: Nesrine Abbas
*------------------------------------------------------*/
#ifndef PEEP_H
#define PEEP_H
#include <unistd.h>
#include <getopt.h>
#define ARG_ERROR 2

/*
 void create_new_peep(char *filename);
 Description: peep -c [fileName] - to create (by invoking the editor) a new peep
    creates a new blockchain at the specified filename, will save the the transaction to the blockchain after a session.
    the program will loop if the user is entering more than 64 transactions 
 PARAMS: char *filename
 RETURN: none
 PRE: none
 POST: creates a new blockchian file after the peep editor session is ended
 */
void create_new_peep(char *filename);

/*
 void re_initilaize_modBuffer_to_zero(void);
 Description: if a session lasts more than 64 sessions the program will need clear all of the transactions in the 
    modBuffer to an initialized state so that no erroneous transactions make it into the blockchain
 PARAMS: none
 RETURN: none
 PRE: none
 POST: modBuffer is reinitialized to a 0 state
 */
void re_initilaize_modBuffer_to_zero(void);

/*
 void edit_existing_peep(char *filename);
 Description: peep -e [fileName] - peep -e [fileName] – to edit an existing peep 
    calculates the curent peep from a blockchain at the specified filename, will then start a new editior session
    with the current peep and will apend the newest blocks to the existing file 
    the program will loop if the user is entering more than 64 transactions 
 PARAMS: char *filename
 RETURN: none
 PRE: none
 POST: creates a new blockchian file after the peep editor session is ended
 */
void edit_existing_peep(char *filename);

/*
 void verify_integrity(char *filename);
 Description: peep -v [fileName] – to verify the integrity of an existing peep 
    this function iterates through all the blocks and entire file and recalculates all of the hases to ensure that they
    match what is written in th eblockchain so that the entire blockchain integtirty can be verified at any time
 PARAMS: char *filename
 RETURN: none
 PRE: none
 POST: prints a statement to inform the user about the integrety of the block
 */
void verify_integrity(char *filename);

/*
 void print_transaction_history(char *filename);
 Description: prints all of the valid transactions in the blockchain
 PARAMS: char *filename
 RETURN: none
 PRE: requires a valid filename with a blockchain
 POST: prints a list of transactions in the blockchain
 */
void print_transaction_history(char *filename);

/*
 void display_peep_at_time(unsigned long input_time, char *filename);
 Description: creates the peep from a blockchain file at a specified time and will recreate that peep up to 
    and including the specified time 
 PARAMS: char * peep, Transaction * modbuffer, numTransactions
 RETURN: none
 PRE: needs an input time that has an epoch of 1/1/21 00:00 and a valid filename
 POST: prints the current peep
 */
void display_peep_at_time(unsigned long input_time, char *filename);

/*
 void display_peep_at_time( char *filename);
 Description: creates the current peep from a blockchain file 
 PARAMS: char * peep, Transaction * modbuffer, numTransactions
 RETURN: none
 PRE: needs an input time that has an epoch of 1/1/21 00:00 and a valid filename
 POST: prints the current peep
 */
void display_current_peep(char *filename);

/*
 void printUsage();
 Description: when called prints all the usage options of this program 
 PARAMS: none
 RETURN: none
 PRE: is called when when wrong or no argument is entered
 POST: none
 */
void printUsage();

/*
 void check_file_exist(char * filename);
 Description: checks to see if the user entered fileexists, if file does not exist exits program
 PARAMS: char * filename
 RETURN: none
 PRE: user inputed filename is passed into function
 POST: program continues if file exists otherwise causes program to exit
 */
void check_file_exist(char * filename);
#endif