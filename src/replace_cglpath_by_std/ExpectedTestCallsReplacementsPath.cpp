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
    path.u16string();
    pathPtr->u16string();
    pathRawPtr->u16string();

    path.generic_u16string();

    path.string();

    path.generic_string();

    path.parent_path();

    path.filename();

    path.stem();

    path.extension();

    path.root_path();

    {
        [[maybe_unused]] bool x = path.empty();
    }

    path.is_absolute();

    path.is_relative();

    // 0 argument mutable method to 0 argument mutable method
    mutablePath.clear();
    mutablePathPtr->clear();
    mutablePathRawPtr->clear();

    // 0 argument const method to 1 argument const function
    {
        [[maybe_unused]] auto nodiscard_var = file_size(path);
    }
    {
        [[maybe_unused]] auto nodiscard_var = file_size(*pathPtr);
    }
    {
        [[maybe_unused]] auto nodiscard_var = file_size(*pathRawPtr);
    }

    CoveoFilesystem::GetLastModificationDate(path);

    {
        [[maybe_unused]] auto nodiscard_var = exists(path);
    }

    CoveoFilesystem::HasDrive(path);

    {
        [[maybe_unused]] auto nodiscard_var = is_directory(path);
    }

    {
        [[maybe_unused]] auto nodiscard_var = is_symlink(path);
    }

    {
        [[maybe_unused]] auto nodiscard_var = is_regular_file(path);
    }

    CoveoFilesystem::IsReadOnly(path);

    CoveoFilesystem::IsWritable(path);

    {
        [[maybe_unused]] auto nodiscard_var = canonical(path);
    }

    create_directories(path);

    CoveoFilesystem::CreateEmptyFile(path);

    remove(path);

    remove(path);

    remove(path);

    remove_all(path);

    // 1 argument method to 2 arguments function
    {
        [[maybe_unused]] auto nodiscard_var = equivalent(path, path);
    }
    {
        [[maybe_unused]] auto nodiscard_var = equivalent(*pathPtr, path);
    }
    {
        [[maybe_unused]] auto nodiscard_var = equivalent(*pathRawPtr, path);
    }

    CoveoFilesystem::BasePathContains(path.u16string(), pathPtr->u16string());

    {
        [[maybe_unused]] auto nodiscard_var = relative(*pathPtr, path);
    }

    create_directory_symlink(*pathPtr, path);

    create_symlink(*pathPtr, path);

    // ---

    CoveoFilesystem::DeleteFiles(path, "*.txt");
    CoveoFilesystem::DeleteFiles(*pathPtr, "*.txt");
    CoveoFilesystem::DeleteFiles(*pathRawPtr, "*.txt");

    const std::u16string notLiteral = u"*.png";

    CoveoFilesystem::DeleteFiles(path, CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral));

    mutablePath /= path;
    *mutablePathPtr /= path;
    *mutablePathRawPtr /= path;

    path / path;
    *pathPtr / path;
    *pathRawPtr / path;

    // Replace method ChangeExtension to replace_extension
    mutablePath.replace_extension(CGLFile::Path{u".txt"});
    mutablePathPtr->replace_extension(CGLFile::Path{u".txt"});
    mutablePathRawPtr->replace_extension(CGLFile::Path{u".txt"});

    mutablePath.replace_extension(CGLFile::Path{u".txt"});

    mutablePath.replace_extension(CGLFile::Path{mutablePath.extension().u16string() + u".txt"});

    CoveoFilesystem::ReplaceAllExtensions(mutablePath, CGLFile::Path{u".txt"});
    CoveoFilesystem::ReplaceAllExtensions(*mutablePathPtr, CGLFile::Path{u".txt"});
    CoveoFilesystem::ReplaceAllExtensions(*mutablePathRawPtr, CGLFile::Path{u".txt"});

    // RecursiveCopy to copy
    const bool aBool{};

    copy(path, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    copy(*pathPtr, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    copy(*pathRawPtr, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);

    copy(path, *pathPtr, std::filesystem::copy_options::overwrite_existing);
    copy(path, *pathPtr);
    copy(path, *pathPtr, std::filesystem::copy_options::overwrite_existing);

    // Copy to copy_file
    copy_file(path, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    copy_file(*pathPtr, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    copy_file(*pathRawPtr, path, aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);

    copy_file(path, *pathPtr, std::filesystem::copy_options::overwrite_existing);
    copy_file(path, *pathPtr);
    copy_file(path, *pathPtr, std::filesystem::copy_options::overwrite_existing);

    mutablePath = read_symlink(path);
    mutablePath = read_symlink(*pathPtr);
    mutablePath = read_symlink(*pathRawPtr);

    // GetFileListSize to GetTotalFilesSize
    CoveoFilesystem::GetTotalFilesSize(path);
    CoveoFilesystem::GetTotalFilesSize(*pathPtr);
    CoveoFilesystem::GetTotalFilesSize(*pathRawPtr);

    CoveoFilesystem::GetTotalFilesSize(path, "*.txt");
    CoveoFilesystem::GetTotalFilesSize(path, "*.txt", CoveoFilesystem::FileListingOptions::Recursive);
    CoveoFilesystem::GetTotalFilesSize(path, "*.txt");
    CoveoFilesystem::GetTotalFilesSize(path, "*.txt", aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None);
    CoveoFilesystem::GetTotalFilesSize(path,
                                       CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral),
                                       aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None);

    // GetFileList method to function
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path,
                                                CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral),
                                                aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None,
                                                [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(*pathPtr,
                                                CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral),
                                                aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None,
                                                [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(*pathRawPtr,
                                                CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral),
                                                aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None,
                                                [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::Recursive, [](const Path&) { return false; });
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
    }
    {
        // Two calls with the same output vector
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList.insert(fileList.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        // Output vector initialize with constructor, hence not empty
        std::vector<Path> fileList(1);
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList.insert(fileList.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        // Output vector initialize with copy initialization, hence not empty
        std::vector<Path> fileList = {Path{}};
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList.insert(fileList.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        // Output vector modified
        std::vector<Path> fileList;
        fileList.emplace({});
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList.insert(fileList.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        // Output vector is a reference
        std::vector<Path>  fileList;
        std::vector<Path>& fileList2 = fileList;
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList2.insert(fileList2.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        // Output vector is a pointer
        std::vector<Path>  fileList;
        std::vector<Path>* fileList2 = &fileList;
        {
            std::vector<Path> tmp = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::None, [](const Path&) { return false; });
            fileList2->insert(fileList2->end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
        };
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt");
    }
    {
        std::vector<Path> fileList;
        fileList =
            CoveoFilesystem::GetFileList(path, ".txt", aBool ? CoveoFilesystem::FileListingOptions::Recursive : CoveoFilesystem::FileListingOptions::None);
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt", CoveoFilesystem::FileListingOptions::Recursive);
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path, ".txt");
    }
    {
        std::vector<Path> fileList;
        fileList = CoveoFilesystem::GetFileList(path);
    }

    // MoveFileOnly to MoveFile
    CoveoFilesystem::MoveFile(path.u16string(),
                              path.u16string(),
                              aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    CoveoFilesystem::MoveFile(pathPtr->u16string(),
                              path.u16string(),
                              aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    CoveoFilesystem::MoveFile(pathRawPtr->u16string(),
                              path.u16string(),
                              aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);

    CoveoFilesystem::MoveFile(path.u16string(), pathPtr->u16string(), std::filesystem::copy_options::overwrite_existing);
    CoveoFilesystem::MoveFile(path.u16string(), pathPtr->u16string());
    CoveoFilesystem::MoveFile(path.u16string(), pathPtr->u16string());

    // Move to MoveFile with mutable path
    mutablePath        = CoveoFilesystem::MoveFile(mutablePath.u16string(),
                                            path.u16string(),
                                            aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    *mutablePathPtr    = CoveoFilesystem::MoveFile(mutablePathPtr->u16string(),
                                                path.u16string(),
                                                aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);
    *mutablePathRawPtr = CoveoFilesystem::MoveFile(mutablePathRawPtr->u16string(),
                                                   path.u16string(),
                                                   aBool ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none);

    mutablePath = CoveoFilesystem::MoveFile(mutablePath.u16string(), pathPtr->u16string(), std::filesystem::copy_options::overwrite_existing);
    mutablePath = CoveoFilesystem::MoveFile(mutablePath.u16string(), pathPtr->u16string());
    mutablePath = CoveoFilesystem::MoveFile(mutablePath.u16string(), pathPtr->u16string());

    {
        [[maybe_unused]] const uint64_t x = space(path).available;
    }
    {
        [[maybe_unused]] const uint64_t x = space(*pathPtr).available;
    }
    {
        [[maybe_unused]] const uint64_t x = space(*pathRawPtr).available;
    }

    {
        [[maybe_unused]] const uint64_t x = space(path).capacity;
    }
    {
        [[maybe_unused]] const uint64_t x = space(*pathPtr).capacity;
    }
    {
        [[maybe_unused]] const uint64_t x = space(*pathRawPtr).capacity;
    }

    // GetDiskSpaceStats to space
    {
        [[maybe_unused]] uint64_t available, total;
        {
            const std::filesystem::space_info tmp = space(path);
            available                             = tmp.available;
            total                                 = tmp.capacity;
        };
    }
    {
        [[maybe_unused]] uint64_t available, total;
        {
            const std::filesystem::space_info tmp = space(*pathPtr);
            available                             = tmp.available;
            total                                 = tmp.capacity;
        };
    }
    {
        [[maybe_unused]] uint64_t available, total;
        {
            const std::filesystem::space_info tmp = space(*pathRawPtr);
            available                             = tmp.available;
            total                                 = tmp.capacity;
        };
    }
    {
        [[maybe_unused]] auto available = std::make_unique<uint64_t>();
        [[maybe_unused]] auto total     = std::make_unique<uint64_t>();
        {
            const std::filesystem::space_info tmp = space(path);
            *available.get()                      = tmp.available;
            *total.get()                          = tmp.capacity;
        };
    }
    {
        [[maybe_unused]] uint64_t available;
        {
            const std::filesystem::space_info tmp = space(path);
            available                             = tmp.available;
        };
    }
    {
        [[maybe_unused]] uint64_t total;
        {
            const std::filesystem::space_info tmp = space(path);
            total                                 = tmp.capacity;
        };
    }

    {
        current_path(path);
    }

    {
        [[maybe_unused]] const Path anotherPath = CGLFile::Path{std::filesystem::current_path()};
    }

    {
        [[maybe_unused]] const Path anotherPath = CGLFile::Path{std::filesystem::temp_directory_path()};
    }

    // TempFileName to CreateTempFile
    {
        [[maybe_unused]] const Path anotherPath = CoveoFilesystem::CreateTempFile(path,
                                                                                  CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral),
                                                                                  CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral));
    }
    {
        [[maybe_unused]] const Path anotherPath = CoveoFilesystem::CreateTempFile(path, CGLCharset::Conv::ConvertU16ToU8StringCopy(notLiteral), ".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = CoveoFilesystem::CreateTempFile(path, "template", ".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = CoveoFilesystem::CreateTempFile(notLiteral, "template", ".txt");
    }
    {
        [[maybe_unused]] const Path anotherPath = CoveoFilesystem::CreateTempFile(CGLFile::Path{u"/arbitrary"}, "template", ".txt");
    }

    {
        [[maybe_unused]] std::u16string result;
        result = CGL::Unicode::ToLowerCaseCopy(notLiteral);
    }

    {
        [[maybe_unused]] std::u16string result = u"arbitrary";
        result                                 = CGL::Unicode::ToLowerCaseCopy(result);
    }

    delete pathRawPtr;
    delete mutablePathRawPtr;
}

}  // namespace
