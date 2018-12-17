Commands:
module <name> <function>
connect <moduleName1> <moduleName2>
process <inputString>

Module Functions:
  echo: repeats the input, for example:
          in: "example"
          out: "example example"
  reverse: reverses the input, for example:
          in: "example"
          out: "elpmaxe"
  delay: outputs the last inputed string. Initialized with string "hello" stored. For example:
          in: "example1"
          out: "hello"
          in: "example2"
          out: "example1"
   noop: does nothing, outputs the input, for example:
          in: "example"
          out: "example"

Input strings are processed word by word. After all words of the string have been processed, empty strings ("") are processed until the network is empty.

Example inputs:
1.
  Input:
	module a reverse
	module b delay
	connect a b
	process hello world
  Expected Output: 
	hello oleo dlrow

2.
  Input: 
	module a reverse
	module b delay
	module c delay
	module d reverse
	connect a b
	connect a c
	connect c d
	connect d b
	process ab cd ef gh
	process ab cd
	process ab
  Expected Output: 
	hello ba hello dc ab fe cd hg ef gh
	ba dc ab cd
	ba ab

3.
  Input: 
	module a reverse
	module b echo
	module c noop
	connect a b
	connect b c
	process ab cd ef gh
  Expected Output: 
	ba ba dc dc fe fe hg hg

4.
  Input:
	module a noop
	module b noop
	module c noop
	module d delay
	connect a b
	connect b c
	connect c d
	process ab cd ef gh
  Expected Output: 
	hello ab cd ef gh

5.
  Input:
	module a noop
	module b reverse
	module c noop
	module d delay
	module e echo
	module f delay
	connect a b
	connect a c
	connect b d
	connect c d
	connect c e
	connect e f
	connect d f
	process ab cd ef
  Expected Output:
	hello hello ab ab ba cd cd ab ef ef dc cd fe ef

6.
  Input:
	module a reverse
	module b delay
	module c delay
	module d delay
	connect a b
	connect b c
	module e reverse
	connect a d
	connect c d
	connect a e
	connect e d
	process 1. 2. 3. 4.
	process 1. 2. 3. 4.
  Expected Output:
	hello .1 1. hello .2 2. hello .3 3. .1 .4 4. .2 .3 .4 
	.1 1. .1 .2 2. .2 .3 3. .3 .4 4. .4 
