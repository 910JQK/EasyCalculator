# Easy Calculator


**IMPORTANT: This is free software with ABSOLUTELY NO WARRANTY.**


**IMPORTANT: This implementation(code) is actually not very good.**


Easy Calculator is an extensible calculator. It parses expressions input and figure out their value. Some operators and functions are available. It has two modes: int and float. Mode int uses GMP arbitrary-precision integer; mode float uses double-precision floating-point number. You can define constants, variables and functions (see below).

It is still under development and may be unstable.


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
    >>> def lg(x) = log10(x)
    >>> lg(3) + lg(5)
    1.17609
    >>> def max2(a, b) = (a > b): a; b;
    >>> max2(12, 40)
    40
    >>> max2(12, -25)
    12

File import is available:

    $ cat test.ecs
    # Test
    const A = 2
    const B = 4
    def avg(a, b) = (a+b)/2
    
    $ ./EasyCalculator
    >>> import test
    >>> A
    2
    >>> B
    4
    >>> avg(A, B)
    3

Scientific notation.

    >>> 1e-4
    0.0001
    >>> 1e4
    10000
    >>> 125 000 000 * 2
    2.5e+08

Separate integer mode and float mode,  providing fast integer power calculating.
    
    $ time echo "int 2^10^6" | ./EasyCalculator -q > /dev/null
    
    real    0m0.518s
    user	0m0.413s
    sys	0m0.007s
    
    $ time echo "2^10^6" | bc > /dev/null
    
    real    0m22.264s
    user	0m20.487s
    sys	0m0.023s
    
Variables and constants.

    $ ./EasyCalculator
    >>> set x = 1
    >>> x
    1
    >>> set x = x + 1
    >>> x
    2
    >>> const y = 0
    >>> y
    0
    >>> unset x
    >>> unset y
    >>> x
    x: no such variable or constant
    >>> y
    y: no such variable or constant

Recording the last result.

    >>> mode int
    >>> def f(n) = n%2=1: 3*n+1; n/2;
    >>> f(13)
    40
    >>> f(ans)
    20
    >>> f(ans)
    10
    >>> f(ans)
    5
    >>> f(ans)
    16
    >>> f(ans)
    8
    >>> f(ans)
    4
    >>> f(ans)
    2
    >>> f(ans)
    1
    >>> f(ans)
    4

Finding approximate roots of equations using Newton's Method.

    $ ./EasyCalculator
    >>> root x: x^2 = 2 @ 1
    x = 1.41421
    >>> root x: x^2 = 1-x @ 1
    x = 0.618034
    >>> root x: cos(x/2) = 0 @ 1
    x = 3.14159
    >>> root x: exp(x) = exp(1)*x @ 2
    x = 1
    >>> root x: log(x) = x-1 @ 5
    x = 1
    >>> root x: x^x = 1 @ 10
    x = 1

Solving linear equation systems using Gauss Elimination.

    $ ./EasyCalculator
    >>> solve 2
    1 2 3
    2 1 4
    1.66667 0.666667
    >>> solve 2
    0 1 2
    2 3 1
    -2.5 2
    >>> solve 3
    2 5 9 1
    3 2 1 8
    7 7 6 4
    12.0556 -17.7222 7.27778

## Usage

### Mode

	mode float (default, IEEE 754 double)
	mode int (arbitrary-precision integer, GMP)
	mode ? (display current mode)
	int expr (evaluate expr in int mode)
	float expr (evaluate expr in float mode)

Each mode has separate scope. Variables and functions defined in one mode can't be accessed in another.

### Angle Units

    angle rad (default)
    angle deg
    angle grad
    angle ? (display current unit)

The input of triangle functions and the output of the inverse triangle functions are determined by the unit.

    2*PI rad = 360 deg = 400 grad
    
### File Import

Use `import FileName` to import a file. Appendix `.ecs` is automatically appended. If `FileName.ecs` does not exist, it will try `FileName`.

Logical lines are available. Use `;;` to separate them.

Comments are also available and start with `#` in a logical line.

