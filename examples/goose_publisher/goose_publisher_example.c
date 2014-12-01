/*
 * goose_publisher_example.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "mms_value.h"
#include "goose_publisher.h"

// has to be executed as root!
int
main(int argc, char** argv)
{

	LinkedList dataSetValues = LinkedList_create();

	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
	LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
	LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

	GoosePublisher publisher = GoosePublisher_create(NULL, "eth0");

	GoosePublisher_setGoCbRef(publisher, "Test1/LLN0$GO$gocb1");
	GoosePublisher_setConfRev(publisher, 1);
	GoosePublisher_setDataSetRef(publisher, "Test1/LLN0$dataset1");

	int i = 0;

	for (i = 0; i < 3; i++) {
		sleep(1);

		if (GoosePublisher_publish(publisher, dataSetValues) == -1) {
			printf("Error sending message!\n");
		}
	}

	GoosePublisher_destroy(publisher);
}




