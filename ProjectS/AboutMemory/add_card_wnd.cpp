#include "add_card_wnd.h"
#include <atlstr.h>
#include "file_system.h"
#include "manager.h"

AddCardWnd::~AddCardWnd()
{
}

bool AddCardWnd::DoModal(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
  if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;

  CenterWindow(pa_hwnd_);

  res_layout_ = static_cast<PDUI_TILELAYOUT>(m_PaintManager.FindControl(_T("res_layout")));
  return ShowModal() != 0;
}

void AddCardWnd::Notify(TNotifyUI& msg)
{
  if (msg.sType == _T("itemselect"))
    OnComboSelect(msg);

  __super::Notify(msg);
}

void AddCardWnd::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("add_btn"))   // ȷ�������Ϣ
    OnClickAddBtn();
  else if (msg.pSender->GetName() == _T("cancel_btn"))// ȡ�������Ϣ
    Close();
  else if (msg.pSender->GetName() == _T("delete"))
    OnClickDelBtn(msg.pSender);
}

LRESULT AddCardWnd::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  PDUI_COMBO res_type_combo = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("type")));
  CDuiString res_type_str = res_type_combo->GetText();
  if (res_type_str == _T("Text"))
    return LRESULT();

  HDROP hdrop = (HDROP)wParam;
  int file_count = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0); // ��ק�ļ�����
  TCHAR file_name[MAX_PATH] = { 0 };
  TCHAR ex_name[20] = { 0 };

  for (int i = 0; i < file_count; i++) {
    DragQueryFile(hdrop, i, file_name, MAX_PATH);//�����ҷ���ļ���
    lstrcpy(ex_name, ::PathFindExtension(file_name));	// �õ��ļ���׺��

    bool non_folder = lstrcmp(ex_name, _T(".")) && lstrcmp(ex_name, _T(".."));    // true��ʾ���ļ���
    bool type_ok = FileTypeIsOk(file_name, ex_name, res_type_str);

    if (non_folder && type_ok) {
      SetResElem(file_name);
      res_layout_->Add(res_elem_);
    } else {
      CDuiString message = _T("");
      message += file_name;
      message += _T("\n���Ƿ������͵���Դ��\n������ѡ��");
      MessageBox(NULL, message, _T("Erorr"), MB_OK);
    }
  }
  DragFinish(hdrop);      //�ͷ�hdrop+

  return LRESULT();
}

void AddCardWnd::OnClickAddBtn()
{
  NodeStruct node;
  NodeStructInit(node);

  Manager *manager = Manager::GetInstance();
  UserDataManage *user_manager = UserDataManage::GetInstance();
  xml_node pa_node = user_manager->GetNodeForAttr(_T("Memory")
    , _T("MemoryObject")
    , _T("owner")
    , manager->GetOwner());
  user_manager->AddMemoryCard(node, pa_node);
}

/**
 *  ���ܣ� ��Ӧ Combobox ��ѡ�񣬸��ݲ�ͬ��ѡ����ʾ��������Դ��ʾ��
 */
void AddCardWnd::OnComboSelect(const TNotifyUI & msg)
{
  CDuiString res_type = msg.pSender->GetText();

  if (res_type == _T("Text")) {
    res_layout_->SetVisible(false);
  } else {
    res_layout_->RemoveAll();
    res_layout_->SetVisible(true);
  }
}

/**
 *  ���ܣ� �ж��ļ���ʽ�Ƿ������Դ����
 *  ������ TCHAR ex_name[20] �ļ���չ����  CDuiString res_type ��Դ����
 *  ����ֵ��  bool ���� true ��ʾ��ʽ���� ���� false ��ʾ��ʽ������
 */
bool AddCardWnd::FileTypeIsOk(TCHAR file_name[MAX_PATH], TCHAR ex_name[20], CDuiString res_type)
{
  bool type_ok = false;
  bkimage_ = CPaintManagerUI::GetResourcePath();
  if (res_type == _T("Picture")) {        // ͼƬ��ʽ
    type_ok = !(StrCmpI(ex_name, _T(".bmp"))
      && StrCmpI(ex_name, _T(".jpg"))
      && StrCmpI(ex_name, _T(".jpeg"))
      && StrCmpI(ex_name, _T(".png")));
    bkimage_ = file_name;
  } else if (res_type == _T("Video")) {   // ��Ƶ��ʽ
    type_ok = !(StrCmpI(ex_name, _T(".mp4"))
      && StrCmpI(ex_name, _T(".avi"))
      && StrCmpI(ex_name, _T(".mkv"))
      && StrCmpI(ex_name, _T(".mov"))
      && StrCmpI(ex_name, _T(".wmv"))
      && StrCmpI(ex_name, _T(".3gp")));
    bkimage_ += _T("res\\video.png");
  } else if (res_type == _T("BGM")) {     // ���ָ�ʽ
    type_ok = !(StrCmpI(ex_name, _T(".mp3"))
      && StrCmpI(ex_name, _T(".wav"))
      && StrCmpI(ex_name, _T(".wma")));
    bkimage_ += _T("res\\music.png");
  }

  return type_ok;
}

