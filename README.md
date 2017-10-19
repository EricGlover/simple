# simple
A basic compiler for a simulated machine using a language called Simple

### Description
This was part of an exercise in the textbook for an intro class.
To anyone reading this it's unlikely that you'll find anything of interest in here. 
Also note that it's currently unpolished, I'm still working on expanding it's features.

### Run it yourself!
1.  Clone this repo.
2.  In your terminal / command prompt cd into the directory
3.  Run "./a.out addProg.simple.txt output.txt"

In this example addProg.simple.txt is a program to add to ints together, so in your terminal / command prompt you see tons of printf statements being output (sorry about that), then it'll ask you for a number - enter something, and do the same for the next prompt. Then it prints the execution steps

loading
adding
storing
loading
storing

Then it'll spit out the answer

3

After that it prints out the registers and the memory.

The syntax for the run command is thus:

$ <your executable> <the program written in simple you wish to run> <a file that simpletron will write the 'machine code' into> <an option flag>

##### Simple Programs included in this repo that you can run 
ex1.simple.txt - ex5.simple.txt
ex7.simple.txt
addProg.simple.txt
modtest.simple.txt
modulus.simple.txt

##### Write your own programs in Simple!
The language syntax rules are documented in syntax.txt

### Material
For anyone learning computer science much of the material for the compiler exercise is freely available on the author's website if you want to build a 'compiler' yourself.

http://www.deitel.com/bookresources/chtp8/CompilerExercises.pdf
