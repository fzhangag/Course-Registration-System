#ifndef CHECKER_H
#define CHECKER_H

#include "register_common.h"
#include "regularExpression.h"
#include "hashmap.h"

#include <string>

using namespace std;

class registerChecker {
    public:
        registerChecker() :v(ELFHash, 23) {}
        void insert(string name, string regex) {
			v.insert(name, regularExpression(regex));
		}

		bool check(string name, string query) {
			regularExpression *re = v[name];
			if(re) {
				return re->match(query);
			}
			return false;
		}

    private:
        Hashmap<string, regularExpression, int(*)(const string&, int)> v;
};

#endif