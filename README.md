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

NOTE: The compiled code is limited to the size of 10 pages, and the head starts in the middle of a 30,000 long tape.

Have fun!