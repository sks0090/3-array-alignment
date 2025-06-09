#include "pch.h"
#include "My_MultiProcessing.h"

Kyle_MultiProcessing::Kyle_MultiProcessing()
{

}
Kyle_MultiProcessing::~Kyle_MultiProcessing()
{

}

bool Kyle_MultiProcessing::Init(int Count_ControlThread,int Count_ProcessingThread)//, float* progress)
{
	countControlThread=Count_ControlThread;
	countProcThread=Count_ProcessingThread;
//	Progress = progress;
	
	PID = 0;
	ThreadValueControl = (ThreadControlStruct*)calloc(countControlThread,sizeof(ThreadControlStruct));
	LastControlValue = ThreadValueControl;
	ThreadValue = (ThreadStruct*)calloc(countControlThread*countProcThread, sizeof(ThreadStruct));
	EventMulti = (HANDLE**)calloc(countControlThread, sizeof(HANDLE*));
	EventControl = (HANDLE*)calloc(countControlThread, sizeof(HANDLE));
	for(int i=0;i<countControlThread;i++)
	{
		EventMulti[i] = (HANDLE*)calloc(countProcThread, sizeof(HANDLE));
		for(int j=0;j<countProcThread;j++)
		{
			//Processing thread init 및 생성
			EventMulti[i][j] = (HANDLE)CreateEvent(0, FALSE, FALSE, 0);
			
			ThreadValue[i * countProcThread + j].Event = (HANDLE)CreateEvent(0, FALSE, FALSE, 0);
			ThreadValue[i * countProcThread + j].b_Thread = true;
			ThreadValue[i * countProcThread + j].b_Proc = false;
			ThreadValue[i * countProcThread + j].b_Init = false;
			ThreadValue[i * countProcThread + j].progress = 0.0;
			ThreadValue[i * countProcThread + j].numThread = j;
			ThreadValue[i * countProcThread + j].pDlg = &ThreadValueControl[i];
		}

		//Control thread init 및 생성
		EventControl[i] = (HANDLE)CreateEvent(0, TRUE, FALSE, 0);
		ThreadValueControl[i].Event=(HANDLE)CreateEvent(0,FALSE,FALSE,0);
		ThreadValueControl[i].b_Thread=true;
		ThreadValueControl[i].b_Proc=false;
		ThreadValueControl[i].b_Init = false;
		ThreadValueControl[i].numThread = i;
		ThreadValueControl[i].pDlg = this;
	}

	return 0;
}



bool Kyle_MultiProcessing::Free(void)
{
	//Processing중이면 끝날때까지 기다리고 thread 종료
	for (int i = 0; i < countControlThread * countProcThread;i++)
	{
		while (ThreadValue[i].b_Proc)
			Sleep(1);
		ThreadValue[i].b_Thread = false;
		SetEvent(ThreadValue[i].Event);
	}
	for (int i = 0; i < countControlThread; i++)
	{
		while (ThreadValueControl[i].b_Proc)
			Sleep(1);
		ThreadValueControl[i].b_Thread = false;
		SetEvent(ThreadValueControl[i].Event);
	}
	Sleep(100);
	//핸들반환
	for (int i = 0; i < countControlThread; i++)
	{
		for (int j = 0; j < countProcThread; j++)
		{
			if (ThreadValue[i * countProcThread + j].b_Init)
				CloseHandle(ThreadValue[i * countProcThread + j].m_Thread);
			CloseHandle(ThreadValue[i * countProcThread + j].Event);
			CloseHandle(EventMulti[i][j]);
			
		}
		if (ThreadValueControl[i].b_Init)
			CloseHandle(ThreadValueControl[i].m_Thread);
		CloseHandle(EventControl[i]);
		CloseHandle(ThreadValueControl[i].Event);
		free(EventMulti[i]);		
	}
	free(EventMulti);
	free(EventControl);
	//배열 정리
	free(ThreadValue);
	free(ThreadValueControl);

	return 0;
}

