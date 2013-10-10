
#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include "config.h"

namespace ucltp {

bool Config::parse(const char *cfgfile)
{
    std::ifstream fi(cfgfile);
    if (!fi) return false;
    
    std::string rawline, section, name, value;
    while (std::getline(fi, rawline)) {
        std::string line;
        for (int i=0,s=rawline.length(); i<s; i+=1)
            if (!isspace(rawline[i]))
                line += rawline[i];
        if (line.length()==0 || line[0] == '#') continue;
        if (line[0]=='[' && line[line.length()-1]==']') {
            section = line.substr(1, line.length()-2);
            continue;
        }
        int p = 0;
        for (; p<line.length() && line[p]!='='; p+=1){}
        name = line.substr(0, p);
        value = line.substr(p+1, line.length()-p-1);
        cfg_[section][name] = value;
    }

    fi.close();
    return true;
}

void Config::set(const std::string &section, const std::string &name, const std::string &value)
{
    cfg_[section][name] = value;
}

std::string Config::get(const std::string &section, const std::string &name)
{
    static std::string empty;
    std::map<std::string, Section>::iterator mi1;
    std::map<std::string, std::string>::iterator mi2;
    
    mi1 = cfg_.find(section);
    if (mi1 == cfg_.end()) return empty;

    mi2 = mi1->second.find(name);
    if (mi2 == mi1->second.end()) return empty;
    return mi2->second;
}

int  Config::get_int(const std::string &section, const std::string &name)
{
    std::string value = get(section, name);
    return atoi(value.c_str());
}

}
