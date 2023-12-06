## Inheritance, Overriding and Super
```python
class BaseClass:
    def method_name(self, arguments):
        # Base class implementation

class DerivedClass(BaseClass):
    def method_name(self, arguments):
        super().method_name(arguments)  # Call the base class method
        # Additional code specific to the derived class

```

## Super Class Calling
Calling parent extended class `super`:
```python
def __init__(self, param):
	super().__init__(param)
```
