// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

#include <tuple>
// #include <cstdint>
#include <cstdlib>
#include <sys/types.h>
#include "_compile.hxx"

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
  #include <winbase.h>
  #include <winnt.h>
  #include <basetsd.h>
  #include <fileapi.h>
  #include <handleapi.h>
  #include <minwindef.h>
  #include <io.h>
  #include <fcntl.h>
#else
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  #include <errno.h>
#endif




// An internal namespace helps to hide implementation details.
// This is particularly useful for pre-C++20 modules.
namespace gve {
namespace detail {
using std::tuple;
#if defined(_WIN32) || defined(_WIN64)
typedef HANDLE mmap_fd_t;
#define GVE_BAD_FD INVALID_HANDLE_VALUE
#else
typedef int mmap_fd_t;
#define GVE_BAD_FD -1
#endif




#pragma region MEMORY MAPPED FILE
/**
 * Map a file to memory.
 * @param pth file path
 * @returns file descriptor, mapped data, and file size
 */
inline tuple<mmap_fd_t, void*, size_t> mmapOpenFile(const char *pth) {
  #if defined (_WIN32) || defined (_WIN64)
  HANDLE fd = CreateFileA(pth, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (fd == INVALID_HANDLE_VALUE) return {INVALID_HANDLE_VALUE, nullptr, 0};
  // Get file size.
  LARGE_INTEGER sizes;
  if (!GetFileSizeEx(fd, &sizes)) {
    CloseHandle(fd);
    return {INVALID_HANDLE_VALUE, nullptr, 0};
  }
  SIZE_T size = (SIZE_T) sizes.QuadPart;
  // Map file to memory.
  HANDLE map = CreateFileMappingA(fd, NULL, PAGE_READONLY, 0, 0, NULL);
  if (map == NULL) {
    CloseHandle(fd);
    return {INVALID_HANDLE_VALUE, nullptr, 0};
  }
  LPVOID view = MapViewOfFile(map, FILE_MAP_READ, 0, 0, 0);
  CloseHandle(map);
  if (view == NULL) {
    CloseHandle(fd);
    return {INVALID_HANDLE_VALUE, nullptr, 0};
  }
  return {fd, (void*) view, size};
  #else
  // Open file as read-only.
  int fd = open(pth, O_RDONLY);
  if (fd == -1) return {-1, nullptr, 0};
  // Get file size.
  struct stat sb;
  if (fstat(fd, &sb) == -1) return {-1, nullptr, 0};
  // Map file to memory.
  void *data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE | MAP_NORESERVE, fd, 0);  // MAP_SHARED?
  if (data == MAP_FAILED) return {-1, nullptr, 0};
  madvise(data, sb.st_size, MADV_WILLNEED);  // MADV_SEQUENTIAL?
  // Return file descriptor, mapped data, and file size.
  return {fd, data, sb.st_size};
  #endif
}


/**
 * Unmap a file from memory.
 * @param fd file descriptor
 * @param data mapped data
 * @param size file size
 */
inline void mmapCloseFile(mmap_fd_t fd, void *data, size_t size) {
  #if defined (_WIN32) || defined (_WIN64)
  UnmapViewOfFile(data);
  CloseHandle((HANDLE) fd);
  #else
  munmap(data, size);
  close(fd);
  #endif
}


/**
 * Represents a memory mapped file.
 */
struct MappedFile {
  #pragma region DATA
  private:
  /** Mapped data. */
  void  *_data;
  /** File size. */
  size_t _size;
  /** File descriptor. */
  mmap_fd_t _fd;
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Get mapped data (implicit conversion).
   */
  inline operator void*() const { return _data; }

  /**
   * Get mapped data.
   * @returns mapped data
   */
  inline void* data() const { return _data; }

  /**
   * Get file descriptor.
   * @returns file descriptor
   */
  inline mmap_fd_t fd() const { return _fd; }

  /**
   * Get file size.
   * @returns file size
   */
  inline size_t size() const { return _size; }

  /**
   * Unmap file from memory.
   */
  inline void close() {
    if (_fd == GVE_BAD_FD) return;
    mmapCloseFile(_fd, _data, _size);
    _fd   = GVE_BAD_FD;
    _data = nullptr;
    _size = 0;
  }
  #pragma endregion


  #pragma region CONSTRUCTORS / DESTRUCTORS
  public:
  /**
   * Map file to memory.
   * @param pth file path
   */
  MappedFile(const char *pth) {
    std::tie(_fd, _data, _size) = mmapOpenFile(pth);
  }

  /**
   * Unmap file from memory.
   */
  ~MappedFile() { close(); }
  #pragma endregion
};
#pragma endregion




#pragma region ALLOCATE MEMORY
/**
 * Allocate memory using mmap.
 * @param size memory size
 * @returns allocated memory
 */
inline void* mmapAlloc(size_t size) {
  #if defined (_WIN32) || defined (_WIN64)
  return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  #else
  return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  #endif
}


/**
 * Free memory allocated using mmap.
 * @param addr memory address
 * @param size memory size
 */
inline void mmapFree(void *addr, size_t size) {
  #if defined (_WIN32) || defined (_WIN64)
  VirtualFree(addr, 0, MEM_RELEASE);
  #else
  munmap(addr, size);
  #endif
}


/**
 * Represents a memory allocated with mmap().
 */
template <class T>
struct MappedPtr {
  #pragma region DATA
  private:
  /** Base address of allocated memory. */
  T     *_data;
  /** Size of allocated memory. */
  size_t _size;
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Get base address of allocated memory (implicit conversion).
   * @returns base address
   */
  inline operator T*() const { return _data; }

  /**
   * Get base address of allocated memory.
   * @returns base address
   */
  inline T* data() const { return _data; }

  /**
   * Get size of allocated memory.
   * @returns allocation size
   */
  inline size_t size() const { return _size; }

  /**
   * Unmap file from memory.
   */
  inline void release() {
    if (_data == nullptr) return;
    mmapFree(_data, _size);
    _data = nullptr;
    _size = 0;
  }
  #pragma endregion


  #pragma region CONSTRUCTORS / DESTRUCTORS
  public:
  /**
   * Create an empty allocation.
   */
  MappedPtr() : _data(nullptr), _size(0) {}

  /**
   * Allocate memory using mmap().
   * @param size size of memory to allocate
   */
  MappedPtr(size_t size) {
    _data = (T*) mmapAlloc(size);
    _size = size;
  }

  /**
   * Free memory allocated using mmap().
   */
  ~MappedPtr() { release(); }
  #pragma endregion
};
#pragma endregion
} // namespace detail
} // namespace gve




// Now, we export the public API.
EXPORT namespace gve {
} // namespace gve
