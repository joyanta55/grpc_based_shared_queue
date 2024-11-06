
#include "src/sharedqueue.grpc.pb.h"
#include "cjson/cJSON.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpcSharedQueue::Request;
using grpcSharedQueue::RequestResponse;
using grpcSharedQueue::Response;

using grpcSharedQueue::GetDataparams;

#include <gmock/gmock.h>

using testing::Eq;

class RequestResponseClient {
public:
  RequestResponseClient(std::shared_ptr<Channel> channel)
      : stub_(RequestResponse::NewStub(channel)) {}

  std::string PostData(const std::string &data) {
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
    // request.set_data(data);

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

  bool Reset() {
    Request request;
    // request.set_data(data);

    grpcSharedQueue::GetDataparams empty_req;
    grpcSharedQueue::GetDataparams empty_res;

    //Response reply;
    ClientContext context;

    Status status = stub_->ResetData(&context, empty_req, &empty_res);

    if (status.ok()) {
      return true;
    } else {
     return false;
    }

  }

private:
  std::unique_ptr<RequestResponse::Stub> stub_;
};

TEST(SharedQueueTest, SimpleTestPostData) {
  RequestResponseClient client_instance(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  bool reset = client_instance.Reset();

  EXPECT_TRUE(reset);
  std::string data =
      "{\"name\":\"John\", \"age\":30,\"car\":null,\"job\":\"engineer\"}";
  std::string postdata = client_instance.PostData(data);
  EXPECT_EQ(postdata, "Data stored");
}

TEST(SharedQueueTest, SimpleTestGetData) {
   RequestResponseClient client_instance(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  bool reset = client_instance.Reset();

  EXPECT_TRUE(reset);
  std::string data =
      "{\"name\":\"Test2\", \"age\":30,\"car\":null,\"job\":\"none\"}";
  std::string postdata = client_instance.PostData(data);
  EXPECT_EQ(postdata, "Data stored");
  std::string getData = client_instance.GetData();


  cJSON *request_val = cJSON_Parse(getData.c_str());

  EXPECT_NE(request_val, nullptr);

  if (request_val!=nullptr) {
    cJSON *name = cJSON_GetObjectItemCaseSensitive(request_val, "name");

    char *string = cJSON_Print(name);

    EXPECT_EQ(std::string(string),"\"Test2\"");// "Test2"
  }
  cJSON_Delete(request_val);
  
}

TEST(SharedQueueTest, SimpleTestPushPop) {
  RequestResponseClient client_instance_post(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  RequestResponseClient client_instance_get_1(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

   RequestResponseClient client_instance_get_2(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  bool reset = client_instance_post.Reset(); //Doesn't matter, since any install call Reset method,the internal data structure would be reset

  EXPECT_TRUE(reset);
  std::string data =
      "{\"id\":1, \"age\":20,\"car\":null,\"job\":\"none\"}";
  std::string postdata = client_instance_post.PostData(data);
  EXPECT_EQ(postdata, "Data stored");

  data =
      "{\"id\":2, \"age\":21,\"car\":null,\"job\":\"Apple\"}";
  postdata = client_instance_post.PostData(data);
  EXPECT_EQ(postdata, "Data stored");

  std::string getData = client_instance_get_1.GetData();


  cJSON *request_val = cJSON_Parse(getData.c_str());

  EXPECT_NE(request_val, nullptr);

  if (request_val!=nullptr) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(request_val, "id");
    EXPECT_EQ(data->valueint,1); //expected 1 since top of the queue
  }
  cJSON_Delete(request_val);


  getData = client_instance_get_1.GetData();


  cJSON *request_val2 = cJSON_Parse(getData.c_str());

  EXPECT_NE(request_val2, nullptr);

  if (request_val2!=nullptr) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(request_val2, "id");
    EXPECT_EQ(data->valueint,2); //expected 2 since top of the queue
  }
  cJSON_Delete(request_val2);

  
  
}
