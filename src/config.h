/*
 * 读取配置文件信息
 */
#ifndef UCLTP_CONFIG_H_
#define UCLTP_CONFIG_H_
#include <string>
#include <map>
namespace ucltp {

typedef std::map<std::string, std::string>  Section;

class Config {
public:
    bool parse(const char *cfgfile);
    std::string get(const std::string &section, const std::string &name);
    void set(const std::string &section, const std::string &name, const std::string &value);
    int  get_int(const std::string &section, const std::string &name);
private:
    std::map<std::string, Section> cfg_;
};

}
#endif
