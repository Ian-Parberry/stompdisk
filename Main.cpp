/// \file Main.cpp
/// \brief The only code file.

#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>

#include "Windows.h"

/// \brief File exists.
///
/// Test whether a file exists.
/// \param wstrFileName File name.
/// \return true if the file exists.

bool FileExists(const std::wstring& wstrFileName){
  const DWORD dw = GetFileAttributesW(wstrFileName.c_str());
  return dw != INVALID_FILE_ATTRIBUTES && !(dw & FILE_ATTRIBUTE_DIRECTORY);
} //FileExists

/// \brief Get next file number and name.
///
/// This function is used to get the next unused file name, where the file
/// name is a string followed by a number. If the first such file exists, then
/// the number is incremented until the file does not exist.
/// \param n [in, out] Next unused file number.
/// \param wstrFileName [out] File name.

void GetNextFile(uint64_t& n, std::wstring& wstrFileName){
  bool bExists = false;

  do{
    wstrFileName = L"stomp" + std::to_wstring(n) + L".bin"; //file name
    bExists = FileExists(wstrFileName);
    if(bExists)n++;
  }while(bExists);
} //GetNextFile

/// \brief Numeric string test.
///
/// Test whether an `std::wstring` contains a numeric string, that is,
/// a string of digits 0 through 9.
/// \param s String to test.
/// \return true if the string is a numeric string.

bool IsNumericString(const std::wstring& s){
  return !s.empty() && s.find_first_not_of(L"0123456789") == std::wstring::npos;
} //IsNumericString

/// \brief Read a number.
///
/// Prompt the user and then read an unsigned 64-bit number from `std::wcin`.
/// \param wstrBanner Banner to print before reading the number.
/// \return The number input, equal to zero if something goes wrong.

uint64_t ReadNumber(std::wstring wstrBanner){
  std::wcout << wstrBanner << std::endl << "> ";

  uint64_t n = 0;
  std::wstring wstr;
  std::getline(std::wcin, wstr);

  if(IsNumericString(wstr))
    n = std::stoull(wstr);

  return n;
} //ReadNumber

/// \brief Get error string.
///
/// Convert `DWORD` error code into `std::wstring` describing the error.
/// \param dw Error code.
/// \return Error string.

std::wstring GetErrorString(const DWORD dw){ 
  LPVOID lpMsgBuf = nullptr; //message buffer for Windows API function

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | //get error message to lpMsgBuf
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, 0);

  std::wstring wstr((LPTSTR) &lpMsgBuf); //convert to wstring
  LocalFree(lpMsgBuf); //free the message buffer
  return wstr;
} //GetErrorString

/// \brief Create files of zeros.
///
/// Create large files filled with zeros super-fast under Windows.
/// \param m Number of files.
/// \param n Number of GB per file.
/// \return DWORD error code, zero if no error

DWORD Stomp(const uint64_t m, const uint64_t n){
  LARGE_INTEGER nSize; //thanks for being weird, Microsoft
  nSize.QuadPart = 1073741824*n; //convert to bytes
  DWORD dwErr = 0; //error code, initially no error
  uint64_t nFileNum = 0; //number in file name

  for(uint64_t i=0; i<m && SUCCEEDED(dwErr); i++){ //for each file
    std::wstring wstrFileName; //for file name
    GetNextFile(nFileNum, wstrFileName); //next file number and name

    const HANDLE hFile = CreateFile(wstrFileName.c_str(), GENERIC_WRITE,
      FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    dwErr = GetLastError();

    if(SUCCEEDED(dwErr)){ //file has been opened, hFile is valid
      SetFilePointerEx(hFile, nSize, 0, FILE_BEGIN); //set file ptr to start of file
      dwErr = GetLastError();

      if(SUCCEEDED(dwErr)){ //this should always succeed
        SetEndOfFile(hFile); 
        dwErr = GetLastError();
      } //if

      CloseHandle(hFile);
    } //if
  } //for

  return dwErr; //error code, zero if no error
} //Stomp

/// \brief Main.
///
/// Get number of files and file size from user and create the files. Print
/// an error string if something went wrong and a cheerful message otherwise.
/// \return 0 (What could possibly go wrong?)

int main(){
  std::wcout << L"Create large files of zeros." << std::endl;

  uint64_t m = 0; //number of files
  uint64_t n = 0; //number of GB per file

  //get values of m and n from user

  while(n == 0)
    n = ReadNumber(L"Enter file size in GB: ");

  while(m == 0)
    m = ReadNumber(L"Enter Number of files: ");

  //create files and report

  const DWORD dwErr = Stomp(m, n);

  if(SUCCEEDED(dwErr))
    std::wcout << L"Created " << std::to_wstring(m) << 
      L" files of size " << std::to_wstring(n) << L"GB" << std::endl;
  else{
    std::wcout << L"Error!" << std::endl;
    std::wcout << GetErrorString(dwErr);
  } //else

  return 0;
} //main
