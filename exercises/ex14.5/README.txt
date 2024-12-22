Author: Ethan Kogan (dbzfan@uw.edu)

*** SUMMARY ***
Maps and Vectors and combinations of the two in the C++ Standard 
Template Library are much more intuitive and easy to use than the
HashTable and LinkedList implementations used in class.

*** DETAILS ***
Some time ago in class we started to get introduced to the Standard Template
Library in C++, but I feel that we did not go too in-depth on it and I wanted
to explore some of its features myself. For example, I wanted to see how we can
use maps, and potentially more complex uses of it similar to our HW1, such as using
a vector within a map, similarly to out linked list within a hash table. I wanted
to see how this would be implemented, and how much easier it would be compared
to the version we created.

To test this, I wrote a file ex14_5.cc which contains three functions:
- main: for creating the initial map of string to vectors of strings
- fill_map: for filling up the map with a given "amount" of key/value pairs
- print_contents: prints out the contents of the map

My plan was mainly to see how I could learn to do this by looking at the documentation
for maps and vectors, as well as seeing how much quicker and more clean this would be
over our HW1 implementation.

What I found was that this implementation was indeed much easier. After getting over
a few hiccups relating to C++ syntax, setting up these maps and vectors is very easy
and intuitive. Unlike the complex methods we used in HW1 where we had to manually allocate
the table and iterator, manage values through HTKeyValue_t objects, and just overall more complex
structure, maps and vectors are quite simple to use. They have many built-in functions like .insert()
for maps and .push_back for vectors. Them being templates makes adding any needed value very easy 
compared, leading me to appreciate these included templates and now I can use them for future C++ coding projects.