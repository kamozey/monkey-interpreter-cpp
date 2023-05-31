# Monkey Interpreter in C++

**An Interpreter for the Monkey language written in C++**

This is my translation of the book written by Thorsten Ball, Writing an interpreter in Go, but in C++ instead of Go
lang.

I started writing this in Elixir first, but I got stuck while writing the parser. Translating imperative code into
functional programming code is harder than it looks.

So, I am writing this interpreter in C++ first, and will translate this into elixir once I fully understand the
mechanics behind parsing.


##### Update 31/05/2023

The interpreter is now complete. 

```c++
// basic functionality
let x = 5 + 5 - 4;
let y = 10 + x / 2;
let foobar = 838383 + y * 5;

// return statements
return 4 + (y+1);
return 9+x;

// if-else condition 
let z = if(1+1==2) { return 1; } else { return true; }

// functions
let f = fn(x,y,z){ return x+y+z; }
let value = add(z,f);

// higher order functions
let f = fn(x) { fn(y) { x+y }; }; // closures too!
let m = f(999);
 m(1)

// strings
let x = "hello";
return x + " " + x; 

// arrays
let array = ["hello", 1, fn(x){ return x;}]; 
array[2](7);  

// hash objects
let x = { "hello" : "world" }; 
x["hello"];
```

All the lines are valid!!

This was a rewarding journey. I've been wanting to write an interpreter for many many years.

Thanks to Thorsten Ball for his amazing book.

I'll share the learnings from this journey in my blog. Be sure to read it!

### Future Scope

In the book, Thorsten wrote the code in Golang. But my implementation is in C++. Apart from the obvious differences, C++ does not have a garbage collector, while Go does.

Therefore, an exciting new addition to this project would be adding a garbage collector. I've been reading about the conventional Mark-Sweep collector, and I'll try to add it in the future.

Another addition would be built-in functions. This is the part I skipped from the book.