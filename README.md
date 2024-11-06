# GRPC based in-memory shared queue
This approach is useful in scenarios where multiple instances or applications are deployed within a distributed system and need to communicate with each other. It serves as one of the inter-process communication (IPC) methods, where gRPC requests facilitate communication between the deployed applications

## Run grpc server.
```
bazel run //src:server
```
This would listen to port 50051 for incoming requests.

## Run grpc client
checkout the code in `examples` directory. A sample client code has been added there. To run the client code run
```
bazel run //examples:client
```
Make sure you run the grpc server beforehand (`bazel run //src:server`) in another tab.

## Internal
The grpc server accepts data in json form. If the input data is not correctly json formatted, the request will be rejected.
### Example
one application requires to share a data as follows:
```json
{
  "name": "John",
  "age": 30,
  "job" "none"
}
```
The application makes a `PostData` RPC request to the gRPC server, which stores the provided JSON value in an associated in-memory queue. Subsequent requests are appended to the queue, maintaining the order of incoming data.

On the other hand, other applications can query the gRPC server to check if any data has been pushed by other applications. The `GetData` RPC retrieves the top items from the internal queue. For reference, you can follow the structure in the `test` directory to understand the implementation. The better way to implement this could be polling the grpc server via `GetData` RPC.
