#pragma once
#include"vlc/vlc.h"

class LibVLCManager
{
public:
  LibVLCManager();
  ~LibVLCManager();

  void Init();
  void Play(HWND player_wnd);  // ����
  void Stop(HWND hWnd);  // ֹͣ
  void Pause(); // ��ͣ
  void Goon();  // ��������

private:
  libvlc_instance_t *inst_;  // libVLC ��һ��ʵ��
  libvlc_media_t *media_;    // һ���ɲ��ŵ�ý��
  libvlc_media_player_t *player_; // VLC ý�岥����
  libvlc_time_t time_len_;  // ý��ʱ��
  int width_;               // ��Ƶ���
  int height_;              // ��Ƶ�߶�
  int wait_time_ = 5000;    // �ȴ�ʱ��
};

