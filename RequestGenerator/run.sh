#!/bin/bash
#
BIN=./RequestGenerator 
#
OUTPUT_DIR=../generated_requests
#
SOURCE_DIR=basic
#
mkdir -p "$OUTPUT_DIR"
#
"$BIN" "$SOURCE_DIR/header.txt" > "$OUTPUT_DIR/base_request.txt"
