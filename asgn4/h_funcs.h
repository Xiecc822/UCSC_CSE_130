/****************************************
* Name: Chucheng Xie
* UCSC UserID: cxie15
* Student ID: 1989752
* CSE_130 Assignment_4
* h_funcs.h
****************************************/

#ifndef _HT_H
#define _HT_H

#include "response.h"
#include "request.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// Hash table structure: create with ht_create, free with ht_destroy.
typedef struct ht ht;

// Create hash table and return pointer to it, or NULL if out of memory.
ht *ht_create(void);

// Free memory allocated for hash table, including allocated keys.
void ht_destroy(ht *table);

// Get item with given key (NUL-terminated) from hash table. Return
// value (which was set with ht_set), or NULL if key not found.
void *ht_get(ht *table, const char *key);

// Set item with given key (NUL-terminated) to value (which must not
// be NULL). If not already present in table, key is copied to newly
// allocated memory (keys are freed automatically when ht_destroy is
// called). Return address of copied key, or NULL if out of memory.
const char *ht_set(ht *table, const char *key, void *value);

// Return number of items in hash table.
size_t ht_length(ht *table);

// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct {
    const char *key; // current key
    void *value; // current value

    // Don't use these fields directly.
    ht *_table; // reference to hash table being iterated
    size_t _index; // current index into ht._entries
} hti;

// Return new hash table iterator (for use with ht_next).
hti ht_iterator(ht *table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
bool ht_next(hti *it);

typedef struct Conn conn_t;

// Constructor
conn_t *conn_new(int connfd);

// Destructor
void conn_delete(conn_t **conn);

// Parse the data from connection. Checks static correctness (i.e.,
// that each field fits within our required bounds), but does not
// check for semantic correctness (e.g., does not check that a URI is
// not a directory).
const Response_t *conn_parse(conn_t *conn);

//////////////////////////////////////////////////////////////////////
// Functions that get stuff we might need elsewhere from a connection

// Return the RequestType from parsing.
const Request_t *conn_get_request(conn_t *conn);

// Return URI from parsing.
char *conn_get_uri(conn_t *conn);

// Return the value for the header field named header.  Only
// implemented for header named "Content-Length" and "Request-Id".
char *conn_get_header(conn_t *conn, char *header);

//////////////////////////////////////////////////////////////////////
// Functions that help get data from a connection

// write the data form the connection into the file (fd).
const Response_t *conn_recv_file(conn_t *conn, int fd);

//////////////////////////////////////////////////////////////////////
// Functions that help write responses to the client:

// send a message body from the file (fd)
const Response_t *conn_send_file(conn_t *conn, int fd, uint64_t count);

// send canonical message for a response type
const Response_t *conn_send_response(conn_t *conn, const Response_t *res);

//Functions for debugging:
char *conn_str(conn_t *conn);

#endif
