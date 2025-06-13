#include <iostream>
#include <fstream>
#include <librdkafka/rdkafkacpp.h>

int main() {
    const char* pipe_path = "./ipc_pipe";
    std::ifstream pipe(pipe_path);

    std::string msg;
    std::string brokers = "localhost:9092";
    std::string topic = "chat-logs";

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, nullptr);
    RdKafka::Producer* producer = RdKafka::Producer::create(conf, nullptr);

    while (std::getline(pipe, msg)) {
        producer->produce(topic, RdKafka::Topic::PARTITION_UA,
                          RdKafka::Producer::RK_MSG_COPY,
                          const_cast<char*>(msg.c_str()), msg.length(),
                          nullptr, nullptr);
        producer->poll(0);
    }

    producer->flush(1000);
    delete producer;
    delete conf;
    return 0;
}
