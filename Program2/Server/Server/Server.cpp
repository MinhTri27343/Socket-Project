// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include"Func.h"
//#pragma pack(push, 1)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

DWORD WINAPI function_cal(LPVOID arg)
{
    
    cout << "Connect is successful!\n";

    SOCKET* hConnected = (SOCKET*)arg;
    CSocket mysock;
    //Chuyen ve lai CSocket
    mysock.Attach(*hConnected);
    if (SendInfoAllFileToClient(ref(mysock)) == false)
    {
        return 0;
    }
    vector<pair<ifstream, File>> v;
    vector<File> tmp;
    while (true)
    {
        if (SendFileDownloadToClient(ref(mysock), v) == false)
            break;
    }
    cout << endl << "Disconnected from client" << endl;
    delete hConnected;
    return 0;
}
int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            cout << "\t\t\t\t\t ____  _____ ______     _______ ____\n";
            cout << "\t\t\t\t\t/ ___|| ____|  _ \\ \\   / /____ |  _ \\\n";
            cout << "\t\t\t\t\t\\___ \\|  _| | |_) \\ \\ / /|  _| | |_) |\n";
            cout << "\t\t\t\t\t ___) | |___|  _ < \\ V / | |___|  _ <\n";
            cout << "\t\t\t\t\t|____/|_____|_| \\_\\ \\_/  |_____|_| \\_\\\n";

            // TODO: code your application's behavior here.
            AfxSocketInit(NULL);
            CSocket server, s;
            DWORD threadID;
            HANDLE threadStatus;
            server.Create(1234);
            do
            {
                server.Listen();
                cout << "Server is listening!\n";
                server.Accept(s);
                cout << "\n";


                //Khoi tao con tro Socket
                SOCKET* hConnected = new SOCKET();
                //Chuyển đỏi CSocket thanh Socket
                *hConnected = s.Detach();
                //Khoi tao thread tuong ung voi moi client Connect vao server.
                //Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
                threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);
            } while (true);

        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