When the program enters a imported file and executes its commands, it will turn into float mode. After that, it will restore to the original mode you set before. 

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
	cbrt(x)
	floor(x)
	ceil(x)
	sin(x)
	cos(x)
	tan(x)
	asin(x)
	acos(x)
	atan(x)
    atan2(y, x)
	exp(x)
	erf(x)
	log(x)
	log2(x)
	log10(x)
	logfac(x) (natural log of factorial of x, calculated using approximation of gamma function)
	fac(x) (exp(logfac(x)))
	rand() (generate random number in range [0, 1] )
    deg(d, m, s) (convert d°m′s″ to degree)
	det2(a, b, c, d) = a*d - b*c
    det3(a11, a12, a13, a21, a22, a23, a31, a32, a33)
    linspace(start, end, n, index) = start + (end-start)*index/(n-1)
	
#### Integer Mode
    
	abs(n)
	sgn(n)	(sign function)
	fac(n)	(factorial of n) O(n)
	P(n, r) (permutation) O(r)
	C(n, r) (combination) O(r)
	max(a, b)
	min(a, b)
	gcd(a, b)
	lcm(a, b)
	rand(n) (generate random number in range [0, n) )
	det2(a, b, c, d) = a*d - b*c
    det3(a11, a12, a13, a21, a22, a23, a31, a32, a33)

### Variables and constants
#### Builtin constant
    PI = 3.14159 (float mode)
#### Use constants and variables
    set name = expression
    const name = expression
    unset name

### Command "factor"

Show the standard prime decomposition of the given integer. (complexity: O(sqrt(n)) )

Example:

	>>> factor 10
	1*2*5
	>>> factor 100
	1*2^2*5^2

### Command "seq"

    seq n: [assignment1; assignment2...]: [expression1 | expression2 ...]

Generate a sequence of length n with variable "_" changing from 0 to n-1. Assignments and expressions are evaluated sequently.

### Examples for command "seq"

	Ex. 1
    >>> seq 10: n=_+1: _ | n | n^2 | n^3
    0 1 1 1 
    1 2 4 8 
    2 3 9 27 
    3 4 16 64 
    4 5 25 125 
    5 6 36 216 
    6 7 49 343 
    7 8 64 512 
    8 9 81 729 
    9 10 100 1000 


	Ex. 2
    >>> set s = 0
    >>> set p = 1
    >>> seq 10: n=_+1; s=s+n; p=p*n
    >>> s
    55
    >>> p
    3.6288e+06
    
    Ex. 3
    >>> set n = 10
    >>> angle deg
    >>> seq n: x = linspace(0, 90, n, _): x | sin(x) | cos(x) | tan(x)
    0 0 1 0 
    10 0.173648 0.984808 0.176327 
    20 0.34202 0.939693 0.36397 
    30 0.5 0.866025 0.57735 
    40 0.642788 0.766044 0.8391 
    50 0.766044 0.642788 1.19175 
    60 0.866025 0.5 1.73205 
    70 0.939693 0.34202 2.74748 
    80 0.984808 0.173648 5.67128 
    90 1 6.12323e-17 1.63312e+16 


### Command "def"

	def name([args, ...]) = expression
	def name([args, ...]) = condition1: expression1; [condition2: expression2; ...]
    
Condition is also an expression. If its value is true (non-zero), corresponding expression will become the value of the function. If not, the program will check next condition-expression pair. If no condition equals true, the program will tell you "Out of function definition".

### Examples for command "def"

	Ex. 1
	>>> def f(x) = 1
	>>> f()
	1

	Ex. 2
	>>> def f(x) = x + 1
	>>> f(1)
	2

	Ex. 3
	>>> def a(x, y) = (x + y) / 2
	>>> a(3, 5)
	4

	Ex. 4
	>>> def abs2(x) = (x < 0): -x; x;
	>>> abs2(-5)
	5
	>>> abs2(5)
	5
    
    Ex. 5
    >>> mode int
    >>> def fac2(n) = (x > 0): n * fac2(n - 1); (x = 0): 1;
	>>> fac(3)
	6
	>>> fac(-1)
	Out of function definition
    
    Ex. 6
    >>> def gcd2(a, b) = (b > 0): gcd2(b, a % b); (b = 0): a;
    >>> gcd2(36, 96)
    12
