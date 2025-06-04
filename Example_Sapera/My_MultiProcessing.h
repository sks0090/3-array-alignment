//20240524 modified

#include "windows.h"

#ifndef __KYLE_MULTIPROCESSING__
#define __KYLE_MULTIPROCESSING__

class Kyle_MultiProcessing
{
	typedef struct {
		HANDLE Event;
		HANDLE m_Thread;
		DWORD ThreadID;
		bool b_Thread;
		bool b_Proc;
		bool b_Init;
		int numThread;
		void (*Func)(void*, int, int, float*);
		void (*FuncEnd)(void*, int);
		Kyle_MultiProcessing* pDlg;
	}ThreadControlStruct;

	typedef struct {
		HANDLE Event;
		HANDLE m_Thread;
		DWORD ThreadID;
		bool b_Thread;
		bool b_Proc;
		bool b_Init;
		float progress;
		int numThread;

		ThreadControlStruct* pDlg;
	}ThreadStruct;


public:
	Kyle_MultiProcessing();
	~Kyle_MultiProcessing();

	int countControlThread;
	int countProcThread;
	int paramnum;
	ThreadControlStruct* LastControlValue;
	ThreadStruct* ThreadValue;
	ThreadControlStruct *ThreadValueControl;

	bool Init(int Count_ControlThread, int Count_ProcessingThread);
	bool Free(void);
	//synchronized�� true�� Processing ������ return(ProcEnd�� ���� Ÿ�̹�) 
	//synchronized�� false�� Processing ������ return 
	//false return�� ���� Processing �ȵȰ�
	//progress ������Ʈ�� �ʿ��ϸ� *progress �� 0~100.0���� ����� �Լ����� update�� �ָ� ��
	//static void MyProc(void* userdataptr, int threadnum, int controlnum,float* progress);
	//int blocksize = pDlg->t_sizey / pDlg->t_cntthread;
	//int starty = (threadnum == 0) ? 0 : threadnum * blocksize;
	//int endy = ((threadnum == pDlg->t_cntthread - 1) ? pDlg->t_sizey : (threadnum + 1) * blocksize);
	//static void MyProcEnd(void* userdataptr, int controlnum);
	bool InputProcess(void (*func) (void*, int, int, float*), void(*funcend) (void*, int), void* userdataptr, bool synchronize);
	void* UserDataPtr;
	unsigned int PID;
	//���� thread�� End Event üũ
	HANDLE** EventMulti;
	//control thread�� End Event üũ
	HANDLE* EventControl;
	float GetThreadProgress(void);

	
protected:
	static DWORD WINAPI ThreadProc(LPVOID lParam);
	static DWORD WINAPI ThreadControl(LPVOID lParam);
};

#endif