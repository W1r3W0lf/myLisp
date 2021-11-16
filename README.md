# myLisp

This project was a challenge to myself. I wanted to see if I could write my own programming language without taking a class or following a tutorial.

The current version of myLisp is missing features that most lisps provide.

I may come back to this project later to clean up the code, add more comments, and work on it's feature set.

I would like to one day turn it into a R7RS-small Scheme. I would also want to change it's name if I decide to do that.

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

