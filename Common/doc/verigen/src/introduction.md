# Introduction

@<i>"Performance can't beat convenience."@</i>

When designing with the verilog of a large-scale project, \
one of the most problematic parts is that \
it takes a lot of time and effort to configure the control path between modules. \
In addition, if you need to modify some of the control paths of \
a design that has been completed with a lot of time and effort, \
or if you need a major change, you have to be more careful. \
Otherwise, it may introduce new errors or require the same amount of \
effort as recreating the design from scratch.

Therefore, I made a verigen tool that creates control paths with minimal design. \
This tool makes it easy and quick to build a control path programmatically with minimal effort, \
and has a function that allows you to check the structured control path as a design hierarchy at a glance. \
It can also allow for faster design changes and sharing of designs with other team members.


:::NoteHeading
If you have a new feature to suggest, or find improvements or bugs, please contact me (@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>).
 

## Main functions

verigen was created using codegen of TestDrive Profiling Master. \
This tool runs code written in lua, builds a verilog design, \
includes all codegen functionality, and generates the following files.

* Automatically generate verilog design (.sv, .f)
* Automatic creation of constraint (.xdc)
* Automatic creation of hierarchy diagram (.svg), HTML highlited source code (.html)
 

## Verilog automation creation process
Creating a project through verigen proceeds in the following steps.

* >1. Write Lua scripts
** > 1). Create modules
** > 2). Connect modules
** > 3). Declaring parameters and interface to the module (option)
* >2. Write verilog codes
** > 1). Declaring parameters and interface to the module (option)
** > 2). Write Verilog additional code or write Lua mixed code
* >3. Run verigen to generate verilog code
 
## How to run

To run verigen, run the following command.

```txt
> verigen

Verilog Generator for TestDrive Profiling Master. v1.06
Usage: verigen  [--help] input_file [output_path]

      --help                display this help and exit
  input_file                input Lua file
  output_path               output path
                            default : ./output
```

:::NoteHeading
Command : @<fixed>verigen @<color:FF0000>INPUT_LUA_FILE@</color>  @<color:FF0000>OUTPUT_PATH@</color>@</fixed>
 
A Lua script corresponding to @<color:FF0000>INPUT_LUA_FILE@</color> is created and executed. \
If @<color:FF0000>OUTPUT_PATH@</color> is not specified, the result is created in the default "./output" folder. 
 

## license grant
The source implemented in verigen complies with the BSD license, \
and the user's individual scripts used to create verilog or derivative works such as verilog are wholly owned by the user.
