
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef pair<string, string> Attribute;
typedef unordered_map<string, string> Attributes;

enum HTMLElementType {
	NORMAL,
	NOCLOSE
};

