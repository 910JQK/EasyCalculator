# Easy Calculator


**IMPORTANT: This is free software with ABSOLUTELY NO WARRANTY.**


**IMPORTANT: This implementation(code) is actually not very good. For that reason, installing the executable file to /opt is suggested.**


Easy Calculator is an extensible calculator. It parses expressions input and figure out their value. Some operators and functions are available. It has two modes: int and float. Mode int uses GMP arbitrary-precision integer; mode float uses double-precision floating-point number. You can define constants, variables and functions (see below).

It is still testing and may be unstable.


### LICENSE
MIT License


### Testing

Dependencies:
    `gmp`
	`readline`

Building dependencies:
	`g++`
    `make`
	`cmake`
    (C++11 support is required)

Build:

	cmake .
	make

Run:

    ./EasyCalculator
    
### Overview
It is extensible, providing a more convenient way to calculate numbers:

    $ ./EasyCalculator
    >>> defun lg(x) = log10(x)
    >>> lg(3) + lg(5)
    1.17609
    >>> defun max2(a, b) = (a > b): a; b;
    >>> max2(12, 40)
    40
    >>> max2(12, -25)
    12

    
Separate integer mode and float mode,  providing fast integer power calculating.
    
    $ time echo -e "mode int \n 2^10^6" | ./EasyCalculator -q > /dev/null
    
    real    0m0.518s
    user	0m0.413s
    sys	0m0.007s
    
    $ time echo "2^10^6" | bc > /dev/null
    
    real    0m22.264s
    user	0m20.487s
    sys	0m0.023s
    
Variables and constants support.

    $ ./EasyCalculator
    >>> set x = 1
    1
    >>> x
    1
    >>> set x = x + 1
    2
    >>> x
    2
    >>> const y = 0
    0
    >>> y
    0
    >>> unset x
    >>> unset y
    >>> x
    x: no such variable or constant
    >>> y
    y: no such variable or constant

    
## Usage

### Mode

	mode float (default, IEEE 754 double)
	mode int (arbitrary-precision integer, GMP)
	mode ? (display current mode)

### Commands
Press tab key twice to show the list of commands. Type the following command to show details about commands.

    help COMMAND


### Operators
	
	+ (unary plus)
	- (unary minus)
	^ (power)
	+ (plus)
	- (minus)
	* (multiply)
	/ (divide)
	% (mod)
	! (not)
	= (equal)
	!= (not equal)
	> (greater than)
	< (less than)
	>= (greater than or equal to)
	<= (less than or equal to)
#### Logical
Zero is false, non-zero value is true. Logical operators always return 1 as true.


### Builtin Functions

#### Float mode
	
	abs(x)
	sgn(x) (sign function)
	sqrt(x)
	floor(x)
	ceil(x)
	sin(x)
	cos(x)
	tan(x)
	asin(x)
	acos(x)
	atan(x)
	exp(x)
	log(x)
	log2(x)
	log10(x)
	rand() (generate random number in range [0, 1] )
	
#### Integer Mode
    
	abs(x)
	sgn(x)	(sign function)
	fac(x)	(factorial of x)
	P(n, r) (permutation)
	C(n, r) (combination)
	max(a, b)
	min(a, b)
	gcd(a, b)
	lcm(a, b)
	rand(n) (generate random number in range [0, n) )

### Variables and constants
#### Builtin constant
    PI = 3.14159 (float mode)
#### Use constants and variables
    set name = expression
    const name = expression
    unset name

### Command "seq"

	seq n: expression
    
The expression will be parsed n times with variable "_" changing from 0 to n-1, generating the sequence. The sum as well as the product will be figured out.

### Examples for command "seq"

	Ex. 1
	>>> seq 3:_*2
	0: 0
	1: 2
	2: 4
	sum = 6
	product = 0

	Ex. 2
	>>> seq 10:2^_
	0: 1
	1: 2
	2: 4
	3: 8
	4: 16
	5: 32
	6: 64
	7: 128
	8: 256
	9: 512
	sum = 1023
	product = 3.51844e+13

### Command "defun"

	defun name([args, ...]) = expression
	defun name([args, ...]) = condition1: expression1; [condition2: expression2; ...]
    
Condition is also an expression. If its value is true (non-zero), corresponding expression will become the value of the function. If not, the program will check next condition-expression pair. If no condition equals true, the program will tell you "Out of function definition".

### Examples for command "defun"

	Ex. 1
	>>> defun f(x) = 1
	>>> f()
	1

	Ex. 2
	>>> defun f(x) = x + 1
	>>> f(1)
	2

	Ex. 3
	>>> defun a(x, y) = (x + y) / 2
	>>> a(3, 5)
	4

	Ex. 4
	>>> defun abs2(x) = (x < 0): -x; x;
	>>> abs2(-5)
	5
	>>> abs2(5)
	5
    
    Ex. 5
    >>> mode int
    >>> defun fac2(n) = (x > 0): n * fac2(n - 1); (x = 0): 1;
	>>> fac(3)
	6
	>>> fac(-1)
	Out of function definition
    
    Ex. 6
    >>> defun gcd2(a, b) = (b > 0): gcd2(b, a % b); (b = 0): a;
    >>> gcd2(36, 96)
    12

### Configuration file:

    calc_config.txt
    
Fixed, evaluated line by line when the program starts. You can define constants and functions in it to extend the program.
