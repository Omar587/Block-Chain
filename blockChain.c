/*-------------------------------------------------------
# Student's Name: Orest Dushkevich, Axel Nguyen, and Omar Mohamed
# Assignment #3 Milestone 3
# Lab Section: X02L and X01l
# CMPUT 201
# Instructor's Name: Nesrine Abbas
*------------------------------------------------------*/

#include "blockChain.h"
#include <openssl/evp.h>



/*Paramater: type Block passed in
 Returns: a haskey of the headHash elements
 such as previousHash, blockNum, blockSize, and time.  */
Hash_key create_headHash(Block *newBlock)
{

    //The variable digest_len will hold the length of the sha256 digest
    unsigned int digest_len = EVP_MD_size(EVP_sha256());
    //The variable digest will hold the final digest (hash)
    unsigned char *digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));
    //Declare the message digest context variable
    EVP_MD_CTX *mdctx;
    //create a message digest context
    mdctx = EVP_MD_CTX_create();
    //initialise the context by identifying the algorithm to be used
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    //Update the digest with a message
    //All the information in the block combined
    //will make a unique hash

    EVP_DigestUpdate(mdctx, &newBlock->previousHash, sizeof(newBlock->previousHash));
    EVP_DigestUpdate(mdctx, &newBlock->blockNum, sizeof(newBlock->blockNum));
    EVP_DigestUpdate(mdctx, &newBlock->blockHash, sizeof(newBlock->blockHash));
    EVP_DigestUpdate(mdctx, &newBlock->blockSize, sizeof(newBlock->blockSize));
    EVP_DigestUpdate(mdctx, &newBlock->time, sizeof(&newBlock->time));

    EVP_DigestFinal_ex(mdctx, digest, &digest_len);
    //Clean up the context
    EVP_MD_CTX_destroy(mdctx);
    //place the headHash in the block itself

    //Trunacate the hash from 64 bytes to a 7 byte hash key
    Hash_key headHash;
    unsigned char truncHash[7];
    char finalHash[7];

    memcpy(truncHash, digest, 4);

    int i;

    //turns the unsigned char hash to a char array
    for (i = 0; i < 3; i++)
    {
        sprintf(finalHash + (i * 2), "%02x", truncHash[i]);
    }
    
    for (int i=0; i < 7; i++){
     headHash.hash[i]=finalHash[i];
    }   

    //printf("%s", headHash.hash);

    return headHash;
}

/*Helper function for the merkele tree, takes a transaction
and spits out a hashkey for the transaction. These hashes
will be part of the leaf nodes of the tree */

Hash_key transaction_hash(Transaction *trans_input)
{

    //The variable digest_len will hold the length of the sha256 digest
    unsigned int digest_len = EVP_MD_size(EVP_sha256());
    //The variable digest will hold the final digest (hash)
    unsigned char *digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));
    //Declare the message digest context variable
    EVP_MD_CTX *mdctx;
    //create a message digest context
    mdctx = EVP_MD_CTX_create();
    //initialise the context by identifying the algorithm to be used
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, trans_input, sizeof(Transaction));
    EVP_DigestFinal_ex(mdctx, digest, &digest_len);
    EVP_MD_CTX_destroy(mdctx);

    Hash_key retKey;
    char finalHash[7];
    int i;

    for (i = 0; i < 3; i++)
    {
        sprintf(finalHash + (i * 2), "%02x", digest[i]);
    }
    
    for (int i=0; i < 7; i++){
    retKey.hash[i]=finalHash[i];
    }
    
    // printf("%s", retKey.hash);

    return retKey;
}

