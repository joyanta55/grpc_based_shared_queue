// greeter_server.cpp

#include "cjson/cJSON.h"
#include "grpcpp/grpcpp.h"
#include "iostream"
#include "src/shared/sharedQueue.h"
#include "src/sharedqueue.grpc.pb.h"
#include "string"
#include "threads.h"

#include <cstdlib>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpcSharedQueue::GetDataparams;
using grpcSharedQueue::Request;
using grpcSharedQueue::RequestResponse;
using grpcSharedQueue::Response;

SharedQueue shared_queue_instance;

class RequestResonseServiceImpl final : public RequestResponse::Service {
  Status PostData(ServerContext *context, const Request *request,
                  Response *response) override {

    const char *env_var = std::getenv("principle");

    std::cout << env_var << std::endl;

    sharedRT insert = shared_queue_instance.insertValue(request->data());

    if (!insert.success) {
      response->set_data(insert.reason);
      return Status::CANCELLED;
    } else {
      response->set_data("Data stored");
    }

    return Status::OK;
  }
  Status GetData(ServerContext *context, const GetDataparams *request,
                 Response *response) override {

    sharedRT value = shared_queue_instance.popValue();

    response->set_data(value.data);

    return Status::OK;
  }


  Status ResetData(ServerContext *context, const GetDataparams *request,
                 GetDataparams *response) override {

    shared_queue_instance.reset();
    return Status::OK;
  }
};

void RunServer() {

  shared_queue_instance.init();
  std::string server_address("0.0.0.0:50051");
  RequestResonseServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char **argv) {
  RunServer();
  return 0;
}
