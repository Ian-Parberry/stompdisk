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

  uint64_t n = 0; //for the number
  std::wstring wstr; //for the input line
  std::getline(std::wcin, wstr); //input a line, hopefully with a number in it

  if(IsNumericString(wstr)) //if it's a number
    n = std::stoull(wstr); //convert from string to uint64_t

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
/// Create large file filled with zeros super-fast under Windows.
/// \param nWanted [in] Number of GB wanted.
/// \param nCreated [out] Number of GB actually created.
/// \param wstrFile [out] Name of file created.
/// \return DWORD error code, zero if no error.

DWORD Stomp(const uint64_t nWanted, uint64_t& nCreated, std::wstring& wstrFile){
  const uint64_t nBytesInGB = 1073741824; //bytes in a gigabyte
  const uint64_t nBytes = nBytesInGB*nWanted; //convert GB to bytes
  LARGE_INTEGER nSize; //thanks for being weird, Microsoft
  nSize.QuadPart = nBytes; //convert to bytes
  DWORD dwErr = 0; //error code, initially no error
  uint64_t nFileNum = 0; //number in file name

  GetNextFile(nFileNum, wstrFile); //next file number and name

  const HANDLE hFile = CreateFile(wstrFile.c_str(), GENERIC_WRITE,
    FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  dwErr = GetLastError(); //check for error

  if(SUCCEEDED(dwErr)){ //file has been opened, hFile is valid
    SetFilePointerEx(hFile, nSize, 0, FILE_BEGIN); //set file ptr to start
    dwErr = GetLastError(); //check for error

    if(SUCCEEDED(dwErr)){ //no error
      SetEndOfFile(hFile); 
      dwErr = GetLastError(); //check for error
    } //if
    GetFileSizeEx(hFile, &nSize); //read back file size
    nCreated = nSize.QuadPart/nBytesInGB; //report back file size

    CloseHandle(hFile); //close valid file handle
  } //if

  return dwErr; //error code, zero if no error
} //Stomp

/// \brief Main.
///
/// Get number of files and file size from user and create the files. Print
/// an error string if something went wrong.
/// \return 0 (What could possibly go wrong?)

int main(){
  std::wcout << L"StompDisk: Create a large file of zeros very very fast.\n";

  uint64_t nWanted = 0; //number of GB wanted
  uint64_t nCreated = 0; //number of GB in created file
  std::wstring wstr; //for file name

  while(nWanted == 0) //get nWanted from user
    nWanted = ReadNumber(L"Enter file size in GB: ");

  const DWORD dwErr = Stomp(nWanted, nCreated, wstr); //create file

  if(SUCCEEDED(dwErr)){ //no error reported, check size to be sure
    std::wcout << L"Created " << nCreated << L" GB file " << wstr << std::endl;
    
    if(nCreated != nWanted)
      std::wcout << L"Error: Something went wrong!" << std::endl;
  } //if

  else{ //error report
    std::wcout << L"Error!" << std::endl;
    std::wcout << GetErrorString(dwErr);
  } //if

  system("pause"); //wait for user

  return 0; //what could possibly go wrong?
} //main
