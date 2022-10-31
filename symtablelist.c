/* Name: Chimwemwe Chinkuyu
*
*
*/

#include <assert.h>
#include <stdlib.h>
#include "symtable.h"
#include <string.h>

struct SymTableNode {
    /* Pointer to a node's key*/
    const char *pcKey;

    /* Pointer to a node's value*/
    const void *pvValue;

    /*Pointer to the next symtable binding*/
    struct SymTableNode *psNextNode;
    
};

/* Establishes that the managing structure for the SymTable contains a
 * pointer to the first node and the number of nodes in the symtable.*/
struct SymTable {
    struct SymTableNode *psFirstNode;
    size_t numNodes;
};

/* Creates and returns an empty symtable or returns NULL if insufficient memory
 * is available.*/
SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL) return NULL;

    oSymTable->psFirstNode = NULL;
    oSymTable->numNodes = 0;
    return oSymTable;
}

/* Frees all memory occupied by the symtable. */
void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
         psCurrentNode != NULL;
         psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        free((char *)psCurrentNode->pcKey);
        free(psCurrentNode);
    }
    free(oSymTable);
}

/* Returns the number of symtable nodes in the symtable parameter
 * oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable) {
    return oSymTable->numNodes;
}

/**/
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    struct SymTableNode *psNewNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    
    for (psCurrentNode = oSymTable->psFirstNode;
         psCurrentNode != NULL;
         psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        if ((strcmp((psCurrentNode->pcKey), pcKey)) == 0) return 0;
    }

    psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
    if (psNewNode == NULL) return 0;

    psNewNode->pcKey = malloc(strlen(pcKey) +1);

    if (psNewNode->pcKey == NULL) {
       free(psNewNode);
       return 0;
    }

    psNewNode->pcKey = strcpy((char *)psNewNode->pcKey, pcKey);
    psNewNode->pvValue = pvValue;
    psNewNode->psNextNode = oSymTable->psFirstNode;
    oSymTable->psFirstNode = psNewNode;
    oSymTable->numNodes++;
    return 1;
}

/**/
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    const void *pvOValue;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
         psCurrentNode != NULL;
         psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        if ((strcmp((psCurrentNode->pcKey), pcKey)) == 0) {
            pvOValue = psCurrentNode->pvValue;
            psCurrentNode->pvValue = pvValue;
            return (void *) pvOValue;
        }
    }
    return NULL;
}

/**/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        if ((strcmp((psCurrentNode->pcKey), pcKey)) == 0) {
            return 1;
        }
    }
    return 0;
}


void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->psNextNode;
        if ((strcmp((psCurrentNode->pcKey), pcKey)) == 0) {
           return (void *)psCurrentNode->pvValue;
        }
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psPrevNode = NULL;
    const void *pvValue;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode) {
        if ((strcmp((psCurrentNode->pcKey), pcKey)) == 0) {
            pvValue = psCurrentNode->pvValue;
            if (psPrevNode == NULL) {
                oSymTable->psFirstNode = psCurrentNode->psNextNode;
            }
            else {
                psPrevNode->psNextNode = psCurrentNode->psNextNode;
            }
            free((char *) psCurrentNode->pcKey);
            free(psCurrentNode);
            oSymTable->numNodes--;
            return (void *) pvValue;
        }
        psPrevNode = psCurrentNode;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
 void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
 const void *pvExtra){

    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode) {
        (*pfApply) (psCurrentNode->pcKey, (void *) psCurrentNode->pvValue, (void *) pvExtra);
    }

}
