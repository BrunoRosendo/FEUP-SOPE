**Compilation**

To compile simply do make on the src folder.
> make
 
**Running**

To run the program, call the executable, followed by the flags, permissions and file or directory to apply to.
For example.
> ./xmod -R -c -v a=rwx test.txt
In a more general way
> ./xmod <[-R|-c|-v]> <[a|u|g|o][+|=|-][-|r|w|x] <file or directory>

-R: Uses recursion on the directory
-v: Provides extra information about every step taken
-c: Provides extra information when a change to the permissions is made

 a: Sets the permission for all users
 u: Sets the permission for the user who owns the file
 g: Sets the permission for other users in the file group
 o: Sets the permission for users not in the file group

 r: Sets reading permission
 w: Sets writing permission
 x: Sets executing permission
   
**Errors and input**

There must always be at least 3 arguments (including the program call). The last and second to last will be the file or directory to be affected and the permissions, respectively.
Failure to provide a valid file or directory will yield an error
Failure to provide a valid mode will yield an error
Providing wrong flags will yield an error.
Trying to terminate the program will prompt the user
  
  **Implementation**
  
1. Set the logfile and signals
2. Parse flags used
3. Get an octal number from the permissions
4. Apply the permission to the given file or directory

When the -R (recursive) flag is applied. Changing the permissions of a directory will be done by forking the proccess. Since a brand new version is created without the same memory space, environment variables are used to verify if the program is a child.
  
  **Self-evaluation**
  
Henrique Sousa up201906681: 12
Domingos Santos up2019
Bruno Rosendo up2019
João Mesquita up2019
