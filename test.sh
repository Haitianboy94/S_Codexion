#!/bin/bash

# ============================================================
#  Codexion Test Suite
#  Usage: bash test_codexion.sh [path_to_binary]
#  Default binary: ./codexion
# ============================================================

BINARY="${1:-./codexion}"
PASS=0
FAIL=0
TOTAL=0

# Colors
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
CYAN="\033[36m"
RESET="\033[0m"

# ============================================================
# Helpers
# ============================================================

run_test()
{
	local desc="$1"
	local expected="$2"   # "ok", "burnout", "error", "no_burnout"
	local timeout_sec="$3"
	shift 3
	local args=("$@")

	TOTAL=$((TOTAL + 1))
	printf "${CYAN}[TEST %02d]${RESET} %-55s" "$TOTAL" "$desc"

	output=$(timeout "$timeout_sec" "$BINARY" "${args[@]}" 2>&1)
	exit_code=$?

	if [ $exit_code -eq 124 ]; then
		printf "${RED}FAIL${RESET} (timeout after ${timeout_sec}s)\n"
		FAIL=$((FAIL + 1))
		return
	fi

	case "$expected" in
		"ok")
			# Should exit 0, no burnout message
			if [ $exit_code -eq 0 ] && ! echo "$output" | grep -q "burned out"; then
				printf "${GREEN}PASS${RESET}\n"
				PASS=$((PASS + 1))
			else
				printf "${RED}FAIL${RESET} (exit=$exit_code, burnout=$(echo "$output" | grep -c "burned out"))\n"
				FAIL=$((FAIL + 1))
			fi
			;;
		"burnout")
			# Should exit 1, burnout message present
			if [ $exit_code -eq 1 ] && echo "$output" | grep -q "burned out"; then
				printf "${GREEN}PASS${RESET}\n"
				PASS=$((PASS + 1))
			else
				printf "${RED}FAIL${RESET} (exit=$exit_code, burnout=$(echo "$output" | grep -c "burned out"))\n"
				FAIL=$((FAIL + 1))
			fi
			;;
		"error")
			# Should exit non-zero with no simulation output
			if [ $exit_code -ne 0 ] && ! echo "$output" | grep -q "is compiling"; then
				printf "${GREEN}PASS${RESET}\n"
				PASS=$((PASS + 1))
			else
				printf "${RED}FAIL${RESET} (exit=$exit_code)\n"
				FAIL=$((FAIL + 1))
			fi
			;;
		"no_burnout")
			# Should complete without any burnout
			if ! echo "$output" | grep -q "burned out"; then
				printf "${GREEN}PASS${RESET}\n"
				PASS=$((PASS + 1))
			else
				printf "${RED}FAIL${RESET} (unexpected burnout)\n"
				FAIL=$((FAIL + 1))
			fi
			;;
	esac
}

check_burnout_timing()
{
	local desc="$1"
	local burnout_ms="$2"   # expected burnout time
	local tolerance="$3"    # tolerance in ms
	shift 3
	local args=("$@")

	TOTAL=$((TOTAL + 1))
	printf "${CYAN}[TEST %02d]${RESET} %-55s" "$TOTAL" "$desc"

	output=$(timeout 5 "$BINARY" "${args[@]}" 2>&1)

	burnout_line=$(echo "$output" | grep "burned out" | head -1)
	if [ -z "$burnout_line" ]; then
		printf "${RED}FAIL${RESET} (no burnout detected)\n"
		FAIL=$((FAIL + 1))
		return
	fi

	actual_ms=$(echo "$burnout_line" | awk '{print $1}')
	diff=$(( actual_ms - burnout_ms ))
	if [ $diff -lt 0 ]; then diff=$(( -diff )); fi

	if [ $diff -le $tolerance ]; then
		printf "${GREEN}PASS${RESET} (burnout at ${actual_ms}ms, expected ~${burnout_ms}ms)\n"
		PASS=$((PASS + 1))
	else
		printf "${RED}FAIL${RESET} (burnout at ${actual_ms}ms, expected ~${burnout_ms}ms, diff=${diff}ms)\n"
		FAIL=$((FAIL + 1))
	fi
}

check_compile_count()
{
	local desc="$1"
	local nb_coders="$2"
	local required="$3"
	shift 3
	local args=("$@")

	TOTAL=$((TOTAL + 1))
	printf "${CYAN}[TEST %02d]${RESET} %-55s" "$TOTAL" "$desc"

	output=$(timeout 30 "$BINARY" "${args[@]}" 2>&1)
	exit_code=$?

	if [ $exit_code -eq 124 ]; then
		printf "${RED}FAIL${RESET} (timeout)\n"
		FAIL=$((FAIL + 1))
		return
	fi

	# Count compiles per coder
	all_ok=1
	for i in $(seq 1 "$nb_coders"); do
		count=$(echo "$output" | grep "^[0-9]* $i is compiling" | wc -l)
		if [ "$count" -lt "$required" ]; then
			all_ok=0
			printf "${RED}FAIL${RESET} (coder $i compiled $count/$required times)\n"
			FAIL=$((FAIL + 1))
			return
		fi
	done

	if [ $all_ok -eq 1 ]; then
		printf "${GREEN}PASS${RESET}\n"
		PASS=$((PASS + 1))
	fi
}

