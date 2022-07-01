# StompDisk


<img src="https://github.com/Ian-Parberry/stompdisk/blob/main/logo300.png" alt="Logo."  width="25%"/>

When you delete a file under Windows, the file data on disk doesn't get
deleted, just the pointers that Windows uses to access the file.
Until that disk space is actually over-written by another file,
a moderately competent hacker with access to your old disk drive
could potentially reconstruct your files. This means that
before you sell, give away, or throw out your old computer or your old disk drive,
it's a good idea to over-write as much of the data on the disk as you can with noise.

`StompDisk` is a program for Windows that creates large files of pseudo-random noise,
created and maintained
by [Ian Parberry](http://ianparberry.com/).
For instructions on how to compile and grok the code, read the Doxygen-generated
[documentation](https://ian-parberry.github.io/stompdisk). 

## Requirements

Windows 10 and Visual C++.
This code has been tested with Visual Studio 2019 Community under Windows 10.

## License

This project is released under the [MIT License ](https://github.com/Ian-Parberry/stompdisk/blob/master/LICENSE).
