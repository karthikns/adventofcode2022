#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

vector<string> GetFileContents(const string& fileName)
{
    ifstream fileStream(fileName, ios::in);
    vector<string> input;

    string line;
    while(getline(fileStream, line, '\n'))
        input.emplace_back(line);

    return input;
}

class Directory;
class File;

class Item
{
public:
    Item(const string name, weak_ptr<Directory> parent):
        name(name),
        parent(parent)
    {
    }

    string GetName()
    {
        return name;
    }

    weak_ptr<Directory> GetParent()
    {
        return parent;
    }

private:
    string name;
    weak_ptr<Directory> parent;
};

class File : public Item
{
public:
    File(const string& name, weak_ptr<Directory> parent, const size_t size) :
        Item(name, parent),
        size(size)
    {
    }

    size_t GetSize() const
    {
        return size;
    }

private:
    size_t size;
};

class Directory : public Item, public std::enable_shared_from_this<Directory>
{
public:
    Directory(const string name, weak_ptr<Directory> parent):
        Item(name, parent)
    {
    }

    void AddFile(const string& name, const size_t size)
    {
        nameToFileMap[name] = make_shared<File>(name, shared_from_this(), size);
    }

    void AddDirectory(const string& name)
    {
        nameToDirectoryMap[name] = make_shared<Directory>(name, shared_from_this());
    }

    weak_ptr<Directory> GetSubDirectory(const string& name)
    {
        return nameToDirectoryMap[name];
    }

    vector<shared_ptr<File>> GetFiles()
    {
        auto range = nameToFileMap | views::transform([](auto p) { return p.second; });
        return vector<shared_ptr<File>>(range.begin(), range.end());
    }

    vector<shared_ptr<Directory>> GetSubDirectories()
    {
        auto range = nameToDirectoryMap | views::transform([](auto p) { return p.second; });
        return vector<shared_ptr<Directory>>(range.begin(), range.end());
    }

private:
    map<string, shared_ptr<File>> nameToFileMap;
    map<string, shared_ptr<Directory>> nameToDirectoryMap;
};

class FileSystemNavigator
{
public:
    FileSystemNavigator(weak_ptr<Directory> rootDir)
    {
        currentDirectory = rootDir;
        root = rootDir;
    }

    shared_ptr<Directory> GetCurrentDirectory()
    {
        return currentDirectory.lock();
    }

    void NavigateToRoot()
    {
        currentDirectory = root;
    }

    void NavigateToParent()
    {
        if(currentDirectory.lock().get() == root.lock().get())
            return;

        currentDirectory = currentDirectory.lock()->GetParent();
    }

    void NavigateToSubDirectory(const string& subDirectoryName)
    {
        shared_ptr<Directory> currentDirectoryShared = currentDirectory.lock();
        currentDirectory = currentDirectoryShared->GetSubDirectory(subDirectoryName);
    }

private:
    weak_ptr<Directory> currentDirectory;
    weak_ptr<Directory> root;
};

bool IsInputLineInstruction(const string& inputLine)
{
    assert(!inputLine.empty());
    return (inputLine.front() == '$');
}

void CreateRootDirectoryLayout(const vector<string>& inputLines, FileSystemNavigator& navigator)
{
    for(const string& inputLine: inputLines)
    {
        if(IsInputLineInstruction(inputLine))
        {
            vector<string> commandParams;
            stringstream commandParamsInputStream(inputLine.substr(2));
            string candidate;
            while(getline(commandParamsInputStream, candidate, ' '))
            {
                if(candidate.empty())
                    continue;

                commandParams.emplace_back(candidate);
            }

            if(commandParams[0] == "cd")
            {
                if(commandParams[1] == "/")
                    navigator.NavigateToRoot();
                else if(commandParams[1] == "..")
                    navigator.NavigateToParent();
                else
                    navigator.NavigateToSubDirectory(commandParams[1]);
            }
        }
        else // This is the output from ls
        {
            stringstream commandParamsInputStream(inputLine);
            string first, second;
            commandParamsInputStream >> first >> second;

            auto currentDirectory = navigator.GetCurrentDirectory();
            if(first == "dir")
                currentDirectory->AddDirectory(second);
            else
                currentDirectory->AddFile(second, stoi(first));
        }
    }
}

size_t GetSumOfDirectoriesSizesWithSizeUptoInternal(shared_ptr<Directory> root, const size_t maxSize, size_t &result)
{
    vector<shared_ptr<File>> filePtrs = root->GetFiles();
    auto fileSizeRange = filePtrs | views::transform([](auto filePtr) { return filePtr->GetSize(); });

    size_t currentDirectorySize = accumulate(fileSizeRange.begin(), fileSizeRange.end(), size_t(0));

    vector<shared_ptr<Directory>> directoryPtrs = root->GetSubDirectories();
    for(auto subdirectory: directoryPtrs)
    {
        currentDirectorySize += GetSumOfDirectoriesSizesWithSizeUptoInternal(subdirectory, maxSize, result);
    }

    if(currentDirectorySize <= maxSize)
        result += currentDirectorySize;

    return currentDirectorySize;
}

size_t GetSumOfDirectoriesSizesWithSizeUpto(shared_ptr<Directory> root, const size_t maxSize)
{
    size_t result = 0;
    GetSumOfDirectoriesSizesWithSizeUptoInternal(root, maxSize, result);
    return result;
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    shared_ptr<Directory> root = make_shared<Directory>("/", weak_ptr<Directory>());
    FileSystemNavigator navigator(root);

    CreateRootDirectoryLayout(inputLines, navigator);
    size_t returnValue = GetSumOfDirectoriesSizesWithSizeUpto(root, 100000ul);

    return returnValue;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}
