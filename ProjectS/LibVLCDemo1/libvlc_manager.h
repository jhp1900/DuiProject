#pragma once
#include"vlc/vlc.h"

class LibVLCManager
{
public:
  LibVLCManager();
  ~LibVLCManager();

  void Init();
  void Play(HWND player_wnd);  // 播放
  void Stop(HWND hWnd);  // 停止
  void Pause(); // 暂停
  void Goon();  // 继续播放

private:
  libvlc_instance_t *inst_;  // libVLC 的一个实例
  libvlc_media_t *media_;    // 一个可播放的媒体
  libvlc_media_player_t *player_; // VLC 媒体播放器
  libvlc_time_t time_len_;  // 媒体时长
  int width_;               // 视频宽度
  int height_;              // 视频高度
  int wait_time_ = 5000;    // 等待时长
};

