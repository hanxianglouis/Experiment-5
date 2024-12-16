#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<queue>

using namespace std;

struct arc{
    int index;
    int weight;
    int cost;
    arc* next;

    arc(int index,int weight,int cost):index(index),weight(weight),cost(cost),next(nullptr){}
};

class patent{
    public:
        string id;
        string company;
        string date;
        arc* first;

        patent(string id,string company,string date):id(id),company(company),date(date),first(nullptr){}

        void add_cited(string cited_id,int weight,int cost,vector<patent*> patents){
            for(int i=0;i<patents.size();i++){
                if(patents[i]->id==cited_id){
                    arc* new_arc=new arc(i,weight,cost);
                    new_arc->next=first;
                    first=new_arc;
                    return;
                }
            }
            cout<<"No such patent!"<<endl;
        }
};

struct path_node {
    patent* aim;
    int cost;
    string path;

    path_node(patent* p) : aim(p), cost(10000), path("") {}//默认初始均为10000
    path_node(patent* p,int cost) : aim(p), cost(cost), path("") {}
    path_node():aim(nullptr),cost(10000),path(""){}
};

class graph{
    public:
        vector<patent*> patents;

        graph(){
            fstream file1;
            file1.open("patent.csv",ios::in);
            string line;
            getline(file1,line);
            while(!file1.eof() && getline(file1,line)){
                stringstream ss(line);
                string id,company,date;
                getline(ss,id,',');
                getline(ss,company,',');
                getline(ss,date,',');
                patent* new_patent=new patent(id,company,date);
                patents.push_back(new_patent);
            }
            file1.close();
            fstream file2;
            file2.open("citation.csv",ios::in);
            getline(file2,line);
            while(!file2.eof() && getline(file2,line)){
                stringstream ss(line);
                string id,cited_id;
                string temp;
                int weight,cost;
                getline(ss,id,',');
                getline(ss,cited_id,',');
                temp.clear();
                getline(ss,temp,',');
                weight=stoi(temp);
                temp.clear();
                getline(ss,temp,',');
                cost=stoi(temp);
                for(int i=0;i<patents.size();i++){
                    if(patents[i]->id==id){
                        patents[i]->add_cited(cited_id,weight,cost,patents);
                        break;
                    }
                }
            }
            file2.close();
        }

        void DFS(patent* node,map<string,bool> &visited){
            cout<<"Patent id:"<<node->id<<" Company:"<<node->company<<" Filing date:"<<node->date<<endl;
            visited[node->id] = true;
            arc* nownode = node->first;
            while(nownode!=nullptr){
                if(visited[patents[nownode->index]->id]==false){
                    DFS(patents[nownode->index],visited);
                }
                nownode=nownode->next;
            }
            return;
        }

        void BFS(patent* start, map<string,bool> &visited){
            queue<patent*> q;
            q.push(start);
            visited[start->id]=true;
            while(!q.empty()){
                patent* nowpatent;
                nowpatent=q.front();
                q.pop();
                cout<<"Patent id:"<<nowpatent->id<<" Company:"<<nowpatent->company<<" Filing date:"<<nowpatent->date<<endl;
                arc* nownode=nowpatent->first;
                while(nownode!=nullptr){
                    if(visited[patents[nownode->index]->id]==false){
                        q.push(patents[nownode->index]);
                        visited[patents[nownode->index]->id]=true;
                    }
                    nownode=nownode->next;
                }
            }
        }

        void min_path(){
            vector<path_node> result;
            vector<path_node> assist;
            for(auto& temp : patents){
                assist.push_back(path_node(temp));
            }
            result.push_back(path_node(patents[0],0));
            patent* newadd=patents[0];
            while(result.size()<8){
                int newdis;//新加顶点的最短路径距离
                string newpath;
                for(int i=0;i<result.size();i++){
                    if(result[i].aim==newadd){
                        newdis=result[i].cost;
                        newpath=result[i].path;
                    }
                }
                arc* nowarc=newadd->first;
                while(nowarc!=nullptr){
                    for(int i=0; i<assist.size();i++){
                        if(assist[i].aim==patents[nowarc->index] && (newdis+nowarc->cost)<assist[i].cost){
                            assist[i].cost=newdis+nowarc->cost;
                            assist[i].path=newpath+"->"+newadd->id;//记录路径
                        }
                    }
                    nowarc=nowarc->next;
                }
                for(int i=0;i<result.size();i++){
                    for(int j=0;j<assist.size();j++){
                        if(assist[j].aim==result[i].aim){
                            assist[j].cost=10000;//如果已经找了，就设回10000，否则前一次找到的那个永远是最短的，就会重复找这个
                        }
                    }
                }
                int min_path=10000;
                path_node min_node;
                for(int i=0;i<assist.size();i++){
                    if(assist[i].cost<min_path){
                        min_node=assist[i];
                        min_path=assist[i].cost;
                    }
                }
                result.push_back(min_node);
                newadd=min_node.aim;
            }
            for(int i=0;i<result.size();i++){
                result[i].path=result[i].path+"->"+result[i].aim->id;
                cout<<"The path from P1 to "<<result[i].aim->id<<" is: "<<result[i].path<<"  The cost is:"<<result[i].cost<<endl;
            }
        }

