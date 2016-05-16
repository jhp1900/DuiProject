#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"


using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;

/**
 *  结构体名： NodeStruct 节点结构体
 *  功能： 用于构建一个节点，节点本身是一个简单的只有 节点名和属性 构造的节点
 *        eg: <NodeName owner="" time="" type="" path="" />
 *        但本结构体可以包含一个 子节点向量，这样也就形成了一颗节点子树
 */
typedef struct _NodeStruct
{
  string node_name;   // 节点名
  vector<pair<string, string> > attrs;  // 属性对向量,first为属性名，second为属性值
  vector<struct _NodeStruct *> child_nodes;   // 子节点向量
} NodeStruct;

/**
 *  类名： NodeFd
 *  功能： 用于 find_node() 查找函数的参数传递
 */
class NodeFd
{
public:
  NodeFd() {}
  NodeFd(string name) : node_name(name) {}

  void SetNodeName(string name) { node_name = name; }

  bool operator()(xml_node node) const
  {
    return (string(node.name()) == node_name);
  }

private:
  string node_name;
};

/**
 *  类名： XmlManageBase
 *  功能： XML 文件操作基类。
 *  说明： 本类通过对 pugixml 工具库的使用，来实现本程序对 XML 文件的各种操作
 *         内容主要为对 XML 文件的节点、属性的  增、删、改、查 等操作的实现。
 */
class XmlManageBase
{
public:
  XmlManageBase() {}
  ~XmlManageBase() {}

public:     // API 部分
  bool LoadFile(CDuiString file_path, CDuiString file_name); // 加载文件
  bool IsLoad() const { return is_load_; }    // 文件加载是否成功
  xml_node FindNode(char *node_name);     // 获取某个节点

  xml_node GetNodeForAttr(    // 根据属性，获取某个节点
    CDuiString pa_node_name, 
    CDuiString node_name, 
    CDuiString attr_name, 
    CDuiString attr_value);

  vector<xml_node> GetAllChild(CDuiString pa_node_name);    // 得到某节点的所有子节点
  void SetAttr(char *node_name, char *attr_name, char *attr_value);// 设置某个节点的某个属性
  CDuiString GetAttr(char *str_node, char *attr_name);            // 获取某个节点的某个属性
  void InsertSimpleNode(NodeStruct node_info, xml_node pa_node);  // 添加一个简单节点
  bool IsHave(char *pa_node_name, char *node_name);     // 检测某个父节点下是否拥有某个子节点

public:     // 工具函数部分
  char * WideToMulti(CDuiString wide, char * multi);    // 宽字符转多字节
  CDuiString MultiToWide(string multi);                 // 多字节转宽字符 

protected:
  xml_document file_;  // 配置文件
  xml_node root_node_;      // 文件根节点
  CDuiString path_and_name_;  // 带路径文件名
  bool is_load_;      // 文件加载状态， true 表加载成功， false 表加载失败或没加载
};

