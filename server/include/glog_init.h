#ifndef __GLOG_INIT__
#define __GLOG_INIT__

#include <glog/logging.h>

void glog_init(const char* argv0)
{
	//FLAGS_log_dir = log_path;//����log���Ŀ¼
	FLAGS_stderrthreshold = google::ERROR;//���ڵ���ERROR����ĲŻ����������̨
	FLAGS_stop_logging_if_full_disk = 1; //����û�ռ���ֹͣдlog
	FLAGS_logbufsecs = 0;//��log�ͻ�д���������棬��������
	google::InitGoogleLogging(argv0);
}

#endif