/*Takes two hash keys and return a single has value
helper function for merkleRoot */
Hash_key hash_hashKey(Hash_key *key_1, Hash_key *key_2){

 unsigned int digest_len=EVP_MD_size(EVP_sha256());
 //The variable digest will hold the final digest (hash)
 unsigned char *digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));
 EVP_MD_CTX *mdctx;
 //create a message digest context
 mdctx = EVP_MD_CTX_create();
 EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
 
 EVP_DigestUpdate(mdctx, key_1, sizeof(Hash_key));
 EVP_DigestUpdate(mdctx, key_2, sizeof(Hash_key));

 //finalize digest
 EVP_DigestFinal_ex(mdctx, digest, &digest_len);
 //Clean up the context
 EVP_MD_CTX_destroy(mdctx);

 Hash_key retKey;
 char finalHash[7]; 
 int i;
 for (i = 0; i < 3; i++){
 
 sprintf(finalHash + (i * 2), "%02x", digest[i]);
  }

  for (int i=0; i < 7; i++){
   retKey.hash[i]=finalHash[i];
  }


return retKey;

} 




/*
Paramater: takes the modbuffer
Returns: the rootHash

This function will hash multiple transactions
from the modbuffer and build up the merkle tree 
unti the root is made.

Commented out code shows the result of two
keypairs that were hashed together. Serves as 
a debugging tool.  */

Hash_key Merkle_tree_hash(Transaction *modBuffer)
{


    
    Hash_key hashKey_64[64]; //level 1
    Hash_key hashKey_32[32]; //level 2
    Hash_key hashKey_16[16]; //level 3
    Hash_key hashKey_8[8];   //level 4   
    Hash_key hashKey_4[4];   //level 5   
    Hash_key hashKey_2[2];   //level 6
    Hash_key root; //Root of the hash key return value
 
    Transaction trans[64];
     
    //make hash for level 1 
    for (int j = 0; j < MXTXNUM; j++)
    {
        trans[j] = modBuffer[j];

              
        hashKey_64[j]=transaction_hash(&trans[j]);  
   
    }

   //make hash array for level 2 
   for (int i=0; i < MXTXNUM; i++){
   
      
      hashKey_32[i]=hash_hashKey(&hashKey_64[i] ,&hashKey_64[i+1]);
    // printf("%s ", hash_hashKey(&hashKey_64[i] ,&hashKey_64[i+1]).hash);
    // printf("%d\n", i);  
   }

  
  //make hash array for level 3
   for (int i=0; i < 32; i++){

   hashKey_16[i]=hash_hashKey(&hashKey_32[i] ,&hashKey_32[i+1]);      
   
   //printf("%s ", hash_hashKey(&hashKey_32[i] ,&hashKey_32[i+1]).hash);    
   //printf("%d\n", i);
   //printf("%d\n", i+1);
 
   }
  
  //make hash for level 4
  for (int i=0; i < 16; i++){

  hashKey_8[i]=hash_hashKey(&hashKey_16[i] ,&hashKey_16[i+1]); 
  //printf("%s ", hash_hashKey(&hashKey_16[i] ,&hashKey_16[i+1]).hash);
  //printf("%d\n", i); 
 
  }
  
 //make hash for level 5
  for (int i=0; i < 8; i++){
  hashKey_4[i]=hash_hashKey(&hashKey_8[i] ,&hashKey_8[i+1]);
  //printf("%s ", hash_hashKey(&hashKey_8[i] ,&hashKey_8[i+1]).hash); 
  //printf("%d\n", i);
  }
  
  //make hash for level 6

   for (int i=0; i < 4; i++){
   hashKey_2[i]=hash_hashKey(&hashKey_4[i] ,&hashKey_4[i+1]);  
  // printf("%s ", hash_hashKey(&hashKey_4[i] ,&hashKey_4[i+1]).hash);  
  //printf("%d\n", i);
  
   }
   
 //make the root
 root=hash_hashKey(&hashKey_2[0] ,&hashKey_4[1]);
 
 //printf("%s", root.hash);

 return root;
}


