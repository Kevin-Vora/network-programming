#include "file_transfer.h"

extern __thread int errno;

readfile_res *
retrieve_file_1_svc(request *req, struct svc_req *rqstp)
{

        FILE *file;
   	 char data[1024];
   	 int bytes;
   	 static readfile_res res;

    file = fopen(req->name, "rb");
   	 if (file == NULL) {
   		 res.errno = errno;
   		 return (&res);
   	 }

   	 fseek (file, req->start, SEEK_SET);
   	 bytes = fread(res.readfile_res_u.chunk.data, 1, 1024, file);

   	 //res.readfile_res_u.chunk.data = data;
   	 res.readfile_res_u.chunk.bytes = bytes;

	   	 res.errno = 0;
   	 fclose(file);
   	 return (&res);
}

int *
send_file_1_svc(chunksend *rec, struct svc_req *rqstp)
{

        FILE *file;
   	 int write_bytes;
   	 static int result;

   	 file = fopen(rec->name, "a");
   	 if (file == NULL) {
   		 result = errno;
   		 return &result;
   	 }

   	 write_bytes = fwrite(rec->data, 1, rec->bytes, file);
   	 fclose(file);

   	 result = 0;
    return &result;
}

