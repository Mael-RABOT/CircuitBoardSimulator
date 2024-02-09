# NanoTekSpice

## Project overview

NanoTekSpice is a logic simulator that can parse and simulate a circuit described in a file.
A shell interface is available to interact with the simulator.

## Usage

    ./nanotekspice [file.nts] (flag)

### Flags

- `-h` or `--help`: Display the help message.
- `--run-empty` : launch the simulation with only one input and one output.
- `--table-dir [dir]` : generate the truth tables of the circuit from a user defined directory. The directory must containt .nts.init file, format explained below. Standard truth table are still generated with this flag.

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
    .[NAME]
    [VALUE][VALUE][...]
    [VALUE][VALUE][...]
    [...]

- `[NAME]` is the name of the chipset. the name must be in capital letters.
- `[VALUE]` is the value of the pin. The value can be `T` (true), `F` (false), `U` (undefined).

The truth table must be in the same order as the pins in the chipset. (will be changed in the future to be more flexible)

The truth table must describe every possible combination of the pins, including undefined states.

ATM, only the file extension and the pin VALUE are checked, the file format is not checked. Missing or extra lines will result in undefined behavior.

It is possible to describe multiple truth tables in the same file.   

User defined truth table are loaded last, which let the user override the standard truth table.<br>Note that the truth tables used internally cannot be overridden.

<details>
  <summary>e.g.</summary>

    # AND Truth Table
    .AND
    TTT
    TFF
    TUF
    FTF
    FFF
    FUF
    UTF
    UUF
    UUU

    # OR Truth Table
    .OR
    TTT
    TFT
    TUT
    FTT
    FFF
    FUF
    UTT
    UUF
    UUU

</details>

## Shell commands

- `exit`: Exit the simulator.
- `clear`: Clear the terminal.
- `display`, `ls`: Display the current state of the circuit.
- `simulate`, `sm`: Simulate the circuit for one tick.
- `loop`: Simulate the circuit until a SIGINT (ctrl+c).
- `dump`: Display the current state of all the chipsets, pins and links.
- `add [type] [name]`: Add a new chipset to the circuit.
- `link [name]:[pin] [name]:[pin]`: Link two pins together.
- `removeChipset [name]`: Remove a chipset from the circuit.
- `removeLink [name]:[pin] [name]:[pin]`: Remove a link from the circuit.

Commands can be combined with the `&` character.<br>
Errors are displayed in red.

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

- [**Charles MEDJERI**](https://www.linkedin.com/in/charles-madjeri/)
- [**Maël RABOT**](https://www.linkedin.com/in/mael-rabot/)
