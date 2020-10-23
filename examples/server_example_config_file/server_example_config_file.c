/*
 *  server_example_config_file.c
 *
 *  This example shows how to use dynamic server data model with a configuration file.
 *
 *  - How to open and parse the model configuration file
 *  - How to access data attributes by object reference strings
 *  - How to access data attributes by short addresses
 *
 *  Note: If building with cmake the vmd-filestore folder containing the configuration file
 *  (model.cfg) has to be copied to the folder where the example is executed!
 *  The configuration file can be created from the SCL(ICD) file with the Java tool genconfig.jar
 *  that is included in the source distribution of libiec61580.
 *
 */

#include "iec61850_server.h"
#include "hal_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "iec61850_config_file_parser.h"

static int running = 0;

void sigint_handler(int signalId)
{
	running = 0;
}

int
main(int argc, char** argv)
{
    int tcpPort = 102;

    if (argc > 1) {
        tcpPort = atoi(argv[1]);
    }

    /* parse the configuration file and create the data model */
    IedModel* model = ConfigFileParser_createModelFromConfigFileEx("model.cfg");

    if (model == NULL) {
        printf("Error parsing config file!\n");
        return 1;
    }

	IedServer iedServer = IedServer_create(model);

	/* Access to data attributes by object reference */

    DataAttribute* anIn1_mag_f = (DataAttribute*)
            IedModel_getModelNodeByObjectReference(model, "simpleIOGenericIO/GGIO1.AnIn1.mag.f");

    DataAttribute* anIn1_t = (DataAttribute*)
            IedModel_getModelNodeByObjectReference(model, "simpleIOGenericIO/GGIO1.AnIn1.t");

    if (anIn1_mag_f == NULL)
        printf("Error getting AnIn1.mag.f data attribute!\n");

    /* Access to data attributes by short address */
    DataAttribute* anIn2_mag = (DataAttribute*)
            IedModel_getModelNodeByShortAddress(model, 101);

    DataAttribute* anIn2_t = (DataAttribute*)
            IedModel_getModelNodeByShortAddress(model, 102);

    DataAttribute* anIn2_mag_f = NULL;

    if (anIn2_mag == NULL)
        printf("Error getting AnIn2.mag data attribute!\n");
    else
        anIn2_mag_f = (DataAttribute*) ModelNode_getChild((ModelNode*) anIn2_mag, "f");

	IedServer_start(iedServer, tcpPort);

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}

	running = 1;

	signal(SIGINT, sigint_handler);

	float val = 0.f;

	MmsValue* floatValue = MmsValue_newFloat(val);

	while (running) {

	    if (anIn1_mag_f != NULL) {
            MmsValue_setFloat(floatValue, val);

            IedServer_lockDataModel(iedServer);

            MmsValue_setUtcTimeMs(anIn1_t->mmsValue, Hal_getTimeInMs());
            IedServer_updateAttributeValue(iedServer, anIn1_mag_f, floatValue);

            IedServer_unlockDataModel(iedServer);
	    }

	    if (anIn2_mag_f != NULL) {
	        MmsValue_setFloat(floatValue, 0.f - val);

	        IedServer_lockDataModel(iedServer);

            MmsValue_setUtcTimeMs(anIn2_t->mmsValue, Hal_getTimeInMs());
            IedServer_updateAttributeValue(iedServer, anIn2_mag_f, floatValue);

            IedServer_unlockDataModel(iedServer);
	    }

	    val += 0.1f;

		Thread_sleep(100);
	}

	MmsValue_delete(floatValue);

	IedServer_stop(iedServer);

	IedServer_destroy(iedServer);

	IedModel_destroy(model);
  return 0;
} /* main() */
