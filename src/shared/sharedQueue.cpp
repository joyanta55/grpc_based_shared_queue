#include "sharedQueue.h"

void SharedQueue::init() { list_grpc_item = {}; }

sharedRT SharedQueue::insertValue(std::string str) {
  sharedRT rt;
  rt.success = true;
  if (size > 0 && list_grpc_item.size() >= size) {
    rt.success = false;
    rt.reason = "Queue/Stack size exceeded";
    return rt;
  }
  cJSON *request_val = cJSON_Parse(str.c_str());

  if (request_val == NULL) {
    rt.success = false;
    rt.reason = "Input json not formatted properly";
  } else {
    list_grpc_item.push_back(request_val);
  }

  return rt;
}

sharedRT SharedQueue::popValue() {
  sharedRT rt;
  rt.success = true;
  if (list_grpc_item.empty()) {
    rt.data = "";
    return rt;
  }
  cJSON *list_front_item = list_grpc_item.front();
  list_grpc_item.pop_front();
  char *data = cJSON_Print(list_front_item);
  rt.data = std::string(data);
  return rt;
}

void SharedQueue::reset(){
  list_grpc_item.clear();
}



