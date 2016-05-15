#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NodeStruct
{
  string node_name;   // 节点名
  vector<pair<string, string>> attrs;  // 属性对向量, first为属性名， second为属性值
} NodeStruct;

class XmlManager
{
public:
  XmlManager();
  ~XmlManager();

public:
  BOOL IsLoad() const { return is_load_; }
  BOOL LoadFile(CDuiString file_path, CDuiString file_name);
  void InsertNode(NodeStruct node_info);
  NodeStruct GetNodeInfo(LPCTSTR node_name);
  vector<NodeStruct> GetAllNode();
  vector<LPCTSTR> GetAllNodeName();

public:
  char * WideToMulti(CDuiString wide, char * multi);    // 宽字符转多字节
  CDuiString MultiToWide(string multi);                 // 多字节转宽字符 

private:
  pugi::xml_document file_;       // 配置文件
  pugi::xml_node root_node_;      // 文件根节点
  CDuiString path_and_name_;      // 带路径文件名
  BOOL is_load_;                  // 文件加载状态， true 表示加载成； false 表示加载失败或没加载。
};

