/*********************************Queue TEST.C*******************************
 *
 *	Test program for the circular queue 
 *  These tests are pulled directly from the phase 1 tests in CSCI 320
 *
 *      Original code by Michael Goldweber on May 15, 2004
 *      Modified to only test circular queues and work separate from the
 *      simulator by Gary Lewandowski (April 2022)
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct queueNode	*p, *qa, *q, *firstproc, *lastproc, *midproc;

int main() {
	int i;
	/* create a 10-element queue */
	qa = makeEmptyQueue();
	if (!emptyQ(qa)) {
		printf("emptyProcQ: unexpected FALSE   ");
		exit(-1);
	}
	printf("Inserting...   \n");
	for (i = 0; i < 10; i++) {
		if ((q = malloc(sizeof(struct queueNode))) == NULL) {
			printf("malloc: unexpected NULL while insert   ");
			exit(-2);
		}
		switch (i) {
		case 0:
			firstproc = q;
			break;
		case 5:
			midproc = q;
			break;
		case 9:
			lastproc = q;
			break;
		default:
			break;
		}
		insertQ(&qa, q);
	}
	printf("inserted 10 elements   \n");

	if (emptyQ(qa)) {
		printf("emptyProcQ: unexpected TRUE"   );
		exit(-3);
	}
	/* Check outProc and headProc */
	if (headQ(qa) != firstproc){
		printf("headProcQ failed   ");
		exit(-4);
	}

	/* Check if removeProc and insertProc remove in the correct order */
	printf("Removing...   \n");
	for (i = 0; i < 10; i++) {
		if ((q = removeQ(&qa)) == NULL){
			printf("removeProcQ: unexpected NULL   ");
			exit(-8);
		}
		free(q);
	}
	if (q != lastproc){
		printf("removeProcQ: failed on last entry   ");
		exit(-9);
	}
	if (removeQ(&qa) != NULL){
		printf("removeProcQ: removes too many entries   ");
		exit(-10);
	}
        if (!emptyQ(qa)){
                printf("emptyProcQ: unexpected FALSE   ");
				exit(-11);
		}
	printf("insertProcQ, removeProcQ and emptyProcQ ok   \n");
	printf("process queues module ok      \n");

}

