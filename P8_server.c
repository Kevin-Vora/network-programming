#include "file_transfer.h"

extern __thread int errno;

int get_file(char *host, char *name)
{
	CLIENT *clnt;
	int total_bytes = 0, write_bytes;
	readfile_res *result;
	request req;
	FILE *file;

	req.name = name;
	req.start = 0;

		clnt = clnt_create(host, FTPPROG, FTPVER, "tcp");
	if (clnt == NULL) {
    	 	clnt_pcreateerror(host);
     	exit(1);
	}

	file = fopen(name, "wb");

	while (1) {
    	req.start = total_bytes;
    	result = retrieve_file_1(&req, clnt);
    	if (result == NULL) {
        	/*
         	* An RPC error occurred while calling the server.
         	* Print error message and stop.
         	*/
        	clnt_perror(clnt, host);
        	exit(1);
    	}


