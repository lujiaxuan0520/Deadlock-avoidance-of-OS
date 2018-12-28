#include<stdio.h>
#include<iostream>
using namespace std;
#define P 100
#define M 3 //资源数
#define N 5 //进程数

int Available[P];
int Max[P][P];
int Allocation[P][P];
int Need[P][P];

bool tryAllocation(int process_id , int *r);
bool checkSecurity();
void printResources();

int main()
{
    cout<<"INPUT VECTOR OF AVAILABLE (ex.3 3 2)"<<endl;
    for(int i=0;i<M;i++)
        cin>>Available[i];
    cout<<"INPUT MAXTRIX OF MAX (ex.7 5 3;3 2 2)"<<endl;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)
            cin>>Max[i][j];
    }
    cout<<"INPUT MAXTRIX OF ALLOCATION (ex.0 1 0;2 0 0)"<<endl;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)
        {
            cin>>Allocation[i][j];
            Need[i][j]=Max[i][j]-Allocation[i][j];
        }
    }
    printResources();

    int process_id,choiprintResourcesce;
    int *Request=new int[M];
    while(true)
    {
        cout<<"INPUT VECTOR OF PROCESS ID AND REQUEST(-1 FOR EXIT)(ex.1 1 0 2): "<<endl;
        cin>>process_id;
        for(int i=0;i<M;i++)
            cin>>Request[i];
        if(process_id>=0)
        {
          if(tryAllocation(process_id , Request))
          {

              if(checkSecurity())
              {
                  cout<<"RESOURCE ALLOCATION SUCCESS!"<<endl;
                  //分配完成时进行资源的回收
                  for(int j=0;j<M;j++)
                  {
                      if(!Need[process_id][j])
                        Available[j]-=Need[process_id][j];
                  }
                  printResources();
              }
              else
              {
                  cout<<"RESOURCE ALLOCATION UNSAFE!"<<endl;
                  //进行回滚
                  for(int i=0;i<M;i++)
                  {
                      Available[i]+=Request[i];
                      Allocation[process_id][i]-=Request[i];
                      Need[process_id][i]+=Request[i];
                  }
              }
          }
          else
          {
              cout<<"RESOURCE REQUEST FAILURE!"<<endl;
              continue;
          }
        }
        else
        {
            break;
        }
    }
    cout<<"END";
    return 0 ;
}

bool tryAllocation(int process_id , int *r)
{
    for(int i=0;i<M;i++)
    {
        if(r[i]>Need[process_id][i] || r[i]>Available[i])
            return false;
    }
    //试探性分配资源
    for(int i=0;i<M;i++)
    {
        Available[i]-=r[i];
        Allocation[process_id][i]+=r[i];
        Need[process_id][i]-=r[i];
    }
    return true ;
}

 bool checkSecurity()
 {
    bool *finish=new bool[N];//标志进程是否已经完成
    for(int i=0;i<N;i++)
        finish[i]=false;//初始化为false
    int *work=new int[M];
    for(int i=0;i<M;i++)
        work[i]=Available[i];//将work初始化为Available
    while(true)
    {
        int pro_id,i; //记录满足条件的进程
        //寻找满足分配条件的进程
        for(i=0;i<N;i++)
        {
          if(!finish[i])
          {
            int ok=1;
            for(int j=0;j<M;j++)
            {
                if(Need[i][j]>work[j])//不满足分配条件，break
                {
                    ok=0;
                    break;
                }
            }
            if(ok==1)
            {
                pro_id=i;
                break;
            }
            else
                continue;
          }
        }
        if(i == N)
        {
            for(int j = 0 ; j < i ; j++)
            {
                if(!finish[j])
                {
                    return false ;
                }
            }
            return true ;
        }
        //释放资源
        for(int j=0;j<M;j++)
        {
            work[j]+=Allocation[i][j];
        }
        finish[pro_id]=true;//标志该进程结束
    }
    return false;
 }

void printResources()
{
    cout<<"----------------------------------------------"<<endl;
    cout<<"\tMAX\t\tALLOCATION\tNeed"<<endl;
    for(int i=0;i<N;i++)
    {
        cout<<"\t";
        for(int j=0;j<M;j++)
        {
            cout<<Max[i][j]<<" ";
        }
        cout<<"\t\t";
        for(int j=0;j<M;j++)
        {
            cout<<Allocation[i][j]<<" ";
        }
        cout<<"\t\t";
        for(int j=0;j<M;j++)
        {
            cout<<Need[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"\tAVAILABLE RESOURCES: ";
    for(int i=0;i<M;i++)
        cout<<Available[i]<<" ";
    cout<<endl;
    cout<<"----------------------------------------------"<<endl;
}