/*Helper function makes a single block 
returns a Block that will be used for writing to the 
blockchain*/
Block create_new_block(Transaction *modbuffer, Hash_key previous_hash, unsigned short num_transactions, int previous_block_num)
{
    //helper for writeBlockChain
    // use  a fake hash for merkle tree
    // use a fake head hash  for now

    Block newBlock;
    newBlock.previousHash = previous_hash;

    newBlock.blockNum = previous_block_num + 1;
    newBlock.time = create_Time_stamp();

    //num of tranaactions in the block
    newBlock.blockSize = num_transactions;

    for (int i = 0; i < MXTXNUM; i++)
    {
        newBlock.transactions[i] = modBuffer[i];
    }

   

    //merkle hash
    Hash_key merkleHash = Merkle_tree_hash(modBuffer);
    strcpy(newBlock.blockHash.hash, merkleHash.hash);

    //place headHash for the block
    Hash_key headBlockHash = create_headHash(&newBlock);
    strcpy(newBlock.headHash.hash, headBlockHash.hash);
    return newBlock;
}

Block recreate_new_block(Block block){
    Block newBlock;
    //num of tranaactions in the block
    //merkle hash
    Hash_key merkleHash = Merkle_tree_hash(block.transactions);
    strcpy(block.blockHash.hash, merkleHash.hash);

    //place headHash for the block
    Hash_key headBlockHash = create_headHash(&block);
    strcpy(block.headHash.hash, headBlockHash.hash);

    newBlock = block;
    return newBlock;
};

void printTransactions(char *filename) //prints all transactions contained in the blockchain
{
    //Block *block_list;
    int total_blocks;

    Block *block_list = open_block_list(filename, &total_blocks); // get an array of blocks from file

    // iterates through all the blocks
    for (int i = 0; i < total_blocks; i++)
    {

        print_transaction_array(block_list[i].transactions);
    }
    // will itterate throgh each block
    // print_transaction_array(block_tans_array);
    free (block_list);
}

void print_transaction_array(Transaction *tans_array)
{
    // timestamp at which the transactions are invalid
    unsigned long epoch_time, first, mid, last;
    int position, event;
    char character;
    Time_stamp void_time;
    void_time.first = 0xFFFF;
    void_time.mid = 0xFFFF;
    void_time.last = 0xFFFF;

    for (int i = 0; i < MXTXNUM; i++)
    {
        if (void_time.first == tans_array[i].time.first && void_time.mid == tans_array[i].time.mid && void_time.last == tans_array[i].time.last)
            break;
        // converts the Time_stamp time to ephoch time
        first = tans_array[i].time.first;
        first = first << 32;
        mid = tans_array[i].time.mid << 16;
        last = tans_array[i].time.last;
        epoch_time = (first | mid) | last;
        position = tans_array[i].mod.position;
        event = tans_array[i].mod.event;

        if (event == 0) // insert
        {
            character = tans_array[i].mod.character;
            printf("insertion at position :%d of character: %c at a reletive epoch time of %lu\n", position, character, epoch_time);
        }
        else // delete
        {
            printf("deletion at position :%d at a reletive epoch time of %lu\n", position, epoch_time);
        }
    }
}

void getPeep(char *peep, char *filename, long peep_time) //produces the current peep by replaying all the transactions contained in the block chain
{
    //Block * block_list;
    int total_blocks;

    Block *block_list = open_block_list(filename, &total_blocks); // get an array of blocks from file

    // iterates through all the blocks
    for (int i = 0; i < total_blocks; i++)
    {
        modify_peep_from_trans_list(block_list[i].transactions, peep, peep_time);
    }

    // is also getPeepAtTime -produces the peep at a certain time stamp by replaying all the transactions in the block chain up to(and including) the time stamp
    // returns the peep
    // will used a modified  insert_character(char *, Transaction *, int) and delete_character(char *, Transaction *, int); to build the peep
    // skips transactions that equal
    free (block_list);
    return;
}

