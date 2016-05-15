#include "Windows.h"
#include "stdafx.h"
#include "libvlc_manager.h"

LibVLCManager::LibVLCManager()
{
  inst_ = libvlc_new(0, NULL);  // ����һ�� VLC ��ʵ��
}


LibVLCManager::~LibVLCManager()
{
}

void LibVLCManager::Init()
{
  if (!(media_ && player_)) {
    media_ = libvlc_media_new_path(inst_, "cuc_ieschool.flv");  // ����(����)һ���ɲ��ŵ�ý��
    player_ = libvlc_media_player_new_from_media(media_);   // ���ò���ý�崴��һ��������
    libvlc_media_release(media_);
  }
}

void LibVLCManager::Play(HWND player_wnd)
{
  libvlc_media_player_set_hwnd(player_, player_wnd);
  libvlc_media_player_play(player_);  // 
  Sleep(wait_time_);
}

void LibVLCManager::Stop(HWND hWnd)
{
  libvlc_media_player_stop(player_);
  libvlc_media_player_release(player_);
}

void LibVLCManager::Pause()
{
  libvlc_media_player_set_pause(player_, 1);
}

void LibVLCManager::Goon()
{
  libvlc_media_player_set_pause(player_, 0);
}
