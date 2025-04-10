//
// Created by YeSho on 2024/10/18.
//
#include <hiredis/hiredis.h>
#include <string>
#include <chrono>
#include <thread>
#include <uuid/uuid.h>
#include <iostream>

// 生成唯一标识符
std::string generate_uuid() {
  uuid_t uuid;
  uuid_generate(uuid);
  char uuid_str[37]; // 36 characters + null terminator
  uuid_unparse(uuid, uuid_str);
  return std::string(uuid_str);
}

class RedisDistributedLock {
public:
  RedisDistributedLock(redisContext* context, const std::string& lock_key, int lock_timeout_ms = 10000)
      : context_(context), lock_key_(lock_key), lock_timeout_ms_(lock_timeout_ms), locked_(false) {
    uuid_ = generate_uuid();
  }

  // 尝试获取锁
  bool lock() {
    // 使用 SET 命令，并设置 NX 和 PX 选项
    std::string command = "SET " + lock_key_ + " " + uuid_ + " NX PX " + std::to_string(lock_timeout_ms_);
    redisReply* reply = (redisReply*)redisCommand(context_, command.c_str());
    if (reply == nullptr) {
      std::cerr << "Redis command failed\n";
      return false;
    }

    bool success = false;
    if (reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK") {
      success = true;
      locked_ = true;
    }
    freeReplyObject(reply);
    return success;
  }

  // 释放锁
  bool unlock() {
    if (!locked_) {
      return false;
    }

    // 使用 Lua 脚本确保原子性：检查值是否匹配，再删除
    const char* lua_script =
            "if redis.call('GET', KEYS[1]) == ARGV[1] then "
            "   return redis.call('DEL', KEYS[1]) "
            "else "
            "   return 0 "
            "end";

    redisReply* reply = (redisReply*)redisCommand(context_, "EVAL %s 1 %s %s",
                                                    lua_script, lock_key_.c_str(), uuid_.c_str());

    if (reply == nullptr) {
      std::cerr << "Redis EVAL command failed\n";
      return false;
    }

    bool success = false;
    if (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1) {
      success = true;
      locked_ = false;
    }
    freeReplyObject(reply);
    return success;
  }

  ~RedisDistributedLock() {
    if (locked_) {
      unlock();
    }
  }

private:
  redisContext* context_;
  std::string lock_key_;
  std::string uuid_;
  int lock_timeout_ms_;
  bool locked_;
};

int main() {
  // 连接到 Redis
  redisContext* context = redisConnect("127.0.0.1", 6379);
  if (context == nullptr || context->err) {
    if (context) {
      std::cerr << "Connection error: " << context->errstr << "\n";
      redisFree(context);
    } else {
      std::cerr << "Connection error: can't allocate redis context\n";
    }
    return 1;
  }

  std::string lock_key = "my_distributed_lock";
  RedisDistributedLock lock(context, lock_key, 5000); // 锁超时 5 秒

  if (lock.lock()) {
    std::cout << "Lock acquired!\n";

    // 执行临界区代码
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 释放锁
    if (lock.unlock()) {
      std::cout << "Lock released!\n";
    } else {
      std::cout << "Failed to release lock.\n";
    }
  } else {
    std::cout << "Failed to acquire lock.\n";
  }

  // 关闭 Redis 连接
  redisFree(context);
  return 0;
}