check_log_format()
{
	local desc="$1"
	shift
	local args=("$@")

	TOTAL=$((TOTAL + 1))
	printf "${CYAN}[TEST %02d]${RESET} %-55s" "$TOTAL" "$desc"

	output=$(timeout 10 "$BINARY" "${args[@]}" 2>&1)

	# Every line must match: NUMBER NUMBER MESSAGE
	bad_lines=$(echo "$output" | grep -v "^[0-9]\+ [0-9]\+ " | grep -v "^$")
	if [ -z "$bad_lines" ]; then
		printf "${GREEN}PASS${RESET}\n"
		PASS=$((PASS + 1))
	else
		printf "${RED}FAIL${RESET} (malformed lines found)\n"
		echo "$bad_lines" | head -3
		FAIL=$((FAIL + 1))
	fi
}

check_valgrind()
{
	local desc="$1"
	shift
	local args=("$@")

	if ! command -v valgrind &> /dev/null; then
		TOTAL=$((TOTAL + 1))
		printf "${CYAN}[TEST %02d]${RESET} %-55s${YELLOW}SKIP${RESET} (valgrind not found)\n" "$TOTAL" "$desc"
		return
	fi

	TOTAL=$((TOTAL + 1))
	printf "${CYAN}[TEST %02d]${RESET} %-55s" "$TOTAL" "$desc"

	vg_output=$(timeout 15 valgrind --leak-check=full --error-exitcode=42 \
		"$BINARY" "${args[@]}" 2>&1)
	vg_exit=$?

	leaks=$(echo "$vg_output" | grep "definitely lost" | grep -v "0 bytes")
	errors=$(echo "$vg_output" | grep "ERROR SUMMARY" | grep -v "0 errors")

	if [ $vg_exit -eq 124 ]; then
		printf "${RED}FAIL${RESET} (timeout)\n"
		FAIL=$((FAIL + 1))
	elif [ -z "$leaks" ] && [ -z "$errors" ]; then
		printf "${GREEN}PASS${RESET}\n"
		PASS=$((PASS + 1))
	else
		printf "${RED}FAIL${RESET} (memory issues detected)\n"
		echo "$vg_output" | grep -E "definitely lost|ERROR SUMMARY" | head -3
		FAIL=$((FAIL + 1))
	fi
}

# ============================================================
# Check binary exists
# ============================================================

if [ ! -f "$BINARY" ]; then
	printf "${RED}ERROR: Binary '$BINARY' not found. Run 'make' first.${RESET}\n"
	exit 1
fi

printf "\n${CYAN}=====================================================${RESET}\n"
printf "${CYAN}  Codexion Test Suite — Binary: $BINARY${RESET}\n"
printf "${CYAN}=====================================================${RESET}\n\n"

# ============================================================
# SECTION 1: Invalid arguments
# ============================================================

printf "${YELLOW}--- Invalid Arguments ---${RESET}\n"

run_test "No arguments"                         "error" 2
run_test "Too few arguments (7 instead of 8)"   "error" 2  5 800 200 200 200 3 50
run_test "Too many arguments (9 instead of 8)"  "error" 2  5 800 200 200 200 3 50 fifo extra
run_test "Scheduler FIFO uppercase"             "error" 2  5 800 200 200 200 3 50 FIFO
run_test "Scheduler EDF uppercase"              "error" 2  5 800 200 200 200 3 50 EDF
run_test "Scheduler invalid string"             "error" 2  5 800 200 200 200 3 50 random
run_test "Zero coders"                          "error" 2  0 800 200 200 200 3 50 fifo
run_test "Negative coders"                      "error" 2  -1 800 200 200 200 3 50 fifo
run_test "Negative burnout time"                "error" 2  5 -800 200 200 200 3 50 fifo
run_test "Zero burnout time"                    "error" 2  5 0 200 200 200 3 50 fifo
run_test "Zero compile time"                    "error" 2  5 800 0 200 200 3 50 fifo
run_test "Zero debug time"                      "error" 2  5 800 200 0 200 3 50 fifo
run_test "Zero refactor time"                   "error" 2  5 800 200 200 0 3 50 fifo
run_test "Non-integer argument"                 "error" 2  5 abc 200 200 200 3 50 fifo
run_test "Float argument"                       "error" 2  5 800.5 200 200 200 3 50 fifo

# ============================================================
# SECTION 2: Basic functionality
# ============================================================

printf "\n${YELLOW}--- Basic Functionality ---${RESET}\n"

