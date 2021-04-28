#ifndef BLOCKCHAIN
#define BLOCKCHAIN
// The maximum number of transactions allowed in one block.
#define MXTXNUM 64
#define PEEPLEN 256

//DEPENDENCIES:
#include <time.h>
#include <stdio.h>
#include <string.h>

/*
DATA STRUCTURES

Structure to Represent a modification [2 bytes]. (modification)
	position	- Position in the peep that the event occured: Maximum position of 256
	character	- The character that was modified  - ASCII codes 32 to 127
     event_type	- Type of event that has occured.
				  0 for Insert
				  1 for Delete
NOTE: Bitfields must be used to condense data in memory and when written


Structure to Represent a Transaction [8 bytes] (transaction) 
	timestamp	- Time that the event occurred (EPOCH time since January 1-st 2021				). [6 bytes]
	modification	- The modification part of the transaction.                          [2 bytes]

Structure to Represent a Block  [32 bytes for the  head + up to 8*64 bytes for transactions] (block)
	previousHash	- Hash of the Previous block head. 0 if first          [7 bytes]
	timeStamp		- Time that the hashing of the block occured           [6 bytes]
	blockNum		- Incremental number of the block.                     [4 bytes]
	blockSize		- Size of transaction elements in the block            [1 byte]
	blockHash		- The Merkle tree hash of the transactions             [7 bytes]
                                
                                
	headHash		- Hash of the head elements above                      [7 bytes]
      
	transactions	- Transactions contained in the block (maximum of 64 transactions)

NOTE: a hash of length 7 bytes is obtained by taking the last 7 bytes of the sha256 hash 
      The truncation is to be applied only as the final step. 
      All intermediary hashes are full sha256 hashes
*/

typedef struct
{                                 // find out if we can initialize
    unsigned short position : 8;  // 8 bit 256 positions
    unsigned short character : 7; // first 7 bits character,
    unsigned short event : 1;     //last bit event_type 0 for Insert 1 for Delete
} Modification;

typedef struct
{
    // the 6 byte number gets stored over three 2 byte shorts
    unsigned short first, mid, last;
} Time_stamp;

typedef struct
{
    Time_stamp time;  // bit field of second since Jan 1, 2021 00:00 == today - epcoch time on that date
    Modification mod; //The modification part of the transaction.
} Transaction;

typedef struct
{
    char hash[7];
} Hash_key;

typedef struct
{
    Hash_key previousHash;             //Hash of the Previous block head. 0 if first
    Time_stamp time;                   // bit field of second since Jan 1, 2021 00:00 == today - epcoch time on that date
    unsigned char blockSize;           //Size of transaction elements in the block
    int blockNum;                      //count of blocks
    Hash_key blockHash;                //The Merkle tree hash of the transactions
    Hash_key headHash;                 //Hash of the head elements above
    Transaction transactions[MXTXNUM]; //Transactions contained in the block ( transactions)
} Block;

// ########################################################################################################################## #
//GLOBAL VARIABLE:
Transaction modBuffer[MXTXNUM];
// this is initialized an array where all bits are 1 will be easier to code a fixed array size

// ########################################################################################################################## #
//Functions
void writeBlockChain(char * filename, Transaction *modBuffer, unsigned short num_transactions);
    // opens a file, creats a new one if does not exist
    // use readBlockChain to get the previous block hash and the block counter
    // create_new_block(Transaction *modbuffer);
    // creates the first block if does not exist or appends a new block

Block create_new_block(Transaction *modbuffer, Hash_key previous_hash, unsigned short num_transactions, int previous_block_num);

    // helper for writeBlockChain to create a block
    
Block recreate_new_block(Block);

Block readBlockChain(char *filename); // - reads a block chain from a file
    // opens a file, and returns the last block in that file

Hash_key Merkle_tree_hash(Transaction *modBuffer);

//builds a merkle tree and returns the hash of the root
Hash_key create_headHash(Block *newBlock);

//creates the hash of the head of block

