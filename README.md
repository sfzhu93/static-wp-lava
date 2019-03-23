# Weakest Preconditions on LAVA dataset

Make with cmake. Run `run.sh` to run the main pass. The llvm pass to run is implemented in `MainPass.cpp`.

benchmarks
==========

## sort3
Sort three numbers less than 100 and return them in one integer. E.g.  for input 5 4 3, it returns 30405.
The output of WP is complex. Solve it with an online z3 demo using the following template:
```
(declare-const a Int)
(declare-const b Int)
(declare-const c Int)

(assert (the WP expr))
(apply ctx-solver-simplify)
```
, we get
```
(goals
(goal
  (let ((a!1 (not (<= 1234567 (+ a (* 10000 c) (* 100 b)))))
        (a!2 (not (<= 1234567 (+ a (* 10000 b) (* 100 c)))))
        (a!3 (not (<= 1234567 (+ c (* b 10000) (* 100 a)))))
        (a!6 (not (<= 1234567 (+ b (* c 10000) (* a 100)))))
        (a!7 (and (not (> b a)) (< (+ a (* c 10000) (* b 100)) 1234567)))
        (a!9 (not (<= 1234567 (+ b (* 10000 a) (* c 100)))))
        (a!10 (not (<= 1234567 (+ c (* a 10000) (* b 100))))))
  (let ((a!4 (or (and (> a c) a!2) (and (not (> a c)) a!3)))
        (a!8 (and (> a c) (or (and (> b a) a!6) a!7)))
        (a!11 (or (and (> b c) a!9) (and (not (> b c)) a!10))))
  (let ((a!5 (or (and (not (<= b c)) (> b c) (> a b) a!1)
                 (and (not (> b c)) a!4)))
        (a!12 (or a!8 (and (not (> a c)) a!11))))
    (or (and (not (<= a b)) (> a b) a!5) (and (not (> a b)) a!12)))))
  :precision precise :depth 1)
)
```

There should be 6 permutations of a, b and c. For each permutation, we could find its corresponding conditions.
