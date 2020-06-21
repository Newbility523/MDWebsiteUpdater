#include<iostream>
#include<vector>
#include<cstring>
#include<filesystem>
#include<io.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

struct FileInfo {
    char dir[100] = "";
    char name[100] = "";
    char extend[10] = "";

    FileInfo(const char* directory, const char* fullName)
    {
        strcpy(dir, directory);
        const char* ext = strrchr(fullName, '.');
        strcpy(extend, ext);
        strncpy(name, fullName, strlen(fullName) - strlen(ext));
    }

    void FullName(char* buffer)
    {
        strcpy(buffer, name);
        strcat(buffer, extend);
    }

    void FullPath(char* buffer)
    {
        strcpy(buffer, dir);
        strcat(buffer, "\\");
        strcat(buffer, name);
        strcat(buffer, extend);
     }
};

class Updater
{
private:
    vector<FileInfo>* paths;
public:
    Updater()
    {
        paths = new vector<FileInfo>();
    }

    void CreateCatalog(char const* dir)
    {
        const char* searchTail = "\\*";
        char curPath[30];
        char searchPath[30];
        strcpy(curPath, dir);
        strcpy(searchPath, curPath);
        strcat(searchPath, searchTail);

        intptr_t handle;
        _finddata_t findData;

        handle = _findfirst(searchPath, &findData);
        // string a = "abc";
        // handle = _findfirst(a, &findData);
        if (handle == -1 or findData.attrib & _A_HIDDEN)
        {
            return;
        }

        do
        {
            if (findData.attrib & _A_SUBDIR)
            {
                if (strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0)
                {
                    char subDir[100];
                    strcpy(subDir, curPath);
                    strcat(subDir, "\\");
                    strcat(subDir, findData.name);
                    CreateCatalog(subDir);
                }
            }
            else
            {
                /*char fileChar[100];
                strcpy(fileChar, curPath);
                strcat(fileChar, "/");
                strcat(fileChar, findData.name);
                string filePath(fileChar);*/
                FileInfo newFile(curPath, findData.name);
                paths->push_back(newFile);
            }
        } while (_findnext(handle, &findData) == 0);
    }

    void DisplayVectorStr()
    {
        /*cout << paths->size() << endl;
        for (int i = 0; i < paths->size(); ++i)
        {
            cout << (*paths)[i] << endl;
        }*/
    }

    FileInfo GetOne()
    {
        return (*paths)[0];
    }

    vector<FileInfo>* GetFileInfos()
    {
        return paths;
    }
};