void verifyBlockChain(char *filename); //- verifies the integrity of the block chain
    // use  readBlockChain to get the last block
    // extracts Hash_key previousHash;    //Hash of the Previous block head. 0 if first
    // extracts these from last block
    //      Time_stamp time;            // bit field of second since Jan 1, 2021 00:00 == today - epcoch time on that date
    //      unsigned char blockSize;    //Size of transaction elements in the block
    //      int blockNum;               //count of blocks
    //      Hash_key blockHash;         //The Merkle tree hash of the transactions
    //      Hash_key headHash;          //Hash of the head elements above
    //      Transaction modBuffer[MXTXNUM]; //Transactions contained in the block ( transactions)
    // uses the extracted values to regenerate the  headHash should match

//  addTransactions - adds a new session of transactions at the end of the block chain starting with a new block

/*
 printTransactions(char *filename);
 Description: this function prints the list of all transactions in the entire blockchain file skipping over any invailid transactions
 PARAMS: char *filename
 RETURN: none
 PRE: needs to have a valid filename
 POST: prints all valid transactions
 */
void printTransactions(char *filename);


/*
 print_transaction_array (Transaction *tans_array)
 Description: helper function for printTransactions that prints all the transactions an array of trasactions
    skipping over any invailid transactions
 PARAMS: Transaction *tans_array
 RETURN: none
 PRE: needs to have a transaction array that's already been parced from a block.
 POST: none
 */
void print_transaction_array (Transaction *tans_array);

/*
 re_initilaize_modBuffer_to_zero(void)
 Description: overwrites the modBuffer so that all transactions have a value of 0 so that the modBuffer can be reused 
    if a session lasts longer than 64 sessions
 PARAMS: none
 RETURN: none
 PRE: none
 POST: the modBuffer is empty
 */
void re_initilaize_modBuffer_to_zero(void);


/*
 getPeep(char *peep, char *filename, long peep_time);
 Description: this function recreates a peep from a file up to and including the peep_time specified by the user, if current peep
    is desired passing any future peep_time will work up to a value of "long input_time = 281474976710654" as this is the largest valid number
 PARAMS: char *peep, char *filename, long peep_time
 RETURN: none
 PRE: needs to have a valid filename, an initialized peep, and a peep_time for the function to stop at
 POST: modifies the peep so that peep is current up to a specific time
 */
void getPeep(char *peep, char *filename, long peep_time); //produces the current peep by replaying all the transactions contained in the block chain

/*
 modify_peep_from_trans_list(Transaction *tans_array, char *peep, long peep_time);
 Description: helper function for getPeep that modifies an existing peep using an array of trasactions, 
    will stop at but also include the peep_time tansactions
 PARAMS: Transaction *tans_array, char *peep, long peep_time
 RETURN: none
 PRE: needs to have a transaction array that's already been parced from a block, an initialized peep, and a peep_time to stop at
 POST: modifies the peep so that peep is current up to a specific time
 */
void modify_peep_from_trans_list(Transaction *tans_array, char *peep, long peep_time);
/*
 create_Time_stamp(void)
 Description: creates a 6 byte time stamp that has an epoch of jan 1 00:00
 PARAMS: none
 RETURN: Time_stamp struc
 PRE: none
 POST: returns a Time_stamp that can be used for the blockchain and transaction list
 */
Time_stamp create_Time_stamp(void);

/*
 create_epoch_time_from_Time_stamp(Time_stamp peep_time)
 Description: converts a Time_stamp to an unsigned long
 PARAMS: Time_stamp peep_time
 RETURN: unsigned long
 PRE: needs an initialized Time_stamp
 POST: returns a Time_stamp thats been unpacked to an unsigned long
 */
unsigned long create_epoch_time_from_Time_stamp(Time_stamp peep_time);

/*
 open_block_list(char *filename, int * num);
 Description: this function is a helper function that opens a file and returs an array of blocks 
    from a file and the number of blocks in that file
 PARAMS: char *filename, int * num
 RETURN: int * num, Block *
 PRE: needs to have a valid filename, an initialized mum
 POST: returns a pointer to an array of blocks and the number of blocks in that array 
 */
Block * open_block_list(char *filename, int * num);

/*
 verify_last_Block(char *filename)
 Description: this function verifies that the last block in a blockchain is valid and has not been tampered with
 PARAMS: char *filename
 RETURN: none
 PRE: needs to have a valid filename
 POST: prints a statement as if the last block's interety has been comprimised
 */
void verify_last_Block(char *filename);

#endif
