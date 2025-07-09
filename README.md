# C++ Delegates Library

type-safe implementation of delegates in C++ that provides a flexible callback mechanism similar to C# delegates or JavaScript event handlers.

## Overview

This library implements a delegate system that allows you to bind and invoke multiple callable objects (functions, lambdas, member functions) with a unified interface. It leverages modern C++ features including variadic templates, perfect forwarding, and `std::function` for optimal performance and type safety.

## Features

- **Type-safe**: Full compile-time type checking with variadic templates
- **Modern C++**: Uses C++17 features for optimal performance
- **Flexible binding**: Support for free functions, lambdas, and member functions
- **Multiple callbacks**: Execute multiple bound functions in sequence
- **RAII compliant**: Automatic memory management with no manual allocations
- **Exception safe**: Proper error handling and strong exception safety
- **Move semantics**: Efficient copying and moving of delegates
- **Intuitive API**: Clean, easy-to-use interface with operator overloading

## Requirements

- C++17 compatible compiler
- Standard library with `<functional>`, `<vector>`, `<algorithm>`

## Installation

Simply include the `delegate.h` header file in your project:

```cpp
#include "delegate.h"
using namespace delegates;
```

## Usage

### Basic Function Binding

```cpp
#include "delegate.h"
using namespace delegates;

int add(int a, int b) {
    return a + b;
}

// Create delegate and bind function
delegate<int(int, int)> math_delegate;
math_delegate.bind(add);

// Invoke the delegate
int result = math_delegate(5, 3); // Returns 8
```

### Member Function Binding

```cpp
class Calculator {
public:
    int multiply(int a, int b) {
        return a * b;
    }
    
    void print_result(int value) const {
        std::cout << "Result: " << value << std::endl;
    }
};

Calculator calc;
delegate<int(int, int)> mult_delegate;
delegate<void(int)> print_delegate;

// Bind member functions
mult_delegate.bind(&calc, &Calculator::multiply);
print_delegate.bind(&calc, &Calculator::print_result);

int result = mult_delegate(4, 6); // Returns 24
print_delegate(result);           // Prints "Result: 24"
```

### Lambda and Multiple Callbacks

```cpp
delegate<void(const std::string&)> message_delegate;

// Bind multiple callbacks
message_delegate += [](const std::string& msg) {
    std::cout << "Lambda: " << msg << std::endl;
};

message_delegate += [](const std::string& msg) {
    std::cout << "Another lambda: " << msg << std::endl;
};

// All bound callbacks will be executed
message_delegate("Hello World!");
```

### Advanced Usage

```cpp
// Check if delegate has callbacks
if (message_delegate) {
    message_delegate("This will execute");
}

// Get number of bound callbacks
std::cout << "Callbacks count: " << message_delegate.size() << std::endl;

// Clear all callbacks
message_delegate.clear();

// Check if empty
if (message_delegate.empty()) {
    std::cout << "No callbacks bound" << std::endl;
}
```

## API Reference

### Core Methods

| Method | Description |
|--------|-------------|
| `bind(func)` | Bind a callable object (function, lambda, etc.) |
| `bind(object, method)` | Bind a member function to an object |
| `operator()` | Invoke all bound callbacks |
| `operator+=` | Add a callback (alias for bind) |
| `operator-=` | Remove a callback (limited functionality) |
| `clear()` | Remove all callbacks |
| `size()` | Get number of bound callbacks |
| `empty()` | Check if no callbacks are bound |
| `operator bool()` | Check if callbacks are bound |

### Return Value Behavior

- **Void delegates**: All callbacks are executed in order
- **Non-void delegates**: All callbacks except the last are executed, the return value of the last callback is returned

## Examples

### Event System

```cpp
class Button {
private:
    delegate<void()> on_click_;
    
public:
    delegate<void()>& on_click() { return on_click_; }
    
    void click() {
        if (on_click_) {
            on_click_();
        }
    }
};

// Usage
Button button;
button.on_click() += []() { 
    std::cout << "Button clicked!" << std::endl; 
};
button.on_click() += []() { 
    std::cout << "Handling click event" << std::endl; 
};

button.click(); // Executes both callbacks
```

### Observer Pattern

```cpp
class Subject {
private:
    delegate<void(int)> observers_;
    int state_ = 0;
    
public:
    void subscribe(const std::function<void(int)>& observer) {
        observers_ += observer;
    }
    
    void set_state(int new_state) {
        state_ = new_state;
        observers_(state_); // Notify all observers
    }
};
```

## Performance Considerations

- Delegates use `std::function` internally, which may have slight overhead compared to raw function pointers
- Multiple callbacks are stored in `std::vector` for cache-friendly access
- Move semantics are used throughout for optimal performance
- No dynamic memory allocation during callback execution

## Thread Safety

This implementation is **not thread-safe**. If you need to use delegates across multiple threads, you must provide your own synchronization mechanisms.

## Error Handling

- Binding a null object pointer throws `std::invalid_argument`
- Invoking an empty non-void delegate throws `std::runtime_error`
- All operations provide strong exception safety guarantee

## Limitations

- The `operator-=` has limited functionality due to `std::function` comparison constraints
- Callback removal by value is not fully reliable - consider using the ID-based version for precise removal
- Not thread-safe out of the box

## Contributing

Contributions are welcome! Please ensure:

1. Code follows C++17 standards
2. All new features include appropriate tests
3. Documentation is updated for API changes
4. Code maintains the existing style and conventions

## License

This project is licensed under the GNU Lesser General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Authors

- Jose Luis P. Cardenas (jluis.pcardenas@gmail.com) - Original implementation
- Contributors welcome

---
