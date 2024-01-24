#! /usr/bin/env bash
# add feature: skip rc check
# add feature: assert tests-out/* is empty if tests/* is omitted
# add feature: -q

GREEN='\033[0;32m'
RED='\033[0;31m'
NONE='\033[0m'

# run_test testdir testnumber
run_test () {
    local testdir=$1
    local testnum=$2
    local verbose=$3
    local quiet=$4

    # pre: execute this after before the test is done, to set up
    local prefile=$testdir/$testnum.pre
    if [[ -f $prefile ]]; then
	eval $(cat $prefile)
	if (( $verbose == 1 )); then
	    echo -n "pre-test:  "
	    cat $prefile
	fi
    fi
    local testfile=$testdir/$testnum.run
    if (( $verbose == 1 )); then
	echo -n "test:      "
	cat $testfile
    fi
    eval $(cat $testfile) > tests-out/$testnum.out 2> tests-out/$testnum.err
    echo $? > tests-out/$testnum.rc

    # post: execute this after the test is done, to clean up
    local postfile=$testdir/$testnum.post
    if [[ -f $postfile ]]; then
	eval $(cat $postfile)
	if (( $verbose == 1 )); then
	    echo -n "post-test: "
	    cat $postfile
	fi
    fi
    return 
}

print_error_message () {
    local testnum=$1
    local contrunning=$2
    local filetype=$3

    builtin echo -e "test $testnum: ${RED}$testnum.$filetype incorrect${NONE}"

    if [[ -f $testdir/$testnum.$filetype ]]; then
        echo "  what results should be found in file: $testdir/$testnum.$filetype"
        echo "  what results produced by your program: tests-out/$testnum.$filetype"
        echo "  compare the two using diff, cmp, or related tools to debug, e.g.:"
        echo "  prompt> diff $testdir/$testnum.$filetype tests-out/$testnum.$filetype"
    else
        echo "  what results produced by your program: tests-out/$testnum.$filetype"
        echo "  tests-out/$testnum.$filetype is expected to be empty"
        echo "  prompt> cat tests-out/$testnum.$filetype"
    fi

    echo "  See tests/$testnum.run for what is being run"
    if (( $contrunning == 0 )); then
	exit 1
    fi
}

# check_test testdir testnumber contrunning out/err
check_test () {
    local testdir=$1
    local testnum=$2
    local contrunning=$3
    local filetype=$4

    # option to use cmp instead?
    if [[ -f $testdir/$testnum.$filetype ]]; then
        returnval=$(diff $testdir/$testnum.$filetype tests-out/$testnum.$filetype)
    else
        returnval=$([[ ! -s tests-out/$testnum.$filetype ]])
    fi
    if (( $? == 0 )); then
	echo 0
    else
	echo 1
    fi
}

# run_and_check testdir testnumber contrunning verbose printerror
#   testnumber: the test to run and check
#   printerrer: if 1, print an error if test does not exist
run_and_check () {
    local testdir=$1
    local testnum=$2
    local contrunning=$3
    local verbose=$4
    local failmode=$5
    local quiet=$6

    if [[ ! -f $testdir/$testnum.run ]]; then
	if (( $failmode == 1 )); then
	    echo "test $testnum does not exist" >&2; exit 1
	fi
	exit 0
    fi
    if (( $verbose == 1 )); then
	echo -n -e "running test $testnum: "
	cat $testdir/$testnum.desc
    fi
    run_test $testdir $testnum $verbose $quiet
    # rccheck=$(check_test $testdir $testnum $contrunning rc)
    rccheck=0  # skip rc check
    outcheck=$(check_test $testdir $testnum $contrunning out)
    errcheck=$(check_test $testdir $testnum $contrunning err)
    othercheck=0
    if [[ -f $testdir/$testnum.other ]]; then
	othercheck=$(check_test $testdir $testnum $contrunning other)
    fi
    # echo "results: outcheck:$outcheck errcheck:$errcheck"
    if (( $rccheck == 0 )) && (( $outcheck == 0 )) && (( $errcheck == 0 )) && (( $othercheck == 0 )); then
    if (( $quiet == 0 )); then
        echo -e "test $testnum: ${GREEN}passed${NONE}"
    fi
	if (( $verbose == 1 )); then
	    echo ""
	fi
    else
	if (( $rccheck == 1 )); then
	    print_error_message $testnum $contrunning rc
	fi
	if (( $outcheck == 1 )); then
	    print_error_message $testnum $contrunning out
	fi
	if (( $errcheck == 1 )); then
	    print_error_message $testnum $contrunning err
	fi
	if (( $othercheck == 1 )); then
	    print_error_message $testnum $contrunning other
	fi
    fi
}

# usage: call when args not parsed, or when help needed
usage () {
    echo "usage: run-tests.sh [-h] [-v] [-q] [-t test] [-c] [-s] [-d testdir]"
    echo "  -h                help message"
    echo "  -v                verbose"
    echo "  -q                quiet (only print errors)"
    echo "  -t n              run only test n"
    echo "  -c                continue even after failure"
    echo "  -s                skip pre-test initialization"
    echo "  -d testdir        run tests from testdir"
    return 0
}

#
# main program
#
verbose=0
quiet=0
testdir="tests"
contrunning=0
skippre=0
specific=""

args=`getopt hvqsct:d: $*`
if [[ $? != 0 ]]; then
    usage; exit 1
fi

set -- $args
for i; do
    case "$i" in
    -h)
	usage
	exit 0
        shift;;
    -v)
        verbose=1
        quiet=0
        shift;;
    -q)
        quiet=1
        verbose=0
        shift;;
    -c)
        contrunning=1
        shift;;
    -s)
        skippre=1
        shift;;
    -t)
        specific=$2
	shift
	number='^[0-9]+$'
	if ! [[ $specific =~ $number ]]; then
	    usage
	    echo "-t must be followed by a number" >&2; exit 1
	fi
        shift;;
    -d)
        testdir=$2
	shift
        shift;;
    --)
        shift; break;;
    esac
done

# need a test directory; must be named "tests-out"
if [[ ! -d tests-out ]]; then
    mkdir tests-out
fi

# do a one-time setup step
if (( $skippre == 0 )); then
    if [[ -f tests/pre ]]; then
    if (( $quiet == 0 )); then
        echo -e "doing one-time pre-test (use -s to suppress)"
    fi
	source tests/pre
	if (( $? != 0 )); then
	    echo "pre-test: failed"
	    exit 1
	fi
	echo ""
    fi
fi

# run just one test
if [[ $specific != "" ]]; then
    run_and_check $testdir $specific $contrunning $verbose 1 $quiet
    exit 0
fi

# run all tests
(( testnum = 1 ))
while true; do
    run_and_check $testdir $testnum $contrunning $verbose 0 $quiet
    (( testnum = $testnum + 1 ))
done

exit 0
