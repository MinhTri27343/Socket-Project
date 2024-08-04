// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Func.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

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
            // TODO: code your application's behavior here.
            //Initialize
            AfxSocketInit(NULL);
            CSocket client;
            char ip[100];
            int port;
            client.Create();
            cout << "\t \t \t \t \t  ____ _     ___ _____ _   _ _____ \n";
            cout << "\t \t \t \t \t / ___| |   |_ _| ____| \\ | |_   _|\n";
            cout << "\t \t \t \t \t| |   | |    | ||  _| |  \\| | | |  \n";
            cout << "\t \t \t \t \t| |___| |___ | || |___| |\\  | | |  \n";
            cout << "\t \t \t \t \t \\____|_____|___|_____|_| \\_| |_|  \n";
            // input
            //cout << "Input IP address of server: "; cin >> ip;
            //cout << "Input port of server: "; cin >> port;

            if (client.Connect(CA2W("127.0.0.1"), 1234))
            {

                // =============================================== Duc Tri ====================================================================
                cout << "\nClient has connected to the server";
                cout << endl;
                ReceiveInfoAllFileFromServer(ref(client));
                signal(SIGINT, SignalCallBack);
                COORD cursorPos = getCoordinate();
                // ============================================== Duc Tri ====================================================================


               // =============================================== Minh tri ====================================================================


                // ================= Sending file name need to download for server ======================
                string file_user = "input.txt";
                string file_name = "file.txt";
                int size_pre_file = 0;
                int size_after_file = 0;
                while (true)
                {
                    string file_check1 = file_user;
                    string file_check2 = file_name;
                    bool CheckIsExist = true;
                    char* str = NULL;
                    while (size_pre_file == size_after_file && CheckIsExist == true)
                    {
                        // ================= Sending file name need to download for server ======================
                        SendFileNeedDownToServer(ref(client), file_user, size_pre_file, size_after_file, file_check1, file_check2, CheckIsExist, str);
                    }
                    size_pre_file = size_after_file;
                    int num_file = 0;
                    if (str != NULL)
                        num_file = NumOfFile(str);
                    int count_file = 0;
                    // ===================== Ghi nhan ten file download tu server  =============================
                    // Initialize
                    while (count_file != num_file)
                    {
                        int size_name_file_download = 0;
                        unsigned long long size_file_download = 0;
                        char* name_file_download = NULL;

                        // ===================== Ghi nhan ten  file download tu server  =============================

                        // ================== Display console percent of file downloading ================

                        ReceiveInfo1FileFromServer(ref(client), size_name_file_download, size_file_download, name_file_download);
                        Receive1FileFromServer(ref(client), name_file_download, size_file_download, cursorPos);
                        count_file++;
                    }
                }
                // ================== Display console percent of file downloading ================
            }
            // =============================================== Minh tri ====================================================================
            else
            {
                cout << "\nUnable to connect to the server";
            }

            client.Close();

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
