#ifndef _UTILS_INCLUDE
#define _UTILS_INCLUDE

#include <string>
#include <vector>

using namespace std;

class Utils {
private:
	Utils();
	virtual ~Utils();

public:
	// string split implementation by using delimiter as a character.
	static vector<string> split(string strToSplit, char delimeter);
	// string split implementation by using delimeter as an another string
	static vector<string> split(string stringToBeSplitted, string delimeter);
};

#endif // _UTILS_INCLUDE
