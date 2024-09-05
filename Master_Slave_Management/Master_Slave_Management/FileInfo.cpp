#include "fileInfo.h"

FileInfo::FileInfo()
	:fileName("baseName")/*, startTime(system_clock::now()),endTime(system_clock::now())*/
{
}

FileInfo::FileInfo(const string& fileName, const tm& start, const tm& end)
: fileName(fileName), startTime(start), endTime(end)
{
}

