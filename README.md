# myLisp


I love programming languages. They are so much fun to work and play with.
myLisp is my first attempt at making my own programming language.

## The future of myLisp

Currently I'm working on turning myLisp into a usable language.
It's a toy language right now, but once I think It's robust enough I'll graduate it from myLisp into having a proper name.


## Later on down the road

I would like to turn myLisp into a better more useful language.
These are things I would love in a language.
Once I have the current interpretor up to snuff, I'd like to look into doing some other fun stuff.

Some ideas I have for it are
* Compilation to an executable
* having a C ffi
* creation of header and object files for linking with C object files
* concurrent programming
* explicitly denoting mutability
* manual memory management
* meta programming
* dictionaries as struts



## Language features that myLisp has
### Variable assignment

```
(define a 1)
```

### Built in functions

```
(+ 1 1)
```

### User functions

```
(lambda (b) (+ b b) )
```

### Function assignment

```
(define c (lambda (b) (+ b b) ))
```

### Conditionals

```
(if (+ -1 1) 'a 'b)
```

