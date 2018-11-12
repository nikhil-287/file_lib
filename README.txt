Title : FILE Library
Name : Nikhil Santosh Koli
MIS : 111703031

Implemented following functions by using a buffer of size 512

a) myfopen
b) myfread
c) myfwrite
d) myfseek
e) myfsetpos
f) myfgetpos
g) myfeof
h) myftell
i) myfclose

Call to fclose is mandatory for proper functioning of the library
*fopen opens the file in asked mode;
*fread memcpys the data in the array passed. memcpy happens from buffer to array. If there is less data in buffer next 512 bytes are loaded in buffer
*fwrite writes the data in buffer. It is written in the file only when the buffer is full except myfclose.
*fseek seeks to the asked position(reloads the appropriate buffer if required)
*fgetpos generates a unique key for every file opened and stores the current file pointer position
*fsetpos works only when the key matches(You cannot call fsetpos before calling fgetpos) and moves the fileptrpos to positiong stored from fgetpos
*feof tells when the file ends
*ftell returns the current fileptrpos
*fclosewrites any residual data and frees all malloced memory 
