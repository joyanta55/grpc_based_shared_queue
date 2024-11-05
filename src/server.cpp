// greeter_server.cpp

#include "iostream"
#include "string"
#include "grpcpp/grpcpp.h"
#include "src/sharedqueue.grpc.pb.h"
#include "threads.h"
#include "cjson/cJSON.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpcSharedQueue::Request;
using grpcSharedQueue::RequestResponse;
using grpcSharedQueue::Response;
using grpcSharedQueue::GetDataparams;

std::list<cJSON*> list_grpc;

class RequestResonseServiceImpl final : public RequestResponse::Service {
  Status PostData(ServerContext* context, const Request* request, Response* response) override {
    cJSON* request_val = cJSON_Parse(request->data().c_str());

    if (request_val==NULL) {
      response->set_data("data is not properly json formatted");
      return Status::CANCELLED;
    }
    else {
      list_grpc.push_back(request_val);
    }

    response->set_data("Data stored");

    return Status::OK;
  }
  Status GetData(ServerContext* context, const GetDataparams* request, Response* response) override {

    if (list_grpc.empty()) {
       response->set_data("No data available");
       return Status::OK;
    }
    cJSON* list_front_item = list_grpc.front();
    list_grpc.pop_front();
    char* data = cJSON_Print(list_front_item);
    response->set_data(std::string(data));

    free(data);
    cJSON_free(list_front_item);
    return Status::OK;
  }
};

void RunServer() {
  
  list_grpc = {};

  std::string server_address("0.0.0.0:50051");
  RequestResonseServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
