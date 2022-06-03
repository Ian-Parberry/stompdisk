#include <cstdio>
#include <cstdint>
#include <string>

#include "Windows.h"

/// \brief Create files of zeros.
///
/// Create large files filled with zeros super-fast under Windows.
/// \param m Number of files.
/// \param n Number of GB per file.

DWORD Stomp(const uint64_t m, const uint64_t n){
  LARGE_INTEGER nSize; //thanks for being weird, Microsoft
  nSize.QuadPart = 1073741824*n; //convert to bytes

  for(uint64_t i=0; i<m; i++){ //for each file
    const std::wstring wstrFileName = L"stomp" + std::to_wstring(i); //file name

    HANDLE hFile = CreateFile(wstrFileName.c_str(), GENERIC_WRITE,
      FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    const DWORD dwErr = GetLastError();

    if(dwErr > 0) //fail
      return dwErr;

    else{ //success
      SetFilePointerEx(hFile, nSize, 0, FILE_BEGIN); 
      SetEndOfFile(hFile);
      CloseHandle(hFile);
    } //else
  } //for

  return 0; //success
} //Stomp

/// \brief Main.
///
/// Main.
/// \return 0 (What could possibly go wrong?)

int main(){
  const uint64_t m = 2; //number of files
  const uint64_t n = 100; //number of GB per file

  if(SUCCEEDED(Stomp(m, n)))
    printf("Created %zu files of size %zu\n", m, n);
  else
    printf("Error!\n");

  return 0;
} //main
