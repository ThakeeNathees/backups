---
layout: post
title:  "Implementing Dynamic Type System [1/2]"
date:   2021-01-04 03:14:00 +05300
categories: jekyll update
permalink: "/compiler-design/dynamic-type-system/1"
---

Todo:
 - fix Point of View
 - introduction,
 - terminology variant, unions, enums
 - (using c/c++ for explaining) ...
 


In order to understand the implementation, I recommend you to have a basic knowledge of C/C++ and [pointers](https://www.cplusplus.com/doc/tutorial/pointers/).

## Unions
Structs (or classes) uses consecutive address space for each of its members and the size of the struct is the combined size of each of its members, and unlike structs `unions` uses the same memory space for each of its members and it has the size of it's the biggest member. At a given time at most one of the data members can be active. Which is an efficient way of using memory when you only use one member at any given time, but you have no way of knowing it from the union. You will have to store that information somewhere each time you set the member of the union.

![unions.png](/assets/unions.png)

## Tagged unions
The tagged unions are one of the popular methods to implement `variants`. It's a struct or a class that has a union value of different types and a `tag` field which indicates which is the current type the union is holding now.

```c++
class var {
public: // everything is public just for learning don't bother.

    // tags as enum
    enum Type {
        NIL,
        BOOL,
        NUMBER,
        STRING,
    };
    
    // type of the curent value (tag)
    Type type;
    
    // values, these members share their memory
    union {
        bool _bool;
        double _number;
        std::string _string;
    };
};
```

We'll add more types later and refactor the string type from `const char*` to `std::string` (which has a destructor that needs to be called manually since it's inside a union, we'll learn about that later in details). Now let's add some constructor to our `var` class.

```c++
class var {
    ... /* the tag and union of values here */
	
    var() {
        type = NIL;
    }

    var(bool v) {
        type = BOOL;
        _bool = v;
    }

    var(double v) {
        type = NUMBER;
        _number = v;
    }
	
    var(int v) {
        type = NUMBER;
        _number = (double)v;
    }

    var(const char* v) {
        type = STRING;
        _string = v;
    }
};
```

Now we can construct our variant with the defined types, lets implement the `to_string` method as a poor man's debugger to see what's going on. (the `const` keyword after between the method name and its body simply tells the compiler this method will **never** change it's members unless it's [`mutable`](https://en.cppreference.com/w/cpp/language/cv). see [const qualified member functions](https://en.cppreference.com/w/cpp/language/member_functions#const-_and_volatile-qualified_member_functions) for more details)

```c++
// string header needed to use std::string, std::to_string
#include <string>

class var {
    ... /* the tag and union of values here */
    ... /* constructors here */
	
    std::string to_string() const {
        switch (type) {
            case NIL: return "nil";
            case BOOL: return (_bool) ? "true" : "false";
            case NUMBER: return std::to_string(_number);
            case STRING: return _string;

            default: throw std::exception("Oops: we've missed a type here");
        }
    }
};
```

Finally, we've setup everything to get started on a working variant type. Now we can assign any defined type to our variant using a simple assignment operator like `var v = 3.14;`, you don't need to explicitly construct a variant from double like `var v = var(3.14);` to assign, C++ is smart enough to do that for us using [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion) if we don't have an [assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment).
Let's run some tests.

```c++
int main() {
#define print(v) std::cout << (v).to_string() << std::endl

    var v;     print(v);
    v = 3.14;  print(v);
    v = 42;    print(v);
    v = "the answer to life the universe and everything";
               print(v);
    v = true;  print(v);

    return 0;
}
```

Ta-da, we've built a tiny dynamic type system. The output for the above programe is...

```
nil
3.140000
42.000000
the answer to life the universe and everything
true
```

## Using derived types

All the types in the union currently are primitive, and pointers. Those values are not constructed and no need for destrction, when the life of `var` ends the union's memory just freed without needing to call any cleanup methods for that memory. If we're using derived to we can't just perform a [mem copy](https://en.cppreference.com/w/cpp/string/byte/memcpy) to create that member, that type need to be constructed and initialized with it's own constructor. And when the var dies we need to destruct the currently active member, 



Remember when we construct var using a double literal `var v = 3.14;`, what C++ does here is that it tries to call `var::operator=(double)` but we've not implemented that, so it'll try to do the next best thing that [copy assignment](https://en.cppreference.com/w/cpp/language/copy_constructor)[^1] using `var& var::var=(const var& v)` method since all classes has a default copy assignment, still our right hand operand `3.14` isn't a var instance. Don't worry C++ know how to make a var from double with our previously defined method `var::var(double)`. But if our class uses a union with derived type we also are responsible to initialize it's memory depending on the type tag, because of it the copy assignment isn't default anymore. Let's just define it.

[^1]: > A copy assignment operator of class T is a non-template non-static member function with the name operator= that takes exactly one parameter of type T, T&, const T&, volatile T&, or const volatile T&. For a type to be CopyAssignable, it must have a public copy assignment operator.

```c++
class var {
    ... /* the tag and union of values here */
    ... /* constructors here */
	
    var& operator=(const var& copy) {
        type = copy.type;
        switch (type) {
            case NIL: break;
            case BOOL: _bool = copy._bool; break;
            case NUMBER: _number = copy._number; break;
            case STRING: _string = copy._string; break;

            default: throw std::exception("Oops: we've missed a type here");
        }
        return *this;
    }
	
    ... /* methods here */
};
```

We're almost there to use `std::string` as our string value, first let's refactor our source.

```diff
      double _number;
--    const char* _string;
++    std::string _string;
  };
  
--  var(const char* v) {
++  var(const std::string& v) {
        type = STRING;
        _string = v;
    }
  
```

If you compile your source at this point you'll receive an error that `error C2280: 'var::~var(void)': attempting to reference a deleted function` (on MSVC) because we also have to define how to clean up the unions value depending on its tag.

```c++
// destructor
~var() {
    switch (type) {
        // these values doesn't require any cleanup
        case NIL:
        case BOOL:
        case NUMBER:
            break;

        case STRING:
            _string.~basic_string();
            break;

        default: throw std::exception("Oops: we've missed a type here");
    }
}
```

To call it a day we need to perform arithmetic operations between those variants, you could define your own behaviors as you decide to evaluate the operator. In python `"str" * 2` is a valid expression and evaluated to `"strstr"` but it's invalid in java, but you can add integers with strings in java which is invalid in python.

```c++
var operator+(const var& other) const {
    switch (type) {
        case NIL:  throw std::exception("nil type not supports operator '+'.");
        case BOOL: throw std::exception("bool type not supports operator '+'.");

        // Like Java we support `+` between integer and string
        case NUMBER: {
            if (other.type == NUMBER)
                return _number + other._number;
            else if (other.type == STRING)
                return to_string() + other._string;
            throw std::exception("unsupported operands for operator '+'.");
        }

        case STRING: {
            if (other.type == NUMBER)
                return _string + other.to_string();
            else if (other.type == STRING)
                return _string + other._string;
            throw std::exception("unsupported operands for operator '+'.");
        }

        default: throw std::exception("Oops: we've missed a type here");
    }
}
```

TODO: ENDING HERE

<br/>
