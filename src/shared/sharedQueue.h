#include "cjson/cJSON.h"
#include "iostream"
#include "list"
#include "string"

typedef struct sharedQueueReturnType
{
    bool success;
    std::string reason;
    std::string data;
} sharedRT;


class SharedQueue {
public:
  SharedQueue() { this->size = -1; }
  SharedQueue(long unsigned int size) { this->size = size; }
  void init();
  sharedRT insertValue(std::string str);
  sharedRT popValue();
  void reset();

private:
  long unsigned int size;
  std::list<cJSON *> list_grpc_item;
};
