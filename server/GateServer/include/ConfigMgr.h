//
// Created by YeSho on 2024/9/11.
//

#ifndef GATESERVER_CONFIGMGR_H
#define GATESERVER_CONFIGMGR_H

#include "const.h"

struct SectionInfo {
  SectionInfo()= default;
  ~SectionInfo(){
    _section_datas.clear();
  }
  SectionInfo(const SectionInfo& src) {
    _section_datas = src._section_datas;
  }
  SectionInfo& operator = (const SectionInfo& src) {
    if (&src != this) {  // 注意这里的条件被修改了
      this->_section_datas = src._section_datas;
    }
    return *this;  // 无论如何都返回 *this
  }
  std::map<std::string, std::string> _section_datas;
  std::string  operator[](const std::string  &key) {
    if (_section_datas.find(key) == _section_datas.end()) {
      return "";
    }
    // 这里可以添加一些边界检查
    return _section_datas[key];
  }
};

class ConfigMgr
{
public:
  ~ConfigMgr() {
    _config_map.clear();
  }
  SectionInfo operator[](const std::string& section) {
    if (_config_map.find(section) == _config_map.end()) {
      return {};
    }
    return _config_map[section];
  }
  ConfigMgr& operator=(const ConfigMgr& src) {
    if (&src != this) {
      this->_config_map = src._config_map;
    }
    return *this;
  };
  ConfigMgr(const ConfigMgr& src) {
    this->_config_map = src._config_map;
  }

  static ConfigMgr& Inst(){
    static ConfigMgr cfg_mgr;
    return cfg_mgr;
  }
private:
  // 存储section和key-value对的map
  ConfigMgr();
  std::map<std::string, SectionInfo> _config_map;
};


#endif//GATESERVER_CONFIGMGR_H
