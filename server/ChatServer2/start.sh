#!/bin/bash

PROTOC_PATH="/usr/local/bin/protoc"
GRPC_PLUGIN_PATH="/usr/local/bin/grpc_cpp_plugin"
PROTO_FILE="proto/message.proto"

echo "Generating gRPC code..."
"$PROTOC_PATH" -I="." --grpc_out="." --plugin=protoc-gen-grpc="$GRPC_PLUGIN_PATH" "$PROTO_FILE"

echo "Generating C++ code..."
"$PROTOC_PATH" --cpp_out=. "$PROTO_FILE"


echo "Done."