void modify_peep_from_trans_list(Transaction *tans_array, char *peep, long peep_time)
{
    // helper function for getPeep that iterates through a transaction array and re creates the peep
    unsigned long epoch_time;
    int position, event;
    char character;
    // initializez what a void

    for (int i = 0; i < MXTXNUM; i++)
    {
        //printf("test peep: %s\n", peep);
        // determines if the Time_stamp in the transaction is valid
        epoch_time = create_epoch_time_from_Time_stamp(tans_array[i].time);

        // if the timestamp is invalid or out of specified peep_time range will return the peep
        if (epoch_time == 0 || epoch_time > peep_time)
            return;

        // extracts the position and event
        position = tans_array[i].mod.position;
        event = tans_array[i].mod.event;

        if (event == 0) // insert
        {
            // extracts the  character
            character = tans_array[i].mod.character;

            /********insertion copied from peedEditior.c**********/
            if (strlen(peep) == 0)
            {

                peep[0] = character;
                peep[1] = '\0';
                //printf("test 2 peep: %s\n", peep);
                continue;
            }
            int nullTermPos = 0;
            //stores position of the null terminator
            while (nullTermPos < PEEPLEN && *(peep + nullTermPos) != '\0')
            {
                nullTermPos++;
            }
            //if position is after null terminator
            if (nullTermPos < position - 1)
            {
                continue; //modified to do nothing
            }
            //if position is an empty string fill the position with a character
            else if (nullTermPos == position - 1)
            {
                *(peep + position - 1) = character;
                *(peep + nullTermPos + 1) = '\0';
            }

            //else the desired position is before the null terminator.
            //This means we must shift the array position.
            else
            {
                //shifts every element after the desired position 1 to the right
                memcpy(&peep[position], &peep[position - 1], nullTermPos - position + 1);
                *(peep + position - 1) = character;
                *(peep + nullTermPos + 1) = '\0';
            }
        }
        else // delete
        {
            /********deletion copied from peedEditior.c**********/
            //printf("deletion at position :%d at a reletive epoch time of %lu\n", position, epoch_time);
            if (position >= 1 && position <= strlen(peep))
            {
                //modBuffer[num_transactions] = create_transaction(position, peep[position - 1], 1);
                memmove(&peep[position - 1], &peep[position], strlen(peep) - position + 1);
                //calls view_characters
                //printf("Peep becomes\n");
                //view_characters(peep);
                //num_transactions++
                //return num_transactions + 1;
            }
            //else
            //return num_transactions;
        }
    }

    return;
}

// Helper functions

unsigned long create_epoch_time_from_Time_stamp(Time_stamp peep_time)
{
    // defentition of a void timestamp
    Time_stamp void_time;
    void_time.first = 0xFFFF;
    void_time.mid = 0xFFFF;
    void_time.last = 0xFFFF;

    // returns 0 if invalid Time_stamp peep_time
    unsigned long epoch_time, first, mid, last;
    first = peep_time.first;
    first = first << 32;
    mid = peep_time.mid;
    mid = mid << 16;
    last = peep_time.last;
    epoch_time = (first | mid) | last;

    // returns 0 if
    if (void_time.first == peep_time.first && void_time.mid == peep_time.mid && void_time.last == peep_time.last)
        return 0;

    else
        return epoch_time;
}

Time_stamp create_Time_stamp(void)
{
    Time_stamp new_time;
    time_t curEpoch = time(NULL);   // current time
    time_t startEpoch = 1609459200; // jan 1 2021 00:00
    unsigned long current_time = (unsigned long)(curEpoch - startEpoch);
    unsigned long mask_first = 0x0000FFFF00000000, mask_mid = 0x00000000FFFF0000, mask_last = 0x000000000000FFFF;

    unsigned short num;
    num = (unsigned short)(current_time & mask_last);
    new_time.last = num;

    num = (unsigned short)((current_time & mask_mid) >> 16);
    new_time.mid = num;

    num = (unsigned short)((current_time & mask_first) >> 32);
    new_time.first = num;

    return new_time;
}

void re_initilaize_modBuffer_to_zero(void)
{

    for (int i = 0; i < MXTXNUM; i++)
    {
        modBuffer[i].time.first = 0;
        modBuffer[i].time.mid = 0;
        modBuffer[i].time.last = 0;
        modBuffer[i].mod.position = 0;
        modBuffer[i].mod.character = 0;
        modBuffer[i].mod.event = 0;
    }
}

