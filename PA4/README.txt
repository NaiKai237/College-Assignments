Ian Bell
DFS PA4

iabe3069_dfs.c and iabe3069_dfc.c contain the source code for the distributed file system

To compile run 'make' command
-performs 'gcc iabe3069_dfc.c -o dfc.c' and 'gcc iabe3069_dfs.c -o dfc'

'make clean' removes the executable objects

To run the distributed file system enter:
'./dfs /DFS1 10001 &'
'./dfs /DFS2 10002 &'
'./dfs /DFS3 10003 &'
'./dfs /DFS4 10004 &'
'./dfc dfc.conf'

If you want to move the program to different directories it is important that the subdirectories listed in the commands exhist with the dfs executable
dfc.conf needs to be with the dfc executable and dfs.conf needs to be with the dfs executable.
It is important to update dfc.conf with the appropriate server addresses and user information.
dfs.conf contains all possible User/Pass that are approved.

To use the distributed file system enter your command into the prompt given by the client program.
	get <filename>
		-retrieves the file parts from the server and reassembles them
		-if all parts are not available a message is printed
	list
		-lists all file that exist in the distributed system and if they are complete/read to get
	put <filename>
		-breaks a file into parts and sends to the appropriate servers.
	exit
		-closes the client program

Most of the functionality of the code is simply passing messages back and forth between client and servers,
then parsing those messages, and taking the appropriate actions.

The parts are broken up and sent to specific servers based on the md5hash sum of the contents of the original file.
-First file size is determined, then memory for parts are created based on the total size (any remainding size is added to the first part).
-The file is read into the parts, and the parts are sent to the servers which saves them to the local directory. 