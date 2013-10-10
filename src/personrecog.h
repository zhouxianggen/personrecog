/*
 * person recognition 
 * Author: zhouxg@ucweb.com
 *
 */
#ifndef	UCLTP_PERSONRECOG_H_
#define	UCLTP_PERSONRECOG_H_
#include "common.h"
namespace ucltp {

class PersonRecog {
public:
    bool recog(const char *text, Words &persons);
    bool init(const char *data_dir);
    bool set(const char *res, const char *path);
    void release();
    PersonRecog() 
        : conf_(0), tagger_(0), dict_(0) {}
    ~PersonRecog() { release(); }
private:
	bool init();
    bool tokenize(const char *text, Tokens &toks);
    bool stat_recog(const Tokens &toks, Tags &tags);
    bool dict_recog(const Tokens &toks, Tags &tags);
	bool rule_check(const char *person);
    void *conf_;
    void *tagger_;
    void *dict_;
};

}
#endif
