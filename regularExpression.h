#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

#include "register_common.h"

#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <cstring>
#include <cstdlib>

using namespace std;

static const char* go_through_pa(const char* str) {
    while(*str && ( *str != ')' || *(str - 1) == '\\' )) {
        if(*str == '(' && *(str - 1) != '\\') {
            str = go_through_pa(str + 1);
        }
        else {
            str++;
        }
    }
    return (*str)?(str + 1):(str);
}

class regularExpression {
    public:
        regularExpression() :Re("") {}
		
        regularExpression(string _r) :Re(_r) {}
		
        regularExpression(const regularExpression& r) :Re(r.Re) {}
		
        regularExpression& operator=(const regularExpression& r) {
			Re = r.Re;
			return (*this);
		}
		
        bool operator<(const regularExpression& r) const {
			return Re < r.Re;
		}
		
        bool operator<=(const regularExpression& r) const {
			return Re <= r.Re;
		}
		
        bool operator>(const regularExpression& r) const {
			return Re > r.Re;
		}
		
        bool operator>=(const regularExpression& r) const {
			return Re >= r.Re;
		}
		
        bool operator==(const regularExpression& r) const {
			return Re == r.Re;
		}
		
        bool operator!=(const regularExpression& r) const {
			return Re != r.Re;
		}
		
        bool match(string& str) {
			return match(str.c_str());
		}
		
        bool match(const char *str) {
			if(Re[0] == '^') {
				return matchhere(Re.c_str() + 1, str);
			}
			do {
				if(matchhere(Re.c_str(), str))
					return true;
			}while(*str++ != '\0');
			return false;
		}

		string Re;
		
    protected:
        static bool matchhere(const char *regex, const char *str) {
			int cur_type;
			char c; 
			string s;
			vector<string> v;
			vector<string>::iterator itr;
			char *end;
			long int a, b;
			const char* last_seg;

			switch(regex[0]) {
				case '\0':
					return true;
				case '\\':
					cur_type = 1;
					c = regex[1];
					if(!regex[1]) {
						return false;
					}
					regex += 2;
					break;
				case '[':
					cur_type = 2;
					regex++;
					while(*regex && (*regex != ']' || *(regex - 1) == '\\')) {
						if(regex[0] == '\0') {
							return false;
						}
						s.push_back(regex[0]);
						regex++;
					}
					if(!*regex) {
						return false;
					}
					regex++;
					break;
				case '(':
					cur_type = 3;
					regex++;
					last_seg = regex;
					while(*regex && (*regex != ')' || *(regex - 1) == '\\')) {
						if(*regex == '(' && *(regex - 1) != '\\') {
							regex = go_through_pa(regex + 1);
						}
						else if(*regex == '|' && *(regex - 1) != '\\' ) {
							v.push_back(string(last_seg, regex - last_seg));
							last_seg = regex + 1;
							regex++;
						}
						else {
							regex++;
						}
					}
					if(!*regex) {
						return false;
					}
					v.push_back(string(last_seg, regex - last_seg));
					regex++;
					break;
				default: 
					cur_type = 0;
					c = regex[0];
					regex++;
					break;
			}
			
			switch(regex[0]) {
				case '*':
					switch(cur_type) {
						case 0:
							return matchstar(regex + 1, c, str);
						case 1:
							return matchstar_s(regex + 1, c, str);
						case 2:
							return matchstar(regex + 1, s, str);
					}
					break;
				case '{':
					a = strtol(regex + 1, &end, 10);
					switch(end[0]) {
						case '}':
							switch(cur_type) {
								case 0:
									return matchexactly(end + 1, c, a, str);
								case 1:
									return matchexactly_s(end + 1, c, a, str);
								case 2:
									return matchexactly(end + 1, s, a, str);
							}
							break;
						case ',':
							b = strtol(end + 1, &end, 10);
							if(end[0] != '}'){
								return false;
							}
							switch(cur_type) {
								case 0:
									return matchminmax(end + 1, c, a, b, str);
								case 1:
									return matchminmax_s(end + 1, c, a, b, str);
								case 2:
									return matchminmax(end + 1, s, a, b, str);
							}
						default:
							return false;
					}
					
				default:
					switch(cur_type) {
						case 0:
							if(c == '$') {
								return !regex[0] && !str[0];
							}
							if(str[0] && (c == '.' || c == str[0])) {
								return matchhere(regex, str + 1);
							}
							return false;
						case 1:
							if(str[0] && testcharacter(c, str[0])) {
								return matchhere(regex, str + 1);
							}
							return false;
						case 2:
							if(str[0] && testcharacter(s, str[0])) {
								return matchhere(regex, str + 1);
							}
							return false;
						case 3:
							for(itr = v.begin(); itr != v.end(); itr++) {
								*itr += regex;
								if(matchhere(itr->c_str(), str)) {
									return true;
								}
							}
							return false;
					}
			}
			return false;
		}
		
