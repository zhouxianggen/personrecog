
#include <string.h>
#include <stdio.h>
#include <fstream>
using namespace std;
#include "personrecog.h"
using namespace ucltp;

void test_sentence(PersonRecog &pr, const char *sentence)
{
    Words words;
	pr.recog(sentence, words);
    for (int i=0; i<words.size(); i+=1)
		printf("%s\n", words[i].c_str());
}

void test_file(PersonRecog &pr, const char *file)
{
	ifstream fi(file);
	string line;
	Words words;
	if (!fi) {
		printf("can not open file: %s\n", file);
		return;
	}
	while (getline(fi, line)) {
		pr.recog(line.c_str(), words);
		//printf("%s\n", line.c_str());
		for (int i=0; i<words.size(); i+=1)
			printf("%s\n", words[i].c_str());
	}
    fi.close();
}

int main(int argc, char **argv)
{
    PersonRecog pr;
	const char *usage = "./run_test data_dir [-f file] [-s sentence]";
	printf("%s\n", usage);
    if (argc < 4)
        return -1;
    // init
    if (!pr.init(argv[1])) {
        printf("init failed.\n");
        return -1;
    }
    // seg
	for (int i=2; i+1<argc; i+=2) {
		if (strcmp(argv[i], "-s") == 0) test_sentence(pr, argv[i+1]);
		else if (strcmp(argv[i], "-f") == 0) test_file(pr, argv[i+1]);
		else NULL;
	}
    
	return 0;
}