void writeBlockChain(char *filename, Transaction *modBuffer, unsigned short num_transactions)
{
    // use readBlockChain to get the previous block hash and the block_num
    Block prev_block = readBlockChain(filename);
    Block new_block;
    if (prev_block.blockNum != -1)
    { //There is blocks existing
        new_block = create_new_block(modBuffer, prev_block.blockHash,
                                     num_transactions, prev_block.blockNum);
    }
    else
    {
        Hash_key prev_hash;
        strncpy(prev_hash.hash, "0000000", 7);
        new_block = create_new_block(modBuffer, prev_hash, num_transactions, 0);
    }

    // opens a file, creats a new one if does not exist
    FILE *fp = fopen(filename, "ab");
    if (fp == NULL)
    {
        fprintf(stderr, "Error while opening file, exiting...1\n");
        exit(EXIT_FAILURE);
    }
    // creates the first block if does not exist or appends a new block
    fwrite(&new_block, sizeof(Block), 1, fp);
    fclose(fp);
}

Block readBlockChain(char *filename) // - reads a block chain from a file
{
    Block lastBlock;
    int sz;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        lastBlock.blockNum = -1; //Indicate there is no block
        return lastBlock;
    }

    //Get list of blocks
    fseek(fp, 0L, SEEK_END); //move file position to end of file
    sz = ftell(fp);          //will return current position
    fseek(fp, 0L, SEEK_SET); //back to start of file

    Block *blockList = malloc(sz);
    if (!blockList)
    {
        fprintf(stderr, "Cannot malloc, exiting...\n");
        exit(EXIT_FAILURE);
    }
    fread(blockList, sizeof(Block), sz / sizeof(Block), fp);
    fclose(fp);

    lastBlock = blockList[(sz / sizeof(Block)) - 1];
    free(blockList);
    return lastBlock;
}

Block *open_block_list(char *filename, int *num)
{

    int sz;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {

        return 0;
    }

    //Get list of blocks
    fseek(fp, 0L, SEEK_END); //move file position to end of file
    sz = ftell(fp);          //will return current position
    fseek(fp, 0L, SEEK_SET); //back to start of file

    Block *blockList = malloc(sz);
    if (!blockList)
    {
        fprintf(stderr, "Cannot malloc, exiting...\n");
        exit(EXIT_FAILURE);
    }
    fread(blockList, sizeof(Block), sz / sizeof(Block), fp);
    fclose(fp);
    //block_list = blockList; // returns the blocklist
    int num_blocks = (sz / sizeof(Block)); // total amount of blocks
    *num = num_blocks;
    return blockList;
}

void verify_last_Block(char *filename)
{
    Block last_block = readBlockChain(filename);            // get lastblock
    Hash_key /*og_previousHash , og_blockHash,*/ og_headHash;
    //og_previousHash = last_block.previousHash;              // probably dont need
    //og_blockHash = last_block.blockHash;                    // probably dont need
    og_headHash = last_block.headHash;
    //Transaction *og_transactions = last_block.transactions; // probably dont need
    //Time_stamp og_time = last_block.time;                   // probably dont need
    //unsigned char og_blockSize = last_block.blockSize;      // probably dont need
    //int og_blockNum = last_block.blockNum;                // probably dont need

    Hash_key new_headHash;
   
    int num_blocks;
    Block *blockList = open_block_list(filename, &num_blocks);
    Hash_key hash_zero;
    strncpy(hash_zero.hash, "0000000", 7);
    for(int i = 0; i < num_blocks; i++) {
        blockList[i] = recreate_new_block(blockList[i]);
    }
    strncpy(new_headHash.hash, blockList[num_blocks - 1].headHash.hash, 7);

    if (strncmp(new_headHash.hash, og_headHash.hash, 7) == 0) //
        printf("The blockchain is valid\n");
    else 
        printf("The blockchain has been compromised and has an invalid hash\n");
}
