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

            // input
            cout << "Input IP address of server: "; cin >> ip;
            cout << "Input port of server: "; cin >> port;

            if (client.Connect(CA2W(ip), port))
            {

                // =============================================== Duc Tri ====================================================================
                cout << "\nClient has connected to the server";
                cout << endl;
                ReceiveInfoAllFileFromServer(ref(client)); 
                signal(SIGINT, SignalCallBack);
                // ============================================== Duc Tri ====================================================================


               // =============================================== Minh tri ====================================================================


                // ================= Sending file name need to download for server ======================
                string file_user = "input.txt";
                string file_name = "text1.txt";
                unsigned long long size_pre_file = getByteSum(file_user);
                unsigned long long size_after_file = getByteSum(file_user);
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
                    size_after_file = size_pre_file;
                    int num_file = 0;
                    if (str != NULL)
                        num_file = NumOfFile(str);
                    int count_file = 0;
                    // ===================== Ghi nhan ten file download tu server  =============================
                    // Initialize
                    while (count_file != num_file)
                    {
                        unsigned long long size_name_file_download = 0;
                        unsigned long long size_file_download = 0;
                        char* name_file_download = NULL;

                        // ===================== Ghi nhan ten  file download tu server  =============================

                        // ================== Display console percent of file downloading ================

                        ReceiveInfo1FileFromServer(ref(client), size_name_file_download, size_file_download, name_file_download);
                        Receive1FileFromServer(ref(client), name_file_download, size_file_download);
                        count_file++;
                    }
                }
                // ================== Display console percent of file downloading ================
            }
            // =============================================== Minh tri ====================================================================
            else
            {
                cout << "\n Unable to connect to the server";
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