        void topological_sorting(){
            map<patent*,int> indegree;
            for (int i=0;i<patents.size();i++){
                arc* nowarc=patents[i]->first;
                while(nowarc!=nullptr){
                    indegree[patents[nowarc->index]]++;
                    nowarc=nowarc->next;
                }
            }
            int judge=0;
            while(judge==0){
                int i;
                for(i=0;i<patents.size();i++){
                    if(indegree[patents[i]]==0){
                        break;
                    }
                }
                if(i==patents.size()){
                    judge=1;
                }
                else{
                    cout<<"Patent id:"<<patents[i]->id<<" Company:"<<patents[i]->company<<" Filing date:"<<patents[i]->date<<endl;
                    indegree[patents[i]]=-1;
                    arc* nowarc=patents[i]->first;
                    while(nowarc!=nullptr){
                        indegree[patents[nowarc->index]]--;
                        nowarc=nowarc->next;
                    }
                }
            }
            for(int i=0;i<patents.size();i++){
                if(indegree[patents[i]]>0){
                    cout<<"This is not a AOV! There is a loop!"<<endl;
                    break;
                }
            }
        }
};

struct treenode{//观察到原图最大出度为2，所以这里就用二叉树，只保留两个指针
    patent* data;
    treenode* left;
    treenode* right;

    treenode(patent* data):data(data),left(nullptr),right(nullptr){}
};

struct lowcost_node{
    patent* from;
    int weight;

    lowcost_node(patent* from,int weight):from(from),weight(weight){}
    lowcost_node():from(nullptr),weight(10000){}
};

class MST{//基于Prim算法
    public:
        treenode* root;
        int weight;

        MST(graph g):root(nullptr),weight(0){
            map<patent*,lowcost_node> lowcost;
            for (int i=0;i<g.patents.size();i++){
                lowcost[g.patents[i]]=lowcost_node(nullptr,10000);//10000视作正无穷
            }
            vector<treenode*> added;//先放顺序表里面，方便构建关系指针时找到指针起点
            vector<patent*> added_for_low;//为了保证每次找lowcost最小的都是没加入的点，要在比较前把已经加入的点的weight全部赋值10000
            added.push_back(new treenode(g.patents[0]));
            added_for_low.push_back(g.patents[0]);
            root=added[0];
            patent* newnode=g.patents[0];
            while(added.size()<8){
                arc* nowarc=newnode->first;
                while(nowarc!=nullptr){
                    if(lowcost[g.patents[nowarc->index]].weight > nowarc->weight){
                        lowcost[g.patents[nowarc->index]].weight=nowarc->weight;
                        lowcost[g.patents[nowarc->index]].from=newnode;
                    }
                    nowarc=nowarc->next;
                }
                for(int i=0;i<added_for_low.size();i++){
                    lowcost[added_for_low[i]].weight=10000;
                }
                int min_weight=10000;
                patent* entry;
                patent* from;
                for(auto& node : lowcost){
                    if(node.second.weight<min_weight){
                        min_weight=node.second.weight;
                        entry=node.first;
                        from=node.second.from;
                    }
                }
                newnode=entry;
                treenode* newtreenode=new treenode(entry);
                added.push_back(newtreenode);
                added_for_low.push_back(entry);
                weight+=min_weight;
                for(auto& temp : added){
                    if(temp->data == from){
                        if(temp->left==nullptr){//左边空
                            temp->left=newtreenode;
                        }
                        else{
                            temp->right=newtreenode;
                        }
                    }
                }
            }
        }

        void mid(treenode* root){
            if(root==nullptr){
                return;
            }
            else{
                cout<<"Patent id:"<<root->data->id<<" Company:"<<root->data->company<<" Filing date:"<<root->data->date<<endl;
                mid(root->left);
                mid(root->right);
                return;
            }
        }

        void show(){
            cout<<"The mid-traversal of the tree:"<<endl;
            mid(root);
            cout<<"The total weight of the tree is:"<<weight<<endl;
        }
};

int main(){
    graph g;
    map<string, bool> visited;
    MST mst(g);
    int judge = 0; // 判断是否退出
    while (judge == 0)
    {
        int choice;
        cout<<"1.Show the result of DFS."<<endl;
        cout<<"2.show the result of BFS."<<endl;
        cout<<"3.Show the Minimum Spanning Tree."<<endl;
        cout<<"4.Show the minimum path from P1."<<endl;
        cout<<"5.Show the topologocal sorting."<<endl;
        cout<<"0.exit"<<endl;
        cout<<"----------------------"<<endl;
        cout<<"Please choose a function:";
        cin>>choice;
        switch (choice)
        {
        case 1:{
            for(int i=0;i<g.patents.size();i++){
                visited[g.patents[i]->id]=false;
            }
            g.DFS(g.patents[0],visited);
            getchar();
            getchar();
            system("clear");
            break;
        }
        case 2:
            for(int i=0;i<g.patents.size();i++){
                visited[g.patents[i]->id]=false;
            }
            g.BFS(g.patents[0],visited);
            getchar();
            getchar();
            system("clear");
            break;
        case 3:
            mst.show();
            getchar();
            getchar();
            system("clear");
            break;
        case 4:
            g.min_path();
            getchar();
            getchar();
            system("clear");
            break;
        case 5:
            g.topological_sorting();
            getchar();
            getchar();
            system("clear");
            break;
        case 0:
            judge = 1;
            break;
        default:
            cout<<"Invalid input! Please choose again!"<<endl;
        }
    }
    return 0;
}