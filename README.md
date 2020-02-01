# Brainfuck Interpreter
This project is a commandline brainfuck interpreter I made to use some c++17 features, and to learn how to use vscode.

## Dependencies
This project uses conan to manage those, so it is assumed you have conan install. If not run `brew install conan` and it will install it.

### VSCode Extensions

I found that in order to debug on macOS I had to have the _CodeLLDB_ extension. I also have the _C/C++_ extensions to make life easier. For testing I used _Test Explorer UI_ and _Catch2, Google Test and doctest Explorer_. These were really cool and allowed me to auto run tests and zoom straight to failure which I found easier than when using vim and lldb directly.

## Getting
```bash
git clone https://github.com/geekskick/brainfuck_interpreter.git
```

## Building

### From the Command Line
```bash
cd brainfuck
make remake
```

### From within VSCode
The default build task will clean and build the workspace so run that. 

## Using
To show help message:
```bash
./interpreter --help
```
To just use as normal:
```bash
./interpreter
```
To show the process in slowtime where the memory of the brainfuck machine is shown after each command:
```bash
./interpreter --slow
```
To hello world use the input `++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.` 
## Tests

### From the Command Line
```bash
make test
```

### From within  VSCode
I used the _Test Explorer UI_ plugin, which is configured in my `settings.json` file to file and use those tests.

## Contributing
I accept issues/PRs from anyone. Please feel free to give me feedback :) 

If you're submitting a PR please format code first using the `.clang-format` provided. This can be performed using `make format`. 

Please also use some kind of static analysis. I quite like cppcheck and you can use it by performing `make lint`.