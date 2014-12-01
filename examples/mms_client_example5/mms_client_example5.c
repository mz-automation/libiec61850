
#include <stdlib.h>
#include <stdio.h>
#include "mms_client_connection.h"

int main(int argc, char** argv) {

	char* hostname;
	int tcpPort = 102;

	if (argc > 1)
		hostname = argv[1];
	else
		hostname = "localhost";

	if (argc > 2)
		tcpPort = atoi(argv[2]);

	MmsConnection con = MmsConnection_create();

	MmsError error;

	if (!MmsConnection_connect(con, &error, hostname, tcpPort)) {
		printf("MMS connect failed!\n");
		goto exit;
	}
	else
		printf("MMS connected.\n\n");

	LinkedList dataSetEntries = LinkedList_create();

	MmsVariableAccessSpecification* dataSetEntry =
	    MmsVariableAccessSpecification_create("BayControllerQ", "QA1CSWI1$ST$Pos");

	LinkedList_add(dataSetEntries, (void*) dataSetEntry);

	dataSetEntry =
	        MmsVariableAccessSpecification_create("BayControllerQ", "QA1XCBR1$ST$Pos");

	LinkedList_add(dataSetEntries, (void*) dataSetEntry);

	MmsConnection_defineNamedVariableList(con, &error, "BayControllerQ", "LLN0$LIBIEC61850_CLIENT", dataSetEntries);

	/* delete list and all elements */
	LinkedList_destroy(dataSetEntries);

exit:
	MmsConnection_destroy(con);
}

