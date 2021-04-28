/*-------------------------------------------------------
# Student's Name: Orest Dushkevich, Axel Nguyen, and Omar Mohamed
# Assignment #3 Milestone 3
# Lab Section: X02L and X01l
# CMPUT 201
# Instructor's Name: Nesrine Abbas
*------------------------------------------------------*/

//DEPENDENCIES
#ifndef PEEPEDITOR_H
#define PEEPEDITOR_H
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "blockChain.h"

//=======================================================================
//FUNCTIONS' PROTOTYPES

/*
 editor(char *peep, Transaction *modBuffer);
 Description: edits the peep, saves transactions in modBuffer, and returns the number of transactions written in the buffer
 and is part of the user interface that modifies a peep durring a session
 PARAMS: char *peep, Transaction *modBuffer
 RETURN: the amount of changes that were made to the peep
 PRE: none
 POST: none*/
unsigned int editor(char *peep, Transaction *modBuffer);

/*
 menu_options(void)
 Print out the program's menu and provide choices. Return the choice user made. Also validate input.
 PARAMS: none
 RETURN: the choice that user made
 PRE: none
 POST: none
*/
int menu_options(void);
/*
 view_characters(char*)
 Description: prints the peep to stdout, it prints
 all characters in the peep until the loop hits a null terminator
 PARAMS: peep char array
 RETURN: void
 PRE: needs a peep char array
 POST: no modification done to the array it just prints to stdout*/
void view_characters(char *);
/*
 insert_character(char *, Transaction *, int);
 Description: 
 PARAMS: char * peep, Transaction * modbuffer, numTransactions
 RETURN: num_transaction
 PRE: needs the user to enter a position between 0-256 and a ASCI character between 32-127
 POST: inserts the character in the desired position. */
int insert_character(char *, Transaction *, int);
/*
 delete_character(char *, Transaction *, int)
 Prompt for index to delete. Delete a character at the given position.
 PARAMS: char * peep, Transaction * modbuffer, numTransactions
 RETURN: the one-incremented numTransactions if input is valid, else numTransactions
 PRE: none
 POST: none
*/
int delete_character(char *, Transaction *, int);
/*
 save_exit(Transaction *modBuffer, int num_transactions);
 Description: fills in all the empty transactions of modBuffer with invalid timestamps and initialized Modifications
    so that the blockchain has no eroneous data in the empty transactions
 PARAMS: Transaction *modBuffer, int num_transactions
 RETURN: none
 PRE: need the number of valid transactions in the modbuffer
 POST: fills in the Time_stamp of the the un initialized transactions
 */
void save_exit(Transaction *modBuffer, int num_transactions);
/*
 create_transaction(unsigned short, char, unsigned short);
 Description: this function is a helper function that creates a transaction, it also calls on create_Time_stamp to
 create the transaction 
 PARAMS: unsigned short, char, unsigned short
 RETURN: Transaction struct
 PRE: needs the position in the peep, the caracter and and the type of peep modification occuring
 POST: returns a transaction that can be be inserted into a transaction array
 */
Transaction create_transaction(unsigned short, char, unsigned short);

/*
 *load_peep_from_file(FILE *filename);
 Description:  THE PLAN IS TO BUILD A FUNCTION THAT WILL READ A FILE AND PROBABLY USE THE delete_character() AND insert_character();
 FUNCTIONS TO BUILD THE PEEP 
 PARAMS: none
 RETURN: the choice that user made
 PRE: none
 POST: none*/
// use getPeep and readBlockChain
char *load_peep_from_file(FILE *filename);

#endif