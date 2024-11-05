// greeter_client.cpp

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "src/sharedqueue.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpcSharedQueue::Request;
using grpcSharedQueue::RequestResponse;
using grpcSharedQueue::Response;

using grpcSharedQueue::GetDataparams;

class RequestResponseClient {
public:
  RequestResponseClient(std::shared_ptr<Channel> channel) : stub_(RequestResponse::NewStub(channel)) {}

  std::string PostData(const std::string& data) {
    Request request;
    request.set_data(data);

    Response reply;
    ClientContext context;

    Status status = stub_->PostData(&context, request, &reply);

    if (status.ok()) {
      return reply.data();
    } else {
      std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
      return "RPC failed";
    }
  }
  std::string GetData() {
    Request request;
    //request.set_data(data);

    grpcSharedQueue::GetDataparams empty;

    Response reply;
    ClientContext context;

    Status status = stub_->GetData(&context, empty, &reply);

    if (status.ok()) {
      return reply.data();
    } else {
      std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<RequestResponse::Stub> stub_;
};

int main(int argc, char** argv) {
  RequestResponseClient client_instance(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string data = "{\"name\":\"John\", \"age\":30,\"car\":null,\"job\":\"engineer\"}";
  std::string postdata = client_instance.PostData(data);

  data = "{\"name\":\"Joyanta\", \"age\":20,\"car\":null}";
  postdata = client_instance.PostData(data);
  std::string reply = client_instance.GetData();
  std::cout << "Response received: " << reply << std::endl;

  reply = client_instance.GetData();
  std::cout << "Response received: " << reply << std::endl;

   reply = client_instance.GetData();
  std::cout << "Response received: " << reply << std::endl;
  return 0;
}
