SOURCE_DIR=generated_requests
INTERFACE=localhost
PORT=8002
nc "$INTERFACE" "$PORT" < "$SOURCE_DIR/base_request.txt"
