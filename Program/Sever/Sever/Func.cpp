#include"Func.h"
#include"pch.h"

void writeFileRecei(string file_recei, char * buffer_receive)
{
   
    ofstream out;
    stringstream ss(buffer_receive);
   
    out.open(file_recei, ios::trunc);//Overwrite
    if (!out.is_open())
    {
        cout << "Write file error";
    }
    else
    {
        string temp_1;
        while (ss >> temp_1)
        {
            out << temp_1 << "\n";
        }
        out.close();
    }


}
unsigned long long readSizeFile(string file_name)
{
    ifstream in;
    in.open(file_name, ios::binary);
    unsigned long long size = 0;
    if (!in.is_open())return size;
    else
    {
        in.seekg(ios::end);
        size = in.tellg();

        in.close();

    }
    return size;

}