void AddCardWnd::SetResElem(LPCTSTR file_path_name)
{
  CDialogBuilder builder;
  res_elem_ = nullptr;
  res_elem_ = static_cast<PDUI_CONTAINER>(builder.Create(_T("res_elem.xml")));

  res_elem_->SetMinHeight(100);
  res_elem_->SetMinWidth(90);
  res_elem_->SetBkImage(bkimage_);

  CDuiString file_name = file_path_name;
  file_name = file_name.Right(file_name.GetLength() - file_name.ReverseFind('\\') - 1);
  //PDUI_LABEL label = static_cast<PDUI_LABEL>(res_elem_->FindSubControl(_T("name")));
  //label->SetText(file_name);
  //label = static_cast<PDUI_LABEL>(res_elem_->FindSubControl(_T("path_name")));
  //label->SetText(file_path_name);

  PDUI_LABEL label = static_cast<PDUI_LABEL>(res_elem_->FindSubControl(_T("path_name")));
  label->SetText(file_path_name);
  label = static_cast<PDUI_LABEL>(res_elem_->FindSubControl(_T("name")));
  label->SetText(file_name);
}

void AddCardWnd::OnClickDelBtn(CControlUI * pSender)
{
  PDUI_CONTROL control = static_cast<PDUI_CONTROL>(pSender->GetParent());
  int count = res_layout_->GetItemIndex(control);
  res_layout_->RemoveAt(count);
}

LPCTSTR AddCardWnd::GetSysTime()
{
  TCHAR str_sys_time[MAX_PATH] = { 0 };
  SYSTEMTIME st;
  ::GetLocalTime(&st);
  _stprintf_s(str_sys_time, _T("%04d%02d%02d%02d%02d%02d%04d")
    , st.wYear, st.wMonth, st.wDay, st.wHour
    , st.wMinute, st.wSecond, st.wMilliseconds);

  return str_sys_time;
}

void AddCardWnd::NodeStructInit(NodeStruct & node)
{
  PDUI_DATETIME card_time = static_cast<PDUI_DATETIME>(m_PaintManager.FindControl(_T("card_time")));
  PDUI_COMBO type_combo = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("type")));
  UserDataManage *user_manager = UserDataManage::GetInstance();

  char time[MAX_PATH] = { 0 };
  char type[20] = { 0 };
  char res_name[MAX_PATH] = { 0 };
  user_manager->WideToMulti(card_time->GetText(), time);
  user_manager->WideToMulti(type_combo->GetText(), type);

  node.node_name = "MemoryCard";    // �����������ڵ� �� MemoryCard �ڵ�
  node.attrs.push_back({ "time", time });   // ���� MemoryCard ��ʱ������

  Manager *manager = Manager::GetInstance();
  DataFileInfo user_info = manager->GetUserInfo();
  CDuiString file_name = _T("");
  int count = res_layout_->GetCount();
  NodeStruct *child_node = nullptr;
  TCHAR ex_name[10] = { 0 };

  for (int i = 0; i < count; ++i) {
    res_elem_ = static_cast<PDUI_CONTAINER>(res_layout_->GetItemAt(i));
    PDUI_LABEL label = static_cast<PDUI_LABEL>(res_elem_->FindSubControl(_T("path_name")));
    LPCTSTR temp = label->GetText();
    lstrcpy(ex_name, ::PathFindExtension(res_elem_->GetBkImage()));	// �õ��ļ���׺��
    file_name = manager->GetOwner();  // �µ��ļ������� ������ + ϵͳʱ�� + �ļ���׺
    file_name += GetSysTime();
    file_name += ex_name;
    /* ���ļ����ݵ���Դ�ļ��� */
    CopyFile(res_elem_->GetBkImage(), user_info.path + _T("res\\") + file_name, true);
    user_manager->WideToMulti(file_name, res_name);
    child_node = new NodeStruct;
    child_node->node_name = "Resources";
    child_node->attrs.push_back({ "res_name", res_name });
    child_node->attrs.push_back({ "type", type });
    node.child_nodes.push_back(child_node);
    Sleep(1);
  }
}
