// #define TEST_WITH_PATH_BEING_AN_ALIAS_OF_STD_FILESYSTEM
#ifdef TEST_WITH_PATH_BEING_AN_ALIAS_OF_STD_FILESYSTEM
#    include "CGLPathIsStdfilesystemPath.h"
#else
#    include "CGLPathStub.h"
#endif

#include <memory>  // std::make_unique
#include <string>

namespace CGLCharset::Conv {

std::string ConvertU16ToU8StringCopy(const std::u16string&)  // Required after replacement
{
    return {};
}

}  // namespace CGLCharset::Conv

namespace CGL::Unicode {

std::u16string ToLowerCaseCopy(const std::u16string&)  // Required after replacement
{
    return {};
}

}  // namespace CGL::Unicode

namespace {

using ::CGLFile::Path;

[[maybe_unused]] void TestCallsReplacements()
{
    // `const char*` constructor replacement for const objects
    // Raw and smart pointers test the dereferencing (*) and access (->) operators later
    const auto  path       = Path{"/arbitrary.txt"};
    const Path* pathRawPtr = new Path("/arbitrary.txt");
    auto        pathPtr    = std::make_unique<const Path>("/arbitrary.txt");

    // `const char*` constructor replacement for mutable objects
    // Raw and smart pointers test the dereferencing (*) and access (->) operators later
    auto  mutablePath       = Path{"/arbitrary.txt"};
    Path* mutablePathRawPtr = new Path("/arbitrary.txt");
    auto  mutablePathPtr    = std::make_unique<Path>("/arbitrary.txt");

    // 0 argument const method to 0 argument const method
    path.FullPath();
    pathPtr->FullPath();
    pathRawPtr->FullPath();

    path.GenericFullPath();

    path.FullPathA();

    path.GenericFullPathA();

    path.Directory();

    path.FileName();

    path.FileNameNoExtension();

    path.FileExtension();

    path.Root();

    {
        [[maybe_unused]] bool x = path.Empty();
    }

    path.IsAbsolute();

    path.IsRelative();

    // 0 argument mutable method to 0 argument mutable method
    mutablePath.Clear();
    mutablePathPtr->Clear();
    mutablePathRawPtr->Clear();

    // 0 argument const method to 1 argument const function
    {
        [[maybe_unused]] auto nodiscard_var = path.Size();
    }
    {
        [[maybe_unused]] auto nodiscard_var = pathPtr->Size();
    }
    {
        [[maybe_unused]] auto nodiscard_var = pathRawPtr->Size();
    }

    path.GetLastModificationDate();

    {
        [[maybe_unused]] auto nodiscard_var = path.Exists();
    }

    path.HasDrive();

    {
        [[maybe_unused]] auto nodiscard_var = path.IsDirectory();
    }

    {
        [[maybe_unused]] auto nodiscard_var = path.IsSymbolicLink();
    }

    {
        [[maybe_unused]] auto nodiscard_var = path.IsFile();
    }

    path.IsReadOnly();

    path.IsWritable();

    {
        [[maybe_unused]] auto nodiscard_var = path.GetCanonicalPath();
    }

    path.CreateDir();

    path.CreateFile();

    path.DeleteSymbolicLink();

    path.Delete();

    path.DeleteNoRetry();

    path.RecursiveDelete();

    // 1 argument method to 2 arguments function
    {
        [[maybe_unused]] auto nodiscard_var = path.Equivalent(path);
    }
    {
        [[maybe_unused]] auto nodiscard_var = pathPtr->Equivalent(path);
    }
    {
        [[maybe_unused]] auto nodiscard_var = pathRawPtr->Equivalent(path);
    }

    path.Contains(*pathPtr);

    {
        [[maybe_unused]] auto nodiscard_var = path.GetRelativePath(*pathPtr);
    }

    path.CreateDirectorySymbolicLink(*pathPtr);

    path.CreateFileSymbolicLink(*pathPtr);

    // ---

    path.DeleteFileList(u"*.txt");
    pathPtr->DeleteFileList(u"*.txt");
    pathRawPtr->DeleteFileList(u"*.txt");

    const std::u16string notLiteral = u"*.png";

    path.DeleteFileList(notLiteral);

    mutablePath.Append(path);
    mutablePathPtr->Append(path);
    mutablePathRawPtr->Append(path);

    path.Combine(path);
    pathPtr->Combine(path);
    pathRawPtr->Combine(path);

    // Replace method ChangeExtension to replace_extension
    mutablePath.ChangeExtension(u".txt");
    mutablePathPtr->ChangeExtension(u".txt");
    mutablePathRawPtr->ChangeExtension(u".txt");

    mutablePath.ChangeExtension(u".txt", CGLFile::Path::REPLACE_LAST);

    mutablePath.ChangeExtension(u".txt", CGLFile::Path::APPEND);

    mutablePath.ChangeExtension(u".txt", CGLFile::Path::REPLACE_ALL);
    mutablePathPtr->ChangeExtension(u".txt", CGLFile::Path::REPLACE_ALL);
    mutablePathRawPtr->ChangeExtension(u".txt", CGLFile::Path::REPLACE_ALL);

    // RecursiveCopy to copy
    const bool aBool{};

    path.RecursiveCopy(path, aBool);
    pathPtr->RecursiveCopy(path, aBool);
    pathRawPtr->RecursiveCopy(path, aBool);

    path.RecursiveCopy(*pathPtr, true);
    path.RecursiveCopy(*pathPtr, false);
    path.RecursiveCopy(*pathPtr);

    // Copy to copy_file
    path.Copy(path, aBool);
    pathPtr->Copy(path, aBool);
    pathRawPtr->Copy(path, aBool);

    path.Copy(*pathPtr, true);
    path.Copy(*pathPtr, false);
    path.Copy(*pathPtr);

    path.ReadSymbolicLink(mutablePath);
    pathPtr->ReadSymbolicLink(mutablePath);
    pathRawPtr->ReadSymbolicLink(mutablePath);

    // GetFileListSize to GetTotalFilesSize
    path.GetFileListSize();
    pathPtr->GetFileListSize();
    pathRawPtr->GetFileListSize();

    path.GetFileListSize(u"*.txt");
    path.GetFileListSize(u"*.txt", true);
    path.GetFileListSize(u"*.txt", false);
    path.GetFileListSize(u"*.txt", aBool);
    path.GetFileListSize(notLiteral, aBool);

    // GetFileList method to function
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, notLiteral, aBool, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        pathPtr->GetFileList(fileList, notLiteral, aBool, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        pathRawPtr->GetFileList(fileList, notLiteral, aBool, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", true, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Two calls with the same output vector
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Output vector initialize with constructor, hence not empty
        std::vector<Path> fileList(1);
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Output vector initialize with copy initialization, hence not empty
        std::vector<Path> fileList = {Path{}};
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Output vector modified
        std::vector<Path> fileList;
        fileList.emplace({});
        path.GetFileList(fileList, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Output vector is a reference
        std::vector<Path>  fileList;
        std::vector<Path>& fileList2 = fileList;
        path.GetFileList(fileList2, u".txt", false, [](const Path&) { return false; });
    }
    {
        // Output vector is a pointer
        std::vector<Path>  fileList;
        std::vector<Path>* fileList2 = &fileList;
        path.GetFileList(*fileList2, u".txt", false, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", false);
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", aBool);
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt", true);
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList, u".txt");
    }
    {
        std::vector<Path> fileList;
        path.GetFileList(fileList);
    }

    // MoveFileOnly to MoveFile
    path.MoveFileOnly(path, aBool);
    pathPtr->MoveFileOnly(path, aBool);
    pathRawPtr->MoveFileOnly(path, aBool);

    path.MoveFileOnly(*pathPtr, true);
    path.MoveFileOnly(*pathPtr, false);
    path.MoveFileOnly(*pathPtr);

    // Move to MoveFile with mutable path
    mutablePath.Move(path, aBool);
    mutablePathPtr->Move(path, aBool);
    mutablePathRawPtr->Move(path, aBool);

    mutablePath.Move(*pathPtr, true);
    mutablePath.Move(*pathPtr, false);
    mutablePath.Move(*pathPtr);

    {
        [[maybe_unused]] const uint64_t x = path.GetAvailableDiskSpace();
    }
    {
        [[maybe_unused]] const uint64_t x = pathPtr->GetAvailableDiskSpace();
    }
    {
        [[maybe_unused]] const uint64_t x = pathRawPtr->GetAvailableDiskSpace();
    }

    {
        [[maybe_unused]] const uint64_t x = path.GetTotalDiskSpace();
    }
    {
        [[maybe_unused]] const uint64_t x = pathPtr->GetTotalDiskSpace();
    }
    {
        [[maybe_unused]] const uint64_t x = pathRawPtr->GetTotalDiskSpace();
    }

    // GetDiskSpaceStats to space
    {
        [[maybe_unused]] uint64_t available, total;
        path.GetDiskSpaceStats(&available, &total);
    }
    {
        [[maybe_unused]] uint64_t available, total;
        pathPtr->GetDiskSpaceStats(&available, &total);
    }
    {
        [[maybe_unused]] uint64_t available, total;
        pathRawPtr->GetDiskSpaceStats(&available, &total);
    }
    {
        [[maybe_unused]] auto available = std::make_unique<uint64_t>();
        [[maybe_unused]] auto total     = std::make_unique<uint64_t>();
        path.GetDiskSpaceStats(available.get(), total.get());
    }
    {
        [[maybe_unused]] uint64_t available;
        path.GetDiskSpaceStats(&available, nullptr);
    }
    {
        [[maybe_unused]] uint64_t total;
        path.GetDiskSpaceStats(nullptr, &total);
    }

    {
        Path::CurrentPath(path);
    }

    {
        [[maybe_unused]] const Path anotherPath = Path::CurrentPath();
    }

    {
        [[maybe_unused]] const Path anotherPath = Path::TempPath();
    }

    // TempFileName to CreateTempFile
    {
        [[maybe_unused]] const Path anotherPath = Path::TempFileName(path, notLiteral, notLiteral);
    }
    {
        [[maybe_unused]] const Path anotherPath = Path::TempFileName(path, notLiteral, u".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = Path::TempFileName(path, u"template", u".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = Path::TempFileName(notLiteral, u"template", u".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = Path::TempFileName(u"/arbitrary", u"template", u".txt");
    }

    {
        [[maybe_unused]] std::u16string result;
        Path::ToLowerCase(notLiteral, result);
    }

    {
        [[maybe_unused]] std::u16string result = u"arbitrary";
        Path::ToLowerCaseInPlace(result);
    }

    delete pathRawPtr;
    delete mutablePathRawPtr;
}

}  // namespace
