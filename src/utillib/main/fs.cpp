#include "utillib/fs.h"

using namespace std;

string getDir(const string& path)
{
    auto pos = path.find_last_of("\\/");
    return pos == string::npos  ? "" : path.substr(0, pos);
}

string joinPaths(const string& path0, const string& path1)
{
#ifdef BUILD_WINDOWS
    return path0 + "\\" + path1;
#else
    return path0 + "/" + path1;
#endif
}
