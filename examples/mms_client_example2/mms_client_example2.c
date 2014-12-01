
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

	MmsError mmsError;

	/* Set maximum MMS PDU size (local detail) to 2000 byte */
	MmsConnection_setLocalDetail(con, 2000);

	if (!MmsConnection_connect(con, &mmsError, hostname, tcpPort)) {
		printf("MMS connect failed!\n");
		goto exit;
	}
	else
		printf("MMS connected.\n\n");

	printf("Domains present on server:\n--------------------------\n");
	LinkedList nameList = MmsConnection_getDomainNames(con, &mmsError);
	LinkedList_printStringList(nameList);
	printf("\n");

	LinkedList element = nameList;

	while ((element = LinkedList_getNext(element)) != NULL) {
		printf("\nNamed variables in domain: %s\n-------------------------------------------------\n", (char*) element->data);

		LinkedList variableList = MmsConnection_getDomainVariableNames(con, &mmsError, (char*) element->data);

		LinkedList_printStringList(variableList);

		LinkedList_destroy(variableList);

		printf("\nNamed variable lists (data sets) in domain: %s\n", (char*) element->data);

		LinkedList dataSetList = MmsConnection_getDomainVariableListNames(con, &mmsError, (char*) element->data);

		LinkedList_printStringList(dataSetList);

		LinkedList_destroy(dataSetList);

	}

	LinkedList_destroy(nameList);

exit:
	MmsConnection_destroy(con);
}

