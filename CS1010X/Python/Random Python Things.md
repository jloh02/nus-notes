To make new list as old list "in place"

```python
arr.clear()
arr.extend(new_arr)
```


Pass 0 or more arguments
Note: Need to handle if g takes no arguments
```python
def f(*args):
	g(*args)
```

## Memory Address
```python
id(variable)
```

## Build-In Python

```python
abs(x)
# x (numeric value): The numeric value for which the absolute value is computed.

len(s)
# s (sequence or collection): The sequence or collection for which the length is computed.

type(obj)
# obj (object): The object whose type is returned.

range(start, stop, step)
# start (optional, default: 0): The starting value of the sequence.
# stop (required): The ending value of the sequence (exclusive).
# step (optional, default: 1): The difference between each pair of consecutive values.

print(*objects, sep=' ', end='\n')
# *objects (variable number of objects): The objects to be printed.
# sep (optional, default: ' '): The separator between objects.
# end (optional, default: '\n'): The string appended after the last object.

int(x)
# x (object): The object to be converted to an integer.

float(x)
# x (object): The object to be converted to a floating-point number.

str(object)
# object (object): The object to be converted to a string.

bool(x)
# x (object): The object to be converted to a Boolean value.

sum(iterable, start=0)
# iterable (required): The iterable containing numbers to be summed.
# start (optional, default: 0): The value added to the sum of numbers.

max(iterable)
# iterable (required): The iterable containing comparable elements.

min(iterable)
# iterable (required): The iterable containing comparable elements.

sorted(iterable, key=None, reverse=False)
# iterable (required): The iterable to be sorted.
# key (optional, default: None): A function to extract a comparison key from each element.
# reverse (optional, default: False): If True, the iterable is sorted in descending order.

map(function, iterable, ...)
# function (required): A function to be applied to each element of the iterable(s).
# iterable (required): One or more iterables containing elements to be mapped.

filter(function, iterable)
# function (required): A function that returns True or False for each element of the iterable.
# iterable (required): The iterable containing elements to be filtered.

reversed(sequence)
# sequence (required): The sequence to be reversed.
```

## Strings
```python
str.upper()
# No parameters. Converts the string to uppercase.

str.lower()
# No parameters. Converts the string to lowercase.

str.strip(chars=None)
# chars (optional, default: None): The characters to be removed from the beginning and end of the string.

str.split(sep=None, maxsplit=-1)
# sep (optional, default: None): The delimiter for splitting the string into a list of substrings.
# maxsplit (optional, default: -1): The maximum number of splits to perform.

str.join(iterable)
# iterable (required): The iterable containing strings to be joined.

str.find(sub, start=0, end=-1)
# sub (required): The substring to be searched.
# start (optional, default: 0): The starting index for the search.
# end (optional, default: -1): The ending index for the search.

str.replace(old, new, count=-1)
# old (required): The substring to be replaced.
# new (required): The new substring to replace the old one.
# count (optional, default: -1): The maximum number of replacements to perform.

```