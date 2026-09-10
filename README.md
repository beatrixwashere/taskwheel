# taskwheel

very much a work in progress; selects a task from a list and starts a 10 minute timer for it.

*format:*

```
taskA 5
taskB 3
taskC 2
...
taskN X
```

each line has two sections separated by a space: the task name and its weight, which determines how likely it is to be chosen among all weights. for example, if `taskA` has a weight of 2 and `taskB` has a weight of 3, `taskA` has a 2/(2+3) = 40% chance of being chosen.