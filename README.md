# File Search Engine
A multithreaded search engine in C++ with a JavaScript web interface that indexed hundreds of files using
POSIX/IO. Run through the http333d executable in ./hw4 </br>
A class assignment for CSE 333
## Running the Search Engine
Note: This has only been tested running on a Linux terminal</br>
Run the following command from the hw4 directory: ./http333d (port number) (file directory) (files to index through)</br>
An example looks like this (with the included files within projdocs): ./http333d 12345 ../projdocs/ ../projdocs/unit_test_indices/* </br>
Access the web interface through the given port number. It provides a search engine to look through words, or multiple words. </br>
- If searching through with multiple words, it finds files containing both words, but not exclusively in the same order as searched (i.e. searching "hello world" will find documents containing both "hello" and "world")
