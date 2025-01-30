# vDrop

In my free time I make personal tools like this one, a CLI tool for fast directory search and navigation.  

It's not big or complex, just simple. It searches for subdirectories in the current working directory, and when it finds the directory you want to visit, it takes you there.  

My motivation was that I was too lazy to keep navigating through folders like any Linux user, and too lazy to search for an existing tool. So I created this and shared it. I'll probably post any other tools I make in the future.  

### Build  

To build, just run:  

```
$ make
```  

Or manually compile with:  

```
$ gcc src/vdrop.c -o vdrop
```  

Add the binary to your local bin directory or shell aliases.  

To clean the build:  

```
$ make clean
```  

### Note  

If you're searching inside a giant directory like `home` or the root directory `/`, it might take some time. I usually know where the folder I'm looking for is, so I enter the first folder manually to make the search smaller. You can do the same, or contribute to add support for memorizing paths for future use.
