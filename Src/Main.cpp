/// \file Main.cpp
/// \brief Main.

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

//Winmm.lib

#include "Windows.h"

#include <cstdint>
#include <cstdlib>

#include <cinttypes>
#include <string>

#include <iostream>
#include <sstream>

#include "shishua.h"

/// \brief File exists.
///
/// Test whether a file exists.
/// \param wstrFileName File name.
/// \return true if the file exists.

bool FileExists(const std::wstring& wstrFileName){
  const DWORD dw = GetFileAttributesW(wstrFileName.c_str());
  return dw != INVALID_FILE_ATTRIBUTES && !(dw & FILE_ATTRIBUTE_DIRECTORY);
  return false;
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
    wstrFileName = L"stomp" + std::to_wstring(n) + L".dat"; //file name
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

/// \brief Main.
///
/// Get the file size from `stdin` and create a file of that many GB of
/// pseudo-random numbers.
/// \return 0 (What could possibly go wrong?)

int main(){
  std::cout << "Create a large file of pseudo-random bytes." << std::endl;
  const size_t nBufSize = 1073741824; //1GB buffer
  uint8_t* buffer = new uint8_t[nBufSize]; //buffer for pseudo-random numbers

  prng_state s; //state for shishua
  uint64_t seed[4] = {0}; //seed for shishua

  srand(timeGetTime());

  for(int i=0; i<4; i++)
    seed[i] = uint64_t(rand()) << 48 | uint64_t(rand()) << 32 |
      uint64_t(rand()) << 16 | uint64_t(rand());

  prng_init(&s, seed); //initialize shishua

  uint64_t n = 0; //file size in GB
  while(n == 0) //get n from user
    n = ReadNumber(L"Enter file size in GB: ");

  uint64_t nFile = 0; //output file number
  std::wstring wstrFile; //output file name
  GetNextFile(nFile, wstrFile); //get next output file number and name

  FILE* output = nullptr; //output file pointer
  _wfopen_s(&output, wstrFile.c_str(), L"wb"); //open output file for writing

  if(output == 0) //open failed
    std::cout << "Error opening file." << std::endl;
  
  else{ //output file opened successfully
    for(uint64_t i=0; i<n; i++){ //once for each GB of output
      prng_gen(&s, buffer, nBufSize); //generate 1GB using shishua
      fwrite(buffer, nBufSize, 1, output); //write to disk
      fflush(output);
      std::cout << "."; //to show user progress
    } //for
    
    std::cout << std::endl;
    fclose(output);
  } //if

  delete [] buffer;

  system("pause"); //wait for user

  return 0; //what could possibly go wrong?
} //main
