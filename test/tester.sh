#!/bin/bash

script_dir=$(dirname "$0")
program="$script_dir/../nanotekspice"

############################# Declare the tests ###############################
# Command/response pairs for each file :

declare -A clock_nts=(
    ["display"1]=$'> tick: 0\ninput(s):\n  cl: U\noutput(s):\n  out: U\n> '
    ["cl=0"]=$'> > '
    ["display"2]=$'> tick: 0\ninput(s):\n  cl: U\noutput(s):\n  out: U\n> '
    ["simulate"1]=$'> > '
    ["display"3]=$'> tick: 1\ninput(s):\n  cl: 0\noutput(s):\n  out: 0\n> '
    ["simulate"2]=$'> > '
    ["display"4]=$'> tick: 2\ninput(s):\n  cl: 1\noutput(s):\n  out: 1\n> '
)
#declare -A true_nts=(
#    ["command3"]="expected_result3"
#    ["command4"]="expected_result4"
#)

# Files to test and their associated tests :

declare -A files=(
    ["nts_single/clock.nts"]=clock_nts
#    ["nts_single/true.nts"]=true_nts
)

# Order of the tests for each file :

order_clock_nts=("display1" "cl=0" "display2" "simulate1" "display3" "simulate2" "display4")
#order_true_nts=("command3" "command4")

############################ Execute the tests ################################

for file in "${!files[@]}"; do
    printf "\nTesting file %s:\n\n" "$file"
    declare -n tests=${files[$file]}

    if [ "$file" == "nts_single/clock.nts" ]; then
        keys=("${order_clock_nts[@]}")
    else
        keys=("${order_true_nts[@]}")
    fi

    for command in "${keys[@]}"; do
        expected_output=${tests[$command]}
        output=$(echo "$command" | "$program" "$file")

        if [ "$output" == "$expected_output" ]; then
            printf "\e[32mTest for command '%s' in file '%s' passed\n\e[0m" "$command" "$file"
        else
            printf "\e[31mTest for command '%s' in file '%s' failed\n\e[0m" "$command" "$file"
            printf "\e[31mExpected: '%s'\nGot: '%s'\n\e[0m" "$expected_output" "$output"
        fi
    done
done
