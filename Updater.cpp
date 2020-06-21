#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <io.h>
#include <direct.h>
#include "WebUpdate.h"
#include <regex>
#include <fstream>

using namespace std;
using std::string;
using std::regex;
using namespace std::experimental::filesystem;

int CMDRunder(const char* cmd, char* result)
{
    char buffer[128];
    FILE* pipe = _popen(cmd, "r");
    if (!pipe)
    {
        return 0;
    }

    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe))
        {
            strcat(result, buffer);
        }
    }

    _pclose(pipe);

    return 1;
}

void ConvertPath(path filePath)
{
    string path = absolute(filePath).string();
    //cout << path << endl;

    ifstream ifileStream;
    ifileStream.open(path, ios::out);
    int length;
    ifileStream.seekg(0, ios::end);
    length = ifileStream.tellg();
    char* buffer = new char[length + 1];
    ifileStream.seekg(0, ios::beg);
    ifileStream.read(buffer, length);
    buffer[length] = '\0';
    ifileStream.close();
    //cout << "buffer: " << buffer << endl;

    //<p><a href="other/OtherMD.md#third">To other</a></p>
    regex reg("(<a href=\".*?)\\.md(.*?\">)");
    string newStr = regex_replace(buffer, reg, "$1.html$2");
    const char* newBuffer = newStr.c_str();

    ofstream ofileStream;
    ofileStream.open(path, ios::trunc | ios::in);
    ofileStream.write(newBuffer, strlen(newBuffer));
    cout << "new buffer: " << newBuffer << endl;

    ofileStream.close();
}

int main()
{
    const char* srcDir = "E:\\ExampleNote";
    const char* targetDir = "E:\\Web";
    const char* cmdFormat = "showdown makehtml -i %s -o %s";

    Updater updater = Updater();
    updater.CreateCatalog(srcDir);

    vector<FileInfo>* infos = updater.GetFileInfos();
    if (infos != NULL)
    {
        for (vector<FileInfo>::const_iterator iter = infos->begin(); iter != infos->end(); ++iter)
        {
            FileInfo info = *iter;

            char srcFullPath[300] = "";
            info.FullPath(srcFullPath);
            path srcPath(srcFullPath);

            if (srcPath.extension() == ".md")
            {
                string desPathStr = srcPath.string();
                desPathStr = desPathStr.substr(strlen(srcDir), desPathStr.length());
                desPathStr = targetDir + desPathStr;
                cout << desPathStr << endl;
                path desPath(desPathStr);
                desPath.replace_extension(".html");
                if (!exists(desPath.parent_path()))
                {
                    create_directories(desPath.parent_path());
                }

                char cmd[300] = "";
                snprintf(cmd, 200, cmdFormat, srcPath.string().c_str(), desPath.string().c_str());
                cout << cmd << endl;

                char result[1024 * 4] = "";
                if (CMDRunder(cmd, result) == 1)
                {
                    cout << result << endl;
                }

                ConvertPath(desPath);
            }
        }
    }

    return 0;
}