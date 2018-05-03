#!/bin/bash
#fare chmod +x nel makefile
#cp input.sh run
#gcc runc.c -o runexe

format=0
control=0
log=0

while [[ $# -gt 0 ]]; do
	arg=$1
	case $arg in
		-H|--help)
			echo "Usage:"
            echo "./run [-L|--logfile] ..."
            echo "./run [-F|--format] ..." 
            echo "./run cmd "       #da fare le "" dentro il printf  
            echo "./run [-O|--outfile] ... not"
            echo "./run [-E|--errfile] ... not"
            echo "./run [-M|--maxlen] ... not"
            echo "./run [-C|--code] ... not"
			echo "./run [-H| --help] ... "
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
    echo "!error, cmd needed"                          
    exit -1
else 
    touch $log                      #crea il file di log
    ./runexe $format $log $cmd      #chiama il programma C con gli argomenti ordinati
fi
