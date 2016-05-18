#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NetStruct
{
  CDuiString play_name;   // 方案名
  CDuiString ip_address;  // IP 地址
  CDuiString netmask;     // 子网掩码
  CDuiString gateway;     // 默认网关
  CDuiString firstDNS;    // 首选DNS
  CDuiString secondDNS;   // 备用DNS
  vector<pair<CDuiString, CDuiString>> more_ip_mask;  // 可添加的多个 IP 信息
} NetStruct;

class XmlManager
{
public:
  XmlManager();
  ~XmlManager();

public:
  BOOL IsLoad() const { return is_load_; }
  BOOL LoadFile(CDuiString file_path, CDuiString file_name);
  void InsertNode(NetStruct net_info);
  NetStruct GetNodeInfo(LPCTSTR name);
  vector<NetStruct> GetAllNode();
  vector<CDuiString> GetAllNodeName();

public:
  char * WideToMulti(CDuiString wide, char * multi);    // 宽字符转多字节
  CDuiString MultiToWide(string multi);                 // 多字节转宽字符 

public:
  vector<LPCTSTR> net_attrs_;

private:
  pugi::xml_document file_;       // 配置文件
  pugi::xml_node root_node_;      // 文件根节点
  CDuiString path_and_name_;      // 带路径文件名
  BOOL is_load_;                  // 文件加载状态， true 表示加载成； false 表示加载失败或没加载。
};

