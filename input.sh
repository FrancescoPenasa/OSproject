#!/bin/bash

format="STD"
log="Log.txt"
outfile=0
errfile=0
control=0
reset=0

while [[ $# -gt 0 ]]; do
	arg=$1
	case $arg in
		-H|--help)
			echo " "
			echo " "
			echo "Run usage:"
			echo " "
			echo "  ./run [-H| --help] to see this interface."
			echo " "
            echo "  ./run [-L|--logfile] to specify the logfile of the analyser."
			echo " "
            echo "  ./run [-F|--format] to specify the format of the logfile."
            echo "    format type supported: STD to print basic info, COMPLETE for more info."
			echo " " 
            echo "  ./run [-O|--outfile] to specify the output file of the command."
			echo " "
            echo "  ./run [-E|--errfile] to specify the errorr file of the command."
			echo " "		
			echo "  ./run \"command\" to execute the bash command"
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
		-O|--outfile)
			shift 
			outfile=$1
		;;
		-E|--errfile)
			shift
			errfile=$1
		;;
		--reset)
			shift
			reset=1
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
    ./crun $format $log $cmd $errfile $outfile $reset    #chiama il programma C con gli argomenti ordinati
fi

