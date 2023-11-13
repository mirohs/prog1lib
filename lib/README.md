The <i>Programming I C Library</i> defines basic data types and functions to simplify C programming for beginners and to enable the step-by-step design approach outlined in the <a href ="/files/prog1script/script.html">script</a>. It contains facilities for testing code, e.g., functions for comparing actual and expected return values.

Download and Installation
-------------------------

Download the library with examples from the script:<br/>
<a href="/files/prog1lib-1.5.3.zip">prog1lib-1.5.3.zip</a>

Extract the zip file, go to directory prog1lib/lib, make libprog1.a, go to directory prog1lib/script_examples, make celsius_to_fahrenheit (or any other example from the script). In more detail, assuming that you have opened the terminal (on Linux or Mac) or installed MinGW (on Windows, including the @c bin class of msys-wget, msys-zip, msys-unzip), perform these steps:

<code>
cd DirectoryToWhichYouDownloadedTheLibrary<br/>
unzip prog1lib-1.5.3.zip<br/>
cd prog1lib/lib<br/>
make<br/>
cd ../script_examples<br/>
make celsius_to_fahrenheit<br/>
./celsius_to_fahrenheit<br/>
</code>

Detailed Steps for MinGW on Windows
-----------------------------------

- Download the library and save it in your MinGW home directory:<br/>
<code>
C:\\MinGW\\msys\\1.0\\home\\MyUserName
</code>

- Start the MinGW shell by double-clicking<br/>
<code>
C:\\MinGW\\msys\\1.0\\msys.bat
</code>

- The MinGW shell starts in your MinGW home directory. List its contents:<br/>
<code>
ls -l
</code><br/>
The library zip-file should be in the list.

- Unpack the zip-file to generate the <code>prog1lib</code> directory:<br/>
<code>
unzip prog1lib-1.5.3.zip
</code><br/>
If <code>unzip</code> is not available, install it via the MinGW setup. (See the exercise slides for more details.)

- Change to the <code>lib</code> directory:<br/>
<code>
cd prog1lib/lib
</code>

- Compile the library:<br/>
<code>
make
</code>

- Change to the script examples directory:<br/>
<code>
cd ../script_examples
</code>

- Compile the example:<br/>
<code>
make celsius_to_fahrenheit
</code>

- Execute the example:<br/>
<code>
./celsius_to_fahrenheit<br/>
</code>
or<br/>
<code>
celsius_to_fahrenheit.exe
</code>
    
- To compile other examples, only perform the last two steps again. They may be combined:<br/>
<code>
make wages && ./wages
</code>



Header Files
------------

Functions for output, input, testing, assertions, etc. contained in @ref base.h.

<a href="/files/prog1lib/files.html">Overview of all files</a>

- base.h
- basedefs.h
