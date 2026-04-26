# Brainfuck JIT Compiler
Single-day project since I was stuck in Waterloo for a few days after finishing my last final for OS's.

To run, just use `make` and provide a file with brainfuck in it.

Example:
```
./bf_jit hello.bf
```
Contents of `hello.bf`:
```
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>
---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```
Expected Output:
```
Hello World!
```

Have fun!