bool Kyle_MultiProcessing::InputProcess(void (*func) (void*, int, int, float*), void(*funcend) (void*, int), void* userdataptr, bool synchronize)
{
	//Func = func;
	//FuncEnd = funcend;
	UserDataPtr = userdataptr;
	int id;
	bool flag;
	bool b_inque = false;
	//thread schedule
	for (int i = 0; i < countControlThread; i++)
	{
		id = (PID + i) % countControlThread;
		if (ThreadValueControl[id].b_Proc == false)
		{
			ThreadValueControl[id].b_Proc = true;
			ResetEvent(EventControl[id]);

			ThreadValueControl[id].Func = func;
			ThreadValueControl[id].FuncEnd = funcend;
			LastControlValue = &ThreadValueControl[id];
			if (ThreadValueControl[id].b_Init)
				SetEvent(ThreadValueControl[id].Event);
			else
				ThreadValueControl[id].m_Thread = CreateThread(NULL, 0, ThreadControl, &ThreadValueControl[id], 0, &ThreadValueControl[id].ThreadID);
			PID = PID + i + 1;
			i = countControlThread;
			b_inque = true;
			if (synchronize)
			{
				bool flag = true;
				while (flag)
				{
					MSG msg;
					if (MsgWaitForMultipleObjects(1, &EventControl[id], FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
						flag = false;
					else
					{
						if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}
				}

			}
		}
	}
	return b_inque;
}

float Kyle_MultiProcessing::GetThreadProgress(void)
{
	float ret = 0;
	for (int i = 0; i < countProcThread; i++)
	{
		ret += ThreadValue[LastControlValue->numThread * countProcThread + i].progress;
	}
	return ret / float(countProcThread);
}


//Proc 함수 실행하는 thread
DWORD Kyle_MultiProcessing::ThreadProc(LPVOID lParam)
{
	ThreadStruct *pDlg=(ThreadStruct *)lParam;
	pDlg->b_Init = true;
	while(pDlg->b_Thread)
	{	
		pDlg->pDlg->Func(pDlg->pDlg->pDlg->UserDataPtr, pDlg->numThread, pDlg->pDlg->numThread, &pDlg->progress);
		pDlg->progress = 100.0;
		pDlg->b_Proc=false;
		SetEvent(pDlg->pDlg->pDlg->EventMulti[pDlg->pDlg->numThread][pDlg->numThread]);
		WaitForSingleObject(pDlg->Event,INFINITE);
	}
	return 0;
}

//하위 thread control및 endproc 수행
DWORD Kyle_MultiProcessing::ThreadControl(LPVOID lParam)
{
	ThreadControlStruct *pDlg=(ThreadControlStruct *)lParam;
	bool flag;
	pDlg->b_Init = true;
	while(pDlg->b_Thread)
	{
		for(int i=0;i<pDlg->pDlg->countProcThread;i++)
		{
			pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].progress = 0.0;
			pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].b_Proc = true;
			ResetEvent(pDlg->pDlg->EventMulti[pDlg->numThread][i]);
			
			if(pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].b_Init)
				SetEvent(pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].Event);
			else
				pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].m_Thread = CreateThread(NULL, 0, pDlg->pDlg->ThreadProc, &pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread], 0, &pDlg->pDlg->ThreadValue[i + pDlg->pDlg->countProcThread * pDlg->numThread].ThreadID);
		}
		WaitForMultipleObjects(pDlg->pDlg->countProcThread, pDlg->pDlg->EventMulti[pDlg->numThread], TRUE, INFINITE);
		if(pDlg->FuncEnd !=NULL)
			pDlg->FuncEnd(pDlg->pDlg->UserDataPtr, pDlg->numThread);
		
		pDlg->b_Proc=false;
		SetEvent(pDlg->pDlg->EventControl[pDlg->numThread]);
		WaitForSingleObject(pDlg->Event,INFINITE);
	}
	return 0;
}