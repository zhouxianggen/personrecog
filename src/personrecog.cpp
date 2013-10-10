
#include <stdio.h>
#include <math.h>
#include "crfpp.h"
using namespace CRFPP;
#include "config.h"
#include "personrecog.h"
namespace ucltp {

#define  isujoint(u) (u==0xFF0E || u==0x2014 || u==0x2D || u==0xB7 || u==0x2E)
bool ischar(uint16 u)
{
    if (isujoint(u)) //.-
        return true;
    if (isuchn(u))
        return true;
    if (isualpha(u))
        return true;
    return false;
}

void PersonRecog::release()
{
	if (conf_) delete (Config*)conf_;
    if (tagger_) delete (Tagger*)tagger_;
	if (dict_) delete (Config*)dict_;
}

bool PersonRecog::init(const char *data_dir)
{
    release();

    // new config
    std::string dir = std::string(data_dir);
    if (dir.length() && dir[dir.length()-1]!='/')
        dir += "/";
    Config *cfg = new Config;
    conf_ = (void*)cfg;
    if (!cfg)
        return false;
    cfg->set("person", "model", dir+"person.model");
    
    return init();
}

bool PersonRecog::set(const char *res, const char *path)
{
    if (!conf_)
        return false;
    Config *cfg = (Config*)conf_;
    cfg->set("person", res, path);

    return init();
}

bool PersonRecog::init()
{
    if (!conf_)
        return false;

    Config *cfg = (Config*)conf_;
    char arg[256];
    sprintf(arg, "-m %s -v3", cfg->get("person", "model").c_str());
    Tagger *tagger = createTagger(arg);
    if (!tagger)
        return false;
    tagger_ = (void*)tagger;

    return true;
}

bool PersonRecog::tokenize(const char *text, Tokens &toks)
{
    uint32 len, code, i;
    if (!text) return false;
    for (i=0; getu8char(text+i, len, code); i+=len) {
        code = u2l(d2s(code));
        if (code > 0xFFFF) return false;
		if (code <= 0x7F) toks.push_back(Char(char(code), i, code)); 
		else toks.push_back(Char(text+i, i, len, code));
    }
    return !text[i];
}

bool PersonRecog::recog(const char *text, Words &persons)
{
    Tokens toks;
    if (!tokenize(text, toks))
        return false;
	
    //for (int i=0; i<toks.size(); i+=1) printf("%s ", toks[i].name_); printf("\n");
    Tags tags(toks.size(), 'N');
    if (tagger_)
        stat_recog(toks, tags);
	//for (int i=0; i<tags.size(); i+=1) printf("%s/%c  ", toks[i].name_, tags[i]); printf("\n");
    if (dict_)
        dict_recog(toks, tags);

    persons.clear();
    for (int i=0,s=tags.size(); i<s; ) {
        if (tags[i] == 'B') {
            std::string w = std::string(toks[i].name_);
            for (i=i+1; i<s && tags[i]=='I'; i+=1) {
                w += std::string(toks[i].name_);
            }
			if (rule_check(w.c_str()))
            	persons.push_back(w);
        }
        else i += 1;
    }
	return true;
}

bool PersonRecog::rule_check(const char *person)
{
	char pat[32];
	uint32 len, code, i=0;
	const char *tmp = person;
	for (; person[0] && getu8char(person, len, code); person+=len) {
		if (i >= 32) return false;
		code = u2l(d2s(code));
		if (isujoint(code)) pat[i++] = '.';
		else if (isuchn(code) && (i==0 || pat[i-1]!='C')) pat[i++] = 'C';
		else if (isualpha(code) && (i==0 || pat[i-1]!='E')) pat[i++] = 'E';
		else NULL;
	}
	if (i >= 32) return false;
	pat[i] = '\0';
	if (strcmp(pat, "E") == 0) return true;
	if (strcmp(pat, "E.E") == 0) return true;
	if (strcmp(pat, "E.E.E") == 0) return true;
	if (strcmp(pat, "E.E.E.E") == 0) return true;
	if (strcmp(pat, "E.E.E.E.E") == 0) return true;
	if (strcmp(pat, "C") == 0) return true;
	if (strcmp(pat, "C.C") == 0) return true;
	if (strcmp(pat, "C.C.C") == 0) return true;
	if (strcmp(pat, "C.C.C.C") == 0) return true;
	if (strcmp(pat, "C.C.C.C.C") == 0) return true;
	if (strcmp(pat, "C.C.C.C.C.C") == 0) return true;
	if (strcmp(pat, "E.C") == 0) return true;
	if (strcmp(pat, "E.E.C") == 0) return true;
	if (strcmp(pat, "E.E.E.C") == 0) return true;
	if (strcmp(pat, "E.E.E.E.C") == 0) return true;
	if (strcmp(pat, "E.C.C") == 0) return true;
	if (strcmp(pat, "E.C.C.C") == 0) return true;
	if (strcmp(pat, "E.C.C.C.C") == 0) return true;
	if (strcmp(pat, "C.E.C") == 0) return true;
	if (strcmp(pat, "EC") == 0) return true;
	if (strcmp(pat, "CE") == 0) return true;
	if (strcmp(pat, "E.EC") == 0) return true;
	//printf("pat of %s : %s\n", tmp, pat);
	return false;
}

bool PersonRecog::dict_recog(const Tokens &chars, Tags &tags)
{
	return true;
}

bool PersonRecog::stat_recog(const Tokens &toks, Tags &tags)
{
    Tagger *tagger = (Tagger*)tagger_;
	char context[512];
    for (int i=0,s=toks.size(); i<=s; i+=1) {
        if (i<s && ischar(toks[i].code_)) {
            sprintf(context, "%s", toks[i].name_);
            tagger->add(context);
        }
        else {
            if (tagger->size() > 0) {
			    tagger->parse();
			    for (int j=0,ts=tagger->size(); j<ts; j+=1) {
                    char t = tagger->y2(j)[0];
				    if (t == 'B') tags[i-ts+j] = 'B';
                    else if (t == 'I' || t == 'E') tags[i-ts+j] = 'I';
                    else tags[i-ts+j] = 'N';
                }
            }
            if (i < s) tags[i] = 'N';
            tagger->clear();
        }
    }
    return true;
}

} // namepsace

