#!/bin/bash

format="STD"
log="Log.txt"
control=0

while [[ $# -gt 0 ]]; do
	arg=$1
	case $arg in
		-H|--help)
			echo " "
			echo "Usage: [OPTION]... [FILE]...  \"COMMAND\" "
			echo "The options below may be used to select to change the output and the behaviour of the program"
			echo "  ./run [-H| --help] to see this interface."
			echo " "
            echo "  ./run [-L|--logfile] to specify the logfile of the analyser."
			echo " "
            echo "  ./run [-F|--format] to specify the format of the logfile."
            echo "    format type supported: STD to print basic info, COMPLETE for more info."
			echo " "		
			echo "  ./run \"command\" to execute the bash command"
			echo "   it's possible to use a pipe with this syntax \"cmd1|cmd2\""
			echo " "
			echo " "
			echo "  Example: ./run -L Log.txt -F STD \"cmd1|cmd2\""
			echo " "
			echo " "
			exit 1
		;;
		-L|--logfile)
			shift
            log=$1
		;;
		-F|--format)
			shift
			format=$1
		;;
        *)
			cmd=$1
            control=1
		;;
	esac
	shift							#shifta gli argomenti
done

if [ $control -eq 0 ] ; then        #controlla che sia stato specificato un comando
    echo "Error! Command needed to execute run!"                          
    exit -1
else 
    ./crun $format $log $cmd 0 0     #chiama il programma C con gli argomenti ordinati
fi

