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

class Path
{
private:
    typedef std::function<bool(const Path&)> FilterCallback;

public:
    enum ChangeExtensionMode { APPEND, REPLACE_LAST, REPLACE_ALL };
    static constexpr auto PATH_DEFAULT_FILTER = u"*";

    Path()
    {
    }
    Path(const std::string&)
    {
    }
    Path(const std::u16string&)
    {
    }
    Path(const std::filesystem::path&)
    {
    }
    explicit Path(const char*)
    {
    }
    explicit Path(const char16_t*)
    {
    }

    Path& operator=(const std::string&)
    {
        return *this;
    }
    Path& operator=(const std::u16string&)
    {
        return *this;
    }
    Path& operator=(const char*)
    {
        return *this;
    }
    Path& operator=(const char16_t*)
    {
        return *this;
    }

    bool Equivalent(const Path&) const
    {
        return {};
    }
    bool Equivalent(const std::string&) const
    {
        return {};
    }
    bool Equivalent(const std::u16string&) const
    {
        return {};
    }
    bool operator==(const Path&) const
    {
        return {};
    }
    bool operator==(const std::string&) const
    {
        return {};
    }
    bool operator==(const std::u16string&) const
    {
        return {};
    }
    bool operator!=(const Path&) const
    {
        return {};
    }
    bool operator!=(const std::string&) const
    {
        return {};
    }
    bool operator!=(const std::u16string&) const
    {
        return {};
    }
    bool operator<(const Path&) const
    {
        return {};
    }
    bool operator<(const std::string&) const
    {
        return {};
    }
    bool operator<(const std::u16string&) const
    {
        return {};
    }

    // Accessors and modifiers
    const std::u16string FullPath(bool = false) const
    {
        return {};
    }
    std::u16string u16string() const
    {
        return {};
    }
    const std::u16string GenericFullPath() const
    {
        return {};
    }
    std::u16string generic_u16string() const
    {
        return {};
    }
    const std::string FullPathA() const
    {
        return {};
    }
    std::string string() const
    {
        return {};
    }
    const std::string GenericFullPathA() const
    {
        return {};
    }
    std::string generic_string() const
    {
        return {};
    }

    std::u16string Directory() const
    {
        return {};
    }
    Path parent_path() const
    {
        return {};
    }
    std::u16string FileName() const
    {
        return {};
    }
    Path filename() const
    {
        return {};
    }
    std::u16string FileNameNoExtension() const
    {
        return {};
    }
    Path stem() const
    {
        return {};
    }
    std::u16string FileExtension() const
    {
        return {};
    }
    Path extension() const
    {
        return {};
    }

    std::u16string Root() const
    {
        return {};
    }
    Path root_path() const
    {
        return {};
    }
    bool Empty() const
    {
        return {};
    }
    bool empty() const
    {
        return {};
    }
    void Clear()
    {
    }
    void clear()
    {
    }
    uint64_t Size() const
    {
        return {};
    }

    const CGL::HighPrecisionDateTime GetLastModificationDate() const
    {
        return {};
    }

    bool Exists() const
    {
        return {};
    }
    bool HasDrive() const
    {
        return {};
    }
    bool IsDirectory(bool = true) const
    {
        return {};
    }
    bool IsSymbolicLink() const
    {
        return {};
    }
    bool IsFile() const
    {
        return {};
    }
    bool IsReadOnly() const
    {
        return {};
    }
    bool IsWritable() const
    {
        return {};
    }
    bool IsAbsolute() const
    {
        return {};
    }
    bool is_absolute() const
    {
        return {};
    }
    bool IsRelative() const
    {
        return {};
    }
    bool is_relative() const
    {
        return {};
    }

