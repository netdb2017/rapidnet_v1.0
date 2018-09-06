
Rapidnet
========

Using the data-structures
-------------------------

`-I`: indicates the location of the header files

`-l`: indicates we want to use the CVC4 libraries

`-L`: where the implementation of the header files can be found

    Run g++ -I<path-to-rapidnet>/src/rapidnet-compiler -<path-to-rapidnet>/lkloh/rapidnet/src/rapidnet-compiler <file-using-data-structures>.cc -o <file-using-data-structures> -lcvc4

Example: I clone this folder to my home directory, with path `/home/lkloh`. To run the file `hellpeworld.cpp` which sources the data structure files from rapidnet, I cd into the location where `helloworld.cpp` may be found, and type:

    g++ -I/home/lkloh/rapidnet/src/rapidnet-compiler -L/home/lkloh/rapidnet/src/rapidnet-compiler helloworld.cpp -o helloworld -lcvc4

Remove DS_Store before pushing.
-------------------------------

At the top level of the repository, type

    find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch
    
As suggested [here](http://stackoverflow.com/questions/107701/how-can-i-remove-ds-store-files-from-a-git-repository).


Installation Instructions
-------------------------
* [Installing on MacOS](https://github.com/lkloh/rapidnet/wiki/Installing-On-MacOS)
* [Installing on Fedora](https://github.com/powerist/rapidnet/wiki/Installing-on-Fedora-Cluster-Machine)


Issue with Building RapidNet on Ubuntu 16.04
----------------------------------------------

There is an issue with building rapidnet on Ubuntu 16.04 (and higher). If one tries to build using ./waf An error occurs in the Scripting.py file. To resolve this issue, either change line 190 in Tools/python.py from

``` 
"for incstr in Utils.cmd_output(\"%s %s --includes\"%(python,python_config)).strip().split():"
```

to

```
"for incstr in Utils.cmd_output(\"%s --includes\"%(python_config)).strip().split():
```

or use build.sh to build the project initially.

How to run a NDLog program from outside the folder
--------------------------------------------------

In the command line, cd into the rapidnet folder. Get the entire path the the NDLog file you want to run `<path-to-file>/<My-NDLog-Program>.olg`. Run 

    ./rapidnet/dpcompiler/dpcompile <path-to-file>/<My-NDLog-Program>.olg
    
In NDLog Programs, all string constants must be assigned a value larger than 1000 using 

    #define <STR-CONSTANT-NAME> value
    
where value > 1000,  so we know it is a string constant. (March 12, 2015)

SMT-LIB Resources
-----------------
* [SMT-LIB Official Website](http://smtlib.cs.uiowa.edu/)
* [String Declaration](http://cvc4.cs.nyu.edu/wiki/Strings)
* [Installin Z3 on MacOS](http://z3.codeplex.com/wikipage?title=Building%20the%20unstable%20%28working-in-progress%29%20branch&referringTitle=Documentation)

Running examples
----------------
* [Upenn Tutorial](http://netdb.cis.upenn.edu/rapidnet/doxygen/html/rapidnet-ndlog-application.html)
* Rapidnet-A small tutorial.pdf

Contact Us
--------------

For any queries or questions, kindly send us an email to netdb.seas.upenn@gmail.com. Thank you!
