# NanoTekSpice

## Description

NanoTekSpice is a logic simulator that can parse and simulate a circuit described in a file.
A shell interface is available to interact with the simulator.

## Usage

    ./nanotekspice [file.nts] (flag)

### Flags

- `-h` or `--help`: Display the help message.
- `--run-empty` : launch the simulation with only one input and one output.
- `-ncurses` : launch the simulation with the ncurses interface. (WIP)

## File format

The file format is a simple text file with the following format:

    .chipsets:
    [type] [name]
    # Comments
    .links:
    [name]:[pin] [name]:[pin]

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

## Build

    make

## Author

- [**Charles MEDJERI**](https://www.linkedin.com/in/charles-madjeri/)
- [**Maël RABOT**](https://www.linkedin.com/in/mael-rabot/)