        static bool matchstar(const char *regex, char c, const char *str) {
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && (*str++ == c || c == '.'));
			return false;
		}

		static bool matchstar(const char *regex, string c, const char *str) {
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && testcharacter(c, *str++));
			return false;
		}

		static bool matchstar_s(const char *regex, char c, const char *str) {
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && testcharacter(c, *str++));
			return false;
		}

		static bool matchexactly(const char *regex, char c, int time, const char *str) {
			while(time) {
				if(!str[0] || (*str++ != c && c != '.')) {
					return false;
				}
				time--;
			}
			return matchhere(regex, str);
		}

		static bool matchexactly(const char *regex, string c, int time, const char *str) {
			while(time) {
				if(!str[0] || !testcharacter(c, *str++)) {
					return false;
				}
				time--;
			}
			return matchhere(regex, str);
		}

		static bool matchexactly_s(const char *regex, char c, int time, const char *str) {
			while(time) {
				if(!str[0] || !testcharacter(c, *str++)) {
					return false;
				}
				time--;
			}
			return matchhere(regex, str);
		}

		static bool matchminmax(const char *regex, char c, int min, int max, const char *str) {
			max -= min;
			while(min) {
				if(!str[0] || (*str++ != c && c != '.')) {
					return false;
				}
				min--;
			}
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && (*str++ == c || c == '.') && max--);
			return false;
		}

		static bool matchminmax(const char *regex, string c, int min, int max, const char *str) {
			max -= min;
			while(min) {
				if(!str[0] || !testcharacter(c, *str++)) {
					return false;
				}
				min--;
			}
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && testcharacter(c, *str++) && max--);
			return false;
		}

		static bool matchminmax_s(const char *regex, char c, int min, int max, const char *str) {
			max -= min;
			while(min) {
				if(!str[0] || !testcharacter(c, *str++)) {
					return false;
				}
				min--;
			}
			do {
				if(matchhere(regex, str)) {
					return true;
				}
			}while(*str && testcharacter(c, *str++) && max--);
			return false;
		}

		static bool testcharacter(char s, char c) {
			switch(s) {
				case 'd':
					return isdigit(c);
				case 's':
					return isspace(c);
				case 'w':
					return isupper(c) || islower(c);
				case 'c':
					return islower(c);
				case 'C':
					return isupper(c);
				case '^':
				case '$':
				case '\\':
				case '.':
				case '[':
				case ']':
				case '{':
				case '}':
				case '|':
				case '(':
				case ')':
					return (c == s);
				default:
					return false;
			}
			return false;
		}

		static bool testcharacter(string s, char c) {
			const char *str = s.c_str();
			bool flag = false;
			if(s[0] == '^') {
				str++;
				flag = true;
			}
			while(*str) {
				if(*str == '\\') {
					if(testcharacter(str[1], c)) {
						return true && !flag;
					}
					str += 2;
				}
				else {
					if(*str == c) {
						return true && !flag;
					}
					str++;
				}
			}
			return (false || flag);
		}
};

#endif