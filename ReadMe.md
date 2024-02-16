# NanoTekSpice

## Project overview

NanoTekSpice is a logic simulator that can parse and simulate a circuit described in a file.<br>
A shell interface is available to interact with the simulator (see [Shell commands](#shell-commands)).

## Usage

    ./nanotekspice [file.nts] (flag)

### Flags

- `-h` or `--help`: Display the help message.
- `--run-empty` : launch the simulation with only one input and one output.<br>
If this flag is used, the program will not load the input file, and the user will have to add the chipsets and links manually.
- `--table-dir [dir]` : generate (or override) truth tables from a user defined directory.<br>
The directory must containt .nts.init file, [format](#nts-file-format) explained below. Standard truth table are still generated with this flag, more information in the [Loading order](#loading-order) section.

## `.nts` File format

The file format is a simple text file with the following format:

    .chipsets:
    [type] [name]
    # Comments
    .links:
    [name]:[pin] [name]:[pin]

- `[type]` is the type of the chipset.
- `[name]` is the name of the chipset.
- `[pin]` is the id of the pin.

## `.nts.init` File format

The file format is a simple text file with the following format:

    #comment
    .LABEL:[NAME]
    .PINNB:[NUMBER]
    .INPUT:[PIN1,PIN2,...]
    .OUTPUT:[PIN1,PIN2,...]
    [VALUE][VALUE][...]
    [VALUE][VALUE][...]
    [...]

- `[NAME]` is the name of the chipset. The name must be in capital letters.
- `[NUMBER]` is the number of pins for the component.
- `[PIN1,PIN2,...]` is a comma-separated list of pin numbers for the input or output pins.
- `[VALUE]` is the value of the pin. The value can be `T` (true), `F` (false), `U` (undefined), or `X` (don't care).

The order of .LABEL, .PINNB, .INPUT and .OUTPUT cannot be changed.<br>
Giving less .PINNB than the actual number of pins **will result in undefined behavior**.

The values of the pin are to define in the correct order, with all the input pins first, then the output pins.<br>
The example below show the truth table of a AND Gate, with 2 inputs and 1 output.<br>
**Note:** *the simulation does not handle Vdd and Vss pins.*

The truth table must describe every possible combination of the pins, including undefined and "don't care" values.

ATM, only the file extension and the pin VALUE are checked, the file format **is not** checked.<br>
Missing or extra lines will result in undefined behavior.

It is possible to describe multiple truth tables in the same file.

User defined truth table are loaded last, which let the user override the standard truth table.<br>Note that the truth tables used internally cannot be overridden.
<details>
  <summary>e.g.</summary>

    # AND Truth Table
    .LABEL:AND
    .PINNB:3
    .INPUT:1,2
    .OUTPUT:3
    TTT
    TFF
    TUF
    FTF
    FFF
    FUU
    UTF
    UFU
    UUU

</details>

Some components need special behavior: clock, input, output, ram, etc.<br>
Thoses components **aren't** described in the .nts.init file, and are handled by the simulator.
If one of those components is found in the .nts file, the simulator will throw an error.

## `.nts.config` File format

The file format is a simple text file with the following format:

    [GATE_NAME]
    PinNb [NUMBER]
    componentsData
    [COMPONENT_NAME] [TYPE] [TRUTH_TABLE] [INPUTS] [OUTPUTS]
    [COMPONENT_NAME] [TYPE] [TRUTH_TABLE] [INPUTS] [OUTPUTS]
    [...]
    end
    pinRefTable
    [PIN] [COMPONENT_NAME] [COMPONENT_PIN]
    [PIN] [COMPONENT_NAME] [COMPONENT_PIN]
    [...]
    end

#### Gate description:

- [GATE_NAME] is the name of the gate.<br>
- [NUMBER] is the number of pins for the gate.<br>

#### Internal components description:

- [COMPONENT_NAME] is the name of an internal component.<br>
- [TYPE] is the type of the internal component. (Input, Standard, Output)<br>
*It is not recommended to use the type "Input" or "Output" for internal components.*
- [TRUTH_TABLE] is the truth table of the internal component.<br>
Thoses truth tables are the ones described in the .nts.init file.
- end is used to mark the end of the internal components' description.
- [INPUTS] is the list of input pins of the internal component.<br>
The format for the input is: {1,2,...}<br>
*Default value is: {1,2}*
- [OUTPUTS] is the list of output pins of the internal component.<br>
The format for the output is: {1,2,...}<br>
  *Default value is: {3}*

Inputs and outputs aren't mandatory, but the format must be respected.<br>

#### Pin reference table:

- [PIN] is the pin number of the gate.<br>
- [COMPONENT_NAME] is the name of the internal component.<br>
- [COMPONENT_PIN] is the pin number of the internal component.<br>
- end is used to mark the end of the pin reference table.

The .nts.config file is used to describe a custom gate, with internal components and pin reference table.<br>

It is not mandatory to connect all the pins of the internal components.

The format of the .nts.config file is not checked. Missing or extra lines **will result in undefined behavior**.

If a custom Truth Table is loaded with `--table-dir`, it is possible to use it in the .nts.config file.

**Note:** *The three files format purposely do not have the same format, to avoid confusion between the files.*

## Loading order

- The standard truth tables are loaded first.
- The user defined truth tables are loaded second.
- The .nts.config files are loaded last.

Overriding a standard truth table with a user defined truth table will result in the user defined truth table being used.<br>
Gates will also use the user defined truth table if one is found.

## Shell commands

- `exit`                                  : Exit the simulator.
- `help`                                  : Display the help message.
- `clear`                                 : Clear the terminal.
- `display`, `ls`                         : Display the current state of the circuit.
- `simulate`, `sm`                        : Simulate the circuit for one tick.
- `loop`                                  : Simulate the circuit until a SIGINT (ctrl+c).
- `dump`                                  : Display the current state of all the chipsets, pins and links.
- `add [type] [name]`                     : Add a new chipset to the circuit.
- `link [name]:[pin] [name]:[pin]`        : Link two pins together.
- `removeChipset [name]`                  : Remove a chipset from the circuit.
- `removeLink [name]:[pin] [name]:[pin]`  : Remove a link from the circuit.

Commands can be combined with the `&` character.
## Architecture

<details>
  <summary>explanations</summary>

### Component Management

The `Manager` class is responsible for managing components in the application. It uses a factory pattern to create components based on their type. The `_factory` method is used to create components of various types, including logic gates and special components like input, output, clock, true, and false. The created component is then added to the `_components` map using the `_addComponent` method.

### Link Management

The `Manager` class also manages the links between components. The `addLink` method is used to establish a link between two components. It takes the source component, source pin, destination component, and destination pin as parameters. The `removeLink` method is used to remove a link between two components.

### Simulation

The `Manager` class handles the simulation of the circuit. The `simulate` method is used to update the state of all components in the circuit based on the current tick.

### Parsing

The `Manager` class is responsible for parsing command line arguments and .nts files. It uses the `_parserLoop` method to read each line of the file and handle different stages of parsing, such as chipset and link stages.

### Truth Tables

The `Manager` class initializes truth tables from a specified directory using the `initializeTruthTables` method. It reads each file in the directory and generates a truth table from it using the `_generateTruthTableFromFile` method.

### Command Execution

The `Manager` class can execute various commands, such as `simulate`, `display`, `dump`, and `loop`. It also handles commands for adding and removing links and chipsets. The `_handleCommand` method is used to execute a command based on the input string.

### Error Handling

The `CustomError` class is used to handle custom exceptions in the application. It is thrown when an error occurs, such as when a file cannot be opened or an invalid character is found in a truth table.

</details>

## Build & Run

    make && ./nanotekspice [file.nts] (flag)

## Author

This project was carried out with the heart by:

- [**Charles MEDJERI**](https://www.linkedin.com/in/charles-madjeri/)
- [**Maël RABOT**](https://www.linkedin.com/in/mael-rabot/)

As second year IT students, we discovered the C++ language and the OOP paradigm with this project.<br>
We are proud of the result and we hope you will enjoy it as much as we do.
