#pragma once

class Log
{
public:
	static Log& getInstance(); 
	
	//Ĭ�ϳ�ʼ������
	void init();
	
	//ͨ�������ļ���ʼ������
	void configure(const std::string& cfg);
	
	//������־���ĵȼ�
	void setLogLevel(const std::string &level);

   /**
	*����context֮�󣬵��ô˷���
	*@param msg ��ʶ����Ϣ
	*/
	void pushNDC(const std::string& msg);

	/**
	*�뿪context֮�󣬵��ô˷���
	*/
	void popNDC();

	/**
	*��Ҫ�뿪threadʱ�����ô˷�����ȷ���ͷ��ڴ�
	*/
	void removeNDC();
	
	//������־����
	void Trace(const std::string& msg);
	void Debug(const std::string& msg);
	void Info(const std::string& msg);
	void Warn(const std::string& msg);
	void Error(const std::string& msg);
	void Fatal(const std::string& msg);

	Log::~Log();

protected:
	Log::Log();

private: 
	static Log *log;
	static log4cplus::Logger logger;
	static log4cplus::NDC& ndc;
};