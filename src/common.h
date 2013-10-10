
#ifndef UCLTP_COMMON_H_
#define UCLTP_COMMON_H_
#include <string>
#include <vector>
namespace ucltp {

typedef  unsigned short  uint16;
typedef  unsigned int    uint32;

#define  u2l(u)      (((u)>=0x41 && (u)<=0x5A)? ((u)+0x20) : (u)) // upper to lower
#define  d2s(u)      (((u)>=0xFF01 && (u)<=0xFF5D)? ((u)-0xFEE0) : (u)) // db to sb
#define  isuchn(u)   (((u)>=0x4E00 && (u)<=0x9FA5) || u==0x25CB || u==0xE844)
#define  isudigit(u) (d2s(u)>=0x30 && d2s(u)<=0x39)
#define  isulower(u) (d2s(u)>=0x61 && d2s(u)<=0x7A)
#define  isuupper(u) (d2s(u)>=0x41 && d2s(u)<=0x5A)
#define  isualpha(u) (isulower(u) || isuupper(u))
#define  isualnum(u) (isualpha(u) || isudigit(u))
#define  isuspace(u) (((u)>=0x09 && (u)<=0x0d) || (u)==0x20 || (u)==0x3000)
#define  max(x, y)   ((x)>(y)? (x) : (y))
#define  min(x, y)   ((x)<(y)? (x) : (y))

class Char {
public:
    int     beg_; // begin pos of text
    int     end_; // end pos of text
    char    name_[8];
    uint16  code_;
    Char(const char *s="", int b=0, int len=0, uint16 c=0)
        : beg_(b), end_(b+len), code_(c) {
		int i = 0;
		for (i=0; i<len && i<8; i+=1) name_[i] = s[i];
		name_[i] = '\0';
	}
	Char(char ch, int b, uint16 c)
		:beg_(b), end_(b+1), code_(c) {
			name_[0] = ch;
			name_[1] = '\0';
	}
};

typedef std::vector<Char>          Tokens;
typedef std::vector<std::string>   Words;
typedef std::vector<char>          Tags;

// get an utf-8 char
inline bool getu8char (const char *str, uint32 &len, uint32 &code)
{
    const unsigned char	*p = (unsigned char*)str;
    if (!p || !p[0]) return false;
    else if (p[0] <= 0x7F) {
        len = 1;
        code = p[0];
        return true;
    }
    else if (p[0]>=0xC0 && p[0]<=0xDF) len = 2;
    else if (p[0]>=0xE0 && p[0]<=0xEF) len = 3;
    else if (p[0]>=0xF0 && p[0]<=0xF7) len = 4;
    else if (p[0]>=0xF8 && p[0]<=0xFB) len = 5;
    else if (p[0]>=0xFC && p[0]<=0xFD) len = 6;
    else return false;
    code = (p[0] & (0xFF>>(1+len))) << (len-1)*6;
    for (int i=len-1,j=0; i>0; i-=1,j+=6)
        code += (p[i]&0x3F)<<j;
    return true;
}

}
#endif 