    void Append(const Path&)
    {
    }
    void Append(const std::string&)
    {
    }
    void Append(const std::u16string&)
    {
    }
    template<class Source> Path& operator/=(const Source&)
    {
        return *this;
    }
    Path Combine(const Path&) const
    {
        return {};
    }
    Path Combine(const std::string&) const
    {
        return {};
    }
    Path Combine(const std::u16string&) const
    {
        return {};
    }
    Path& ChangeExtension(const std::u16string&, ChangeExtensionMode = REPLACE_LAST)
    {
        return *this;
    }
    Path& replace_extension(const Path& = Path())
    {
        return *this;
    }
    bool Contains(const Path&) const
    {
        return {};
    }
    Path GetRelativePath(const Path&) const
    {
        return {};
    }
    Path GetCanonicalPath() const
    {
        return {};
    }

    // Disk operation functions
    void CreateDir() const
    {
    }
    void CreateFile() const
    {
    }
    void CreateDirectorySymbolicLink(const Path&) const
    {
    }
    void CreateFileSymbolicLink(const Path&) const
    {
    }
    void DeleteSymbolicLink() const
    {
    }
    void ReadSymbolicLink(Path&) const
    {
    }
    void Delete() const
    {
    }
    void DeleteNoRetry() const
    {
    }
    void RecursiveDelete() const
    {
    }
    void RecursiveCopy(const Path&, bool = true) const
    {
    }
    void Copy(const Path&, bool = true) const
    {
    }

    uint64_t GetFileListSize(const std::u16string& = PATH_DEFAULT_FILTER, bool = false) const
    {
        return {};
    }
    void GetFileList(std::vector<Path>&, const std::u16string& = PATH_DEFAULT_FILTER, bool = false, FilterCallback = nullptr) const
    {
    }
    void DeleteFileList(const std::u16string&) const
    {
    }
    void Move(const Path&, bool = false)
    {
    }
    Path MoveFileOnly(const Path&, bool = false) const
    {
        return {};
    }
    uint64_t GetAvailableDiskSpace() const
    {
        return {};
    }
    uint64_t GetTotalDiskSpace() const
    {
        return {};
    }
    void GetDiskSpaceStats(uint64_t*, uint64_t*) const
    {
    }

    // Static functions
    static Path CurrentPath()
    {
        return {};
    }
    static void CurrentPath(const Path&)
    {
    }
    static Path TempPath()
    {
        return {};
    }
    static Path TempFileName(const Path&, const std::u16string&, const std::u16string&)
    {
        return {};
    }
    static Path TempFileName(const std::u16string&, const std::u16string&, const std::u16string&)
    {
        return {};
    }
    static void ToLowerCase(const std::u16string&, std::u16string&)
    {
    }
    static void ToLowerCaseInPlace(std::u16string&)
    {
    }
};

inline Path canonical(const Path&)
{
    return {};
}
inline bool equivalent(const Path&, const Path&)
{
    return {};
}
inline bool exists(const Path&)
{
    return {};
}
inline bool is_directory(const Path&)
{
    return {};
}
inline bool is_regular_file(const Path&)
{
    return {};
}
inline bool is_symlink(const Path&)
{
    return {};
}
inline std::uintmax_t file_size(const Path&)
{
    return {};
}
inline Path relative(const Path&, const Path&)
{
    return {};
}
template<class Source> Path operator/(const Path&, const Source&)
{
    return {};
}

inline bool create_directories(const Path&)
{
    return {};
}
inline void create_symlink(const Path&, const Path&)
{
}
inline void create_directory_symlink(const Path&, const Path&)
{
}
inline bool remove(const Path&)
{
    return {};
}
inline Path read_symlink(const Path&)
{
    return {};
}
inline std::uintmax_t remove_all(const Path&)
{
    return {};
}
inline bool copy_file(const Path&, const Path&, std::filesystem::copy_options = std::filesystem::copy_options{})
{
    return {};
}
inline void copy(const Path&, const Path&, std::filesystem::copy_options = std::filesystem::copy_options{})
{
}
inline std::ostream& operator<<(std::ostream& out, const Path&)
{
    return out;
}

inline std::filesystem::space_info space(const Path&)
{
    return {};
}
inline Path current_path()
{
    return {};
}
inline void current_path(const Path&)
{
}
inline Path temp_directory_path()
{
    return {};
}

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
