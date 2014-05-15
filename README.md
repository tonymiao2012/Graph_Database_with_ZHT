This is a graph database built on top of ZHT, a newly developed highly efficient zero-hop distibuted hash table. ZHT serves as a key-value store and offers APIs to the graph database for storage and search engine.

How to config on terminal:

1, make sure export your LD_LIBRARY_PATH to /usr/local/lib, where protobuf installed.

2, Since we have already offered the Makefile, type make will make the graphdb binary, type make test will make the test_graphdb binary.

3, Make sure the zhtserver is running. About how to run zhtserver, see Troubleshoot below.

How to config on Eclipse:

1, First you should have zht, google protobuf, prtobuf-c installed

2, Create an c++ project.

3, Include directory/to/graph/include

4, If google protobuf is not installed default in usr/local/include, include it as well

5, Set library search path to directory/to/graph/lib

6, If google protobuf is not installed default in usr/local/lib, add library search path to it as well.

7, Set library in exact the same order as: zht, protobuf, protobuf-c

8, If lib zht is not comming as the first, there will be compilation error.

9, Add -std=c++0x to C++ compiler option.

10, Add __GXX_EXPERIMENTAL_CXX0X__ to symbol.

11, Now compile and run the command_line_main.cpp, end.

12, Since there are multiple main functions in the project, you can build one project for each main function. And assign one as the container project, which contains all the parts of the graph. And in this proejct property, assign project references to each sub-project. This will solve the problem most.

TroubleShooting:

1, If you encounter a ZHT init fails, please check if zht.conf and neighbor.conf is placed correctly so the binary can find them. Usually they are at the project root directory. If you launch binary in Eclipse, it is fine. But if you launch binary from terminal, make sure the config files and binary are in the same directory.

2, If you encounter a bad file discriptor failure, please check if zhtserver is running. Cd to zht directory, and make and ./zhtserver -z zht.conf -n neighbor.conf

3, If your eclipse cannot index std::to_string, open the project property -> c/c++ Build -> Settings -> GCC C++ Compiler -> Miscellaneous -> Other flags. Add -std=c++0x. And in C/C++ General -> Paths and Symbols -> Symbols -> GNU C++, Add "__GXX_EXPERIMENTAL_CXX0X__"

Reference:
http://docs.neo4j.org/chunked/stable/tutorials-java-embedded-hello-world.html#_prepare_the_database
