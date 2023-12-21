#pragma once

#include <cstdint>  // uint64_t
#include <filesystem>
#include <functional>  // std::function
#include <iostream>
#include <string>
#include <vector>

namespace CGL {

struct HighPrecisionDateTime {
};

}  // namespace CGL

namespace CGLFile {

using Path = std::filesystem::path;

}  // namespace CGLFile

namespace CoveoFilesystem {

enum class FileListingOptions : unsigned short {
    None,
    Recursive,
};
inline bool BasePathContains(const std::filesystem::path&, const std::filesystem::path&)
{
    return {};
}
inline std::string ConvertPathToLowerCase(const CGLFile::Path&)
{
    return {};
}
inline void CreateEmptyFile(const CGLFile::Path&)
{
}
inline CGLFile::Path CreateTempFile(const CGLFile::Path&, const std::string&, const std::string&)
{
    return {};
}
inline void DeleteFiles(const CGLFile::Path&, const std::string&)
{
}
inline std::vector<CGLFile::Path> GetFileList(const CGLFile::Path&,
                                              const std::string&                        = "*",
                                              FileListingOptions                        = FileListingOptions::None,
                                              std::function<bool(const CGLFile::Path&)> = nullptr)
{
    return {};
}
inline CGL::HighPrecisionDateTime GetLastModificationDate(const CGLFile::Path&)
{
    return {};
}
inline uint64_t GetTotalFilesSize(const CGLFile::Path&, const std::string& = "*", FileListingOptions = FileListingOptions::None)
{
    return {};
}
inline bool HasDrive(const CGLFile::Path&)
{
    return {};
}
inline bool IsReadOnly(const CGLFile::Path&)
{
    return {};
}
inline bool IsWritable(const CGLFile::Path&)
{
    return {};
}
inline CGLFile::Path MoveFile(const std::filesystem::path&, const std::filesystem::path&, std::filesystem::copy_options = std::filesystem::copy_options{})
{
    return {};
}
inline CGLFile::Path ReplaceAllExtensions(CGLFile::Path, const CGLFile::Path& = CGLFile::Path())
{
    return {};
}

}  // namespace CoveoFilesystem
