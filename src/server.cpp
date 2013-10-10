
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <iconv.h>
using namespace std;
#include "personrecog.h"
using namespace ucltp;
#include "mongoose.h"

#define MAX_POST_DATA_SIZE  409600

PersonRecog pr;

char* get_ip (long ip)
{
	struct in_addr in;
	in.s_addr = ip;
	return inet_ntoa(in);
}

static void *callback (enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request) 
{
	char postdata[MAX_POST_DATA_SIZE];
	string result = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

	if (event == MG_NEW_REQUEST) {
		const char *query = request->query_string;
		printf("qeury=%s\tip=%s\n", query, get_ip(request->remote_ip));
		int s = mg_read(conn, postdata, MAX_POST_DATA_SIZE);
		if (s<=0 or s>=MAX_POST_DATA_SIZE) {
			printf("post data too large\n");
			return (void*)"";
		}
		postdata[s] = '\0';
		//printf("data=%s\n", postdata);
		
		Words words;
		pr.recog(postdata, words);
		result += words.size()? words[0] : "";
		for (int i=1; i<words.size(); i+=1)
			result += ","+words[i];
		printf("result=%s\n", result.c_str());
		mg_write(conn, result.c_str(), result.length());
		return (void*)"";  // Mark as processed
	}
	 else {
		return NULL;
	}
}

int main (int argc, char **argv)
{
	struct mg_context *ctx;
	const char *usage = "args: port, thread-num";
	const char *options[] = {"listening_ports", "8081", "num_threads", "10", "error_log_file", "error_log", NULL};

	if (argc != 3) {
		printf("%s\n", usage);
		return -1;
	}
	options[1] = argv[1];
	options[3] = argv[2];

	if (!pr.init("../data")) {
		printf("ner init failed\n");
		return -1;
	}

	printf("init ok\n");
	ctx = mg_start(&callback, NULL, options);
	while(getchar() != 's') { sleep(5); };  // Wait until user hits "s"
	mg_stop(ctx);

	return 0;
}


