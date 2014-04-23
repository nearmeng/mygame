#ifndef __GLOG_INIT__
#define __GLOG_INIT__

#include <glog/logging.h>

void glog_init(const char* argv0)
{
	//FLAGS_log_dir = log_path;//设置log输出目录
	FLAGS_stderrthreshold = google::ERROR;//高于等于ERROR级别的才会输出到控制台
	FLAGS_stop_logging_if_full_disk = 1; //磁盘没空间了停止写log
	FLAGS_logbufsecs = 0;//有log就会写道磁盘上面，不做缓存
	google::InitGoogleLogging(argv0);
}

#endif