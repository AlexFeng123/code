#include<iostream>
using namespace std;

int main()
{
    string lstr;
    while(true)
    {
        getline(cin, lstr);
        if(lstr.empty())
        {
            cout << 0 <<endl;
            break;
        }
        size_t llen = lstr.size();
        int  times = 0;
        for(size_t i  = 0, j = llen - 1;  i < j;)
        {
            while(i < llen && lstr[i] != '1') ++i;
            while(j > 0 && lstr[j] != '0') j--;
            if( i < llen && j > 0)
                 times++;
            else 
                break;
            swap(lstr[i],lstr[j]);
            i++;
            j--;
        }
        cout << times << endl;
    }
    return 0;
}