run_test "1 coder, 3 compiles, fifo"            "ok"     5   1 800 200 200 200 3 50 fifo
run_test "2 coders, 3 compiles, fifo"           "ok"     10  2 800 200 200 200 3 50 fifo
run_test "5 coders, 3 compiles, fifo"           "ok"     15  5 800 200 200 200 3 50 fifo
run_test "5 coders, 3 compiles, edf"            "ok"     15  5 800 200 200 200 3 50 edf
run_test "3 coders, 5 compiles, fifo"           "ok"     20  3 800 200 200 200 5 50 fifo
run_test "4 coders, 2 compiles, edf"            "ok"     10  4 800 200 200 200 2 50 edf

# ============================================================
# SECTION 3: Burnout scenarios
# ============================================================

printf "\n${YELLOW}--- Burnout Scenarios ---${RESET}\n"

run_test "Compile > burnout → burnout"          "burnout" 5  5 200 800 200 200 3 50 fifo
run_test "1 coder, compile > burnout"           "burnout" 5  1 200 800 200 200 3 50 fifo
run_test "2 coders, tight burnout"              "burnout" 5  2 100 800 200 200 3 50 fifo

# Burnout timing precision (within 10ms)
check_burnout_timing "Burnout logged within 10ms (200ms burnout)" \
	201 15  5 200 800 200 200 3 50 fifo

check_burnout_timing "Burnout logged within 10ms (500ms burnout)" \
	501 15  5 500 800 200 200 3 50 fifo

# ============================================================
# SECTION 4: Compile count verification
# ============================================================

printf "\n${YELLOW}--- Compile Count Verification ---${RESET}\n"

check_compile_count "1 coder reaches 3 compiles"   1 3   1 800 200 200 200 3 50 fifo
check_compile_count "2 coders reach 3 compiles"    2 3   2 800 200 200 200 3 50 fifo
check_compile_count "5 coders reach 3 compiles"    5 3   5 800 200 200 200 3 50 fifo
check_compile_count "5 coders reach 1 compile"     5 1   5 800 200 200 200 1 50 fifo
check_compile_count "3 coders reach 5 compiles"    3 5   3 800 200 200 200 5 50 fifo

# ============================================================
# SECTION 5: Log format verification
# ============================================================

printf "\n${YELLOW}--- Log Format ---${RESET}\n"

check_log_format "Log format valid (1 coder)"     1 800 200 200 200 3 50 fifo
check_log_format "Log format valid (5 coders)"    5 800 200 200 200 3 50 fifo
check_log_format "Log format valid (burnout)"     5 200 800 200 200 3 50 fifo

# ============================================================
# SECTION 6: No burnout under valid parameters
# ============================================================

printf "\n${YELLOW}--- No Burnout Under Valid Parameters ---${RESET}\n"

run_test "No burnout: generous burnout time"    "no_burnout" 15  5 2000 200 200 200 3 50 fifo
run_test "No burnout: edf scheduler"            "no_burnout" 15  5 2000 200 200 200 3 50 edf
run_test "No burnout: 0 cooldown"               "no_burnout" 15  5 800 200 200 200 3 0 fifo
run_test "No burnout: 2 coders"                 "no_burnout" 10  2 800 200 200 200 3 50 fifo

# ============================================================
# SECTION 7: Edge cases
# ============================================================

printf "\n${YELLOW}--- Edge Cases ---${RESET}\n"

run_test "1 compile required"                   "ok"     5   5 800 200 200 200 1 50 fifo
run_test "1 coder, edf"                         "ok"     5   1 800 200 200 200 3 50 edf
run_test "Large cooldown"                       "ok"     20  2 2000 200 200 200 3 500 fifo
run_test "Many coders (10)"                     "ok"     30  10 800 200 200 200 3 50 fifo
run_test "Very short compile time"              "ok"     10  5 800 10 10 10 5 5 fifo
run_test "Zero cooldown fifo"                   "ok"     15  5 800 200 200 200 3 0 fifo
run_test "Zero cooldown edf"                    "ok"     15  5 800 200 200 200 3 0 edf

# ============================================================
# SECTION 8: Memory leaks (valgrind)
# ============================================================

printf "\n${YELLOW}--- Memory Leaks (valgrind) ---${RESET}\n"

check_valgrind "No leaks: 1 coder, clean stop"      1 800 200 200 200 1 50 fifo
check_valgrind "No leaks: 2 coders, clean stop"     2 800 200 200 200 3 50 fifo
check_valgrind "No leaks: 5 coders, clean stop"     5 800 200 200 200 2 50 fifo
check_valgrind "No leaks: burnout scenario"         5 200 800 200 200 3 50 fifo
check_valgrind "No leaks: edf scheduler"            3 800 200 200 200 2 50 edf

# ============================================================
# Summary
# ============================================================

printf "\n${CYAN}=====================================================${RESET}\n"
printf "${CYAN}  Results: ${GREEN}$PASS passed${RESET}, ${RED}$FAIL failed${RESET}, $TOTAL total\n"
printf "${CYAN}=====================================================${RESET}\n\n"

if [ $FAIL -eq 0 ]; then
	printf "${GREEN}All tests passed!${RESET}\n\n"
	exit 0
else
	printf "${RED}$FAIL test(s) failed.${RESET}\n\n"
	exit 1
fi