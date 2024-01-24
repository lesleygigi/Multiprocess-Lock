#!/bin/bash
# Run test script many times, util it fails.
# Normally 100 repetitions will suffice.

cd "$(dirname "$0")"

if [ $# -ne 1 ]; then
    echo "Usage: $0 numTrials"
    exit 1
fi

runs=$1

trap 'kill -INT -$pid 2>/dev/null; exit 1' INT

# run pre
timeout 1s ./test-lock.sh -q

for i in $(seq 1 $runs); do
    timeout 1s ./test-lock.sh -sq &  # suppress pre
    pid=$!
    if ! wait $pid; then
        echo '***' FAILED TESTS IN TRIAL $i
        exit 1
    fi
    echo -ne "$(($i*100/$runs))%\r"  # progress
done
echo '***' PASSED ALL $i TESTING TRIALS
